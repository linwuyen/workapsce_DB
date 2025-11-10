
#include "common.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum {
    _EV_INIT_SHOW_VALUE          =  0x0000,
    _EV_PUSH_SHOW_VALUE_OUT      = (0x0001<<0),
    _EV_PUSH_RETURN_CHAR_OUT     = (0x0001<<1),
    _EV_PUSH_PROGRAM_EEPROM_FLAG = (0x0001<<2),
    _EV_VERIFY_EEPROM            = (0x0001<<3),
    _MASK_EERPROM_ERROR          =  0x8000
} FSM_EEPROM;

typedef struct {
    FSM_EEPROM fsm;

    uint32_t   u32SuccessCnt ;

    uint16_t   len;
    uint16_t   iTx;
    char       buffer[20]; // Make sure the buffer is large enough
} UART_CALBACK;