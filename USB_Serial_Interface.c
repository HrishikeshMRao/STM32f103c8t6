#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "string.h"
#include "stdlib.h"


#define buffer 20
#define IMU_address 0b0101000

void SystemClock_Config(void);
void init_i2c2(void);
void Error_Handler(void);

uint8_t i2c_buff[buffer];
char* data1= "1st loop going\n";
char* data2= "2nd loop going\n";
char* data3= "3rd loop going\n";

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

void init_i2c2(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C2->CR2 |= 36;
	I2C2->CCR |= 180;
	I2C2->TRISE |= 37;
	I2C2->CR1 |= I2C_CR1_ACK;
	I2C2->OAR1 |= 14<<1;
	I2C2->OAR1 |= 1<<IMU_address;
	GPIOB->CRH |= GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE11;//Alt func output open drain
	I2C2->CR1 |= I2C_CR1_PE; //enable peripheral done last
}

void i2c_read(uint8_t device_address)
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	I2C2->CR2 |= I2C_CR2_DMAEN;
	I2C2->CR1 |= I2C_CR1_ACK;
	DMA1_Channel5->CMAR = (uint32_t)i2c_buff;
	DMA1_Channel5->CPAR = (uint32_t)&I2C2->DR;
	DMA1_Channel5->CNDTR = 5;
	DMA1_Channel5->CCR |= DMA_CCR_TCIE | DMA_CCR_MINC | DMA_CCR_EN;
	I2C2->CR1 |= I2C_CR1_START;
	while(!(I2C2->SR1 & I2C_SR1_SB))
	{
		CDC_Transmit_FS((uint8_t*)data1, strlen(data1));
		HAL_Delay(500);
	}
	I2C2->DR = device_address;
	while(!(I2C2->SR1 & I2C_SR1_ADDR))
	{
		CDC_Transmit_FS((uint8_t*)data2, strlen(data2));
		HAL_Delay(500);
	}
	uint32_t temp = I2C2->SR2;
	while((DMA1->ISR & DMA_ISR_TCIF5)==0)
	{
		CDC_Transmit_FS((uint8_t*)data3, strlen(data3));
		HAL_Delay(500);
	}
}

int main(void) {
	//RCC->APB2ENR |= (1 << 4);
	//GPIOC->CRH |= ((1 << 20) | (1 << 21));
	//GPIOC->CRH &= ~((1 << 22) | (1 << 23));
	HAL_Init();
	SystemClock_Config();
	MX_USB_DEVICE_Init();
	init_i2c2();
	while (1)
	{
		i2c_read(IMU_address);
		CDC_Transmit_FS((uint8_t*)data3, strlen(data3));
	}
}


void Error_Handler(void) {
	__disable_irq();
}
