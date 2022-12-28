#include "stm32f1xx.h"
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
 RCC->CFGR |= (1<<15);
 RCC->APB2ENR |= (1<<9);
 GPIOA->CRL &= ~((1<<8)|(1<<9));
 GPIOA->CRL &= ~((1<<10)|(1<<11));
 ADC1->SMPR2 |= ((1<<8)|(1<<7)|(1<<6));
 ADC1->SQR3 |= (1<<1);
 ADC1->CR2 |= ((1<<0)|(1<<1));
 delay(1);
 ADC1->CR2 |= (1<<0);
 delay(1);
 ADC1->CR2 |= (1<<2);
 while(ADC1->CR2 & (1<<2));
}
void TIM2_conf (void)
{
 TIM2->PSC = 7;
 TIM2->ARR = 1000;
 TIM2->CCR2 =0;
 TIM2->EGR |=(1<<0);
 TIM2->CR1 |= (1<<0);
 TIM2->CCMR1 |= ((1<<11)|(1<<14)|(1<<13));
 TIM2->CCMR1 &= ~((1<<12));
 TIM2->CCER |= (1<<4);
}
int map(int z)
{
 return(z*1000/4096);
}
int main (void)
{
    RCC->APB1ENR |= (1<<0);
    RCC->APB2ENR |= ((1<<2)|(1<<0));
    GPIOA->CRL |= ((1 << 4) | (1 << 5) | (1<<7));
    GPIOA->CRL &= ~((1<<6));
    TIM2_conf();
    ADC_conf();
    while(1)
    {
    	 x = ADC1->DR;
    	 TIM2->CCR2 = map(x);
    }

}
