//###########################################################################
//
// FILE:    SCI_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210915
//###########################################################################

#include "Global_VariableDefs.h"


SCI_MODULE_VARIABLES_REG SCI_Module_Variables;


/////////////////////////////////////////////////////////////////////
void SCIC_TXRX_Process(void)
{

    switch( SCI_Module_Variables.SCIC_TxRx_State )
    {

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case SCIC_READY:
        {
            Uint16 checksum, i;

            checksum = 0;

            if ( SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig )
            {
                //
                // RS485 TX Trig
                //

                SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 0;


                //
                // 計算CheckSum及更新
                //
                for ( i = 0; i < 14; i ++ )
                {
                    checksum = checksum + SCI_Module_Variables.SCIC_Tx_Data[ i ];
                }
                SCI_Module_Variables.SCIC_Tx_Data[ 14 ] = ( checksum >> 8 ) & 0xFF;
                SCI_Module_Variables.SCIC_Tx_Data[ 15 ] = checksum & 0xFF;

                //
                // 20221230-2
                //
                // 設定輸出IO PAR_ENA = 0
                // 讓下一台機器可以接收資料
                //
                //PAR_ENA_0;

                //
                // 20221230-2
                // 傳送RS485不設定PAR_ENA, 改成設定GPIO66 TXC_RXC
                //
                TXC_RXC_1;


                SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy = 1;

                //
                // 轉態
                //
                SCI_Module_Variables.SCIC_TxRx_State = SCIC_TX_LOAD_BUFFER;

                break;
            }
            else if ( SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_RX_Trig )
            {
                //
                // RS485 RX Trig
                //

                SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_RX_Trig = 0;


                //
                // 計算CheckSum及更新
                //
                for ( i = 0; i < 14; i ++ )
                {
                    checksum = checksum + SCI_Module_Variables.SCIC_Rx_Data_Shadow[ i ];
                }

                //
                // 比對CheckSum
                //
                if ( ( SCI_Module_Variables.SCIC_Rx_Data_Shadow[ 14 ] != ( ( checksum >> 8 ) & 0xFF ) )
                     || ( SCI_Module_Variables.SCIC_Rx_Data_Shadow[ 15 ] != ( checksum & 0xFF ) ) )
                {
                    SCI_Module_Variables.MS_SCI_RX_NG_Count ++;
                    SCI_Module_Variables.MS_SCI_RX_TOTAL_Count ++;

                    ScicRegs.SCICTL1.bit.SWRESET = 0;       // Reset SCI
                    ScicRegs.SCIFFTX.bit.TXFIFORESET = 0;   // Reset TX FIFO
                    ScicRegs.SCIFFTX.bit.TXFIFORESET = 1;
                    ScicRegs.SCICTL1.bit.SWRESET = 1;

                    //
                    // SCI CheckSum NG
                    //

                    if ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
                    {
                        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                    }
                    else
                    {
                        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
                    }

                    Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CHECKSUM_NG;

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

                }
                else
                {
                    SCI_Module_Variables.MS_SCI_RX_OK_Count ++;
                    SCI_Module_Variables.MS_SCI_RX_TOTAL_Count ++;

                    //
                    // 更新RX Buffer
                    //
                    for ( i = 0; i < 16; i ++ )
                    {
                        SCI_Module_Variables.SCIC_Rx_Data[ i ] = SCI_Module_Variables.SCIC_Rx_Data_Shadow[ i ];
                    }

                    SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 1;
                }
            }

            //
            // RX異常發生, 直接重置SCI
            //
            if ( ScicRegs.SCIRXST.bit.RXERROR )
            {
                ScicRegs.SCICTL1.bit.SWRESET = 0;       // Reset SCI
                ScicRegs.SCIFFTX.bit.TXFIFORESET = 0;   // Reset TX FIFO
                ScicRegs.SCIFFTX.bit.TXFIFORESET = 1;
                ScicRegs.SCICTL1.bit.SWRESET = 1;
            }

            break;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case SCIC_TX_LOAD_BUFFER:
        {
            Uint16 i;

            //TXC_RXC_1;

            for ( i = 0; i < 16; i ++ )
            {
                ScicRegs.SCITXBUF.all = SCI_Module_Variables.SCIC_Tx_Data[ i ];
            }

            SCI_Module_Variables.SCIC_TX_Start_Time = CpuTimer0.InterruptCount;

            //
            // 轉態
            //
            SCI_Module_Variables.SCIC_TxRx_State = SCIC_TX_BUSY;

            break;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case SCIC_TX_BUSY:
        {
            if ( Calc_Elapsed_Time( SCI_Module_Variables.SCIC_TX_Start_Time, CpuTimer0.InterruptCount ) > SCIC_TX_HOLD_COUNT )
            {
                //
                // 20221230-3
                // 傳送資料結束, 改設定恢復TXC_RXC
                //
                TXC_RXC_0;

                //
                // 20221230-3
                //
                //PAR_ENA_1;

                SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy = 0;


                //
                // 轉態
                //
                SCI_Module_Variables.SCIC_TxRx_State = SCIC_READY;
            }


            break;
        }


    }


}





/////////////////////////////////////////////////////////////////////
//寫入SCI TXBUF char
void scia_TXwrite_char(char *pointer, Uint16 length)
{
    Uint16 i;

    //DELAY_US(10000);

    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {;}

    for(i=0;i<length;i++)
    {
        FANFAIL1_start_time_reset();
        FANFAIL2_start_time_reset();
        PFC_UVP_start_time_reset();
        PFC_OVP_start_time_reset();
        PFC_OTP_start_time_reset();
        ACFAIL_start_time_reset();
        DCAC_OTP_start_time_reset();
        p_I_peak_ocp_start_time_reset();
        n_I_peak_ocp_start_time_reset();
        //p_I_short_ocp_start_time_reset();
        //n_I_short_ocp_start_time_reset();
        I_rms_ocp_start_time_reset();
        V_rms_ovp_start_time_reset();
        opp_start_time_reset();
        remote_OVP_start_time_reset();

        FANFAIL_start_time_reset(); //for 全寬機種
        PFC_UVP1_start_time_reset(); //for 全寬機種
        PFC_OVP1_start_time_reset(); //for 全寬機種
        PFC_UVP2_start_time_reset(); //for 全寬機種
        PFC_OVP2_start_time_reset(); //for 全寬機種

        DC_OVP_start_time_reset();


        if(SciaRegs.SCIFFTX.bit.TXFFST<=15)
        {
            SciaRegs.SCITXBUF.all = *pointer;
            pointer++;
        }
        else
        {
            i--;
        }
    }
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {;}
}



/////////////////////////////////////////////////////////////////////
//
// scia_msg - Transmit message via SCIA
//
void SCI_A_Send_Message(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        SCI_A_Transmit(msg[i]);
        i++;
    }
}




/////////////////////////////////////////////////////////////////////
void SCI_A_Transmit(int data)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}

    SciaRegs.SCITXBUF.all = data;
}







/////////////////////////////////////////////////////////////////////
void Init_SCI_Module_Variables(void)
{
    Uint16 i;

    SCI_Module_Variables.Receive_Char = 0;
    SCI_Module_Variables.msg = "";

    for ( i = 0; i < 16; i ++ )
    {
        SCI_Module_Variables.SCIC_Tx_Data[i] = 160 + i;
        SCI_Module_Variables.SCIC_Rx_Data[i] = 0;
        SCI_Module_Variables.SCIC_Rx_Data_Shadow[i] = 0;
    }

    SCI_Module_Variables.MS_SCI_RX_NG_Count = 0;
    SCI_Module_Variables.MS_SCI_RX_OK_Count = 0;
    SCI_Module_Variables.MS_SCI_RX_TOTAL_Count = 0;

    SCI_Module_Variables.SCI_Module_Status_flag.all = 0;

    SCI_Module_Variables.SCI_Module_Control_flag.all = 0;

    SCI_Module_Variables.SCIC_TxRx_State = SCIC_READY;

    SCI_Module_Variables.SCIC_TX_Start_Time = 0;

    for ( i = 0; i < 100; i ++ )
    {
        SCI_Module_Variables.Debug_Msg[i] = 0;
        SCI_Module_Variables.SCIA_Rx_Data[i] = 0;
        SCI_Module_Variables.SCIA_Rx_Data_Count = 0;
    }



    //
    // 20221230-4 預設RS485 IC接收
    //
    TXC_RXC_0;
}






/////////////////////////////////////////////////////////////////////
void Init_SCI_A_GPIO(void)
{
    //
    // TX
    //
    GPIO_SetupPinOptions(65, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 6);

    //
    // RX
    //
    GPIO_SetupPinOptions(64, GPIO_INPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 6);

}

/////////////////////////////////////////////////////////////////////
void Init_SCI_A_Reg(void)
{
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.SCIA_RX_INT = &scia_rx_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers

    SciaRegs.SCICTL1.bit.SWRESET = 0;       // Reset SCI

    SciaRegs.SCIFFTX.bit.SCIRST = 0;        // Reset SCI
    SciaRegs.SCIFFTX.bit.SCIRST = 1;
    SciaRegs.SCIFFTX.bit.SCIFFENA = 1;      // Enable FIFO
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 0;   // Reset TX FIFO
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 1;
    SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;    // Clear TX FIFO INT
    SciaRegs.SCIFFTX.bit.TXFFIENA = 0;      // Disable TX FIFO INT
    SciaRegs.SCIFFTX.bit.TXFFIL = 0;        // TX FIFO interrupt level


    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;    // Clear RX FIFO OVF
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;   // Reset RX FIFO
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;    // Clear RX FIFO INT
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;      // Enable RX FIFO INT
    SciaRegs.SCIFFRX.bit.RXFFIL = 1;       // RX FIFO interrupt level

    SciaRegs.SCIFFCT.all = 0x0;

    SciaRegs.SCICCR.bit.STOPBITS = 0;       // Stop bit: 1
    SciaRegs.SCICCR.bit.PARITY = 0;
    SciaRegs.SCICCR.bit.PARITYENA = 0;      // No Parity
    SciaRegs.SCICCR.bit.LOOPBKENA = 0;      // Loop back disable
    SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0;  // Idle-line mode (Normal communication)
    SciaRegs.SCICCR.bit.SCICHAR = 7;        // Char bits: 8


    //
    // BRR = LSPCLK / ( SCI Asynchronous Baud x 8 ) - 1
    // LSPCLK = 200 MHz ( LSPCLKDIV = 0 )
    //
    // Example. BRR = 200000000 / ( 9600 x 8 ) - 1 = 2603 = 0A2C(Hex)
    //
    //
    // SCIA at 115200 baud
    //
    SciaRegs.SCIHBAUD.all = 0x0000;
    SciaRegs.SCILBAUD.all = 0x00D8;

    SciaRegs.SCICTL1.bit.RXERRINTENA = 0;   // Disable RX ERR INT
    SciaRegs.SCICTL1.bit.TXWAKE = 0;
    SciaRegs.SCICTL1.bit.SLEEP = 0;
    SciaRegs.SCICTL1.bit.TXENA = 1;         // Enable TX
    SciaRegs.SCICTL1.bit.RXENA = 1;         // Enable RX

    SciaRegs.SCICTL2.bit.RXBKINTENA = 0;    // Disable RXRDY/BRKDT interrupt
    SciaRegs.SCICTL2.bit.TXINTENA = 0;      // Disable TXRDY interrupt

    //
    // Re-enable SCI
    //
    SciaRegs.SCICTL1.bit.SWRESET = 1;

    //
    // Enable interrupts
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;   // PIE Group 9, INT1

    //
    // Set CPU IER
    //
    IER |= M_INT9;

}



/////////////////////////////////////////////////////////////////////
//
// 多機連線使用 (RS485)
//
void Init_SCI_C_GPIO(void)
{
    //
    // TX
    //
    GPIO_SetupPinOptions(63, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(63, GPIO_MUX_CPU1, 1);

    //
    // RX
    //
    GPIO_SetupPinOptions(62, GPIO_INPUT, GPIO_PULLUP | GPIO_ASYNC);
    GPIO_SetupPinMux(62, GPIO_MUX_CPU1, 1);

    //
    // TXC_RXC
    // RS485 Direction Control
    //
    GPIO_SetupPinOptions(66, GPIO_OUTPUT, GPIO_PUSHPULL);

}



/////////////////////////////////////////////////////////////////////
//
// 多機連線使用 (RS485)
//
void Init_SCI_C_Reg(void)
{
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.SCIC_RX_INT = &scic_rx_isr;
    //PieVectTable.SCIC_TX_INT = &scic_tx_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers

    ScicRegs.SCICTL1.bit.SWRESET = 0;       // Reset SCI


    ScicRegs.SCIFFTX.bit.SCIRST = 0;        // Reset SCI
    ScicRegs.SCIFFTX.bit.SCIRST = 1;
    ScicRegs.SCIFFTX.bit.SCIFFENA = 1;      // Enable FIFO
    ScicRegs.SCIFFTX.bit.TXFIFORESET = 0;   // Reset TX FIFO
    ScicRegs.SCIFFTX.bit.TXFIFORESET = 1;
    ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;    // Clear TX FIFO INT
    ScicRegs.SCIFFTX.bit.TXFFIENA = 0;      // Disable TX FIFO INT
    ScicRegs.SCIFFTX.bit.TXFFIL = 0;        // TX FIFO interrupt level

    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;    // Clear RX FIFO OVF
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;   // Reset RX FIFO
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
    ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;    // Clear RX FIFO INT
    ScicRegs.SCIFFRX.bit.RXFFIENA = 1;      // Enable RX FIFO INT
    ScicRegs.SCIFFRX.bit.RXFFIL = 16;       // RX FIFO interrupt level

    ScicRegs.SCIFFCT.all = 0x0;

    ScicRegs.SCICCR.bit.STOPBITS = 0;       // Stop bit: 1
    ScicRegs.SCICCR.bit.PARITY = 0;
    ScicRegs.SCICCR.bit.PARITYENA = 0;      // No Parity
    ScicRegs.SCICCR.bit.LOOPBKENA = 0;      // Loop back disable
    ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;  // Idle-line mode (Normal communication)
    ScicRegs.SCICCR.bit.SCICHAR = 7;        // Char bits: 8

    //
    // BRR = LSPCLK / ( SCI Asynchronous Baud x 8 ) - 1
    // LSPCLK = 200 MHz ( LSPCLKDIV = 0 )
    //
    // Example. BRR = 200000000 / ( 9600 x 8 ) - 1 = 2603 = 0A2C(Hex)
    //
    ScicRegs.SCIHBAUD.all = 0x0000;
    ScicRegs.SCILBAUD.all = 0x0018;


    //ScicRegs.SCICTL1.bit.RXERRINTENA = 1; // Enable RX ERR INT
    ScicRegs.SCICTL1.bit.RXERRINTENA = 0;   // Disable RX ERR INT
    ScicRegs.SCICTL1.bit.TXWAKE = 0;
    ScicRegs.SCICTL1.bit.SLEEP = 0;
    ScicRegs.SCICTL1.bit.TXENA = 1;         // Enable TX
    ScicRegs.SCICTL1.bit.RXENA = 1;         // Enable RX

    ScicRegs.SCICTL2.bit.RXBKINTENA = 0;    // Disable RXRDY/BRKDT interrupt
    ScicRegs.SCICTL2.bit.TXINTENA = 0;      // Disable TXRDY interrupt


    //
    // Re-enable SCI
    //
    ScicRegs.SCICTL1.bit.SWRESET = 1;


    //
    // Enable interrupts
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER8.bit.INTx5 = 1;   // PIE Group 8, INT5
    //PieCtrlRegs.PIEIER8.bit.INTx6 = 1;   // PIE Group 8, INT6

    //
    // Set CPU IER
    //
    IER |= M_INT8;

}


////////////////////////////////////////////////////////////////////
//
// SCI C TX Interrupt ISR
//
/*
__interrupt void scic_tx_isr(void)
{

    asm("       NOP");                    // Wait one cycle


    //ScicRegs.SCIFFTX.bit.TXFFINTCLR=1;   // Clear SCI Interrupt flag


    //
    // Acknowledge this interrupt to receive more interrupts from group 8
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

}
*/

////////////////////////////////////////////////////////////////////
//
// SCI C RX Interrupt ISR
//
__interrupt void scic_rx_isr(void)
{
    Uint16 i;

    for ( i = 0; i < 16; i ++ )
    {
        SCI_Module_Variables.SCIC_Rx_Data_Shadow[i] = ScicRegs.SCIRXBUF.all;
    }

    SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_RX_Trig = 1;


    ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    ScicRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    //
    // Acknowledge this interrupt to receive more interrupts from group 8
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

}


////////////////////////////////////////////////////////////////////
//
// SCI A RX Interrupt ISR
//
__interrupt void scia_rx_isr(void)
{
    SCI_Module_Variables.SCIA_Rx_Data[ SCI_Module_Variables.SCIA_Rx_Data_Count ] = SciaRegs.SCIRXBUF.bit.SAR;

    SCI_Module_Variables.SCIA_Rx_Data_Count ++;


    if ( SCI_Module_Variables.SCIA_Rx_Data[ SCI_Module_Variables.SCIA_Rx_Data_Count - 1 ] == '\r' )
    {
        Event_Module.Event2_Flag.bit.SCIA_ask_service = 1;
    }
    else if ( (Uint16)SCI_Module_Variables.SCIA_Rx_Data[ SCI_Module_Variables.SCIA_Rx_Data_Count - 1 ] == 127 )
    {
        //
        // 觸發echo
        //
        SCI_Module_Variables.SCIA_echo_Data[0] = SCI_Module_Variables.SCIA_Rx_Data[ SCI_Module_Variables.SCIA_Rx_Data_Count - 1 ];
        Event_Module.Event_Flag.bit.SCIA_echo = 1;

        SCI_Module_Variables.SCIA_Rx_Data_Count = SCI_Module_Variables.SCIA_Rx_Data_Count - 2;
    }
    else
    {
        //
        // 觸發echo
        //
        SCI_Module_Variables.SCIA_echo_Data[0] = SCI_Module_Variables.SCIA_Rx_Data[ SCI_Module_Variables.SCIA_Rx_Data_Count - 1 ];
        Event_Module.Event_Flag.bit.SCIA_echo = 1;
    }

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    //
    // Acknowledge this interrupt to receive more interrupts from group 8
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

}


//
// End of file
//
