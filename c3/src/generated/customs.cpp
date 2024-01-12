#include "customs.h"

lv_timer_t *lvgl_task_clock;
lv_timer_t *lvgl_task_update;

#if 1
const lv_img_dsc_t * weather_animimg_1_imgs[8] = { 
	&weather_animimg_1xiaoxin1,
	&weather_animimg_1xiaoxin2,
	&weather_animimg_1xiaoxin3,
	&weather_animimg_1xiaoxin4,
	&weather_animimg_1xiaoxin5,
	&weather_animimg_1xiaoxin6,
	&weather_animimg_1xiaoxin7,
	&weather_animimg_1xiaoxin8, 
};
#endif

#if 1
/*这里gif图原本解析有90张，太大了，只取一半*/
const lv_img_dsc_t * mika_animimg_1_imgs[45] = { 	
	&mika1,
	&mika3,
	&mika5,
	&mika7,
	&mika9,
	&mika11,
	&mika13,
	&mika15,
	&mika17,
	&mika19,
	&mika21,
	&mika23,
	&mika25,
	&mika27,
	&mika29,
	&mika31,
	&mika33,
	&mika35,
	&mika37,
	&mika39,
	&mika41,
	&mika43,
	&mika45,
	&mika47,
	&mika49,
	&mika51,
	&mika53,
	&mika55,
	&mika57,
	&mika59,
	&mika61,
	&mika63,
	&mika65,
	&mika67,
	&mika69,
	&mika71,
	&mika73,
	&mika75,
	&mika77,
	&mika79,
	&mika81,
	&mika83,
	&mika85,
	&mika87,
	&mika89, 
};
#endif

#if 1
/*更新任务处理函数*/
void weather_update(){
	date_get();
	weather_get();
	update_flag = 1;
}
#endif

/*更新天气图标*/
void update_weather_img(lv_obj_t* obj, const char* weather){
	String w = (String)weather;
	if(w == "晴"){
		lv_img_set_src(obj, &_qing_alpha_24x24);
	} 
	else if(w == "多云" || w == "晴间多云" || w == "少云"){
		lv_img_set_src(obj, &_duoyun_alpha_24x24);
	}
	else if(w == "阵雨"){
		lv_img_set_src(obj, &_zhenyu_alpha_24x24);
	}
	else if(w == "大雨"){
		lv_img_set_src(obj, &_dayu_alpha_24x24);
	}
	else if(w == "雷阵雨" || w == "暴雨" || w == "大暴雨" || w == "特大暴雨"){
		lv_img_set_src(obj, &_leiyu_alpha_24x24);
	}
	else if(w == "雪" || w == "阵雪" || w == "小雪" || w == "中雪" || w == "大雪"){
		lv_img_set_src(obj, &_xue_alpha_24x24);
	}
	else if(w == "阴"){
		lv_img_set_src(obj, &_yin_alpha_24x24);
	}
	else if(w == "雨" || w == "小雨" || w == "中雨"){
		lv_img_set_src(obj, &_yu_alpha_24x24);
	}
	else if(w == "雾霾" || w == "雾" || w == "霾"){
		lv_img_set_src(obj, &_wumai_alpha_24x24);
	}
	else{
		return;
	}
	return;
}


