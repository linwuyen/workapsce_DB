//###########################################################################
//
// FILE:    28377S_FLASH_lnk.cmd
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210826
//###########################################################################


/*-------------------- for CLA use --------------------*/
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are.
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
/*-------------------- for CLA use --------------------*/


MEMORY
{
PAGE 0 :  /* Program Memory */
          /* Memory (RAM/FLASH) blocks can be moved to PAGE1 for data allocation */
          /* BEGIN is used for the "boot to Flash" bootloader mode   */

   BEGIN           	: origin = 0x080000, length = 0x000002
   RAMM0           	: origin = 0x000122, length = 0x0002DE
   RAMD0           	: origin = 0x00B000, length = 0x000800

   RAMLS3      		: origin = 0x009800, length = 0x000800
   //RAMLS4      		: origin = 0x00A000, length = 0x000800
   //RAMLS5         	: origin = 0x00A800, length = 0x000800
   RAMLS4_LS5     		: origin = 0x00A000, length = 0x001000

   //RAMGS14     		: origin = 0x01A000, length = 0x001000
   RAMGS15     		: origin = 0x01B000, length = 0x001000
   RESET           	: origin = 0x3FFFC0, length = 0x000002
   
   /* Flash sectors */
   FLASHA           : origin = 0x080002, length = 0x001FFE	/* on-chip Flash */
   FLASHB           : origin = 0x082000, length = 0x002000	/* on-chip Flash */
   FLASHC           : origin = 0x084000, length = 0x002000	/* on-chip Flash */
   FLASHD           : origin = 0x086000, length = 0x002000	/* on-chip Flash */
   FLASHE           : origin = 0x088000, length = 0x008000	/* on-chip Flash */
   FLASHF           : origin = 0x090000, length = 0x008000	/* on-chip Flash */
   FLASHG           : origin = 0x098000, length = 0x008000	/* on-chip Flash */
   FLASHH           : origin = 0x0A0000, length = 0x008000	/* on-chip Flash */
   FLASHI           : origin = 0x0A8000, length = 0x008000	/* on-chip Flash */
   FLASHJ           : origin = 0x0B0000, length = 0x008000	/* on-chip Flash */
   FLASHK           : origin = 0x0B8000, length = 0x002000	/* on-chip Flash */
   FLASHL           : origin = 0x0BA000, length = 0x002000	/* on-chip Flash */
   FLASHM           : origin = 0x0BC000, length = 0x002000	/* on-chip Flash */
   FLASHN           : origin = 0x0BE000, length = 0x002000	/* on-chip Flash */
   FLASHO           : origin = 0x0C0000, length = 0x002000	/* on-chip Flash */
   FLASHP           : origin = 0x0C2000, length = 0x002000	/* on-chip Flash */
   FLASHQ           : origin = 0x0C4000, length = 0x002000	/* on-chip Flash */   
   FLASHR           : origin = 0x0C6000, length = 0x002000	/* on-chip Flash */
   FLASHS           : origin = 0x0C8000, length = 0x008000	/* on-chip Flash */
   FLASHT           : origin = 0x0D0000, length = 0x008000	/* on-chip Flash */   
   FLASHU           : origin = 0x0D8000, length = 0x008000	/* on-chip Flash */   
   FLASHV           : origin = 0x0E0000, length = 0x008000	/* on-chip Flash */   
   FLASHW           : origin = 0x0E8000, length = 0x008000	/* on-chip Flash */   
   FLASHX           : origin = 0x0F0000, length = 0x008000	/* on-chip Flash */
   FLASHY           : origin = 0x0F8000, length = 0x002000	/* on-chip Flash */
   FLASHZ           : origin = 0x0FA000, length = 0x002000	/* on-chip Flash */
   FLASHAA          : origin = 0x0FC000, length = 0x002000	/* on-chip Flash */   
   FLASHAB          : origin = 0x0FE000, length = 0x002000	/* on-chip Flash */   

PAGE 1 : /* Data Memory */
         /* Memory (RAM/FLASH) blocks can be moved to PAGE0 for program allocation */

   BOOT_RSVD       : origin = 0x000002, length = 0x000120     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
   RAMD1           : origin = 0x00B800, length = 0x000800

   RAMLS0          	: origin = 0x008000, length = 0x000800
   RAMLS1          	: origin = 0x008800, length = 0x000800
   RAMLS2      		: origin = 0x009000, length = 0x000800

   RAMGS0      : origin = 0x00C000, length = 0x001000
   RAMGS1      : origin = 0x00D000, length = 0x001000
   RAMGS2      : origin = 0x00E000, length = 0x001000
   RAMGS3      : origin = 0x00F000, length = 0x001000
   RAMGS4      : origin = 0x010000, length = 0x001000

   RAMGS_5_TO_10  : origin = 0x011000, length = 0x006000
   //RAMGS6      : origin = 0x012000, length = 0x001000
   //RAMGS7      : origin = 0x013000, length = 0x001000
   //RAMGS8      : origin = 0x014000, length = 0x001000
   //RAMGS9      : origin = 0x015000, length = 0x001000
   //RAMGS10     : origin = 0x016000, length = 0x001000
   RAMGS11     : origin = 0x017000, length = 0x001000
   RAMGS12     : origin = 0x018000, length = 0x001000
   RAMGS13     : origin = 0x019000, length = 0x001000

   RAMGS14     		: origin = 0x01A000, length = 0x001000

   EMIF1_CS0n      : origin = 0x80000000, length = 0x10000000
   EMIF1_CS2n      : origin = 0x00100000, length = 0x00200000
   EMIF1_CS3n      : origin = 0x00300000, length = 0x00080000
   EMIF1_CS4n      : origin = 0x00380000, length = 0x00060000
   
   /*-------------------- for CLA use --------------------*/
   CLA1_MSGRAMLOW   : origin = 0x001480, length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x001500, length = 0x000080
   /*-------------------- for CLA use --------------------*/


}


SECTIONS
{
   /* Allocate program areas: */
   .cinit              : > FLASHB      PAGE = 0, ALIGN(4)
   .pinit              : > FLASHB,     PAGE = 0, ALIGN(4)
   .text               : >> FLASHB | FLASHC | FLASHD | FLASHE | FLASHF | FLASHG     PAGE = 0, ALIGN(4)
   codestart           : > BEGIN       PAGE = 0, ALIGN(4)
						 
   /* Allocate uninitalized data sections: */
   .stack              : > RAMM1        PAGE = 1
   .ebss               : >> RAMLS2 | RAMGS0 | RAMGS1 | RAMGS14      PAGE = 1
   .esysmem            : > RAMLS2       PAGE = 1

   /* Initalized sections go in Flash */
   .econst             : >> FLASHF | FLASHG | FLASHH      PAGE = 0, ALIGN(4)
   .switch             : > FLASHB      PAGE = 0, ALIGN(4)
   
   .reset              : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */

   .farbss          : > EMIF1_CS0n, PAGE = 1
   .farconst        : > EMIF1_CS0n, PAGE = 1

   /* For project use */
   ramgs0           : > RAMGS0,     PAGE = 1  // for SDRAM test
   ramgs1           : > RAMGS1,     PAGE = 1  // V raw
   ramgs2           : > RAMGS2,     PAGE = 1  // I raw
   ramgs3           : > RAMGS3,     PAGE = 1
   ramgs4           : > RAMGS4,     PAGE = 1
   ramgs5_to_10     : > RAMGS_5_TO_10,     PAGE = 1 //WAVE buf
   ramgs11           : > RAMGS11,     PAGE = 1
   ramgs12           : > RAMGS12,     PAGE = 1
   ramgs13           : > RAMGS13,     PAGE = 1


   /*-------------------- for CLA use --------------------*/
   /* CLA specific sections */
   Cla1Prog         : LOAD = FLASHI,
                      //RUN = RAMLS5,
                      RUN = RAMLS4_LS5,
                      LOAD_START(_Cla1funcsLoadStart),
                      LOAD_END(_Cla1funcsLoadEnd),
                      RUN_START(_Cla1funcsRunStart),
                      LOAD_SIZE(_Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(4)

   CLADataLS0		: > RAMLS0, PAGE=1
   CLADataLS1		: > RAMLS1, PAGE=1

   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW,   PAGE = 1
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH,  PAGE = 1

   /* CLA C compiler sections */
   //
   // Must be allocated to memory the CLA has write access to
   //
   CLAscratch       :
                     { *.obj(CLAscratch)
                     . += CLA_SCRATCHPAD_SIZE;
                     *.obj(CLAscratch_end) } >  RAMLS1,  PAGE = 1

   .scratchpad      : > RAMLS1,       PAGE = 1
   .bss_cla		    : > RAMLS1,       PAGE = 1
   .const_cla	    :  LOAD = FLASHB,
                       RUN = RAMLS1,
                       RUN_START(_Cla1ConstRunStart),
                       LOAD_START(_Cla1ConstLoadStart),
                       LOAD_SIZE(_Cla1ConstLoadSize),
                       PAGE = 1
   /*-------------------- for CLA use --------------------*/

#ifdef __TI_COMPILER_VERSION__
   #if __TI_COMPILER_VERSION__ >= 15009000
    .TI.ramfunc : {} LOAD = FLASHD,
                         RUN = RAMLS3,
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_SIZE(_RamfuncsLoadSize),
                         LOAD_END(_RamfuncsLoadEnd),
                         RUN_START(_RamfuncsRunStart),
                         RUN_SIZE(_RamfuncsRunSize),
                         RUN_END(_RamfuncsRunEnd),
                         PAGE = 0, ALIGN(4)
   #else
   ramfuncs            : LOAD = FLASHD,
                         RUN = RAMLS3,
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_SIZE(_RamfuncsLoadSize),
                         LOAD_END(_RamfuncsLoadEnd),
                         RUN_START(_RamfuncsRunStart),
                         RUN_SIZE(_RamfuncsRunSize),
                         RUN_END(_RamfuncsRunEnd),
                         PAGE = 0, ALIGN(4)
   #endif
#endif


}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
