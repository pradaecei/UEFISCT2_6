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
                                                                
  Copyright 2006 - 2012 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  LibPrivate.h

Abstract:

  This file defines the private interfaces of the test support libraries. Only
  the test management system can invokes these private interfaces to initialize
  and configure the test support libraries.

--*/

#ifndef _EFI_LIB_PRIVATE_H_
#define _EFI_LIB_PRIVATE_H_

//
// Includes
//

#include "LibConfig.h"

//
// Private interface for each test support library
//

#define TSL_INIT_PRIVATE_DATA_SIGNATURE     EFI_SIGNATURE_32('T','S','L','I')

typedef struct {
  UINT32                              Signature;
  EFI_HANDLE                          ImageHandle;
  EFI_TSL_INIT_INTERFACE              TslInit;
} TSL_INIT_PRIVATE_DATA;

#define TSL_INIT_PRIVATE_DATA_FROM_THIS(a)  \
  CR(a, TSL_INIT_PRIVATE_DATA, TslInit, TSL_INIT_PRIVATE_DATA_SIGNATURE)

//
// Standard test library's private interface
//

typedef struct _EFI_STANDARD_TSL_PRIVATE_INTERFACE EFI_STANDARD_TSL_PRIVATE_INTERFACE;

typedef
EFI_STATUS
(EFIAPI * EFI_STSL_SET_CONFIG) (
  IN EFI_STANDARD_TSL_PRIVATE_INTERFACE     *This,
  IN EFI_LIB_CONFIG_DATA                    *Config
  );

typedef
EFI_STATUS
(EFIAPI * EFI_STSL_BEGIN_LOGGING) (
  IN EFI_STANDARD_TSL_PRIVATE_INTERFACE     *This
  );

typedef
EFI_STATUS
(EFIAPI * EFI_STSL_END_LOGGING) (
  IN EFI_STANDARD_TSL_PRIVATE_INTERFACE     *This,
  IN EFI_STATUS                             TestStatus
  );

struct _EFI_STANDARD_TSL_PRIVATE_INTERFACE {
  EFI_STSL_SET_CONFIG                       SetConfig;
  EFI_STSL_BEGIN_LOGGING                    BeginLogging;
  EFI_STSL_END_LOGGING                      EndLogging;
};

//
// Test logging library's private interface
//

typedef struct _EFI_TLL_PRIVATE_INTERFACE EFI_TLL_PRIVATE_INTERFACE;

typedef
EFI_STATUS
(EFIAPI * EFI_TLL_SET_CONFIG) (
  IN EFI_TLL_PRIVATE_INTERFACE              *This,
  IN EFI_LIB_CONFIG_DATA                    *Config
  );

typedef
EFI_STATUS
(EFIAPI * EFI_TLL_BEGIN_LOGGING) (
  IN EFI_TLL_PRIVATE_INTERFACE              *This
  );

typedef
EFI_STATUS
(EFIAPI * EFI_TLL_END_LOGGING) (
  IN EFI_TLL_PRIVATE_INTERFACE              *This,
  IN EFI_STATUS                             TestStatus
  );

struct _EFI_TLL_PRIVATE_INTERFACE {
  EFI_TLL_SET_CONFIG                        SetConfig;
  EFI_TLL_BEGIN_LOGGING                     BeginLogging;
  EFI_TLL_END_LOGGING                       EndLogging;
};

//
// Test recovery library's private interface
//

typedef struct _EFI_TRL_PRIVATE_INTERFACE EFI_TRL_PRIVATE_INTERFACE;

typedef
EFI_STATUS
(EFIAPI * EFI_TRL_SET_CONFIG) (
  IN EFI_TRL_PRIVATE_INTERFACE              *This,
  IN EFI_DEVICE_PATH_PROTOCOL               *DevicePath,
  IN CHAR16                                 *FileName
  );

struct _EFI_TRL_PRIVATE_INTERFACE {
  EFI_TRL_SET_CONFIG                        SetConfig;
};

#endif
