//###########################################################################
//
// FILE:    Digital_Filter_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220304
//###########################################################################



#include "Global_VariableDefs.h"


Digital_Filter_Module EXT_AD_LPF;

Digital_Filter_Module EXT_AD_DC_FILTER_LPF;




////////////////////////////////////////////////////////////////////
void Init_Digital_Filter_Module(void)
{
    Init_Digital_One_Order_LPF(&EXT_AD_LPF);
    Init_Digital_One_Order_LPF(&EXT_AD_DC_FILTER_LPF);

}

////////////////////////////////////////////////////////////////////
void Init_Digital_Filter_Module_Variables(void)
{

    EXT_AD_LPF.Cut_Off_Freq  = 1000.0;
    EXT_AD_LPF.damping_ratio = 0.707;

    EXT_AD_DC_FILTER_LPF.Cut_Off_Freq  = 1.0;
    EXT_AD_DC_FILTER_LPF.damping_ratio = 0.707;


}

////////////////////////////////////////////////////////////////////
//
// 一階LPF
// 將Xn, Xn-1, Yn清為0
// 預先計算C1, D1
//
void Init_Digital_One_Order_LPF(Digital_Filter_Module* point)
{
    float temp ;

    temp = ( point->Cut_Off_Freq * 2 * M_PI) / EXTAD_SAMPLE_FREQ;

    point->x_n = 0;
    point->y_n = 0;
    point->x_n_1 = 0;
    point->C1 = temp / ( temp + 2 );
    point->D1 = ( 2 - temp ) / ( 2 + temp );

}

////////////////////////////////////////////////////////////////////
//
// 引用一階數位濾波器
// 回傳yn
//
float Digital_One_Order_LPF_Cal(float input , Digital_Filter_Module* point)
{
    point->x_n = input;
    Digital_One_Order_LPF_Loop(point);

    return point->y_n;

}

////////////////////////////////////////////////////////////////////
//
// 一階數位濾波器運算
// 疊代運算yn及儲存xn
//
void Digital_One_Order_LPF_Loop(Digital_Filter_Module* point)
{
    point->y_n = ( point->C1 * ( point->x_n + point->x_n_1 ) ) + ( point->D1 * point->y_n ); //31 cycles
    point->x_n_1 = point->x_n;

}


////////////////////////////////////////////////////////////////////
//
// 二階LPF
// 將Xn, Xn-1, Xn-2, Yn, Yn-1, Yn-2清為0
// 預先計算C1, D1, E1
//
void Init_Digital_Two_Order_LPF(Digital_Filter_Module* point)
{
    float temp;

    temp = ( point->Cut_Off_Freq * 2 * M_PI ) / EXTAD_SAMPLE_FREQ;

    point->x_n_2 = 0;
    point->x_n_1 = 0;
    point->x_n = 0;

    point->y_n_2 = 0;
    point->y_n_1 = 0;
    point->y_n = 0;

    point->C1 = ( temp * temp ) / ( temp * temp + 4 * point->damping_ratio * temp + 4 );
    point->D1 = ( 2 * temp * temp - 8 ) / ( temp * temp + 4 * point->damping_ratio * temp + 4);
    point->E1 = ( temp * temp - 4 * point->damping_ratio * temp + 4 ) / ( temp * temp + 4 * point->damping_ratio * temp + 4);

}

////////////////////////////////////////////////////////////////////
//
// 引用二階數位濾波器
// 回傳yn
//
float Digital_Two_Order_LPF_Cal(float input , Digital_Filter_Module* point)
{
    point->x_n = input;
    Digital_Two_Order_LPF_Loop(point);

    return point->y_n;
}


////////////////////////////////////////////////////////////////////
//
// 二階數位濾波器運算
// 疊代計算yn及儲存xn
//
void Digital_Two_Order_LPF_Loop(Digital_Filter_Module* point)
{
    point->y_n = ( point->C1 * ( point->x_n + point->x_n_1 + point->x_n_1 + point->x_n_2 ) ) - ( point->D1 * point->y_n_1 ) - ( point->E1 * point->y_n_2 );
    point->x_n_2 = point->x_n_1;
    point->x_n_1 = point->x_n;
    point->y_n_2 = point->y_n_1;
    point->y_n_1 = point->y_n;

}




//
// End of file
//
