#include "spi_rgb_config.h"

#define USER_RGB_NUMBER (28)

#define USER_RGB_CODE0  0X60//?1?7?1?72bit ?1?7?1?76bit 0xC0 ?1?7?1?7?1?7?1?71λ 0x60
#define USER_RGB_CODE1  0X7c//?1?7?1?74bit ?1?7?1?74bit 0xF0 ?1?7?1?7?1?7?1?71λ 0x78
#define USER_RGB_COLOUR_LEN (USER_RGB_NUMBER*3)

#define USER_RGB_BUFF_MALLOC_EN 0//rgb?1?7?1?7spi buff?1?7?0?9?1?7?0?2?1?7?1?7?0?0?1?7?1?7?1?7?1?7?0?8?1?7 ?1?7?1?7?0?0?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?0?7?0?8?1?7?1?7?1?7С ?1?7?1?7?1?7?1?7?1?7?1?7?1?7?7?3?1?7?1?7?1?7?1?7?1?7?1?7С?1?7?5?6?1?7?1?7 ?1?7?1?7?1?7?1?7?1?7?1?7?0?9?1?7?1?7?0?4?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7л?1?7?1?7?1?7?1?7?1?7?1?7?1?7

//跟随音乐跳动参数
#define MAX_ENERGY		13		//最大能量等级
#define down_speed		5		//数值越大下降速度越慢


typedef struct _USER_RGB_MAPPING_NXY_{
    // u8 clash;//1?1?7?1?7xy->n 2:n->xy 3:round0->n 4:n->round0 5:round1->n 6:n->round1
    u16 N;//?1?7?0?6?1?7?1?7?1?7?1?7?1?7?1?8?1?7?1?7?1?7
    u8 X;//?1?7?0?6?1?7?1?7?1?8?1?7x?1?7?1?7?1?7?1?7
    u8 Y;//?1?7?0?6?1?7?1?7?1?8?1?7y?1?7?1?7?1?7?1?7
    // u32 round;
}RGB_MAP_NXY;

typedef struct _USER_RGB_MAPPING_XYN_{
    // u8 clash;//1?1?7?1?7xy->n 2:n->xy 3:round0->n 4:n->round0 5:round1->n 6:n->round1
    u8 X;//?1?7?0?6?1?7?1?7?1?8?1?7x?1?7?1?7?1?7?1?7
    u8 Y;//?1?7?0?6?1?7?1?7?1?8?1?7y?1?7?1?7?1?7?1?7
    u32 N;//?1?7?0?6?1?7?1?7?1?7?1?7?1?7?1?8?1?7?1?7?1?7
    // u32 round;
}RGB_MAP_XYN;
//?1?7?1?7?0?2
typedef struct _RGB_DEV_COLOUR_CNT_{
    int g;
    int r;
    int b;
}RGB_DEV_COLOUR_CNT;

//spi rgb ?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7 ?0?3?1?7?1?7 ?1?7ο?1?7?1?7?0?1?1?7?1?7?0?4?1?7?1?7?0?2?1?7
typedef struct _RGB_DEV_SPI_COLOUR_{
    u8 g[8];
    u8 r[8];
    u8 b[8];
}RGB_DEV_SPI_COLOUR;

//0?1?7?1?7 1?1?7?1?7 ?1?7?1?7?1?7?1?7?0?5?1?7?1?7?1?7?1?3?1?7?1?7?0?4?1?7?1?7?0?2?1?7?1?7?1?7 ?1?7?1?1?0?3?1?7?0?9?1?7?1?7?0?8?1?7?0?7?1?7?1?7?1?7?1?7?1?7?0?6?1?7?1?7u8?1?7?1?7?1?7?1?70?1?7?1?71?1?7?0?3?1?7?1?7?1?7  ?1?7?1?5?1?7?1?7spi?1?7?1?7?1?7?1?7?0?7u8?1?7?1?7?0?2?1?7?1?7?1?7?1?7 ?1?7?1?7?1?7?1?7?1?7?0?9?1?7?1?7?1?7?1?7?1?7
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
    V_L Brightness_level;//?1?7?1?7?1?7?0?0?0?7?1?7
    volatile bool rend_flag;
    volatile bool updata_flag;//?1?7?1?7?1?7?1?7?0?2?1?7?1?7?1?7±?1?7?0?4
    volatile u8   updata_cnt;//?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?2?1?7?1?7?1?7
    const RGB_DEV_CODE code;
    const u8 spi_port;//spi
    const u32 number;//?1?7?1?7?1?7?1?9?1?7?1?7?1?7
    u16 spi_scan_time;//spi ?1?7?1?7?1?7?1?6?1?7?1?7?0?5?1?7?1?7?1?7?1?7?0?0?1?7?1?7?0?2?1?7?1?7

    #if USER_RGB_BUFF_MALLOC_EN
    RGB_DEV_COLOUR *rgb_buff;//[USER_RGB_NUMBER];//?1?7?1?7?1?7?1?7?0?2 ?1?7洢buff
    RGB_DEV_SPI_COLOUR *spi_buff;//[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi ?1?7?1?7?1?7?1?7buff
    #elif (defined(USER_RGB_NUMBER) && USER_RGB_NUMBER)
    RGB_DEV_COLOUR rgb_buff[USER_RGB_NUMBER];//?1?7?1?7?1?7?1?7?0?2 ?1?7洢buff
    RGB_DEV_SPI_COLOUR spi_buff[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi ?1?7?1?7?1?7?1?7buff
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
    // .spi_scan_time = 20,//?1?7?1?7?1?7?1?3?0?1?1?7?1?7?1?7?1?7?1?7?1?7?1?7?0?9?1?7?1?7?0?2?1?7?1?7
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
    u32 mode_scan_ms;//?0?0?0?4?0?9?1?7?1?7?0?2?1?7?1?7
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

//?1?7?0?5?1?7?1?7?1?7rgb?1?7?1?7?1?7?1?7?0?8?1?7?1?7?1?7?1?7spi?1?7?1?7
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
 * ?1?7?1?7?1?7?0?1?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7
 * ?1?7?1?7?0?8?1?7?1?7rgb buff ?1?7?1?7?1?7?1?7
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
 * ?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7
 * ?0?8?1?7?1?7rgb buff ?1?7?1?7?1?7?1?7
*/
static void rgb_set_all_data_brightness(void){

    if(!_this_rgb || !_this_rgb->Brightness_level.max || !_this_rgb->rgb_buff){
        return;
    }

    for(s32 i=0;i<_this_rgb->number;i++){
        _this_rgb->rgb_buff[i] = rgb_set_only_brightness(_this_rgb->rgb_buff[i]);
    }
}


//?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?0?2?1?7?1?7?1?7?1?7?0?8?1?7?1?7?1?7?1?7spi?1?7?1?7?1?7?1?7
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

//?1?7?1?7?1?7?0?1?1?8?1?7?1?7?1?7rgb?1?7?1?7?0?2
/*
priv?1?7?1?7rgb ?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7
colour?1?7?1?7rgb ?1?7?1?7?0?2
number:?1?7?1?7io?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?0?6?0?7?1?7?0?2?1?7?1?7?0?5?1?7?0?0?1?7?1?7?1?7?1?7?0?5?1?7?0?7?1?7?1?7?0?3?1?7?0?5?0?2 -1?1?7?1?7?1?7?1?7(USER_RGB_NUMBER-1)
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

//?1?7?1?7?1?7?1?7?0?2?0?4?1?7?1?7?1?7?1?7?0?2
void user_rgb_same_colour(RGB_DEV_COLOUR *colour){
    RGB_DEV_INFO *rgb = _this_rgb;
    if(!rgb || !colour || !rgb->number){
        return;
    }

    for(s32 i=0;i<rgb->number;i++){
        user_rgb_colour_only_set(colour,i);
    }
}

//?1?7?1?7?1?7?1?7?1?7?0?2
void user_rgb_clear_colour(void){
    RGB_DEV_INFO *rgb = _this_rgb;
    RGB_DEV_COLOUR colour={0,0,0};

    if(!rgb || !rgb->number){
        return;
    }

    user_rgb_same_colour(&colour);
}

/*---------------------------------------------------------*/
/**@brief   ?1?7?1?7?1?7?1?7?1?4?1?7λ?1?7?0?1?0?1?1?7?1?7?1?7?0?2
   @param   _start:?1?7?1?7?0?3λ?1?7?1?7
   @param   _end: ?1?7?1?7?1?7?1?7λ?1?7?1?7
   @return  null
   @note	?1?7?0?2?1?7?1?7?0?0?1?70?1?7?1?7?0?3?1?7?1?7_end?1?7?1?7?1?7?1?7_this_rgb->number?1?7?1?7?1?70?1?7?1?7?0?3
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
/**@brief   ?1?7趨?1?7?1?4?1?7λ?1?7?0?1?0?1?1?7?1?7?1?7?0?2
   @param   _start:?1?7?1?7?0?3λ?1?7?1?7
   @param   _end: ?1?7?1?7?1?7?1?7λ?1?7?1?7
   @param   _color: ?1?7?1?7?0?2
   @return  null
   @note	?1?7?0?2?1?7?1?7?0?0?1?70?1?7?1?7?0?3?1?7?1?7_end?1?7?1?7?1?7?1?7_this_rgb->number?1?7?1?7?1?70?1?7?1?7?0?3
/*---------------------------------------------------------*/
void set_color(u8 _start,u8 _end,RGB_DEV_COLOUR _color){
    for(s8 i=_start;i<_end+1;i++){
		  user_rgb_colour_only_set(&_color,i);
	}
}


//?1?7?1?7?1?7?1?7?0?5?1?7?1?7?1?7?1?7?1?7?1?7
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

//?1?7?1?7?1?7?1?7spi?1?7?1?7?1?7?1?7
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

//?1?7?1?7?1?7?1?5?1?7?1?7?0?3?1?7?1?7?1?7
//0 ?1?7?1?7?1?7cnt?1?7?1?7?1?7?1?7 ?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7
//?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?2?1?7?1?7?1?7
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

//?1?7?1?7?0?3?1?7?1?7
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

    //?1?7?1?7?1?7?1?3?0?1?1?7?1?7?1?7?1?7?1?7?1?7?1?7?0?9?1?7?1?7?0?2?1?7?1?7
    u32 tp_time = 7*_this_rgb->number/100;
    _this_rgb->spi_scan_time = tp_time+4;
    if(!(_this_rgb->spi_scan_time)){
        _this_rgb->spi_scan_time = 20;
    }
    r_printf("spi scan time=%d tp time=%d",_this_rgb->spi_scan_time,tp_time);

    user_rgb_send_cnt(0);
    _this_rgb->init_flag = true;

     sys_s_hi_timerout_add(NULL,user_rgb_send,_this_rgb->spi_scan_time+3000);//?1?7?1?7spi?0?8?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7?0?4?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7 ?1?7?1?7?0?6rgb?1?7?1?7?1?7?1?7?0?8?0?5?1?7?1?7
}

void user_rgb_dev_exit(void){
    // local_irq_disable();

    if(_this_rgb){
        user_rgb_clear_colour();
        _this_rgb->updata_flag = 0;//?1?7?1?7?1?7?1?7 user_rgb_send_strl ?1?7?1?7updata_flag?0?21
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

void rgb_light_bar_oneway_flow_mode(u8 colour[][3],u8 rownum)
{
	once_led_breath_light();
	static u8 colour_number,run_dir,flow_mode_time;
	static u16 change_dir_cnt;
	static s8 old_colour;
	u8 now_colour;//?1?7?1?7?1?7?1?7?1?7?1?7?0?5?1?7?1?7?0?8?1?7?1?7?0?5?1?7?1?7?1?7?1?7?0?2
	static s8 run_num=0;//?1?7?1?7?1?7?0?1?1?7?1?7?1?9?1?7
	
	RGB_DEV_COLOUR tp_rgb;

	u8 c_be = get_energyStep(MAX_ENERGY);
	//?1?7?0?3?5?0?1?7?1?7
	if(change_dir_cnt++>2000){
		change_dir_cnt = 0;
		run_dir =! run_dir;
	}
	
	//?1?7?1?7?0?8?1?7?1?7?0?5?1?7?1?7?0?2?1?7?1?7?1?7?1?7?1?7?1?7
	colour_number=rownum;

	if(flow_mode_time++<((MAX_ENERGY*10)/((c_be+1)*4))) return;
	flow_mode_time=0;

	for(int i=0;i<36;i++)
	{
		if(i<run_num)//?1?7?0?9?1?7?1?7?1?7?1?7?1?7
		{
			now_colour=(old_colour+rgb_light_bar_oneway_flow_mode_piece_set)%colour_number;
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		#if rgb_light_bar_oneway_flow_mode_piece_set>=2	//?1?7?1?7?1?7?1?7?1?7?0?3?1?7?1?7?1?7?1?7?1?7
		else if(i<(rgb_light_bar_oneway_flow_mode_space_set+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?7?0?5?1?7?1?7
		{
			#if rgb_light_bar_oneway_flow_mode_piece_set==2
			now_colour=(old_colour+1)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==3
			now_colour=(old_colour+2)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==4
			now_colour=(old_colour+3)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==5
			now_colour=(old_colour+4)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==6
			now_colour=(old_colour+5)%colour_number;
			#endif
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		#endif	
		#if rgb_light_bar_oneway_flow_mode_piece_set>=3	//?1?7?1?7?1?7?1?7?1?7?0?3?1?7?1?7?1?7?1?7?1?7
		else if(i<(2*rgb_light_bar_oneway_flow_mode_space_set+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?2?1?7?1?7?1?7
		{
			#if rgb_light_bar_oneway_flow_mode_piece_set==3
			now_colour=(old_colour+1)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==4
			now_colour=(old_colour+2)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==5
			now_colour=(old_colour+3)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==6
			now_colour=(old_colour+4)%colour_number;
			#endif
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		#endif
		#if rgb_light_bar_oneway_flow_mode_piece_set>=4	//?1?7?1?7?1?7?1?7?1?7?0?3?1?7?1?7?0?8?1?7
		else if(i<(3*rgb_light_bar_oneway_flow_mode_space_set+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?7?1?7?1?7?1?7?1?7
		{
			#if rgb_light_bar_oneway_flow_mode_piece_set==4
			now_colour=(old_colour+1)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==5
			now_colour=(old_colour+2)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==6
			now_colour=(old_colour+3)%colour_number;
			#endif
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		#endif
		#if rgb_light_bar_oneway_flow_mode_piece_set>=5	//?1?7?1?7?1?7?1?7?1?7?0?3?1?75?1?7?1?7
		else if(i<(4*rgb_light_bar_oneway_flow_mode_space_set+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?74?1?7?1?7
		{
			#if rgb_light_bar_oneway_flow_mode_piece_set==5
			now_colour=(old_colour+1)%colour_number;
			#elif rgb_light_bar_oneway_flow_mode_piece_set==6
			now_colour=(old_colour+2)%colour_number;
			#endif
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		#endif
		#if rgb_light_bar_oneway_flow_mode_piece_set>=6//?1?7?1?7?1?7?1?7?1?7?0?3?1?76?1?7?1?7
		else if(i<(5*rgb_light_bar_oneway_flow_mode_space_set+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?75?1?7?1?7
		{
			#if rgb_light_bar_oneway_flow_mode_piece_set==6
			now_colour=(old_colour+1)%colour_number;
			#endif
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		#endif
		else //?1?7?1?7?0?2?1?7?1?7?1?7?1?7?1?7?0?5?1?7?1?7
		{
			now_colour=old_colour;
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}
		user_rgb_colour_only_set(&tp_rgb,i);
	}

	tp_rgb = get_gradual_change_color();
	set_color(36,44,tp_rgb);

	if(run_dir){
		run_num++;
		if(run_num>=rgb_light_bar_oneway_flow_mode_space_set)
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
			run_num=rgb_light_bar_oneway_flow_mode_space_set-1;
			old_colour--;
			if(old_colour<0)
			{
				old_colour=colour_number-1;
			}
		}
	}
	
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
	//?1?7?0?3?5?0?1?7?1?7
	if(change_dir_cnt++>2000){
		change_dir_cnt = 0;
	//	run_dir =! run_dir;
	}
	
	//?1?7?1?7?0?8?1?7?1?7?0?5?1?7?1?7?0?2?1?7?1?7?1?7?1?7?1?7?1?7
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
		if((i-_start)<run_num)//?1?7?0?9?1?7?1?7?1?7?1?7?1?7
		{
			now_colour=(old_colour+marquee_num)%colour_number;
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(light_num+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?7?0?5?1?7?1?7
		{
			now_colour=(old_colour+marquee_num-1)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}


		else if((i-_start)<(2*light_num+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?2?1?7?1?7?1?7
		{
			now_colour=(old_colour+marquee_num-2)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(3*light_num+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?7?1?7?1?7?1?7?1?7
		{
			now_colour=(old_colour+marquee_num-3)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(4*light_num+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?74?1?7?1?7
		{
			now_colour=(old_colour+marquee_num-4)%colour_number;
			
			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else if((i-_start)<(5*light_num+run_num))//?1?7?1?7?1?7?1?7?1?7?0?7?1?75?1?7?1?7
		{
			now_colour=(old_colour+marquee_num-5)%colour_number;

			tp_rgb.r=colour[now_colour][0];
			tp_rgb.g=colour[now_colour][1];
			tp_rgb.b=colour[now_colour][2];
		}

		else //?1?7?1?7?0?2?1?7?1?7?1?7?1?7?1?7?0?5?1?7?1?7
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

void once_light_breath(s8 pos){
	static u8 bright_level,cnt=0,breath_status;
	RGB_DEV_COLOUR color;
	//user_rgb_clear_colour();

	color = get_gradual_change_color();

	if(color.r-bright_level<=0)color.r=0;else color.r-=bright_level;
	if(color.g-bright_level<=0)color.g=0;else color.g-=bright_level;
	if(color.b-bright_level<=0)color.b=0;else color.b-=bright_level;

	user_rgb_colour_only_set(&color,pos);
	if(cnt++>10){
		if(!breath_status && (bright_level+=1)>245)breath_status=1; 
		else if(breath_status && (bright_level-=1)<1)breath_status=0;
	}
}

void some_light_breath(u8 _start,u8 _end,RGB_DEV_COLOUR color){
	static u8 bright_level,cnt=0,breath_status;
	//RGB_DEV_COLOUR color;

	//color = get_gradual_change_color();
	
	if(color.r-bright_level<=0)color.r=0;else color.r-=bright_level;
	if(color.g-bright_level<=0)color.g=0;else color.g-=bright_level;
	if(color.b-bright_level<=0)color.b=0;else color.b-=bright_level;

	set_color(_start, _end, color);

	if(cnt++>10){
		if(!breath_status && (bright_level+=1)>245)breath_status=1; 
		else if(breath_status && (bright_level-=1)<1)breath_status=0;
	}
}

void music_dn_to_up_mode(u8 _start,u8 _end,RGB_DEV_COLOUR color){
	static u8 pos,cur_be,old_be,count_to_add,light_number;
	//RGB_DEV_COLOUR color2;
	u8 c_be = get_energyStep(MAX_ENERGY);
	//color = get_gradual_change_color();

	cur_be=c_be;

	light_number = abs(_end - _start) + 1;
	
	if(old_be<=cur_be){
		pos=(cur_be*light_number)/MAX_ENERGY;
		if(pos<1)pos=1;
		if(cur_be<1)cur_be=1;
		old_be=cur_be;
	}else{
		if(count_to_add++>down_speed){
			count_to_add=0;
			if(pos>2)pos--;
			else pos=1;
			old_be=pos*MAX_ENERGY/light_number;
			if(old_be<1)old_be=1;
		}
	}

	if((_end-_start)>0){
		for(s8 i=_start;i<pos+_start;i++){
			user_rgb_colour_only_set(&color,i);
#if 1			
			if(i>=pos+_start-1){
				color.r = 0xff - color.r;
				color.g = 0xff - color.g;
				color.b = 0xff - color.b;
				user_rgb_colour_only_set(&color,i);
			}
#endif			
		}
	}else{
		for(s8 i=_start;i>_start-pos;i--){
			user_rgb_colour_only_set(&color,i);
#if 1			
			if(i<=_start-pos+1){
				color.r = 0xff - color.r;
				color.g = 0xff - color.g;
				color.b = 0xff - color.b;
				user_rgb_colour_only_set(&color,i);
			}
#endif			
		}
	}
	
}

void marquee_and_breath_mode(){
	static u8 be_null,null_count;
	u8 c_be = get_energyStep(MAX_ENERGY);
	RGB_DEV_COLOUR color;

	color = get_gradual_change_color();
	
	if(c_be>0){
		be_null=1;
		null_count=0;
		music_dn_to_up_mode(36,44,color);
	}else{
		if(be_null){
			if(null_count++>200){
				null_count = 0;
				be_null = 0;
			}
		}else{
			some_light_breath(36,44,color);
		}
	}
}

void gradual_mode(){
	RGB_DEV_COLOUR color;
	color = get_gradual_change_color();
	set_color(0,35,color);
	color.r = 0xff - color.r;
	color.g = 0xff - color.g;
	color.b = 0xff - color.b;
	set_color(36,44,color);
}

void gradual_and_marquee_mode(){
	static u8 color_status,pos,cnt;
	
	RGB_DEV_COLOUR color;
	color = get_gradual_change_color();
	set_color(0,35,color);

	if(color_status){
		color.r = 0xff - color.r;
		color.g = 0xff - color.g;
		color.b = 0xff - color.b;
	}else{
		color.r = 0;
		color.g = 0;
		color.b = 0;
	}
	
	user_rgb_colour_only_set(&color,36+pos);

	if(cnt++>10){
		cnt = 0;
		if(pos++>8){
			pos = 0;
			color_status =! color_status;
		}
	}
}

void two_marquee_and_gradual(){
	static u8 pos=1,cnt,dir_status=0;
	static u16 dir_cnt=0;
	RGB_DEV_COLOUR color;

	color = get_gradual_change_color();
	
	u8 c_be = get_energyStep(MAX_ENERGY);

	clear_color(0, 35);

	if(dir_cnt++>2000){
		dir_cnt = 0;
		dir_status =! dir_status;
	}

	//pos = 13;

	if(pos<12){
		set_color(0, pos-1, color);
		set_color(pos+6, pos+16, color);
		set_color(24+pos, 35, color);
	}else{
		set_color(pos-12, pos-1, color);
		set_color(6+pos, pos+17, color);
	}

	if(cnt++>((MAX_ENERGY*10)/((c_be+1)*6))){
		cnt=0;
		if(dir_status){
			if(pos++>17)pos=1;
		}else{
			if(pos--<2)pos=18;
		}
	}

	color.r = 0xff - color.r;
	color.g = 0xff - color.g;
	color.b = 0xff - color.b;
	some_light_breath(36,44,color);
}

void no_music_double_dn_to_up(){
	static u8 pos=0,cnt=0;
	RGB_DEV_COLOUR color;

	color = get_gradual_change_color();
	//pos=0;
	u8 c_be = get_energyStep(MAX_ENERGY);

	if(pos<=14){
		user_rgb_colour_only_set(&color,14-pos);
	}else{
		user_rgb_colour_only_set(&color,35-pos+15);
	}
	user_rgb_colour_only_set(&color,15+pos);

	if(pos>4&&pos<14){
		user_rgb_colour_only_set(&color,44-pos+5);
	}

	if(cnt++>((MAX_ENERGY*10)/((c_be+1)*6))){
		cnt = 0;
		if(pos++>16)pos=0;
	}

}

void music_double_dn_to_up(){
	RGB_DEV_COLOUR color;
	color = get_gradual_change_color();
	music_dn_to_up_mode(14,0,color);
	music_dn_to_up_mode(15,29,color);
	music_dn_to_up_mode(32,30,color);
	music_dn_to_up_mode(33,35,color);
	color.r = 0xff - color.r;
	color.g = 0xff - color.g;
	color.b = 0xff - color.b;
	music_dn_to_up_mode(36,44,color);
}

void ssm_gradual_color_mode(){
	static u8 cnt=0;
	static RGB_DEV_COLOUR old_color0,old_color1,old_color2,old_color3,old_color4;
	static RGB_DEV_COLOUR old_color5,old_color6,old_color7,old_color8,old_color9;

	if(cnt%2==0){
		old_color0 = get_gradual_change_color();
	}

	if(cnt++>30){
		old_color8 = old_color7;
		old_color7 = old_color6;
		old_color6 = old_color5;
		old_color5 = old_color4;
		old_color4 = old_color3;
		old_color3 = old_color2;
		old_color2 = old_color1;
		old_color1 = old_color0;
		cnt=0;
	}

	for(u8 i=13;i<17;i++){
		user_rgb_colour_only_set(&old_color0,i);
	}
	user_rgb_colour_only_set(&old_color0,36);

	user_rgb_colour_only_set(&old_color1,12);
	user_rgb_colour_only_set(&old_color1,11);
	user_rgb_colour_only_set(&old_color1,17);
	user_rgb_colour_only_set(&old_color1,18);
	
	user_rgb_colour_only_set(&old_color1,37);

	user_rgb_colour_only_set(&old_color2,10);
	user_rgb_colour_only_set(&old_color2,9);
	user_rgb_colour_only_set(&old_color2,19);
	user_rgb_colour_only_set(&old_color2,20);

	user_rgb_colour_only_set(&old_color2,38);
	
	user_rgb_colour_only_set(&old_color3,8);
	user_rgb_colour_only_set(&old_color3,7);
	user_rgb_colour_only_set(&old_color3,21);
	user_rgb_colour_only_set(&old_color3,22);

	user_rgb_colour_only_set(&old_color3,39);

	user_rgb_colour_only_set(&old_color4,6);
	user_rgb_colour_only_set(&old_color4,5);
	user_rgb_colour_only_set(&old_color4,23);
	user_rgb_colour_only_set(&old_color4,24);

	user_rgb_colour_only_set(&old_color4,40);

	user_rgb_colour_only_set(&old_color5,4);
	user_rgb_colour_only_set(&old_color5,3);
	user_rgb_colour_only_set(&old_color5,25);
	user_rgb_colour_only_set(&old_color5,26);
	
	user_rgb_colour_only_set(&old_color5,41);
	
	user_rgb_colour_only_set(&old_color6,2);
	user_rgb_colour_only_set(&old_color6,1);
	user_rgb_colour_only_set(&old_color6,27);
	user_rgb_colour_only_set(&old_color6,28);

	user_rgb_colour_only_set(&old_color6,42);

	user_rgb_colour_only_set(&old_color7,0);
	user_rgb_colour_only_set(&old_color7,35);
	user_rgb_colour_only_set(&old_color7,29);
	user_rgb_colour_only_set(&old_color7,30);

	user_rgb_colour_only_set(&old_color7,43);

	user_rgb_colour_only_set(&old_color8,34);
	user_rgb_colour_only_set(&old_color8,33);
	user_rgb_colour_only_set(&old_color8,31);
	user_rgb_colour_only_set(&old_color8,32);

	user_rgb_colour_only_set(&old_color8,44);

}

void no_music_gradual_and_music_jump(){
	static u8 be_status,be_null;
	u8 c_be = get_energyStep(MAX_ENERGY);

	if(c_be>0){
		be_status = 1;
		be_null = 0;
		music_double_dn_to_up();
	}else{
		if(be_status){
			if(be_null++>200){
				be_null = 0;
				be_status = 0;
			}
		}else{
			ssm_gradual_color_mode();
		}
	}
	

}

void l_mode(RGB_DEV_COLOUR color){
	for(int i=18;i<29;i++){
		user_rgb_colour_only_set(&color,i);
	}
}

void r_mode(RGB_DEV_COLOUR color){
	for(int i=0;i<12;i++){
		user_rgb_colour_only_set(&color,i);
	}
}

void u_mode(RGB_DEV_COLOUR color){
	for(int i=30;i<35;i++){
		user_rgb_colour_only_set(&color,i);
	}
}

void d_mode(RGB_DEV_COLOUR color){
	for(int i=12;i<18;i++){
		user_rgb_colour_only_set(&color,i);
	}
}

void ld_mode(RGB_DEV_COLOUR color){
	l_mode(color);
	d_mode(color);
}

void ru_mode(RGB_DEV_COLOUR color){
	r_mode(color);
	u_mode(color);
}

void ud_mode(RGB_DEV_COLOUR color){
	d_mode(color);
	u_mode(color);
}

void lr_mode(RGB_DEV_COLOUR color){
	l_mode(color);
	r_mode(color);
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


void quick_trans(){
	static u8 cur_mode,cnt,dk;
	RGB_DEV_COLOUR color;
	u8 c_be=0;
	c_be = get_energyStep(MAX_ENERGY);
	
	color = get_gradual_change_color();

	light_flash(36,44,color);
	
	if(cnt++>((MAX_ENERGY*20)/((c_be+1)*2))){
		cnt = 0;
		if(cur_mode++>6){
			cur_mode=0;
		}
	}

	user_rgb_clear_colour();

	switch(cur_mode){
		case 0:
			l_mode(color);
			break;
		case 1:
			r_mode(color);
			break;
		case 2:
			u_mode(color);
			break;
		case 3:
			d_mode(color);
			break;
		case 4:
			ld_mode(color);
			break;
		case 5:
			ru_mode(color);
			break;	
		case 6:
			ud_mode(color);
			break;
		case 7:
			lr_mode(color);
			break;
		default:
			break;
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
	static u8 kcnt,cnt;
	u16 energy_value=0;
	energy_value = audio_dac_energy_get();	

	if(!(use_ctl.led_vol+abs(use_ctl.mic_energy))){
		if(kcnt++>50){
			bE=0;
			kcnt = 0;
		}
	}else{
		if(use_ctl.mic_energy){
			bE=((energy_value+abs(use_ctl.mic_energy))/2)/(use_ctl.led_vol/3+20);
			if(cnt++>20){
				cnt=0;
				//printf("get_energyStep==%d %d %d\n",bE,use_ctl.mic_energy,energy_value);
			}
			
		}else
			bE=energy_value/((use_ctl.led_vol/3)+20);
			if(cnt++>20){
				cnt=0;
				//printf("get_energyStep==%d %d %d\n",bE,use_ctl.mic_energy,energy_value);
			}
	}
    bE = bE/13;
	//printf("bE=======%d\n",bE);

	if(bE>max_energy)bE=max_energy;
	return bE;
}


void no_music_circle_and_music_jump(){
	u8 energy;
	static u16 time_to_change;
	static bool n_status;
	energy = audio_dac_energy_get()+abs(use_ctl.mic_energy);
	RGB_DEV_COLOUR color = get_gradual_change_color();

	//printf("get_energyStep==%d\n",energy);

	if(energy){
		n_status = 1;
		time_to_change = 0;
		user_rgb_clear_colour();
		music_dn_to_up_mode(13,0,color);
		music_dn_to_up_mode(14,27,color);
	}else{
		if(n_status){
			if(time_to_change++<500)return;
			time_to_change = 0;
			n_status = 0;
		}
		rgb_marquee_mode(rgbycp_six_color,3,0,28);
	}
}


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
	
	//light_mode_mark = RGB_MODE_1;

    switch (light_mode_mark){
    case RGB_OFF:
        user_rgb_clear_colour();
		user_rgb_send_cnt(1);
        break;
    case RGB_MODE_1:
		no_music_circle_and_music_jump();
		user_rgb_send_cnt(1);
        break;		
	case RGB_MODE_2:
		///gradual_mode();
		rgb_marquee_mode(rgbycp_six_color,3,0,18);
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_3: 
		rgb_marquee_mode(rgbycp_six_color,3,0,22);
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_4:
		rgb_marquee_mode(rgbycp_six_color,3,0,22);
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_5:
		rgb_marquee_mode(rgbycp_six_color,3,0,22);
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_6:
		rgb_marquee_mode(rgbycp_six_color,3,0,22);
		user_rgb_send_cnt(1);
		break;
    default:
        break;
    }
}
//?1?7?1?7?0?3?1?7?1?7
void user_rgb_fun_init(void){
    if(user_rgb_info.init_flag){
        return;
    }


    //rgb spi init
    user_rgb_dev_init(&user_rgb_info);

    //?1?7?1?7?1?7?1?7?0?5?1?7?1?7spi?1?7?1?7?1?7?1?7
   // user_rgb_send_cnt(1);

}

//?1?7?0?3?1?7
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


