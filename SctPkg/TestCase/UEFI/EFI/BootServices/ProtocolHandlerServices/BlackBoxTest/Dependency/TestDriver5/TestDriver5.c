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
                                                                
  Copyright 2006 - 2017 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2017, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  TestDriver5.c

Abstract:

  for Protocol Handler Boot Services Black Box Test

--*/

#include "SctLib.h"
#include "ProtocolDefinition.h"
#include "../Inc/TestDriver.h"

//
// data definition here
//
#define TEST_DRIVER_5_PRIVATE_DATA_FROM_THIS(a) \
 BASE_CR(a, TEST_DRIVER_PRIVATE_DATA, ExProt1)

TEST_DRIVER_PRIVATE_DATA  *mPrivateData;

EFI_STATUS
EFIAPI
InitializeTestDriver5 (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

VOID
TestDriver5GetNextStatusReport (
  IN EXTERNAL_DRIVER_PROTOCOL_1   *This,
  IN EFI_STATUS                   *NextStatus
  );

EFI_STATUS
EFIAPI
TestDriver5Unload (
  IN EFI_HANDLE       ImageHandle
  );


EFI_STATUS
EFIAPI
InitializeTestDriver5 (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                            Status;
  INTERFACE_FUNCTION_TEST_PROTOCOL_1    *IFTestProt1;
  INTERFACE_FUNCTION_TEST_PROTOCOL_2    *IFTestProt2;
  INTERFACE_FUNCTION_TEST_PROTOCOL_3    *IFTestProt3;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 NoHandles;
  EFI_LOADED_IMAGE_PROTOCOL             *LoadedImageInfoPtr;

  HandleBuffer = NULL;

  EfiInitializeTestLib (ImageHandle, SystemTable);

  //
  // allocate memory for PrivateData
  //
  Status = gtBS->AllocatePool (
                        EfiBootServicesData,
                        sizeof (TEST_DRIVER_PRIVATE_DATA),
                        (VOID**)&mPrivateData
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  gtBS->SetMem (mPrivateData,sizeof (TEST_DRIVER_PRIVATE_DATA),0);

  //
  // UnLoad Function Handler
  //
  gtBS->HandleProtocol (
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID*)&LoadedImageInfoPtr
        );

  LoadedImageInfoPtr->Unload = TestDriver5Unload;

  mPrivateData->ArrayCount = 3;
  Status = gtBS->AllocatePool (
                        EfiBootServicesData,
                        mPrivateData->ArrayCount * sizeof (EFI_STATUS),
                        (VOID**)&(mPrivateData->StatusArray)
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  //
  // init StatusArray with invalid data
  //
  gtBS->SetMem (
        mPrivateData->StatusArray,
        mPrivateData->ArrayCount * sizeof (EFI_STATUS),
        0xff
        );

  Status = gtBS->AllocatePool (
                        EfiBootServicesData,
                        mPrivateData->ArrayCount * sizeof (EFI_HANDLE),
                        (VOID**)&(mPrivateData->HandleArray)
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  gtBS->SetMem (
        mPrivateData->HandleArray,
        mPrivateData->ArrayCount * sizeof (EFI_HANDLE),
        0
        );

  Status = gtBS->AllocatePool (
                        EfiBootServicesData,
                        mPrivateData->ArrayCount * sizeof (EFI_GUID),
                        (VOID**)&(mPrivateData->ProtGuidArray)
                        );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  mPrivateData->ProtGuidArray[0] = mInterfaceFunctionTestProtocol1Guid;
  mPrivateData->ProtGuidArray[1] = mInterfaceFunctionTestProtocol2Guid;
  mPrivateData->ProtGuidArray[2] = mInterfaceFunctionTestProtocol3Guid;

  Status = gtBS->InstallProtocolInterface (
                  &mPrivateData->ChildHandle,
                  &mTestNoInterfaceProtocol1Guid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // locate handle which contains  INTERFACE_FUNCTION_TEST_PROTOCOL_1
  //
  Status = gtBS->LocateHandleBuffer (
                  ByProtocol,
                  &mInterfaceFunctionTestProtocol1Guid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  mPrivateData->HandleArray[0] = HandleBuffer[0];
  mPrivateData->StatusArray[0] = gtBS->OpenProtocol (
                                  HandleBuffer[0],
                                  &mInterfaceFunctionTestProtocol1Guid,
                                  (VOID **) &IFTestProt1,
                                  ImageHandle,
                                  mPrivateData->ChildHandle,
                                  EFI_OPEN_PROTOCOL_BY_DRIVER
                                  );

  if (HandleBuffer != NULL) {
    gtBS->FreePool (HandleBuffer);
    HandleBuffer = NULL;
  }

  Status = gtBS->LocateHandleBuffer (
                  ByProtocol,
                  &mInterfaceFunctionTestProtocol2Guid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    goto Done;
  }

  mPrivateData->HandleArray[1] = HandleBuffer[0];
  mPrivateData->StatusArray[1] = gtBS->OpenProtocol (
                                  HandleBuffer[0],
                                  &mInterfaceFunctionTestProtocol2Guid,
                                  (VOID **) &IFTestProt2,
                                  ImageHandle,
                                  mPrivateData->ChildHandle,
                                  EFI_OPEN_PROTOCOL_EXCLUSIVE
                                  );
  if (HandleBuffer != NULL) {
    gtBS->FreePool (HandleBuffer);
    HandleBuffer = NULL;
  }

  Status = gtBS->LocateHandleBuffer (
                  ByProtocol,
                  &mInterfaceFunctionTestProtocol3Guid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    goto Done;
  }

  mPrivateData->HandleArray[2] = HandleBuffer[0];
  mPrivateData->StatusArray[2] = gtBS->OpenProtocol (
                                  HandleBuffer[0],
                                  &mInterfaceFunctionTestProtocol3Guid,
                                  (VOID **) &IFTestProt3,
                                  ImageHandle,
                                  mPrivateData->ChildHandle,
                                  EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE
                                  );

  mPrivateData->ExProt1.GetNextStatusReport = TestDriver5GetNextStatusReport;

  Status = gtBS->InstallProtocolInterface (
                  &mPrivateData->Handle,
                  &mExternalDriverProtocol1Guid,
                  EFI_NATIVE_INTERFACE,
                  &mPrivateData->ExProt1
                  );

Done:

  if (HandleBuffer != NULL) {
    gtBS->FreePool (HandleBuffer);
    HandleBuffer = NULL;
  }

  return EFI_SUCCESS;
}

VOID
TestDriver5GetNextStatusReport (
  IN EXTERNAL_DRIVER_PROTOCOL_1   *This,
  IN EFI_STATUS                   *NextStatus
  )
{
  TEST_DRIVER_PRIVATE_DATA            *PrivateData;

  PrivateData = TEST_DRIVER_5_PRIVATE_DATA_FROM_THIS (This);

  if (*NextStatus == 0xffffffff) {
    PrivateData->Count = 0;
  } else {
    PrivateData->Count ++;
  }

  if (PrivateData->Count >= PrivateData->ArrayCount) {
    *NextStatus = 0xffffffff;
  } else {
    *NextStatus = PrivateData->StatusArray[PrivateData->Count];
  }

  return;
}


/**
 *  The driver's Unload function
 *  @param  ImageHandle The test driver image handle
 *  @return EFI_SUCCESS Indicates the interface was Uninstalled
*/
EFI_STATUS
EFIAPI
TestDriver5Unload (
  IN EFI_HANDLE       ImageHandle
  )
{
  UINTN                               Index;

  //
  // close protocols
  //
  if (mPrivateData->HandleArray != NULL && mPrivateData->ProtGuidArray != NULL) {
    for (Index = 0; Index < mPrivateData->ArrayCount; Index ++) {
      if (mPrivateData->HandleArray[Index] != NULL) {
        gtBS->CloseProtocol (
              mPrivateData->HandleArray[Index],
              &mPrivateData->ProtGuidArray[Index],
              ImageHandle,
              mPrivateData->ChildHandle
              );
      }
    }
  }

  //
  // free resources
  //
  if (mPrivateData->Handle != NULL) {
    gtBS->UninstallProtocolInterface (
                  mPrivateData->Handle,
                  &mExternalDriverProtocol1Guid,
                  &mPrivateData->ExProt1
                  );
  }

  if (mPrivateData->HandleArray != NULL) {
    gtBS->FreePool (mPrivateData->HandleArray);
  }

  if (mPrivateData->ProtGuidArray != NULL) {
    gtBS->FreePool (mPrivateData->ProtGuidArray);
  }

  if (mPrivateData->StatusArray != NULL) {
    gtBS->FreePool (mPrivateData->StatusArray);
  }

  if (mPrivateData->ChildHandle != NULL) {
    gtBS->UninstallProtocolInterface (
              mPrivateData->ChildHandle,
              &mTestNoInterfaceProtocol1Guid,
              NULL
              );
  }
  gtBS->FreePool (mPrivateData);

  return EFI_SUCCESS;
}
