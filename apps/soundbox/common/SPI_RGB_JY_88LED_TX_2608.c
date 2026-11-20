#include "spi_rgb_config.h"

#define USER_RGB_NUMBER (24+64)

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
#ifdef SUPPORT_KEY_RGB_

#define KEY_RGB_LIST(x)  u8 key##x##_rgb_list[]={SUPPORT_KEY##x##_RGB_}
#define KEY_RGB_LIST_ENUM(x)  key##x##_rgb_list


#ifdef SUPPORT_KEY_VOL_AD_RGB_
KEY_RGB_LIST(_VOL_AD);
#endif

#ifdef SUPPORT_KEY_MIC_AD_RGB_
KEY_RGB_LIST(_MIC_AD);
#endif

#ifdef SUPPORT_KEY_ECHO_AD_RGB_
								KEY_RGB_LIST(_ECHO_AD);

								
#endif	
	
#ifdef SUPPORT_KEY_M_TRE_AD_RGB_
									KEY_RGB_LIST(_M_TRE_AD);

									
#endif	
	
#ifdef SUPPORT_KEY_GT_AD_RGB_
									KEY_RGB_LIST(_GT_AD);

									
#endif	
	
#ifdef SUPPORT_KEY_M_BAS_AD_RGB_
									KEY_RGB_LIST(_M_BAS_AD);

									
#endif	

#ifdef SUPPORT_KEY_TRE_AD_RGB_
										KEY_RGB_LIST(_TRE_AD);

										
#endif	
		
#ifdef SUPPORT_KEY_BAS_AD_RGB_
										KEY_RGB_LIST(_BAS_AD);

										
#endif	

#ifdef SUPPORT_KEY0_RGB_
KEY_RGB_LIST(0);
#endif

#ifdef SUPPORT_KEY1_RGB_
KEY_RGB_LIST(1);
#endif

#ifdef SUPPORT_KEY2_RGB_
KEY_RGB_LIST(2);
#endif

#ifdef SUPPORT_KEY3_RGB_
KEY_RGB_LIST(3);
#endif

#ifdef SUPPORT_KEY4_RGB_
KEY_RGB_LIST(4);
#endif

#ifdef SUPPORT_KEY5_RGB_
KEY_RGB_LIST(5);
#endif

#ifdef SUPPORT_KEY6_RGB_
KEY_RGB_LIST(6);
#endif

#ifdef SUPPORT_KEY7_RGB_
KEY_RGB_LIST(7);
#endif

#ifdef SUPPORT_KEY8_RGB_
KEY_RGB_LIST(8);
#endif

#ifdef SUPPORT_KEY9_RGB_
KEY_RGB_LIST(9);
#endif

u8 *key_rgb_list[]={	

#ifdef SUPPORT_KEY0_RGB_
							KEY_RGB_LIST_ENUM(0),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY1_RGB_
							KEY_RGB_LIST_ENUM(1),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY2_RGB_
							KEY_RGB_LIST_ENUM(2),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY3_RGB_
							KEY_RGB_LIST_ENUM(3),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY4_RGB_
							KEY_RGB_LIST_ENUM(4),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY5_RGB_
							KEY_RGB_LIST_ENUM(5),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY6_RGB_
							KEY_RGB_LIST_ENUM(6),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY7_RGB_
							KEY_RGB_LIST_ENUM(7),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY8_RGB_
							KEY_RGB_LIST_ENUM(8),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY9_RGB_
							KEY_RGB_LIST_ENUM(9),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY_VOL_AD_RGB_
							KEY_RGB_LIST_ENUM(_VOL_AD),
#else
							NULL,
#endif

#ifdef SUPPORT_KEY_MIC_AD_RGB_
							KEY_RGB_LIST_ENUM(_MIC_AD),
#else
							NULL,
#endif	

#ifdef SUPPORT_KEY_ECHO_AD_RGB_
								KEY_RGB_LIST_ENUM(_ECHO_AD),
#else
								NULL,
#endif	
	
#ifdef SUPPORT_KEY_M_TRE_AD_RGB_
									KEY_RGB_LIST_ENUM(_M_TRE_AD),
#else
									NULL,
#endif	
	
#ifdef SUPPORT_KEY_GT_AD_RGB_
									KEY_RGB_LIST_ENUM(_GT_AD),
#else
									NULL,
#endif	
	
#ifdef SUPPORT_KEY_M_BAS_AD_RGB_
									KEY_RGB_LIST_ENUM(_M_BAS_AD),
#else
									NULL,
#endif	

#ifdef SUPPORT_KEY_TRE_AD_RGB_
										KEY_RGB_LIST_ENUM(_TRE_AD),
#else
										NULL,
#endif	
		
#ifdef SUPPORT_KEY_BAS_AD_RGB_
										KEY_RGB_LIST_ENUM(_BAS_AD),
#else
										NULL,
#endif	

};

bool user_rgb_adkey_get_(u8 cnt)
{
	if ( cnt >= (ARRAY_SIZE(key_rgb_list)-2) )
	{
		return false;
	}
	if (key_rgb_list[cnt]==NULL)
	{
		return false;
	}
	return true;
}
bool user_rgb_adc_get_(u8 cnt)
{
	if ( cnt >= (ARRAY_SIZE(key_rgb_list)-10) )
	{
		return false;
	}
	if (key_rgb_list[cnt+10]==NULL)
	{
		return false;
	}
	return true;
}

#endif

#ifdef SUPPORT_KEY_RGB_ADKEY
static u8 kkkkkkkkkkkkkkkkk=0;
void user_KEY_RGB_ADKEY_cnt()
{
	
			
			if (kkkkkkkkkkkkkkkkk>60)
				kkkkkkkkkkkkkkkkk=0;
			
#ifdef SUPPORT_KEY_RGB_ADKEY_MUSIC_PP
			if (app_get_curr_task()==APP_MUSIC_TASK)
			{
				#include "audio_dec_file.h"
				if (true == file_dec_is_pause()) 
				{
					if (kkkkkkkkkkkkkkkkk>30)
						kkkkkkkkkkkkkkkkk=0;
				}
				if (kkkkkkkkkkkkkkkkk>20)
				{
					if ( key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY] )
					{
						for (int j = 0;   ; ++	j)
						{
							if( key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j] && (key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j]<USER_RGB_NUMBER) )
							{
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].r=0;
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].g=0;
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].b=0;
							}
							else
							{
								break;
							}
						}
					}

				}
			}
#endif

#ifdef SUPPORT_KEY_RGB_ADKEY_LINEIN_PP

#if defined board_ac695x_JY_tx_2608_88led_ac6951c|| defined board_ac695x_JY_tx_2610_7_4v_88led_ac6951c\
	|| defined board_ac695x_JY_TX_2610_12v_88led_AC6951C
			JL_PORTA->DIR |= BIT(0);JL_PORTA->DIE |= BIT(0);JL_PORTA->PU |= BIT(0);JL_PORTA->PD &= ~BIT(0);
			if ( (app_get_curr_task()==APP_LINEIN_TASK) && !(JL_PORTA->IN  &  BIT(0)))
#endif
			{
				if ( linein_get_status() ) ;
				else
				{
					if (kkkkkkkkkkkkkkkkk>30)
						kkkkkkkkkkkkkkkkk=0;
				}
				if (kkkkkkkkkkkkkkkkk>20)
				{
					if ( key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY] )
					{
						for (int j = 0;   ; ++	j)
						{
							if( key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j] && (key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j]<USER_RGB_NUMBER) )
							{
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].r=0;
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].g=0;
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].b=0;
							}
							else
							{
								break;
							}
						}
					}
				}
			}
#endif
#ifdef SUPPORT_KEY_RGB_ADKEY_BT_PP
			if ((app_get_curr_task()==APP_BT_TASK) && (get_bt_connect_status()>=BT_STATUS_CONNECTING))
			{
				if ( get_bt_connect_status()>BT_STATUS_CONNECTING) ;
				else
				{
					if (kkkkkkkkkkkkkkkkk>30)
						kkkkkkkkkkkkkkkkk=0;
				}
				if (kkkkkkkkkkkkkkkkk>20)
				{
					if ( key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY] )
					{
						for (int j = 0;   ; ++	j)
						{
							if( key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j] && (key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j]<USER_RGB_NUMBER) )
							{
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].r=0;
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].g=0;
								_this_rgb->rgb_buff[(key_rgb_list[SUPPORT_KEY_RGB_ADKEY_PP_KEY][j])-1].b=0;
							}
							else
							{
								break;
							}
						}
					}

				}
			}
#endif
			
			if (kkkkkkkkkkkkkkkkk%30>10)
			{
				
				if (use_ctl.key_rgb_cnt_adkey )
				{

					if ( key_rgb_list[use_ctl.key_rgb_adkey])
					{
						for (int j = 0;   ; ++	j)
						{
							if( key_rgb_list[use_ctl.key_rgb_adkey][j] && (key_rgb_list[use_ctl.key_rgb_adkey][j]<USER_RGB_NUMBER) )
							{
								_this_rgb->rgb_buff[(key_rgb_list[use_ctl.key_rgb_adkey][j])-1].r=0;
								_this_rgb->rgb_buff[(key_rgb_list[use_ctl.key_rgb_adkey][j])-1].g=0;
								_this_rgb->rgb_buff[(key_rgb_list[use_ctl.key_rgb_adkey][j])-1].b=0;
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						use_ctl.key_rgb_cnt_adkey = 0;
					}
				}
				
			}
	
}
#endif

#ifdef SUPPORT_KEY_RGB_ADC
static u8 kkkkkkkkk2kkkkkkkk=0;
#endif

//????????????????????
//0 ??????cnt???????? ????????????????????
//??????????????????????????????
void user_rgb_send_cnt(u8 cnt){
        // return;
    if(!_this_rgb){
        return;
    }
#ifdef SUPPORT_KEY_RGB_

	user_KEY_RGB_ADKEY_cnt();

#ifdef SUPPORT_KEY_RGB_ADC
		u8 cccccccccccc=0;
		if ( use_ctl.key_rgb_cnt_adc && use_ctl.key_rgb_adc )
		{
			
			if (kkkkkkkkk2kkkkkkkk>10)
			{
				if (kkkkkkkkk2kkkkkkkk>30)
					kkkkkkkkk2kkkkkkkk=0;
				for (cccccccccccc = 0;   cccccccccccc < 16; ++  cccccccccccc)
				{
					if (use_ctl.key_rgb_adc & BIT(cccccccccccc))
					{
						break;
					}
				}
				cccccccccccc += 10;
				if ( cccccccccccc<ARRAY_SIZE(key_rgb_list) && key_rgb_list[cccccccccccc] )
				{
					for (int j = 0;   ; ++	j)
					{
						if( key_rgb_list[cccccccccccc][j] && (key_rgb_list[cccccccccccc][j]<USER_RGB_NUMBER) )
						{
							_this_rgb->rgb_buff[(key_rgb_list[cccccccccccc][j])-1].r=0;
							_this_rgb->rgb_buff[(key_rgb_list[cccccccccccc][j])-1].g=0;
							_this_rgb->rgb_buff[(key_rgb_list[cccccccccccc][j])-1].b=0;
						}
						else
						{
							break;
						}
					}
				}
				
				else
				{
					use_ctl.key_rgb_adc=0;
					use_ctl.key_rgb_cnt_adc = 0;
				}
			}
			
		}
#endif

#endif

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


void rgb_marquee_mode(u8 colour[][3],u8 marquee_num,u8 _start,u8 _end)
{
	static u8 colour_number,run_dir,flow_mode_time;
	static u16 change_dir_cnt;
	static s8 old_colour;
	u8 now_colour,light_num;
	static s8 run_num=0;
	
	RGB_DEV_COLOUR tp_rgb;

	u8 c_be = get_energyStep(MAX_ENERGY);
	//??????????
	if(change_dir_cnt++>2000){
		change_dir_cnt = 0;
	run_dir =! run_dir;
	}
	
	//??????????????????????????????
	colour_number=marquee_num;
	
	light_num = (_end + 1 - _start)/colour_number;

	if((_end + 1 - _start)%colour_number){
		light_num++;
	}

	//if(flow_mode_time++<((MAX_ENERGY*10)/((c_be+1)*4))) return;
	if(flow_mode_time++<10) return;
	flow_mode_time=0;

	for(int i=_start;i<_end+1;i++)
	{
		if((i-_start)<run_num)//??????????????
		{
			now_colour=(old_colour+marquee_num)%colour_number;
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(light_num+run_num))//????????????????????
		{
			now_colour=(old_colour+marquee_num-1)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}


		else if((i-_start)<(2*light_num+run_num))//????????????????????
		{
			now_colour=(old_colour+marquee_num-2)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(3*light_num+run_num))//??????????????????????
		{
			now_colour=(old_colour+marquee_num-3)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(4*light_num+run_num))//??????????????4????
		{
			now_colour=(old_colour+marquee_num-4)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(5*light_num+run_num))//??????????????5????
		{
			now_colour=(old_colour+marquee_num-5)%colour_number;

			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else //??????????????????????
		{
			now_colour=old_colour;
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		user_rgb_colour_only_set(&tp_rgb,i);
	}

	if(run_dir){
		run_num++;
		if(run_num>=light_num)
		{
			run_num=0;
			old_colour++;
			if(old_colour>=colour_number)
			{
				old_colour=0;
			}
		}
	}else{
		run_num--;
		if(run_num<0)
		{
			run_num=light_num-1;
			old_colour--;
			if(old_colour<0)
			{
				old_colour=colour_number-1;
			}
		}
	}

}


void music_dn_to_up_mode(u8 _start,u8 _end,RGB_DEV_COLOUR color){
	static u8 pos,cur_be,old_be,count_to_add,light_number;
	//RGB_DEV_COLOUR color2;
	u8 kkkkkkkk=MAX(_start, _end)-MIN(_start, _end);
	u8 c_be = get_energyStep(kkkkkkkk+1);
	//color = get_gradual_change_color();

	cur_be=c_be;

	light_number = abs(_end - _start) + 1;
	
	if(old_be<=cur_be){
		pos=(cur_be*light_number)/kkkkkkkk;
		if(pos<1)pos=1;
		if(cur_be<1)cur_be=1;
		old_be=cur_be;
	}else{
		if(count_to_add++> (kkkkkkkk/4) ){
			count_to_add=0;
			if(pos>2)pos--;
			else pos=1;
			old_be=pos*kkkkkkkk/light_number;
			if(old_be<1)old_be=1;
		}
	}

	if (pos > light_number)
	{
		pos = light_number;
	}
	
	if( _end > _start ){
		for(int i=_start;i< (pos+_start) ;i++)
		{
			user_rgb_colour_only_set(&color,i);
#if 1			
			if(i == (pos+_start-1))
			{
				color.r = 0xff - color.r;
				color.g = 0xff - color.g;
				color.b = 0xff - color.b;
				user_rgb_colour_only_set(&color,i);
			}
#endif			
		}
	}else{
		for(int i=_start;i> (_start-pos) ;i--)
		{
			user_rgb_colour_only_set(&color,i);
#if 1			
			if(i == (_start-pos+1))
			{
				color.r = 0xff - color.r;
				color.g = 0xff - color.g;
				color.b = 0xff - color.b;
				user_rgb_colour_only_set(&color,i);
			}
#endif			
		}
	}
	
}






u8 get_energy_2(u8 kkkkkkkk){
	static u8 pos,cur_be,old_be,count_to_add,light_number;
	
	u8 c_be = get_energyStep(kkkkkkkk);
	

	cur_be=c_be;

	light_number = kkkkkkkk;
	
	if(old_be<=cur_be){
		pos=(cur_be*light_number)/kkkkkkkk;
		if(pos<1)pos=1;
		if(cur_be<1)cur_be=1;
		old_be=cur_be;
	}else{
		if(count_to_add++> (kkkkkkkk*2/3) ){
			count_to_add=0;
			if(pos>2)pos--;
			else pos=1;
			old_be=pos*kkkkkkkk/light_number;
			if(old_be<1)old_be=1;
		}
	}

	return pos;
	
}



void light_flash(u8 _start,u8 _end,RGB_DEV_COLOUR color){
	static u16 cu_status;
	RGB_DEV_COLOUR clean_color={0,0,0};

	clear_color(_start,_end);
	
	if(cu_status++>590)cu_status=0;

	if(cu_status%150 < 20){
		set_color(_start,_end, color);
	}
}

RGB_DEV_COLOUR get2_gradual_change_color(){
	static u8 X_STATUS=status_bga;
	static RGB_DEV_COLOUR DCOLOR={0,0,0xff};

	switch(X_STATUS){
	case status_rba:
		DCOLOR.b++;
		if(DCOLOR.b>=0xff){
			X_STATUS++;
		}
		DCOLOR.g=0;
	break;
	case status_brs:
		DCOLOR.r--;
		if(DCOLOR.r<=0){
			X_STATUS++;
		}
		DCOLOR.g=0;
	break;
	case status_bga:
		DCOLOR.g++;
		if(DCOLOR.g>=0xff){
			X_STATUS++;
		}
		DCOLOR.r=0;
	break;
	case status_gbs:
		DCOLOR.b--;
		if(DCOLOR.b<=0){
			X_STATUS++;
		}
		DCOLOR.r=0;
	break;
	case status_gra:
		DCOLOR.r++;
		if(DCOLOR.r>=0xff){
			X_STATUS++;
		}
		DCOLOR.b=0;
	break;
	case status_rgs:
		DCOLOR.g--;
		if(DCOLOR.g<=0){
			X_STATUS=0;
		}
		DCOLOR.b=0;

	break;
	default:
		break;
	}

	return DCOLOR;
}

RGB_DEV_COLOUR get_gradual_change_color(){
	static u8 X_STATUS=status_bga;
	static RGB_DEV_COLOUR DCOLOR={0,0,0xff};

	switch(X_STATUS){
	case status_rba:
		DCOLOR.b++;
		if(DCOLOR.b>=0xff){
			X_STATUS++;
		}
		DCOLOR.g=0;
	break;
	case status_brs:
		DCOLOR.r--;
		if(DCOLOR.r<=0){
			X_STATUS++;
		}
		DCOLOR.g=0;
	break;
	case status_bga:
		DCOLOR.g++;
		if(DCOLOR.g>=0xff){
			X_STATUS++;
		}
		DCOLOR.r=0;
	break;
	case status_gbs:
		DCOLOR.b--;
		if(DCOLOR.b<=0){
			X_STATUS++;
		}
		DCOLOR.r=0;
	break;
	case status_gra:
		DCOLOR.r++;
		if(DCOLOR.r>=0xff){
			X_STATUS++;
		}
		DCOLOR.b=0;
	break;
	case status_rgs:
		DCOLOR.g--;
		if(DCOLOR.g<=0){
			X_STATUS=0;
		}
		DCOLOR.b=0;

	break;
	default:
		break;
	}

	return DCOLOR;
}

static const u16 _dvol_table[] = {
	220,//0	, //0
	220 , //1
	480 , //2
	600 , //3
	700 , //4
	800 , //5
	1200 , //6
	2500 , //7
	3200 , //8
	4800 , //9
	5300 , //10
	6000 , //11
	6500 , //12
	7000 , //13
	7500 , //14
	8000, //15
	8400, //16
	8800, //17
	9200, //18
	9600, //19
	10000, //20
	10400, //21
	10800, //22
	11200, //23
	11600, //24
	12000, //25
	12400, //26
	12800, //27
	13200,//28
	13600,//29
	14800,//30
	15584 //31
};

u8 get_energyStep(u8 max_energy){
	u8 bE=0,micbE=0,b_E;
	static u8 kcnt;
	static u8 k_cnt=0,k2_cnt=0;
	static u8 bEkcnt[6],led_rgb_vol;
	u16 exenergy_value;
	u16 energy_value=0;
	energy_value = audio_dac_energy_get();	

	if(!(use_ctl.led_vol)){
		if(kcnt++>50){
			bE=bEkcnt[k_cnt]=0;
			bE=0;
			kcnt = 0;
		}
		bE=bEkcnt[k_cnt];
	}else{

		if (use_ctl.led_vol<5)
			bE = energy_value * max_energy / (_dvol_table[use_ctl.led_vol*use_ctl.dac_sys_max_vol/LED_SHOW_MAX_VOL]);
		else
			bE = energy_value * max_energy / ( _dvol_table[use_ctl.led_vol*use_ctl.dac_sys_max_vol/LED_SHOW_MAX_VOL] /2);
	
		if(bE>max_energy)bE=max_energy;
		
		if (led_rgb_vol!=use_ctl.led_vol)
		{
			led_rgb_vol=use_ctl.led_vol;
			for (int i = 0;   i < 6; ++  i)
			{
				bEkcnt[i]=bE;
			}
			
		}
		else
		{
			k_cnt++;
			if (k_cnt==3)
			{
				k_cnt=1;
				bEkcnt[0]=bEkcnt[2];
			}
			bEkcnt[k_cnt]=bE;
			
			if ( max_energy==bE )
			{
				if (k2_cnt>1)
				{
					bEkcnt[k_cnt]=0;
					bE=0;
					k2_cnt=0;
				}
				else if (k2_cnt)
				{
					bE=max_energy/2;
				}
				else
				{
					k2_cnt=0;
				}
				k2_cnt++;
			}
			else
			{
				k2_cnt=0;
				if (energy_value	&&	bEkcnt[k_cnt-1]==bE)
				{
					if( energy_value > exenergy_value + (_dvol_table[use_ctl.led_vol]/2) /max_energy )
					{
						if (bE>1)	bE-=2;
						else	bE=1;
					}		
					else if(bEkcnt[k_cnt-2]!=bE)
					{
						if (bE+1<max_energy)	bE=bE+2;
					}		
				}
			}

		}
	}
	exenergy_value	=	energy_value;
	int micb_E=mic_energy_get();
	if (use_ctl.mic_is_online)
	{
		micb_E = micb_E * max_energy / (11200);
	}
	bE=MAX(micb_E, bE);
	if(bE>max_energy)bE=max_energy;
	return bE;
}

/***************************************************************************
void rgb_roll_main(u8 *roll_buf,RGB_DEV_COLOUR *colour,u8 max,u8 num,bool direct)
参数 :
       roll_buf:数组排序RGB
	max:滚动总共灯数量
	num:滚动灯数量
	direct:转动方向  1-顺时针0-逆时针
**************************************************************************/

const u8 rgb_roll_buf_l[] = 
{
0,1,2,3,4,5,6,7,8,9,10,11,12,13,
};
const u8 rgb_roll_buf_r[] = 
{
14,15,16,17,18,19,20,21,22,23,24,25,26,27
};

#define RGB_ROLL_MAX  14
#define RGB_ROLL_NUM 6

#define RGB_6_13_mode			64
#define RGB_6_13_mode_h_addr			24
#define RGB_6_13_mode_e_addr			(RGB_6_13_mode_h_addr+RGB_6_13_mode-1)

void set_color_0(u8 _start,u8 _end){
	RGB_DEV_COLOUR _color={0,0,0};

    for(s8 i=_start;i<_end+1;i++){
		  user_rgb_colour_only_set(&_color,i);
	}
}

void no_music_circle_and_music_jump(){
	u8 energy;
	static u16 time_to_change;
	static bool n_status;
	static u8 add_cnt,pos,i,bSpeed;
	energy = audio_dac_energy_get()+abs(use_ctl.mic_energy);
	RGB_DEV_COLOUR color = get_gradual_change_color();

	//printf("get_energyStep==%d\n",energy);

	if(energy){
		n_status = 1;
		time_to_change = 0;
		set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);
		music_dn_to_up_mode(RGB_6_13_mode/2-1+RGB_6_13_mode_h_addr	,	RGB_6_13_mode_h_addr,						color);
		music_dn_to_up_mode(RGB_6_13_mode/2+RGB_6_13_mode_h_addr	,	RGB_6_13_mode_h_addr+RGB_6_13_mode-1,			color);
	}else{
		if(n_status)
		{
			if(time_to_change++<500)return;
			time_to_change = 0;
			n_status = 0;
		}
//		rgb_marquee_mode(color,3,0,14);


		rgb_marquee_mode(rgbycp_six_color2,6,RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);
	//	rgb_marquee_mode(rgbycp_six_color2,3,14,27);
		
//		 bSpeed=50;
//	        user_rgb_clear_colour();
//		 rgb_roll_speed_main(rgb_roll_buf_l,&color,RGB_ROLL_MAX,RGB_ROLL_NUM,0,bSpeed);
//		 rgb_roll_speed_main(rgb_roll_buf_r,&color,RGB_ROLL_MAX,RGB_ROLL_NUM,0,bSpeed);

	}
}


const u8 rgb_circel_breath_speed[] =
{3,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1};
void rgb_breath_mode()
{
static u8 be_null,cnt;
u8 c_be = get_energyStep(MAX_ENERGY);
RGB_DEV_COLOUR color;

be_null=rgb_circel_breath_speed[c_be];
if(++cnt<be_null)	return;else	cnt=0;

	color = get_gradual_change_color();
	some_light_breath(0,63,color);

}


u8 rgb_buf[2][3];
const u8 rgb_round_speed[] =
{
		18,
		19,
		18,
		15,
		14,
		11,
		10,
		9,
		9,
		6,
		6,
		4,
		4,
		2,
		2,
		1
};

RGB_DEV_COLOUR get__color1(u8 intex,u8 N_Md)
{
	RGB_DEV_COLOUR DCOLOR={0,0,0};
	u8 nn=N_Md%3;
	u8 nn2=N_Md/3;
	u16 vl=0;
	if (intex>N_Md)
	{
		return DCOLOR;
	}

	if (intex<	nn2	)
	{
		DCOLOR.g=0x0;
		DCOLOR.b=0xff*intex/nn2;
		DCOLOR.r=0xff-0xff*intex/nn2;
	}
	else if (	intex<(nn2*2	+	(nn>0)	)	)
	{
		vl=0xff*(intex-(nn2))/(nn2	+	(nn>0)	);
		if (vl>0xff)
		{
			vl=0xff;
		}
		DCOLOR.g=vl;
		DCOLOR.b=0xff	-	vl;
		DCOLOR.r=0;
	}
	else
	{
		vl=0xff*( intex - ( nn2*2	+	(nn>0) ) )/( nn2	+	( nn>1) );
		if (vl>0xff)
		{
			vl=0xff;
		}
		DCOLOR.g = 0xff	-	vl;
		DCOLOR.b = 0;
		DCOLOR.r = vl;
	}
	return DCOLOR;
}
RGB_DEV_COLOUR get__color2(u8 intex,u8 N_Md)
{
	RGB_DEV_COLOUR DCOLOR={0,0,0};
	u8 nn=N_Md%3;
	u8 nn2=N_Md/3;
	u16 vl=0;
	if (intex>N_Md)
	{
		return DCOLOR;
	}

	if (intex<	nn2	)
	{
		DCOLOR.b=0x0;
		DCOLOR.r=0xff*intex/nn2;
		DCOLOR.g=0xff-0xff*intex/nn2;
	}
	else if (	intex<(nn2*2	+	(nn>0)	)	)
	{
		vl=0xff*(intex-(nn2))/(nn2	+	(nn>0)	);
		if (vl>0xff)
		{
			vl=0xff;
		}
		DCOLOR.b=vl;
		DCOLOR.r=0xff	-	vl;
		DCOLOR.g=0;
	}
	else
	{
		vl=0xff*( intex - ( nn2*2	+	(nn>0) ) )/( nn2	+	( nn>1) );
		if (vl>0xff)
		{
			vl=0xff;
		}
		DCOLOR.b = 0xff	-	vl;
		DCOLOR.r = 0;
		DCOLOR.g = vl;
	}
	return DCOLOR;
}

static u16 be_cnt=0;
void rgb__mode_6()
{
static u8 be_null,cnt;
u8 c_be = get_energy_2(MAX_ENERGY);
RGB_DEV_COLOUR color,color2;
static u8 pos,l,m,n;

be_null=rgb_round_speed[c_be];
if(++cnt<be_null)	return;else	cnt=0;

	set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);

	be_cnt++;
	if (be_cnt>0x3FF)
	{
		be_cnt=0;
	}
	if (be_cnt&0x200)
	{
		m++;
		if (m>RGB_6_13_mode/2-1)
		{
			m=0;
			n=!n;
			l++;
			if(l>60)
				l=0;
		}
	}
	else
	{
		if (m)
		{
			m--;
		}
		else
		{
			m=RGB_6_13_mode/2-1;
			n=!n;
			l++;
			if(l>60)
				l=0;
		}
	}

	
	if (n)
	{
		color = get__color1(l,60);
		color2 = get__color2(l,60);
	}
	else
	{
		color2 = get__color1(l,60);
		color = get__color2(l,60);
	}

	for (int i = 0;   i < RGB_6_13_mode/2/2; ++  i)
	{
		if (m+i<RGB_6_13_mode/2)
		{
			_this_rgb->rgb_buff[m+i+RGB_6_13_mode_h_addr]=color;
		}
		else
		{
			_this_rgb->rgb_buff[m+i-RGB_6_13_mode/2+RGB_6_13_mode_h_addr]=color2;
		}
	}
	for (int i = 0;   i < RGB_6_13_mode/2; ++  i)
	{
		_this_rgb->rgb_buff[i+RGB_6_13_mode/2+RGB_6_13_mode_h_addr]=_this_rgb->rgb_buff[RGB_6_13_mode/2-1+RGB_6_13_mode_h_addr-i];
	}
	
}

void rgb__mode_7()
{
static u8 be_null,cnt;
u8 c_be = get_energy_2(MAX_ENERGY);
RGB_DEV_COLOUR color,color2;
static u8 pos,l,m,n;

be_null=rgb_round_speed[c_be];
if(++cnt<be_null)	return;else	cnt=0;

	set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);

	be_cnt++;
	if (be_cnt>0x3FF)
	{
		be_cnt=0;
	}
	if (be_cnt&0x200)
	{
		m++;
		if (m>RGB_6_13_mode/2-1)
		{
			m=0;
		}
	}
	else
	{
		if (m)
		{
			m--;
		}
		else
		{
			m=RGB_6_13_mode/2-1;
		}
	}
	
	l++;
	if(l>60)
		l=0;

	color = get__color1(l,60);


	for (int i = 0;   i < RGB_6_13_mode/2/3; ++  i)
	{
		if (m+i<RGB_6_13_mode/2)
		{
			_this_rgb->rgb_buff[m+i+RGB_6_13_mode/2/2+RGB_6_13_mode_h_addr]=color;
		}
		else
		{
			_this_rgb->rgb_buff[m+i-RGB_6_13_mode/2+RGB_6_13_mode/2/2+RGB_6_13_mode_h_addr]=color;
		}
	}
	for (int i = 0;   i < RGB_6_13_mode/2/2; ++  i)
	{
		_this_rgb->rgb_buff[RGB_6_13_mode-1+RGB_6_13_mode_h_addr-i]=_this_rgb->rgb_buff[RGB_6_13_mode/2-1+RGB_6_13_mode_h_addr-i];
		_this_rgb->rgb_buff[i+RGB_6_13_mode_h_addr]=_this_rgb->rgb_buff[RGB_6_13_mode/2+RGB_6_13_mode_h_addr+i];
	}
	
}

void rgb__mode_8()
{
static u8 be_null,cnt;
u8 c_be = get_energy_2(MAX_ENERGY);
RGB_DEV_COLOUR color,color2;
static u8 pos=0,l,m=RGB_6_13_mode/2/2-1,n;

be_null=rgb_round_speed[c_be];
if(++cnt<be_null)	return;else	cnt=0;

	set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);

	l++;
	if(l>60)
		l=0;

	color = get__color1(l,60);
	color2 = get__color2(l,60);



	if (n)
	{
		for (int i = 0;   i < pos; ++  i)
		{
			_this_rgb->rgb_buff[i+RGB_6_13_mode_h_addr]=color;
		}
		_this_rgb->rgb_buff[m+RGB_6_13_mode_h_addr]=color2;
	}
	else
	{
		for (int i = 0;   i < pos; ++  i)
		{
			_this_rgb->rgb_buff[RGB_6_13_mode/2/2-1+RGB_6_13_mode_h_addr-i]=color;
		}
		_this_rgb->rgb_buff[RGB_6_13_mode/2/2-1+RGB_6_13_mode_h_addr-m]=color2;
	}

	
	for (int i = 0;   i < RGB_6_13_mode/2/2; ++  i)
	{
		_this_rgb->rgb_buff[RGB_6_13_mode-1+RGB_6_13_mode_h_addr-i]=_this_rgb->rgb_buff[RGB_6_13_mode/2+RGB_6_13_mode_h_addr+i]=_this_rgb->rgb_buff[RGB_6_13_mode/2-1+RGB_6_13_mode_h_addr-i]=_this_rgb->rgb_buff[RGB_6_13_mode_h_addr+i];
	}

	if (	m == pos	)
	{
		m=RGB_6_13_mode/2/2;
		pos++;
		if (	pos == RGB_6_13_mode/2/2	)
		{
			pos=0;
			n=!n;
		}
	}
	m--;
}

void rgb__mode_9()
{
u8 be_null;
u8 c_be = get_energy_2(MAX_ENERGY);
RGB_DEV_COLOUR color,color2;
static u8 pos=0,cnt,l,m=RGB_6_13_mode/2/2-1,n;

be_null=rgb_round_speed[c_be];
if(++cnt<be_null)	return;else	cnt=0;

	set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);

	l++;
	if(l>60)
		l=0;

	color = get__color1(l,60);
	color2 = get__color2(l,60);


	be_cnt++;
	if (be_cnt>0x3FF)
	{
		be_cnt=0;
	}
	if (be_cnt&0x200)
	{
		m++;
		if (m>RGB_6_13_mode/2-1)
		{
			m=0;
		}
	}
	else
	{
		if (m)
		{
			m--;
		}
		else
		{
			m=RGB_6_13_mode/2-1;
		}
	}

	for (int i = 0;   i < RGB_6_13_mode/2/2/2; ++  i)
	{
		be_null=m+i;
		if (be_null>RGB_6_13_mode/2-1)
		{
			be_null = be_null-RGB_6_13_mode/2;
		}
		_this_rgb->rgb_buff[be_null+RGB_6_13_mode_h_addr]=color;
	}
	for (int i = 0;   i < RGB_6_13_mode/2/2/2; ++  i)
	{
		be_null=m+RGB_6_13_mode/2/2+i;
		while (be_null>RGB_6_13_mode/2-1)
		{
			be_null = be_null-RGB_6_13_mode/2;
		}
		_this_rgb->rgb_buff[be_null+RGB_6_13_mode_h_addr]=color2;
	}

	
	for (int i = 0;   i < RGB_6_13_mode/2; ++  i)
	{
		_this_rgb->rgb_buff[RGB_6_13_mode/2+RGB_6_13_mode_h_addr+i]=_this_rgb->rgb_buff[RGB_6_13_mode_h_addr+i];
	}

}

void rgb__mode_10()
{
u8 be_null;
u8 c_be = get_energy_2(MAX_ENERGY);
RGB_DEV_COLOUR color,color2;
static u8 pos=0,cnt,l,m=RGB_6_13_mode/2/2-1,n;

be_null=rgb_round_speed[c_be];
if(++cnt<be_null)	return;else	cnt=0;

	set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);

	l++;
	if(l>20)
		l=0;

	color = get__color1(l,20);

	be_cnt++;
	if (be_cnt>0x3FF)
	{
		be_cnt=0;
	}
	if (be_cnt&0x200)
	{
		m++;
		if (m>RGB_6_13_mode-1)
		{
			m=0;
		}

	}
	else
	{
		if (m)
		{
			m--;
		}
		else
		{
			m=RGB_6_13_mode-1;
		}
	}


	for (int i = 0;   i < RGB_6_13_mode;   i=i+RGB_6_13_mode/8)
	{
		be_null=m+i;
		if (be_null>RGB_6_13_mode-1)
		{
			be_null = be_null-RGB_6_13_mode;
		}
		_this_rgb->rgb_buff[be_null+RGB_6_13_mode_h_addr]=color;
		
		be_null=be_null+1; 
		_this_rgb->rgb_buff[((be_null>=RGB_6_13_mode)?(be_null-RGB_6_13_mode):be_null)+RGB_6_13_mode_h_addr]=color;
	}

}

void rgb__mode_11(){
	u8 energy;
	static u8 l,m;
	static u16 time_to_change;
	static bool n_status;
	static u8 add_cnt,pos,i,bSpeed;
	energy = audio_dac_energy_get()+abs(use_ctl.mic_energy);
	RGB_DEV_COLOUR color;

	m++;
	if (m>2)
	{
		m=0;
		l++;
		if (l>240)
		{
			l=0;
		}
	}

	color = get__color1(l,240);
	if(energy){
		n_status = 1;
		time_to_change = 0;
		set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);
		music_dn_to_up_mode(RGB_6_13_mode_h_addr+RGB_6_13_mode/2/2-1,RGB_6_13_mode_h_addr,color);
		for (int i = 0;   i < RGB_6_13_mode/2/2; ++  i)
		{
			_this_rgb->rgb_buff[RGB_6_13_mode-1+RGB_6_13_mode_h_addr-i]=_this_rgb->rgb_buff[RGB_6_13_mode/2+RGB_6_13_mode_h_addr+i]=_this_rgb->rgb_buff[RGB_6_13_mode/2-1+RGB_6_13_mode_h_addr-i]=_this_rgb->rgb_buff[RGB_6_13_mode_h_addr+i];
		}
	}else{
		if(n_status)
		{
			if(time_to_change++<500)return;
			time_to_change = 0;
			n_status = 0;
		}
		set_color(RGB_6_13_mode_h_addr, RGB_6_13_mode_e_addr, color);

	}
}

void rgb__mode_12()
{
u8 be_null;
u8 c_be = get_energy_2(MAX_ENERGY);
RGB_DEV_COLOUR color,color2;
static u8 pos=0,cnt,l,m=RGB_6_13_mode/2/2-1,n;

be_null=rgb_round_speed[c_be];
if(++cnt<be_null)	return;else	cnt=0;

	set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);

	be_cnt++;
	if (be_cnt>0x3FF)
	{
		be_cnt=0;
	}
	if (be_cnt&0x200)
	{
		m++;
		if (m>RGB_6_13_mode/2-1)
		{
			m=0;
		}
	}
	else
	{
		if (m>RGB_6_13_mode/2)
		{
			m=RGB_6_13_mode/2;
		}
		if (m)
		{
			m--;
		}
		else
		{
			m=RGB_6_13_mode/2-1;
		}
	}

	color.r=0xf0;
	color.g=0xf0;
	color.b=0x0;
	for (int i = 0;   i < RGB_6_13_mode/2/6;   i++)
	{
		be_null=m+i;
		if (be_null>RGB_6_13_mode/2-1)
		{
			be_null = be_null-RGB_6_13_mode/2;
		}
		_this_rgb->rgb_buff[be_null+RGB_6_13_mode_h_addr]=color;
	}
	color.r=0;
	color.g=0x20;
	color.b=0xf0;
	for (int i = 0;   i < RGB_6_13_mode/2/6;   i++)
	{
		be_null=m+i+RGB_6_13_mode/3;
		while (be_null>RGB_6_13_mode/2-1)
		{
			be_null = be_null-RGB_6_13_mode/2;
		}
		_this_rgb->rgb_buff[be_null+RGB_6_13_mode_h_addr]=color;
	}
	color.r=0xf0;
	color.g=0;
	color.b=0xf0;
	for (int i = 0;   i < RGB_6_13_mode/2/6;   i++)
	{
		be_null=m+i+RGB_6_13_mode*2/3;
		while (be_null>RGB_6_13_mode/2-1)
		{
			be_null = be_null-RGB_6_13_mode/2;
		}
		_this_rgb->rgb_buff[be_null+RGB_6_13_mode_h_addr]=color;
	}	
	for (int i = 0;   i < RGB_6_13_mode/2; ++  i)
	{
		_this_rgb->rgb_buff[RGB_6_13_mode/2+RGB_6_13_mode_h_addr+i]=_this_rgb->rgb_buff[RGB_6_13_mode_h_addr+i];
	}
}

void rgb__mode_13(){
	u8 energy;
	static u8 l,m=0;
	static u16 time_to_change;
	static bool n_status;
	static u8 add_cnt,pos,i,bSpeed;
	energy = audio_dac_energy_get()+abs(use_ctl.mic_energy);
	RGB_DEV_COLOUR color;

	m++;
	if (m>2)
	{
		m=0;
		l++;
		if (l>240)
		{
			l=0;
		}
	}
	color = get__color1(l,240);
	if(energy){
		n_status = 1;
		time_to_change = 0;
		set_color_0(RGB_6_13_mode_h_addr,RGB_6_13_mode_e_addr);
		music_dn_to_up_mode(RGB_6_13_mode_h_addr,RGB_6_13_mode_h_addr+RGB_6_13_mode/2/2-1,color);
		for (int i = 0;   i < RGB_6_13_mode/2/2; ++  i)
		{
			_this_rgb->rgb_buff[RGB_6_13_mode-1+RGB_6_13_mode_h_addr-i]=_this_rgb->rgb_buff[RGB_6_13_mode/2+RGB_6_13_mode_h_addr+i]=_this_rgb->rgb_buff[RGB_6_13_mode/2-1+RGB_6_13_mode_h_addr-i]=_this_rgb->rgb_buff[RGB_6_13_mode_h_addr+i];
		}
	}else{
		if(n_status)
		{
			if(time_to_change++<500)return;
			time_to_change = 0;
			n_status = 0;
		}
		set_color(RGB_6_13_mode_h_addr, RGB_6_13_mode_e_addr, color);

	}
}

void gpio_out_flash(u32 gpio)
{
	static u16 cu_status;
	static u8 cnt;
	
	u8 be_null;
	u8 c_be = get_energyStep(MAX_ENERGY);
	
	be_null=rgb_round_speed[c_be];
	if(++cnt<be_null)	return;
	cnt=0;
	
	gpio_set_direction(gpio, 0);
	gpio_set_pull_down(gpio, 0);
	if(cu_status++>299)cu_status=0;

	if(cu_status%75 < 10)
	{
		gpio_set_pull_up(gpio, 1);
		gpio_direction_output(gpio, 1);
	}
	else
	{
		gpio_set_pull_up(gpio, 0);
		gpio_direction_output(gpio, 0);
	}
}

#include "user_cfg.h"


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
#ifdef SUPPORT_KEY_RGB_ADKEY
	++kkkkkkkkkkkkkkkkk;
#endif
#ifdef SUPPORT_KEY_RGB_ADC
	++kkkkkkkkk2kkkkkkkk;
#endif
#ifdef SUPPORT_RGB_MODE_IO_FLASH
		gpio_out_flash(SUPPORT_RGB_MODE_IO_FLASH);
#endif
	static RGB_DEV_COLOUR color1x ;
	static u8 kkkkkkkkkkk=0;
	if (++kkkkkkkkkkk>6)
	{
		kkkkkkkkkkk=0;
		color1x = get2_gradual_change_color();
	}
	set_color(0,RGB_6_13_mode_h_addr-1,color1x);
	

#ifdef SUPPORT_init_RGB_MODE_15s_change
    switch ((light_mode_mark & 0x0f)){
#else
    switch (light_mode_mark){
#endif
    case RGB_OFF:
        user_rgb_clear_colour();
		user_rgb_send_cnt(1);
        break;
    case RGB_MODE_1:
		no_music_circle_and_music_jump();
		user_rgb_send_cnt(1);
        break;		
//	case RGB_MODE_2:
//		///gradual_mode();
//		rgb_marquee_mode(rgbycp_six_color,3,0,63);
//		user_rgb_send_cnt(1);
//	break;	
//
//	case RGB_MODE_3:
//		rgb_mult_color_circle_mode_1();
//		user_rgb_send_cnt(1);
//	break;	
//	case RGB_MODE_4:
//		rgb_mult_color_circle_mode_2();
//		user_rgb_send_cnt(1);
//	break;	
//	
//	case RGB_MODE_5:
//		///gradual_mode();
//		rgb_breath_mode();
//		user_rgb_send_cnt(1);
//	break;	

	case RGB_MODE_2:
		rgb__mode_6();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_3:
		rgb__mode_7();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_4:
		rgb__mode_8();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_5:
		rgb__mode_9();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_6:
		rgb__mode_10();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_7:
		rgb__mode_11();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_8:
		rgb__mode_12();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_9:
		rgb__mode_13();
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


#ifdef SUPPORT_LED_SIGLE
const u8 led_sigle_speed[] =
{30,10,9,8,7,6,5,4,4,3,2,1,};
void user_led_sigle(void) // 10ms
{
	static u8 cnt,cnt1;
	static u8 steptemp;

#ifdef SUPPORT_RGB_MODE_KEY
	if(light_mode_mark == RGB_OFF)	
	{
		cnt=0;
		LED_S_OFF();	
		return;
	}
#endif

	if((app_get_curr_task() == APP_FM_TASK)||(app_get_curr_task() == APP_IDLE_TASK))
	{
		cnt=0;
		LED_S_OFF();
		return;
	}
	
	steptemp=led_sigle_speed[get_energyStep(MAX_ENERGY)];

	if(++cnt1<steptemp)
		return;else	cnt1=0;



	if(cnt<30)
		cnt++;
	
	if(cnt==1)
	{
		if(light_mode_mark != RGB_OFF)	
			{
				 LED_S_ON();
			}
	}
	else  if(cnt==2)
	{
		LED_S_OFF();
	}
	else if(cnt==30)
		cnt=0;

}

#endif


