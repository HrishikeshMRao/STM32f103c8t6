#include "stm32f103xb.h"

#define SystemClock 72000000
#define baudrate 115200

void USART1_Init(void) {

		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	    GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_MODE9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF9);//reset the pins first

	    GPIOA->CRH |= GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;

    	// Configure PA10 (RX) as floating input
	    GPIOA->CRH |= GPIO_CRH_CNF10_0;

	    // Configure USART1
	    USART1->CR1 &= ~(USART_CR1_UE);  // Disable USART1 during configuration

	    //uint32_t baud = (uint32_t)(SystemClock / baudrate);//calculate the baud rate

	    // Set baud rate to 115200 (assuming PCLK2 = 72MHz)
	    USART1->BRR = 0x251;  // Integer part of the division
	    USART1->CR1 |= USART_CR1_TE;  // Enable transmitter
	    USART1->CR1 |= USART_CR1_UE;  // Enable USART1
}

void SystemClock_Config(void) {
    // Enable HSE (High-Speed External) oscillator (8MHz on Blue Pill)
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Configure PLL
    RCC->CFGR |= RCC_CFGR_PLLSRC;  // Use HSE as PLL source
    RCC->CFGR |= RCC_CFGR_PLLMULL9; // PLL multiplication factor = 9 (8MHz * 9 = 72MHz)

    // Set PLL as the system clock source
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

    RCC->CFGR &= ~RCC_CFGR_PPRE2;

}

void USART1_SendChar(char c) {
    // Wait until the Transmit Data Register (TDR) is empty
    while (!(USART1->SR & USART_SR_TXE));

    // Send the character
    USART1->DR = c;
}

void USART1_SendString(const char* str) {
    while (*str != '\0') {
        USART1_SendChar(*str);
        str++;
    }
}

int main(void) {
	SystemClock_Config();
    USART1_Init();
    int n=0;
    char str[64];
    while (1) {
        // Send a string repeatedly
    	sprintf(str,"%d",n);
    	strcat(str, "\n");
        USART1_SendString(str);
        n++;
        // Add a delay (you might want to use a proper delay function)
        for (volatile int i = 0; i < 1000000; i++);
    }
}
