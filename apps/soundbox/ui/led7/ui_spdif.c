#include "system/includes.h"
#include "ui/ui_api.h"

#if TCFG_APP_SPDIF_EN
#include "user_cfg.h"

#if (TCFG_UI_ENABLE&&(CONFIG_UI_STYLE == STYLE_JL_LED7))


static void led7_show_spdif(void *hd)   //只有切换模式才执行
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
   
  
   
    if(use_ctl.bSpdifMode==SPDIF_COAX)   
   	{
        dis->show_string((u8 *)" COA");
         printf(">>>>enter_COA_UI");
   	}
    else if(use_ctl.bSpdifMode==SPDIF_OPT)  
 	{
 		  dis->show_string((u8 *)" OPT");		
           printf(">>>>enter_OPT_UI");	
   	}
    else if (use_ctl.bSpdifMode==SPDIF_HDMI)
    {
        dis->show_string((u8 *)"HD");	
    }
    dis->lock(0);
}







static void *ui_open_spdif(void *hd)
{
    ui_set_auto_reflash(500);
    return NULL;
}

static void ui_close_spdif(void *hd, void *private)
{
    LCD_API *dis = (LCD_API *)hd;
    if (!dis) {
        return ;
    }
    if (private) {
        free(private);
    }
}

static void ui_spdif_main(void *hd, void *private) 
{
    if (!hd) {
        return;
    }
 #ifdef SUPPORT_CHANGE_PANEL_FLASH_ERRO	
	if(use_ctl.bMusicExitCnt)
		return;
#endif	

        led7_show_spdif(hd);

}


static int ui_spdif_user(void *hd, void *private, u8 menu, u32 arg)
{
    int ret = true;
    LCD_API *dis = (LCD_API *)hd;
    if (!dis) {
        return false;
    }
    switch (menu) 
    {
    case MENU_SPDIF:
        led7_show_spdif(hd);
        break;
    default:
        ret = false;
        break;
    }

    return ret;

}



const struct ui_dis_api spdif_main = {
    .ui      = UI_SPDIF_MENU_MAIN,
    .open    = ui_open_spdif,
    .ui_main = ui_spdif_main,
    .ui_user = ui_spdif_user,
    .close   = ui_close_spdif,
};

#endif
#endif
