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
                                                                
  Copyright 2016 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

    Hash2BBTestConformance.c

Abstract:

    for EFI Driver Hash2 Protocol's Conformance Test

--*/

#include "Hash2BBTestMain.h"

#define UNSUPPORTED_ALGO_GUID    \
  { 0x7ea76813, 0x6a66, 0x401a, {0x9d, 0xe6, 0xb3, 0xd0, 0x8b, 0x30, 0x52, 0xca }}

#define HASH2_SHA1_NOPAD_ALGO_GUID    \
  { 0x24c5dc2f, 0x53e2, 0x40ca, {0x9e, 0xd6, 0xa5, 0xd9, 0xa4, 0x9f, 0x46, 0x3b }}

#define HASH2_SHA256_NOPAD_ALGO_GUID    \
  { 0x8628752a, 0x6cb7, 0x4814, {0x96, 0xfc, 0x24, 0xa8, 0x15, 0xac, 0x22, 0x26 }}

EFI_GUID UnsupportedAlgoGuid                       = UNSUPPORTED_ALGO_GUID;
EFI_GUID gBlackBoxEfiHash2AlgorithmSha1NoPadGuid   = HASH2_SHA1_NOPAD_ALGO_GUID;
EFI_GUID gBlackBoxEfiHash2AlgorithmSha256NoPadGuid = HASH2_SHA256_NOPAD_ALGO_GUID;

extern EFI_GUID gHashAlgorithmGuids[6];

EFI_STATUS
BBTestGetHashSizeConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status, Status1, Status2, Status3, Status4;
  EFI_HASH2_SERVICE_BINDING_PROTOCOL    *Hash2SB;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_HANDLE                            ChildHandle = NULL;
  EFI_HASH2_PROTOCOL                    *Hash2 = NULL;
  UINTN                                 HashSize;
  UINT8                                 Index;

  Hash2SB = (EFI_HASH2_SERVICE_BINDING_PROTOCOL*)ClientInterface;
  if (Hash2SB == NULL)
  	return EFI_UNSUPPORTED;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **) &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = Hash2ServiceBindingCreateChild(Hash2SB, &ChildHandle, &Hash2);

  Status1 = Hash2->GetHashSize (Hash2, NULL, &HashSize);
  Status2 = Hash2->GetHashSize (Hash2, &UnsupportedAlgoGuid, &HashSize);
  Status3 = Hash2->GetHashSize (Hash2, &gBlackBoxEfiHash2AlgorithmSha1NoPadGuid, &HashSize);
  Status4 = Hash2->GetHashSize (Hash2, &gBlackBoxEfiHash2AlgorithmSha256NoPadGuid, &HashSize);
  if ((Status1 == EFI_UNSUPPORTED) && (Status2 == EFI_UNSUPPORTED) && (Status3 == EFI_UNSUPPORTED) && (Status4 == EFI_UNSUPPORTED))
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  else
    AssertionType = EFI_TEST_ASSERTION_FAILED;

  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHash2BBTestConformanceAssertionGuid001 ,
                 L"HASH2_PROTOCOL.GetHashSize - GetHashSize() returns EFI_UNSUPPORTED with unsupported HashAlgorithm or HashAlgorithm being NULL.",
                 L"%a:%d: Status1 - %r, Status2 - %r, Status3 - %r, Status4 - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status1,
                 Status2,
                 Status3,
                 Status4
                 );

  for (Index = 0; Index < 6; Index++) {
    Status = Hash2->GetHashSize (Hash2, &gHashAlgorithmGuids[Index], &HashSize);
	if (Status == EFI_SUCCESS) {
      Status = Hash2->GetHashSize (Hash2, &gHashAlgorithmGuids[Index], NULL);
      if (Status == EFI_INVALID_PARAMETER)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED; 

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid002,
                     L"HASH2_PROTOCOL.GetHashSize - GetHashSize() returns EFI_INVALID_PARAMETER with HashSize being NULL.",
                     L"%a:%d: Status - %r, Index - %d",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status,
                     Index
                     );
    }
  }

  Status = Hash2ServiceBindingDestoryChild(Hash2SB, ChildHandle);
  
  return EFI_SUCCESS;
}

EFI_STATUS
BBTestHashConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status, Status1, Status2, Status3, Status4;
  EFI_HASH2_SERVICE_BINDING_PROTOCOL    *Hash2SB;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_HANDLE                            ChildHandle = NULL;
  EFI_HASH2_PROTOCOL                    *Hash2 = NULL;
  UINTN                                 HashSize;
  UINT8                                 Index;

  CHAR8                                 Message[] = "ABCDEF";
  UINTN                                 MessageSize = 6;
  UINT8                                 Hash2Out[64];

  Hash2SB = (EFI_HASH2_SERVICE_BINDING_PROTOCOL*)ClientInterface;
  if (Hash2SB == NULL)
  	return EFI_UNSUPPORTED;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **) &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = Hash2ServiceBindingCreateChild(Hash2SB, &ChildHandle, &Hash2);

  Status1 = Hash2->Hash (Hash2, NULL, Message, MessageSize, (EFI_HASH2_OUTPUT*) Hash2Out);
  Status2 = Hash2->Hash (Hash2, &UnsupportedAlgoGuid, Message, MessageSize, (EFI_HASH2_OUTPUT*) Hash2Out);
  Status3 = Hash2->Hash (Hash2, &gBlackBoxEfiHash2AlgorithmSha1NoPadGuid, Message, MessageSize, (EFI_HASH2_OUTPUT*) Hash2Out);
  Status4 = Hash2->Hash (Hash2, &gBlackBoxEfiHash2AlgorithmSha256NoPadGuid, Message, MessageSize, (EFI_HASH2_OUTPUT*) Hash2Out);
  if ((Status1 == EFI_UNSUPPORTED) && (Status2 == EFI_UNSUPPORTED) && (Status1 == EFI_UNSUPPORTED) && (Status1 == EFI_UNSUPPORTED))
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  else
    AssertionType = EFI_TEST_ASSERTION_FAILED;

  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHash2BBTestConformanceAssertionGuid003,
                 L"HASH2_PROTOCOL.Hash - Hash() returns EFI_UNSUPPORTED with unsupported HashAlgorithm or HashAlgorithm being NULL.",
                 L"%a:%d: Status1 - %r, Status2 - %r, Status3 - %r, Status4 - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status1,
                 Status2,
                 Status3,
                 Status4
                 );

  for (Index = 0; Index < 6; Index++) {
    Status = Hash2->GetHashSize (Hash2, &gHashAlgorithmGuids[Index], &HashSize);
	if (Status == EFI_SUCCESS) {
      Status = Hash2->Hash (Hash2, &gHashAlgorithmGuids[Index], Message, MessageSize, NULL);
      if (Status == EFI_INVALID_PARAMETER)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED; 

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid004,
                     L"HASH2_PROTOCOL.Hash - Hash() returns EFI_INVALID_PARAMETER with Hash being NULL.",
                     L"%a:%d: Status - %r, Index - %d",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status,
                     Index
                     );
    }
  }

  Status = Hash2ServiceBindingDestoryChild(Hash2SB, ChildHandle);

  return EFI_SUCCESS;  
}


EFI_STATUS
BBTestHashInitConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status, Status1, Status2, Status3, Status4;
  EFI_HASH2_SERVICE_BINDING_PROTOCOL    *Hash2SB;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_HANDLE                            ChildHandle = NULL;
  EFI_HASH2_PROTOCOL                    *Hash2 = NULL;
  UINTN                                 HashSize;
  UINT8                                 Index;

  CHAR8                                 Message[] = "ABCDEF";
  UINTN                                 MessageSize = 6;

  Hash2SB = (EFI_HASH2_SERVICE_BINDING_PROTOCOL*)ClientInterface;
  if (Hash2SB == NULL)
  	return EFI_UNSUPPORTED;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **) &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = Hash2ServiceBindingCreateChild(Hash2SB, &ChildHandle, &Hash2);

  Status1 = Hash2->HashInit (Hash2, NULL);
  Status2 = Hash2->HashInit (Hash2, &UnsupportedAlgoGuid);
  Status3 = Hash2->HashInit (Hash2, &gBlackBoxEfiHash2AlgorithmSha1NoPadGuid);
  Status4 = Hash2->HashInit (Hash2, &gBlackBoxEfiHash2AlgorithmSha256NoPadGuid);
  if ((Status1 == EFI_UNSUPPORTED) && (Status2 == EFI_UNSUPPORTED) && (Status1 == EFI_UNSUPPORTED) && (Status1 == EFI_UNSUPPORTED))
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  else
    AssertionType = EFI_TEST_ASSERTION_FAILED;

  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHash2BBTestConformanceAssertionGuid005,
                 L"HASH2_PROTOCOL.HashInit - HashInit() returns EFI_UNSUPPORTED with unsupported HashAlgorithm or HashAlgorithm being NULL.",
                 L"%a:%d: Status1 - %r, Status2 - %r, Status3 - %r, Status4 - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status1,
                 Status2,
                 Status3,
                 Status4
                 );

  for (Index = 0; Index < 6; Index++) {
    Status = Hash2->GetHashSize (Hash2, &gHashAlgorithmGuids[Index], &HashSize);
	if (Status == EFI_SUCCESS) {
      Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
      Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
	  
      if (Status == EFI_ALREADY_STARTED)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED;

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid014,
                     L"HASH2_PROTOCOL.HashInit - HashInit() returns EFI_ALREADY_STARTED when it follows the call to HashInit().",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 
 

      Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
	  Status = Hash2->HashUpdate (Hash2, Message, MessageSize);

	  Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
      if (Status == EFI_ALREADY_STARTED)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED;  

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid015,
                     L"HASH2_PROTOCOL.HashInit - HashInit() returns EFI_ALREADY_STARTED when it follows the call to HashUpdate().",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 

    }
  }
  
  Status = Hash2ServiceBindingDestoryChild(Hash2SB, ChildHandle);

  return EFI_SUCCESS;  
}

EFI_STATUS
BBTestHashUpdateConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_HASH2_SERVICE_BINDING_PROTOCOL    *Hash2SB;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_HANDLE                            ChildHandle = NULL;
  EFI_HASH2_PROTOCOL                    *Hash2 = NULL;
  UINTN                                 HashSize;
  UINT8                                 Index;

  CHAR8                                 Message[] = "ABCDEF";
  UINTN                                 MessageSize = 6;
  UINT8                                 Hash2Out[64];

  Hash2SB = (EFI_HASH2_SERVICE_BINDING_PROTOCOL*)ClientInterface;
  if (Hash2SB == NULL)
  	return EFI_UNSUPPORTED;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **) &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = Hash2ServiceBindingCreateChild(Hash2SB, &ChildHandle, &Hash2);

  Status = Hash2->HashUpdate (Hash2, Message, MessageSize);
  if (Status == EFI_NOT_READY)
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  else
    AssertionType = EFI_TEST_ASSERTION_FAILED;

  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHash2BBTestConformanceAssertionGuid006,
                 L"HASH2_PROTOCOL.HashUpdate - HashUpdate() returns EFI_NOT_READY when it is not preceded by a call to HashInit().",
                 L"%a:%d: Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );  

  for (Index = 0; Index < 6; Index++) {
    Status = Hash2->GetHashSize (Hash2, &gHashAlgorithmGuids[Index], &HashSize);
	if (Status == EFI_SUCCESS) {
      Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
      Status = Hash2->Hash (Hash2, &gHashAlgorithmGuids[Index], Message, MessageSize, (EFI_HASH2_OUTPUT*) Hash2Out);
 
      Status = Hash2->HashUpdate (Hash2, Message, MessageSize);
      if (Status == EFI_NOT_READY)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED;

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid007,
                     L"HASH2_PROTOCOL.HashUpdate - HashUpdate() returns EFI_NOT_READY when it follows the call to Hash().",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 
 

      Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
	  Status = Hash2->HashUpdate (Hash2, Message, MessageSize);
	  Status = Hash2->HashFinal (Hash2, (EFI_HASH2_OUTPUT *) Hash2Out);
	  Status = Hash2->HashUpdate (Hash2, Message, MessageSize);
      if (Status == EFI_NOT_READY)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED;  

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid008,
                     L"HASH2_PROTOCOL.HashUpdate - HashUpdate() returns EFI_NOT_READY when it follows the call to HashFinal().",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 

    }
  }

  Status = Hash2ServiceBindingDestoryChild(Hash2SB, ChildHandle);

  return EFI_SUCCESS;  
}

EFI_STATUS
BBTestHashFinalConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_HASH2_SERVICE_BINDING_PROTOCOL    *Hash2SB;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_HANDLE                            ChildHandle = NULL;
  EFI_HASH2_PROTOCOL                    *Hash2 = NULL;
  UINTN                                 HashSize;
  UINT8                                 Index;

  CHAR8                                 Message[] = "ABCDEF";
  UINTN                                 MessageSize = 6;
  UINT8                                 Hash2Out[64];

  Hash2SB = (EFI_HASH2_SERVICE_BINDING_PROTOCOL*)ClientInterface;
  if (Hash2SB == NULL)
  	return EFI_UNSUPPORTED;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **) &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = Hash2ServiceBindingCreateChild(Hash2SB, &ChildHandle, &Hash2);

  Status = Hash2->HashFinal (Hash2,  (EFI_HASH2_OUTPUT *) Hash2Out);
  if (Status == EFI_NOT_READY)
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  else
    AssertionType = EFI_TEST_ASSERTION_FAILED;

  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHash2BBTestConformanceAssertionGuid009,
                 L"HASH2_PROTOCOL.HashFinal - HashFinal() returns EFI_NOT_READY when it is not preceded by the call to HashInit()/HashUpdate().",
                 L"%a:%d: Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );  

  for (Index = 0; Index < 6; Index++) {
    Status = Hash2->GetHashSize (Hash2, &gHashAlgorithmGuids[Index], &HashSize);
	if (Status == EFI_SUCCESS) {
      Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
      Status = Hash2->HashFinal (Hash2, (EFI_HASH2_OUTPUT *) Hash2Out);
      if (Status == EFI_NOT_READY)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED;

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid010,
                     L"HASH2_PROTOCOL.HashFinal - HashFinal() returns EFI_NOT_READY when it doesn't follow the call to HashUpdate().",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 

      Status = Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
      Status = Hash2->HashUpdate (Hash2, Message, MessageSize);
	  Status =Hash2->Hash (Hash2, &gHashAlgorithmGuids[Index], Message, MessageSize, (EFI_HASH2_OUTPUT*) Hash2Out);
	  Status = Hash2->HashFinal (Hash2,  (EFI_HASH2_OUTPUT *) Hash2Out);
      if (Status == EFI_NOT_READY)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED;

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid011,
                     L"HASH2_PROTOCOL.HashFinal - HashFinal() returns EFI_NOT_READY when it follows the call to Hash().",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 
 

      Status= Hash2->HashInit (Hash2, &gHashAlgorithmGuids[Index]);
	  Status =Hash2->HashUpdate (Hash2, Message, MessageSize);

      Status = Hash2->HashFinal (Hash2, (EFI_HASH2_OUTPUT *) NULL);
      if (Status == EFI_INVALID_PARAMETER)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED; 	  

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid012,
                     L"HASH2_PROTOCOL.HashFinal - HashFinal() returns EFI_INVALID_PARAMETER when Hash is NULL.",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 
      
      Status = Hash2->HashFinal (Hash2,  (EFI_HASH2_OUTPUT *) Hash2Out);
      Status = Hash2->HashFinal (Hash2, (EFI_HASH2_OUTPUT*) Hash2Out);;
      if (Status == EFI_NOT_READY)
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      else
        AssertionType = EFI_TEST_ASSERTION_FAILED;  

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gHash2BBTestConformanceAssertionGuid013,
                     L"HASH2_PROTOCOL.HashFinal - HashFinal() returns EFI_NOT_READY when it follows the call to HashFinal().",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     ); 

    }
  }

  Status = Hash2ServiceBindingDestoryChild(Hash2SB, ChildHandle);

  return EFI_SUCCESS;  
}

