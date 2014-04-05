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
    RuntimeServicesDriver2.c

Abstract:
    for Loaded Image Protocol Black Box Test

--*/

#include "LoadedImageBBTestProtocolDefinition.h"
#include <Library/EfiTestLib.h>

EFI_STATUS
InitializeRuntimeServicesDriver2 (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

EFI_STATUS
RuntimeServicesDriver2Unload (
  IN EFI_HANDLE       ImageHandle
  );

EFI_DRIVER_ENTRY_POINT(InitializeRuntimeServicesDriver2)

EFI_STATUS
InitializeRuntimeServicesDriver2 (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_LOADED_IMAGE_PROTOCOL             *LoadedImage;
  EFI_HANDLE                            Handle;
  UINTN                                 *Options;

  Handle = NULL;

  EfiInitializeTestLib (ImageHandle, SystemTable);

  //
  // UnLoad Function Handler
  //
  Status = gtBS->HandleProtocol (
                   ImageHandle,
                   &gEfiLoadedImageProtocolGuid,
                   (VOID*)&LoadedImage
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  LoadedImage->Unload = RuntimeServicesDriver2Unload;

  Options = (UINTN*)(LoadedImage->LoadOptions);

  if (Options == NULL) {
    return EFI_SUCCESS;
  }

  if (*Options == 5) {

    gtBS->InstallProtocolInterface (
            &Handle,
            &mLoadedImageTestNoInterfaceProtocol5Guid,
            EFI_NATIVE_INTERFACE,
            NULL
            );
  } else if (*Options == 6) {
    gtBS->InstallProtocolInterface (
            &Handle,
            &mLoadedImageTestNoInterfaceProtocol6Guid,
            EFI_NATIVE_INTERFACE,
            NULL
            );
  }

  if (Handle != NULL) {
    gtBS->UninstallProtocolInterface (
            Handle,
            &mLoadedImageTestNoInterfaceProtocol5Guid,
            NULL
            );
    gtBS->UninstallProtocolInterface (
            Handle,
            &mLoadedImageTestNoInterfaceProtocol6Guid,
            NULL
            );
  }
  return EFI_SUCCESS;
}

EFI_STATUS
RuntimeServicesDriver2Unload (
  IN EFI_HANDLE       ImageHandle
  )
{
  return EFI_SUCCESS;
}