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
# Copyright 2006, 2007, 2008, 2009, 2010, 2011 Unified EFI, Inc. All
# Rights Reserved, subject to all existing rights in all      
# matters included within this Test Suite, to which United    
# EFI, Inc. makes no claim of right.                          
#                                                             
# Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR> 
#
#
################################################################################
CaseLevel         FUNCTION
CaseAttribute     AUTO
CaseVerboseLevel  DEFAULT 
set reportfile    report.csv

#
# test case Name, category, description, GUID...
#
CaseGuid        8D9C869B-8FCC-4a58-9CAB-6FC591016FB9
CaseName        Configure.Func6.Case1
CaseCategory    Udp4
CaseDescription {Test the Configure Function of UDP4 - Invoke Configure() to   \
                 check if the parameter TypeOfService can effect the sending out\
                 of the packet.}
################################################################################

Include UDP4/include/Udp4.inc.tcl

proc CleanUpEutEnvironment {} {
  DelEntryInArpCache
  Udp4ServiceBinding->DestroyChild {@R_Handle, &@R_Status}
  GetAck

  BS->CloseEvent "@R_Token.Event, &@R_Status"
  GetAck

  EndCapture
  
  VifDown 0
  
  EndScope _UDP4_CONFIGURE_FUNCTION6_CASE1_
  
  EndLog
}

#
# Begin log ...
#
BeginLog

#
# Begin scope
#
BeginScope _UDP4_CONFIGURE_FUNCTION6_CASE1_

set hostmac    [GetHostMac]
set targetmac  [GetTargetMac]
set targetip   192.168.88.1
set hostip     192.168.88.2
set subnetmask 255.255.255.0
set targetport 4000
set hostport   4000

VifUp 0 $hostip

set test_tos   1

#
# Parameter Definition
# R_ represents "Remote EFI Side Parameter"
# L_ represents "Local OS Side Parameter"
#
UINTN                            R_Status
UINTN                            R_Handle
EFI_UDP4_CONFIG_DATA             R_Udp4ConfigData
UINTN                            R_Context
EFI_UDP4_COMPLETION_TOKEN        R_Token
EFI_UDP4_TRANSMIT_DATA           R_TxData
EFI_UDP4_FRAGMENT_DATA           R_FragmentTable
CHAR8                            R_FragmentBuffer(600)

#
# Add an entry in ARP cache.
#
AddEntryInArpCache

Udp4ServiceBinding->CreateChild "&@R_Handle, &@R_Status"
GetAck
SetVar   [subst $ENTS_CUR_CHILD]  @R_Handle
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4SBP.Configure - Func - Create Child"                      \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

SetVar R_Udp4ConfigData.AcceptBroadcast             TRUE
SetVar R_Udp4ConfigData.AcceptPromiscuous           FALSE
SetVar R_Udp4ConfigData.AcceptAnyPort               TRUE
SetVar R_Udp4ConfigData.AllowDuplicatePort          TRUE
SetVar R_Udp4ConfigData.TypeOfService               $test_tos
SetVar R_Udp4ConfigData.TimeToLive                  1
SetVar R_Udp4ConfigData.DoNotFragment               TRUE
SetVar R_Udp4ConfigData.ReceiveTimeout              0
SetVar R_Udp4ConfigData.TransmitTimeout             0
SetVar R_Udp4ConfigData.UseDefaultAddress           FALSE
SetIpv4Address R_Udp4ConfigData.StationAddress      $targetip
SetIpv4Address R_Udp4ConfigData.SubnetMask          $subnetmask
SetVar R_Udp4ConfigData.StationPort                 $targetport
SetIpv4Address R_Udp4ConfigData.RemoteAddress       $hostip
SetVar R_Udp4ConfigData.RemotePort                  $hostport

#
# check point
#
Udp4->Configure {&@R_Udp4ConfigData, &@R_Status}
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $Udp4ConfigureFunc6AssertionGuid001                    \
                "Udp4.Configure - Func - Config Child"                         \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

BS->CreateEvent "$EVT_NOTIFY_SIGNAL, $EFI_TPL_CALLBACK, 1, &@R_Context,        \
                 &@R_Token.Event, &@R_Status"
GetAck

SetVar R_TxData.DataLength      20
SetVar R_TxData.FragmentCount   1
SetVar R_TxData.UdpSessionData  0 ;#NULL
SetVar R_TxData.GatewayAddress  0 ;#NULL

SetVar R_FragmentBuffer               "UdpConfigureTest" 
SetVar R_FragmentTable.FragmentBuffer &@R_FragmentBuffer
SetVar R_FragmentTable.FragmentLength 20
SetVar R_TxData.FragmentTable         @R_FragmentTable

SetVar R_Token.Packet &@R_TxData

set L_Filter "udp port $targetport"
StartCapture CCB $L_Filter

Udp4->Transmit {&@R_Token, &@R_Status}

ReceiveCcbPacket CCB aaa 10

if { ${CCB.received} == 0} {
  #
  # If have not captured the packet. Fail
  #
  GetAck
  GetVar R_Status
  GetVar R_Token.Status
  set assert fail
  RecordAssertion $assert $GenericAssertionGuid                                \
                  "Udp4.Configure - Func - Transmit a packet"                  \
                  "Packet not captured, R_Status - $R_Status, R_Token.Status - \
                  ${R_Token.Status}"

  CleanUpEutEnvironment

  return
}

#
# If have captured the packet.
#
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Configure - Func - Transmit a packet"                    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

#
# Verify R_Token.Status
#
GetVar R_Token.Status
if {${R_Token.Status} == $EFI_SUCCESS} {
  set assert pass
} else {
  set assert fail
}
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Configure - Func - check R_Token.Status"                 \
                "ReturnStatus - ${R_Token.Status}, ExpectedStatus - $EFI_SUCCESS"

#
# Verify the Event
#
GetVar R_Context
if {$R_Context == 1} {
  set assert pass
} else {
  set assert fail
}
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Configure - Func - check Event"                          \
                "Return R_Context - $R_Context, Expected R_Context - 1"

#
# Verify the IP header in the packet
#
ParsePacket aaa -t ip -Ipv4_tos tos

if {$tos == $test_tos } {
  set assert pass
} else {
  set assert fail
}
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Configure - Func - check the packet's IP header"         \
                "Actual IP TOS - $tos,Expected IP TOS - $test_tos"

CleanUpEutEnvironment
