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
# Copyright 2017 Unified EFI, Inc. All
# Rights Reserved, subject to all existing rights in all      
# matters included within this Test Suite, to which United    
# EFI, Inc. makes no claim of right.                          
#                                                             
# Copyright (c) 2017, Intel Corporation. All rights reserved.<BR> 
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
CaseGuid        CAA30FA3-1BB7-4F47-A6A5-FB1BF809B257
CaseName        GetData.Func1.Case1
CaseCategory    IP4Config2
CaseDescription {GetData must succeed with valid parameters.}

################################################################################
Include IP4Config2/Include/Ip4Config2.inc.tcl


#
# Begin log ...
#
BeginLog

#
# BeginScope
#
BeginScope _IP4CONFIG2_GETDATA_FUNC1

#
# Parameter Definition
# R_ represents "Remote EFI Side Parameter"
# L_ represents "Local ENTS Side Parameter"
#
UINTN                            R_Status
UINTN                            R_Context
UINTN                            R_Context1
UINTN                            R_DoneEvent
UINTN                            R_DoneEvent1
UINTN                            R_Ip4Config2DataSize
UINT32                           R_Ip4Config2DataType
UINT32                           R_Ip4Config2Policy

#
# Check Point: Call IP4Config2->GetData to set Static Policy
#
SetVar R_Ip4Config2DataType   $IP4C2DT(Policy)
SetVar R_Ip4Config2DataSize   [Sizeof UINT32]
SetVar R_Ip4Config2Policy     $IP4C2P(Static)

SetVar R_Context 0
BS->CreateEvent "$EVT_NOTIFY_SIGNAL, $EFI_TPL_NOTIFY, 1, &@R_Context,\
                &@R_DoneEvent, &@R_Status"
GetAck
set assert    [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid    \
                "BS.CreateEvent."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

Ip4Config2->RegisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent,&@R_Status"
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid    \
                "Ip4Config2.RegisterDataNotify - Register notification event for configuration."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

Ip4Config2->SetData "@R_Ip4Config2DataType,@R_Ip4Config2DataSize,&@R_Ip4Config2Policy,&@R_Status"
GetAck
GetVar R_Status
if { $R_Status == $EFI_SUCCESS } {
  set assert pass
  RecordAssertion $assert $GenericAssertionGuid    \
                  "Ip4Config2.SetData - Call SetData to set Static policy."    \
                  "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"
} elseif { $R_Status == $EFI_NOT_READY } {
  set i 0
  set L_TimeOut 30
  while { 1 > 0 } {
    GetVar R_Context
    if { $R_Context == 1 } {
      break
    } elseif { $i == $L_TimeOut } {
      set assert fail
      RecordAssertion $assert $GenericAssertionGuid    \
                      "SetData failed.(event hasn't been signaled before TIMEOUT)."    \
                      "TIMEOUT value is $L_TimeOut (sec), "
      SetVar  R_Ip4Config2DataType  $IP4C2DT(Policy)
      Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent,&@R_Status"
      GetAck
      BS->CloseEvent {@R_DoneEvent,&@R_Status}
      GetAck
      EndScope _IP4CONFIG2_GETDATA_FUNC1
      EndLog
      return
    }
    incr i
    Stall 1
  }
} else {
  set assert fail
  RecordAssertion $assert $GenericAssertionGuid    \
                  "Ip4Config2.SetData - Call SetData to set Static Policy."    \
                  "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

  SetVar  R_Ip4Config2DataType  $IP4C2DT(Policy)
  Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent,&@R_Status"
  GetAck
  BS->CloseEvent {@R_DoneEvent,&@R_Status}
  GetAck
  EndScope _IP4CONFIG2_GETDATA_FUNC1
  EndLog
  return
}

Ip4Config2->GetData "@R_Ip4Config2DataType,&@R_Ip4Config2DataSize,&@R_Ip4Config2Policy,&@R_Status"
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $Ip4Config2GetDataFunc1AssertionGuid001    \
                "Ip4Config2.GetData - Call GetData to get Static Policy."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"
GetVar R_Ip4Config2Policy
if { $R_Ip4Config2Policy == $IP4C2P(Static) } {
  set assert pass
} else {
  set assert fail
}
RecordAssertion $assert $Ip4Config2GetDataFunc1AssertionGuid002        \
                "Set Static Policy succeeds(event is signaled correctly and data correct)."

#
# Check Point: Call Ip4Config2->SetData to set ManualAddress
#
SetVar R_Ip4Config2DataType       $IP4C2DT(ManualAddress)
SetVar R_Ip4Config2DataSize       [Sizeof EFI_IP4_CONFIG2_MANUAL_ADDRESS]
EFI_IP4_CONFIG2_MANUAL_ADDRESS    R_Temp_Ip4Config2ManualAddress
EFI_IP4_CONFIG2_MANUAL_ADDRESS    R_Ip4Config2ManualAddress
SetIpv4Address R_Temp_Ip4Config2ManualAddress.Address       "192.168.100.1"
SetIpv4Address R_Temp_Ip4Config2ManualAddress.SubnetMask    "255.255.255.0"
SetVar R_Ip4Config2ManualAddress @R_Temp_Ip4Config2ManualAddress

SetVar R_Context1 0
BS->CreateEvent "$EVT_NOTIFY_SIGNAL, $EFI_TPL_NOTIFY, 1, &@R_Context1,\
                &@R_DoneEvent1, &@R_Status"
GetAck
set assert    [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid    \
                "BS.CreateEvent."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

Ip4Config2->RegisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent1,&@R_Status"
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid     \
                "Ip4Config2.RegisterDataNotify - Register notification event for configuration."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

Ip4Config2->SetData "@R_Ip4Config2DataType,@R_Ip4Config2DataSize,&@R_Ip4Config2ManualAddress,&@R_Status"
GetAck
GetVar R_Status
if { $R_Status == $EFI_SUCCESS } {
  set assert pass
  RecordAssertion $assert $GenericAssertionGuid    \
                  "Ip4Config2.SetData - Call SetData to set ManualAddress."    \
                  "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"
} elseif { $R_Status == $EFI_NOT_READY } {
  set i 0
  set L_TimeOut 30
  while { 1 > 0 } {
    GetVar R_Context1
    if { $R_Context1 == 1 } {
      break
    } elseif { $i == $L_TimeOut } {
      set assert fail
      RecordAssertion $assert $GenericAssertionGuid    \
                      "SetData failed.(event hasn't been signaled before TIMEOUT)."    \
                      "TIMEOUT value is $L_TimeOut (sec), "
      SetVar  R_Ip4Config2DataType  $IP4C2DT(Policy)
      Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent,&@R_Status"
      GetAck
      BS->CloseEvent {@R_DoneEvent,&@R_Status}
      GetAck
      SetVar  R_Ip4Config2DataType  $IP4C2DT(ManualAddress)
      Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent1,&@R_Status"
      GetAck
      BS->CloseEvent {@R_DoneEvent1,&@R_Status}
      GetAck
      EndScope _IP4CONFIG2_GETDATA_FUNC1
      EndLog
      return
    }
    incr i
    Stall 1
  }
} else {
  set assert fail
  RecordAssertion $assert $GenericAssertionGuid    \
                  "Ip4Config2.SetData - Call SetData to set ManualAddress."    \
                  "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

  SetVar  R_Ip4Config2DataType  $IP4C2DT(Policy)
  Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent,&@R_Status"
  GetAck
  BS->CloseEvent {@R_DoneEvent,&@R_Status}
  GetAck
  SetVar  R_Ip4Config2DataType  $IP4C2DT(ManualAddress)
  Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent1,&@R_Status" 
  GetAck
  BS->CloseEvent {@R_DoneEvent1,&@R_Status}
  GetAck
  EndScope _IP4CONFIG2_GETDATA_FUNC1
  EndLog
  return
}

Ip4Config2->GetData "@R_Ip4Config2DataType,&@R_Ip4Config2DataSize,&@R_Ip4Config2ManualAddress,&@R_Status"
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $Ip4Config2GetDataFunc1AssertionGuid003    \
                "Ip4Config2.GetData - Call GetData to get ManualAddress value."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"

EFI_IPv4_ADDRESS  R_Addr1
EFI_IPv4_ADDRESS  R_Addr2
set R_Addr1 [GetIpv4Address R_Ip4Config2ManualAddress.Address]
puts "R_Addr1 - $R_Addr1"
set R_Addr2 [GetIpv4Address R_Ip4Config2ManualAddress.SubnetMask]
puts "R_Addr1 - $R_Addr2"

if {[string compare -nocase $R_Addr1 192.168.100.1] == 0} {
  if {[string compare -nocase $R_Addr2 255.255.255.0] == 0} {
    set assert pass
  } else {
    set assert fail
  }
} else {
  set assert fail
}
RecordAssertion $assert $Ip4Config2GetDataFunc1AssertionGuid004    \
                "Set ManualAddress succeeds(event is signaled correctly and data correct)."

#
# Clean up
#
SetVar  R_Ip4Config2DataType  $IP4C2DT(Policy)
Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent,&@R_Status"
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid     \
                "Ip4Config2.UnregisterDataNotify - Unregister notification event for configuration."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"
BS->CloseEvent {@R_DoneEvent,&@R_Status}
GetAck

SetVar  R_Ip4Config2DataType  $IP4C2DT(ManualAddress)
Ip4Config2->UnregisterDataNotify "@R_Ip4Config2DataType,@R_DoneEvent1,&@R_Status"
GetAck
set assert [VerifyReturnStatus R_Status $EFI_SUCCESS]
RecordAssertion $assert $GenericAssertionGuid     \
                "Ip4Config2.UnregisterDataNotify - Unregister notification event for configuration."    \
                "ReturnStatus - $R_Status, ExpectedStatus - $EFI_SUCCESS"
BS->CloseEvent {@R_DoneEvent1,&@R_Status}
GetAck

EndScope _IP4CONFIG2_GETDATA_FUNC1

EndLog