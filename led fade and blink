#include "stm32f1xx.h"
int i=0,n=0,k ;
void TIM1_conf (void)
{
 TIM1->PSC = 7; // divide clock by 8
 TIM1->ARR = 1000; // set timer max as 1000
 TIM1->CCR4 =0; // set the capture compare mode value as 0
 TIM1->EGR |=(1<<0); // set UG bit 1 or enable update generation
 TIM1->CR1 |= ((1<<0)|(1<<7)); // enable counter enable preload buffer
 TIM1->CCMR2 |= ((1<<11)|(1<<14)|(1<<13)); // set the pwm mode for channel 4
 TIM1->CCMR2 &= ~((1<<12)); // set the pwm mode and enable preload for the timer
 TIM1->CCMR2 |= (1<<7);
 TIM1->CCER |= (1<<12); // enable capture compare for channel 4
 TIM1->BDTR |= (1<<15); //enable main output enable
}
void delay_us (int us) // set delay in ms
{
 TIM1->CNT = 0;
 while(TIM1->CNT < us);// limit max counter value at 1000 tiks
}
void delay_ms (int ms) // set delay in milli seconds
{
 for(int k=0;k<ms;k++)
 delay_us(1000);
}
void delay(int m)
{
  for(int j=0;j<m;j++)
  {
   delay_ms(1);
   if (GPIOA->IDR & (1<<9))
   {
   	delay_ms(100);
   	n++;
   	break;
   }
  }
}
int main (void)
{
	RCC->APB2ENR |= (1<<11);
    RCC->APB2ENR |= ((1<<2)|(1<<0)); //enable timer 1, clock for port A and enable alternate function input output
    GPIOA->CRH |= ((1<<13)|(1<<12)); // set output frequency max
    GPIOA->CRH |= (1<<15); // set A11 in output mode and alternate function push pull
    GPIOA->CRH &= ~((1<<14));
    GPIOA->CRH &= ~((1<<5)|(1<<4)); // set a9 as input mode and input mode push pull
    GPIOA->CRH |= (1<<7);
    GPIOA->CRH &= ~((1<<6));
    TIM1_conf();
    while(1)
    {
    	  if(n>=0)
    	  n%=10;
    	  k=n%2;
    	  switch(k)
    	  {
    	   case 0:
    	   {
    	    for(i;i<1000;i++)
    	    {
    	     if (GPIOA->IDR & (1<<9))
    	     {
    	      n++;
    		  break;
    	     }
    	     TIM1->CCR4 = i;//create pwm
    	     delay_ms(1);
    	    }
    	    if (GPIOA->IDR & (1<<9))
    	    {
    	     delay_ms(100);
    	     break;
    	    }
    	    for(i;i>=0;i--)
    	    {
    	     if (GPIOA->IDR & (1<<9))
    	     {
    	      delay_ms(100);
    	      n++;
    	      break;
    	     }
    	     TIM1->CCR4 = i;
    	     delay_ms(1);
    	    }
    	    break;
    	   }
    	   case 1:
    	   {
    		TIM1->CCR4 = 0;
    	    delay(1000);
    	    TIM1->CCR4 = i;//keep the i value and blink
    	    delay(1000);
    	    break;
    	   }
    	 }
    }
}

