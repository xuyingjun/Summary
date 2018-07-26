/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "math.h"	  
#include "stdlib.h"
#include "sys.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

frame frm=
{
    0x00,
    0x00,
    0x00,
    0x00,
    0x00   
};
disframe disfrm=
{
    0x00,
    0x00,
    0x00
};

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
   
}
/*定时器1中断处理*/
void TIM1_UP_IRQHandler(void)
{
  
	
   if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
   {
     Tflag++;

	if(Tflag==6)
	{
		Oflag=1;
	}
	else if(Tflag==18)
	{	
		Oflag=0;
		Tflag=0;
	
	}
    TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);  

   }

   	
}

/*定时器2中断处理*/

void TIM2_IRQHandler(void)
{
   u8 j=0;
   colcnt++;
       
	if (TIM_GetITStatus(TIM2,TIM_IT_CC2) != RESET)
      {
          TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);                                      //计算PWM频率。

		    IC2Value = TIM_GetCapture2(TIM2); 
		    IC1Value = TIM_GetCapture1(TIM2) ;                        //读取IC2捕获寄存器的值，即为PWM周期的计数值


		  IC2sum[19] = IC2Value;

	//	 if (IC1sum[0]==0)
	//	  IC1sum[49] = IC1Value;
	//	 else if((IC1sum[0]!=0)&(abs(IC1Value-IC1sum[48])<800))
		  IC1sum[19] = IC1Value;
		 

	      for(j=0;j<19;j++)
          {
	  	    IC1sum[j] =  IC1sum[j+1];
			IC2sum[j] =  IC2sum[j+1];

	      }
	

	  }


//	  Tim2SP() ;

}




/*定时器3中断处理，主要处理串口接收数据*/
void TIM3_IRQHandler(void)
{     

   if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
   {
      TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);      
      getdataflag=1;
   }
  
}

/*定时器4中断处理，主要处理串口接收数据*/
void TIM4_IRQHandler(void)
{     

   if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
   {
      TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);      
	  dataProcess();
   }
  
}

void TIM5_IRQHandler(void)
{     

   if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
   {
      TIM_ClearITPendingBit(TIM5,TIM_FLAG_Update); 

	 if (lcdBG_timecount==8)
    	{
			lcdBG=0;
			lcdBG_timecount=0;
			PCout(6)=0;
		}
		lcdBG_timecount++;
   }

}

//void TIM6_IRQHandler(void)
//{
//    u8 i;

//   if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
//   {
//      TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update); 
//	  
//	  mCount++;
//	  if(mCount==0x80)			  //22
//	  {
//	     mCount=0;
//	     for(i=0;i<24;i++)
//		 {
//		    usart_r_buff[i]=0x00; 
//		 }
//         //Cntnum=0;  		
//         TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE); 
//	  }		
//		 
//   }
//  
//}
/*串口1中断处理*/
void USART1_IRQHandler(void)
{
 
//    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
//    {
//        USART_ClearFlag(USART1,USART_FLAG_ORE); //读SR其实就是清除标志
//        USART_ReceiveData(USART1);        
//    }
//    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
//    {
//        usart1_buff[frm.num]=USART_ReceiveData(USART1);
//        usart1_buff[frm.num]-=0x33;
//        frm.num++;
////        if(frm.num==1)
////        {
////            TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);             
////        }
//        USART_ClearITPendingBit(USART1,USART_IT_RXNE);        
//    }  
//	usart2_senddata(Send_read);
}

/*串口1发送数据*/
void usart_senddata(u8 arr[])
{

}

void USART2_IRQHandler(void)
{
  u8 i=0;

    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR其实就是清除标志
        USART_ReceiveData(USART2);		
    }

    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
    {
        usart_r_buff[Cntnum]=USART_ReceiveData(USART2);
        Cntnum++;
        
//		if(Cntnum==1)
//        {
//            TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
//        }
		 USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }     
    if(Cntnum==24)
    {
        Cntnum=0;
        format=1;
        if((usart_r_buff[0]!=0x55)||(usart_r_buff[1]!=0x66)||(usart_r_buff[22]!=0x66)||(usart_r_buff[23]!=0x55))
            format=0;
        if(format)
        {
            for(i=0;i<24;i++) 
                usart2_buff[i]=usart_r_buff[i];

		     	Receiver_process();
				Cntnum=0;
				kflag0=0;
				
        }
		else
		Cntnum=0;

}
	   Usart2_Rx()	 ;

		USART_ITConfig(USART2,USART_IT_TXE,DISABLE);			  	//发送完成 关闭中断
}


void usart2_senddata(u8 typeA)
{
    u8 i;
   
  Usart2_Tx() ;
  delay_ms(2);
	switch(typeA)
 	{
		case 0:
		for(i=0;i<24;i++)
		{
		  USART_SendData(USART2,Send_read[i]); //read  
		  delay_ms(2);
		 }
		break;
		
		case 1:
		for(i=0;i<24;i++)
		{
		USART_SendData(USART2,Send_set[i]); //set
		 delay_ms(2);
		return_flag=1;
		}
		break;
		
		case 2:
		for(i=0;i<24;i++)
		{USART_SendData(USART2,Send_start[i]);//start
		 delay_ms(2);
		return_flag=2;
		}
		break;
	
		case 3:
		for(i=0;i<24;i++)
		{USART_SendData(USART2,Send_stop[i]);//stop
		 delay_ms(2);
		return_flag=3;	 }
		break;
	
		case 4:
		for(i=0;i<24;i++)
		{USART_SendData(USART2,Send_Emstart[i]);//stop
		 delay_ms(2);
		return_flag=4;	 }
		break;

		case 5:
		for(i=0;i<24;i++)
		{USART_SendData(USART2,Send_Emstop[i]);//stop
		 delay_ms(2);
		return_flag=5;	 }

		break;

    	default:
		break;
	}

	USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
	Usart2_Rx()	 ;

}


u8 CRC_check(u8 arr[],u8 start, u8 stop)

{
    u8 i,value;
    u16 tep=0;
 
    for(i=start;i<stop;i++)
    {
        tep+=arr[i];
    }
    value= tep%100;
    return value;
}


void packagedata(u16 PvRH,u16 PvTe,u16 SvTe,u16 PvCO2,u16 SvCO2,u8 flag)
{

	Send_read[3]=PvCO2/100;
	Send_read[4]=PvCO2%100;
	Send_read[5]=SvCO2/100;
	Send_read[6]=SvCO2%100;
	Send_read[7]=PvTe/100;
	Send_read[8]=PvTe%100;
	Send_read[9]=SvTe/100;
	Send_read[10]=SvTe%100;
	Send_read[11]=PvRH/100;
	Send_read[12]=PvRH%100;
	Send_read[13]=flag;
	//Send_set[20]=unoccup;
	Send_read[21]=(Send_read[3]+Send_read[4]+Send_read[5]+Send_read[6]+Send_read[7]+Send_read[8]+Send_read[9]+Send_read[10]+Send_read[11]+Send_read[12]+Send_read[13]+Send_read[14])%100;
}

void Receiver_process(void)
{
	switch(usart2_buff[2])
    {
        case 1:PSvTe=usart2_buff[3]*256+usart2_buff[4];break;			   
		case 2:Send_finish(3);break;
		case 3:Send_finish(1);break;
		case 4:Send_finish(2);break;
	    case 5:Send_finish(4);break;
		case 6:Send_finish(5);break;

        default: break;
     }
}	  
	  


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
