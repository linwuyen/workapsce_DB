/*
 * dl_flashctrl.h
 *
 *  Created on: 2023¦~9¤ë13¤é
 *      Author: cody_chen
 */

#ifndef DL_FLASHCTRL_H_
#define DL_FLASHCTRL_H_


#include "common.h"

/* FLASHCTL_STATCMD Bits */
/* FLASHCTL_STATCMD[CMDDONE] Bits */
#define FLASHCTL_STATCMD_CMDDONE_OFS             (0)                             /* !< CMDDONE Offset */
#define FLASHCTL_STATCMD_CMDDONE_MASK            ((uint32_t)0x00000001U)         /* !< Command Done */
#define FLASHCTL_STATCMD_CMDDONE_STATNOTDONE     ((uint32_t)0x00000000U)         /* !< Not Done */
#define FLASHCTL_STATCMD_CMDDONE_STATDONE        ((uint32_t)0x00000001U)         /* !< Done */
/* FLASHCTL_STATCMD[CMDPASS] Bits */
#define FLASHCTL_STATCMD_CMDPASS_OFS             (1)                             /* !< CMDPASS Offset */
#define FLASHCTL_STATCMD_CMDPASS_MASK            ((uint32_t)0x00000002U)         /* !< Command Pass - valid when CMD_DONE
                                                                                    field is 1 */
#define FLASHCTL_STATCMD_CMDPASS_STATFAIL        ((uint32_t)0x00000000U)         /* !< Fail */
#define FLASHCTL_STATCMD_CMDPASS_STATPASS        ((uint32_t)0x00000002U)         /* !< Pass */
/* FLASHCTL_STATCMD[CMDINPROGRESS] Bits */
#define FLASHCTL_STATCMD_CMDINPROGRESS_OFS       (2)                             /* !< CMDINPROGRESS Offset */
#define FLASHCTL_STATCMD_CMDINPROGRESS_MASK      ((uint32_t)0x00000004U)         /* !< Command In Progress */
#define FLASHCTL_STATCMD_CMDINPROGRESS_STATCOMPLETE ((uint32_t)0x00000000U)         /* !< Complete */
#define FLASHCTL_STATCMD_CMDINPROGRESS_STATINPROGRESS ((uint32_t)0x00000004U)         /* !< In Progress */
/* FLASHCTL_STATCMD[FAILWEPROT] Bits */
#define FLASHCTL_STATCMD_FAILWEPROT_OFS          (4)                             /* !< FAILWEPROT Offset */
#define FLASHCTL_STATCMD_FAILWEPROT_MASK         ((uint32_t)0x00000010U)         /* !< Command failed due to Write/Erase
                                                                                    Protect Sector Violation */
#define FLASHCTL_STATCMD_FAILWEPROT_STATNOFAIL   ((uint32_t)0x00000000U)         /* !< No Fail */
#define FLASHCTL_STATCMD_FAILWEPROT_STATFAIL     ((uint32_t)0x00000010U)         /* !< Fail */
/* FLASHCTL_STATCMD[FAILVERIFY] Bits */
#define FLASHCTL_STATCMD_FAILVERIFY_OFS          (5)                             /* !< FAILVERIFY Offset */
#define FLASHCTL_STATCMD_FAILVERIFY_MASK         ((uint32_t)0x00000020U)         /* !< Command failed due to verify error */
#define FLASHCTL_STATCMD_FAILVERIFY_STATNOFAIL   ((uint32_t)0x00000000U)         /* !< No Fail */
#define FLASHCTL_STATCMD_FAILVERIFY_STATFAIL     ((uint32_t)0x00000020U)         /* !< Fail */
/* FLASHCTL_STATCMD[FAILMISC] Bits */
#define FLASHCTL_STATCMD_FAILMISC_OFS            (12)                            /* !< FAILMISC Offset */
#define FLASHCTL_STATCMD_FAILMISC_MASK           ((uint32_t)0x00001000U)         /* !< Command failed due to error other
                                                                                    than write/erase protect violation or
                                                                                    verify error.  This is an extra bit
                                                                                    in case a new failure mechanism is
                                                                                    added which requires a status bit. */
#define FLASHCTL_STATCMD_FAILMISC_STATNOFAIL     ((uint32_t)0x00000000U)         /* !< No Fail */
#define FLASHCTL_STATCMD_FAILMISC_STATFAIL       ((uint32_t)0x00001000U)         /* !< Fail */
/* FLASHCTL_STATCMD[FAILILLADDR] Bits */
#define FLASHCTL_STATCMD_FAILILLADDR_OFS         (6)                             /* !< FAILILLADDR Offset */
#define FLASHCTL_STATCMD_FAILILLADDR_MASK        ((uint32_t)0x00000040U)         /* !< Command failed due to the use of an
                                                                                    illegal address */
#define FLASHCTL_STATCMD_FAILILLADDR_STATNOFAIL  ((uint32_t)0x00000000U)         /* !< No Fail */
#define FLASHCTL_STATCMD_FAILILLADDR_STATFAIL    ((uint32_t)0x00000040U)         /* !< Fail */
/* FLASHCTL_STATCMD[FAILMODE] Bits */
#define FLASHCTL_STATCMD_FAILMODE_OFS            (7)                             /* !< FAILMODE Offset */
#define FLASHCTL_STATCMD_FAILMODE_MASK           ((uint32_t)0x00000080U)         /* !< Command failed because a bank has
                                                                                    been set to a mode other than READ.
                                                                                    Program and Erase commands cannot be
                                                                                    initiated unless all banks are in
                                                                                    READ mode. */
#define FLASHCTL_STATCMD_FAILMODE_STATNOFAIL     ((uint32_t)0x00000000U)         /* !< No Fail */
#define FLASHCTL_STATCMD_FAILMODE_STATFAIL       ((uint32_t)0x00000080U)         /* !< Fail */
/* FLASHCTL_STATCMD[FAILINVDATA] Bits */
#define FLASHCTL_STATCMD_FAILINVDATA_OFS         (8)                             /* !< FAILINVDATA Offset */
#define FLASHCTL_STATCMD_FAILINVDATA_MASK        ((uint32_t)0x00000100U)         /* !< Program command failed because an
                                                                                    attempt was made to program a stored
                                                                                    0 value to a 1. */
#define FLASHCTL_STATCMD_FAILINVDATA_STATNOFAIL  ((uint32_t)0x00000000U)         /* !< No Fail */
#define FLASHCTL_STATCMD_FAILINVDATA_STATFAIL    ((uint32_t)0x00000100U)         /* !< Fail */

/*! @enum DL_FLASHCTL_COMMAND_STATUS */
typedef enum {
    /*! Flash Command passed */
    DL_FLASHCTL_COMMAND_STATUS_PASSED = (FLASHCTL_STATCMD_CMDDONE_STATDONE |
                                         FLASHCTL_STATCMD_CMDPASS_STATPASS),
    /*! Flash Command failed */
    DL_FLASHCTL_COMMAND_STATUS_FAILED = (FLASHCTL_STATCMD_CMDDONE_STATDONE |
                                         FLASHCTL_STATCMD_CMDPASS_STATFAIL),
    /*! Flash Command is still in progress */
    DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS =
        (FLASHCTL_STATCMD_CMDINPROGRESS_STATINPROGRESS)

} DL_FLASHCTL_COMMAND_STATUS;

extern void DL_FlashCTL_unprotectSector(FMC_T *flashctl);

extern DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64(
    FMC_T *flashctl, uint32_t address, uint32_t *data);

extern DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseMemory(
    FMC_T *flashctl, uint32_t address);

#endif /* DL_FLASHCTRL_H_ */
