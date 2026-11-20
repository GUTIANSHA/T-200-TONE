#include "spi_rgb_config.h"

#define USER_RGB_NUMBER (12+60+34)

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

RGB_DEV_COLOUR get_gradual_change_color(){
	static u8 X_STATUS=0;
	static RGB_DEV_COLOUR DCOLOR={0xff,0,0};

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
	u8 bE=0;
	u16 energy_value=0;
	energy_value = audio_dac_energy_get();
	
	if(use_ctl.led_vol==0) 
		bE=0;
	else
	{
		bE=energy_value;
	}
	
                bE = bE/20;
	//printf("bE=======%d\n",bE);
#ifdef  RBG_ENERGY_LEVEL
     if((use_ctl.led_vol<=5)&&(use_ctl.led_vol!=0)){
             bE = bE *2;
	 }
	else if(use_ctl.led_vol<=10){
             bE = bE *1.8;
	} 
   else if(use_ctl.led_vol<=15){
             bE = bE *1.5;
	} 	
   else if(use_ctl.led_vol<=20){
             bE = bE *2.0;
	}
  else if(use_ctl.led_vol<=24){
             bE = bE *2.3;
	}  
    else if(use_ctl.led_vol<=28){
             bE = bE *2.3;
	}  
   else{
             bE = bE *3.0;
   }
  if(use_ctl.led_vol == 0)  bE = 0;
#if 1//def CHANGE_RGB_ENERGY_LEVEL_SET
          bE  = bE/2.2;
#endif
#endif
	if(bE>max_energy)bE=max_energy;	 
	if(	bE==0	&&	energy_value>10	)bE=1;	 
	return bE;
}

RGB_DEV_COLOUR get_gradual_change_color2(int intex,int N_Md)
{
	static RGB_DEV_COLOUR DCOLOR={0,0,0};
	int nn=N_Md%3;
	int nn2=N_Md/3;
	int vl=0;
	while (intex>=N_Md)
	{
		intex-=N_Md;
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
RGB_DEV_COLOUR get_gradual_change_color3(u8 intex,u8 N_Md)
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
		DCOLOR.r=0xff*intex/nn2;
		DCOLOR.b=0xff-0xff*intex/nn2;
	}
	else if (	intex<(nn2*2	+	(nn>0)	)	)
	{
		vl=0xff*(intex-(nn2))/(nn2	+	(nn>0)	);
		if (vl>0xff)
		{
			vl=0xff;
		}
		DCOLOR.g=vl;
		DCOLOR.r=0xff	-	vl;
		DCOLOR.b=0;
	}
	else
	{
		vl=0xff*( intex - ( nn2*2	+	(nn>0) ) )/( nn2	+	( nn>1) );
		if (vl>0xff)
		{
			vl=0xff;
		}
		DCOLOR.g = 0xff	-	vl;
		DCOLOR.r = 0;
		DCOLOR.b = vl;
	}
	return DCOLOR;
}
RGB_DEV_COLOUR *get_gradual_change_color4(int intex,int N_Md)
{
	static RGB_DEV_COLOUR DCOLOR={0,0,0};
	int nn=N_Md%3;
	int nn2=N_Md/3;
	int vl=0;
	while (intex>=N_Md)
	{
		intex-=N_Md;
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
	return &DCOLOR;
}

#include "user_cfg.h"

#define COM_RGB_start		12

void user_rgb_illumination(u8 _start,u8 _end,int intex,int N_Md)
{
	if (_start>_end)
	{
		_start ^= _end;
		_end ^= _start;
		_start ^= _end;
	}
	for (int i = 0;   i <= (_end - _start); ++  i)
	{
		user_rgb_colour_only_set(get_gradual_change_color4( intex , N_Md),_start + i);
		if (intex)
		{
			intex--;
		}
		else
		{
			intex = N_Md - 1;
		}
	}
}
void user_rgb_key_colour(u8 key_light_mode_mark,int intex,int N_Md)
{
	switch (key_light_mode_mark)
	{
		default:
		case RGB_MODE_2:
			set_color(0,	COM_RGB_start,get_gradual_change_color2(intex,N_Md));
			break;
		case RGB_MODE_1:
			user_rgb_illumination(0,	COM_RGB_start,intex,N_Md);
			break;


			break;
	}
}

void light_flash0(u16 *cnt)
{
	if((*cnt)++>1200)	*cnt=0;
	if((*cnt)%300 < 10)
	{
		JL_PORTC->DIE |= BIT(3);JL_PORTC->DIR &= ~BIT(3);
		JL_PORTC->OUT &= ~BIT(3);
		JL_PORTC->PD |= BIT(3);JL_PORTC->PU &= ~BIT(3);
	}
	else
	{
		JL_PORTC->PD &= ~BIT(3);JL_PORTC->PU &= ~BIT(3);
		JL_PORTC->DIE |= BIT(3);JL_PORTC->DIR |= BIT(3);
	}
}

RGB_DEV_COLOUR colorm1={0,0xff,0};
RGB_DEV_COLOUR colorm2={0xff,0,0};
RGB_DEV_COLOUR colorm3={0,0,0xff};
RGB_DEV_COLOUR color2={0xff,0xff,0xff};
RGB_DEV_COLOUR color3={0,0,0};
RGB_DEV_COLOUR color4={0x1f,0x1f,0x1f};
RGB_DEV_COLOUR color5={0x5f,0x5f,0x5f};
static u8 flash0cnt=0;

void COM_RGB_MODE_1(void)
{
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);
	if(time1++<(	(8*8)/((c_be+1)*4) +1	)) 
	{
		return;
	}
	time1=0;
	light_flash0(&flash0cnt);

	static int intex;
	intex++;
	if ( intex >= (12*30*17) )
	{
		intex = 0;
	}
	user_rgb_key_colour(light_mode_mark,intex,12);
	user_rgb_illumination(COM_RGB_start,	COM_RGB_start+60-1,intex,60);
	user_rgb_illumination(COM_RGB_start+60,	COM_RGB_start+60+34-1,intex,34);
}

void COM_RGB_MODE_2(void)
{
	u8 c_be;
	static u8 time1;
	static u8 time2;
	static u8 time3;
	c_be = get_energyStep(16);
	if(time1++<(	((8*8)/((c_be+1)*4) +1) * 6	)) 
	{
		return;
	}
	time1=0;

	light_flash0(&flash0cnt);
	static int intex;
	intex++;
	if ( intex >= (12*30*17) )
	{
		intex = 0;
	}
	user_rgb_key_colour(light_mode_mark,intex,60);
	set_color(COM_RGB_start,USER_RGB_NUMBER-1,color3);
	switch (time2%4)
	{
		default:
			if (time2>10)
			{
				switch (time3++)
				{
					default:
						time2=0;
						time3=0;
						set_color(COM_RGB_start+15+7,COM_RGB_start+30+15+7,color2);
						break;
					case 0:
						set_color(COM_RGB_start+69,COM_RGB_start+85,color2);
						break;
					case 1:
						set_color(COM_RGB_start+60+18,USER_RGB_NUMBER-1,color5);
						break;
					case 2:
						set_color(COM_RGB_start+60+26,COM_RGB_start+60+33,color2);
						set_color(COM_RGB_start+60,COM_RGB_start+60+8,color2);
						break;
					case 3:
						set_color(COM_RGB_start+69,COM_RGB_start+85,color5);
//						set_color(COM_RGB_start+60,COM_RGB_start+60+17,color5);
						break;
					
					case 7:
						set_color(COM_RGB_start+60+9,COM_RGB_start+60+25,color5);
						break;
					case 6:
						set_color(COM_RGB_start+60+18,USER_RGB_NUMBER-1,color2);
						break;
					case 5:
						set_color(COM_RGB_start+60+26,COM_RGB_start+60+33,color5);
						set_color(COM_RGB_start+60,COM_RGB_start+60+8,color5);
						break;
					case 4:
						set_color(COM_RGB_start+60,COM_RGB_start+60+17,color2);
						break;
					
					case 8:
						set_color(COM_RGB_start+30+15+7,COM_RGB_start+60-1,color2);
						set_color(COM_RGB_start,COM_RGB_start+15+7,color2);
						break;
				}
				break;
			}
			time2++;

		case 0:
			set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,color4);
			time2++;
			break;
		case 1:
			set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,color2);
			time2++;
			break;
		case 2:
			set_color(COM_RGB_start,COM_RGB_start+60-1,color2);
			time2++;
			break;
	}
	
}

void COM_RGB_MODE_3(void)
{
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);
	if(time1++<(	(4*8)/((c_be+1)*4) +1	)) 
	{
		return;
	}
	time1=0;
	light_flash0(&flash0cnt);

	static int intex;
	static int intex2;
	static int intex3;
	static int intex4;
	
	intex++;
	if ( intex >= (12*30*17) )
	{
		intex = 0;
	}
	
	user_rgb_key_colour(light_mode_mark,intex,60);
	
	intex2++;
	if (intex2>=30)
	{
		intex2=0;
	}
	
	set_color(COM_RGB_start,COM_RGB_start+60-1,color3);
	for (int i = 0;   i < 10; ++  i)
	{
		user_rgb_colour_only_set(get_gradual_change_color4( intex , 240),COM_RGB_start + 7 + (i+intex2)%30 );
		if ( (i+intex2)%30 > 6 )
		{
			user_rgb_colour_only_set(get_gradual_change_color4( intex , 240),COM_RGB_start + 60 + 6 - (i+intex2)%30 );
		}
		else
		{
			user_rgb_colour_only_set(get_gradual_change_color4( intex , 240),COM_RGB_start + 6 - (i+intex2)%30 );
		}
	}
	
	if ( (intex2%2) == 0 )
	{
		set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,color3);

		
		for (int i = 0;   i < 3; ++  i)
		{
			user_rgb_colour_only_set(get_gradual_change_color4( intex , 240),COM_RGB_start + 70 + (i+intex2/2)%15 );
			if ( (i+intex2/2)%15 > 7 )
			{
				user_rgb_colour_only_set(get_gradual_change_color4( intex , 240),COM_RGB_start + 93 + 8 - (i+intex2/2)%15 );
			}
			else
			{
				user_rgb_colour_only_set(get_gradual_change_color4( intex , 240),COM_RGB_start + 67 - (i+intex2/2)%15 );
			}
		}
		
		for (int i = 0;   i < 3; ++  i)
		{
			user_rgb_colour_only_set(get_gradual_change_color4( intex + 120 , 240),COM_RGB_start + 84 - (i+intex2/2)%15 );
			if ( (i+intex2/2)%15 > 6 )
			{
				user_rgb_colour_only_set(get_gradual_change_color4( intex + 120 , 240),COM_RGB_start + 60 - 7 + (i+intex2/2)%15 );
			}
			else
			{
				user_rgb_colour_only_set(get_gradual_change_color4( intex + 120 , 240),COM_RGB_start + 87 + (i+intex2/2)%15 );
			}
		}
		_this_rgb->rgb_buff[69]=_this_rgb->rgb_buff[68]=_this_rgb->rgb_buff[70];
		_this_rgb->rgb_buff[85]=_this_rgb->rgb_buff[86]=_this_rgb->rgb_buff[87];
				
	}


	
}



typedef struct {
    u8 old_be;
    u8 pos;
    u8 count_to_add;
}rgb_mode_music_dat;
rgb_mode_music_dat loop_1={.old_be=0,.pos=0,.count_to_add=0};
rgb_mode_music_dat loop_2={.old_be=0,.pos=0,.count_to_add=0};


void music_dn_to_up_mode(u8 _start,u8 _end,RGB_DEV_COLOUR color,rgb_mode_music_dat *x)
{
	u8 c_be = get_energyStep((abs(_end - _start) + 1));
		
	if((x->old_be)<=c_be){
		(x->pos)=c_be;
		if((x->pos)<1)(x->pos)=1;
		if(c_be<1)c_be=1;
		(x->old_be)=c_be;
	}else{
		if((x->count_to_add)++>2){
			(x->count_to_add)=0;
			if((x->pos)>2)(x->pos)--;
			else (x->pos)=1;
			(x->old_be)=(x->pos);
			if((x->old_be)<1)(x->old_be)=1;
		}
	}

	clear_color(_start,_end);

	if((_end-_start)>0){
		for(s8 i=_start;i<(x->pos)+_start;i++){
			user_rgb_colour_only_set(&color,i);
		}
	}else{
		for(s8 i=_start;i>_start-(x->pos);i--){
			user_rgb_colour_only_set(&color,i);
		}
	}
	
}

void COM_RGB_MODE_4(void)
{
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);
	if(time1++<(	(4*4)/((c_be+1)*4) +1	)) 
	{
		return;
	}
	time1=0;
	light_flash0(&flash0cnt);

	static int intex;
	static int intex2;
	static int intex3;
	int intex4;
	

	
	user_rgb_key_colour(light_mode_mark,intex,60);
	
	intex2++;
	if (intex2>59)
	{
		intex2=0;
		if (intex3)
		{
			intex3=0;
		}
		else
		{
			intex3=1;
		}
	}
	intex++;
	if ( intex >= (29*16*3) )
	{
		intex = 0;
	}

	intex4 = ( intex2 + 1 ) * 34 / 60 - 1 ;
	if (intex3)
	{
		for (int i = 0;i < 30 ; ++	i)
		{
			if (i < ((intex2>30)?(29+30-intex2):intex2))
			{
				user_rgb_colour_only_set(get_gradual_change_color4( ( intex/16 + 29 * 3 - i ) , 29 * 3 ),COM_RGB_start + 36 - i );
				if ( i > 22 )
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/16 + 29 * 3 - i ), 29 * 3 ),COM_RGB_start - 23 + i );
				}
				else
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/16 + 29 * 3 - i ), 29 * 3 ),COM_RGB_start + 37 + i );
				}
			}
			else
			{
				user_rgb_colour_only_set( &color3 , COM_RGB_start + 36 - i );
				if ( i > 22 )
				{
					user_rgb_colour_only_set(&color3, COM_RGB_start - 23 + i );
				}
				else
				{
					user_rgb_colour_only_set(&color3, COM_RGB_start + 37 + i );
				}
			}
		}

		//68 69 85 86
		for (int i = 0;i < 17 ; ++	i)
		{
			if (i < ((intex4>17)?(17+16-intex4):intex4))
			{
				user_rgb_colour_only_set(get_gradual_change_color4( ( intex/29 + 16 * 3 - i ) , 16 * 3 ),COM_RGB_start + 69 + i );
				if ( i > 8 )
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/29 + 16 * 3 - i ), 16 * 3 ),COM_RGB_start + 93 + 9 - i );
				}
				else
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/29 + 16 * 3 - i ), 16 * 3 ),COM_RGB_start + 68 - i );
				}
			}
			else
			{
				user_rgb_colour_only_set( &color3 , COM_RGB_start + 69 + i );
				if ( i > 8 )
				{
					user_rgb_colour_only_set(&color3, COM_RGB_start + 93 + 9 - i );
				}
				else
				{
					user_rgb_colour_only_set(&color3, COM_RGB_start + 68 - i );
				}
			}
		}
	}
	else
	{
		for (int i = 0;i < 30 ; ++	i)
		{
			if (i < ((intex2>30)?(29+30-intex2):intex2))
			{
				user_rgb_colour_only_set(get_gradual_change_color4( ( intex/16 + 29 * 3 - i ) , 29 * 3 ),COM_RGB_start + 7 + i );
				if ( i > 6 )
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/16 + 29 * 3 - i ), 29 * 3 ),COM_RGB_start + 60 + 6 - i );
				}
				else
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/16 + 29 * 3 - i ), 29 * 3 ),COM_RGB_start + 6 - i );
				}
			}
			else
			{
				user_rgb_colour_only_set( &color3 , COM_RGB_start + 7 + i );
				if ( i > 6 )
				{
					user_rgb_colour_only_set(&color3,COM_RGB_start + 60 + 6 - i );
				}
				else
				{
					user_rgb_colour_only_set(&color3,COM_RGB_start + 6 - i );
				}
			}
		}

		
		//68 69 85 86
		for (int i = 0;i < 17 ; ++	i)
		{
			if (i < ((intex4>17)?(17+16-intex4):intex4))
			{
				user_rgb_colour_only_set(get_gradual_change_color4( ( intex/29 + 16 * 3 - i ) , 16 * 3 ),COM_RGB_start + 85 - i );
				if ( i > 7 )
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/29 + 16 * 3 - i ), 16 * 3 ),COM_RGB_start + 60 - 8 + i );
				}
				else
				{
					user_rgb_colour_only_set(get_gradual_change_color4( ( intex/29 + 16 * 3 - i ), 16 * 3 ),COM_RGB_start + 86 + i );
				}
			}
			else
			{
				user_rgb_colour_only_set( &color3 , COM_RGB_start + 85 - i );
				if ( i > 7 )
				{
					user_rgb_colour_only_set(&color3, COM_RGB_start + 60 - 8 + i );
				}
				else
				{
					user_rgb_colour_only_set(&color3, COM_RGB_start + 86 + i );
				}
			}
		}
	}
}

void COM_RGB_MODE_5(void)
{
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);

	static int intex;
	static int intex2;
	static int intex3;
	int intex4;
	static u8 static_mode_cnt=0;
	

	light_flash0(&flash0cnt);
	
	user_rgb_key_colour(light_mode_mark,intex,60);

	if (audio_dac_energy_get()<5	&&	static_mode_cnt<100)
	{
		static_mode_cnt++;
	}
	else if (audio_dac_energy_get()>5)
	{
		static_mode_cnt=0;
	}
	
	if (static_mode_cnt>=100)
	{
		
		if(time1++<(	(4*4)/((c_be+1)*4) +1	)) 
		{
			return;
		}
		time1=0;
		
		intex2++;
		if (intex2>59)
		{
			intex2=0;
			if (intex3)
			{
				intex3=0;
			}
			else
			{
				intex3=1;
			}
		}
		intex++;
		if ( intex >= (60*3) )
		{
			intex = 0;
		}
		
		intex4 = ( intex2 + 1 ) * 34 / 60 - 1 ;
		if (intex3)
		{
			for (int i = 0;i < 30 ; ++	i)
			{
				if (i < ((intex2>30)?(29+30-intex2):intex2))
				{
					user_rgb_colour_only_set(get_gradual_change_color4( intex , 60*3 ),COM_RGB_start + 36 - i );
					if ( i > 22 )
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start - 23 + i );
					}
					else
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 37 + i );
					}
				}
				else
				{
					user_rgb_colour_only_set( &color3 , COM_RGB_start + 36 - i );
					if ( i > 22 )
					{
						user_rgb_colour_only_set(&color3, COM_RGB_start - 23 + i );
					}
					else
					{
						user_rgb_colour_only_set(&color3, COM_RGB_start + 37 + i );
					}
				}
			}
		
			//68 69 85 86
			for (int i = 0;i < 17 ; ++	i)
			{
				if (i < ((intex4>17)?(17+16-intex4):intex4))
				{
					user_rgb_colour_only_set(get_gradual_change_color4( intex , 60*3 ),COM_RGB_start + 69 + i );
					if ( i > 8 )
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 93 + 9 - i );
					}
					else
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 68 - i );
					}
				}
				else
				{
					user_rgb_colour_only_set( &color3 , COM_RGB_start + 69 + i );
					if ( i > 8 )
					{
						user_rgb_colour_only_set(&color3, COM_RGB_start + 93 + 9 - i );
					}
					else
					{
						user_rgb_colour_only_set(&color3, COM_RGB_start + 68 - i );
					}
				}
			}
		}
		else
		{
			for (int i = 0;i < 30 ; ++	i)
			{
				if (i < ((intex2>30)?(29+30-intex2):intex2))
				{
					user_rgb_colour_only_set(get_gradual_change_color4( intex , 60*3 ),COM_RGB_start + 7 + i );
					if ( i > 6 )
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 60 + 6 - i );
					}
					else
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 6 - i );
					}
				}
				else
				{
					user_rgb_colour_only_set( &color3 , COM_RGB_start + 7 + i );
					if ( i > 6 )
					{
						user_rgb_colour_only_set(&color3,COM_RGB_start + 60 + 6 - i );
					}
					else
					{
						user_rgb_colour_only_set(&color3,COM_RGB_start + 6 - i );
					}
				}
			}
		
			
			//68 69 85 86
			for (int i = 0;i < 17 ; ++	i)
			{
				if (i < ((intex4>17)?(17+16-intex4):intex4))
				{
					user_rgb_colour_only_set(get_gradual_change_color4( intex , 60*3 ),COM_RGB_start + 85 - i );
					if ( i > 7 )
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 60 - 8 + i );
					}
					else
					{
						user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 86 + i );
					}
				}
				else
				{
					user_rgb_colour_only_set( &color3 , COM_RGB_start + 85 - i );
					if ( i > 7 )
					{
						user_rgb_colour_only_set(&color3, COM_RGB_start + 60 - 8 + i );
					}
					else
					{
						user_rgb_colour_only_set(&color3, COM_RGB_start + 86 + i );
					}
				}
			}			
		}

		return;
	}
	
	if(time1++<(	(4*4)/((c_be+1)*4) +1	)) ;
	else
	{
		time1=0;
		intex++;
	}
	if ( intex >= (60*3) )
	{
		intex = 0;
	}
//	void music_dn_to_up_mode(u8 _start,u8 _end,RGB_DEV_COLOUR color,rgb_mode_music_dat *x)
	music_dn_to_up_mode(COM_RGB_start + 69,COM_RGB_start + 85,get_gradual_change_color2( intex , 60*3 ),&loop_1);
	for (int i = 0;i < 17 ; ++	i)
	{
		if ( i > 7 )
		{
			_this_rgb->rgb_buff[COM_RGB_start + 60 - 8 + i] = _this_rgb->rgb_buff[COM_RGB_start + 85 - i] ;
		}
		else
		{
			_this_rgb->rgb_buff[COM_RGB_start + 86 + i] = _this_rgb->rgb_buff[COM_RGB_start + 85 - i] ;
		}
	}			
	
	music_dn_to_up_mode(COM_RGB_start + 36,COM_RGB_start + 7,get_gradual_change_color2( intex , 60*3 ),&loop_2);
	for (int i = 0;i < 30 ; ++	i)
	{
		if ( i > 6 )
		{
			_this_rgb->rgb_buff[COM_RGB_start + 60 + 6 - i] = _this_rgb->rgb_buff[COM_RGB_start + 7 + i] ;
		}
		else
		{
			_this_rgb->rgb_buff[COM_RGB_start + 6 - i] = _this_rgb->rgb_buff[COM_RGB_start + 7 + i] ;
		}
	}
	return;

	
}

void COM_RGB_MODE_6(void)
{
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);
	if(time1++<(	(4*6)/((c_be+1)*4) +1	)) 
	{
		return;
	}
	time1=0;

	static int intex;
	static int intex2;
	static int intex3;
	int intex4;
	
	user_rgb_key_colour(light_mode_mark,intex,60);
	
	light_flash0(&flash0cnt);
	intex2++;
	if (intex2>59)
	{
		intex2=0;
	}
	intex++;
	if ( intex >= (60*3) )
	{
		intex = 0;
	}

	intex4 = ( intex2 + 1 ) * 34 / 60 - 1 ;

	set_color(COM_RGB_start,COM_RGB_start+60-1,color3);
	for (int i = 0;i < 22 ; ++	i)
	{
		user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + (i+intex2)%60 );
	}

	
	set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,color3);
	for (int i = 0;i < 12 ; ++	i)
	{
		user_rgb_colour_only_set(get_gradual_change_color4( intex + 30*3, 60*3 ),COM_RGB_start + 60 + ( 33 + 34 - i -intex4)%34 );
	}
	for (int i = 0;i < 12 ; ++	i)
	{
		user_rgb_colour_only_set(get_gradual_change_color4( intex, 60*3 ),COM_RGB_start + 60 + (i+intex4)%34 );
	}

}

void COM_RGB_MODE_7(void)
{
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);
	if(time1++<(	(4*6)/((c_be+1)*4) +1	)) 
	{
		return;
	}
	time1=0;

	static int intex;
	static int intex2;
	static int intex3;
	int intex4;
	
	user_rgb_key_colour(light_mode_mark,intex,60);

	light_flash0(&flash0cnt);
	if (intex>150*3)
	{
		intex2++;
	}
	else
	{
		intex2--;
	}

	if (intex2>59)
	{
		intex2=0;
	}
	if (intex2<0)
	{
		intex2=59;
	}
	
	intex++;
	if ( intex >= (300*3) )
	{
		intex = 0;
	}

	intex4 = ( intex2 + 1 ) * 34 / 60 - 1 ;

	for (int i = 0;i < 20 ; ++	i)
	{
		user_rgb_colour_only_set(&colorm1,COM_RGB_start+(59-intex2+i)%60);
		user_rgb_colour_only_set(&colorm2,COM_RGB_start+(59-intex2+20+i)%60);
		user_rgb_colour_only_set(&colorm3,COM_RGB_start+(59-intex2+40+i)%60);
	}
	for (int i = 0;i < 11 ; ++	i)
	{
		user_rgb_colour_only_set(&colorm1,COM_RGB_start+60+(intex4+i)%34);
		user_rgb_colour_only_set(&colorm2,COM_RGB_start+60+(intex4+11+i)%34);
		user_rgb_colour_only_set(&colorm3,COM_RGB_start+60+(intex4+22+i)%34);
	}
	user_rgb_colour_only_set(&colorm3,COM_RGB_start+60+(intex4+33)%34);

}

void COM_RGB_MODE_8(void)
{
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);
	if(time1++<(	(4*6)/((c_be+1)*4) +1	)) 
	{
		return;
	}
	time1=0;

	static int intex;
	static int intex2;
	static int intex3;
	int intex4;
	
	user_rgb_key_colour(light_mode_mark,intex,60);

	light_flash0(&flash0cnt);
	if (intex>150*3)
	{
		intex2++;
	}
	else
	{
		intex2--;
	}

	if (intex2>59)
	{
		intex2=0;
	}
	if (intex2<0)
	{
		intex2=59;
	}
	
	intex++;
	if ( intex >= (300*3) )
	{
		intex = 0;
	}
	
	intex3++;
	if ( intex3 >= (60*3) )
	{
		intex3 = 0;
	}

	intex4 = ( intex2 + 1 ) * 34 / 60 - 1 ;

	for (int i = 0;i < 15 ; ++	i)
	{
		user_rgb_colour_only_set(get_gradual_change_color4( intex3, 60*3 ),COM_RGB_start+(intex2+i)%60);
		user_rgb_colour_only_set(&color3,COM_RGB_start+(intex2+15+i)%60);
		user_rgb_colour_only_set(get_gradual_change_color4( intex3, 60*3 ),COM_RGB_start+(intex2+30+i)%60);
		user_rgb_colour_only_set(&color3,COM_RGB_start+(intex2+45+i)%60);
	}
	for (int i = 0;i < 8 ; ++	i)
	{
		user_rgb_colour_only_set(get_gradual_change_color4( intex3, 60*3 ),COM_RGB_start+60+(intex4+i)%34);
		user_rgb_colour_only_set(&color3,COM_RGB_start+60+(intex4+9+i)%34);
		user_rgb_colour_only_set(get_gradual_change_color4( intex3, 60*3 ),COM_RGB_start+60+(intex4+17+i)%34);
		user_rgb_colour_only_set(&color3,COM_RGB_start+60+(intex4+26+i)%34);
	}
	user_rgb_colour_only_set(get_gradual_change_color4( intex3, 60*3 ),COM_RGB_start+60+(intex4+8)%34);
	user_rgb_colour_only_set(get_gradual_change_color4( intex3, 60*3 ),COM_RGB_start+60+(intex4+25)%34);

}

void COM_RGB_MODE_9(void)
{
	const RGB_DEV_COLOUR color_[]={{0,0xff,0},{0xff,0,0},{0,0,0xff},{0,0xff,0xff},{0xff,0,0xff},{0xff,0xff,0}};
	u8 c_be;
	static u8 time1;
	c_be = get_energyStep(16);
	if(time1++<(	(4*6)/((c_be+1)*4) +1	)) 
	{
		return;
	}
	time1=0;

	static int intex = 0 ;
	static int intex2 = 0 ;
	static int intex3 = 0 ;
	static int intex4=0;
	static int intex5 = 0 ;
	
light_flash0(&flash0cnt);
RGB_DEV_COLOUR colou__;
	user_rgb_key_colour(light_mode_mark,intex,60);

	if (intex4<10)
	{
		if (intex>=ARRAY_SIZE(color_))
		{
			intex=0;
		}

		intex5=  ((intex3>240)?(480-intex3):intex3);
		if (intex2==2)
		{
			colou__.r = color_[intex].r *intex5/255/4;
			colou__.g = color_[intex].g *intex5/255/4;
			colou__.b = color_[intex].b *intex5/255/4;
		}
		else
		{
			colou__.r = color_[intex].r *intex5/255;
			colou__.g = color_[intex].g *intex5/255;
			colou__.b = color_[intex].b *intex5/255;
		}
		switch (intex2)
		{
			default:
				intex2 = 0;
				intex3=0;
				intex4=0;
			case 0:
				set_color(COM_RGB_start,USER_RGB_NUMBER-1,colou__);
				break;
			case 1:
				set_color(COM_RGB_start,COM_RGB_start+60-1,colou__);
				set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,color3);

				break;
			case 2:
				set_color(COM_RGB_start,COM_RGB_start+60-1,color3);
				set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,colou__);

				break;
			case 3:
				set_color(COM_RGB_start,COM_RGB_start+60-1,color3);
				set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,colou__);
				break;
		}
		intex3+=10;
		if (intex3>=480)
		{
			intex4++;
			intex2 = rand() % 4;
			intex3=0;
			intex++;
		}
	}
	else
	{
		intex5++;
		if (intex5>=60*5)
		{
			intex5=0;
		}
		switch (intex4%10%3)
		{
			default:
				intex4=10;
			case 0:
				colou__ = get_gradual_change_color2(intex5/5,60);
				colou__.r = colou__.r /20;
				colou__.g = colou__.g /20;
				colou__.b = colou__.b /20;
				set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,colou__);
				set_color(COM_RGB_start,COM_RGB_start+60-1,color3);
				intex3+=10;
				if (intex3>=120)
				{
					intex4++;
					intex3=0;
				}
				break;	
			case 1:
				set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,get_gradual_change_color2(intex5/5,60));
				set_color(COM_RGB_start,COM_RGB_start+60-1,color3);
				intex3+=10;
				if (intex3>=120)
				{
					intex4++;
					intex3=0;
				}
				break;
			case 2:
				set_color(COM_RGB_start+60,USER_RGB_NUMBER-1,color3);
				set_color(COM_RGB_start,COM_RGB_start+60-1,get_gradual_change_color2(intex5/5,60));
				intex3+=10;
				if (intex3>=120)
				{
					intex4++;
					intex3=0;
				}
				break;
		}
		if (intex4>15)
		{
			intex4=0;
			intex3=0;
		}
	}

}

void user_rgb_scan_mode(void){
    if(!_this_->info->init_flag){
        g_printf("*");
        return;
    }

    wdt_clear();
	
    static u32 time_ = 0;
    if(!_this_->mode_scan_ms || (timer_get_ms()-time_<_this_->mode_scan_ms) || _this_->info->updata_flag || _this_->info->rend_flag){
        return;
    }else{
        time_ = timer_get_ms();     
    }

	switch (light_mode_mark)
	{
		case RGB_OFF:
			user_rgb_clear_colour();
			user_rgb_send_cnt(1);
			break;
		case RGB_MODE_1:
//			user_rgb_clear_colour();
//			set_color(0,use_ctl.led_vol,color4);
//			user_rgb_send_cnt(1);
//			break;			
			COM_RGB_MODE_1();
			user_rgb_send_cnt(1);
			break;
		case RGB_MODE_2:
			COM_RGB_MODE_2();
			user_rgb_send_cnt(1);
			break;
		case RGB_MODE_3:
			COM_RGB_MODE_3();
			user_rgb_send_cnt(1);
			break;		
		case RGB_MODE_4:
			COM_RGB_MODE_4();
			user_rgb_send_cnt(1);
			break;		
		case RGB_MODE_5:
			COM_RGB_MODE_5();
			user_rgb_send_cnt(1);
			break;
		case RGB_MODE_6:
			COM_RGB_MODE_6();
			user_rgb_send_cnt(1);
			break;		
		case RGB_MODE_7:
			COM_RGB_MODE_7();
			user_rgb_send_cnt(1);
			break;		
		case RGB_MODE_8:
			COM_RGB_MODE_8();
			user_rgb_send_cnt(1);
			break;
		case RGB_MODE_9:
			COM_RGB_MODE_9();
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


