#include "stdio.h"
#include "stm32f1xx.h"
uint16_t slash[2]={0,0};
int i,k,x;
void delay(int m)
{
	for(i=0;i<m;i++)
	{
	 int n=0;
	 for(k=0;k<500;k++)
	 n++;
	}
}
void ADC_conf (void)
{
 RCC->CFGR |= (1<<15);// pre-scaler divide by 6
 RCC->APB2ENR |= (1<<9);//adc1 enable
 GPIOA->CRL &= ~((1<<28)|(1<<29)|(1<<30)|(1<<31));//A7 as input analog
 GPIOA->CRL &= ~((1<<8)|(1<<9)|(1<<10)|(1<<11)); //A2 as input analog
 //set sampling rate of channel 1 and channel 2 as max [111]
 ADC1->SMPR2 |= ((1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3));
 //set channel 1 as first and channel 2 as second priority to be converted
 ADC1->SQR3 |= ((1<<1)|(1<<5));
 //set 2 conversions
 ADC1->SQR1 |= (1<<20);
 //enable scan mode
 ADC1->CR1 |= (1<<8);
  //DMA1 enable
  RCC->AHBENR |= (1<<0);
  //enable DMA
  ADC1->CR2 |= (1<<8);
  DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);//link adress of the DR
  DMA1_Channel1->CMAR = (uint32_t)slash;//link adress of the array
  DMA1_Channel1->CNDTR = 2;
  DMA1_Channel1->CCR |= ((1<<11)|(1<<9)|(1<<7)|(1<<5)|(1<<6));
  DMA1_Channel1->CCR |= (1<<0);
  //enable ADon , continuous conversion
 ADC1->CR2 |= ((1<<0)|(1<<1));
 delay(1);
 ADC1->CR2 |= (1<<0);
 delay(1);
 //calibration complete wait for it to turn 1
 ADC1->CR2 |= (1<<2);
 while(ADC1->CR2 & (1<<2));
}
void TIM2_conf (void)
{
 TIM2->PSC = 72;
 TIM2->ARR = 1000;
 TIM2->CNT = 0;
 TIM2->CCR2 =0;
 TIM2->CCR1 =0;
 TIM2->CR1 |= (1<<0);
 TIM2->CCMR1 |= ((1<<11)|(1<<14)|(1<<13));//ch2 output compare enable
 TIM2->CCMR1 &= ~(1<<12);
 TIM2->CCMR1 |= ((1<<3)|(1<<5)|(1<<6));//ch1 output compare enable
 TIM2->CCMR1 &= ~(1<<4);
 TIM2->CCER |= ((1<<4)|(1<<0));
}
int map(int z)
{
 return(z*1000/4096);
}
int main (void)
{
    RCC->APB1ENR |= (1<<0);
    RCC->APB2ENR |= ((1<<2)|(1<<0));
    GPIOA->CRL |= ((1<<4)|(1<<5)|(1<<7)); //A1 as pwm output pin tim2-ch2
	GPIOA->CRL &= ~((1<<6));
	GPIOA->CRL |= ((1<<0)|(1<<1)|(1<<3));//A0 as pwm output pin  tim2-ch1
	GPIOA->CRL &= ~((1<<2));
	TIM2_conf();
	ADC_conf();
    while(1)
    {
    	 //printf("chan[1]:[%d],chan[2]:[%d]\r",slash[0],slash[1]);
    	 TIM2->CCR1 = map(slash[1]);
    	 TIM2->CCR2 = map(slash[0]);
    }

}
