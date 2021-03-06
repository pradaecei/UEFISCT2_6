#
# The material contained herein is not a license, either      
# expressly or impliedly, to any intellectual property owned  
# or controlled by any of the authors or developers of this   
# material or to any contribution thereto. The material       
# contained herein is provided on an "AS IS" basis and, to the
# maximum extent permitted by applicable law, this information
# is provided AS IS AND WITH ALL FAULTS, and the authors and  
# developers of this material hereby disclaim all other       
# warranties and conditions, either express, implied or       
# statutory, including, but not limited to, any (if any)      
# implied warranties, duties or conditions of merchantability,
# of fitness for a particular purpose, of accuracy or         
# completeness of responses, of results, of workmanlike       
# effort, of lack of viruses and of lack of negligence, all   
# with regard to this material and any contribution thereto.  
# Designers must not rely on the absence or characteristics of
# any features or instructions marked "reserved" or           
# "undefined." The Unified EFI Forum, Inc. reserves any       
# features or instructions so marked for future definition and
# shall have no responsibility whatsoever for conflicts or    
# incompatibilities arising from future changes to them. ALSO,
# THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,
# QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR          
# NON-INFRINGEMENT WITH REGARD TO THE TEST SUITE AND ANY      
# CONTRIBUTION THERETO.                                       
#                                                             
# IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THIS MATERIAL OR
# ANY CONTRIBUTION THERETO BE LIABLE TO ANY OTHER PARTY FOR   
# THE COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST    
# PROFITS, LOSS OF USE, LOSS OF DATA, OR ANY INCIDENTAL,      
# CONSEQUENTIAL, DIRECT, INDIRECT, OR SPECIAL DAMAGES WHETHER 
# UNDER CONTRACT, TORT, WARRANTY, OR OTHERWISE, ARISING IN ANY
# WAY OUT OF THIS OR ANY OTHER AGREEMENT RELATING TO THIS     
# DOCUMENT, WHETHER OR NOT SUCH PARTY HAD ADVANCE NOTICE OF   
# THE POSSIBILITY OF SUCH DAMAGES.                            
#                                                             
# Copyright 2006, 2007, 2008, 2009, 2010 Unified EFI, Inc. All
# Rights Reserved, subject to all existing rights in all      
# matters included within this Test Suite, to which United    
# EFI, Inc. makes no claim of right.                          
#                                                             
# Copyright (c) 2010, Intel Corporation. All rights reserved.<BR> 
#
#
################################################################################
CaseLevel         FUNCTION
CaseAttribute     AUTO
CaseVerboseLevel  DEFAULT

#
# test case Name, category, description, GUID...
#
CaseGuid          98D401D8-32B2-4a23-D232-839B38D3F231
CaseName          Others.Func1.Case2
CaseCategory      TCP
CaseDescription   {This item is to test [EUT] can correctly handle SYN flood.  \
                   --[EUT] accepts one or more connection request, thus making \
                   MaxSynBacklog NOT full.                                     \
                   --Accept following incoming <SYN> segment when MaxSynBacklog\
                   is NOT full.}
################################################################################

Include TCP4/include/Tcp4.inc.tcl

proc CleanUpEutEnvironment {} {
  global RST SYN_TIMES

  for { set i 1 } { $i <= $SYN_TIMES } { incr i } {
    UpdateTcpSendBuffer TCB$i -c $RST
    SendTcpPacket TCB$i
  }

  UpdateTcpSendBuffer TCB -c $RST
  SendTcpPacket TCB

  DestroyTcb
  DelEntryInArpCache

  Tcp4ServiceBinding->DestroyChild "@R_Tcp4Handle, &@R_Status"
  GetAck

  Tcp4ServiceBinding->DestroyChild "@R_Accept_NewChildHandle, &@R_Status"
  GetAck

  BS->CloseEvent "@R_Accept_CompletionToken.Event, &@R_Status"
  GetAck

  EndLogPacket
  EndScope _TCP4_RFC_COMPATIBILITY_
  EndLog
}

#
# Begin log ...
#
BeginLog

#
# BeginScope on OS.
#
BeginScope _TCP4_RFC_COMPATIBILITY_

BeginLogPacket Others.Func1.Case2      "host $DEF_EUT_IP_ADDR and host         \
                                             $DEF_ENTS_IP_ADDR"

#
# Parameter Definition
# R_ represents "Remote EFI Side Parameter"
# L_ represents "Local OS Side Parameter"
#
set      SYN_TIMES                               50
set      L_ControlOption_MaxSynBackLog           5
set      L_ControlOption_ConnectionTimeout       60
set      L_ACK_NUMBER                            1

UINTN                            R_Status
UINTN                            R_Tcp4Handle
UINTN                            R_Context

EFI_TCP4_ACCESS_POINT            R_Configure_AccessPoint
EFI_TCP4_CONFIG_DATA             R_Configure_Tcp4ConfigData

EFI_TCP4_COMPLETION_TOKEN        R_Accept_CompletionToken
EFI_TCP4_LISTEN_TOKEN            R_Accept_ListenToken
UINTN                            R_Accept_NewChildHandle

EFI_TCP4_OPTION                  R_Configure_ControlOption

#
# Initialization of TCB related on OS side.
#
CreateTcb TCB $DEF_ENTS_IP_ADDR $DEF_ENTS_PRT $DEF_EUT_IP_ADDR $DEF_EUT_PRT

for { set i 1 } { $i <= $SYN_TIMES } { incr i } {
  set temp_port [ expr 400 + $i ]
  CreateTcb TCB$i $DEF_ENTS_IP_ADDR $temp_port $DEF_EUT_IP_ADDR $DEF_EUT_PRT
}

LocalEther  $DEF_ENTS_MAC_ADDR
RemoteEther $DEF_EUT_MAC_ADDR
LocalIp     $DEF_ENTS_IP_ADDR
RemoteIp    $DEF_EUT_IP_ADDR

#
# Add an entry in ARP cache.
#
AddEntryInArpCache

#
# Create Tcp4 Child.
#
Tcp4ServiceBinding->CreateChild "&@R_Tcp4Handle, &@R_Status"
GetAck
SetVar     [subst $ENTS_CUR_CHILD]  @R_Tcp4Handle
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Tcp4SBP.CreateChild - Create Child 1"                         \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

#
# Configure TCP instance.
#
SetVar          R_Configure_AccessPoint.UseDefaultAddress  FALSE
SetIpv4Address  R_Configure_AccessPoint.StationAddress     $DEF_EUT_IP_ADDR
SetIpv4Address  R_Configure_AccessPoint.SubnetMask         $DEF_EUT_MASK
SetVar          R_Configure_AccessPoint.StationPort        $DEF_EUT_PRT
SetIpv4Address  R_Configure_AccessPoint.RemoteAddress      0
SetVar          R_Configure_AccessPoint.RemotePort         0
SetVar          R_Configure_AccessPoint.ActiveFlag         FALSE

SetVar  R_Configure_Tcp4ConfigData.TypeOfService       0
SetVar  R_Configure_Tcp4ConfigData.TimeToLive          128
SetVar  R_Configure_Tcp4ConfigData.AccessPoint         @R_Configure_AccessPoint
SetVar  R_Configure_ControlOption.ConnectionTimeout    $L_ControlOption_ConnectionTimeout
SetVar  R_Configure_ControlOption.MaxSynBackLog        $L_ControlOption_MaxSynBackLog
SetVar  R_Configure_Tcp4ConfigData.ControlOption       &@R_Configure_ControlOption

Tcp4->Configure {&@R_Configure_Tcp4ConfigData, &@R_Status}
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Tcp4.Configure - Configure Child 1."                          \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

#
# Call Tcp4.Accept for an passive TCP instance.
#
BS->CreateEvent "$EVT_NOTIFY_SIGNAL, $EFI_TPL_CALLBACK, 1, &@R_Context,        \
                 &@R_Accept_CompletionToken.Event, &@R_Status"
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid   "BS.CreateEvent."              \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

SetVar R_Accept_ListenToken.CompletionToken @R_Accept_CompletionToken
SetVar R_Accept_ListenToken.CompletionToken.Status $EFI_INCOMPATIBLE_VERSION

Tcp4->Accept {&@R_Accept_ListenToken, &@R_Status}
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Tcp4.Accept - Open an passive connection."                    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

#
# <SYN> flood
#
for { set i 1 } { $i <= $SYN_TIMES } { incr i } {
  UpdateTcpSendBuffer TCB$i -c $SYN
}
for { set i 1 } { $i <= $SYN_TIMES } { incr i } {
  SendTcpPacket TCB$i
}

for { set i 1 } { $i <= $L_ACK_NUMBER } { incr i } {
  ReceiveTcpPacket TCB$i 5

  UpdateTcpSendBuffer TCB$i -c $ACK
  SendTcpPacket TCB$i
}

#
# Handles the three-way handshake.
#
UpdateTcpSendBuffer TCB -c $SYN
SendTcpPacket TCB

#
# OS gets <SYN, ACK> segment
#
ReceiveTcpPacket TCB $L_ControlOption_ConnectionTimeout

if { ${TCB.received} == 1 } {
  if { $L_ACK_NUMBER > 1 } {
    if { ${TCB.r_f_syn} != 1 || ${TCB.r_f_ack} != 1 } {
    set assert fail
    RecordAssertion $assert $Tcp4OthersFunc1AssertionGuid002                   \
                            "EUT should send out <SYN, ACK> segment,           \
                             because MaxSynBacklog is NOT full."
    } else {
      set assert pass
      RecordAssertion $assert $Tcp4OthersFunc1AssertionGuid002                 \
                              "EUT does send out <SYN, ACK> segment."
      #
      # OS sends <ACK> segment
      #
      UpdateTcpSendBuffer TCB -c $ACK
      SendTcpPacket TCB
    }
  }
} else {
  set assert fail
  RecordAssertion $assert $Tcp4OthersFunc1AssertionGuid002                     \
                  "EUT doesn't send out any segment."
}

SetVar R_Accept_NewChildHandle 0
#
# Get the NewChildHandle value.
#
GetVar R_Accept_ListenToken.NewChildHandle
SetVar R_Accept_NewChildHandle ${R_Accept_ListenToken.NewChildHandle}

Stall 2
#
# Clean up the environment on EUT side.
#
CleanUpEutEnvironment
