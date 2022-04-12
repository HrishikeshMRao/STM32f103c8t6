#include "stm32f1xx.h"
int i;
void TIM2_conf (void)
{
 RCC->APB1ENR |= (1<<0);
 TIM2->CR1 |= (1<<0);
 TIM2->PSC = 7;
 TIM2->ARR = 0xffff;
 while (!(TIM2->SR & (1<<0)));
}
void delay_us (int us)
{
 TIM2->CNT = 0;
 while(TIM2->CNT < us);
}
void delay_ms (int ms)
{
 for(int i=0;i<ms;i++)
 delay_us(1000);
}
int main (void)
{
    RCC->APB2ENR |= (1<<4);
    GPIOC->CRH |= ((1 << 20) | (1 << 21));
    GPIOC->CRH &= ~((1 << 22) | (1 << 23));
    TIM2_conf();
    while(1)
    {
      for(i=0;i<256;i++)
      {
       GPIOC->BSRR = (1<<29);
       delay_ms(5*i/255);
       GPIOC->BSRR = (1<<13);
       delay_ms(5-5*i/255);
      }
      for(i=255;i>=0;i--)
      {
       GPIOC->BSRR = (1<<29);
       delay_ms(i*5/255);
       GPIOC->BSRR = (1<<13);
       delay_ms(5-5*i/255);
      }
    }
}
