// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
// Project Properties -> C2000 Linker -> Advanced Options -> Command File 
// Preprocessing -> --define
#ifdef CLA_C
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are. 
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C

MEMORY
{
PAGE 0 :  /* Program Memory */
          /* Memory (RAM/FLASH) blocks can be moved to PAGE1 for data allocation */
          /* BEGIN is used for the "boot to Flash" bootloader mode   */

   BEGIN           	: origin = 0x080000, length = 0x000002
   RAMM0           	: origin = 0x000123, length = 0x0002DD
   RAMD0           	: origin = 0x00B000, length = 0x000800

   RAMLS01          : origin = 0x008000, length = 0x001000    // CPU1 Ram Function
//   RAMLS0          	: origin = 0x008000, length = 0x000800    // CPU1 Ram Function
//   RAMLS1          	: origin = 0x008800, length = 0x000800    // CPU1 Ram Function

   RAMLS34          : origin = 0x009800, length = 0x001000    // CLA Ram Function
//   RAMLS3      	    : origin = 0x009800, length = 0x000800    // CLA Ram Function
//   RAMLS4      	    : origin = 0x00A000, length = 0x000800    // CLA Ram Function

   RAMGS0           : origin = 0x00C000, length = 0x001000
   RAMGS1           : origin = 0x00D000, length = 0x001000

   /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS14          : origin = 0x01A000, length = 0x001000     // .fapi_ram for CPU1


//   RAMGS15_RSVD     : origin = 0x01BFF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   RESET           	: origin = 0x3FFFC0, length = 0x000002

   /* Flash sectors */
   FLASHA           : origin = 0x080002, length = 0x001FFE	/* on-chip Flash */ // .cinit  .pinit
   FLASHB           : origin = 0x082000, length = 0x002000	/* on-chip Flash */ // .text
   FLASHC           : origin = 0x084000, length = 0x002000	/* on-chip Flash */ // .text
   FLASHD           : origin = 0x086000, length = 0x002000	/* on-chip Flash */ // .text, .TI.ramfunc
   FLASHE           : origin = 0x088000, length = 0x008000	/* on-chip Flash */ // .switch
   FLASHF           : origin = 0x090000, length = 0x008000	/* on-chip Flash */ // .init_array
   FLASHG           : origin = 0x098000, length = 0x008000	/* on-chip Flash */ // .const
   FLASHH           : origin = 0x0A0000, length = 0x008000	/* on-chip Flash */ // Cla1Prog
   FLASHI           : origin = 0x0A8000, length = 0x008000	/* on-chip Flash */
   FLASHJ           : origin = 0x0B0000, length = 0x008000	/* on-chip Flash */
   FLASHK           : origin = 0x0B8000, length = 0x002000	/* on-chip Flash */
   FLASHL           : origin = 0x0BA000, length = 0x002000	/* on-chip Flash */ // .fapi_ram
   FLASHM           : origin = 0x0BC000, length = 0x002000	/* on-chip Flash */ // C28_STORE_ADDRESS
   FLASHN           : origin = 0x0BE000, length = 0x001FF0	/* on-chip Flash */

//   FLASHN_RSVD     : origin = 0x0BFFF0, length = 0x000010    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

PAGE 1 : /* Data Memory */
         /* Memory (RAM/FLASH) blocks can be moved to PAGE0 for program allocation */

   BOOT_RSVD       : origin = 0x000002, length = 0x000121   /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x0003F8   /* on-chip RAM block M1 */
//   RAMM1_RSVD      : origin = 0x0007F8, length = 0x000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
   RAMD1           : origin = 0x00B800, length = 0x000800



   RAMLS2      	   : origin = 0x009000, length = 0x000800    // CPU1 Ram Data

   RAMLS5          : origin = 0x00A800, length = 0x000800    // CLA Ram Data
   RAMGS2          : origin = 0x00E000, length = 0x001000
   RAMGS3          : origin = 0x00F000, length = 0x001000
   RAMGS4          : origin = 0x010000, length = 0x001000

//   RAMGS10     : origin = 0x016000, length = 0x001000
//   RAMGS11     : origin = 0x017000, length = 0x000FF8   /* Uncomment for F28374D, F28376D devices */
//   RAMGS11_RSVD : origin = 0x017FF8, length = 0x000008  /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
//   RAMGS11     : origin = 0x017000, length = 0x001000   /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS10_11  : origin = 0x016000, length = 0x002000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

//   RAMGS12     : origin = 0x018000, length = 0x001000   /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
//   RAMGS13     : origin = 0x019000, length = 0x001000   /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS12_13  : origin = 0x018000, length = 0x002000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

   CLA1_MSGRAMLOW   : origin = 0x001480, length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x001500, length = 0x000080

   CPU2TOCPU1RAM   : origin = 0x03F800, length = 0x000400
   CPU1TOCPU2RAM   : origin = 0x03FC00, length = 0x000400
}

SECTIONS
{
   /* Allocate program areas: */
   .cinit              : > FLASHA      PAGE = 0,    ALIGN(8)
   .pinit              : > FLASHA      PAGE = 0,    ALIGN(8)
   .text               : >> FLASHB | FLASHC | FLASHD      PAGE = 0, ALIGN(8)
   codestart           : > BEGIN       PAGE = 0,    ALIGN(8)
   /* Allocate uninitalized data sections: */
   .stack              : > RAMM1       PAGE = 1
   .switch             : > FLASHE      PAGE = 0,    ALIGN(8)
   .reset              : > RESET,      PAGE = 0, TYPE = DSECT /* not used, */

   .init_array         : > FLASHF,     PAGE = 0,    ALIGN(8)
   .bss                : > RAMLS2,     PAGE = 1
   .bss:output         : > RAMLS2,     PAGE = 1
   .bss:cio            : > RAMLS2,     PAGE = 1
   .data               : > RAMLS2,     PAGE = 1
   .sysmem             : > RAMLS2,     PAGE = 1
   /* Initalized sections go in Flash */
   .const              : > FLASHG,     PAGE = 0,    ALIGN(8)

    .TI.ramfunc : {}
      LOAD = FLASHD,
      RUN = RAMLS01,
      LOAD_START(RamfuncsLoadStart),
      LOAD_SIZE(RamfuncsLoadSize),
      LOAD_END(RamfuncsLoadEnd),
      RUN_START(RamfuncsRunStart),
      RUN_SIZE(RamfuncsRunSize),
      RUN_END(RamfuncsRunEnd),
      PAGE = 0, ALIGN(8)


	RD_CPU2         : > RAMGS10_11,           PAGE = 1
    RW_CPU1         : > RAMGS12_13,           PAGE = 1

    MSGRAM_CPU1_TO_CPU2 : > CPU1TOCPU2RAM, type=NOINIT
    MSGRAM_CPU2_TO_CPU1 : > CPU2TOCPU1RAM, type=NOINIT

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
