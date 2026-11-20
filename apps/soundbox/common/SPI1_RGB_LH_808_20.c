#include "spi_rgb_config.h"

#define USER_RGB_NUMBER (20)

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
    for(s8 i=MIN(_start,_end);i<MAX(_start, _end)+1;i++){
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


void music_dn_to_up_mode(u8 _start,u8 _end,RGB_DEV_COLOUR color,u8 c_be){
	static u8 pos,cur_be,old_be,count_to_add,light_number;
	//RGB_DEV_COLOUR color;
	//color = get_gradual_change_color();

	cur_be=c_be;

	light_number = abs(_end - _start) + 1;
	
	if(old_be<=cur_be){
		pos=(cur_be*light_number)/13;
		if(pos<1)pos=1;
		if(cur_be<1)cur_be=1;
		old_be=cur_be;
	}else{
		if(count_to_add++>2){
			count_to_add=0;
			if(pos>2)pos--;
			else pos=1;
			old_be=pos*13/light_number;
			if(old_be<1)old_be=1;
		}
	}

	clear_color(_start,_end);

	if((_end-_start)>0){
		for(s8 i=_start;i<pos+_start;i++){
			user_rgb_colour_only_set(&color,i);
		}
	}else{
		for(s8 i=_start;i>_start-pos;i--){
			user_rgb_colour_only_set(&color,i);
		}
	}
	
}


void music2_dn_to_up_mode(u8 _start,u8 _end,RGB_DEV_COLOUR color,u8 c_be){
	static u8 pos,cur_be,old_be,count_to_add,light_number;
	//RGB_DEV_COLOUR color;
	//color = get_gradual_change_color();

	cur_be=c_be;

	light_number = abs(_end - _start) + 1;
	
	if(old_be<=cur_be){
		pos=(cur_be*light_number)/12;
		if(pos<1)pos=1;
		if(cur_be<1)cur_be=1;
		old_be=cur_be;
	}else{
		if(count_to_add++>2){
			count_to_add=0;
			if(pos>2)pos--;
			else pos=1;
			old_be=pos*12/light_number;
			if(old_be<1)old_be=1;
		}
	}

	clear_color(_start,_end);

	if((_end-_start)>0){
		for(s8 i=_start;i<pos+_start;i++){
			user_rgb_colour_only_set(&color,i);
		}
	}else{
		for(s8 i=_start;i>_start-pos;i--){
			user_rgb_colour_only_set(&color,i);
		}
	}
	
}


void light_flash(u8 _start,u8 _end,RGB_DEV_COLOUR color){
	static u16 cu_status;
	RGB_DEV_COLOUR clean_color={0,0,0};

	clear_color(_start,_end);
	
	if(cu_status++>300)cu_status=0;

	if(cu_status%75 < 10){
		set_color(_start,_end, color);
	}
}
void light_flash2(u8 _start,u8 _end,RGB_DEV_COLOUR color,u16 *cnt){
	RGB_DEV_COLOUR clean_color={0,0,0};

	clear_color(_start,_end);
	
	if((*cnt)++>60)	*cnt=0;

	if((*cnt)%15 < 2){
		set_color(_start,_end, color);
	}
}
void light_flash0(u16 *cnt)
{
	if((*cnt)++>300)	*cnt=0;
	if((*cnt)%75 < 10)
	{
		JL_PORTB->DIE |= BIT(2);JL_PORTB->DIR &= ~BIT(2);JL_PORTB->OUT |= BIT(2);JL_PORTB->PD &= ~BIT(2);JL_PORTB->PU |= BIT(2);
	}
	else
	{
		JL_PORTB->DIE |= BIT(2);JL_PORTB->DIR &= ~BIT(2);JL_PORTB->OUT &= ~BIT(2);JL_PORTB->PD &= ~BIT(2);JL_PORTB->PU &= ~BIT(2);
	}
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

RGB_DEV_COLOUR get_gradual_change_color2(u8 intex,u8 N_Md)
{
	RGB_DEV_COLOUR DCOLOR={0,0,0};
	u8 nn=N_Md%3;
	u8 nn2=N_Md/3;
	u16 vl=0;
//	if (intex>N_Md)
	while (intex>=N_Md)
	{
//		return DCOLOR;
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

u16 flash1cnt=0;


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
	
//	light_mode_mark = RGB_MODE_3;
		static u16 time1,time2;
		static u16 cnt_1,cnt_2;
		RGB_DEV_COLOUR colorm1={0,0xff,0};
		RGB_DEV_COLOUR colorm2={0xff,0,0};
		RGB_DEV_COLOUR colorm3={0,0,0xff};
		RGB_DEV_COLOUR color2={0xff,0xff,0xff};
		RGB_DEV_COLOUR color3={0,0,0};
		RGB_DEV_COLOUR color4;
		RGB_DEV_COLOUR *colour[]={&colorm1,&colorm2,&colorm3,&color2};
		u8 c_be=0,d_x;
		
		static u8 kkkkkkkkkkkk=0;
		if (light_mode_mark!=RGB_MODE_6)
		{
			kkkkkkkkkkkk=0;
		}
		if (light_mode_mark%2)
		{
			time1=200;
		}
		else
		{
			time2=200;
		}
		

	
	
		switch (light_mode_mark){
		case RGB_OFF:
			user_rgb_clear_colour();
			user_rgb_send_cnt(1);
			break;
		case RGB_MODE_1:
			c_be = get_energyStep(16);
			
			if (light_mode_mark%2)
			{
				if(time2++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time2=0;
			}
			else
			{
				if(time1++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time1=0;
			}
		
		
			cnt_1++;
			if (cnt_1>=USER_RGB_NUMBER)
			{
				cnt_1=0;
			}
			for (int i = 0;   i < USER_RGB_NUMBER; ++  i)
			{
				_this_rgb->rgb_buff[i]=get_gradual_change_color2(cnt_1+i,USER_RGB_NUMBER-1);
			}
		
		
			user_rgb_send_cnt(1);
			break;		
		case RGB_MODE_2:
			c_be = get_energyStep(16);
			if (light_mode_mark%2)
			{
				if(time2++<(	(60*10)/((c_be+1)*4) +1 )) 
				{
					return;
				}
				time2=0;
			}
			else
			{
				if(time1++<(	(60*10)/((c_be+1)*4) +1 )) 
				{
					return;
				}
				time1=0;
			}
			user_rgb_clear_colour();
			
			cnt_1++;
			if (cnt_1>=4)
			{
				cnt_1=0;
			}
			user_rgb_same_colour( colour[cnt_1] );
		
			user_rgb_send_cnt(1);
			break;	
		case RGB_MODE_3: 
			c_be = get_energyStep(16);
			if (light_mode_mark%2)
			{
				if(time2++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time2=0;
			}
			else
			{
				if(time1++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time1=0;
			}
		
			user_rgb_clear_colour();
			
			if (0)
			{
				cnt_1++;
		
			}
			else
			{
				if (cnt_1)
				{
					cnt_1--;
				}
				else
				{
					cnt_1=USER_RGB_NUMBER-1;
				}
			}
				if (cnt_1>=USER_RGB_NUMBER)
				{
					cnt_1=0;
				}
			cnt_2++;
			if (cnt_2>=60)
			{
				cnt_2=0;
			}
			color4=get_gradual_change_color2(cnt_2,60);
			
			for (int i = 0;   i < 8; ++  i)
			{
				if (cnt_1>=i)
				{
					d_x=cnt_1-i;
				}
				else
				{
					d_x=cnt_1+USER_RGB_NUMBER-i;
				}
				_this_rgb->rgb_buff[d_x]=color4;
			}
			
			user_rgb_send_cnt(1);
			break;	
		case RGB_MODE_4:
			c_be = get_energyStep(12);
			if (light_mode_mark%2)
			{
				if(time2++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time2=0;
			}
			else
			{
				if(time1++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time1=0;
			}
		
			user_rgb_clear_colour();
			if (0)
			{
				cnt_1++;
			}
			else
			{
				if (cnt_1)
				{
					cnt_1--;
				}
				else
				{
					cnt_1=USER_RGB_NUMBER-1;
				}
			}
			if (cnt_1>=USER_RGB_NUMBER)
			{
				cnt_1=0;
			}
			cnt_2++;
			if (cnt_2>=60)
			{
				cnt_2=0;
			}
			color4=get_gradual_change_color2(cnt_2,60);
			
			for (int i = 0;   i < USER_RGB_NUMBER/3; ++  i)
			{
				d_x=cnt_1+i;
				while (d_x>=USER_RGB_NUMBER)
				{
					d_x=d_x-USER_RGB_NUMBER;
				}
				_this_rgb->rgb_buff[d_x]=color4;
			}
			
			color4=get_gradual_change_color2(cnt_2+20,60);
			
			for (int i = 0;   i < USER_RGB_NUMBER/3; ++  i)
			{
				d_x=cnt_1+i+USER_RGB_NUMBER/3;
				while (d_x>=USER_RGB_NUMBER)
				{
					d_x=d_x-USER_RGB_NUMBER;
				}
				_this_rgb->rgb_buff[d_x]=color4;
			}		
		
			color4=get_gradual_change_color2(cnt_2+40,60);
			
			for (int i = 0;   i < (USER_RGB_NUMBER-USER_RGB_NUMBER/3*2); ++  i)
			{
				d_x=cnt_1+i+USER_RGB_NUMBER/3*2;
				while (d_x>=USER_RGB_NUMBER)
				{
					d_x=d_x-USER_RGB_NUMBER;
				}
				_this_rgb->rgb_buff[d_x]=color4;
			}		
			
			user_rgb_send_cnt(1);
			break;	
		case RGB_MODE_5:
			c_be = get_energyStep(16);
			if (light_mode_mark%2)
			{
				if(time2++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time2=0;
			}
			else
			{
				if(time1++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					return;
				}
				time1=0;
			}
			
			cnt_2++;
			if (cnt_2>=200)
			{
				cnt_2=0;
			}
			for (int i = 0;   i < USER_RGB_NUMBER/2; ++  i)
			{
				color4=get_gradual_change_color2(cnt_2+i,200);
				_this_rgb->rgb_buff[USER_RGB_NUMBER/2-1-i]=color4;
				_this_rgb->rgb_buff[USER_RGB_NUMBER/2+i]=color4;
			}
			cnt_1++;
			if (cnt_1>=(USER_RGB_NUMBER/2))
			{
				cnt_1=0;
			}
			for (int i = 0;   i < USER_RGB_NUMBER/2/2; ++  i)
			{
				d_x=cnt_1+i;
				if (d_x>=USER_RGB_NUMBER/2)
				{
					d_x=d_x-USER_RGB_NUMBER/2;
				}
				_this_rgb->rgb_buff[USER_RGB_NUMBER-1-d_x]=color3;
				_this_rgb->rgb_buff[d_x]=color3;
			}
			user_rgb_send_cnt(1);
		break;	
		case RGB_MODE_6:
			c_be = get_energyStep(16);
			if (light_mode_mark%2)
			{
				if(time2++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					if ( c_be)
					{
						kkkkkkkkkkkk=30;
						color4=get_gradual_change_color2(cnt_2,60);
						music_dn_to_up_mode(USER_RGB_NUMBER/2, USER_RGB_NUMBER-1, color4,c_be);
						for (int i = 0;   i < USER_RGB_NUMBER/2; ++  i)
						{
							_this_rgb->rgb_buff[USER_RGB_NUMBER/2-1-i]=_this_rgb->rgb_buff[USER_RGB_NUMBER/2+i];
						}
					}
					return;
				}
				time2=0;
			}
			else
			{
				if(time1++<(	(8*16)/((c_be+1)*4) +1	)) 
				{
					if ( c_be)
					{
						kkkkkkkkkkkk=30;
						color4=get_gradual_change_color2(cnt_2,60);
						music_dn_to_up_mode(USER_RGB_NUMBER/2, USER_RGB_NUMBER-1, color4,c_be);
						for (int i = 0;   i < USER_RGB_NUMBER/2; ++  i)
						{
							_this_rgb->rgb_buff[USER_RGB_NUMBER/2-1-i]=_this_rgb->rgb_buff[USER_RGB_NUMBER/2+i];
						}
					}
					return;
				}
				time1=0;
			}
			user_rgb_clear_colour();
		
			cnt_2++;
			if (cnt_2>=60)
			{
				cnt_2=0;
			}
			color4=get_gradual_change_color2(cnt_2,60);
		
			if ( c_be==0 && kkkkkkkkkkkk==0)
			{
				cnt_1++;
				if (cnt_1>=(USER_RGB_NUMBER-1))
				{
					cnt_1=1;
				}
				d_x=cnt_1;
				if (d_x>=USER_RGB_NUMBER/2)
				{
					d_x=USER_RGB_NUMBER-2-d_x;
				}
				for (int i = 0;   i <= d_x; ++	i)
				{
					_this_rgb->rgb_buff[USER_RGB_NUMBER/2-1-i]=color4;
					_this_rgb->rgb_buff[USER_RGB_NUMBER/2+i]=color4;
				}
			}
			else
			{
				if (c_be)
				{
					kkkkkkkkkkkk=30;
				}
				else
				{
					if (kkkkkkkkkkkk)
					{
						kkkkkkkkkkkk--;
					}
				}
				music_dn_to_up_mode(USER_RGB_NUMBER/2, USER_RGB_NUMBER-1, color4,c_be);
				for (int i = 0;   i < USER_RGB_NUMBER/2; ++  i)
				{
					_this_rgb->rgb_buff[USER_RGB_NUMBER/2-1-i]=_this_rgb->rgb_buff[USER_RGB_NUMBER/2+i];
				}
		
			}
		
		
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


