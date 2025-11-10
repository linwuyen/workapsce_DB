#include "dl_flashctrl.h"

DL_FLASHCTL_COMMAND_STATUS FlashAPIState;

void DL_FlashCTL_unprotectSector(FMC_T *flashctl)
{
		/* Unlock protected registers */
    SYS_UnlockReg();	
		FMC_Open();
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64(
    FMC_T *flashctl, uint32_t address, uint32_t *data)
{
	  //FMC_Write(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * Current_Valid_Page) + Current_Cursor, temp);
		FMC_Write(address,    data[0]);
	  FMC_Write(address+4,  data[1]);
	
		FMC_Close();
    SYS_LockReg();
	
		return DL_FLASHCTL_COMMAND_STATUS_PASSED;
	
}


DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseMemory(
    FMC_T *flashctl, uint32_t address)
{
		int32_t s32FlashStatus = 0;
		//FMC_Erase(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * Current_Valid_Page));	
	  s32FlashStatus += FMC_Erase(address);
	  s32FlashStatus += FMC_Erase(address+512);
	
		FMC_Close();
		SYS_LockReg();
	
		return (0 <= s32FlashStatus? DL_FLASHCTL_COMMAND_STATUS_PASSED: DL_FLASHCTL_COMMAND_STATUS_FAILED);
}

