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
                                                                
  Copyright 2006, 2007, 2008, 2009, 2010 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  Udp4ENTSTestCase.c

Abstract:

--*/

#include "SctLib.h"
#include "Udp4ENTSTestCase.h"

//
// Udp4.GetModeData
//
static EFI_STATUS                       *GetModeData_Status;
static EFI_UDP4_CONFIG_DATA             *GetModeData_Udp4ConfigData;
static EFI_IP4_MODE_DATA                *GetModeData_Ip4ModeData;
static EFI_MANAGED_NETWORK_CONFIG_DATA  *GetModeData_MnpConfigData;
static EFI_SIMPLE_NETWORK_MODE          *GetModeData_SnpModeData;

ENTS_ARG_FIELD                          gUdp4GetModeDataArgField[] = {
  {
    POINTER,
    &GetModeData_Udp4ConfigData
  },
  {
    POINTER,
    &GetModeData_Ip4ModeData
  },
  {
    POINTER,
    &GetModeData_MnpConfigData
  },
  {
    POINTER,
    &GetModeData_SnpModeData
  },
  {
    POINTER,
    &GetModeData_Status
  },
  0
};

EFI_STATUS
Udp4GetModeData_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 GetModeData Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *GetModeData_Status = Udp4->GetModeData (
                                Udp4,
                                GetModeData_Udp4ConfigData,
                                GetModeData_Ip4ModeData,
                                GetModeData_MnpConfigData,
                                GetModeData_SnpModeData
                                );
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *GetModeData_Status
    );
  return EFI_SUCCESS;
}

//
// Udp4.Configure
//
static EFI_STATUS               *Configure_Status;
static IN EFI_UDP4_CONFIG_DATA  *Configure_UdpConfigData;

ENTS_ARG_FIELD                  gUdp4ConfigureArgField[] = {
  {
    POINTER,
    &Configure_UdpConfigData
  },
  {
    POINTER,
    &Configure_Status
  },
  0
};

EFI_STATUS
Udp4Configure_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 Configure Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *Configure_Status = Udp4->Configure (
                              Udp4,
                              Configure_UdpConfigData
                              );
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *Configure_Status
    );
  return EFI_SUCCESS;
}

//
// Udp4.Groups
//
static EFI_STATUS       *Groups_Status;
static BOOLEAN          Groups_JoinFlag;
static EFI_IPv4_ADDRESS *Groups_MulticastAddress;

ENTS_ARG_FIELD          gUdp4GroupsArgField[] = {
  {
    BOOL,
    &Groups_JoinFlag
  },
  {
    POINTER,
    &Groups_MulticastAddress
  },
  {
    POINTER,
    &Groups_Status
  },
  0
};

EFI_STATUS
Udp4Groups_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 Groups Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *Groups_Status = Udp4->Groups (
                          Udp4,
                          Groups_JoinFlag,
                          Groups_MulticastAddress
                          );
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *Groups_Status
    );
  return EFI_SUCCESS;
}

//
// Udp4.Routes
//
static EFI_STATUS       *Routes_Status;
static BOOLEAN          Routes_DeleteRoute;
static EFI_IPv4_ADDRESS *Routes_SubnetAddress;
static EFI_IPv4_ADDRESS *Routes_SubnetMask;
static EFI_IPv4_ADDRESS *Routes_GatewayAddress;

ENTS_ARG_FIELD          gUdp4RoutesArgField[] = {
  {
    BOOL,
    &Routes_DeleteRoute
  },
  {
    POINTER,
    &Routes_SubnetAddress
  },
  {
    POINTER,
    &Routes_SubnetMask
  },
  {
    POINTER,
    &Routes_GatewayAddress
  },
  {
    POINTER,
    &Routes_Status
  },
  0
};

EFI_STATUS
Udp4Routes_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 Routes Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *Routes_Status = Udp4->Routes (
                          Udp4,
                          Routes_DeleteRoute,
                          Routes_SubnetAddress,
                          Routes_SubnetMask,
                          Routes_GatewayAddress
                          );
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *Routes_Status
    );
  return EFI_SUCCESS;
}

//
// Udp4.Poll
//
static EFI_STATUS *Poll_Status;

ENTS_ARG_FIELD    gUdp4PollArgField[] = {
  {
    POINTER,
    &Poll_Status
  },
  0
};

EFI_STATUS
Udp4Poll_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 Poll Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4          = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *Poll_Status  = Udp4->Poll (Udp4);
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *Poll_Status
    );
  return EFI_SUCCESS;
}

//
// Udp4.Receive
//
static EFI_STATUS                 *Receive_Status;
static EFI_UDP4_COMPLETION_TOKEN  *Receive_Token;

ENTS_ARG_FIELD                    gUdp4ReceiveArgField[] = {
  {
    POINTER,
    &Receive_Token
  },
  {
    POINTER,
    &Receive_Status
  },
  0
};

EFI_STATUS
Udp4Receive_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 Receive Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *Receive_Status = Udp4->Receive (
                            Udp4,
                            Receive_Token
                            );
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *Receive_Status
    );
  return EFI_SUCCESS;
}

//
// Udp4.Transmit
//
static EFI_STATUS                 *Transmit_Status;
static EFI_UDP4_COMPLETION_TOKEN  *Transmit_CompletionToken;

ENTS_ARG_FIELD                    gUdp4TransmitArgField[] = {
  {
    POINTER,
    &Transmit_CompletionToken
  },
  {
    POINTER,
    &Transmit_Status
  },
  0
};

EFI_STATUS
Udp4Transmit_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 Transmit Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *Transmit_Status = Udp4->Transmit (
                            Udp4,
                            Transmit_CompletionToken
                            );
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *Transmit_Status
    );
  return EFI_SUCCESS;
}

//
// Udp4.Cancel
//
static EFI_STATUS                 *Cancel_Status;
static EFI_UDP4_COMPLETION_TOKEN  *Cancel_Token;

ENTS_ARG_FIELD                    gUdp4CancelArgField[] = {
  {
    POINTER,
    &Cancel_Token
  },
  {
    POINTER,
    &Cancel_Status
  },
  0
};

EFI_STATUS
Udp4Cancel_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 Cancel Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *Cancel_Status = Udp4->Cancel (
                          Udp4,
                          Cancel_Token
                          );
  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *Cancel_Status
    );
  return EFI_SUCCESS;
}

//
// SignalRecycleEvent
//
static EFI_STATUS                 *SignalRecycleEvent_Status;
static EFI_UDP4_COMPLETION_TOKEN  *Completion_Token;

ENTS_ARG_FIELD                    gUdp4SignalRecycleEventArgField[] = {
  {
    POINTER,
    &Completion_Token
  },
  {
    POINTER,
    &SignalRecycleEvent_Status
  },
  0
};

EFI_STATUS
Udp4SignalRecycleEvent_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Udp4 SignalRecycleEvent Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_UDP4_PROTOCOL *Udp4;

  Udp4 = (EFI_UDP4_PROTOCOL *) ClientInterface;

  *SignalRecycleEvent_Status = EFI_SUCCESS;
  if (Completion_Token->Status == EFI_SUCCESS) {
    *SignalRecycleEvent_Status = tBS->SignalEvent (Completion_Token->Packet.RxData->RecycleSignal);
  }

  RecordMessage (
    &gUdp4EntsRuntimeInfo,
    &gUdp4EntsRuntimeInfoSize,
    L"Status - %r",
    *SignalRecycleEvent_Status
    );
  return EFI_SUCCESS;
}

