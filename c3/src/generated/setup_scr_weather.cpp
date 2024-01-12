#include <TFT_eSPI.h>
#include <lvgl.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <stdio.h>
#include "gui_guider.h"
#include "customs.h"
#include "my_function.hpp"

/*刷新时间任务*/
void time_second_task(lv_timer_t* timer){
	if(!getLocalTime(&timeinfo))
    	return;
	const char* second = ((timeinfo.tm_sec < 10 ? "0" + std::to_string(timeinfo.tm_sec) : std::to_string(timeinfo.tm_sec))).c_str();  //显示到秒
	lv_label_set_text(guider_ui.weather_label_17, second);
	if(timeinfo.tm_sec == 0){
		const char* minute = ((timeinfo.tm_hour < 10 ? "0" + std::to_string(timeinfo.tm_hour) : std::to_string(timeinfo.tm_hour)) + ":" + 
					    	  (timeinfo.tm_min < 10 ? "0" + std::to_string(timeinfo.tm_min) : std::to_string(timeinfo.tm_min))       
					   		 ).c_str();  //更新时分
		lv_label_set_text(guider_ui.weather_label_16, minute);
	}
	if(update_flag == 1){
		const char* minute = ((timeinfo.tm_hour < 10 ? "0" + std::to_string(timeinfo.tm_hour) : std::to_string(timeinfo.tm_hour)) + ":" + 
					    	  (timeinfo.tm_min < 10 ? "0" + std::to_string(timeinfo.tm_min) : std::to_string(timeinfo.tm_min))       
					   		 ).c_str();  //时分
		lv_label_set_text(guider_ui.weather_label_16, minute);
		const char* today = (std::to_string(year) + "/" + 
							 (month < 10?"0"+std::to_string(month):std::to_string(month)) + "/" + 
							 (date < 10?"0"+std::to_string(date):std::to_string(date)) + "\n" + 
							 (std::string)day
							).c_str();
		lv_label_set_text(guider_ui.weather_label_1, today);
		lv_label_set_text(guider_ui.weather_label_3, city);
		lv_label_set_text(guider_ui.weather_label_4, ((String)now_temp + "度").c_str());
		lv_label_set_text(guider_ui.weather_label_2, now_weather);
		lv_label_set_text(guider_ui.weather_label_6, ((String)now_humidity + "%").c_str());
		lv_bar_set_value(guider_ui.weather_bar_1, (int32_t)now_humidity_int, LV_ANIM_OFF);
		lv_label_set_text(guider_ui.weather_label_10, ((String)day0_min_temp + "~" + (String)day0_max_temp + "度").c_str());
		lv_label_set_text(guider_ui.weather_label_11, ((String)day1_min_temp + "~" + (String)day1_max_temp + "度").c_str());
		lv_label_set_text(guider_ui.weather_label_12, ((String)day2_min_temp + "~" + (String)day2_max_temp + "度").c_str());
		lv_label_set_text(guider_ui.weather_label_13, ((String)day0_dayweather+"\n"+(String)day0_nightweather).c_str());
		lv_label_set_text(guider_ui.weather_label_14, ((String)day1_dayweather+"\n"+(String)day1_nightweather).c_str());
		lv_label_set_text(guider_ui.weather_label_15, ((String)day2_dayweather+"\n"+(String)day2_nightweather).c_str());
		update_weather_img(guider_ui.weather_img_2, now_weather);
		update_weather_img(guider_ui.weather_img_3, day0_dayweather);
		update_weather_img(guider_ui.weather_img_5, day1_dayweather);
		update_weather_img(guider_ui.weather_img_4, day2_dayweather);
		update_flag = 0;	
	}
}



void setup_scr_weather(lv_ui *ui)
{
	//屏幕
	ui->weather = lv_obj_create(NULL);
	lv_obj_set_size(ui->weather, 240, 240);
	lv_obj_set_style_bg_opa(ui->weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->weather, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);

	//横线
	ui->weather_line_1 = lv_line_create(ui->weather);
	static lv_point_t weather_line_1[] ={{0, 0},{240, 0},};
	lv_line_set_points(ui->weather_line_1, weather_line_1, 2);
	lv_obj_set_pos(ui->weather_line_1, 0, 160);
	lv_obj_set_size(ui->weather_line_1, 240, 1);
	lv_obj_set_style_line_width(ui->weather_line_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->weather_line_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->weather_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->weather_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//竖线1
	ui->weather_line_2 = lv_line_create(ui->weather);
	static lv_point_t weather_line_2[] ={{0, 0},{0, 80},};
	lv_line_set_points(ui->weather_line_2, weather_line_2, 2);
	lv_obj_set_pos(ui->weather_line_2, 80, 160);
	lv_obj_set_size(ui->weather_line_2, 1, 80);
	lv_obj_set_style_line_width(ui->weather_line_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->weather_line_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->weather_line_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->weather_line_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//竖线2
	ui->weather_line_3 = lv_line_create(ui->weather);
	static lv_point_t weather_line_3[] ={{0, 0},{0, 80},};
	lv_line_set_points(ui->weather_line_3, weather_line_3, 2);
	lv_obj_set_pos(ui->weather_line_3, 160, 160);
	lv_obj_set_size(ui->weather_line_3, 1, 80);
	lv_obj_set_style_line_width(ui->weather_line_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->weather_line_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->weather_line_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->weather_line_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);
	
	//小新动图
	ui->weather_animimg_1 = lv_animimg_create(ui->weather);
	lv_animimg_set_src(ui->weather_animimg_1, (lv_img_dsc_t **) weather_animimg_1_imgs, 8);
	lv_animimg_set_duration(ui->weather_animimg_1, 150*8);
	lv_animimg_set_repeat_count(ui->weather_animimg_1, LV_ANIM_REPEAT_INFINITE);
	lv_animimg_start(ui->weather_animimg_1);
	lv_obj_set_pos(ui->weather_animimg_1, 130, 87);
	lv_obj_set_size(ui->weather_animimg_1, 85, 90);
	

	//今天日期
	ui->weather_label_1 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_1, "2024/01/08\n星期一");
	lv_label_set_long_mode(ui->weather_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_1, 87, 5);
	lv_obj_set_size(ui->weather_label_1, 146, 56);
	lv_obj_set_style_border_width(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_1, &lv_font_ShangShouZongYiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//现在天气
	ui->weather_label_2 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_2, "晴");
	lv_label_set_long_mode(ui->weather_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_2, -7, 69);
	lv_obj_set_size(ui->weather_label_2, 76, 20);
	lv_obj_set_style_border_width(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_2, &lv_font_ShangShouZongYiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//城市
	ui->weather_label_3 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_3, "洪山区");
	lv_label_set_long_mode(ui->weather_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_3, 139, 49);
	lv_obj_set_size(ui->weather_label_3, 88, 19);
	lv_obj_set_style_border_width(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_3, &lv_font_ShangShouZongYiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//现在温度
	ui->weather_label_4 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_4, "14度");
	lv_label_set_long_mode(ui->weather_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_4, -59, 90);
	lv_obj_set_size(ui->weather_label_4, 173, 19);
	lv_obj_set_style_border_width(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_4, &lv_font_ShangShouZongYiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//空气湿度条
	ui->weather_bar_1 = lv_bar_create(ui->weather);
	lv_obj_set_style_anim_time(ui->weather_bar_1, 1000, 0);
	lv_bar_set_mode(ui->weather_bar_1, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->weather_bar_1, 53, LV_ANIM_OFF);
	lv_obj_set_pos(ui->weather_bar_1, 7, 141);
	lv_obj_set_size(ui->weather_bar_1, 56, 10);
	lv_obj_set_style_bg_opa(ui->weather_bar_1, 79, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->weather_bar_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->weather_bar_1, LV_GRAD_DIR_HOR, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->weather_bar_1, lv_color_hex(0x0684a4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_bar_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_bar_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->weather_bar_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_bar_1, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//"空气湿度:"
	ui->weather_label_5 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_5, "空气湿度:");
	lv_label_set_long_mode(ui->weather_label_5, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_5, 5, 113);
	lv_obj_set_size(ui->weather_label_5, 107, 20);
	lv_obj_set_style_border_width(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_5, &lv_font_ShangShouZongYiTi_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//空气湿度
	ui->weather_label_6 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_6, "53%");
	lv_label_set_long_mode(ui->weather_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_6, 69, 138);
	lv_obj_set_size(ui->weather_label_6, 47, 16);
	lv_obj_set_style_border_width(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_6, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_6, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//城市图标
	ui->weather_img_1 = lv_img_create(ui->weather);
	lv_obj_add_flag(ui->weather_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->weather_img_1, &_city_alpha_34x33);
	lv_img_set_pivot(ui->weather_img_1, 50,50);
	lv_img_set_angle(ui->weather_img_1, 0);
	lv_obj_set_pos(ui->weather_img_1, 103, 45);
	lv_obj_set_size(ui->weather_img_1, 34, 33);
	lv_obj_set_style_img_opa(ui->weather_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//"今天"
	ui->weather_label_7 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_7, "今天");
	lv_label_set_long_mode(ui->weather_label_7, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_7, 5, 164);
	lv_obj_set_size(ui->weather_label_7, 69, 16);
	lv_obj_set_style_border_width(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_7, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//"明天"
	ui->weather_label_8 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_8, "明天");
	lv_label_set_long_mode(ui->weather_label_8, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_8, 86, 164);
	lv_obj_set_size(ui->weather_label_8, 69, 16);
	lv_obj_set_style_border_width(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_8, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//"后天"
	ui->weather_label_9 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_9, "后天");
	lv_label_set_long_mode(ui->weather_label_9, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_9, 167, 164);
	lv_obj_set_size(ui->weather_label_9, 69, 16);
	lv_obj_set_style_border_width(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_9, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_9, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_9, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//今天气温
	ui->weather_label_10 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_10, "12~24度");
	lv_label_set_long_mode(ui->weather_label_10, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_10, 3, 220);
	lv_obj_set_size(ui->weather_label_10, 103, 15);
	lv_obj_set_style_border_width(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_10, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_10, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_10, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//明天气温
	ui->weather_label_11 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_11, "11~20度");
	lv_label_set_long_mode(ui->weather_label_11, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_11, 81, 220);
	lv_obj_set_size(ui->weather_label_11, 78, 16);
	lv_obj_set_style_border_width(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_11, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//后天气温
	ui->weather_label_12 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_12, "19~17度");
	lv_label_set_long_mode(ui->weather_label_12, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_12, 152, 220);
	lv_obj_set_size(ui->weather_label_12, 85, 16);
	lv_obj_set_style_border_width(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_12, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_12, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_12, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_12, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//现在天气图标
	ui->weather_img_2 = lv_img_create(ui->weather);
	lv_obj_add_flag(ui->weather_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->weather_img_2, &_qing_alpha_24x24);
	lv_img_set_pivot(ui->weather_img_2, 50,50);
	lv_img_set_angle(ui->weather_img_2, 0);
	lv_obj_set_pos(ui->weather_img_2, 59, 88);
	lv_obj_set_size(ui->weather_img_2, 24, 24);
	lv_obj_set_style_img_opa(ui->weather_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	

	//今天天气图标
	ui->weather_img_3 = lv_img_create(ui->weather);
	lv_obj_add_flag(ui->weather_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->weather_img_3, &_duoyun_alpha_24x24);
	lv_img_set_pivot(ui->weather_img_3, 50,50);
	lv_img_set_angle(ui->weather_img_3, 0);
	lv_obj_set_pos(ui->weather_img_3, 3, 187);
	lv_obj_set_size(ui->weather_img_3, 24, 24);
	lv_obj_set_style_img_opa(ui->weather_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	
	//后天天气图标
	ui->weather_img_4 = lv_img_create(ui->weather);
	lv_obj_add_flag(ui->weather_img_4, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->weather_img_4, &_zhenyu_alpha_24x24);
	lv_img_set_pivot(ui->weather_img_4, 50,50);
	lv_img_set_angle(ui->weather_img_4, 0);
	lv_obj_set_pos(ui->weather_img_4, 165, 187);
	lv_obj_set_size(ui->weather_img_4, 24, 24);
	lv_obj_set_style_img_opa(ui->weather_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//明天天气图标
	ui->weather_img_5 = lv_img_create(ui->weather);
	lv_obj_add_flag(ui->weather_img_5, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->weather_img_5, &_dayu_alpha_24x24);
	lv_img_set_pivot(ui->weather_img_5, 50,50);
	lv_img_set_angle(ui->weather_img_5, 0);
	lv_obj_set_pos(ui->weather_img_5, 83, 187);
	lv_obj_set_size(ui->weather_img_5, 24, 24);
	lv_obj_set_style_img_opa(ui->weather_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//今天天气
	ui->weather_label_13 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_13, "多云\n多云");
	lv_label_set_long_mode(ui->weather_label_13, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_13, 28, 182);
	lv_obj_set_size(ui->weather_label_13, 100, 32);
	lv_obj_set_style_border_width(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_13, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_13, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_13, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_13, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//明天天气
	ui->weather_label_14 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_14, "大雨\n小雨");
	lv_label_set_long_mode(ui->weather_label_14, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_14, 111, 182);
	lv_obj_set_size(ui->weather_label_14, 100, 32);
	lv_obj_set_style_border_width(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_14, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_14, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_14, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_14, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//后天天气
	ui->weather_label_15 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_15, "阵雨\n阵雨");
	lv_label_set_long_mode(ui->weather_label_15, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_15, 192, 182);
	lv_obj_set_size(ui->weather_label_15, 100, 32);
	lv_obj_set_style_border_width(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_15, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_15, &lv_font_ShangShouZongYiTi_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_15, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_15, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//时分
	ui->weather_label_16 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_16, "12:23");
	lv_label_set_long_mode(ui->weather_label_16, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_16, 92, 71);
	lv_obj_set_size(ui->weather_label_16, 123, 43);
	lv_obj_set_style_border_width(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_16, lv_color_hex(0xfe7e2b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_16, &lv_font_banana_font_50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_16, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//秒数
	ui->weather_label_17 = lv_label_create(ui->weather);
	lv_label_set_text(ui->weather_label_17, "57");
	lv_label_set_long_mode(ui->weather_label_17, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->weather_label_17, 205, 87);
	lv_obj_set_size(ui->weather_label_17, 42, 17);
	lv_obj_set_style_border_width(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->weather_label_17, lv_color_hex(0x861078), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->weather_label_17, &lv_font_banana_font_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->weather_label_17, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->weather_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->weather_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	#if 1
	//mika动图
	ui->mika_animimg_1 = lv_animimg_create(ui->weather);
	lv_animimg_set_src(ui->mika_animimg_1, (lv_img_dsc_t **) mika_animimg_1_imgs, 45);
	lv_animimg_set_duration(ui->mika_animimg_1, 100*45);
	lv_animimg_set_repeat_count(ui->mika_animimg_1, LV_ANIM_REPEAT_INFINITE);
	lv_obj_set_pos(ui->mika_animimg_1, 0, -2);
	lv_obj_set_size(ui->mika_animimg_1, 64, 66);
	lv_animimg_start(ui->mika_animimg_1);
	#endif
	
	//取消滚动条显示
	lv_obj_clear_flag(ui->weather, LV_OBJ_FLAG_SCROLLABLE);

	//Update current screen layout.
	lv_obj_update_layout(ui->weather);


	/*进入界面后更新时间*/
	if(WiFi.status() == WL_CONNECTED){
		const char* today = (std::to_string(year) + "/" + 
							 (month < 10?"0"+std::to_string(month):std::to_string(month)) + "/" + 
							 (date < 10?"0"+std::to_string(date):std::to_string(date)) + "\n" + 
							 (std::string)day
							).c_str();
		lv_label_set_text(ui->weather_label_1, today);
		if(!getLocalTime(&timeinfo)){
			const char* minute = ((timeinfo.tm_hour < 10 ? "0" + std::to_string(timeinfo.tm_hour) : std::to_string(timeinfo.tm_hour)) + ":" + 
						          (timeinfo.tm_min < 10 ? "0" + std::to_string(timeinfo.tm_min) : std::to_string(timeinfo.tm_min))       
						         ).c_str();  //时分
			const char* second = ((timeinfo.tm_sec < 10 ? "0" + std::to_string(timeinfo.tm_sec) : std::to_string(timeinfo.tm_sec))
			                     ).c_str(); //秒
			lv_label_set_text(ui->weather_label_16, minute);
			lv_label_set_text(ui->weather_label_17, second);
		}
		//创造定时器任务
		lvgl_task_clock = lv_timer_create(time_second_task, 1000, 0);//每一秒钟刷新一次时间显示,每次秒为0时刷新分钟显示,同时若按钮按下则刷新天气
	}
}
