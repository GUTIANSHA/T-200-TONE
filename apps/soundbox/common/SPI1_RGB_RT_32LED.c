
u16 rgbpostest;

u8 light_mode_mark;
u8 bEnergyStep=0; //当前能量级数


#define USER_RGB_NUMBER (28)

#define USER_RGB_CODE0  0X60//高2bit 低6bit 0xC0 右移1位 0x60
#define USER_RGB_CODE1  0X7c//高4bit 低4bit 0xF0 右移1位 0x78
#define USER_RGB_COLOUR_LEN (USER_RGB_NUMBER*3)

#define USER_RGB_BUFF_MALLOC_EN 0//rgb、spi buff是否为动态分配空间 动态分配如果超过栈剩余大小 会有问题。超过得小会报错 超过得太多没报错但是运行会有问题


typedef struct _USER_RGB_MAPPING_NXY_ {
    // u8 clash;//1：xy->n 2:n->xy 3:round0->n 4:n->round0 5:round1->n 6:n->round1
    u16 N;//灯板物理第几颗
    u8 X;//灯板逻辑x坐标
    u8 Y;//灯板逻辑y坐标
    // u32 round;
}RGB_MAP_NXY;

typedef struct _USER_RGB_MAPPING_XYN_ {
    // u8 clash;//1：xy->n 2:n->xy 3:round0->n 4:n->round0 5:round1->n 6:n->round1
    u8 X;//灯板逻辑x坐标
    u8 Y;//灯板逻辑y坐标
    u32 N;//灯板物理第几颗
    // u32 round;
}RGB_MAP_XYN;
//颜色
typedef struct _RGB_DEV_COLOUR_ {
    u8 g;
    u8 r;
    u8 b;
}RGB_DEV_COLOUR;

typedef struct _RGB_DEV_COLOUR_CNT_{
    int g;
    int r;
    int b;
}RGB_DEV_COLOUR_CNT;

//spi rgb 数据排序 顺序 参考灯的用户手册
typedef struct _RGB_DEV_SPI_COLOUR_{
    u8 g[8];
    u8 r[8];
    u8 b[8];
}RGB_DEV_SPI_COLOUR;

//0码 1码 具体值根据灯用户手册中 高低电平的占空比设置源码u8类型0与1的比例  再结合spi发送每u8的时间间隔 决定是否右移
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
    V_L Brightness_level;//亮度等级
    volatile bool rend_flag;
    volatile bool updata_flag;//灯颜色更新标志
    volatile u8   updata_cnt;//更新数据次数
    const RGB_DEV_CODE code;
    const u8 spi_port;//spi
    const u32 number;//灯总颗数
    u16 spi_scan_time;//spi 数据更新并发送间隔时间

    #if USER_RGB_BUFF_MALLOC_EN
    RGB_DEV_COLOUR *rgb_buff;//[USER_RGB_NUMBER];//灯颜色 存储buff
    RGB_DEV_SPI_COLOUR *spi_buff;//[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi 数据buff
    #elif (defined(USER_RGB_NUMBER) && USER_RGB_NUMBER)
    RGB_DEV_COLOUR rgb_buff[USER_RGB_NUMBER];//灯颜色 存储buff
    RGB_DEV_SPI_COLOUR spi_buff[USER_RGB_NUMBER] __attribute__((aligned(4)));//spi 数据buff
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
    // .spi_scan_time = 20,//根据灯颗数计算扫描时间
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
    u32 mode_scan_ms;//模式扫描时间
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

//把单个rgb数据转换成spi数
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
 * 设置单个数据亮度
 * 不影响rgb buff 数据
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
 * 设置所有数据亮度
 * 影响rgb buff 数据
*/
static void rgb_set_all_data_brightness(void){

    if(!_this_rgb || !_this_rgb->Brightness_level.max || !_this_rgb->rgb_buff){
        return;
    }

    for(s32 i=0;i<_this_rgb->number;i++){
        _this_rgb->rgb_buff[i] = rgb_set_only_brightness(_this_rgb->rgb_buff[i]);
    }
}


//把所有颜色数据转换成spi数据
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

//设置第几颗rgb颜色
/*
priv：rgb 配置数据
colour：rgb 颜色
number:离io口最近的那颗灯为第一颗，最后一颗可以表示为 -1或者(USER_RGB_NUMBER-1)
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

//设置为同样颜色
void user_rgb_same_colour(RGB_DEV_COLOUR *colour){
    RGB_DEV_INFO *rgb = _this_rgb;
    if(!rgb || !colour || !rgb->number){
        return;
    }

    for(s32 i=0;i<rgb->number;i++){
        user_rgb_colour_only_set(colour,i);
    }
}

//清除颜色
void user_rgb_clear_colour(void){
    RGB_DEV_INFO *rgb = _this_rgb;
    RGB_DEV_COLOUR colour={0,0,0};

    if(!rgb || !rgb->number){
        return;
    }

    user_rgb_same_colour(&colour);
}
//清除颜色
void user_rgb_all_colour(void){
    RGB_DEV_INFO *rgb = _this_rgb;
    RGB_DEV_COLOUR colour={255,255,255};

    if(!rgb || !rgb->number){
        return;
    }

    user_rgb_same_colour(&colour);
}


//发送一次数据
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

//发送spi数据
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

//数据发送控制
//0 清除cnt计数 不发送数据
//其他发送数据次数
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

//初始化
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

    //根据灯颗数计算扫描时间
    u32 tp_time = 7*_this_rgb->number/100;
    _this_rgb->spi_scan_time = tp_time+4;
    if(!(_this_rgb->spi_scan_time)){
        _this_rgb->spi_scan_time = 20;
    }
    r_printf("spi scan time=%d tp time=%d",_this_rgb->spi_scan_time,tp_time);

    user_rgb_send_cnt(0);
    _this_rgb->init_flag = true;

     sys_s_hi_timerout_add(NULL,user_rgb_send,_this_rgb->spi_scan_time+3000);//打开spi之后不能马上去发送数据 不然rgb会闪烁一下
}

void user_rgb_dev_exit(void){
    // local_irq_disable();

    if(_this_rgb){
        user_rgb_clear_colour();
        _this_rgb->updata_flag = 0;//避免 user_rgb_send_strl 中updata_flag为1
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



//-------------------------------------------------
//------------4格火焰旋转-------------------
//-------------------------------------------------
// grb
const u8 rgb_flame_color[6][3] =
{
  {0x00,0xb0,0x00}, 
  {0x00,0xe0,0x00}, 
  {0x05,0xe0,0x00}, 
  {0x10,0xe0,0x00}, 
  {0x15,0xe0,0x00}, 
  {0x20,0xe0,0x00}, 
  
  #if 0
  {0x00,0xFF,0x32}, 
  {0x00,0xFF,0x5A},
  {0x00,0xFF,0x82},
  {0x00,0xFF,0xAA}, 
  {0x00,0xFF,0xD2},
  {0x00,0xFF,0xFF}, 
#endif
};
/*
	   <
4	v  	^
	v	^
	v	^
	v	^
	   > 
*/
const u8 rgb_flamecolor_speed[] =
{30,24,20,14,10,6,3,1};

void user_rgb_flame_round(void)
{
static u8 pos=0,i,j,k,l,m,cnt;
RGB_DEV_COLOUR colour;
static u8 steptemp;

// if(++cnt<10)	return;else	cnt=0;

steptemp=rgb_flamecolor_speed[bEnergyStep];

if(++cnt<steptemp)	return;else	cnt=0;


user_rgb_clear_colour();

l=_this_rgb->number;

//pos=rgbpostest;
for(i=pos;i<pos+6;i++)
{
	j=i-pos; 	
	if(i>l-1) m=i-l;else m=i;		
	colour.g=rgb_flame_color[j][0];
	colour.r=rgb_flame_color[j][1];
	colour.b=rgb_flame_color[j][2];		
	user_rgb_colour_only_set(&colour,m);		
}

if(pos<l-1)	pos++;else	pos=0;
}
//-------------------------------------------------
//------绿黄红紫蓝  快速从上到下变色--
// grb
#if (USER_RGB_NUMBER==28)
const u8 rgb_flame_color_down[14][3] =
{
  {0x00,0x00,0xb0}, 
  {0x2c*2,0x00,0xb0},
  {0xb0,0x00,0xb0},
  
  {0xb0,0x00,0x2c*2},
  {0xb0,0x00,0x00},
  
  {0xb0,0x2c*2,0x00},
  {0xb0,0x2c*3,0x00},
  {0xb0,0xb0,0x00},
  
  {0x2c*3,0xb0,0x00},
  {0x2c*2,0xb0,0x00},
  {0x00,0xb0,0x00},
  
  {0x00,0xb0,0x2c*2},
  {0x00,0xb0,0x2c*3},
  {0x00,0xb0,0xb0},
};
#elif  (USER_RGB_NUMBER==32)

const u8 rgb_flame_color_down[16][3] =
{
  {0x00,0x00,0xb0}, 
  {0x2c*2,0x00,0xb0},
  {0x2c*3,0x00,0xb0},
  {0xb0,0x00,0xb0},  
  {0xb0,0x00,0x2c*3},
  {0xb0,0x00,0x2c*2},
  {0xb0,0x00,0x00},  
  {0xb0,0x2c*2,0x00},
  {0xb0,0x2c*3,0x00},
  {0xb0,0xb0,0x00},  
  {0x2c*3,0xb0,0x00},
  {0x2c*2,0xb0,0x00},
  {0x00,0xb0,0x00},  
  {0x00,0xb0,0x2c*2},
  {0x00,0xb0,0x2c*3},
  {0x00,0xb0,0xb0},
};

#else // 43
const u8 rgb_flame_color_down[21][3] =
{
  {0x00,0x00,0xb0}, 
  {0x2c*1,0x00,0xb0},
  {0x2c*2,0x00,0xb0},
  {0x2c*3,0x00,0xb0},
  {0xb0,0x00,0xb0},
  
  {0xb0,0x00,0x2c*3},
  {0xb0,0x00,0x2c*2},
  {0xb0,0x00,0x2c*1},
  {0xb0,0x00,0x00},
  
  {0xb0,0x2c*1,0x00},
  {0xb0,0x2c*2,0x00},
  {0xb0,0x2c*3,0x00},
  {0xb0,0xb0,0x00},
  
  {0x2c*3,0xb0,0x00},
  {0x2c*2,0xb0,0x00},
  {0x2c*1,0xb0,0x00},
  {0x00,0xb0,0x00},
  
  {0x00,0xb0,0x2c*1},
  {0x00,0xb0,0x2c*2},
  {0x00,0xb0,0x2c*3},
  {0x00,0xb0,0xb0},
};
#endif
/*
16	v	v
	v	v
	v	v
	v	v
	  > <
*/
const u8 rgb_colordown_speed[] =
{15,12,10,8,6,4,2,1};

void user_rgb_color_down(void)
{
static u8 pos=0,i,j,k,l,cnt;
RGB_DEV_COLOUR colour;
static u8 steptemp;

steptemp=rgb_colordown_speed[bEnergyStep];

if(++cnt<steptemp)	return;else	cnt=0;

l=(_this_rgb->number-(_this_rgb->number%2))/2; // 21

user_rgb_clear_colour();

for(i=0;i<l;i++) // 0-20
{
	//发送1 到_this_rgb->number/2     buf0-buf20 led1-led21
		{
			if(i<pos) j=(l)-(pos-i); else j=i-pos; //
			colour.g=rgb_flame_color_down[j][0];
			colour.r=rgb_flame_color_down[j][1];
			colour.b=rgb_flame_color_down[j][2];		
			user_rgb_colour_only_set(&colour,i);			
		}	

	//同步发送(_this_rgb->number/2 +_this_rgb->number%2+1)到_this_rgb->number 24-43
		{
			if(i<pos) j=(l)-(pos-i); else j=i-pos; 
			k=_this_rgb->number-i-1;
			colour.g=rgb_flame_color_down[j][0];
			colour.r=rgb_flame_color_down[j][1];
			colour.b=rgb_flame_color_down[j][2];
			user_rgb_colour_only_set(&colour,k);			
		}	
	//同步发送(_this_rgb->number/2 +_this_rgb->number%2)  
	// 总数奇数个灯点亮中间单灯偶数不点亮	
	if(_this_rgb->number%2==1)
		{
		   if(i==(l-1))
			{
				if(i<pos) j=(l)-(pos-i); else j=i-pos; //
				k=l;
				colour.g=200;
				colour.r=200;
				colour.b=200;
				user_rgb_colour_only_set(&colour,k);			
			}	
		}
}

if(pos<l-1)	pos++;else	pos=0;
}

//-----------------------------------
//------火焰双边从上到下循环----
// grb
const u8 rgb_flame_down[7][3] =
{
#if 1
  {0x00,0xa0,0x00}, 
  {0x00,0xb0,0x00}, 
  {0x10,0xe0,0x00}, 
  {0x20,0xe0,0x00}, 
  {0x10,0xe0,0x00}, 
  {0x00,0xb0,0x00}, 
  {0x00,0xa0,0x00}, 
#else
  {0x00,0xcf,0x00}, 
  {0x00,0xe0,0x00}, 
  {0x00,0xf0,0x00}, 
  {0x20,0xff,0x00}, 
  {0x00,0xf0,0x00}, 
  {0x00,0xe0,0x00}, 
  {0x00,0xc0,0x00}, 
  
  {0x00,0x90,0x00}, 
  {0x00,0xb0,0x00},
  {0x00,0xe0,0x00},
  {0x10,0xe0,0x00}, 
  {0x30,0xe0,0x00},
  {0x60,0xe0,0x00}, 
  {0x90,0xe0,0x00}, 
#endif  
 
};
/*
7	v	v
	v	v
	v	v
	v	v
	  > <
*/
const u8 rgb_flamedown_speed[] =
{15,12,9,7,5,3,2,1};

void user_rgb_flame_down(void)
{
static u8 pos=0,i,j,k,l,m,n,cnt;
RGB_DEV_COLOUR colour;

static u8 steptemp;

//  if(++cnt<20)	return;else	cnt=0;

steptemp=rgb_flamedown_speed[bEnergyStep];


if(++cnt<steptemp)	return;else	cnt=0;

l=(_this_rgb->number-(_this_rgb->number%2))/2; // 21

user_rgb_clear_colour();

//pos=rgbpostest;

for(i=pos;i<pos+7;i++)
{
	j=i-pos; 		
	if(i>l-1) m=i-l;else m=i;			
	colour.g=rgb_flame_down[j][0];
	colour.r=rgb_flame_down[j][1];
	colour.b=rgb_flame_down[j][2];		
	user_rgb_colour_only_set(&colour,m);			

	j=i-pos; 		
	if(i>l-1) m=i-l;else m=i;	
	n=_this_rgb->number-m-1;
	colour.g=rgb_flame_down[j][0];
	colour.r=rgb_flame_down[j][1];
	colour.b=rgb_flame_down[j][2];		
	user_rgb_colour_only_set(&colour,n);		
	
	//同步发送(_this_rgb->number/2 +_this_rgb->number%2)  23		
	// 总数奇数个灯点亮中间单灯偶数不点亮	
	if(_this_rgb->number%2==1)
		{
		   if(i>(l-1))
			{
				j=(l-pos)%7;
				colour.g=rgb_flame_down[j][0];
				colour.r=rgb_flame_down[j][1];
				colour.b=rgb_flame_down[j][2];		
				user_rgb_colour_only_set(&colour,l);			
			}
		}
}

if(pos<l-1)
	pos++;
else
	pos=0;
}
//-----------------------------------------------------
//-------- 双边从上到下4级跑马---6  66 4 43-------
// grb

#if (USER_RGB_NUMBER==28)
const u8 rgb_flame_color_rance[14][3] =
{
  {0x00,0x50,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},   
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*7},  
  {0x00,0xb0,0xb0},  
  {0x16*2,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},  
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
};
#elif  (USER_RGB_NUMBER==32)
const u8 rgb_flame_color_rance[16][3] =
{
#if 1 //  橙色->红色_>->紫  16级过渡

  {0,200,0}, 
  {10,200,0}, 
  {0,210,5}, 
  {0,220,10}, 
  
  {0,230,15}, 
  {0,240,20},   
  {0,250,25}, 
  {0,250,35}, 

  {0,250,35}, 
  {0,250,45}, 
  {0,250,50}, 
  {5,250,60}, 

  {10,250,70}, 
  {20,250,80}, 
  {30,250,100}, 
  {40,250,100}, 
  
#else
  {0x00,0x50,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},   
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*3},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*7},  
  {0x00,0xb0,0xb0},  
  {0x16*2,0xb0,0x00},
  {0x16*3,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},  
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
#endif  
};
#else
const u8 rgb_flame_color_rance[21][3] =
{
  {0x00,0x50,0x00},
  {0x00,0x70,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},  
  {0x00,0xb0,0x16*1},
  
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*3},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*6},
  {0x00,0xb0,0x16*7},
  
  {0x00,0xb0,0xb0},  
  {0x16*1,0xb0,0x00},
  {0x16*2,0xb0,0x00},
  {0x16*3,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},
  
  {0x16*6,0xb0,0x00},
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
};

#endif
/*
4	v	v
4	v	v
4	v	v
4	v	v
	  > <
*/

//3344
#define RANCE_STEP 3 
#define RANCE_STEP1 4
const u8 rgb_flamerance_speed[] =
{50,40,30,20,15,10,5,3};
void user_rgb_flame_rance(void)
{
static u8 pos=0,ii,i,k,l,m,cnt;
RGB_DEV_COLOUR colour;

static u8 steptemp;

steptemp=rgb_flamerance_speed[bEnergyStep];

//steptemp=100;

if(++cnt<steptemp)	return;else	cnt=0;

l=(_this_rgb->number-(_this_rgb->number%2))/2; // 21

user_rgb_clear_colour();
// ii=rgbpostest;

	if(ii==4) // 点亮全部火焰
	{
		pos=0;m=l;		
	}
	else if(ii<4)
	{
		pos=ii*4;	m=pos+4; // 4灯 4444
	}
		
	for(i=pos;i<m;i++) // 0-20
	{
		//发送1 到_this_rgb->number/2     buf0-buf20 led1-led21
			{
				colour.g=rgb_flame_color_rance[i][0];
				colour.r=rgb_flame_color_rance[i][1];
				colour.b=rgb_flame_color_rance[i][2];		
				user_rgb_colour_only_set(&colour,i);			
			}	
		//同步发送(_this_rgb->number/2 +_this_rgb->number%2+1)到_this_rgb->number 24-43
			{
				k=_this_rgb->number-i-1;
				colour.g=rgb_flame_color_rance[i][0];
				colour.r=rgb_flame_color_rance[i][1];
				colour.b=rgb_flame_color_rance[i][2];
				user_rgb_colour_only_set(&colour,k);			
			}	
	// 总数奇数个灯点亮中间单灯偶数不点亮	
	if(_this_rgb->number%2==1)
		{
		//同步发送(_this_rgb->number/2 +_this_rgb->number%2)  23				
		   if(i==(l-1))
			{
				colour.g=rgb_flame_color_rance[i][0];
				colour.r=rgb_flame_color_rance[i][1];
				colour.b=rgb_flame_color_rance[i][2];
				user_rgb_colour_only_set(&colour,l);			
			}	
		}
	}		
if(ii<4)	ii++; else ii=0;
}
//-----------------------------------------------------
//-------- 火焰灯呼吸	=--------------------------
// grb
#if (USER_RGB_NUMBER==28)
const u8 rgb_flame_breath[14][3] =
{
  {0x00,0x50,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},   
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*7},  
  {0x00,0xb0,0xb0},  
  {0x16*2,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},  
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
};
#elif  (USER_RGB_NUMBER==32)
const u8 rgb_flame_breath[16][3] =
{
#if 1

  {0,135,0}, 
  {10,145,0}, 
  {0,155,5}, 
  {0,165,10}, 
  
  {0,176,15}, 
  {0,176,20},   
  {0,176,25}, 
  {0,176,35}, 

  {0,176,35}, 
  {0,176,45}, 
  {0,176,50}, 
  {5,176,60}, 

  {10,176,70}, 
  {20,176,80}, 
  {30,176,100}, 
  {40,176,100}, 
  
#else
  {0x00,0x50,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},   
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*3},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*7},  
  {0x00,0xb0,0xb0},  
  {0x16*2,0xb0,0x00},
  {0x16*3,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},  
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
#endif

};
#else
const u8 rgb_flame_breath[21][3] =
{
  {0x00,0x50,0x00},
  {0x00,0x70,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},  
  {0x00,0xb0,0x16*1},
  
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*3},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*6},
  {0x00,0xb0,0x16*7},
  
  {0x00,0xb0,0xb0},  
  {0x16*1,0xb0,0x00},
  {0x16*2,0xb0,0x00},
  {0x16*3,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},
  
  {0x16*6,0xb0,0x00},
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
};
#endif

#define RGB_MAX_VAL  0xb0   // RGB 取最大数值
#define BREATH_SPEED_STEP  4 // 0Xb0 分几级

#define BREATH_STEP  (RGB_MAX_VAL/BREATH_SPEED_STEP)*2
#define BREATH_STEP_HALF  BREATH_STEP/2
const u8 rgb_flamebreath_speed[] =
{7,6,5,4,3,2,1,1};
void user_rgb_flame_breath(void)  // 注意除数不能为0 否则会死机
{
static u8 pos=0,i,k,l,m,j,cnt;
static u16 bStep=0;
RGB_DEV_COLOUR colour;

static u8 steptemp;

//   if(++cnt<2)	return;else	cnt=0;

steptemp=rgb_flamebreath_speed[bEnergyStep];

if(++cnt<steptemp)	return;else	cnt=0;

l=(_this_rgb->number-(_this_rgb->number%2))/2; // 16
user_rgb_clear_colour();
//	 bStep=rgbpostest;
	pos=0;
	m=l;		
	for(i=pos;i<m;i++) // 0-15
	{
	
		//发送1 到_this_rgb->number/2     buf0-buf15 led1-led15
		{
			colour.g=rgb_flame_breath[i][0];  
			colour.r=rgb_flame_breath[i][1];
			colour.b=rgb_flame_breath[i][2];																
		
			 if(bStep<BREATH_STEP_HALF) //减小 led变暗
				{
					if(colour.g>bStep)colour.g-=bStep;else colour.g=0;
					if(colour.r>(bStep*BREATH_SPEED_STEP))colour.r-=bStep*BREATH_SPEED_STEP; else colour.r=0;
					if(colour.b>bStep)colour.b-=bStep;else colour.b=0;	

				}
			else if(bStep==BREATH_STEP_HALF) // led全灭
				{
					colour.g=0;colour.r=0;colour.b=0;					
				}
			else
				{		
					if(bStep>(BREATH_STEP-colour.g))
					    colour.g=bStep-(BREATH_STEP-colour.g);
					else
						colour.g=0;   
					
					if(bStep>(BREATH_STEP-colour.r/BREATH_SPEED_STEP))
					    colour.r=(bStep-(BREATH_STEP-colour.r/BREATH_SPEED_STEP))*BREATH_SPEED_STEP;
					else
						colour.r=0;   
					
					if(bStep>(BREATH_STEP-colour.b))
					    colour.b=bStep-(BREATH_STEP-colour.b);
					else
						colour.b=0;    	   					
						
				}					
			user_rgb_colour_only_set(&colour,i);			
		}
		//同步发送(_this_rgb->number/2 +_this_rgb->number%2+1)到_this_rgb->number 16-31
		{
			colour.g=rgb_flame_breath[i][0];  
			colour.r=rgb_flame_breath[i][1];
			colour.b=rgb_flame_breath[i][2];																
		
			k=_this_rgb->number-i-1;
			
			 if(bStep<BREATH_STEP_HALF) //减小 led变暗
				{
					if(colour.g>bStep)colour.g-=bStep;else colour.g=0;
					if(colour.r>(bStep*BREATH_SPEED_STEP))colour.r-=bStep*BREATH_SPEED_STEP; else colour.r=0;
					if(colour.b>bStep)colour.b-=bStep;else colour.b=0;	
				}
			else if(bStep==BREATH_STEP_HALF) // led全灭
				{
					colour.g=0;colour.r=0;colour.b=0;					
				}
			else
				{	// 101--200							    
					if(bStep>(BREATH_STEP-colour.g))
					    colour.g=bStep-(BREATH_STEP-colour.g);
					else
						colour.g=0;   
					
					if(bStep>(BREATH_STEP-colour.r/BREATH_SPEED_STEP))
					    colour.r=(bStep-(BREATH_STEP-colour.r/BREATH_SPEED_STEP))*BREATH_SPEED_STEP;
					else
						colour.r=0;    
					
					if(bStep>(BREATH_STEP-colour.b))
					    colour.b=bStep-(BREATH_STEP-colour.b);
					else
						colour.b=0;    	 					
						
				}	
			user_rgb_colour_only_set(&colour,k);			
		}		
		
	}
				
if(bStep<BREATH_STEP-1)	bStep++;else	bStep=0;
}

#if 0
//-------------------------------------------------
//----------三团火焰反旋转-----------------
// grb
const u8 rgb_3flame_reround[4][3] =
{
  {0x00,0x80,0x00}, 
  {0x00,0xb0,0x00},
  {0x4f,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
};
/*
	   <
4	v  	^	4
	v	^
4	v	^	4
	v	^
	   > 
*/
const u8 rgb_3flamereround_speed[] =
{25,20,15,10,6,4,2,1};
void user_rgb_3flame_reround(void)
{
static u8 pos=0,i,j,k,l,m,cnt;
RGB_DEV_COLOUR colour;


static u8 steptemp;

//   if(++cnt<50)	return;else	cnt=0;

steptemp=rgb_3flamereround_speed[bEnergyStep];

if(++cnt<steptemp)	return;else	cnt=0;

user_rgb_clear_colour();

l=_this_rgb->number;

// pos=rgbpostest;
for(i=pos;i<pos+4;i++)
{
	 {
	 	j=i-pos; 	
	 	if(i>l-1) m=i-l;else m=i;		
	 	colour.g=rgb_3flame_reround[j][0];
	 	colour.r=rgb_3flame_reround[j][1];
	 	colour.b=rgb_3flame_reround[j][2];		
	 	user_rgb_colour_only_set(&colour,m);	
	 }

	 {
	 	j=i-pos; 	
		k=i+11;
	 	if(k>l-1) m=k-l;else m=k;		
	 	colour.g=rgb_3flame_reround[j][0];
	 	colour.r=rgb_3flame_reround[j][1];
	 	colour.b=rgb_3flame_reround[j][2];		
	 	user_rgb_colour_only_set(&colour,m);	
	 }

	 {
	 	j=i-pos; 	
		k=i+22;
	 	if(k>l-1) m=k-l;else m=k;		
	 	colour.g=rgb_3flame_reround[j][0];
	 	colour.r=rgb_3flame_reround[j][1];
	 	colour.b=rgb_3flame_reround[j][2];		
	 	user_rgb_colour_only_set(&colour,m);	
	 }
	 #if 0
	 {
	 	j=i-pos; 	
		k=i+24;
	 	if(k>l-1) m=k-l;else m=k;		
	 	colour.g=rgb_3flame_reround[j][0];
	 	colour.r=rgb_3flame_reround[j][1];
	 	colour.b=rgb_3flame_reround[j][2];		
	 	user_rgb_colour_only_set(&colour,m);	
	 }	 
	 #endif
}

if(pos<l-1)	pos++;else	pos=0;
}
#endif

//-------------------------------------------------
//---------------//火焰左右正3次反3次接触消失--
// grb
const u8 rgb_flame_dround[8][3] =
{
  {0x00,0xa0,0x00}, 
  {0x00,0xb0,0x00}, 
  {0x10,0xb0,0x00}, 
  {0x20,0xe0,0x00}, 
  {0x30,0xe0,0x00}, 
  {0x20,0xe0,0x00}, 
  {0x10,0xe0,0x00}, 
  {0x00,0xb0,0x00}, 
};
/*
双火焰转到左边碰触消失  循环3次
	  <<
4	v  	^	4
	v	^
4	^	v	4
	^	v
	  <<
双火焰转到右边碰触消失	  循环3次
	  >>
4	^  	v	4
	^	v
4	v	^	4
	v	^
	  >>
	  
*/
const u8 rgb_flamedround_speed[] =
{25,20,15,10,6,4,2,1};
#define START_UPOS 7
#define START_DPOS 8
void user_rgb_flame_dround(void)
{
static u8 pos=0,i,j,k,l,cnt;
RGB_DEV_COLOUR colour;
RGB_DEV_INFO *rgb = _this_rgb;//(RGB_INFO *)priv;

static u8 t_pos; // led buf true postion
static u8 steptemp;

//   if(++cnt<50)	return;else	cnt=0;

steptemp=rgb_flamedround_speed[bEnergyStep];

if(++cnt<steptemp)	return;else	cnt=0;

user_rgb_clear_colour();

//l=_this_rgb->number;

// pos=rgbpostest;


	//连续点亮8颗灯
	for(i=pos;i<pos+8;i++)
	{
		j=i-pos; 		
		if(k<5) // 右转
		t_pos=i;	
		else // 左转
		t_pos=32-i-1;			
		colour.g=rgb_flame_dround[j][0];
		colour.r=rgb_flame_dround[j][1];
		colour.b=rgb_flame_dround[j][2];		
		user_rgb_colour_only_set(&colour,t_pos);			
	}

	//正反赋一样的值
	for(i=0;i<8;i++)
		{
		       rgb->rgb_buff[i].g=rgb->rgb_buff[15-i].g;
		       rgb->rgb_buff[i].r=rgb->rgb_buff[15-i].r;
		       rgb->rgb_buff[i].b=rgb->rgb_buff[15-i].b;
		}

	//正反赋一样的值
	for(i=16;i<24;i++)
		{
		       rgb->rgb_buff[32-i+16].g=rgb->rgb_buff[i].g;
		       rgb->rgb_buff[32-i+16].r=rgb->rgb_buff[i].r;
		       rgb->rgb_buff[32-i+16].b=rgb->rgb_buff[i].b;
		}


		
	if(pos<24-1) 
		pos++;
	else	
	{
		pos=0;
		if(k<10) k++; else k=0;
	}

}


//---------------------------------------------------------------
//----双边跟随音乐跳动从下三分之一出到两端------
//-------21LED分2段上14下7 ---能量检测分七级-----------
// grb  
#if (USER_RGB_NUMBER==28)
const u8 rgb_flame_check[14][3] =
{
  {0x00,0x50,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},   
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*7},  
  {0x00,0xb0,0xb0},  
  {0x16*2,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},  
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
};
#elif  (USER_RGB_NUMBER==32)
const u8 rgb_flame_check[16][3] =
{
#if 1 //  橙色->红色_>->紫  16级过渡

  {0,200,0}, 
  {10,200,0}, 
  {0,210,5}, 
  {0,220,10}, 
  
  {0,230,15}, 
  {0,240,20},   
  {0,250,25}, 
  {0,250,35}, 

  {0,250,35}, 
  {0,250,45}, 
  {0,250,50}, 
  {5,250,60}, 

  {10,250,70}, 
  {20,250,80}, 
  {30,250,100}, 
  {40,250,100}, 
  
#else

  {0x00,0x50,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},   
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*3},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*7},  
  {0x00,0xb0,0xb0},  
  {0x16*2,0xb0,0x00},
  {0x16*3,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},  
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 

#endif

};
#else
const u8 rgb_flame_check[21][3] =
{
  {0x00,0x50,0x00},
  {0x00,0x70,0x00},
  {0x00,0x90,0x00},
  {0x00,0xb0,0x00},
  {0x00,0xb0,0x00},  
  {0x00,0xb0,0x16*1},
  
  {0x00,0xb0,0x16*2},
  {0x00,0xb0,0x16*3},
  {0x00,0xb0,0x16*4},  
  {0x00,0xb0,0x16*5},
  {0x00,0xb0,0x16*6},
  {0x00,0xb0,0x16*7},
  
  {0x00,0xb0,0xb0},  
  {0x16*1,0xb0,0x00},
  {0x16*2,0xb0,0x00},
  {0x16*3,0xb0,0x00},
  {0x16*4,0xb0,0x00},  
  {0x16*5,0xb0,0x00},
  
  {0x16*6,0xb0,0x00},
  {0x16*7,0xb0,0x00},
  {0xb0,0xb0,0x00}, 
};
#endif
/*
5	^	^
5	^	^
	|	|
6	v	v
*/
#define VOL_STEP_UP  5  // 55  6
#define VOL_STEP_DN  6  // 55  6
#define VOL_STEP	  VOL_STEP_DN//VOL_STEP_UP // 取大值后面在处理小值
#define VOL_DN_POS  10
#define VOL_UP_POS  VOL_DN_POS-1 

// 分级最大能量
#define ENERGY_VOL10  100
#define ENERGY_VOL15  350 
#define ENERGY_VOL20  550
#define ENERGY_VOL25  1050
#define ENERGY_VOLMAX  6000 
// 分级补充系数
#define VOL10_RATIO  2.1
#define VOL15_RATIO  4 
#define VOL20_RATIO  5
#define VOL25_RATIO  7.5
#define VOL28_RATIO  33 
#define VOLMAX_RATIO  38 

AT(.volatile_ram_code)
void user_dac_energy_scan(void)
{
static u16 bVolRatio=0; //当前音量能量分级系数
static u32 dac_energy_buf=0;
static u16 dac_energy_value=0;
static u8 eng_one_times=0;
static char left_up_i,left_up_j;
static u16 dac_energy_timer=0;


if(light_mode_mark == RGB_MODE_1 || light_mode_mark == RGB_OFF)
	return;
	
if(use_ctl.led_vol==0) ////0级音量2秒自动进入呼吸模式
{
	bEnergyStep=0;
	return;			
}

//无能量2秒自动进入呼吸模式
    dac_energy_value=audio_dac_energy_get();

if(dac_energy_value>6000)
	dac_energy_value=dac_energy_value/2;
	
if(dac_energy_value==0) 
{ 
    dac_energy_timer++;
	if(dac_energy_timer>=100) // 2m
	{
		dac_energy_timer=101;
		bEnergyStep=0;
		return;			
	}	
}
else
	dac_energy_timer=0;

//----------能量计算-------

   if(eng_one_times<4) 
   {
	   eng_one_times++;
        dac_energy_buf+=audio_dac_energy_get();
	 return;
   }
   else
   {
	eng_one_times=0;
	dac_energy_value=dac_energy_buf/4;
	dac_energy_buf=0;
   } 

		 if(use_ctl.led_vol<10) 
	 	{
	 	  if(dac_energy_value>300)
		  	dac_energy_value=dac_energy_value/3.5;
	 	else if(dac_energy_value>200)
		  	dac_energy_value=dac_energy_value/2.5;		  
	 	else if(dac_energy_value>100)
		  	dac_energy_value=dac_energy_value/2.2;		  
	 		bVolRatio=(use_ctl.led_vol*VOL10_RATIO);
	 	}
		else if(use_ctl.led_vol<15) bVolRatio=(use_ctl.led_vol*VOL15_RATIO);
		else if(use_ctl.led_vol<20) bVolRatio=(use_ctl.led_vol*VOL20_RATIO);
		else if(use_ctl.led_vol<25) bVolRatio=(use_ctl.led_vol*VOL25_RATIO);
		else if(use_ctl.led_vol<28) bVolRatio=(use_ctl.led_vol*VOL28_RATIO);
		else bVolRatio=(use_ctl.led_vol*VOLMAX_RATIO);
		
		bEnergyStep=dac_energy_value/bVolRatio;  //当前能量级数
	 	if(bEnergyStep>VOL_STEP) bEnergyStep=VOL_STEP;	  

//----------能量计算-------------------
}


const u16 energyval[] ={50,88,100,350,550,1050,6000};
const u16 energyratio[] ={2,2,2.1,4,5,7.5,38};
void user_rgb_flame_check(void)  // 注意除数不能为0 否则会死机
{
static u8 pos=0,k,l,cnt;
RGB_DEV_COLOUR colour;

static u16 bVolRatio=0; //当前音量能量分级系数

static u32 dac_energy_buf=0;
static u16 dac_energy_value=0;
static u8 eng_one_times=0;
static char left_up_i,left_up_j;
static u16 dac_energy_timer=0;
static u8 bEnergyStepOld;

	
if(use_ctl.led_vol==0) ////0级音量2秒自动进入呼吸模式
{
	bEnergyStep=0;
	user_rgb_flame_rance();
	return;			
}

//无能量2秒自动进入呼吸模式
    dac_energy_value=audio_dac_energy_get();

if(dac_energy_value>energyval[6])
	dac_energy_value=dac_energy_value/2;

if(dac_energy_value>energyval[6])
	dac_energy_value=dac_energy_value/2;

if(dac_energy_value==0) 
{ 
    dac_energy_timer++;
	if(dac_energy_timer>=100) // 2m
	{
		dac_energy_timer=101;
		bEnergyStep=0;
		user_rgb_flame_rance();
		return;			
	}	
}
else
	dac_energy_timer=0;
//无能量2秒自动进入呼吸模式

//----------能量计算-------

   if(eng_one_times<4) 
   {
	   eng_one_times++;
        dac_energy_buf+=audio_dac_energy_get();
	 return;
   }
   else
   {
	eng_one_times=0;
	dac_energy_value=dac_energy_buf/4;
	dac_energy_buf=0;
   } 
   
    //printf("\n --111---dac_energy_value== %d ",dac_energy_value);


	{
			
	 	  if(dac_energy_value>energyval[use_ctl.led_vol/5]*3)
		  	dac_energy_value=dac_energy_value/3.2;
	 	  if(dac_energy_value>energyval[use_ctl.led_vol/5]*2)
		  	dac_energy_value=dac_energy_value/2.2;
	 	 if(dac_energy_value>energyval[use_ctl.led_vol/5]*2)
		  	dac_energy_value=dac_energy_value/2.2;		  
	 	 if(dac_energy_value>energyval[use_ctl.led_vol/5])
		  	dac_energy_value=dac_energy_value/2;		  

		if(use_ctl.led_vol<5)	
	 		bVolRatio=14;
		else
	 		bVolRatio=(use_ctl.led_vol*energyratio[use_ctl.led_vol/5]);
	
			
		bEnergyStep=dac_energy_value/bVolRatio;  //当前能量级数
	 	if(bEnergyStep>VOL_STEP) bEnergyStep=VOL_STEP;	  

	//	 	if(bEnergyStep==0) bEnergyStep=((dac_energy_value&0x07));


	
	}
 //      printf("\n --111---bEnergyStep== %d ",bEnergyStep);


	
//----------能量计算-------------------


//扫描速度控制
// if(++cnt<3)	return;else	cnt=0;


l=(_this_rgb->number-(_this_rgb->number%2))/2; // 14
   
user_rgb_clear_colour();

//bEnergyStep=rgbpostest;


if(bEnergyStep>0)
{
	// 发送 LED1-LED21数据
	{
		// 处理分级5
	 	if(bEnergyStep>VOL_STEP_UP) bEnergyStep=VOL_STEP_UP;	   	
		
		// send led10--led1
		pos=VOL_UP_POS;	// led10		
		left_up_j=pos+1-bEnergyStep*2;			
		for(left_up_i=pos;left_up_i>=left_up_j;left_up_i--) 
		{
			colour.g=rgb_flame_check[left_up_i][0];  
			colour.r=rgb_flame_check[left_up_i][1];
			colour.b=rgb_flame_check[left_up_i][2];																						
			user_rgb_colour_only_set(&colour,left_up_i);			
		}
		
	 	if(bEnergyStep==VOL_STEP_UP) bEnergyStep=VOL_STEP_DN;	   	
		
		// 处理分级6
	 	if(bEnergyStep>VOL_STEP_DN) bEnergyStep=VOL_STEP_DN;	   	
		// send led11--led16
		pos=VOL_DN_POS;	// LED11		
		left_up_j=pos+bEnergyStep;			
		for(left_up_i=pos;left_up_i<left_up_j;left_up_i++) 
		{
			colour.g=rgb_flame_check[left_up_i][0];  
			colour.r=rgb_flame_check[left_up_i][1];
			colour.b=rgb_flame_check[left_up_i][2];																
						
			user_rgb_colour_only_set(&colour,left_up_i);			
		}	
	 	if(bEnergyStep==VOL_STEP_DN) bEnergyStep=VOL_STEP_UP;	   	

	}

	// 发送 LED23-LED43数据
	{		
		// 处理 分级5
	 	if(bEnergyStep>VOL_STEP_UP) bEnergyStep=VOL_STEP_UP;	   	
		
		//send   led19--led28
		pos=VOL_UP_POS;	// led14		
		left_up_j=pos+1-bEnergyStep*2;			
		for(left_up_i=pos;left_up_i>=left_up_j;left_up_i--) 
		{
			k=_this_rgb->number-left_up_i-1;		
			colour.g=rgb_flame_check[left_up_i][0];  
			colour.r=rgb_flame_check[left_up_i][1];
			colour.b=rgb_flame_check[left_up_i][2];
			user_rgb_colour_only_set(&colour,k);			
		}

	 	if(bEnergyStep==VOL_STEP_UP) bEnergyStep=VOL_STEP_DN;	   	

		// 处理 分级6
	 	if(bEnergyStep>VOL_STEP_DN) bEnergyStep=VOL_STEP_DN;	   	
	
		//send  led18--led15
		pos=VOL_DN_POS;	// LED18
		left_up_j=pos+bEnergyStep;			
		for(left_up_i=pos;left_up_i<left_up_j;left_up_i++) 
		{
			k=_this_rgb->number-left_up_i-1;
			colour.g=rgb_flame_check[left_up_i][0];  
			colour.r=rgb_flame_check[left_up_i][1];
			colour.b=rgb_flame_check[left_up_i][2];
			user_rgb_colour_only_set(&colour,k);
		}	
	 	if(bEnergyStep==VOL_STEP_DN) bEnergyStep=VOL_STEP_UP;	   	
		
	}

		if(_this_rgb->number%2==1)
			{
				// LED22
				if(bEnergyStep==VOL_STEP)	
				{
					colour.g=rgb_flame_check[l-1][0];  
					colour.r=rgb_flame_check[l-1][1];
					colour.b=rgb_flame_check[l-1][2];																				
					user_rgb_colour_only_set(&colour,l);			
				}		
			}
}	
}

//-----------------------------------
//-----------------------------------
//-----------------------------------
//-----------------------------------
//-----------------------------------
//-----------------------------------
AT(.volatile_ram_code)
void user_rgb_scan_mode(void){

	printf("user_rgb_scan_mode");

    if(!_this_->info->init_flag){
        g_printf("*");
        return;
    }
	
    if((app_get_curr_task() == APP_FM_TASK))
    	{    	
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
//	light_mode_mark = use_ctl.extern_led_rgb_flag;
	light_mode_mark = RGB_MODE_1;

	static u16 change_cnt=499;
/*
	if(audio_dac_energy_get()){
		light_mode_mark = RGB_MODE_1;
		change_cnt = 0;
	}else{
		if(change_cnt++>500){
			change_cnt = 0;
			light_mode_mark = RGB_MODE_2;
		}
	}
*/
    switch (light_mode_mark){
	case RGB_OFF:
     	user_rgb_clear_colour();
		user_rgb_send_cnt(1);
    break;
	case RGB_MODE_1:// 火焰反旋转
     	user_rgb_flame_round();
		user_rgb_send_cnt(1);
    break;		
	case RGB_MODE_2:// 彩色双边多彩下旋转
		user_rgb_color_down();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_3: //火焰双边下旋转
		user_rgb_flame_down();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_4:// 火焰4级跑马
		user_rgb_flame_rance();
		user_rgb_send_cnt(1);
	break;
	case RGB_MODE_5:// 双边跟随音乐跳动从下到上
		user_rgb_flame_check();//4
		user_rgb_send_cnt(1);
	break;			
	case RGB_MODE_6://火焰左右正反双旋转
		user_rgb_flame_dround();
		user_rgb_send_cnt(1);
	break;	
	case RGB_MODE_7:// 火焰呼吸
		user_rgb_flame_breath();
		user_rgb_send_cnt(1);
	break;

	//	user_rgb_3flame_reround();  // 三火焰反旋转

    default:
        break;
    }
}
//初始化
void user_rgb_fun_init(void){
    if(user_rgb_info.init_flag){
        return;
    }


    //rgb spi init
    user_rgb_dev_init(&user_rgb_info);

    //发送一次spi数据
   // user_rgb_send_cnt(1);

}

//退出
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

