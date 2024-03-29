#include "stm32f1xx.h"
int i,k;
void TIM2_conf (void)
{
 TIM2->PSC = 7;
 TIM2->ARR = 255;
 TIM2->CCR1 =0;
 TIM2->EGR |=(1<<0);
 TIM2->CR1 |= (1<<0);
 TIM2->CCMR1 |= ((1<<11)|(1<<14)|(1<<13));
 TIM2->CCMR1 &= ~((1<<12));
 TIM2->CCER |= (1<<4);
}
void delay(void)
{
 int n=0;
 for(k=0;k<5000;k++)
 n++;
}
int main (void)
{
    RCC->APB1ENR |= (1<<0);
    RCC->APB2ENR |= ((1<<2)|(1<<0));
    GPIOA->CRL |= ((1 << 4) | (1 << 5));
    GPIOA->CRL |= (1 << 7);
    GPIOA->CRL &= ~((1<<6));
    TIM2_conf();
    while(1)
    {
      for(i=0;i<255;i++)
      {
       TIM2->CCR2 = i;
       delay();
      }
      for(i=255;i>=0;i--)
      {
       TIM2->CCR2 = i;
       delay();
      }
    }
}
