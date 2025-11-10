#include "ti_msp_dl_config.h"
#include "common.h"
#include "timetask.h"


/* Address in main memory to write to */
#define MAIN_BASE_ADDRESS (0x00004000)

/* The address space in memory containing the programmed ECC code */
#define ECC_CODE_ADDRESS_SPACE (0x41804000)

/* The address space in memory containing the ECC-corrected data */
#define CORRECTED_ADDRESS_SPACE (0x00004000)

/* The address space in memory containing the uncorrected data */
#define UNCORRECTED_ADDRESS_SPACE (0x00404000)

/*
 * Array to write 64-bits to flash. Data must be loaded 32-bits at a time,
 * but a single word program is executed.
 */
uint32_t gDataArray64[] = {0xABCDEF00, 0x12345678};

/* Array with "single-bit error" of gDataArray64 */
uint32_t gDataArray64SEC[] = {0xABCDEF01, 0x12345678};

/* Array with "double-bit error" of gDataArray64 */
uint32_t gDataArray64DED[] = {0xABCDEF11, 0x12345678};

volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;

volatile bool gDEDFault;

uint32_t* gECCCodeAddress     = (void*) ECC_CODE_ADDRESS_SPACE;
uint32_t* gUncorrectedAddress = (void*) UNCORRECTED_ADDRESS_SPACE;
uint32_t* gCorrectedAddress   = (void*) CORRECTED_ADDRESS_SPACE;
uint32_t gUncorrectedData __attribute__((unused));
uint32_t gCorrectedData __attribute__((unused));
uint32_t gFaultStatus __attribute__((unused));
uint8_t gECCCode;

typedef enum{
    _NO_TEST_ACTION = 0,
    _WRITE_CORRECTED_DATA_TO_FLASH = (0x00000001<<0),
    _WRITE_UNCORRECTED_SINGLE_BIT_DATA_TO_FLASH = (0x00000001<<1),
    _ERASE_ADDRESS  = (0x00000001<<12),
    _CLEAR_ECC_STATUS  = (0x00000001<<13),
    _ERASE_FAIL  = (0x00000001<<14),
    _MARK_ERROR_FLAG = (0x80000000)
}FSM_TEST;

FSM_TEST evTest = _NO_TEST_ACTION;

/*
 *  Helper function to perform erase of sector at the specified
 *  address. Function will block if command does not pass.
 *
 *  address   The address of sector to erase
 */
void FlashCtl_eraseSectorBlocking(uint32_t address)
{
    /* Erase memory before programming */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, address, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        FG_SET(_MARK_ERROR_FLAG|_ERASE_FAIL, evTest);
    }
}

void writeCorrectedDataToFlash(void)
{
    FlashCtl_eraseSectorBlocking(MAIN_BASE_ADDRESS);

    /*
     * Program single flash word (64-bit data) with ECC enabled.
     * The flash controller hardware will handle generating the 8-bit ECC code.
     */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64[0]);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {

        FG_SET(_MARK_ERROR_FLAG, evTest);
    }
    else {
        FG_RST(_WRITE_CORRECTED_DATA_TO_FLASH, evTest);
    }
}

void writeUncorrectedSingleBitErrorDataToFlash(void)
{
    /* Get the 8-bit ECC code that was generated for this 64-bit data */
    gECCCode = *(gECCCodeAddress);

    FlashCtl_eraseSectorBlocking(MAIN_BASE_ADDRESS);

    /*
     * Enable override of hardware generated ECC. This allows the user to
     * manually set the ECC code to be programmed.
     */
    DL_FlashCTL_enableOverrideHardwareGeneratedECC(FLASHCTL);

    /*
     * Demonstrate a SEC (single bit error correction) error.
     * Program 64-bit data with ECC enabled, but the data in gDataArray64SEC
     * has a one bit difference from the data in gDataArray64.
     * Additionally, the ECC code being programmed corresponds with the data
     * in gDataArray64. Since we are using the ECC code corresponding with
     * gDataArray64, and gDataArray64SEC has a bit difference in the data,
     * this will trigger a SEC error.
     */
    DL_SYSCTL_clearECCErrorStatus();
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64WithECCManual(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64SEC[0], &gECCCode);

    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {

        FG_SET(_MARK_ERROR_FLAG, evTest);
    }
    else {
        FG_RST(_WRITE_UNCORRECTED_SINGLE_BIT_DATA_TO_FLASH, evTest);
    }
}


void task25msec(void * s)
{
    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);
}

void task2D5msec(void * s)
{
    switch(evTest){
    case _WRITE_CORRECTED_DATA_TO_FLASH:
        writeCorrectedDataToFlash();
        break;
    case _WRITE_UNCORRECTED_SINGLE_BIT_DATA_TO_FLASH:
        writeUncorrectedSingleBitErrorDataToFlash();
        break;
    case _ERASE_ADDRESS:
        FlashCtl_eraseSectorBlocking(MAIN_BASE_ADDRESS);
        FG_RST(_ERASE_ADDRESS|_MARK_ERROR_FLAG, evTest);
        break;
    case _CLEAR_ECC_STATUS:
        DL_SYSCTL_clearECCErrorStatus();
        FG_RST(_CLEAR_ECC_STATUS|_MARK_ERROR_FLAG, evTest);
        break;
    default:
        if( FG_GET(_MARK_ERROR_FLAG, evTest)) {
            /* If command did not pass, set a SW breakpoint. */
            __BKPT(0);
        }
        break;
    }


}


uint16_t id_ttask = 0;
ST_TIMETASK time_task[] = {
        {task2D5msec,         0,   T_2D5MS},
        {task25msec,          0,   T_25MS},
        {0, 0, 0}
};


int main(void)
{
    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    /* Default: LED1 and LED3 ON, LED2 OFF */
    //DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);
    //DL_GPIO_setPins(GPIO_LEDS_PORT,GPIO_LEDS_USER_TEST_PIN);

    while (1) {
        scanTimeTask(&time_task[id_ttask++], (void *)0);
        if(0 == time_task[id_ttask].fn) id_ttask = 0;
    }
}
