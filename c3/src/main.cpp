#include <TFT_eSPI.h>
#include <lvgl.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "generated\gui_guider.h"
#include "generated\customs.h"
#include "my_function.hpp"

/*要连接的wifi账密*/
#define ssid "123456789"
#define password "123456789"

/**********PINS**********
TFT_MOSI  6  
TFT_SCLK  9  
TFT_CS    7   
TFT_DC    5   
TFT_RST   10 
KEY       20
***********PINS**********/

/*屏幕长宽*/
#define MY_DISP_HOR_RES  240
#define MY_DISP_VER_RES  240

/*中断*/
#define GPIO20_INPUT_IO   (gpio_num_t)20
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO20_INPUT_IO)
#define ESP_INTR_FLAG_DEFAULT 0
static xQueueHandle gpio_evt_queue = NULL;

/*显示驱动*/
lv_ui guider_ui;
TFT_eSPI tft = TFT_eSPI();  

/*lvgl buffer*/
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf_1[MY_DISP_HOR_RES * 10];  /*A buffer for 10 rows*/

/*中断处理*/
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

/*中断服务的任务*/
static void scr_task(void* arg)
{
    uint32_t io_num;
    while(1) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            weather_update();
        }
    }
}

/*lvgl display flush*/
void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
    lv_disp_flush_ready( disp_drv );
}

void setup()
{
    /*************custom_code**************/
    adcode = "420111";//地区的adcode
    city = "洪山区";
    


    /**************************************/
    
    /*连接wifi后才能进入界面*/
    WiFi.begin(ssid,password);
    while(1){
        delay(2500);
        if(WiFi.status() == WL_CONNECTED){
            break;
        }
        WiFi.begin(ssid,password);
    }
    time_init();
    
    /*tft初始化*/
    tft.begin();          
    tft.setRotation( 0 ); 

    /*LVGL初始化*/
    lv_init();
    
    /*lvgl显示驱动初始化*/
    lv_disp_draw_buf_init(&draw_buf, buf_1, NULL, MY_DISP_HOR_RES * 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );
    
    /*中断服务初始化*/
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = (gpio_pullup_t)1;
    gpio_config(&io_conf);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO20_INPUT_IO, gpio_isr_handler, (void*) GPIO20_INPUT_IO);
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(scr_task, "scr_task", 30000, NULL, 10, NULL);//堆栈大小没有准确计算
    delay(50);

    //ui初始化
    setup_ui(&guider_ui);
}

void loop()
{
    lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(5));
}
