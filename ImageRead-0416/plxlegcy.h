#ifndef __PLX_LEGACY_H
#define __PLX_LEGACY_H

/*******************************************************************************
 * Copyright (c) 2007 PLX Technology, Inc.
 * 
 * PLX Technology Inc. licenses this software under specific terms and
 * conditions.  Use of any of the software or derviatives thereof in any
 * product without a PLX Technology chip is strictly prohibited. 
 * 
 * PLX Technology, Inc. provides this software AS IS, WITHOUT ANY WARRANTY,
 * EXPRESS OR IMPLIED, INCLUDING, WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  PLX makes no guarantee
 * or representations regarding the use of, or the results of the use of,
 * the software and documentation in terms of correctness, accuracy,
 * reliability, currentness, or otherwise; and you rely on the software,
 * documentation and results solely at your own risk.
 * 
 * IN NO EVENT SHALL PLX BE LIABLE FOR ANY LOSS OF USE, LOSS OF BUSINESS,
 * LOSS OF PROFITS, INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES
 * OF ANY KIND.  IN NO EVENT SHALL PLX'S TOTAL LIABILITY EXCEED THE SUM
 * PAID TO PLX FOR THE PRODUCT LICENSED HEREUNDER.
 * 
 ******************************************************************************/

/******************************************************************************
 * 
 * File Name:
 *
 *      PlxLegcy.h
 *
 * Description:
 *
 *      This file includes legacy PLX types and API for applications that
 *      still use pre-PLX SDK v5.0 API.
 *
 * Revision:
 *
 *      01-01-07 : PLX SDK v5.00
 *
 * Note:
 *
 *      This file will eventually be removed from the PLX SDK.  Do not create
 *      any new applications that use the definitions or API in this file.
 *
 ******************************************************************************/


#include "PlxTypes.h"


#ifdef __cplusplus
extern "C" {
#endif




// DLL support
#ifndef EXPORT
    #if defined(PLX_MSWINDOWS)
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT
    #endif
#endif




/***********************************************
 * For portability to previous SDK versions.
 **********************************************/

// Type names that have changed
typedef PLX_PHYSICAL_MEM            PCI_MEMORY;
typedef PLX_ACCESS_TYPE             ACCESS_TYPE;
typedef PLX_EEPROM_TYPE             EEPROM_TYPE;
typedef PLX_DMA_COMMAND             DMA_COMMAND;
typedef PLX_DMA_PARAMS              DMA_TRANSFER_ELEMENT;


// Local Space Types
typedef enum _IOP_SPACE
{
    IopSpace0,
    IopSpace1,
    IopSpace2,
    IopSpace3
} IOP_SPACE;


// Mailbox ID Definitions
typedef enum _MAILBOX_ID
{
    MailBox0 = 0,
    MailBox1,
    MailBox2,
    MailBox3,
    MailBox4,
    MailBox5,
    MailBox6,
    MailBox7
} MAILBOX_ID;


// DMA Channel Definitions
typedef enum _DMA_CHANNEL
{
    IopChannel0,
    IopChannel1,
    IopChannel2,
    PrimaryPciChannel0,
    PrimaryPciChannel1,
    PrimaryPciChannel2,
    PrimaryPciChannel3
} DMA_CHANNEL;


// DMA Channel Priority Definitions
typedef enum _DMA_CHANNEL_PRIORITY
{
    Channel0Highest,
    Channel1Highest,
    Channel2Highest,
    Channel3Highest,
    Rotational
} DMA_CHANNEL_PRIORITY;


// Device Location Structure
typedef struct _DEVICE_LOCATION
{
    U8  BusNumber;
    U8  SlotNumber;
    U16 DeviceId;
    U16 VendorId;
    U8  SerialNumber[20];
} DEVICE_LOCATION;


// PLX Interrupt Structure 
typedef struct _PLX_INTR
{
    unsigned int InboundPost      :1;
    unsigned int OutboundPost     :1;
    unsigned int OutboundOverflow :1;
    unsigned int OutboundOption   :1;
    unsigned int IopDmaChannel0   :1;
    unsigned int PciDmaChannel0   :1;
    unsigned int IopDmaChannel1   :1;
    unsigned int PciDmaChannel1   :1;
    unsigned int IopDmaChannel2   :1;
    unsigned int PciDmaChannel2   :1;
    unsigned int Mailbox0         :1;
    unsigned int Mailbox1         :1;
    unsigned int Mailbox2         :1;
    unsigned int Mailbox3         :1;
    unsigned int Mailbox4         :1;
    unsigned int Mailbox5         :1;
    unsigned int Mailbox6         :1;
    unsigned int Mailbox7         :1;
    unsigned int IopDoorbell      :1;
    unsigned int PciDoorbell      :1;
    unsigned int BIST             :1;
    unsigned int PowerManagement  :1;
    unsigned int PciMainInt       :1;
    unsigned int IopToPciInt      :1;
    unsigned int IopMainInt       :1;
    unsigned int PciAbort         :1;
    unsigned int PciReset         :1;
    unsigned int PciPME           :1;
    unsigned int Enum             :1;
    unsigned int AbortLSERR       :1;
    unsigned int ParityLSERR      :1;
    unsigned int RetryAbort       :1;
    unsigned int LocalParityLSERR :1;
    unsigned int PciSERR          :1;
    unsigned int IopToPciInt_2    :1;
    unsigned int Message          :1;
    unsigned int SwInterrupt      :1;
    unsigned int SecResetDeassert :1;
    unsigned int SecPmeDeassert   :1;
    unsigned int GPIO14           :1;
    unsigned int GPIO4            :1;
} PLX_INTR;


// DMA Channel Descriptor Structure
typedef struct _DMA_CHANNEL_DESC
{
    unsigned int EnableReadyInput         :1;
    unsigned int EnableBTERMInput         :1;
    unsigned int EnableIopBurst           :1;
    unsigned int EnableWriteInvalidMode   :1;
    unsigned int EnableDmaEOTPin          :1;
    unsigned int DmaStopTransferMode      :1;
    unsigned int HoldIopAddrConst         :1;
    unsigned int DemandMode               :1;
    unsigned int EnableTransferCountClear :1;
    unsigned int WaitStates               :4;
    unsigned int IopBusWidth              :2;
    unsigned int EOTEndLink               :1;
    unsigned int ValidStopControl         :1;
    unsigned int ValidModeEnable          :1;
    unsigned int EnableDualAddressCycles  :1;
    unsigned int TholdForIopWrites        :4;
    unsigned int TholdForIopReads         :4;
    unsigned int TholdForPciWrites        :4;
    unsigned int TholdForPciReads         :4;
    DMA_CHANNEL_PRIORITY DmaChannelPriority;
} DMA_CHANNEL_DESC;




/*************************************************************************
 *
 *                LEGACY PLX PCI SDK API FUNCTIONS
 *
 *             -- Do NOT use for new applications! --
 *
 ************************************************************************/

#if !defined(PLX_DRIVER)

/******************************************
 * Miscellaneous Functions
 *****************************************/
RETURN_CODE EXPORT
PlxSdkVersion(
    U8 *VersionMajor,
    U8 *VersionMinor,
    U8 *VersionRevision
    );

RETURN_CODE EXPORT
PlxDriverVersion(
    HANDLE  hDevice,
    U8     *VersionMajor,
    U8     *VersionMinor,
    U8     *VersionRevision
    );

RETURN_CODE EXPORT
PlxChipTypeGet(
    HANDLE  hDevice,
    U32    *pChipType,
    U8     *pRevision
    );

VOID EXPORT
PlxPciBoardReset(
    HANDLE hDevice
    );

RETURN_CODE EXPORT
PlxPciCommonBufferProperties(
    HANDLE            hDevice,
    PLX_PHYSICAL_MEM *pMemoryInfo
    );

RETURN_CODE EXPORT
PlxPciCommonBufferMap(
    HANDLE  hDevice,
    VOID   *pVa
    );

RETURN_CODE EXPORT
PlxPciCommonBufferUnmap(
    HANDLE  hDevice,
    VOID   *pVa
    );

RETURN_CODE EXPORT
PlxPciPhysicalMemoryAllocate(
    HANDLE            hDevice,
    PLX_PHYSICAL_MEM *pMemoryInfo,
    BOOLEAN           bSmallerOk
    );

RETURN_CODE EXPORT
PlxPciPhysicalMemoryFree(
    HANDLE            hDevice,
    PLX_PHYSICAL_MEM *pMemoryInfo
    );

RETURN_CODE EXPORT
PlxPciPhysicalMemoryMap(
    HANDLE            hDevice,
    PLX_PHYSICAL_MEM *pMemoryInfo
    );

RETURN_CODE EXPORT
PlxPciPhysicalMemoryUnmap(
    HANDLE            hDevice,
    PLX_PHYSICAL_MEM *pMemoryInfo
    );


/******************************************
 * PLX Device Management Functions
 *****************************************/
RETURN_CODE EXPORT
PlxPciDeviceOpen(
    DEVICE_LOCATION *pDevice,
    HANDLE          *pHandle
    );

RETURN_CODE EXPORT
PlxPciDeviceClose(
    HANDLE hDevice
    );

RETURN_CODE EXPORT
PlxPciDeviceFind(
    DEVICE_LOCATION *pDevice,
    U32             *pRequestLimit
    );

RETURN_CODE EXPORT
PlxPciBarGet(
    HANDLE   hDevice,
    U8       BarIndex,
    U32     *pPciBar,
    BOOLEAN *pFlag_IsIoSpace
    );

RETURN_CODE EXPORT
PlxPciBarRangeGet(
    HANDLE  hDevice,
    U8      BarIndex,
    U32    *pData
    );

RETURN_CODE EXPORT
PlxPciBarMap(
    HANDLE  hDevice,
    U8      BarIndex,
    VOID   *pVa
    );

RETURN_CODE EXPORT
PlxPciBarUnmap(
    HANDLE  hDevice,
    VOID   *pVa
    );


/******************************************
 * Register Access Functions
 *****************************************/
U32 EXPORT
PlxPciConfigRegisterRead(
    U8           bus,
    U8           slot,
    U16          offset,
    RETURN_CODE *pReturnCode
    );

RETURN_CODE EXPORT
PlxPciConfigRegisterWrite(
    U8   bus,
    U8   slot,
    U16  offset,
    U32 *pValue
    );

U32 EXPORT
PlxPciRegisterReadFast(
    HANDLE       hDevice,
    U16          offset,
    RETURN_CODE *pReturnCode
    );

RETURN_CODE EXPORT
PlxPciRegisterWriteFast(
    HANDLE hDevice,
    U16    offset,
    U32    value
    );

U32 EXPORT
PlxPciRegisterRead_Unsupported(
    U8           bus,
    U8           slot,
    U16          offset,
    RETURN_CODE *pReturnCode
    );

RETURN_CODE EXPORT
PlxPciRegisterWrite_Unsupported(
    U8  bus,
    U8  slot,
    U16 offset,
    U32 value
    );

U32 EXPORT
PlxRegisterRead(
    HANDLE       hDevice,
    U16          offset,
    RETURN_CODE *pReturnCode
    );

RETURN_CODE EXPORT
PlxRegisterWrite(
    HANDLE hDevice,
    U16    offset,
    U32    value
    );

U32 EXPORT
PlxRegisterMailboxRead(
    HANDLE       hDevice,
    U8           MailboxId,
    RETURN_CODE *pReturnCode
    );

RETURN_CODE EXPORT
PlxRegisterMailboxWrite(
    HANDLE hDevice,
    U8     MailboxId,
    U32    value
    );


/******************************************
 * Interrupt Support Functions
 *****************************************/
RETURN_CODE EXPORT
PlxIntrEnable(
    HANDLE    hDevice,
    PLX_INTR *pPlxIntr
    );

RETURN_CODE EXPORT
PlxIntrDisable(
    HANDLE    hDevice,
    PLX_INTR *pPlxIntr
    );

RETURN_CODE EXPORT
PlxNotificationRegisterFor(
    HANDLE             hDevice,
    PLX_INTR          *pPlxIntr,
    PLX_NOTIFY_OBJECT *pEvent
    );

RETURN_CODE EXPORT
PlxNotificationWait(
    HANDLE             hDevice,
    PLX_NOTIFY_OBJECT *pEvent,
    U32                Timeout_ms
    );

RETURN_CODE EXPORT
PlxNotificationStatus(
    HANDLE             hDevice,
    PLX_NOTIFY_OBJECT *pEvent,
    PLX_INTR          *pPlxIntr
    );

RETURN_CODE EXPORT
PlxNotificationCancel(
    HANDLE             hDevice,
    PLX_NOTIFY_OBJECT *pEvent
    );


/******************************************
 * Bus Memory and I/O Functions
 *****************************************/
RETURN_CODE EXPORT
PlxBusIopRead(
    HANDLE       hDevice,
    IOP_SPACE    IopSpace,
    U32          address,
    BOOLEAN      bRemap,
    VOID        *pBuffer,
    U32          ByteCount,
    ACCESS_TYPE  AccessType
    );

RETURN_CODE EXPORT
PlxBusIopWrite(
    HANDLE       hDevice,
    IOP_SPACE    IopSpace,
    U32          address,
    BOOLEAN      bRemap,
    VOID        *pBuffer,
    U32          ByteCount,
    ACCESS_TYPE  AccessType
    );

RETURN_CODE EXPORT 
PlxIoPortRead(
    HANDLE       hDevice,
    U32          IoPort,
    ACCESS_TYPE  AccessType,
    VOID        *pValue
    );

RETURN_CODE EXPORT
PlxIoPortWrite(
    HANDLE       hDevice,
    U32          IoPort,
    ACCESS_TYPE  AccessType,
    VOID        *pValue
    );


/******************************************
 * Serial EEPROM Access Functions
 *****************************************/
BOOLEAN EXPORT 
PlxSerialEepromPresent(
    HANDLE       hDevice,
    RETURN_CODE *pReturnCode
    );

RETURN_CODE EXPORT
PlxSerialEepromReadByOffset(
    HANDLE  hDevice,
    U16     offset,
    U32    *pValue
    );

RETURN_CODE EXPORT
PlxSerialEepromWriteByOffset(
    HANDLE hDevice,
    U16    offset,
    U32    value
    );


/******************************************
 * DMA Functions
 *****************************************/
RETURN_CODE EXPORT
PlxDmaControl(
    HANDLE      hDevice,
    DMA_CHANNEL channel, 
    DMA_COMMAND command
    );

RETURN_CODE EXPORT
PlxDmaStatus(
    HANDLE      hDevice,
    DMA_CHANNEL channel
    );


/******************************************
 * Block DMA Functions
 *****************************************/
RETURN_CODE EXPORT
PlxDmaBlockChannelOpen(
    HANDLE            hDevice,
    DMA_CHANNEL       channel, 
    DMA_CHANNEL_DESC *pDesc
    );

RETURN_CODE EXPORT
PlxDmaBlockTransfer(
    HANDLE                hDevice,
    DMA_CHANNEL           channel,
    DMA_TRANSFER_ELEMENT *dmaData,
    BOOLEAN               returnImmediate
    );

RETURN_CODE EXPORT 
PlxDmaBlockChannelClose(
    HANDLE      hDevice,
    DMA_CHANNEL channel
    );


/******************************************
 * SGL DMA Functions
 *****************************************/
RETURN_CODE EXPORT
PlxDmaSglChannelOpen(
    HANDLE            hDevice,
    DMA_CHANNEL       channel, 
    DMA_CHANNEL_DESC *pDesc
    );

RETURN_CODE EXPORT
PlxDmaSglTransfer(
    HANDLE                hDevice,
    DMA_CHANNEL           channel,
    DMA_TRANSFER_ELEMENT *dmaData,
    BOOLEAN               returnImmediate
    );

RETURN_CODE EXPORT 
PlxDmaSglChannelClose(
    HANDLE      hDevice,
    DMA_CHANNEL channel
    );


/******************************************
 * Messaging Unit Functions
 *****************************************/
RETURN_CODE EXPORT
PlxMuInboundPortRead(
    HANDLE  hDevice,
    U32    *pFrame
    );

RETURN_CODE EXPORT
PlxMuInboundPortWrite(
    HANDLE  hDevice,
    U32    *pFrame
    );

RETURN_CODE EXPORT
PlxMuOutboundPortRead(
    HANDLE  hDevice,
    U32    *pFrame
    );

RETURN_CODE EXPORT
PlxMuOutboundPortWrite(
    HANDLE  hDevice,
    U32    *pFrame
    );


/******************************************
 * VPD Functions
 *****************************************/
U32 EXPORT 
PlxVpdRead(
    HANDLE       hDevice,
    U16          offset,
    RETURN_CODE *pReturnCode
    );

RETURN_CODE EXPORT
PlxVpdWrite(
    HANDLE hDevice,
    U16    offset,
    U32    value
    );

#endif   // PLX_DRIVER



#ifdef __cplusplus
}
#endif

#endif
