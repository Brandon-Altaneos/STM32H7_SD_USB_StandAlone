#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define LOG_ENABLED
#define DISABLE_WATCH_DOG
#undef SPI_CPU_DEBUG
#undef BYPASS_COM_RESET
#undef BYPASS_COM_KEEP_ALIVE
#undef BUZZER_OFF
#undef PARAM_FORCE_RESET
#undef BYPASS_MOTOR_ENGAGE
#define DISPLAY_DEBUG_INFO
#undef CEM_MODE
#undef DISABLE_TOUCHGFX

// function to use for USB → USBDMSC
#undef SD_DISKIO
#define USBDMSC

#ifdef __INTELLISENSE__
    #define PROCPU
    #define DATCPU
    #define DATE "00000000"
    #define TIME "000000"
    typedef __uint32_t uint32_t ;
#endif

#include "stm32h753xx.h"
#include "stm32h7xx_hal.h"
#include "core_cm7.h"

#include "main.h"

#include "errors.h"
#include "log.h"
#include "utils.h"

typedef long unsigned int       uint32_t;

//===========================================================================
//==                              MACROS APP                               ==
//===========================================================================

#define RAM_MAGIC_REG			0
#define RAM_MAGIC_VALUE			0x4186056C
#define RAM_SLEEP_REG			1
#define RAM_SLEEP_BIT			0
#define RAM_PRESLEEP_BIT		1

#define APP_START_BOOT_ADDR 	0x08000000
#define APP_START_ADDR 			0x08040000
#define APP_FWINFO_ADDR			0x0813FFC0
#define APP_UID_ADDR			0x08140000
#define APP_END_ADDR			(APP_UID_ADDR-2)

#define UID_SIZE					62
#define UID_PATTERN_BEGIN 			"(1)" //  TODO UID

// Unit testing

#undef UNIT_TEST_FSM // TODO Unit testing

#if defined(UNIT_TEST_FSM)
#define UNIT_TEST_ENABLED
#endif

#ifndef BOOTLOADER

// Version
#ifndef PROCPU
#ifndef DATCPU
#error please define PROCPU or DATCPU
#endif // DATCPU
#endif

#ifdef PROCPU
#define FW_NAME                  "ECMPRO"             		 ///< Firmware Name
#define FW_VERSION_MAJOR         "00"                         ///< Firmware Version Major
#define FW_VERSION_MINOR         "00"                         ///< Firmware Version Minor
#define FW_VERSION_BUILD         "01"                         ///< Firmware Version Build
#define FW_VERSION_MAJOR_INT     0                            ///< Firmware Version Major
#define FW_VERSION_MINOR_INT     0                            ///< Firmware Version Minor
#define FW_VERSION_BUILD_INT     1                            ///< Firmware Version Build
#endif

#ifdef DATCPU
#define FW_NAME                  "ECMDAT"             		 ///< Firmware Name
#define FW_VERSION_MAJOR         "00"                         ///< Firmware Version Major
#define FW_VERSION_MINOR         "00"                         ///< Firmware Version Minor
#define FW_VERSION_BUILD         "01"                         ///< Firmware Version Build
#endif

#define FW_DATE                  __DATE__ " " __TIME__       ///< Firmware Version Date
#define FW_VERSION              FW_VERSION_MAJOR "." FW_VERSION_MINOR "." FW_VERSION_BUILD
#define FW_VERSION_ID	        FW_NAME "_" FW_VERSION      ///< Firmware Version String

#define FW_VERSION_SIZE     9              // 00.00.00

extern const uint8_t GLOBAL_szFwInfo[64];							///< Initialized at build
extern const uint8_t GLOBAL_szUID[64];

#define HW_NAME                  "ECM_DEVICE"                ///< Hardware Name
#define HW_VERSION               "1"                         ///< Hardware Version

#define HW_VERSION_STRING	    HW_NAME "_v" HW_VERSION ///< Hardware Version String


//===========================================================================
//==                         MACROS BOOTLOADER                             ==
//===========================================================================
#else // BOOTLOADER

#undef BYPASS_CHECK_FW

extern const uint8_t GLOBAL_szFwInfo[64];							///< Initialized at build
extern const uint8_t GLOBAL_szUID[64];

                        ///< Firmware Version Build

#endif

#ifdef PROCPU
#define BOOTLOADER_VERSION_MAJOR         "00"                         ///< Bootloader Version Major
#define BOOTLOADER_VERSION_MINOR         "00"                         ///< Bootloader Version Minor
#define BOOTLOADER_VERSION_BUILD         "00"                         ///< Bootloader Version Build
#define BOOTLOADER_VERSION	BOOTLOADER_VERSION_MAJOR "." BOOTLOADER_VERSION_MINOR "." BOOTLOADER_VERSION_BUILD

#define BOOT_VERSION_MAJOR_INT     1                            ///< Firmware Version Major
#define BOOT_VERSION_MINOR_INT     0                            ///< Firmware Version Minor
#define BOOT_VERSION_BUILD_INT     0    
#endif

#ifdef DATCPU
#define BOOTLOADER_VERSION_MAJOR         "00"                         ///< Bootloader Version Major
#define BOOTLOADER_VERSION_MINOR         "00"                         ///< Bootloader Version Minor
#define BOOTLOADER_VERSION_BUILD         "00"                         ///< Bootloader Version Build
#define BOOTLOADER_VERSION	BOOTLOADER_VERSION_MAJOR "." BOOTLOADER_VERSION_MINOR "." BOOTLOADER_VERSION_BUILD

#define BOOT_VERSION_MAJOR_INT     1                            ///< Firmware Version Major
#define BOOT_VERSION_MINOR_INT     0                            ///< Firmware Version Minor
#define BOOT_VERSION_BUILD_INT     0    

#endif


//===========================================================================
//==                            PUBLIC DATA TYPES                          ==
//===========================================================================

//===========================================================================
//==                              GLOBAL DATA                              ==
//===========================================================================

//===========================================================================
//==                          CLASSES DECLARATION                          ==
//===========================================================================

#ifndef NOP
#define NOP()               do { asm volatile ("nop"); } while (0)
#endif

typedef void (*pFunction)(void);

#ifndef FUNC_PTR
#define FUNC_PTR void *
#endif

#define RAM_D1_NOCACHE __attribute__((section(".ram_d1_nocache"), aligned(32)))
#define DMA_BUFFER __attribute__((section(".dma_buffer"))) __attribute__((aligned(32)))
#define DMA_BUFFER_SRAM4 __attribute__((section(".dma_buffer_sram4"))) __attribute__((aligned(32)))
#define SDRAM_BUFFER __attribute__((section(".sdram_buffer"), aligned(32)))

#ifdef __cplusplus
}
#endif

#endif  // GLOBALS_H
