#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TOUCH_BUTTON_NUM 4

uint32_t touch_value[TOUCH_BUTTON_NUM];
uint32_t touch_calibra_value[TOUCH_BUTTON_NUM];

static const touch_pad_t button[TOUCH_BUTTON_NUM] = {
    // TOUCH_PAD_NUM1,
    // TOUCH_PAD_NUM2,
    // TOUCH_PAD_NUM3,
    TOUCH_PAD_NUM4,
    TOUCH_PAD_NUM5,
    TOUCH_PAD_NUM6,
    TOUCH_PAD_NUM7
    // TOUCH_PAD_NUM8,
    // TOUCH_PAD_NUM9,
    // TOUCH_PAD_NUM10,
    // TOUCH_PAD_NUM11,
    // TOUCH_PAD_NUM12,
    // TOUCH_PAD_NUM13,
    // TOUCH_PAD_NUM14
};

// #include "touch.c"
// #include "usb_keyboard.c"

int touch_posi;

void app_main(void)
{
    // 配置USB
    // tinyusb_driver_install(&tusb_cfg);

    // 初始化触摸
    touch_pad_init();
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        touch_pad_config(button[i]);
    }

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_fsm_start();

    vTaskDelay(200 / portTICK_PERIOD_MS);
    // initializeTouch();
    printf("初始化完成\n");

    vTaskDelay(200 / portTICK_PERIOD_MS);

    // 校准触摸
    // calibraTouch(&touch_calibra_value);
    uint32_t touch_value[TOUCH_BUTTON_NUM][10];

    // 获取校准数据
    for (size_t ii = 0; ii < 10; ii++)
    {
        for (size_t i = 0; i < TOUCH_BUTTON_NUM; i++)
        {
            touch_pad_read_raw_data(button[i], &touch_value[i][ii]);
            printf("%lu ", touch_value[i][ii]);
        }
        printf("\n");
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }

    printf("\n");

    // 计算平均值
    for (size_t i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        for (size_t ii = 0; ii < 10; ii++)
        {
            calibraData[i] = calibraData[i] + touch_value[i][ii];
        }
        calibraData[i] = round((long double)calibraData[i] / 10);
        printf("%lu ", calibraData[i]);
    }
    printf("校准完成完成\n");

    while (1)
    {
        // readTouchData(&touch_value);

        // 判断是否有键被按下
        // for (size_t i = 0; i < TOUCH_BUTTON_NUM; i++)
        // {
        //     if (touch_value[i] > 1000)
        //     {
        //         // 获取触摸位置
        //         touch_posi = get_touch_posi(touch_value);
        //         vTaskDelay(10 / portTICK_PERIOD_MS);
        //         readTouchData(&touch_value);
        //         printf("%d\n", abs(touch_posi - get_touch_posi(touch_value)));
        //         if (abs(touch_posi - get_touch_posi(touch_value)) < 10)
        //         {
        //             printf("%s\n","点按");
        //             if (touch_posi >= 0 && touch_posi < 25)
        //             {
        //                 printf("%s\n","x");
        //             }
        //             if (touch_posi >= 25 && touch_posi < 50)
        //             {
        //                 printf("%s\n","y");
        //             }
        //             if (touch_posi >= 50 && touch_posi < 75)
        //             {
        //                 printf("%s\n","b");
        //             }
        //             if (touch_posi >= 75 && touch_posi < 100)
        //             {
        //                 printf("%s\n","a");
        //             }
        //         }else{
        //             printf("%s\n","滑键");
        //         }

        //         break;
        //     }
        // }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}