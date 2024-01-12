项目基于 vscode platformio 搭建
使用的主控是esp32c3
屏幕为1.54寸 240x240 TFT屏 st7789驱动
使用的库有TFT_ESPI, WiFi, ArduinoJOSN, lvgl(v8.3.0), HTTPClient
main.cpp中可以修改屏幕长宽，修改城市与adcode信息,但注意城市的字体没有导入，需自行导入。
需要修改TFT_ESPI驱动和lvgl的可修改相应的cont

项目中遇到的问题
一.esp32c3性能有限，只有单核，很容易内存溢出，程序需要小心优化。特别是freeRTOS系统任务堆栈
二.MOSI的PIN与adc2冲突，经测试发现连接wifi时这个pin很容易有问题，导致主控连接wifi就反复重启，故选择了先连接wifi再启动tft与lvgl
三.通过网页web获取json也会占用大量内存，故在程序中配置了gpio按键中断，按下按键后lvgl ui停止,核心进程进行更新天气时间,然后重新显示在屏幕上
四.c3的内置flash为4mb,比较有限，已经通过修改分区表将程序存储占用的flash改为3mb

