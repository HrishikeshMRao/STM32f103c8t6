#include "stm32f1xx.h"
int i = 0;
void delay(volatile x)
{
	for (i = 0; i < x; i++)
	{
		long int n = 0;
		for (long int j = 0; j < 5000; j++)
			n++;
	}
}
int main()
{
	RCC->APB2ENR |= (1 << 4);
	GPIOC->CRH |= ((1 << 20) | (1 << 21));
	GPIOC->CRH &= ~((1 << 22) | (1 << 23));
	while (1)
	{
		GPIOC->BSRR = (1 << 13);
	  GPIOC->BSRR &= ~(1<<29);
		delay(20);
		GPIOC->BSRR = (1 << 29);
		GPIOC->BSRR &= ~(1<<13);
		delay(20);
	}
}

