#include "stm32f4xx.h"

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_usart.h>

//Dummy function to avoid compiler error
void _init() {

}

static volatile int TimingDelay;

//Tick Interrupt
void SysTick_Handler(void)
{
    if (TimingDelay != 0)
    {
        TimingDelay --;
    }
}

// nTime=1000 --> 1s
void Delay( int nTime)
{
    TimingDelay = nTime ;
    while(TimingDelay != 0);
}

void GPIODInit()
{
  GPIO_InitTypeDef GPIO_InitStructure ;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);   
  //GPIO_Pin_13=LD3,GPIO_Pin_12=LD4,GPIO_Pin_14=LD5,GPIO_Pin_14=LD6
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                                                         
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD , & GPIO_InitStructure);
}

void EnableSystemCoreClock()
{
    SysTick_Config (SystemCoreClock / 1000);
}

void printf_uart(char * string){
    while(*string){
        USART_SendData (USART3, (unsigned short int ) * string++);
        while ( USART_GetFlagStatus(USART3 , USART_FLAG_TC) == RESET );
    }
}

void EnableUART()
{
   USART_InitTypeDef USART_InitStructure ;
   GPIO_InitTypeDef GPIO_InitStructure ;
   //Enable Clock GPIOC, USART3
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE); 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
   //PC10 ---> USART3_TX, PC11 ---> USART3_RX
   GPIO_PinAFConfig (GPIOC, GPIO_PinSource10, GPIO_AF_USART3 );
   GPIO_PinAFConfig(GPIOC , GPIO_PinSource11, GPIO_AF_USART3);
   //Init GPIO 10,11 as AF
   GPIO_InitStructure .GPIO_OType = GPIO_OType_PP; //
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 3 ]cw ?¢XAF, AF?S3 Q3] |¡L?FGPIO_AF_USART3
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //3 ]PC10
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50 MHz
   GPIO_Init(GPIOC , & GPIO_InitStructure);
   GPIO_InitStructure .GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //3 ]PC10
   GPIO_Init(GPIOC , & GPIO_InitStructure);
   //Init USART
   USART_InitStructure .USART_BaudRate = 115200;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1; //
   USART_InitStructure.USART_Parity = USART_Parity_No; //
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //
   USART_Init(USART3 , & USART_InitStructure); // 
   //Enable USART3  
     USART_Cmd(USART3 , ENABLE);
}

int main( void)
{
  EnableSystemCoreClock();
    GPIODInit();
    EnableUART();
    while(1)
    {           
        printf_uart ("GPIO_Pin_12 Hi\r\n");
        GPIO_SetBits (GPIOD, GPIO_Pin_12);                
    Delay(1000 );
        GPIO_ResetBits (GPIOD, GPIO_Pin_12);   
        printf_uart ("GPIO_Pin_12 Lo\r\n");              
        Delay (1000);                 
        printf_uart ("GPIO_Pin_13 Hi\r\n");              
        GPIO_SetBits (GPIOD, GPIO_Pin_13);                
    Delay(1000 );
      printf_uart ("GPIO_Pin_13 Lo\r\n");                
      GPIO_ResetBits (GPIOD, GPIO_Pin_13);      
        Delay (1000);
      printf_uart ("GPIO_Pin_14 Hi\r\n");                
        GPIO_SetBits (GPIOD, GPIO_Pin_14);                
    Delay(1000 );
      printf_uart ("GPIO_Pin_14 Lo\r\n");                
        GPIO_ResetBits (GPIOD, GPIO_Pin_14);      
        Delay (1000);                 
      printf_uart ("GPIO_Pin_15 Hi\r\n");    
        GPIO_SetBits (GPIOD, GPIO_Pin_15);                
    Delay(1000 );
        printf_uart ("GPIO_Pin_15 Lo\r\n");  
        GPIO_ResetBits (GPIOD, GPIO_Pin_15);      
        Delay (1000);         
    }
}
  