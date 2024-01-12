#ifndef _GUI_GUIDER_H
#define _GUI_GUIDER_H


#include "customs.h"
#include "lvgl.h"


#include <stdio.h>

typedef struct
{
  
	lv_obj_t *weather;
	bool weather_del;
	lv_obj_t *weather_line_1;
	lv_obj_t *weather_line_2;
	lv_obj_t *weather_line_3;
	lv_obj_t *weather_animimg_1;
	lv_obj_t *weather_label_1;
	lv_obj_t *weather_label_2;
	lv_obj_t *weather_label_3;
	lv_obj_t *weather_label_4;
	lv_obj_t *weather_bar_1;
	lv_obj_t *weather_label_5;
	lv_obj_t *weather_label_6;
	lv_obj_t *weather_img_1;
	lv_obj_t *weather_label_7;
	lv_obj_t *weather_label_8;
	lv_obj_t *weather_label_9;
	lv_obj_t *weather_label_10;
	lv_obj_t *weather_label_11;
	lv_obj_t *weather_label_12;
	lv_obj_t *weather_img_2;
	lv_obj_t *weather_img_3;
	lv_obj_t *weather_img_4;
	lv_obj_t *weather_img_5;
	lv_obj_t *weather_label_13;
	lv_obj_t *weather_label_14;
	lv_obj_t *weather_label_15;
	lv_obj_t *weather_label_16;
	lv_obj_t *weather_label_17;
	
	lv_obj_t *mika_animimg_1;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;

void setup_scr_weather(lv_ui *ui);

#include "extra/widgets/animimg/lv_animimg.h"
#if 1
LV_IMG_DECLARE(weather_animimg_1xiaoxin1);
LV_IMG_DECLARE(weather_animimg_1xiaoxin2);
LV_IMG_DECLARE(weather_animimg_1xiaoxin3);
LV_IMG_DECLARE(weather_animimg_1xiaoxin4);
LV_IMG_DECLARE(weather_animimg_1xiaoxin5);
LV_IMG_DECLARE(weather_animimg_1xiaoxin6);
LV_IMG_DECLARE(weather_animimg_1xiaoxin7);
LV_IMG_DECLARE(weather_animimg_1xiaoxin8);
#endif
LV_IMG_DECLARE(_city_alpha_34x33);
LV_IMG_DECLARE(_qing_alpha_24x24);
LV_IMG_DECLARE(_duoyun_alpha_24x24);
LV_IMG_DECLARE(_zhenyu_alpha_24x24);
LV_IMG_DECLARE(_dayu_alpha_24x24);
LV_IMG_DECLARE(_leiyu_alpha_24x24);
LV_IMG_DECLARE(_xue_alpha_24x24);
LV_IMG_DECLARE(_yin_alpha_24x24);
LV_IMG_DECLARE(_yu_alpha_24x24);
LV_IMG_DECLARE(_wumai_alpha_24x24);


#if 1
LV_IMG_DECLARE(mika1);
LV_IMG_DECLARE(mika3);
LV_IMG_DECLARE(mika5);
LV_IMG_DECLARE(mika7);
LV_IMG_DECLARE(mika9);
LV_IMG_DECLARE(mika11);
LV_IMG_DECLARE(mika13);
LV_IMG_DECLARE(mika15);
LV_IMG_DECLARE(mika17);
LV_IMG_DECLARE(mika19);
LV_IMG_DECLARE(mika21);
LV_IMG_DECLARE(mika23);
LV_IMG_DECLARE(mika25);
LV_IMG_DECLARE(mika27);
LV_IMG_DECLARE(mika29);
LV_IMG_DECLARE(mika31);
LV_IMG_DECLARE(mika33);
LV_IMG_DECLARE(mika35);
LV_IMG_DECLARE(mika37);
LV_IMG_DECLARE(mika39);
LV_IMG_DECLARE(mika41);
LV_IMG_DECLARE(mika43);
LV_IMG_DECLARE(mika45);
LV_IMG_DECLARE(mika47);
LV_IMG_DECLARE(mika49);
LV_IMG_DECLARE(mika51);
LV_IMG_DECLARE(mika53);
LV_IMG_DECLARE(mika55);
LV_IMG_DECLARE(mika57);
LV_IMG_DECLARE(mika59);
LV_IMG_DECLARE(mika61);
LV_IMG_DECLARE(mika63);
LV_IMG_DECLARE(mika65);
LV_IMG_DECLARE(mika67);
LV_IMG_DECLARE(mika69);
LV_IMG_DECLARE(mika71);
LV_IMG_DECLARE(mika73);
LV_IMG_DECLARE(mika75);
LV_IMG_DECLARE(mika77);
LV_IMG_DECLARE(mika79);
LV_IMG_DECLARE(mika81);
LV_IMG_DECLARE(mika83);
LV_IMG_DECLARE(mika85);
LV_IMG_DECLARE(mika87);
LV_IMG_DECLARE(mika89);
#endif

LV_FONT_DECLARE(lv_font_ShangShouZongYiTi_20)
LV_FONT_DECLARE(lv_font_ShangShouZongYiTi_15)
LV_FONT_DECLARE(lv_font_banana_font_50)
LV_FONT_DECLARE(lv_font_banana_font_24)

/*字体
!"#$%&'()*+,-./0123456789:;<=>?@ ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~
一二三四五六七八九十星期年月日今明后天温度空气质量晴少间多阴有平静微风和清强劲疾大烈暴狂爆飓热带中重严阵
并伴冰雹小特雷雨极端降毛细雪夹冻浮尘扬沙龙卷浓轻雾冷未知洪山区湿云
*/


#endif