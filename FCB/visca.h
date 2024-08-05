
/*----------------------------------------
FCB Camera command and function prototypes
------------------------------------------*/

#ifndef VISCA_H
#define VISCA_H 


//COMMAND LIST
    // Camera Information
    extern unsigned char cam_versionINQ[];// cam version
    extern unsigned char baud_rate[]; // cam baud rate 
    
   // Camera
     //Camera Function
       //Power
       extern unsigned char power_on[6];
       extern unsigned char power_off[6];
       
       //Reset
       extern unsigned char reset[6];
       //Mute
       extern unsigned char mute_on[6];
       extern unsigned char mute_off[6];       
       //Lens Initialize
       extern unsigned char lens_init[6];
       //reverse
       extern unsigned char lr_reverse_on[6];
       extern unsigned char lr_reverse_off[6];
       //freeze
       extern unsigned char freeze_on[6];
       extern unsigned char freeze_off[6];   
    
       //monochrome
       extern unsigned char monochrome_on[6];
       extern unsigned char monochrome_off[6]; 
       
       //flip
       extern unsigned char picflip_on[6];
       extern unsigned char picflip_off[6];
       

    // Motion detection and frames 
    extern unsigned char motion_on[6];
    extern unsigned char motion_off[6];
    extern unsigned char function_set_displayON[11];
    extern unsigned char function_set_displayOFF[11];
    extern unsigned char window_set[4][10];
    //Camera zoom
    extern unsigned char var_tele[6];
    extern  unsigned char var_wide[6];
    
    //Camera D-zoom
    extern unsigned char dZoom_on[6];
    extern unsigned char dZoom_off[6];
    extern unsigned char combine[6];
    extern unsigned char separate[6];
    extern unsigned char var_Dtele[6];//zoom in to the middle of the screen fast
    extern unsigned char var_Dwide[6];//zoom out of the middle of the screen fast
    extern unsigned char stop_Dzoom[6]; 
    
    // Focus
    extern unsigned char stop_focus[6]; 
    extern unsigned char far_focus[6]; 
    extern unsigned char near_focus[6]; 
    extern unsigned char auto_focus[6]; 
    extern unsigned char manual_focus[6]; 
    extern unsigned char onePush[6]; 
    extern unsigned char fullScan[6]; 
     
    //Auto focus 
    extern unsigned char normalAF[6];
    extern unsigned char intervalAF[6];
    extern unsigned char zoom_triggerAF[6];
    
    //Spot Focus 
    extern unsigned char spot_focus_on[6];
    extern unsigned char spot_focus_off[6];
    extern unsigned char spot_focus_center[10];
    extern unsigned char spot_focus_topL[10];
    extern unsigned char spot_focus_topR[10];
    extern unsigned char spot_focus_bottomL[10];
    extern unsigned char spot_focus_bottomR[10];
    extern unsigned char spot_focus_displayON[6];
   extern  unsigned char spot_focus_displayOFF[6];
    
    //Auto Exposure
    extern unsigned char ae_full_auto[6];
    extern unsigned char ae_manual[6];

    
    //Shutter
   extern  unsigned char shutter_reset[6]; 
   extern  unsigned char shutter_inc[6];
    extern unsigned char shutter_dec[6];
   //unsigned char shutter_direct[];
    
    //iris
   extern  unsigned char iris_reset[6]; 
   extern  unsigned char iris_inc[6];
   extern  unsigned char iris_dec[6];
    
    //gain 
    extern unsigned char gain_reset[6]; 
    extern unsigned char gain_inc[6];
    extern unsigned char gain_dec[6]; 
    
   //Priorities
   extern unsigned char shutter_priority[];
   extern unsigned char iris_priority[];
   
   // White Balance
   extern unsigned char wb_auto[6];
   extern unsigned char wb_indoor[6];
   extern unsigned char wb_outdoor[6];
   extern unsigned char wb_onepush_wb[6];
   extern unsigned char wb_atw[6];
   extern unsigned char wb_manual[6];
   extern unsigned char wb_onepush_trigger[6];
   extern unsigned char wb_outdoor_auto[6];
   extern unsigned char wb_sodlamp_auto[6];
   extern unsigned char wb_sodlamp[6];
   extern unsigned char wb_sodlamp_out_auto[6];

    
   // WB Manual
   extern unsigned char rgain_reset[6];
   extern unsigned char bgain_reset[6];
   extern unsigned char rgain_inc[6];
   extern unsigned char bgain_inc[6];
   extern unsigned char rgain_dec[6];
   extern unsigned char bgain_dec[6];
   
    //Continous
    extern unsigned char contZoomOn[6];
    extern unsigned char contZoomOff[6];
    extern unsigned char contFocusOn[6];
    extern unsigned char contFocusOff[6];
    
	//wide d / ve
	extern unsigned char wideD_on[6];
	extern unsigned char ve_on[6];
	extern unsigned char wideVE_off[6];
	extern unsigned char wideVE_param[13];
	extern unsigned char wideD_paramINQ[6];
    extern unsigned char ve_paramINQ[6];
    
    //Defog
    extern unsigned char defog_on[7];
    extern unsigned char defog_off[7];
    extern unsigned char defogINQ[];
    
    //ICR
    extern unsigned char icr_on[6]; 
	extern unsigned char icr_off[6];
	extern unsigned char autoicr_on[6];
	extern unsigned char autoicr_color[6];
	extern unsigned char autoicr_off[6];

	//HLC
	extern unsigned char hlc_param[7];
	extern unsigned char hlc_paramINQ[5];
	
	//Stabilizer
	extern unsigned char stab_on[6];
	extern unsigned char stab_off[6];
	extern unsigned char stab_hold[6];
	extern unsigned char stab_level[7];
	extern unsigned char stab_modeINQ[5];
	extern unsigned char stab_levelINQ[6];
	
	//Aperture
	extern unsigned char ap_reset[6];
	extern unsigned char ap_inc[6];
	extern unsigned char ap_dec[6];
	extern unsigned char apINQ[5];
	
	//Noise Reduction

	// Noise Reduction
	extern unsigned char nr_param[6];
	extern unsigned char nr_ind[7];
	extern unsigned char nrINQ[5];
	extern unsigned char nrindINQ[5];
	
	//gamma
	// Gamma 
	extern unsigned char gamma_param[6];
	extern unsigned char gamma_offset[11];
    
    // Extended
    extern unsigned char exApe_on[8];//81 01 04 24 5F 00 02 FF
	extern unsigned char exApe_off[10];//81 01 04 1F 42 00 00 0B 04 FF
	extern unsigned char exApe_reset[8];//8x 01 04 1F 02 00 00 FF
	extern unsigned char exApe_inc[8];//8x 01 04 1F 02 02 pp 
	extern unsigned char exApe_dec[8];//8x 01 04 1F 02 02 pp FF
   
   //Function Prototypes
   void camera_information(int fd);
   void camera(int fd);
   void lens(int fd);
   void expwb(int fd); 
   void spotFocus(int fd);
   void motionDetect(int fd);
   void iq(int fd);
   void extend(int fd);
    
    
   
    
  
#endif
