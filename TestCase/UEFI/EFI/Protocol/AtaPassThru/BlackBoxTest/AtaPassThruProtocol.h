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

  AtaPassThruProtocol.h

Abstract:

--*/


#ifndef _ATA_PASS_THRU_FOR_TEST_H_
#define _ATA_PASS_THRU_FOR_TEST_H_

#define EFI_ATA_PASS_THRU_PROTOCOL_GUID \
  { \
    0x1d3de7f0, 0x807, 0x424f, {0xaa, 0x69, 0x11, 0xa5, 0x4e, 0x19, 0xa4, 0x6f } \
  }

typedef struct _EFI_ATA_PASS_THRU_PROTOCOL EFI_ATA_PASS_THRU_PROTOCOL;

typedef struct {
  UINT32    Attributes;
  UINT32    IoAlign;
} EFI_ATA_PASS_THRU_MODE;

///
/// If this bit is set, then the EFI_ATA_PASS_THRU_PROTOCOL interface is for physical
/// devices on the ATA controller.
///
#define EFI_ATA_PASS_THRU_ATTRIBUTES_PHYSICAL   0x0001
///
/// If this bit is set, then the EFI_ATA_PASS_THRU_PROTOCOL interface is for logical
/// devices on the ATA controller.
///
#define EFI_ATA_PASS_THRU_ATTRIBUTES_LOGICAL    0x0002
///
/// If this bit is set, then the EFI_ATA_PASS_THRU_PROTOCOL interface supports non blocking
/// I/O. Every EFI_ATA_PASS_THRU_PROTOCOL must support blocking I/O. The support of non-blocking
/// I/O is optional.
///
#define EFI_ATA_PASS_THRU_ATTRIBUTES_NONBLOCKIO 0x0004

typedef struct _EFI_ATA_COMMAND_BLOCK {
  UINT8 Reserved1[2];
  UINT8 AtaCommand;
  UINT8 AtaFeatures;
  UINT8 AtaSectorNumber;
  UINT8 AtaCylinderLow;
  UINT8 AtaCylinderHigh;
  UINT8 AtaDeviceHead;
  UINT8 AtaSectorNumberExp;
  UINT8 AtaCylinderLowExp;
  UINT8 AtaCylinderHighExp; 
  UINT8 AtaFeaturesExp;
  UINT8 AtaSectorCount;
  UINT8 AtaSectorCountExp;
  UINT8 Reserved2[6];
} EFI_ATA_COMMAND_BLOCK;

typedef struct _EFI_ATA_STATUS_BLOCK {
  UINT8 Reserved1[2];
  UINT8 AtaStatus;
  UINT8 AtaError;
  UINT8 AtaSectorNumber;
  UINT8 AtaCylinderLow;
  UINT8 AtaCylinderHigh;
  UINT8 AtaDeviceHead;
  UINT8 AtaSectorNumberExp;
  UINT8 AtaCylinderLowExp;
  UINT8 AtaCylinderHighExp; 
  UINT8 Reserved;
  UINT8 AtaSectorCount;
  UINT8 AtaSectorCountExp;
  UINT8 Reserved2[6];
} EFI_ATA_STATUS_BLOCK;

typedef UINT8 EFI_ATA_PASS_THRU_CMD_PROTOCOL;

#define EFI_ATA_PASS_THRU_PROTOCOL_ATA_HARDWARE_RESET 0x00
#define EFI_ATA_PASS_THRU_PROTOCOL_ATA_SOFTWARE_RESET 0x01
#define EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA       0x02
#define EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN        0x04
#define EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_OUT       0x05
#define EFI_ATA_PASS_THRU_PROTOCOL_DMA                0x06
#define EFI_ATA_PASS_THRU_PROTOCOL_DMA_QUEUED         0x07
#define EFI_ATA_PASS_THRU_PROTOCOL_DEVICE_DIAGNOSTIC  0x08
#define EFI_ATA_PASS_THRU_PROTOCOL_DEVICE_RESET       0x09
#define EFI_ATA_PASS_THRU_PROTOCOL_UDMA_DATA_IN       0x0A
#define EFI_ATA_PASS_THRU_PROTOCOL_UDMA_DATA_OUT      0x0B
#define EFI_ATA_PASS_THRU_PROTOCOL_FPDMA              0x0C
#define EFI_ATA_PASS_THRU_PROTOCOL_RETURN_RESPONSE    0xFF

typedef UINT8 EFI_ATA_PASS_THRU_LENGTH;

#define EFI_ATA_PASS_THRU_LENGTH_BYTES                0x80


#define EFI_ATA_PASS_THRU_LENGTH_MASK                 0x70
#define EFI_ATA_PASS_THRU_LENGTH_NO_DATA_TRANSFER     0x00
#define EFI_ATA_PASS_THRU_LENGTH_FEATURES             0x10
#define EFI_ATA_PASS_THRU_LENGTH_SECTOR_COUNT         0x20
#define EFI_ATA_PASS_THRU_LENGTH_TPSIU                0x30

#define EFI_ATA_PASS_THRU_LENGTH_COUNT                0x0F

typedef struct {
  ///
  /// A pointer to the sense data that was generated by the execution of the ATA
  /// command. It must be aligned to the boundary specified in the IoAlign field
  /// in the EFI_ATA_PASS_THRU_MODE structure.
  ///
  EFI_ATA_STATUS_BLOCK              *Asb;
  ///
  /// A pointer to buffer that contains the Command Data Block to send to the ATA
  /// device specified by Port and PortMultiplierPort.
  ///
  EFI_ATA_COMMAND_BLOCK             *Acb;
  ///
  /// The timeout, in 100 ns units, to use for the execution of this ATA command.
  /// A Timeout value of 0 means that this function will wait indefinitely for the
  /// ATA command to execute. If Timeout is greater than zero, then this function
  /// will return EFI_TIMEOUT if the time required to execute the ATA command is
  /// greater than Timeout.
  ///
  UINT64                            Timeout;
  ///
  /// A pointer to the data buffer to transfer between the ATA controller and the
  /// ATA device for read and bidirectional commands. For all write and non data
  /// commands where InTransferLength is 0 this field is optional and may be NULL.
  /// If this field is not NULL, then it must be aligned on the boundary specified
  /// by the IoAlign field in the EFI_ATA_PASS_THRU_MODE structure.
  ///
  VOID                              *InDataBuffer;
  ///
  /// A pointer to the data buffer to transfer between the ATA controller and the
  /// ATA device for write or bidirectional commands. For all read and non data
  /// commands where OutTransferLength is 0 this field is optional and may be NULL.
  /// If this field is not NULL, then it must be aligned on the boundary specified
  /// by the IoAlign field in the EFI_ATA_PASS_THRU_MODE structure.
  ///
  VOID                              *OutDataBuffer;
  ///
  /// On input, the size, in bytes, of InDataBuffer. On output, the number of bytes
  /// transferred between the ATA controller and the ATA device. If InTransferLength
  /// is larger than the ATA controller can handle, no data will be transferred,
  /// InTransferLength will be updated to contain the number of bytes that the ATA
  /// controller is able to transfer, and EFI_BAD_BUFFER_SIZE will be returned.
  ///
  UINT32                            InTransferLength;
  ///
  /// On Input, the size, in bytes of OutDataBuffer. On Output, the Number of bytes
  /// transferred between ATA Controller and the ATA device. If OutTransferLength is
  /// larger than the ATA controller can handle, no data will be transferred, 
  /// OutTransferLength will be updated to contain the number of bytes that the ATA
  /// controller is able to transfer, and EFI_BAD_BUFFER_SIZE will be returned.
  ///
  UINT32                            OutTransferLength;
  ///
  /// Specifies the protocol used when the ATA device executes the command.
  ///
  EFI_ATA_PASS_THRU_CMD_PROTOCOL    Protocol;
  ///
  /// Specifies the way in which the ATA command length is encoded.
  ///
  EFI_ATA_PASS_THRU_LENGTH          Length;
} EFI_ATA_PASS_THRU_COMMAND_PACKET;


/**
  Sends an ATA command to an ATA device that is attached to the ATA controller. This function
  supports both blocking I/O and non-blocking I/O. The blocking I/O functionality is required,
  and the non-blocking I/O functionality is optional.

  @param[in]     This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance. 
  @param[in]     Port                The port number of the ATA device to send the command. 
  @param[in]     PortMultiplierPort  The port multiplier port number of the ATA device to send the command.
                                     If there is no port multiplier, then specify 0.
  @param[in,out] Packet              A pointer to the ATA command to send to the ATA device specified by Port
                                     and PortMultiplierPort.
  @param[in]     Event               If non-blocking I/O is not supported then Event is ignored, and blocking
                                     I/O is performed. If Event is NULL, then blocking I/O is performed. If
                                     Event is not NULL and non blocking I/O is supported, then non-blocking
                                     I/O is performed, and Event will be signaled when the ATA command completes.

  @retval EFI_SUCCESS                The ATA command was sent by the host. For bi-directional commands, 
                                     InTransferLength bytes were transferred from InDataBuffer. For write and
                                     bi-directional commands, OutTransferLength bytes were transferred by OutDataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The ATA command was not executed. The number of bytes that could be transferred
                                     is returned in InTransferLength. For write and bi-directional commands, 
                                     OutTransferLength bytes were transferred by OutDataBuffer.
  @retval EFI_NOT_READY              The ATA command could not be sent because there are too many ATA commands
                                     already queued. The caller may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the ATA command.
  @retval EFI_INVALID_PARAMETER      Port, PortMultiplierPort, or the contents of Acb are invalid. The ATA
                                     command was not sent, so no additional status information is available.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ATA_PASS_THRU_PASSTHRU)(
  IN     EFI_ATA_PASS_THRU_PROTOCOL          *This,
  IN     UINT16                              Port,
  IN     UINT16                              PortMultiplierPort,
  IN OUT EFI_ATA_PASS_THRU_COMMAND_PACKET    *Packet,
  IN     EFI_EVENT                           Event OPTIONAL
  );

/**
  Used to retrieve the lis t of legal port numbers for ATA devices on an ATA controller.
  These can either be the list of ports where ATA devices are actually present or the
  list of legal port numbers for the ATA controller. Regardless, the caller of this
  function must probe the port number returned to see if an ATA device is actually
  present at that location on the ATA controller.

  The GetNextPort() function retrieves the port number on an ATA controller. If on input
  Port is 0xFFFF, then the port number of the first port on the ATA controller is returned
  in Port and EFI_SUCCESS is returned.

  If Port is a port number that was returned on a previous call to GetNextPort(), then the
  port number of the next port on the ATA controller is returned in Port, and EFI_SUCCESS
  is returned. If Port is not 0xFFFF and Port was not returned on a previous call to
  GetNextPort(), then EFI_INVALID_PARAMETER is returned.

  If Port is the port number of the last port on the ATA controller, then EFI_NOT_FOUND is
  returned.

  @param[in]     This           A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance. 
  @param[in,out] Port           On input, a pointer to the port number on the ATA controller.
                                On output, a pointer to the next port number on the ATA
                                controller. An input value of 0xFFFF retrieves the first port
                                number on the ATA controller.

  @retval EFI_SUCCESS           The next port number on the ATA controller was returned in Port.
  @retval EFI_NOT_FOUND         There are no more ports on this ATA controller.
  @retval EFI_INVALID_PARAMETER Port is not 0xFFFF and Port was not returned on a previous call
                                to GetNextPort().

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ATA_PASS_THRU_GET_NEXT_PORT)(
  IN EFI_ATA_PASS_THRU_PROTOCOL    *This,
  IN OUT UINT16                    *Port
  );

/**
  Used to retrieve the list of legal port multiplier port numbers for ATA devices on a port of an ATA 
  controller. These can either be the list of port multiplier ports where ATA devices are actually 
  present on port or the list of legal port multiplier ports on that port. Regardless, the caller of this 
  function must probe the port number and port multiplier port number returned to see if an ATA 
  device is actually present.

  The GetNextDevice() function retrieves the port multiplier port number of an ATA device 
  present on a port of an ATA controller.
  
  If PortMultiplierPort points to a port multiplier port number value that was returned on a 
  previous call to GetNextDevice(), then the port multiplier port number of the next ATA device
  on the port of the ATA controller is returned in PortMultiplierPort, and EFI_SUCCESS is
  returned.
  
  If PortMultiplierPort points to 0xFFFF, then the port multiplier port number of the first 
  ATA device on port of the ATA controller is returned in PortMultiplierPort and 
  EFI_SUCCESS is returned.
  
  If PortMultiplierPort is not 0xFFFF and the value pointed to by PortMultiplierPort
  was not returned on a previous call to GetNextDevice(), then EFI_INVALID_PARAMETER
  is returned.
  
  If PortMultiplierPort is the port multiplier port number of the last ATA device on the port of 
  the ATA controller, then EFI_NOT_FOUND is returned.

  @param[in]     This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param[in]     Port                The port number present on the ATA controller.
  @param[in,out] PortMultiplierPort  On input, a pointer to the port multiplier port number of an
                                     ATA device present on the ATA controller. 
                                     If on input a PortMultiplierPort of 0xFFFF is specified, 
                                     then the port multiplier port number of the first ATA device
                                     is returned. On output, a pointer to the port multiplier port
                                     number of the next ATA device present on an ATA controller.

  @retval EFI_SUCCESS                The port multiplier port number of the next ATA device on the port
                                     of the ATA controller was returned in PortMultiplierPort.
  @retval EFI_NOT_FOUND              There are no more ATA devices on this port of the ATA controller.
  @retval EFI_INVALID_PARAMETER      PortMultiplierPort is not 0xFFFF, and PortMultiplierPort was not
                                     returned on a previous call to GetNextDevice().

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ATA_PASS_THRU_GET_NEXT_DEVICE)(
  IN EFI_ATA_PASS_THRU_PROTOCOL    *This,
  IN UINT16                        Port,
  IN OUT UINT16                    *PortMultiplierPort
  );

/**
  Used to allocate and build a device path node for an ATA device on an ATA controller.

  The BuildDevicePath() function allocates and builds a single device node for the ATA
  device specified by Port and PortMultiplierPort. If the ATA device specified by Port and
  PortMultiplierPort is not present on the ATA controller, then EFI_NOT_FOUND is returned.
  If DevicePath is NULL, then EFI_INVALID_PARAMETER is returned. If there are not enough
  resources to allocate the device path node, then EFI_OUT_OF_RESOURCES is returned.

  Otherwise, DevicePath is allocated with the boot service SctAllocatePool (), the contents of
  DevicePath are initialized to describe the ATA device specified by Port and PortMultiplierPort,
  and EFI_SUCCESS is returned.

  @param[in]     This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param[in]     Port                Port specifies the port number of the ATA device for which a
                                     device path node is to be allocated and built.
  @param[in]     PortMultiplierPort  The port multiplier port number of the ATA device for which a
                                     device path node is to be allocated and built. If there is no
                                     port multiplier, then specify 0.
  @param[in,out] DevicePath          A pointer to a single device path node that describes the ATA
                                     device specified by Port and PortMultiplierPort. This function
                                     is responsible for allocating the buffer DevicePath with the
                                     boot service SctAllocatePool (). It is the caller's responsibility
                                     to free DevicePath when the caller is finished with DevicePath.
  @retval EFI_SUCCESS                The device path node that describes the ATA device specified by
                                     Port and PortMultiplierPort was allocated and returned in DevicePath.
  @retval EFI_NOT_FOUND              The ATA device specified by Port and PortMultiplierPort does not
                                     exist on the ATA controller.
  @retval EFI_INVALID_PARAMETER      DevicePath is NULL.
  @retval EFI_OUT_OF_RESOURCES       There are not enough resources to allocate DevicePath.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ATA_PASS_THRU_BUILD_DEVICE_PATH)(
  IN     EFI_ATA_PASS_THRU_PROTOCOL    *This,
  IN     UINT16                        Port,
  IN     UINT16                        PortMultiplierPort,
  IN OUT EFI_DEVICE_PATH_PROTOCOL      **DevicePath
  );

/**
  Used to translate a device path node to a port number and port multiplier port number.

  The GetDevice() function determines the port and port multiplier port number associated with
  the ATA device described by DevicePath. If DevicePath is a device path node type that the
  ATA Pass Thru driver supports, then the ATA Pass Thru driver will attempt to translate the contents 
  DevicePath into a port number and port multiplier port number.

  If this translation is successful, then that port number and port multiplier port number are returned
  in Port and PortMultiplierPort, and EFI_SUCCESS is returned.

  If DevicePath, Port, or PortMultiplierPort are NULL, then EFI_INVALID_PARAMETER is returned.

  If DevicePath is not a device path node type that the ATA Pass Thru driver supports, then 
  EFI_UNSUPPORTED is returned.

  If DevicePath is a device path node type that the ATA Pass Thru driver supports, but there is not 
  a valid translation from DevicePath to a port number and port multiplier port number, then 
  EFI_NOT_FOUND is returned.

  @param[in]  This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param[in]  DevicePath          A pointer to the device path node that describes an ATA device on the
                                  ATA controller.
  @param[out] Port                On return, points to the port number of an ATA device on the ATA controller.
  @param[out] PortMultiplierPort  On return, points to the port multiplier port number of an ATA device
                                  on the ATA controller.

  @retval EFI_SUCCESS             DevicePath was successfully translated to a port number and port multiplier
                                  port number, and they were returned in Port and PortMultiplierPort.
  @retval EFI_INVALID_PARAMETER   DevicePath is NULL.
  @retval EFI_INVALID_PARAMETER   Port is NULL.
  @retval EFI_INVALID_PARAMETER   PortMultiplierPort is NULL.
  @retval EFI_UNSUPPORTED         This driver does not support the device path node type in DevicePath.
  @retval EFI_NOT_FOUND           A valid translation from DevicePath to a port number and port multiplier
                                  port number does not exist.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_ATA_PASS_THRU_GET_DEVICE)(
  IN  EFI_ATA_PASS_THRU_PROTOCOL    *This,
  IN  EFI_DEVICE_PATH_PROTOCOL      *DevicePath,
  OUT UINT16                        *Port,
  OUT UINT16                        *PortMultiplierPort
  );

/**
  Resets a specific port on the ATA controller. This operation also resets all the ATA devices
  connected to the port.

  The ResetChannel() function resets an a specific port on an ATA controller. This operation
  resets all the ATA devices connected to that port. If this ATA controller does not support
  a reset port operation, then EFI_UNSUPPORTED is returned.

  If a device error occurs while executing that port reset operation, then EFI_DEVICE_ERROR is
  returned.

  If a timeout occurs during the execution of the port reset operation, then EFI_TIMEOUT is returned.

  If the port reset operation is completed, then EFI_SUCCESS is returned.

  @param[in]  This          A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param[in]  Port          The port number on the ATA controller.

  @retval EFI_SUCCESS       The ATA controller port was reset.
  @retval EFI_UNSUPPORTED   The ATA controller does not support a port reset operation.
  @retval EFI_DEVICE_ERROR  A device error occurred while attempting to reset the ATA port.
  @retval EFI_TIMEOUT       A timeout occurred while attempting to reset the ATA port.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ATA_PASS_THRU_RESET_PORT)(
  IN EFI_ATA_PASS_THRU_PROTOCOL    *This,
  IN UINT16                        Port
  );

/**
  Resets an ATA device that is connected to an ATA controller.

  The ResetDevice() function resets the ATA device specified by Port and PortMultiplierPort.
  If this ATA controller does not support a device reset operation, then EFI_UNSUPPORTED is
  returned.

  If Port or PortMultiplierPort are not in a valid range for this ATA controller, then 
  EFI_INVALID_PARAMETER is returned.

  If a device error occurs while executing that device reset operation, then EFI_DEVICE_ERROR
  is returned.

  If a timeout occurs during the execution of the device reset operation, then EFI_TIMEOUT is
  returned.

  If the device reset operation is completed, then EFI_SUCCESS is returned.

  @param[in] This                A pointer to the EFI_ATA_PASS_THRU_PROTOCOL instance.
  @param[in] Port                Port represents the port number of the ATA device to be reset.
  @param[in] PortMultiplierPort  The port multiplier port number of the ATA device to reset.
                                 If there is no port multiplier, then specify 0.
  @retval EFI_SUCCESS            The ATA device specified by Port and PortMultiplierPort was reset.
  @retval EFI_UNSUPPORTED        The ATA controller does not support a device reset operation.
  @retval EFI_INVALID_PARAMETER  Port or PortMultiplierPort are invalid.
  @retval EFI_DEVICE_ERROR       A device error occurred while attempting to reset the ATA device
                                 specified by Port and PortMultiplierPort.
  @retval EFI_TIMEOUT            A timeout occurred while attempting to reset the ATA device
                                 specified by Port and PortMultiplierPort.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ATA_PASS_THRU_RESET_DEVICE)(
  IN EFI_ATA_PASS_THRU_PROTOCOL    *This,
  IN UINT16                        Port,
  IN UINT16                        PortMultiplierPort
  );

struct _EFI_ATA_PASS_THRU_PROTOCOL {
  EFI_ATA_PASS_THRU_MODE                 *Mode;
  EFI_ATA_PASS_THRU_PASSTHRU             PassThru;
  EFI_ATA_PASS_THRU_GET_NEXT_PORT        GetNextPort;
  EFI_ATA_PASS_THRU_GET_NEXT_DEVICE      GetNextDevice;
  EFI_ATA_PASS_THRU_BUILD_DEVICE_PATH    BuildDevicePath;
  EFI_ATA_PASS_THRU_GET_DEVICE           GetDevice;
  EFI_ATA_PASS_THRU_RESET_PORT           ResetPort;
  EFI_ATA_PASS_THRU_RESET_DEVICE         ResetDevice;
};

extern EFI_GUID gEfiAtaPassThruProtocolGuid;

#endif /* _ATA_PASS_THRU_FOR_TEST_H_ */

