#include "stm32f1xx.h" \\include lib
int main() \\start the main code
{
 RCC->APB2ENR |= ((1<<4)|(1<<3)); \\set clock bits for port B and C in APB2 bus
 GPIOB->CRL &= ~((1<<28)|(1<<29)); \\ set mode for pin 7 as input or 00
 GPIOB->CRL |= ((1<<30)|(0<<31)); \\ set CNF as input push pull or 1,0
 GPIOC->CRH |=((1<<20)|(1<<21));  \\ set led pin 13 as output with max switching frequency
 GPIOC->CRH &= ~((1<<22)|(1<<23)); \\ set led pin as output push pull
 while(1) \\ start an infinite loop
 {
  if ((GPIOB->IDR & (1<<7))) \\ check if pin 7 is sensing any voltage pulse if true
  GPIOC->BRR |= (1<<13); \\ turn on led as low state powers the led
  else
  GPIOC->ODR |= (1<<13); \\ if false turn the led off by pulling it high
 }
}
