#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/touch_pad.h"

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

// 初始化触摸
void initializeTouch()
{
    touch_pad_init();
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        touch_pad_config(button[i]);
    }

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_fsm_start();

    vTaskDelay(200 / portTICK_PERIOD_MS);
    return;
}

// 校准触摸
void calibraTouch(uint32_t *calibraData)
{
    uint32_t touch_value[TOUCH_BUTTON_NUM][10];
    int temp;

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

    printf("\n");
}

// 获取触摸位置
int get_touch_posi(int touch_value[TOUCH_BUTTON_NUM])
{
    int weight = 100 / (TOUCH_BUTTON_NUM - 1);
    int touch_posi = 0;
    int touch_devia = 0;

    // 计算公式
    // （按键1变化值 * 权重）+（按键2变化值 * 权重）+...+（按键n变化值 * 权重） / 所有按键变化值的和
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        touch_devia = touch_devia + touch_value[i];
        touch_posi = touch_posi + (touch_value[i] * i * weight);
    }

    touch_posi = touch_posi / touch_devia;

    return touch_posi;
}

// 读取传感器值
// void readTouchData(uint32_t *touch_value)
// {
//     for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
//     {
//         touch_pad_read_raw_data(button[i], &touch_value[i]);
//         touch_value[i] = abs(touch_value[i] - touch_calibra_value[i]);
//     }
// }