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

  BlockIoProtocol.h

Abstract:

  Block I/O Protocol (define according to the EFI Spec 2.3.1 )

--*/

#ifndef _EFI_BLOCK_IO_FOR_TEST_H_
#define _EFI_BLOCK_IO_FOR_TEST_H_

//
// Prevent the original BlockIo.h is included
//
#define  __BLOCK_IO_H__

#define EFI_BLOCK_IO_PROTOCOL_GUID \
  { \
    0x964e5b21, 0x6459, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
  }

typedef struct _EFI_BLOCK_IO_PROTOCOL  EFI_BLOCK_IO_PROTOCOL;

///
/// Protocol GUID name defined in EFI1.1.
/// 
#define BLOCK_IO_PROTOCOL       EFI_BLOCK_IO_PROTOCOL_GUID

///
/// Protocol defined in EFI1.1.
/// 
typedef struct _EFI_BLOCK_IO_PROTOCOL   EFI_BLOCK_IO_PROTOCOL;

/**
  Reset the Block Device.

  @param  This                 Indicates a pointer to the calling context.
  @param  ExtendedVerification Driver may perform diagnostics on reset.

  @retval EFI_SUCCESS          The device was reset.
  @retval EFI_DEVICE_ERROR     The device is not functioning properly and could
                               not be reset.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BLOCK_RESET)(
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN BOOLEAN                        ExtendedVerification
  );

/**
  Read BufferSize bytes from Lba into Buffer.

  @param  This       Indicates a pointer to the calling context.
  @param  MediaId    Id of the media, changes every time the media is replaced.
  @param  Lba        The starting Logical Block Address to read from
  @param  BufferSize Size of Buffer, must be a multiple of device block size.
  @param  Buffer     A pointer to the destination buffer for the data. The caller is
                     responsible for either having implicit or explicit ownership of the buffer.

  @retval EFI_SUCCESS           The data was read correctly from the device.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the read.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHANGED     The MediaId does not matched the current device.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BLOCK_READ)(
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  OUT VOID                          *Buffer
  );

/**
  Write BufferSize bytes from Lba into Buffer.

  @param  This       Indicates a pointer to the calling context.
  @param  MediaId    The media ID that the write request is for.
  @param  Lba        The starting logical block address to be written. The caller is
                     responsible for writing to only legitimate locations.
  @param  BufferSize Size of Buffer, must be a multiple of device block size.
  @param  Buffer     A pointer to the source buffer for the data.

  @retval EFI_SUCCESS           The data was written correctly to the device.
  @retval EFI_WRITE_PROTECTED   The device can not be written to.
  @retval EFI_DEVICE_ERROR      The device reported an error while performing the write.
  @retval EFI_NO_MEDIA          There is no media in the device.
  @retval EFI_MEDIA_CHNAGED     The MediaId does not matched the current device.
  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.
  @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not valid, 
                                or the buffer is not on proper alignment.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BLOCK_WRITE)(
  IN EFI_BLOCK_IO_PROTOCOL          *This,
  IN UINT32                         MediaId,
  IN EFI_LBA                        Lba,
  IN UINTN                          BufferSize,
  IN VOID                           *Buffer
  );

/**
  Flush the Block Device.

  @param  This              Indicates a pointer to the calling context.

  @retval EFI_SUCCESS       All outstanding data was written to the device
  @retval EFI_DEVICE_ERROR  The device reported an error while writting back the data
  @retval EFI_NO_MEDIA      There is no media in the device.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BLOCK_FLUSH)(
  IN EFI_BLOCK_IO_PROTOCOL  *This
  );

/**
  Block IO read only mode data and updated only via members of BlockIO
**/
typedef struct {
  ///
  /// The curent media Id. If the media changes, this value is changed.
  ///
  UINT32  MediaId;         
   
  ///
  /// TRUE if the media is removable; otherwise, FALSE.
  ///    
  BOOLEAN RemovableMedia;
  
  ///
  /// TRUE if there is a media currently present in the device;
  /// othersise, FALSE. THis field shows the media present status
  /// as of the most recent ReadBlocks() or WriteBlocks() call.  
  ///
  BOOLEAN MediaPresent;

  ///
  /// TRUE if LBA 0 is the first block of a partition; otherwise
  /// FALSE. For media with only one partition this would be TRUE.
  ///
  BOOLEAN LogicalPartition;
  
  ///
  /// TRUE if the media is marked read-only otherwise, FALSE.
  /// This field shows the read-only status as of the most recent WriteBlocks () call.
  ///
  BOOLEAN ReadOnly;
  
  ///
  /// TRUE if the WriteBlock () function caches write data.
  ///
  BOOLEAN WriteCaching; 
  
  ///
  /// The intrinsic block size of the device. If the media changes, then
  /// this field is updated.  
  ///
  UINT32  BlockSize; 
  
  ///
  /// Supplies the alignment requirement for any buffer to read or write block(s).
  ///
  UINT32  IoAlign; 
  
  ///
  /// The last logical block address on the device.
  /// If the media changes, then this field is updated. 
  ///
  EFI_LBA LastBlock; 

  ///
  /// Only present if EFI_BLOCK_IO_PROTOCOL.Revision is greater than or equal to
  /// EFI_BLOCK_IO_PROTOCOL_REVISION2. Returns the first LBA is aligned to 
  /// a physical block boundary. 
  ///
  EFI_LBA LowestAlignedLba;

  ///
  /// Only present if EFI_BLOCK_IO_PROTOCOL.Revision is greater than or equal to
  /// EFI_BLOCK_IO_PROTOCOL_REVISION2. Returns the number of logical blocks 
  /// per physical block.
  ///
  UINT32 LogicalBlocksPerPhysicalBlock;

  ///
  /// Only present if EFI_BLOCK_IO_PROTOCOL.Revision is greater than or equal to
  /// EFI_BLOCK_IO_PROTOCOL_REVISION3. Returns the optimal transfer length
  /// granularity as a number of logical blocks.
  ///
  UINT32 OptimalTransferLengthGranularity;
} EFI_BLOCK_IO_MEDIA;

#define EFI_BLOCK_IO_PROTOCOL_REVISION  0x00010000
#define EFI_BLOCK_IO_PROTOCOL_REVISION2 0x00020001
#define EFI_BLOCK_IO_PROTOCOL_REVISION3 0x00020031

///
/// Revision defined in EFI1.1.
/// 
#define EFI_BLOCK_IO_INTERFACE_REVISION   EFI_BLOCK_IO_PROTOCOL_REVISION

///
///  This protocol provides control over block devices.
///
struct _EFI_BLOCK_IO_PROTOCOL {
  ///
  /// The revision to which the block IO interface adheres. All future
  /// revisions must be backwards compatible. If a future version is not
  /// back wards compatible, it is not the same GUID.
  ///
  UINT64              Revision;
  ///
  /// Pointer to the EFI_BLOCK_IO_MEDIA data for this device.
  ///
  EFI_BLOCK_IO_MEDIA  *Media;

  EFI_BLOCK_RESET     Reset;
  EFI_BLOCK_READ      ReadBlocks;
  EFI_BLOCK_WRITE     WriteBlocks;
  EFI_BLOCK_FLUSH     FlushBlocks;

};

extern EFI_GUID gBlackBoxEfiBlockIoProtocolGuid;

#endif
