#ifdef SUPPORT_CD4051_PONTEN
extern void delay_2ms(int cnt);





AT(.volatile_ram_code)
void cd4051_ad_check(void)
{
	static u8 cd4051_cur_ch;

#ifdef SUPPORT_CD4051_TEST
/*
 000		X0	EQ3 VR3	  
 100		X1	EQ4 VR2   
 010		X2	EQ5 VR1   
 110		X3	EQ2 VR4
 001		X4	EQ1 VR5   
 101		X5			
 011		X6			 
 111		X7

*/

	switch(use_ctl.bCd4051TestVal){
		
		case 0:
			CD4051_A_L();CD4051_B_L();CD4051_C_L();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;
		case 1:
			CD4051_A_H();CD4051_B_L();CD4051_C_L();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;
		case 2:
			CD4051_A_L();CD4051_B_H();CD4051_C_L();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;
		case 3:
			CD4051_A_H();CD4051_B_H();CD4051_C_L();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;
		case 4:
			CD4051_A_L();CD4051_B_L();CD4051_C_H();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;
		case 5:
			CD4051_A_H();CD4051_B_L();CD4051_C_H();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;

		case 6:
			CD4051_A_L();CD4051_B_H();CD4051_C_H();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;
		case 7:
			CD4051_A_H();CD4051_B_H();CD4051_C_H();			
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			break;
			
		default:	
			break;

	}

  //   printf("\n use_ctl.bCd4051TestVal:%d--use_ctl.bEq1AdVal:%d/n",use_ctl.bCd4051TestVal,use_ctl.bEq1AdVal);
	
#elif 0



	 if(use_ctl.bCd4051ModeDly)
	 {
	 	use_ctl.bCd4051ModeDly--;
	 	return;
	 }



	switch(cd4051_cur_ch){
		
		case 0:
			CD4051_EQ1_CH_INIT();
			use_ctl.bEq1AdVal = adc_get_value(CD4051_AD_CH);
			CD4051_EQ2_CH_INIT();			
			break;
		case 1:
			CD4051_EQ2_CH_INIT();
			use_ctl.bEq2AdVal = adc_get_value(CD4051_AD_CH);
			CD4051_EQ3_CH_INIT();
			break;
		case 2:
			CD4051_EQ3_CH_INIT();
			use_ctl.bEq3AdVal = adc_get_value(CD4051_AD_CH);
			CD4051_EQ4_CH_INIT();
			break;
		case 3:
			CD4051_EQ4_CH_INIT();
			use_ctl.bEq4AdVal = adc_get_value(CD4051_AD_CH);
			CD4051_EQ5_CH_INIT();
			break;
		case 4:
			CD4051_EQ5_CH_INIT();
			use_ctl.bEq5AdVal = adc_get_value(CD4051_AD_CH);
			CD4051_EQ1_CH_INIT();
			break;
		default:	
			break;

	}

    printf("\n   eq:%d-%d-%d-%d-%d\n",use_ctl.bEq1AdVal,use_ctl.bEq2AdVal,use_ctl.bEq3AdVal,use_ctl.bEq4AdVal,use_ctl.bEq5AdVal);
  //   printf("\n use_ctl.bCd4051TestVal:%d--use_ctl.bEq1AdVal:%d/n",use_ctl.bCd4051TestVal,use_ctl.bEq1AdVal);
	
#endif
	
	
	cd4051_cur_ch++;
	if(cd4051_cur_ch>=5)
	{
		cd4051_cur_ch=0;
	}
}

AT(.volatile_ram_code)
void ad_eq1_check(void)
{
   u16 get_val;
   static u8 ad_val,cnt=0;
   static u8 val;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;


 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   
   get_val = use_ctl.bEq1AdVal;


   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt)
		{
			get_val_prev=get_val;		
			use_ctl.bEq1Val= AD_EQ_MAX-adpot_cut(get_val,AD_EQ_MAX);			
			return;
		}
#endif	 

//    printf("\n --1---- get_val:%d--get_val_prev:%d/n",get_val,get_val_prev);

   if((get_val >get_val_prev+(AD_EQ_AD_MAX/(AD_EQ_MAX*2)))||(get_val <get_val_prev-(AD_EQ_AD_MAX/(AD_EQ_MAX*2))))
   	{
		get_val_prev=get_val;
		
	   val = get_val*AD_EQ_MAX/AD_EQ_AD_MAX;
	   if(val>AD_EQ_MAX){
	        val = AD_EQ_MAX;
	   }

	 #ifdef SUPPORT_VOLTAGE_SWAP
	         val = AD_EQ_MAX-val;
	 #endif

	   if(val != use_ctl.bEq1Val)
	   {
	  //    if((cnt++)>5)
	      { 
	          use_ctl.bEq1Val= val;
	          app_common_key_var_event(KEY_EQ1_SET);
	      }
	   }
	   else
	   {
	      cnt=0;
	   }
   	}
   
}

AT(.volatile_ram_code)
void ad_eq2_check(void)
{
   u16 get_val;
   static u8 ad_val,cnt=0;
   static u8 val;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;


 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   
   get_val = use_ctl.bEq2AdVal;


   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }
   
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt)
		{
			get_val_prev=get_val;		
			use_ctl.bEq2Val= AD_EQ_MAX-adpot_cut(get_val,AD_EQ_MAX);			
			return;
		}
#endif	 
   if((get_val >get_val_prev+(AD_EQ_AD_MAX/(AD_EQ_MAX*2)))||(get_val <get_val_prev-(AD_EQ_AD_MAX/(AD_EQ_MAX*2))))
   	{
		get_val_prev=get_val;

		   val = get_val*AD_EQ_MAX/AD_EQ_AD_MAX;
		   if(val>AD_EQ_MAX){
		        val = AD_EQ_MAX;
		   }
		   
	 	 #ifdef SUPPORT_VOLTAGE_SWAP
	 	         val = AD_EQ_MAX-val;
	 	 #endif
		   
		   if(val != use_ctl.bEq2Val)
		   {
		//      if((cnt++)>5)
		      { 
		          use_ctl.bEq2Val= val;
		          app_common_key_var_event(KEY_EQ2_SET);
		      }
		   }
		   else
		   {
		      cnt=0;
		   }
   	}
}

AT(.volatile_ram_code)
void ad_eq3_check(void)
{
   u16 get_val;
   static u8 ad_val,cnt=0;
   static u8 val;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;


 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   
   get_val = use_ctl.bEq3AdVal;


   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }

   #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt)
		{
			get_val_prev=get_val;		
			use_ctl.bEq3Val= AD_EQ_MAX-adpot_cut(get_val,AD_EQ_MAX);			
			return;
		}
#endif	 
   if((get_val >get_val_prev+(AD_EQ_AD_MAX/(AD_EQ_MAX*2)))||(get_val <get_val_prev-(AD_EQ_AD_MAX/(AD_EQ_MAX*2))))
   	{
		get_val_prev=get_val;

	   val = get_val*AD_EQ_MAX/AD_EQ_AD_MAX;
	   if(val>AD_EQ_MAX){
	        val = AD_EQ_MAX;
	   }
	 	 #ifdef SUPPORT_VOLTAGE_SWAP
	 	         val = AD_EQ_MAX-val;
	 	 #endif
	   if(val != use_ctl.bEq3Val)
	   {
	//      if((cnt++)>5)
	      { 
	          use_ctl.bEq3Val= val;
	          app_common_key_var_event(KEY_EQ3_SET);
	      }
	   }
	   else
	   {
	      cnt=0;
	   }
       }
}

AT(.volatile_ram_code)
void ad_eq4_check(void)
{
   u16 get_val;
   static u8 ad_val,cnt=0;
   static u8 val;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;


 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   
   get_val = use_ctl.bEq4AdVal;


   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }
   
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt)
		{
			get_val_prev=get_val;		
			use_ctl.bEq4Val= AD_EQ_MAX-adpot_cut(get_val,AD_EQ_MAX);			
			return;
		}
#endif	 

   if((get_val >get_val_prev+(AD_EQ_AD_MAX/(AD_EQ_MAX*2)))||(get_val <get_val_prev-(AD_EQ_AD_MAX/(AD_EQ_MAX*2))))
   	{
		get_val_prev=get_val;

	   val = get_val*AD_EQ_MAX/AD_EQ_AD_MAX;
	   if(val>AD_EQ_MAX){
	        val = AD_EQ_MAX;
	   }
	 	 #ifdef SUPPORT_VOLTAGE_SWAP
	 	         val = AD_EQ_MAX-val;
	 	 #endif
	   if(val != use_ctl.bEq4Val)
	   {
//	      if((cnt++)>5)
	      { 
	          use_ctl.bEq4Val= val;
	          app_common_key_var_event(KEY_EQ4_SET);
	      }
	   }
	   else
	   {
	      cnt=0;
	   }
   	}
}

AT(.volatile_ram_code)
void ad_eq5_check(void)
{
   u16 get_val;
   static u8 ad_val,cnt=0;
   static u8 val;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;


 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   
   get_val = use_ctl.bEq5AdVal;


   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }
   
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt)
		{
			get_val_prev=get_val;		
			use_ctl.bEq5Val= AD_EQ_MAX-adpot_cut(get_val,AD_EQ_MAX);			
			return;
		}
#endif	 

   if((get_val >get_val_prev+(AD_EQ_AD_MAX/(AD_EQ_MAX*2)))||(get_val <get_val_prev-(AD_EQ_AD_MAX/(AD_EQ_MAX*2))))
   	{
		get_val_prev=get_val;

		   val = get_val*AD_EQ_MAX/AD_EQ_AD_MAX;
		   if(val>AD_EQ_MAX){
		        val = AD_EQ_MAX;
		   }
	 	 #ifdef SUPPORT_VOLTAGE_SWAP
	 	         val = AD_EQ_MAX-val;
	 	 #endif
		   if(val != use_ctl.bEq5Val)
		   {
	//	      if((cnt++)>5)
		      { 
		          use_ctl.bEq5Val= val;
		          app_common_key_var_event(KEY_EQ5_SET);
		      }
		   }
		   else
		   {
		      cnt=0;
		   }
   	}
}

extern const char  mult_eq_table[AD_EQ_MAX+1];

AT(.volatile_ram_code)
void cd4051_val_init(void)
{
static u8 cnt;

	if(use_ctl.bPowerOnShowDlyCnt==1)
		{
			use_ctl.veq1temp = mult_eq_table[use_ctl.bEq1Val];
			user_define_eq_update(CD4051_EQ0_63HZ,use_ctl.veq1temp);		

			use_ctl.veq2temp = mult_eq_table[use_ctl.bEq2Val];
			user_define_eq_update(CD4051_EQ1_400HZ,use_ctl.veq2temp);			  

			use_ctl.veq3temp = mult_eq_table[use_ctl.bEq3Val];
			user_define_eq_update(CD4051_EQ2_1KHZ,use_ctl.veq3temp);			  

			use_ctl.veq4temp = mult_eq_table[use_ctl.bEq4Val];
			user_define_eq_update(CD4051_EQ3_6KHZ,use_ctl.veq4temp);			  

			use_ctl.veq5temp = mult_eq_table[use_ctl.bEq5Val];
			user_define_eq_update(CD4051_EQ4_12KHZ,use_ctl.veq5temp);	
		}
	   
}

#endif

