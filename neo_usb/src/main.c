//TEST
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "stm32f4xx.h"
#include <misc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_usart.h>
//#include "stm32f4xx_conf.h"
//#include "main.h"

//#include "usbd_cdc_core.h"
//#include "usbd_usr.h"
//#include "usbd_desc.h"
//#include "usbd_cdc_vcp.h"

// Private variables
//volatile uint32_t time_var1, time_var2;
//__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

// Private function prototypes
//void Delay(volatile uint32_t nCount);
//void init();
//void calculation_test();

//Dummy function to avoid compiler error
void _init() {

}

void EnableSystemCoreClock()
{
    SysTick_Config(SystemCoreClock / 1000);
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
void Delay( int nTime )
{
    TimingDelay = nTime ;
    while(TimingDelay != 0);
}

void gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void printf_uart(char * string){
    while(*string){
        //ITM_SendChar(65);
        USART_SendData (USART3, (unsigned short int ) * string ++);
        while ( USART_GetFlagStatus(USART3 , USART_FLAG_TC) == RESET );
    }
}

void EnableUART()
{
   USART_InitTypeDef USART_InitStructure ;
   GPIO_InitTypeDef GPIO_InitStructure ;
   //Enable Clock GPIOC, USART3
   RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC , ENABLE);
   RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART3 , ENABLE);
   //PC10 ---> USART3_TX, PC11 ---> USART3_RX
   GPIO_PinAFConfig (GPIOC , GPIO_PinSource10, GPIO_AF_USART3 );
   GPIO_PinAFConfig(GPIOC , GPIO_PinSource11, GPIO_AF_USART3);
   //Init GPIO 10,11 as AF
   GPIO_InitStructure .GPIO_OType = GPIO_OType_PP; //
   GPIO_InitStructure .GPIO_PuPd = GPIO_PuPd_UP; //
   GPIO_InitStructure .GPIO_Mode = GPIO_Mode_AF; // ³ ] ©w ¬° AF, AF¤S ³ Q³ ] ¦¨¤FGPIO_AF_USART3
   GPIO_InitStructure .GPIO_Pin = GPIO_Pin_10; //³ ]PC10
   GPIO_InitStructure .GPIO_Speed = GPIO_Speed_50MHz; //50 MHz
   GPIO_Init(GPIOC , & GPIO_InitStructure);
   GPIO_InitStructure .GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure .GPIO_Pin = GPIO_Pin_11; //³ ]PC10
   GPIO_Init(GPIOC , & GPIO_InitStructure);
   //Init USART
   USART_InitStructure .USART_BaudRate = 115200;
   USART_InitStructure .USART_WordLength = USART_WordLength_8b;
   USART_InitStructure .USART_StopBits = USART_StopBits_1; //
   USART_InitStructure .USART_Parity = USART_Parity_No; //
   USART_InitStructure .USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
   USART_InitStructure .USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;  //
   USART_Init(USART3 , & USART_InitStructure); //
   //Enable USART3 
     USART_Cmd (USART3 , ENABLE);
}


static volatile int USER_BTN;
void EXTI0_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line0 ) != RESET)
 {
	if (USER_BTN){
		USER_BTN=0;
		GPIO_ResetBits(GPIOD ,GPIO_Pin_15);
	}else{
		USER_BTN=1;
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
	}
   /* Clear the EXTI line 0 pending bit */
   EXTI_ClearITPendingBit (EXTI_Line0);
 }
}

static void EXTILine0_Config(void )
{
 /* Enable GPIOA clock */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
 /* Enable SYSCFG clock */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);
 GPIO_InitTypeDef GPIO_InitStructure ;
 /* Configure PA0 pin as input floating */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 GPIO_Init(GPIOA , & GPIO_InitStructure);
 
/* Connect EXTI Line0 to PA0 pin */
 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA , EXTI_PinSource0);
 
/* Configure EXTI Line0 */
 EXTI_InitTypeDef EXTI_InitStructure ;
 EXTI_InitStructure.EXTI_Line = EXTI_Line0;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure );
 
/* Enable and set EXTI Line0 Interrupt to the lowest priority */
 NVIC_InitTypeDef NVIC_InitStructure ;
 // 設定 EXTI0_IRQn 中斷
 NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  // 設定搶占式優先級
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F ;
 // 設定響應式優先級
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F ;
  // 啟用中斷
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure );
}


int main(void) {
	EnableSystemCoreClock();
	gpio_init();
	//Init USER Button
	EXTILine0_Config();
	EnableUART();
	//<<<<<<<<<<<<<<<<<<<<<<<<
	/*USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);*/
	/*
	USBD_Init(NULL,
	            0,
	            NULL,
	            NULL,
	            NULL);
	*/
	//>>>>>>>>>>>>>>>>>>>>>>>>
	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	//setbuf(stdout, NULL);

	//calculation_test();

	for(;;) {
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(1000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		Delay(1000);
	}

	return 0;
}
/*
void calculation_test() {
	float a = 1.001;
	int iteration = 0;

	for(;;) {
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(500);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		Delay(500);

		time_var2 = 0;
		for (int i = 0;i < 1000000;i++) {
			a += 0.01 * sqrtf(a);
		}

		printf("Time:      %lu ms\n\r", time_var2);
		printf("Iteration: %i\n\r", iteration);
		printf("Value:     %.5f\n\n\r", a);

		iteration++;
	}
}

void init() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}

	// ---------- GPIO -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// ------------- USB -------------- //
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);
}
*/
/*
 * Called from systick handler
 */
 /*
void timing_handler() {
	if (time_var1) {
		time_var1--;
	}

	time_var2++;
}
*/
/*
 * Delay a number of systick cycles (1ms)
 */
 /*
void Delay(volatile uint32_t nCount) {
	time_var1 = nCount;
	while(time_var1){};
}
*/
