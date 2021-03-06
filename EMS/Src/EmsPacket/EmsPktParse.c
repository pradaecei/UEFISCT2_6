/*++
  The material contained herein is not a license, either        
  expressly or impliedly, to any intellectual property owned    
  or controlled by any of the authors or developers of this     
  material or to any contribution thereto. The material         
  contained herein is provided on an "AS IS" basis and, to the  
  maximum extent permitted by applicable law, this information  
  is provided AS IS AND WITH ALL FAULTS, and the authors and    
  developers of this material hereby disclaim all other         
  warranties and conditions, either express, implied or         
  statutory, including, but not limited to, any (if any)        
  implied warranties, duties or conditions of merchantability,  
  of fitness for a particular purpose, of accuracy or           
  completeness of responses, of results, of workmanlike         
  effort, of lack of viruses and of lack of negligence, all     
  with regard to this material and any contribution thereto.    
  Designers must not rely on the absence or characteristics of  
  any features or instructions marked "reserved" or             
  "undefined." The Unified EFI Forum, Inc. reserves any         
  features or instructions so marked for future definition and  
  shall have no responsibility whatsoever for conflicts or      
  incompatibilities arising from future changes to them. ALSO,  
  THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,  
  QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR            
  NON-INFRINGEMENT WITH REGARD TO THE TEST SUITE AND ANY        
  CONTRIBUTION THERETO.                                         
                                                                
  IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THIS MATERIAL OR  
  ANY CONTRIBUTION THERETO BE LIABLE TO ANY OTHER PARTY FOR     
  THE COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST      
  PROFITS, LOSS OF USE, LOSS OF DATA, OR ANY INCIDENTAL,        
  CONSEQUENTIAL, DIRECT, INDIRECT, OR SPECIAL DAMAGES WHETHER   
  UNDER CONTRACT, TORT, WARRANTY, OR OTHERWISE, ARISING IN ANY  
  WAY OUT OF THIS OR ANY OTHER AGREEMENT RELATING TO THIS       
  DOCUMENT, WHETHER OR NOT SUCH PARTY HAD ADVANCE NOTICE OF     
  THE POSSIBILITY OF SUCH DAMAGES.                              
                                                                
  Copyright 2006, 2007, 2008, 2009, 2010, 2011 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:
  
    EmsPktParse.c
    
Abstract:

    Implementation for packet parse TCL commands

--*/

#include "EmsPktValidate.h"
#include "EmsTypes.h"
#include "EmsProtocols.h"
#include "EmsPktPattern.h"
#include "EmsPktMain.h"
#include "EmsUtilityString.h"
#include "EmsLogUtility.h"
#include "EmsLogCommand.h"

STATIC Tcl_CmdProc  TclParsePacket;

VOID_P
ParseTclInit (
  IN  Tcl_Interp *Interp
  )
/*++

Routine Description:

  Packet parse related TCL command initialization routine

Arguments:

  Interp      - TCL intepreter.

Returns:

  None.

--*/
{
  //
  // Create Packet validation related TCL commands
  //
  Tcl_CreateCommand (
    Interp,
    "ParsePacket",
    TclParsePacket,
    (ClientData) NULL,
    (Tcl_CmdDeleteProc *) NULL
    );
  //
  // Link Packet related TCL variables
  //
}

STATIC
INT32
TclParsePacket (
  IN ClientData        clientData,
  IN Tcl_Interp        *Interp,
  IN INT32             Argc,
  IN CONST84 INT8      *Argv[]
  )
/*++

Routine Description:

  TCL command "ParsePacket" implementation routine  
  ParsePacket packetname -t type -fieldname varname [field pairs ...]

Arguments:

  clientData  - Private data, if any.
  Interp      - TCL intepreter.
  Argc        - Argument counter.
  Argv        - Argument value pointer array.

Returns:

  TCL_OK or TCL_ERROR

--*/
{
  INT32             Index;
  INT32             Index2;
  UINT32            Index3;
  UINT8             TempHighHalfByte, TempLowHalfByte;
  INT8              *Name;
  INT8              *Type;
  PROTOCOL_ENTRY_T  *Protocol;
  PACKET_T          *Packet;
  FIELD_T           *Unpack;
  INT8              TempBuf[2000];
  INT8              *TempBufPayload;
  UINT32            IsPayload;
  INT8              *FieldName;
  INT8              *VarName;
  PACKET_T          *Payload;
  UINT32            Count;
  INT8              Buff[5];

  LogCurrentCommand (Argc, Argv);

  if (Argc <= 5) {
    goto ErrorExit;
  };

  //
  // The first argument is packet name
  //
  Name = (INT8 *) Argv[1];
  if (Name[0] == '-') {
    goto ErrorExit;

  }

  if (0 != (strcmp_i ((INT8 *) Argv[2], "-t"))) {
    goto ErrorExit;
  }

  Type    = (INT8 *) Argv[3];

  Packet  = EmsPacketFindByName (Name);
  if (NULL == Packet) {
    RecordMessage (
      EMS_VERBOSE_LEVEL_DEFAULT,
      "EMS: ParsePacket: Cannot find Packet %a. %a:%d",
      Name,
      __FILE__,
      __LINE__
      );
    return TCL_OK;
  }
  //
  // Find protocol entry and unpack the packet
  //
  Protocol = GetProtocolByName (Type);
  if (NULL == Protocol) {
    Tcl_AppendResult (
      Interp,
      "EMS: ParsePacket: The Protocol ",
      Type,
      " is not supported!",
      (INT8 *) NULL
      );
    return TCL_ERROR;
  }

  Unpack = Protocol->UnpackPacket (Packet->Data, Packet->DataLen);
  if (NULL == Unpack) {
    Tcl_AppendResult (
      Interp,
      "Cannot Unpack this Packet using Protocol ",
      Type,
      (INT8 *) NULL
      );
    return TCL_OK;
  }
  //
  // validate this packet
  //
  Count = 0;
  for (Index2 = 4; Index2 + 1 < Argc; Index2++) {
    FieldName = (INT8 *) &Argv[Index2][1];
    Index2++;
    VarName = (INT8 *) Argv[Index2];
    for (Index = 0; Unpack[Index].Name; Index++) {
      if (0 == strcmp_i (FieldName, Unpack[Index].Name)) {
        IsPayload = 0;
        switch (Unpack[Index].Type) {
        case OCTET1:
          sprintf (TempBuf, "0x%x", *(UINT8 *) (Unpack[Index].Value));
          break;

        case OCTET2:
          sprintf (TempBuf, "0x%x", *(UINT16 *) (Unpack[Index].Value));
          break;

        case OCTET4:
          sprintf (TempBuf, "0x%x", *(UINT32 *) (Unpack[Index].Value));
          break;

        case IPADDR:
          Ipv4ToAsciiString (TempBuf, *(UINT32 *) (Unpack[Index].Value));
          break;

        case IPV6ADDR:
          Ipv6ToAsciiString (TempBuf, (EMS_IPV6_ADDR*)Unpack[Index].Value);
          break;

        case MACADDR:
          MacToAsciiString (TempBuf, (UINT8 *) (Unpack[Index].Value));
          break;

        case STRING:
          sprintf (TempBuf, *(INT8 **) (Unpack[Index].Value));
          break;

        case PAYLOAD:
          Payload = EmsPacketFindByName (VarName);
          if (Payload) {
            EmsPacketRemove (Payload);
            EmsPacketDestroy (Payload);
          }

          Payload = EmsPacketCreate (
                      VarName,
                      ((PAYLOAD_T *) (Unpack[Index].Value))->Payload,
                      ((PAYLOAD_T *) (Unpack[Index].Value))->Len,
                      &(Packet->Time)
                      );
          EmsPacketAdd (Payload);

          IsPayload = 1;
          TempBufPayload = malloc(Payload -> DataLen * 5);

          for(Index3=0; Index3<(Payload->DataLen);Index3++) {
              TempHighHalfByte = ( Payload->Data[Index3] & 0xf0 ) >> 4;
              TempLowHalfByte = ( Payload->Data[Index3] & 0x0f );
              TempBufPayload[Index3*5]='0';
              TempBufPayload[Index3*5+1]='x';
              if ( TempHighHalfByte < 10 )
                  TempBufPayload[Index3*5+2] = TempHighHalfByte + '0';
              else
                  TempBufPayload[Index3*5+2] = TempHighHalfByte - 10 + 'A';

              if ( TempLowHalfByte < 10 )
                  TempBufPayload[Index3*5+3]=TempLowHalfByte + '0';
              else
                  TempBufPayload[Index3*5+3]=TempLowHalfByte - 10 + 'A';

              TempBufPayload[Index3*5+4]=' ';
          }
          TempBufPayload[Index3*5-1]=0;  //Skip the last Blank, overwrite it.
          break;

        default:
          sprintf (TempBuf, "Wrong Type for %s", FieldName);
          break;
        }

        if(IsPayload == 1) {
          Tcl_SetVar (Interp, VarName, TempBufPayload, TCL_GLOBAL_ONLY);
          free(TempBufPayload);
        } else {
          Tcl_SetVar (Interp, VarName, TempBuf, TCL_GLOBAL_ONLY);
        }
        Count++;
        break;
      }
    }

    if (NULL == Unpack[Index].Name) {
      Tcl_AppendResult (
        Interp,
        "Wrong Field Name:",
        FieldName,
        (INT8 *) NULL
        );
      return TCL_ERROR;
    }
  }

  sprintf (Buff, "%d", Count);

  Tcl_AppendResult (Interp, Buff, (INT8 *) NULL);
  return TCL_OK;
ErrorExit:
  return TCL_ERROR;
}
