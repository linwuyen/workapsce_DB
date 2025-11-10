//###########################################################################
//
// FILE:    EXTIO_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210819
//###########################################################################


#define ASRAM_CS2_START_ADDR    0x100000
#define EXTIO_IN_ADDR           0x100000
//#define EXTIO_OUT_ADDR        0x100400
#define EXTIO_OUT_ADDR          0x100800



Uint16 Read_EXTIO_IN(void);
void Read_EXTIO_Process(void);

void Write_EXTIO_OUT(Uint16 data);







//
// End of file
//
