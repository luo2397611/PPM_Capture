#include "ppm.h"

uint16_t PPM_Sample_Cnt = 0;//通道
uint8_t PPM_Chn_Max = 8;//最大通道数
uint32_t PPM_Time = 0;//获取通道时间
uint16_t PPM_Okay = 0;//下一次解析状态
uint16_t PPM_Databuf[8] = {0};//所有通道的数组

//PPM解析中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin==GPIO_PIN_8)//判断是否为接收器产生的中断，例程设置为PIN8
    {
        PPM_Time = TIM2 ->CNT;//将定时数转存
        TIM2 -> CNT = 0;//计数器归零
        if (PPM_Okay == 1)//判断是否是新的一轮解析
        {
            PPM_Sample_Cnt++;//通道数+1
            PPM_Databuf[PPM_Sample_Cnt - 1] = PPM_Time;//把每一个通道的数值存入数组
            if (PPM_Sample_Cnt >= PPM_Chn_Max)//判断是否超过额定通道数
                PPM_Okay = 0;
        }
        if (PPM_Time >= 2050)//长时间无下降沿即无通道数据，进入下一轮解析
        {
            PPM_Okay = 1;
            PPM_Sample_Cnt = 0;
        }
    }
}

void TEST_send_message(void)
{
    for (uint8_t i = 0; i < PPM_Chn_Max; i++)
        printf("chn%d:%d\n", i+1, PPM_Databuf[i]);
    HAL_Delay(100);
}


