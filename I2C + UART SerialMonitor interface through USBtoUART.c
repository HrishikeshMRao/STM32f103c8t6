#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "stm32f103xb.h"

#define buffer 20
#define IMU_address 0b0101000
#define SystemClock 72000000
#define baudrate 115200

void init_i2c2(void);

uint8_t i2c_buff[buffer];
uint32_t n = 0;
char str[64];
char *data1 = "1st loop going\n";
char *data2 = "2nd loop going\n";
char *data3 = "3rd loop going\n";

void USART1_Init(void) {

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_MODE9 | GPIO_CRH_CNF10
			| GPIO_CRH_CNF9); //reset the pins first

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
	while (!(RCC->CR & RCC_CR_HSERDY))
		;

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY))
		;

	// Configure PLL
	RCC->CFGR |= RCC_CFGR_PLLSRC;  // Use HSE as PLL source
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // PLL multiplication factor = 9 (8MHz * 9 = 72MHz)

	// Set PLL as the system clock source
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL)
		;

	RCC->CFGR &= ~RCC_CFGR_PPRE2;

}

void USART1_SendChar(char c) {
	// Wait until the Transmit Data Register (TDR) is empty
	while (!(USART1->SR & USART_SR_TXE))
		;

	// Send the character
	USART1->DR = c;
}

void USART1_SendString(const char *str) {
	while (*str != '\0') {
		USART1_SendChar(*str);
		str++;
	}
}

void init_i2c2(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C2->CR2 |= 36;
	I2C2->CCR |= 180;
	I2C2->TRISE |= 37;
	I2C2->CR1 |= I2C_CR1_ACK;
	I2C2->OAR1 |= 14 << 1;
	I2C2->OAR1 |= 1 << IMU_address;
	GPIOB->CRH |= GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF11
			| GPIO_CRH_MODE11; //Alt func output open drain
	I2C2->CR1 |= I2C_CR1_PE; //enable peripheral done last
}

void i2c_read(uint8_t device_address) {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	I2C2->CR2 |= I2C_CR2_DMAEN;
	I2C2->CR1 |= I2C_CR1_ACK;
	DMA1_Channel5->CMAR = (uint32_t) i2c_buff;
	DMA1_Channel5->CPAR = (uint32_t) &I2C2->DR;
	DMA1_Channel5->CNDTR = 5;
	DMA1_Channel5->CCR |= DMA_CCR_TCIE | DMA_CCR_MINC | DMA_CCR_EN;
	I2C2->CR1 |= I2C_CR1_START;
	while (!(I2C2->SR1 & I2C_SR1_SB)) {
		USART1_SendString(data1);
	}
	I2C2->DR = device_address;
	while (!(I2C2->SR1 & I2C_SR1_ADDR)) {
		USART1_SendString(data1);
	}
	uint32_t temp = I2C2->SR2;
	while ((DMA1->ISR & DMA_ISR_TCIF5) == 0) {
		USART1_SendString(data1);
	}
}

int main(void) {
	SystemClock_Config();
	init_i2c2();
	USART1_Init();
	while (1) {
		i2c_read(IMU_address);
		// Send a string repeatedly
		sprintf(str, "%d", n);
		strcat(str, "\n");
		USART1_SendString(str);
	    n++;
		// Add a delay (you might want to use a proper delay function)
	    for (volatile int i = 0; i < 1000000; i++);
	}
}
