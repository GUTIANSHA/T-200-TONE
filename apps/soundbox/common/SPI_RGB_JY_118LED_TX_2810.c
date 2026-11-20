#include "spi_rgb_config.h"

#define USER_RGB_NUMBER (118)

#define USER_RGB_CODE0  0X60//????2bit ????6bit 0xC0 ????????1λ 0x60
#define USER_RGB_CODE1  0X7c//????4bit ????4bit 0xF0 ????????1λ 0x78
#define USER_RGB_COLOUR_LEN (USER_RGB_NUMBER*3)

#define USER_RGB_BUFF_MALLOC_EN 0//rgb????spi buff?????????????????????????? ??????????????????????????????????????С ????????????????????????????С???????? ????????????????????????????????????????л??????????????

//跟随音乐跳动参数
#define MAX_ENERGY		13		//最大能量等级
#define down_speed		5		//数值越大下降速度越慢


typedef struct _USER_RGB_MAPPING_NXY_{
    // u8 clash;//1????xy->n 2:n->xy 3:round0->n 4:n->round0 5:round1->n 6:n->round1
    u16 N;//??????????????????????
    u8 X;//????????????x????????
    u8 Y;//????????????y????????
    // u32 round;
}RGB_MAP_NXY;

typedef struct _USER_RGB_MAPPING_XYN_{
    // u8 clash;//1????xy->n 2:n->xy 3:round0->n 4:n->round0 5:round1->n 6:n->round1
    u8 X;//????????????x????????
    u8 Y;//????????????y????????
    u32 N;//??????????????????????
    // u32 round;
}RGB_MAP_XYN;
//??????
typedef struct _RGB_DEV_COLOUR_CNT_{
    int g;
    int r;
    int b;
}RGB_DEV_COLOUR_CNT;

//spi rgb ???????????????? ?????? ??ο????????????????????
typedef struct _RGB_DEV_SPI_COLOUR_{
    u8 g[8];
    u8 r[8];
    u8 b[8];
}RGB_DEV_SPI_COLOUR;

//0???? 1???? ???????????????????????????????????? ????????????????????????????????????u8????????0????1??????????  ????????spi??????????u8?????????????? ??????????????????????
typedef struct _RGB_DEV_CODE_{
    u8 code_0;
    u8 code_1;
}RGB_DEV_CODE;

typedef struct _BRI_LEVEL_{
    int level;
    int max;
}V_L;

u8 LED_YUANQUAN_DATA[30][3]={};

typedef struct _RGB_DEV_INFO_{
    u16 time_id;
    // bool power_off;
    bool init_flag;
    V_L Brightness_level;//????????????
    volatile bool rend_flag;
    volatile bool updata_flag;//????????????????±????
    volatile u8   updata_cnt;//??????????????????????
    const RGB_DEV_CODE code;
    const u8 spi_port;//spi
    const u32 number;//??????????????
    u16 spi_scan_time;//spi ??????????????????????????????????

    #if USER_RGB_BUFF_MALLOC_EN
    RGB_DEV_COLOUR *rgb_buff;//[USER_RGB_NUMBER];//?????????? ??洢buff
    RGB_DEV_SPI_COLOUR *spi_buff;//[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi ????????buff
    #elif (defined(USER_RGB_NUMBER) && USER_RGB_NUMBER)
    RGB_DEV_COLOUR rgb_buff[USER_RGB_NUMBER];//?????????? ??洢buff
    RGB_DEV_SPI_COLOUR spi_buff[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi ????????buff
    #endif
}RGB_DEV_INFO;

RGB_DEV_INFO *_this_rgb = NULL;

RGB_DEV_INFO user_rgb_info={
    .Brightness_level ={
        .level  = 255,
        .max    = 255,
    },
    .time_id   = 0,
    // .power_off = 0,
    .init_flag = 0,
    .rend_flag = 0,
    .updata_flag = 0,
    .updata_cnt = 0,
    .number = USER_RGB_NUMBER,
    // .spi_scan_time = 20,//????????????????????????????????????
    .spi_port = 1,//SPI2,
    .code = {
        .code_0 = USER_RGB_CODE0,
        .code_1 = USER_RGB_CODE1,
    },

    #if USER_RGB_BUFF_MALLOC_EN
    .rgb_buff = NULL,
    .spi_buff = NULL,
    #else
    .rgb_buff = {0},
    .spi_buff = {0},
    #endif
};
typedef struct _RGB_INFO_{
    RGB_DEV_INFO *info;
    u32 mode_scan_ms;//????????????????
}RGB_INFO;
RGB_INFO rgb_cfg = {
    .info = &user_rgb_info,
//    .hsv = {.h=0,.s=255,.v=0},
    .mode_scan_ms = 10,
};



RGB_INFO *_this_ = &rgb_cfg;
void user_rgb_p_timer(u8 cmd){
    // return;
    
    static u32 cnt_timer = 0;
    u32 cnt_timer_tp = sys_timer_get_ms();
    if(cmd){
        r_printf("T %d",cnt_timer_tp-cnt_timer);
    }
    cnt_timer = cnt_timer_tp;
}

//??????????rgb??????????????????spi????
void user_rgb_set_only(RGB_DEV_SPI_COLOUR *spi_buff,RGB_DEV_COLOUR *rgb,RGB_DEV_CODE *code){
    if(!spi_buff || !rgb || !code){
        return;
    }

    for(int i=0;i<8;i++){
        if(rgb->r&BIT(7-i)){
            spi_buff->r[i]=code->code_1;
        }else{
            spi_buff->r[i]=code->code_0;
        }
        if(rgb->g&BIT(7-i)){
            spi_buff->g[i]=code->code_1;
        }else{
            spi_buff->g[i]=code->code_0;
        }
        if(rgb->b&BIT(7-i)){
            spi_buff->b[i]=code->code_1;
        }else{
            spi_buff->b[i]=code->code_0;
        }
    }
}


/***
 * rgb_set_brightness
 * ??????????????????????????????
 * ??????????rgb buff ????????
*/
static RGB_DEV_COLOUR rgb_set_only_brightness(RGB_DEV_COLOUR colour){
    RGB_DEV_COLOUR_CNT colour_int;

    if(!_this_rgb || !_this_rgb->Brightness_level.max || !_this_rgb->rgb_buff){
        return colour;
    }
    
    if(!_this_rgb->Brightness_level.level){
        memset(&colour,0,sizeof(RGB_DEV_COLOUR));
        return colour;
    }else if(_this_rgb->Brightness_level.level>=_this_rgb->Brightness_level.max){
        return colour;
    }

    colour_int.r = colour.r* _this_rgb->Brightness_level.level/_this_rgb->Brightness_level.max;
    colour_int.g = colour.g* _this_rgb->Brightness_level.level/_this_rgb->Brightness_level.max;
    colour_int.b = colour.b* _this_rgb->Brightness_level.level/_this_rgb->Brightness_level.max;

    colour.r = colour_int.r;
    colour.g = colour_int.g;
    colour.b = colour_int.b;
    
    return colour;
}

/***
 * rgb_set_brightness
 * ????????????????????????????????
 * ??????rgb buff ????????
*/
static void rgb_set_all_data_brightness(void){

    if(!_this_rgb || !_this_rgb->Brightness_level.max || !_this_rgb->rgb_buff){
        return;
    }

    for(s32 i=0;i<_this_rgb->number;i++){
        _this_rgb->rgb_buff[i] = rgb_set_only_brightness(_this_rgb->rgb_buff[i]);
    }
}


//????????????????????????????????????spi????????
void user_rgb_set_all(void){
    RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;

    if(!rgb || !rgb->spi_buff || !rgb->rgb_buff || !rgb->number){
        return;
    }

    RGB_DEV_SPI_COLOUR *P=rgb->spi_buff;


    // rgb->rend_flag = 1;
    RGB_DEV_COLOUR colou;
    for(s32 i=0;i<rgb->number;i++){
		
        colou = _this_rgb->rgb_buff[i];
        colou = rgb_set_only_brightness(colou);  
        user_rgb_set_only(P+i,&colou,&(rgb->code));
    }
    // rgb->rend_flag = 0;

    return;
}

//????????????????rgb??????
/*
priv????rgb ????????????????
colour????rgb ??????
number:????io???????????????????????????????????????????????????????????? -1????????(USER_RGB_NUMBER-1)
*/
void user_rgb_colour_only_set(RGB_DEV_COLOUR *colour,s32 number){

    RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;
    s32 mapp_number = number;

    if(!rgb || !colour){
        return;
    }

    mapp_number%=rgb->number;

    mapp_number = mapp_number<0?(rgb->number+mapp_number):mapp_number;

    rgb->rgb_buff[mapp_number]=*colour;

    return;
}

//??????????????????????
void user_rgb_same_colour(RGB_DEV_COLOUR *colour){
    RGB_DEV_INFO *rgb = _this_rgb;
    if(!rgb || !colour || !rgb->number){
        return;
    }

    for(s32 i=0;i<rgb->number;i++){
        user_rgb_colour_only_set(colour,i);
    }
}

//????????????
void user_rgb_clear_colour(void){
    RGB_DEV_INFO *rgb = _this_rgb;
    RGB_DEV_COLOUR colour={0,0,0};

    if(!rgb || !rgb->number){
        return;
    }

    user_rgb_same_colour(&colour);
}

/*---------------------------------------------------------*/
/**@brief   ????????????λ??????????????
   @param   _start:??????λ????
   @param   _end: ????????λ????
   @return  null
   @note	????????????0??????????_end????????_this_rgb->number??????0??????
/*---------------------------------------------------------*/

void clear_color(u8 _start,u8 _end){
    RGB_DEV_COLOUR CLEAR_COLOR={0,0,0};
	if((_end-_start)>0){
		for(s8 i=_start;i<_end+1;i++){
			user_rgb_colour_only_set(&CLEAR_COLOR,i);
		}
	}else{
		for(s8 i=_start;i>_end-1;i--){
			user_rgb_colour_only_set(&CLEAR_COLOR,i);
		}
	}

}

/*---------------------------------------------------------*/
/**@brief   ??趨??????λ??????????????
   @param   _start:??????λ????
   @param   _end: ????????λ????
   @param   _color: ??????
   @return  null
   @note	????????????0??????????_end????????_this_rgb->number??????0??????
/*---------------------------------------------------------*/
void set_color(u8 _start,u8 _end,RGB_DEV_COLOUR _color){
    for(s8 i=_start;i<_end+1;i++){
		  user_rgb_colour_only_set(&_color,i);
	}
}


//??????????????????????
void user_rgb_send_once(void){
    extern volatile bool user_extern_buf_flag;

    if(!_this_rgb || !_this_rgb->number || !_this_rgb->init_flag ||
        !_this_rgb->spi_buff || _this_rgb->updata_flag || 
        !_this_rgb->updata_cnt || _this_rgb->rend_flag){
        // r_printf("spi send data error %d %d %d %d %d %d %d",!_this_rgb,!_this_rgb->number,!_this_rgb->init_flag,
        // !_this_rgb->spi_buff,_this_rgb->updata_flag,!_this_rgb->updata_cnt,_this_rgb->rend_flag        
        // );
        return;
    }
    _this_rgb->rend_flag = 1;
    // putchar('P');

    local_irq_disable();


    user_rgb_set_all();
    //memset(_this_rgb->spi_buff,0xaa,sizeof(RGB_DEV_SPI_COLOUR)*(_this_rgb->number));
    // int send_ret = user_spi_dma_send(_this_rgb->spi_port,(u8*)_this_rgb->spi_buff,sizeof(RGB_DEV_SPI_COLOUR)*(_this_rgb->number));
    
//    int send_ret = spi_dma_send(_this_rgb->spi_port,(u8*)_this_rgb->spi_buff,sizeof(RGB_DEV_SPI_COLOUR)*(_this_rgb->number));
	spi_clear_pending(_this_rgb->spi_port);
	spi_dma_set_addr_for_isr(_this_rgb->spi_port,(u8*)_this_rgb->spi_buff,sizeof(RGB_DEV_SPI_COLOUR)*(_this_rgb->number),0);

	//printf(" send_ret %d\n",send_ret);
    local_irq_enable();


    if(_this_rgb->updata_cnt){
        _this_rgb->updata_cnt -=1;
    }
    _this_rgb->rend_flag = 0;
    // r_printf("spi send data ok");
}

//????????spi????????
void user_rgb_send(void *priv){
    RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;
    if(!rgb){
        return;
    }

    u16 timeout_add_time = rgb->spi_scan_time;

    // printf(">>>> clck spi %d sys %d\n",clk_get("spi"),clk_get("sys"));
    if(rgb->updata_cnt){
        user_rgb_send_once();
    }else{
        timeout_add_time = 10;
    }

    rgb -> time_id = sys_s_hi_timerout_add(rgb,user_rgb_send,timeout_add_time);
}

//????????????????????
//0 ??????cnt???????? ????????????????????
//??????????????????????????????
void user_rgb_send_cnt(u8 cnt){
        // return;
    if(!_this_rgb){
        return;
    }

    _this_rgb->updata_cnt = cnt;

    if(_this_rgb->updata_cnt){
        // user_rgb_p_timer();
        user_rgb_send_once();
    }
}

//??????????
void user_rgb_dev_init(void *priv){
    RGB_DEV_INFO *rgb = (RGB_DEV_INFO *)priv;
    if(!rgb || SPI_MAX_HW_NUM<=(rgb->spi_port)){
        return;
    }
    _this_rgb = rgb;

    g_printf(">>>>>>>>>> _this_rgb %x %x",_this_rgb,priv);

    int ret = spi_open(rgb->spi_port);
    r_printf("1>>>>>>>>>>>>>>>>000 ret=%d",ret);
    if(ret<0){
        return;
    }

    //????????????????????????????????????
    u32 tp_time = 7*_this_rgb->number/100;
    _this_rgb->spi_scan_time = tp_time+4;
    if(!(_this_rgb->spi_scan_time)){
        _this_rgb->spi_scan_time = 20;
    }
    r_printf("spi scan time=%d tp time=%d",_this_rgb->spi_scan_time,tp_time);

    user_rgb_send_cnt(0);
    _this_rgb->init_flag = true;

     sys_s_hi_timerout_add(NULL,user_rgb_send,_this_rgb->spi_scan_time+3000);//????spi???????????????????????????????????? ??????rgb????????????????
}

void user_rgb_dev_exit(void){
    // local_irq_disable();

    if(_this_rgb){
        user_rgb_clear_colour();
        _this_rgb->updata_flag = 0;//???????? user_rgb_send_strl ????updata_flag??1
        user_rgb_send_cnt(1);

        _this_rgb->init_flag = 0;
        if(_this_rgb -> time_id){
            sys_s_hi_timeout_del(_this_rgb -> time_id);
            _this_rgb -> time_id = 0;
        }
        spi_close(_this_rgb->spi_port);
        _this_rgb = NULL;
    }
    // local_irq_enable();
}



#if 1//def SPI_RGB_JY_BN_2610FM_40LED
extern u8 light_mode_mark;

#if 0
double calculate_bE(double bEnergyVal, int led_vol) {
    // 预计算的拉格朗日插值多项式系数
    // 基于五个已知点 (5,10), (15,40), (20,70), (25,90), (30,120)
    // 通过拉格朗日插值法推导出的简化多项式
    
    double x = (double)led_vol;
    
    // 直接计算拉格朗日插值多项式
    // L0(x) = ((x-15)(x-20)(x-25)(x-30))/((5-15)(5-20)(5-25)(5-30))
    // L1(x) = ((x-5)(x-20)(x-25)(x-30))/((15-5)(15-20)(15-25)(15-30))
    // L2(x) = ((x-5)(x-15)(x-25)(x-30))/((20-5)(20-15)(20-25)(20-30))
    // L3(x) = ((x-5)(x-15)(x-20)(x-30))/((25-5)(25-15)(25-20)(25-30))
    // L4(x) = ((x-5)(x-15)(x-20)(x-25))/((30-5)(30-15)(30-20)(30-25))
    
    // 简化后的直接计算公式
    double term1 = (x - 15) * (x - 20) * (x - 25) * (x - 30) / ((5 - 15) * (5 - 20) * (5 - 25) * (5 - 30));
    double term2 = (x - 5) * (x - 20) * (x - 25) * (x - 30) / ((15 - 5) * (15 - 20) * (15 - 25) * (15 - 30));
    double term3 = (x - 5) * (x - 15) * (x - 25) * (x - 30) / ((20 - 5) * (20 - 15) * (20 - 25) * (20 - 30));
    double term4 = (x - 5) * (x - 15) * (x - 20) * (x - 30) / ((25 - 5) * (25 - 15) * (25 - 20) * (25 - 30));
    double term5 = (x - 5) * (x - 15) * (x - 20) * (x - 25) / ((30 - 5) * (30 - 15) * (30 - 20) * (30 - 25));
    
    return 10 * term1 + 40 * term2 + 70 * term3 + 90 * term4 + 120 * term5;
}
#endif

u8 get_energyStep(u8 max_energy)
{
	u8 bE=0;
	u16 bEnergyVal=0,bMicEnergyVal=0;
static u16 bEnergyValTemp,bstepval;	
	bEnergyVal = audio_dac_energy_get();
//	bMicEnergyVal = abs(get_micpr_energ());
	bMicEnergyVal = audio_mic_dac_energy_get();
	
	//MAX(bEnergyVal, abs(get_micpr_energ()));

#if 1
	// 	printf("\n ===0000==energy=%d-mice=%d-mvol=%d-bE=%d\n",audio_dac_energy_get(),audio_mic_dac_energy_get(),use_ctl.mic_vol,bE);	

	//	return bE;		

	if((bEnergyVal==0)&&(bMicEnergyVal==0)) 
	{
		bE=0;
		return bE;		
	}
	 else  if(bMicEnergyVal>0 && bEnergyVal>0)
	 {
	 
	 		if((bEnergyVal<(use_ctl.led_vol/10+1)*180)&&(bEnergyVal<(use_ctl.mic_vol/10+1)*11)) bE=1;
	 		else
	 		{
	 			bE=((bEnergyVal/((use_ctl.led_vol/3)+20+(use_ctl.led_vol/5)*7))+(bMicEnergyVal/((use_ctl.mic_vol/2)+9+(use_ctl.mic_vol/5)*4)))/2;	// 31
	 //	printf("\n ===0000==energy=%d-mice=%d-mvol=%d-bE=%d\n",audio_dac_energy_get(),audio_mic_dac_energy_get(),use_ctl.mic_vol,bE);	
	 		        bE = bE/10;
	 		}

	 //	printf("\n ===0011==energy=%d-mice=%d-mvol=%d-bE=%d\n",audio_dac_energy_get(),audio_mic_dac_energy_get(),use_ctl.mic_vol,bE);	
	 		
	 	
	 	if(bE>max_energy)bE=max_energy;	 
	 }
	 else  if(bMicEnergyVal>0)
	 {
	 
	 		if(bMicEnergyVal<(use_ctl.mic_vol/10+1)*90) bE=1;
	 		else
	 		{
	 			bE=(bMicEnergyVal/((use_ctl.mic_vol/2)+9+(use_ctl.mic_vol/5)*4));	// 31
			// 	printf("\n ===1100==energy=%d-mice=%d-mvol=%d-bE=%d\n",audio_dac_energy_get(),audio_mic_dac_energy_get(),use_ctl.mic_vol,bE);	
	 			
	 		        bE = bE/10;
	 		}

		// 	printf("\n ===1111==energy=%d-mice=%d-mvol=%d-bE=%d\n",audio_dac_energy_get(),audio_mic_dac_energy_get(),use_ctl.mic_vol,bE);	
	 		
	 	
	 	if(bE>max_energy)bE=max_energy;	 
	 }	 
	 else
	 {

	 		if(bEnergyVal<(use_ctl.led_vol/10+1)*75) bE=1;
	 		else
	 		{
//	 			bE=(bEnergyVal/((use_ctl.led_vol/3)+20+(use_ctl.led_vol/5)*2));	// 31    result=120max
	 			bE=(bEnergyVal/((use_ctl.led_vol/3)+20+(use_ctl.led_vol/5)*2));	// 31    result=120max
//	 			bE=calculate_bE(bEnergyVal,use_ctl.led_vol);
//	 		        bE = bE/10; // 12  
	 		        bE = bE/2.5; // 12  
	 		}	
			
		 	if(bE>max_energy)bE=max_energy;
	 		
 //	printf("\n ===222==energy=%d-mice=%d-mvol=%d-bE=%d\n",audio_dac_energy_get(),audio_mic_dac_energy_get(),use_ctl.mic_vol,bE);	
	 	
	 }	

 //	printf("\n ===333==energy=%d-mice=%d-mvol=%d-bE=%d\n",audio_dac_energy_get(),audio_mic_dac_energy_get(),use_ctl.mic_vol,bE);	
		
	
#if 0	
/*
music			mic:	
1- 614  50		356  5
3- 699   54   		483  5
6-1025   77		619  11
9- 1378      81 	829  13
10 1601	  88        947   16
12-1940   108

15 2601  160		1555   20
20 3954  231		2263   24
25 6959 379         4072  26
30- 10543   537     6234  30  

*/

	for( i=15;i>0;i--)
	{
	   if(BIT(i)&bEnergyVal)
	   	{
		   	bE=i;
		   	break;
	   	}
	}
		(use_ctl.led_vol/3+2)
		
	 	  if(bEnergyVal>6000) bEnergyVal=bEnergyVal/2;
	
	 	  if(bEnergyVal>0x18f) 
		 	bEnergyVal=(u8)(bEnergyVal>>4);	
		  
	 	  if(bEnergyVal>0x18)
		 	bEnergyVal=(u8)(bEnergyVal>>4);		
			  
		bE=bEnergyVal;

#endif

		
#else
	if((bEnergyVal==0)&&(bMicEnergyVal==0)) 
	{
		bE=0;
		return bE;		
	}
	else if((bEnergyVal==0)&&(bMicEnergyVal>0))
	{
//			bE=((bEnergyVal+bMicEnergyVal)/2)/((use_ctl.led_vol/3)+20);
		bE=bMicEnergyVal/(use_ctl.mic_vol+1);
	}		
	else
	{
//		bE=bEnergyVal/((use_ctl.led_vol/3)+20);
		bE=bEnergyVal/(use_ctl.led_vol+1);
	}

	printf("bE===11===%d=%d=%d=%d\n",get_dac_energy(),get_micpr_energ(),bE,use_ctl.mic_vol);
	
          //      bE = bE/10;
				

	if(bE>max_energy)bE=bE*0.8;	 
	if(bE>max_energy)bE=bE*0.8;	 
	if(bE>max_energy)bE=bE*0.85;	 	
//	if(bE>max_energy)bE=bE/2;	 	
	if(bE>max_energy)bE=max_energy;	 

	printf("bE===222===%d=%d=%d=%d\n",get_dac_energy(),get_micpr_energ(),bE,use_ctl.mic_vol);

	
#endif	
	
	
	return bE;
}
//----------能量计算-------------------

#define MAX_STEP		31 // 12
u8 bEnergyStep=0; //当前能量级数
void user_dac_energy_scan(void)
{
static u16 bVolRatio=0; //当前音量能量分级系数
static u32 dac_energy_buf=0;
static u16 dac_energy_value=0,dac_energy_value_temp=0,dac_energy_value_old=0;
static u8 eng_one_times=0;
static char left_up_i,left_up_j;
static u16 dac_energy_timer=0;


if( light_mode_mark == RGB_OFF)
	return;

#if 0
if(use_ctl.led_vol==0) ////0级音量2秒自动进入呼吸模式
{
	bEnergyStep=0;
	return;
}
#endif

	bEnergyStep=get_energyStep(MAX_STEP);
	return;

}

//----------能量计算-------------------





// grb
const u8 rgb_buf_set[6][3]=
{
{0x00,0xfa,0x00},//R
{0xfa,0x00,0x00}, //G
{0x00,0x00,0xfa},//B
{0x00,0xfa,0xfa},//R+G
{0xfa,0xfa,0x00},//R+B
{0xfa,0x00,0xfa},// G+B
};

//---------color buf  set------------------
#define RGB_VAL_MAX 0XFA  // =250
#define RGB_CNT_MAX 300
u8 rgb_buf[2][3];
void user_rgb_color_set(u16 val)
{

if(val==0)
{
	rgb_buf[0][0]=0;
	rgb_buf[0][1]=0;
	rgb_buf[0][2]=RGB_VAL_MAX;
}
else
{
    if(val<=50) // G++  B-max
    	{
		rgb_buf[0][0]=val*5;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=RGB_VAL_MAX;
    	}
   else  if(val<=100) // G-max B--
    	{
		rgb_buf[0][0]=RGB_VAL_MAX;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=RGB_VAL_MAX-(val-50)*5;
    	}
   else  if(val<=150) // G-MAX R++
    	{
		rgb_buf[0][0]=RGB_VAL_MAX;
		rgb_buf[0][1]=(val-100)*5;
		rgb_buf[0][2]=0;
    	}
     else  if(val<=200) // R-MAX G--
    	{
		rgb_buf[0][0]=RGB_VAL_MAX-(val-150)*5;
		rgb_buf[0][1]=RGB_VAL_MAX;
		rgb_buf[0][2]=0;
    	}
     else  if(val<=250) // R-MAX B+
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=RGB_VAL_MAX;
		rgb_buf[0][2]=(val-200)*5;
    	}
     else  if(val<=300) // R-- B-MAX
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=RGB_VAL_MAX-(val-250)*5;
		rgb_buf[0][2]=RGB_VAL_MAX;
    	}


}

}



void user_rgb_color_set_mode2(u16 val) // light-off-light
{

if(val==0)
{
	rgb_buf[0][0]=0;
	rgb_buf[0][1]=0;
	rgb_buf[0][2]=0;
}
else
{
    if(val<=50) // B++
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=val*5;
    	}
   else  if(val<=100) // B--
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=RGB_VAL_MAX-(val-50)*5;
    	}
   else  if(val<=150) // G++ B++
    	{
		rgb_buf[0][0]=(val-100)*5;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=(val-100)*5;
    	}
     else  if(val<=200) // G-- B--
    	{
		rgb_buf[0][0]=RGB_VAL_MAX-(val-150)*5;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=RGB_VAL_MAX-(val-150)*5;
    	}
     else  if(val<=250) // G++
    	{
		rgb_buf[0][0]=(val-200)*5;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=0;
    	}
     else  if(val<=300) // G--
    	{
		rgb_buf[0][0]=RGB_VAL_MAX-(val-250)*5;
		rgb_buf[0][1]=0;
		rgb_buf[0][2]=0;
    	}

     else  if(val<=350) // G++ R++
    	{
		rgb_buf[0][0]=(val-300)*5;
		rgb_buf[0][1]=(val-300)*5;
		rgb_buf[0][2]=0;
    	}
     else  if(val<=400) //  G-- R--
    	{
		rgb_buf[0][0]=RGB_VAL_MAX-(val-350)*5;
		rgb_buf[0][1]=RGB_VAL_MAX-(val-350)*5;
		rgb_buf[0][2]=0;
    	}
     else  if(val<=450) // R++
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=(val-400)*5;
		rgb_buf[0][2]=0;
    	}
     else  if(val<=500) // R--
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=RGB_VAL_MAX-(val-450)*5;
		rgb_buf[0][2]=0;
    	}
     else  if(val<=550) // R++B++
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=(val-500)*5;
		rgb_buf[0][2]=(val-500)*5;
    	}

     else  if(val<=600) // R--B--
    	{
		rgb_buf[0][0]=0;
		rgb_buf[0][1]=RGB_VAL_MAX-(val-550)*5;
		rgb_buf[0][2]=RGB_VAL_MAX-(val-550)*5;
    	}	 	 
}

}

//----------mode 1-------------------

/***************************************************************************
void rgb_roll_main(u8 *roll_buf,RGB_DEV_COLOUR *colour,u8 max,u8 num,bool direct)
参数 :
       roll_buf:数组排序RGB
	max:滚动总共灯数量
	num:滚动灯数量
	direct:转动方向  1-顺时针0-逆时针
**************************************************************************/
void rgb_roll_main(u8 *roll_buf,RGB_DEV_COLOUR *colour,u8 max,u8 num,bool direct)
{
static u8 i,m,n,pos1=0,pos2=0;
u8 *_roll_buf = roll_buf;

	for(i=pos2;i<=pos2+pos1;i++)
	{
		m=i;
		if(direct==0)
		m=max-m-1;
		if(i<max)
		{		    
			user_rgb_colour_only_set(colour,*(_roll_buf+m));
		}	
	}

	if(pos1<num)	pos1++;
	else
	{ 
	   if(pos2<max-1)	pos2++;
	   else
	   	{
		  pos1=0;pos2=0;  
	   	}  
	}		
}


/*******************************************
void rgb_colour_set_main(u8 colour_pos,u8 colour_val,bool colour_auto_flag)
参数 :
       colour_pos:colour pos number set (0-6:fixed colour 7:auto colour)
       max_step:colour steps
        colour_val:fixed colour mode set dec val
      colour_auto_flag:auto colour mode 自动变色开关
******************************************/
void rgb_colour_set_main(u8 colour_pos,u8 max_step,u8 colour_val,bool colour_auto_flag)
{
static u16 bVal; // rgb color set

	if(colour_pos<6)
	{
		bVal=0;
		if(colour_val>max_step)
			colour_val=max_step;
		
		
		if(rgb_buf_set[colour_pos][0]==0xfa&&(colour_val>0))			
		  rgb_buf[0][0]=rgb_buf_set[colour_pos][0]-(colour_val-1)*(0xfa/max_step);
		else
		  rgb_buf[0][0]=rgb_buf_set[colour_pos][0];
		
		if(rgb_buf_set[colour_pos][1]==0xfa&&(colour_val>0))			
		  rgb_buf[0][1]=rgb_buf_set[colour_pos][1]-(colour_val-1)*(0xfa/max_step);
		else
		  rgb_buf[0][1]=rgb_buf_set[colour_pos][1];
		
		if(rgb_buf_set[colour_pos][2]==0xfa&&(colour_val>0))			
		  rgb_buf[0][2]=rgb_buf_set[colour_pos][2]-(colour_val-1)*(0xfa/max_step);
		else
		  rgb_buf[0][2]=rgb_buf_set[colour_pos][2];		
	}
	else
	{
		user_rgb_color_set(bVal);
		if(colour_auto_flag==1)
			{
				if(bVal<RGB_CNT_MAX)   bVal++;  else bVal=0;
			}
	}
	
}



void user_rgb_clear_pos(u8 start , u8 end)
{
RGB_DEV_COLOUR colour;

    for( u8 i=start;i<end;i++)
	{
		colour.g=0;
		colour.r=0;
		colour.b=0;		
        user_rgb_colour_only_set(&colour,i);
    }
}

//----------mode tab  start-------------------


const u8 rgb_mod_tab[] = //
{
0,1,2,3,4,5,6,7,8,9,10,
 11,12,13,14,15,16,17,18,19,20,
 21,22,23,24,25,26,27,28,29,30,
 31,32,33,
};

#define RGB_CIRCEL_START 42

#define ROUND5_STEP  5.6   // 14/6_COLOR ==3LED

void user_rgb_circel_mode1_tab(void) 
{
static u8 pos=0,i,j,m,n,cnt,l,cnt1,bDirectVal;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;

static u8 steptemp,ll;

//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;


l=34;
	
	for(i=pos;i<pos+l;i++)
	{		
		if(i>l-1) m=i-l;else m=i;// 取显示位置
		colour.g= rgb_buf_set[(int) ((i-pos)/ROUND5_STEP)][0];
		colour.r= rgb_buf_set[(int) ((i-pos)/ROUND5_STEP)][1];
		colour.b= rgb_buf_set[(int) ((i-pos)/ROUND5_STEP)][2];	
		if(bDirectVal==1)//反转
			{
				m=l-1-m;
				colour.g= rgb_buf_set[(int) ((i-pos)/ROUND5_STEP)][0];
				colour.r= rgb_buf_set[(int) ((i-pos)/ROUND5_STEP)][1];
				colour.b= rgb_buf_set[(int) ((i-pos)/ROUND5_STEP)][2];					
			}
		 user_rgb_colour_only_set(&colour,rgb_mod_tab[m]);		
	}
	
	if(pos<ll-1)
		pos++;
	else	
		{
			pos=0;		
			cnt1++;
			if(cnt1==2)
				ll=l-(ROUND5_STEP-1);
			else
				ll=l;
			if(cnt1==3)
				{
					cnt1=0;
					if(bDirectVal) 
						{
						bDirectVal=0;
						}
						else
						{
							bDirectVal=1;					
						}
				}			

		}

			
}


//----------mode 1 start-------------------


const u8 rgb_mod_tab1[] = 
{
112,113,114,115,116,117
};

void user_rgb_circel_mode1_tab1(void)
{
static u8 pos=0,i,l,m,n,cnt;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;

	l=6;
	
	for(i=pos;i<pos+l;i++)
	{	
		//取色
		n=(i-pos)%6;
	
		if(i>l-1) m=i-l;else m=i;// 取显示位置
		colour.g= rgb_buf_set[n][0];
		colour.r= rgb_buf_set[n][1];
		colour.b= rgb_buf_set[n][2];		
		 user_rgb_colour_only_set(&colour,rgb_mod_tab1[m]);		
	}
	if(pos<l-1)pos++;else	pos=0;

}

//----------mode 1 end-------------------

//----------mode tab  vol mvol start-------------------
//34+8+62+8+6
// 34 42 104 112

const u8 rgb_mod_vol[] = 
{
34,35,36,37,38,39,40,41
};
const u8 rgb_mod_mvol[] = 
{
104,105,106,107,108,109,110,111
};
const u8 rgb_speed_mode4[] =
{5,13,15,16,16,21,25,30,37,50,75,150,};

void user_rgb_circel_mode1_mvol(void) 
{
static u8 i,j,l;
RGB_DEV_COLOUR colour;
static u8 cnt,steptemp;


l=8;	
	for(i=0;i<l;i++)
	{		

		 #ifdef SUPPORT_RGB_VOL_MVOL_FLASH
		  if(use_ctl.bRgbMvolPotenCnt%2==0)
		 	{
				colour.g= rgb_buf_set[j][0];
				colour.r= rgb_buf_set[j][1];
				colour.b= rgb_buf_set[j][2];	
		 	}
		 else
		 	{
				colour.g= 0;
				colour.r= 0;
				colour.b= 0;	
		 	}
		#else
		 	{
				colour.g= rgb_buf_set[j][0];
				colour.r= rgb_buf_set[j][1];
				colour.b= rgb_buf_set[j][2];	
		 	}		
		 #endif	
		 
		user_rgb_colour_only_set(&colour,rgb_mod_mvol[i]);		
	}

#if 1

if(light_mode_mark==1&&bEnergyStep!=0) // mode1  freq  fix vol mvol flash speed
	steptemp=rgb_speed_mode4[0];
else
	steptemp=rgb_speed_mode4[bEnergyStep];
if(++cnt<steptemp)	return;else	cnt=0;

#endif	

	j++;
	if(j>=6) j=0;			
}
void user_rgb_circel_mode1_vol(void) 
{
static u8 i,j,l,cnt1;
RGB_DEV_COLOUR colour;
static u8 cnt,steptemp;


	l=8;
	
	for(i=0;i<l;i++)
	{		
		 
		 #ifdef SUPPORT_RGB_VOL_MVOL_FLASH
		  if(use_ctl.bRgbVolPotenCnt%2==0)
		 	{
				colour.g= rgb_buf_set[j][0];
				colour.r= rgb_buf_set[j][1];
				colour.b= rgb_buf_set[j][2];	
		 	}
		 else
		 	{
				colour.g= 0;
				colour.r= 0;
				colour.b= 0;	
		 	}
		#else
		 	{
				colour.g= rgb_buf_set[j][0];
				colour.r= rgb_buf_set[j][1];
				colour.b= rgb_buf_set[j][2];	
		 	}		
		 #endif	

		 
		user_rgb_colour_only_set(&colour,rgb_mod_vol[i]);		
	}

 cnt1++;
if(cnt1==20)	cnt1=0;

if(light_mode_mark==1&&bEnergyStep!=0)  // mode1  freq  fix vol mvol flash speed
	steptemp=rgb_speed_mode4[0];
else
steptemp=rgb_speed_mode4[bEnergyStep];
if(++cnt<steptemp)	return;else	cnt=0;


	j++;
	if(j>=6) j=0;			
}


void user_rgb_circel_mode1_tab_vol_scan(void)
{
	user_rgb_circel_mode1_tab();
	user_rgb_circel_mode1_vol();
	user_rgb_circel_mode1_tab1();
	user_rgb_circel_mode1_mvol();
}

//----------mode  start-------------------

//----------mode 1 -1 start-------------------


const u8 rgb_buf_set_mode1[3][3]=
{
{0x00,0xfa,0x00},//R
{0x00,0x00,0xfa}, //B
{0xfa,0xfa,0xfa},// R+G+B
};

const u8 rgb_buf_set_mode1_2[3][3]=
{
{0x00,0xfa,0x00},//R
{0xfa,0xfa,0xfa},// R+G+B
{0x00,0x00,0xfa}, //B
};

const u8 rgb_mod_circel[] = 
{
0,1,2,3,4,5,6,7,8,9,
10,11,12,13,14,15,16,17,18,19,
20,21,22,23,24,25,26,27,28,29,
30,31,32,33,34,35,36,37,38,39,
40,41,42,43,44,45,46,47,48,49,
50,51,52,53,54,55,56,57,58,59,
60,61
};



#define ROUND1_STEP  20
#define ROUND2_MAX   18
#define ROUND3_MAX   4



const u8 rgb_speed[] =
{30,20,15,15,10,10,5,5,5,5,5,5,};

#define FREQ_VAL 1
// 红蓝白3色各10led 转动
void user_rgb_circel_mode1_1(void)
{
static u8 pos=0,i,j,m,n,cnt,l,cnt1,bDirectVal;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;

static u8 steptemp,ll;

//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;

steptemp=16*FREQ_VAL;
if(++cnt<steptemp)	return;else	cnt=0;

user_rgb_clear_colour();

#if 1  // 1   // 16*10ms 周期160ms
l=62;
	
	for(i=pos;i<pos+l;i++)
	{		
		if(i>l-1) m=i-l;else m=i;// 取显示位置
		colour.g= rgb_buf_set_mode1[(i-pos)/ROUND1_STEP][0];
		colour.r= rgb_buf_set_mode1[(i-pos)/ROUND1_STEP][1];
		colour.b= rgb_buf_set_mode1[(i-pos)/ROUND1_STEP][2];	
		if(bDirectVal==1)//反转
			{
				m=l-1-m;
				colour.g= rgb_buf_set_mode1_2[(i-pos)/ROUND1_STEP][0];
				colour.r= rgb_buf_set_mode1_2[(i-pos)/ROUND1_STEP][1];
				colour.b= rgb_buf_set_mode1_2[(i-pos)/ROUND1_STEP][2];					
			}
		 user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mod_circel[m]);		
	}
	
	if(pos<ll-1)
		pos++;
	else	
		{
			pos=0;		
			cnt1++;
			if(cnt1==2)
				ll=l-(ROUND1_STEP-1);
			else
				ll=l;
			if(cnt1==3)
				{
					cnt1=0;
					if(bDirectVal) 
						{
						bDirectVal=0;
						}
						else
						{
							bDirectVal=1;					
						}
				}			

		}

  user_rgb_circel_mode1_tab_vol_scan();
	
#endif	
			
}
//----------mode 1 -1 end-------------------

//----------mode 1 -2 start-------------------

void user_rgb_circel_mode1_2(void)
{
static u8 pos=0,i,j,m,n,cnt,l,cnt1,bDirectVal;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;

static u8 steptemp,ll;

//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;

steptemp=16*FREQ_VAL;

if(++cnt<steptemp)	return;else	cnt=0;

user_rgb_clear_colour();


#if 1  // 2  // 16*10ms 周期160ms

l=62;

	// 顺时针旋转
	for(i=pos;i<pos+ROUND2_MAX;i++)
	{
		use_ctl.bRgbColorVal=6;
		if((use_ctl.bRgbColorVal==6)&&(i==pos+ROUND2_MAX-1))
			colour_auto_flag=1;
		else
			colour_auto_flag=0;

			colour_auto_flag=1;

		colour_val=(i-pos);
	 	rgb_colour_set_main(use_ctl.bRgbColorVal,ROUND2_MAX+1,colour_val,colour_auto_flag);
	
		if(i>l-1) m=i-l;else m=i;
		colour.g=rgb_buf[0][0];
		colour.r=rgb_buf[0][1];
		colour.b=rgb_buf[0][2];		

		if(bDirectVal==1)//反转
			{
				m=l-1-m;				
			}		
		 user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mod_circel[m]);
		j=m+12;
		if(j>l-1)n=j-l;else n=j;
	
		user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mod_circel[n]);				
	}
	if(pos<ll-1)
		pos++;
	else	
		{
			pos=0;		
			cnt1++;
			if(cnt1==2)
				ll=l-7;
			else
				ll=l;
			if(cnt1==3)
				{
					cnt1=0;
					if(bDirectVal) 
						{
						bDirectVal=0;
						}
						else
						{
							bDirectVal=1;					
						}
				}			

		}

  user_rgb_circel_mode1_tab_vol_scan();

#endif	

			
}
//----------mode 1 -2 end-------------------


//----------mode 1-3 start-------------------

const u8 rgb_mode1_up_l[] = 
{
0,1,2,3,4,5
};
const u8 rgb_mode1_up_r[] = 
{
23,22,21,20,19,18
};
const u8 rgb_mode1_dn_l[] = 
{
11,10,9,8,7,6
};
const u8 rgb_mode1_dn_r[] = 
{
12,13,14,15,16,17
};
void user_rgb_circel_mode1_3(void) // 10ms
{
static u8 pos=0,i,j,m,n,cnt,l,cnt1;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;

static u8 steptemp;

//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;

steptemp=16*FREQ_VAL;

if(++cnt<steptemp)	return;else	cnt=0;

user_rgb_clear_colour();

#if 1  // 3 // 20*10ms  周期15
static u8 pos1=0,bLrCnt,bDirectCnt,bDirect,postemp=0,bCnt;

l=6;

{
	 	if(bDirectCnt<4)
	 	{
	 		// 从上到下逐个亮0-3
	 		for(i=0;i<pos;i++)
	 		{
		 		pos1=1;	
				use_ctl.bRgbColorVal=6;
				colour_auto_flag=1;
				colour_val=i;
			 	rgb_colour_set_main(use_ctl.bRgbColorVal,l/2,colour_val,colour_auto_flag);
					 		
	 			colour.g=rgb_buf[0][0];
	 			colour.r=rgb_buf[0][1];
	 			colour.b=rgb_buf[0][2];
				 m=i; 
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_r[m]);			
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_r[m]);			
	 		}	
	 	
	 		if(pos<4)pos++;else	pos=1;	
	 	}
	 	else if(bDirectCnt<8)
	 	{ //  6 7 8 9 10 11
		 	pos=3;
			if(pos1<=2)
				postemp=pos1+4;
			else
				postemp=6;
				
	 		// 从上到下逐个移动1-4 到45跟下面67叠加总数4
	 		for(i=pos1;i<postemp;i++)
	 		{
				colour_auto_flag=1;
				colour_val=i;
				colour_val=i;
			 	rgb_colour_set_main(use_ctl.bRgbColorVal,l/2,colour_val,colour_auto_flag);	 		
	 			colour.g=rgb_buf[0][0];
	 			colour.r=rgb_buf[0][1];
	 			colour.b=rgb_buf[0][2];
				if(bDirect<2) m=i; else m=5-i;				
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_r[m]);			
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_r[m]);			
	 		}
		
	 		if(pos1<5)pos1++;else	pos1=0;				
	 	}
	 	else if(bDirectCnt<10)
	 	{
	 		pos1=1;	
	 		//  从下到上弹回去2次 从亮54 到亮5432 
	 		for(i=0;i<pos;i++)
	 		{
				use_ctl.bRgbColorVal=6;
				colour_auto_flag=1;
				colour_val=i;
			 	rgb_colour_set_main(use_ctl.bRgbColorVal,l/2,colour_val,colour_auto_flag);
					 		
	 			colour.g=rgb_buf[0][0];
	 			colour.r=rgb_buf[0][1];
	 			colour.b=rgb_buf[0][2];
				 m=5-i;
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_r[m]);			
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_r[m]);			
	 		}	
	 	
	 		if(pos<4)pos++;else	pos=1;	
	 	}
	 	else if(bDirectCnt<15)
	 	{ //   
		 	pos=0;
	 		// 从上到下逐个移动1-4 到45跟下面67叠加总数4
			if(pos1<=2)
				postemp=pos1+4;
			else
				postemp=6;	 		
	 		for(i=pos1;i<postemp;i++)
	 		{
				colour_auto_flag=1;
				colour_val=i;
				colour_val=i;
			 	rgb_colour_set_main(use_ctl.bRgbColorVal,l/2,colour_val,colour_auto_flag);	 		
	 			colour.g=rgb_buf[0][0];
	 			colour.r=rgb_buf[0][1];
	 			colour.b=rgb_buf[0][2];
				 m=5-i;				
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_up_r[m]);			
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_l[m]);	
	 			user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mode1_dn_r[m]);			
	 		}
		
	 		if(pos1<6)pos1++;else	pos1=0;				
	 	}
		
		// 15*2+13+15*3+13
		if(bDirectCnt<15)  
			bDirectCnt++; 
		else // 15+
		{ 
			
			bCnt++;
			if(bCnt==1) // +15
   			 bDirectCnt=0;
			else if(bCnt==2) 
   			 bDirectCnt=16;			
			else if(bCnt==(2+13)) // +13
   			 bDirectCnt=0;
			else if(bCnt==(2+13+1)) // +15
   			 bDirectCnt=0;							
			else if(bCnt==(2+13+2)) // +15
   			 bDirectCnt=0;	
			else if(bCnt==(2+13+3)) // +15
   			 bDirectCnt=16;	
			else if(bCnt>(2+13+3+13)) // +13
				{
		   			 bDirectCnt=0;
		   			 bCnt=0;
				}
			
		//	bDirect++;
		//	if(bDirect==8) bDirect=0;
		}

	 }



  user_rgb_circel_mode1_tab_vol_scan();
	
#endif	
			
}
//----------mode 3 end-------------------



//----------mode 1-4 start-------------------
#define ROUND4_STEP 10 // 4   62/10 =10led   // 24/6_COLOR ==4LED
void user_rgb_circel_mode1_4(void)
{
static u8 pos=0,i,j,m,n,l,cnt1,bDirectVal,ll;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;


//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;

#if 0
static u8 cnt,steptemp;

steptemp=50*FREQ_VAL;
if(++cnt<steptemp)	return;else	cnt=0;


#endif

user_rgb_clear_colour();

l=62;
	
	for(i=pos;i<pos+l;i++)
	{		
		if(i>l-1) m=i-l;else m=i;// 取显示位置
		colour.g= rgb_buf_set[(i-pos)/ROUND4_STEP][0];
		colour.r= rgb_buf_set[(i-pos)/ROUND4_STEP][1];
		colour.b= rgb_buf_set[(i-pos)/ROUND4_STEP][2];	
		if(bDirectVal==1)//反转
			{
				m=l-1-m;
				colour.g= rgb_buf_set[(i-pos)/ROUND4_STEP][0];
				colour.r= rgb_buf_set[(i-pos)/ROUND4_STEP][1];
				colour.b= rgb_buf_set[(i-pos)/ROUND4_STEP][2];					
			}
		 user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mod_circel[m]);		
	}
	
	if(pos<ll-1)
		pos++;
	else	
		{
			pos=0;		
			cnt1++;
			if(cnt1==2)
				ll=l-(ROUND4_STEP-1);
			else
				ll=l;
			if(cnt1==3)
				{
					cnt1=0;
					if(bDirectVal) 
						{
						bDirectVal=0;
						}
						else
						{
							bDirectVal=1;					
						}
				}			

		}

	user_rgb_circel_mode1_tab_vol_scan();
			
}
//----------mode 1-4 end-------------------

//----------mode 1 start-------------------

#define MODE1_1_TIMER  (62*2+40)*2*16*FREQ_VAL // 24+16+24
#define MODE1_2_TIMER  MODE1_1_TIMER+(62*2+40)*2*16*FREQ_VAL
#define MODE1_3_TIMER  MODE1_2_TIMER+(15*2+13+15*3+13)*1*16*FREQ_VAL


const u8 rgb_mode1_l[] = 
{
31,32,33,34,35,36,37,38,39,40,
41,42,43,44,45,46,47,48,49,50,
51,52,53,54,55,56,57,58,59,60,
61
};

const u8 rgb_mode1_r[] = 
{
30,29,28,27,26,25,24,23,22,21,
20,19,18,17,16,15,14,13,12,11,
10,9,8,7,6,5,4,3,2,1,0
};

const u8 rgb_speed_mode1[] =
{30,12,10,9,8,7,6,5,4,3,2,1,};
void user_rgb_circel_mode_1(void) // 10ms
{
static u16 bChangeModeCnt=0;
static u8 i,l,steptemp,cnt,bEnergyStepTemp,bEnergyStepVal,bColorcnt,cnt1,cnt2;
RGB_DEV_COLOUR colour;

static u32 stepcnt=300*FREQ_VAL;


	// bEnergyStep=1;
	
//	 	printf("\n ===333==energy=%d\n",bEnergyStep);		 			 	
		
	 if(bEnergyStep>0)
	 {
	 
	 	// steptemp=1; //使用最快速度  10ms 
	 
	 //	if(++cnt<steptemp)	return;else	cnt=0;
	 
	 
	 	bChangeModeCnt=0; // 复位无能量自动模式从起始位开始
	  
//	 	 user_rgb_clear_colour();

		user_rgb_clear_pos(RGB_CIRCEL_START,RGB_CIRCEL_START+62);
	 
	 	stepcnt=0;
	 	 bColorcnt++;
	 	 if(bColorcnt==5)
	 	 {
	 	 	bColorcnt=0;
	 	  	rgb_colour_set_main(6,10,1,1);// 后面3个参数无所谓 仅仅补缺
	 	 }
	 	 		 
	 	 rgb_buf[1][0]=0xfa-rgb_buf[0][0];
	 	 rgb_buf[1][1]=0xfa-rgb_buf[0][1];
	 	 rgb_buf[1][2]=0xfa-rgb_buf[0][2];
	 			 
	 	 for(i=0;i<bEnergyStepTemp;i++) 
	 	 {	
	 		if(i==bEnergyStepTemp-1)
	 			{
	 		 		user_rgb_colour_only_set(&rgb_buf[1],RGB_CIRCEL_START+rgb_mode1_l[i]);			 
	 		 		user_rgb_colour_only_set(&rgb_buf[1],RGB_CIRCEL_START+rgb_mode1_r[i]);				
	 			}
	 		else
	 			{
	 		 		user_rgb_colour_only_set(&rgb_buf[0],RGB_CIRCEL_START+rgb_mode1_l[i]);			 
	 		 		user_rgb_colour_only_set(&rgb_buf[0],RGB_CIRCEL_START+rgb_mode1_r[i]);				
	 			}			
	 	 }	
	 
	 	if(bEnergyStepTemp>bEnergyStep)
	 	{
	 		 cnt1++;
		   	if(cnt1%5==1)
	   		{
				bEnergyStepTemp--;
	   		}
	 	}
	 	else if(bEnergyStepTemp<bEnergyStep)
	 	{
	 		bEnergyStepTemp++;
	 	}	


	 	steptemp=16;
	 
	 	if(++cnt2<steptemp)	return;else	cnt2=0;

		user_rgb_clear_pos(0,RGB_CIRCEL_START);
		
		user_rgb_clear_pos(RGB_CIRCEL_START+62,USER_RGB_NUMBER);

		user_rgb_circel_mode1_tab_vol_scan();
		
	 		 	 
	 }
	 else
	 {
	     if(stepcnt<300*FREQ_VAL) 	// 3ms
	     	{
	 	    	if(stepcnt==10*FREQ_VAL) 
	 	    		{
//	 				user_rgb_clear_colour();	
					user_rgb_clear_pos(RGB_CIRCEL_START,RGB_CIRCEL_START+62);
	 				user_rgb_colour_only_set(&rgb_buf[1],RGB_CIRCEL_START+rgb_mode1_l[0]);			 
	 				user_rgb_colour_only_set(&rgb_buf[1],RGB_CIRCEL_START+rgb_mode1_r[0]);							   
	 	    		}
	 		stepcnt++;
			
			 	steptemp=16;
			 
			 	if(++cnt2<steptemp)	return;else	cnt2=0;

				user_rgb_clear_pos(0,RGB_CIRCEL_START);
				user_rgb_clear_pos(RGB_CIRCEL_START+62,USER_RGB_NUMBER);

				user_rgb_circel_mode1_tab_vol_scan();
				
	     	}
	    else if(stepcnt>=300*FREQ_VAL) // 2m
	 	{
	 		steptemp=0;		
	 		stepcnt=310*FREQ_VAL;
	 		
	 		if(bChangeModeCnt<MODE1_1_TIMER)
	 		{
	 			user_rgb_circel_mode1_1();
	 		}
	 		else if(bChangeModeCnt<MODE1_2_TIMER)
	 		{
	 			user_rgb_circel_mode1_2();
	 		}
			
#if 0			
	 		else if(bChangeModeCnt<MODE1_3_TIMER)
	 		{
	 			user_rgb_circel_mode1_3();
	 		}	
	 		if(bChangeModeCnt<MODE1_3_TIMER)
#endif

	 		if(bChangeModeCnt<MODE1_2_TIMER)
	 			bChangeModeCnt++;
	 		else
	 			bChangeModeCnt=0;

	 
	    	}
	 }
	 			
}



void user_rgb_circel_mode_2(void) // 10ms
{
static u16 bChangeModeCnt=0;
static u8 i,l,steptemp,cnt,bEnergyStepTemp,bEnergyStepVal,bColorcnt,cnt1;
RGB_DEV_COLOUR colour;

static u32 stepcnt;


	// bEnergyStep=1;
	
	// 	printf("\n ===333==energy=%d\n",bEnergyStep);		 

	
	 	steptemp=rgb_speed_mode1[bEnergyStep];
	 
	 	if(++cnt<steptemp)	return;else	cnt=0;

		user_rgb_circel_mode1_4();	

	 			
}


#if 0
const u8 rgb_mod_circel_bar[] = 
{
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
};


#define ROUND3_STEP 6
void user_rgb_circel_mode_3_bar() 
{
static u8 pos=0,i,j,m,n,l,cnt1,bDirectVal,ll;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;


//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;

#if 1
static u8 cnt,steptemp;


steptemp=24;
if(++cnt<steptemp)	return;else	cnt=0;

  user_rgb_clear_pos(0,RGB_CIRCEL_START-1);

#endif

l=16;
	
	for(i=0;i<=pos;i++)
	{		
		if(i>l-1) m=i-l;else m=i;// 取显示位置
		colour.g= rgb_buf_set[(i)%ROUND3_STEP][0];
		colour.r= rgb_buf_set[(i)%ROUND3_STEP][1];
		colour.b= rgb_buf_set[(i)%ROUND3_STEP][2];	

		 user_rgb_colour_only_set(&colour,rgb_mod_circel_bar[m]);		
	}
	
	if((pos<l-1)&&(bDirectVal==0))
		pos++;
	else if((pos==(l-1))&&(bDirectVal==0))
	{
		bDirectVal=1;
		pos--;
	}
	else if((pos>0)&&(bDirectVal==1))
		{
			pos--;
		}
	else if((pos==0)&&(bDirectVal==1))
		{
			pos++;
			bDirectVal=0;
			use_ctl.spi_rgb5050_cur_mode=RGB_MODE_1;
		}
			
}

void user_rgb_circel_mode_3_circle() 
{
static u8 pos=0,i,j,m,n,l,cnt1,bDirectVal,ll;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;


//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;

#if 1
static u8 cnt,steptemp;


steptemp=16;
if(++cnt<steptemp)	return;else	cnt=0;

  user_rgb_clear_pos(RGB_CIRCEL_START,RGB_CIRCEL_START+24);

#endif

l=24;
	
	for(i=0;i<=pos;i++)
	{		
		if(i>l-1) m=i-l;else m=i;// 取显示位置
		colour.g= rgb_buf_set[(i)%ROUND3_STEP][0];
		colour.r= rgb_buf_set[(i)%ROUND3_STEP][1];
		colour.b= rgb_buf_set[(i)%ROUND3_STEP][2];	

		 user_rgb_colour_only_set(&colour,RGB_CIRCEL_START+rgb_mod_circel[m]);		
	}
	
	if((pos<l-1)&&(bDirectVal==0))
		pos++;
	else if((pos==(l-1))&&(bDirectVal==0))
	{
		bDirectVal=1;
		pos--;
	}
	else if((pos>0)&&(bDirectVal==1))
		{
			pos--;
		}
	else if((pos==0)&&(bDirectVal==1))
		{
			pos++;
			bDirectVal=0;
			use_ctl.spi_rgb5050_cur_mode=RGB_MODE_1;
		}
			
}


void user_rgb_circel_mode_3(void) // 10ms
{
static u8 pos=0,i,j,m,n,l,cnt1,bDirectVal,ll;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;


//steptemp=rgb_speed[bEnergyStep];

bool colour_auto_flag;
static u8 colour_val;

#if 0
static u8 cnt,steptemp;

if(use_ctl.spi_rgb5050_cur_mode!=RGB_MODE_3) return;

steptemp=50*FREQ_VAL;
if(++cnt<steptemp)	return;else	cnt=0;

#endif

user_rgb_circel_mode_3_bar();
user_rgb_circel_mode_3_circle();
			
}

#endif

#endif




AT(.volatile_ram_code)
void user_rgb_scan_mode(void){
    if(!_this_->info->init_flag){
        g_printf("*");
        return;
    }

    wdt_clear();
    // _this_->mode_scan_ms = 500;
    static u32 time_ = 0;
    if(!_this_->mode_scan_ms || (timer_get_ms()-time_<_this_->mode_scan_ms) || _this_->info->updata_flag || _this_->info->rend_flag){
        return;
    }else{
        time_ = timer_get_ms();     
    }
	

//	light_mode_mark = RGB_MODE_3;

    switch (light_mode_mark){
    case RGB_OFF:
        user_rgb_clear_colour();
		user_rgb_send_cnt(1);
        break;
	case RGB_MODE_1:// nosingle  auto 1-3  15s else  jump freq
	       user_rgb_circel_mode_1();
		user_rgb_send_cnt(1);
	break;	
	
	case RGB_MODE_2:// nosingle  auto 1-3  15s else  jump freq
	       user_rgb_circel_mode_2();
		user_rgb_send_cnt(1);
	break;		
	
	case RGB_MODE_3:// poweron  rgb mode 
//	       user_rgb_circel_mode_3();
		user_rgb_send_cnt(1);
	break;		
	
    default:
        break;
    }
}
//??????????
void user_rgb_fun_init(void){
    if(user_rgb_info.init_flag){
        return;
    }

	use_ctl.bRgbColorVal=6;

    //rgb spi init
    user_rgb_dev_init(&user_rgb_info);

    //??????????????spi????????
   // user_rgb_send_cnt(1);

}

//??????
void user_rgb_fun_exit(void){
    r_printf("rgb fun exit");
    user_rgb_dev_exit();
}
void user_fun_init(void){
   // r_printf("user fun init in");
    user_rgb_fun_init();

    //usr_timer_add(NULL,user_rgb_scan_mode,10,1);		
   // r_printf("user fun init ok");
}




