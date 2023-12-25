#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "touch.c"
#include "usb_keyboard.c"

void app_main(void)
{
    // 初始化触摸
    initializeTouch();
    printf("初始化完成\n");

    // 校准触摸
    calibraTouch(&touch_calibra_value);
    printf("校准完成完成\n");

    while (1)
    {
        for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
        {
            touch_pad_read_raw_data(button[i], &touch_value[i]); // read raw data.
            touch_value[i] = abs(touch_value[i] - touch_calibra_value[i]);
            printf("T%d: [%lu] ", button[i], touch_value[i]);
        }
        printf("\n");

        // 判断是否有键被按下
        for (size_t i = 0; i < TOUCH_BUTTON_NUM; i++)
        {
            if (touch_value[i] > 2000)
            {
                // 获取触摸位置
                printf("                                                             %d", get_touch_posi(touch_value));
                printf("\n");
                break;
            }
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    
}