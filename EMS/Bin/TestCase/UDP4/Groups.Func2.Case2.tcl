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
set reportfile    report.csv

#
# test case Name, category, description, GUID...
#
CaseGuid        63E613C7-CDED-404c-A6B6-F93F08ED654A
CaseName        Groups.Func2.Case2
CaseCategory    Udp4
CaseDescription {Test the Groups Function of UDP4 - Invoke Groups() to leave all\
                 multicast groups with MulticastAddress being NULL.}
################################################################################

Include UDP4/include/Udp4.inc.tcl

#
# Begin log ...
#
BeginLog

#
# Begin scope
#
BeginScope _UDP4_GROUPS_FUNCTION2_CASE2_

set hostmac           [GetHostMac]
set targetmac         [GetTargetMac]             
set targetip          192.168.88.1
set hostip            192.168.88.2
set subnetmask        255.255.255.0
set targetport        4000
set hostport          4000
set groupaddress      224.0.0.55
set groupmacaddress   01:00:5E:0:0:37
set groupaddress1     224.0.0.56
set groupmacaddress1  01:00:5E:0:0:38

#
# Parameter Definition
# R_ represents "Remote EFI Side Parameter"
# L_ represents "Local OS Side Parameter"
#
UINTN                            R_Status
UINTN                            R_Handle
UINTN                            R_PollStatus
EFI_UDP4_CONFIG_DATA             R_Udp4ConfigData
UINTN                            R_Context
EFI_UDP4_COMPLETION_TOKEN        R_Token
UINTN                            R_Context1
EFI_UDP4_COMPLETION_TOKEN        R_Token1
EFI_IP_ADDRESS                   R_MulticastAddress
EFI_IP_ADDRESS                   R_MulticastAddress1

SetIpv4Address R_MulticastAddress.v4  $groupaddress
SetIpv4Address R_MulticastAddress1.v4 $groupaddress1

Udp4ServiceBinding->CreateChild "&@R_Handle, &@R_Status"
GetAck
SetVar   [subst $ENTS_CUR_CHILD]  @R_Handle
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4SBP.Groups - Func - Create Child"                         \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

SetVar R_Udp4ConfigData.AcceptBroadcast             TRUE
SetVar R_Udp4ConfigData.AcceptPromiscuous           FALSE
SetVar R_Udp4ConfigData.AcceptAnyPort               TRUE
SetVar R_Udp4ConfigData.AllowDuplicatePort          TRUE
SetVar R_Udp4ConfigData.TypeOfService               0
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
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Groups - Func - Config Child"                            \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

#
# Joins the two multicast group
#
Udp4->Groups {TRUE, &@R_MulticastAddress, &@R_Status}
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Groups - Func - Join group $groupaddress"                \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

Udp4->Groups {TRUE, &@R_MulticastAddress1, &@R_Status}
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Groups - Func - Join group $groupaddress1"               \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

# Leave all groups
Udp4->Groups {FALSE, NULL, &@R_Status}
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $Udp4GroupsFunc2AssertionGuid002                       \
                "Udp4.Groups - Func - Leave all groups"                        \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

#
# Build a two UDP packets to 2 groups
#
LocalEther  $hostmac
LocalIp     $hostip
CreatePayload aaa const 18 0x0

RemoteEther $groupmacaddress
RemoteIp    $groupaddress
CreatePacket udp_packet -t udp -udp_dp $targetport -udp_sp $hostport           \
                        -udp_payload aaa

RemoteEther $groupmacaddress1
RemoteIp    $groupaddress1
CreatePacket udp_packet1 -t udp -udp_dp $targetport -udp_sp $hostport          \
                         -udp_payload aaa

#
# EFI begins to receive first packet
#
SetVar R_Context  0
BS->CreateEvent "$EVT_NOTIFY_SIGNAL, $EFI_TPL_CALLBACK, 1, &@R_Context,        \
                 &@R_Token.Event, &@R_Status"
GetAck
Udp4->Receive {&@R_Token, &@R_Status}

#
# Sends the Packet
#
SendPacket udp_packet
Stall      1

GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Groups - Func - Receive a packet"                        \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

GetVar R_Context
if {$R_Context == 0} {
  set assert pass
} else {
  set assert fail
}
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Groups - Func - check Event"                             \
                "Return R_Context - $R_Context, Expected R_Context - 0 "

Udp4->Cancel {&@R_Token, &@R_Status}
GetAck

#
# EFI begins to receive Second packet
#
SetVar R_Context1  0
BS->CreateEvent "$EVT_NOTIFY_SIGNAL, $EFI_TPL_CALLBACK, 1, &@R_Context1,        \
                 &@R_Token1.Event, &@R_Status"
GetAck
Udp4->Receive {&@R_Token1, &@R_Status}
#
# Sends the Packet
#
SendPacket udp_packet1
Stall      1

GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Groups - Func - Receive the second packet"               \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

GetVar R_Context1
if {$R_Context1 == 0} {
  set assert pass
} else {
  set assert fail
}
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4.Groups - Func - check Event"                             \
                "Return R_Context - $R_Context1, Expected R_Context - 0"

Udp4->Cancel {&@R_Token, &@R_Status}
GetAck

#
# Destroy the child handle
#
Udp4ServiceBinding->DestroyChild {@R_Handle, &@R_Status}
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid                                  \
                "Udp4SBP.Groups - Func - Destroy Child"                        \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

#
# Close the EVENT.
#
BS->CloseEvent "@R_Token.Event, &@R_Status"
GetAck
BS->CloseEvent "@R_Token1.Event, &@R_Status"
GetAck

#
# End scope
#
EndScope _UDP4_GROUPS_FUNCTION2_CASE2_

#
# End Log
#
EndLog


