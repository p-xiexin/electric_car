#include "Remote_Control.h"
#include "rc.h"
#include "stm32f4xx.h"
#include "stdio.h"

//遥控器出错数据上限
#define RC_CHANNAL_ERROR_VALUE 700

//取正函数
static int16_t RC_abs(int16_t value);
//遥控器处理函数
static void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl);

//遥控器控制变量
static RC_ctrl_t rc_ctrl;
//接收原始数据，为8个字节，给了16个字节长度，防止DMA传输越界
static uint8_t SBUS_rx_buf[2][SBUS_RX_BUF_NUM];

//初始化DMA，串口3
void remote_control_init(void)
{
     RC_Init(SBUS_rx_buf[0], SBUS_rx_buf[1], SBUS_RX_BUF_NUM);
}

//返回遥控器控制变量，通过指针传递方式传递信息
const RC_ctrl_t *get_remote_control_point(void)
{
    return &rc_ctrl;
}

//串口中断
 void USART3_IRQHandler(void)
 {
	 
     if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
     {
         USART_ReceiveData(USART3);
     }
     else if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
     {
         static uint16_t this_time_rx_len = 0;
         USART_ReceiveData(USART3);

         if(DMA_GetCurrentMemoryTarget(DMA1_Stream1) == 0)
         {
             //重新设置DMA
             DMA_Cmd(DMA1_Stream1, DISABLE);
             this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream1);
             DMA_SetCurrDataCounter(DMA1_Stream1, SBUS_RX_BUF_NUM);
             DMA1_Stream1->CR |= DMA_SxCR_CT;
             //清DMA中断标志
             DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
             DMA_Cmd(DMA1_Stream1, ENABLE);
             if(this_time_rx_len == RC_FRAME_LENGTH)
             {
                 //处理遥控器数据
                 SBUS_TO_RC(SBUS_rx_buf[0], &rc_ctrl);
             }
         }
         else
         {
             //重新设置DMA
             DMA_Cmd(DMA1_Stream1, DISABLE);
             this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream1);
             DMA_SetCurrDataCounter(DMA1_Stream1, SBUS_RX_BUF_NUM);
             DMA1_Stream1->CR &= ~(DMA_SxCR_CT);
             //清DMA中断标志
             DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
             DMA_Cmd(DMA1_Stream1, ENABLE);
             if(this_time_rx_len == RC_FRAME_LENGTH)
             {
                 //处理遥控器数据
                 SBUS_TO_RC(SBUS_rx_buf[1], &rc_ctrl);
             }
         }
     }
 }


//取正函数
static int16_t RC_abs(int16_t value)
{
    if (value > 0)
    {
        return value;
    }
    else
    {
        return -value;
    }
}

static void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }

    if(sbus_buf[0] == SBUS_START_BYTE && sbus_buf[7] == SBUS_END_BYTE)
    {
		
        rc_ctrl->ch[0] = (sbus_buf[2] | (sbus_buf[3] << 8));
        rc_ctrl->ch[1] = (sbus_buf[4] | (sbus_buf[5] << 8));
        rc_ctrl->s = sbus_buf[5];
    }
}

