
MEMORY
{
PAGE 0 :  /* Program Memory */
          /* Memory (RAM/FLASH) blocks can be moved to PAGE1 for data allocation */
          /* BEGIN is used for the "boot to Flash" bootloader mode   */

   BEGIN           	: origin = 0x080000, length = 0x000002
   RAMM0           	: origin = 0x000123, length = 0x0002DD
   RAMD0           	: origin = 0x00B000, length = 0x000800

   RAMLS0_S2        : origin = 0x008000, length = 0x001800
//   RAMLS0          	: origin = 0x008000, length = 0x000800
//   RAMLS1          	: origin = 0x008800, length = 0x000800
//   RAMLS2      		: origin = 0x009000, length = 0x000800


   RAMGS0_S4        : origin = 0x00C000, length = 0x005000
//   RAMGS0      : origin = 0x00C000, length = 0x001000
//   RAMGS1      : origin = 0x00D000, length = 0x001000
//   RAMGS2      : origin = 0x00E000, length = 0x001000
//   RAMGS3      : origin = 0x00F000, length = 0x001000
//   RAMGS4      : origin = 0x010000, length = 0x001000


   RESET           	: origin = 0x3FFFC0, length = 0x000002

   /* Flash sectors */
   /* BANK 0 */
   FLASHA           : origin = 0x080002, length = 0x001FFE	/* on-chip Flash */
   FLASHBCDE           : origin = 0x082000, length = 0x008000	/* on-chip Flash .text*/
//   FLASHB           : origin = 0x082000, length = 0x002000	/* on-chip Flash */
//   FLASHC           : origin = 0x084000, length = 0x002000	/* on-chip Flash */
//   FLASHD           : origin = 0x086000, length = 0x002000	/* on-chip Flash */
//   FLASHE           : origin = 0x088000, length = 0x008000	/* on-chip Flash */
   FLASHF           : origin = 0x090000, length = 0x008000	/* on-chip Flash .switch*/
   FLASHG           : origin = 0x098000, length = 0x008000	/* on-chip Flash .TI.ramfunc*/
   FLASHH           : origin = 0x0A0000, length = 0x008000	/* on-chip Flash Cla1Prog*/
   FLASHI           : origin = 0x0A8000, length = 0x008000	/* on-chip Flash .const*/
   FLASHJ           : origin = 0x0B0000, length = 0x008000	/* on-chip Flash .cinit*/
   FLASHKLMN        : origin = 0x0B8000, length = 0x008000	/* on-chip Flash .init_array*/
//   FLASHK           : origin = 0x0B8000, length = 0x002000	/* on-chip Flash */
//   FLASHL           : origin = 0x0BA000, length = 0x002000	/* on-chip Flash */
//   FLASHM           : origin = 0x0BC000, length = 0x002000	/* on-chip Flash */
//   FLASHN           : origin = 0x0BE000, length = 0x002000	/* on-chip Flash */

   /* BANK 1 */
   FLASHO           : origin = 0x0C0000, length = 0x002000	/* on-chip Flash */
   FLASHPQRS        : origin = 0x0C2000, length = 0x008000	/* on-chip Flash .text*/
//   FLASHQ           : origin = 0x0C4000, length = 0x002000	/* on-chip Flash */
//   FLASHR           : origin = 0x0C6000, length = 0x002000	/* on-chip Flash */
//   FLASHS           : origin = 0x0C8000, length = 0x008000	/* on-chip Flash */
//   FLASHT           : origin = 0x0D0000, length = 0x008000	/* on-chip Flash */
   FLASHU           : origin = 0x0D8000, length = 0x008000	/* on-chip Flash .switch*/
   FLASHV           : origin = 0x0E0000, length = 0x008000	/* on-chip Flash .TI.ramfunc*/
   FLASHW           : origin = 0x0E8000, length = 0x008000	/* on-chip Flash Cla1Prog*/
   FLASHX           : origin = 0x0F0000, length = 0x008000	/* on-chip Flash .const*/
   FLASHY           : origin = 0x0F8000, length = 0x008000	/* on-chip Flash .init_array*/
//   FLASHY           : origin = 0x0F8000, length = 0x002000	/* on-chip Flash */
//   FLASHZ           : origin = 0x0FA000, length = 0x002000	/* on-chip Flash */
//   FLASHAA          : origin = 0x0FC000, length = 0x002000	/* on-chip Flash */
//   FLASHBB          : origin = 0x0FE000, length = 0x001FF0	/* on-chip Flash */

//   FLASHBB_RSVD     : origin = 0x0FFFF0, length = 0x000010    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

PAGE 1 : /* Data Memory */
         /* Memory (RAM/FLASH) blocks can be moved to PAGE0 for program allocation */

   BOOT_RSVD       : origin = 0x000002, length = 0x000121     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x0003F8     /* on-chip RAM block M1 */
//   RAMM1_RSVD      : origin = 0x0007F8, length = 0x000008     /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
   RAMD1           : origin = 0x00B800, length = 0x000800

   RAMLS3_S5      : origin = 0x009800, length = 0x001800
//   RAMLS3      : origin = 0x009800, length = 0x000800
//   RAMLS4      : origin = 0x00A000, length = 0x000800
//   RAMLS5      : origin = 0x00A800, length = 0x000800


   RAMGS5      : origin = 0x011000, length = 0x001000
   RAMGS6      : origin = 0x012000, length = 0x001000
   RAMGS7      : origin = 0x013000, length = 0x001000
   RAMGS8      : origin = 0x014000, length = 0x001000
   RAMGS9      : origin = 0x015000, length = 0x001000
   RAMGS10     : origin = 0x016000, length = 0x001000
//   RAMGS11_RSVD : origin = 0x017FF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   RAMGS11     : origin = 0x017000, length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS12     : origin = 0x018000, length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS13     : origin = 0x019000, length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

   CPU2TOCPU1RAM   : origin = 0x03F800, length = 0x000400
   CPU1TOCPU2RAM   : origin = 0x03FC00, length = 0x000400
   CLA1_MSGRAMLOW   : origin = 0x001480, length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x001500, length = 0x000080
}

SECTIONS
{
   codestart           : > BEGIN           PAGE = 0,   ALIGN(8)
   .reset              : > RESET,          PAGE = 0,   TYPE = DSECT /* not used, */
   .text               : > FLASHBCDE,      PAGE = 0,   ALIGN(8)
   .switch             : > FLASHF,         PAGE = 0,   ALIGN(8)
   .const              : > FLASHI,         PAGE = 0,   ALIGN(8)
   .cinit              : > FLASHJ,         PAGE = 0,   ALIGN(8)
   .pinit              : > FLASHJ,         PAGE = 0,   ALIGN(8)
   .init_array         : > FLASHY,         PAGE = 0,   ALIGN(8)


   .stack              : > RAMM1           PAGE = 1
   .bss                : > RAMGS5,         PAGE = 1
   .bss:output         : > RAMGS6,         PAGE = 1
   .bss:cio            : > RAMGS7,         PAGE = 1
   .data               : > RAMGS9,         PAGE = 1
   .sysmem             : > RAMGS11,        PAGE = 1

   .TI.ramfunc : {} LOAD = FLASHG,
                    RUN = RAMGS0_S4,
                    LOAD_START(RamfuncsLoadStart),
                    LOAD_SIZE(RamfuncsLoadSize),
                    LOAD_END(RamfuncsLoadEnd),
                    RUN_START(RamfuncsRunStart),
                    RUN_SIZE(RamfuncsRunSize),
                    RUN_END(RamfuncsRunEnd),
                    PAGE = 0, ALIGN(8)


                       /* CLA specific sections */
   Cla1Prog    :    LOAD = FLASHH,
                    RUN = RAMLS0_S2,
                    LOAD_START(Cla1ProgLoadStart),
                    RUN_START(Cla1ProgRunStart),
                    LOAD_SIZE(Cla1ProgLoadSize),
                    PAGE = 0, ALIGN(8)

   .scratchpad      : > RAMLS3_S5,           PAGE = 1
   .bss_cla         : > RAMLS3_S5,           PAGE = 1
   cla_shared       : > RAMLS3_S5,           PAGE = 1

   .const_cla  :    LOAD = FLASHH,
                    RUN = RAMLS0_S2,
                    RUN_START(Cla1ConstRunStart),
                    LOAD_START(Cla1ConstLoadStart),
                    LOAD_SIZE(Cla1ConstLoadSize),
                    PAGE = 0, ALIGN(8)

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
