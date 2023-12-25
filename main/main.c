#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "touch.c"
#include "usb_keyboard.c"

int touch_posi;

void app_main(void)
{
    // 配置USB
    tinyusb_driver_install(&tusb_cfg);

    // 初始化触摸
    initializeTouch();
    printf("初始化完成\n");

    // 校准触摸
    calibraTouch(&touch_calibra_value);
    printf("校准完成完成\n");

    while (1)
    {
        readTouchData(&touch_value);

        // 判断是否有键被按下
        for (size_t i = 0; i < TOUCH_BUTTON_NUM; i++)
        {
            if (touch_value[i] > 10000)
            {
                // 获取触摸位置
                touch_posi = get_touch_posi(touch_value);
                vTaskDelay(10 / portTICK_PERIOD_MS);
                readTouchData(&touch_value);
                printf("%d\n", abs(touch_posi - get_touch_posi(touch_value)));
                if (abs(touch_posi - get_touch_posi(touch_value)) < 10)
                {
                    printf("%s\n","点按");
                    if (touch_posi >= 0 && touch_posi < 25)
                    {
                        printf("%s\n","x");
                    }
                    if (touch_posi >= 25 && touch_posi < 50)
                    {
                        printf("%s\n","y");
                    }
                    if (touch_posi >= 50 && touch_posi < 75)
                    {
                        printf("%s\n","b");
                    }
                    if (touch_posi >= 75 && touch_posi < 100)
                    {
                        printf("%s\n","a");
                    }
                }else{
                    printf("%s\n","滑键");
                }

                break;
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}