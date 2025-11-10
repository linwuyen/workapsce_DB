//###########################################################################
//
// FILE:    Digital_Filter_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220304
//###########################################################################


//M_PI

typedef struct Digital_Filter_Variable{      // A/D Converter Variable

 float Cut_Off_Freq;
 float damping_ratio;
 float C1;
 float D1;
 float E1;
 float x_n;
 float x_n_1;
 float x_n_2;
 float y_n;
 float y_n_1;
 float y_n_2;

}Digital_Filter_Module;


extern Digital_Filter_Module EXT_AD_LPF;
extern Digital_Filter_Module EXT_AD_DC_FILTER_LPF;



void Init_Digital_Filter_Module(void);
void Init_Digital_Filter_Module_Variables(void);

void Init_Digital_One_Order_LPF(Digital_Filter_Module* point);
float Digital_One_Order_LPF_Cal(float input , Digital_Filter_Module* point);
void Digital_One_Order_LPF_Loop(Digital_Filter_Module* point);

void Init_Digital_Two_Order_LPF(Digital_Filter_Module* point);
float Digital_Two_Order_LPF_Cal(float input , Digital_Filter_Module* point);
void Digital_Two_Order_LPF_Loop(Digital_Filter_Module* point);




//
// End of file
//
