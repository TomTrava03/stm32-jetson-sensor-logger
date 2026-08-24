#include <errno.h>

#include "dht11_port.h"
#include "main.h"
#include "tim.h"

int dht11_port_tim6_init(void)
{
	HAL_StatusTypeDef hal_status = HAL_TIM_Base_Start(&htim6);

	if (hal_status != HAL_OK) {
		switch (hal_status) {
		case HAL_ERROR:
			return -EIO;
		case HAL_BUSY:
			return -EBUSY;
		case HAL_TIMEOUT:
			return -ETIMEDOUT;
		default:
			return -EIO;
		}
	}
	dht11_port_release_bus();
	return 0;
}

void dht11_port_release_bus(void)
{
	HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET);
}

void dht11_port_downgrade_bus(void)
{
	HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_RESET);
}

DHT11_BUS_LEVEL dht11_port_read_bus_level(void)
{
	if (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin) == GPIO_PIN_SET) {
		return DHT11_HIGH_LEVEL;
	}
	return DHT11_LOW_LEVEL;
}

uint32_t dht11_port_read_counter(void)
{
	return __HAL_TIM_GET_COUNTER(&htim6);
}

void dht11_port_wait_us(uint16_t time_us)
{
	uint16_t start_us;

	if (time_us == 0U)
		return;

	start_us = (uint16_t)dht11_port_read_counter();

	while ((uint16_t)(dht11_port_read_counter() - start_us) < time_us) {}
}

uint32_t dht11_port_read_tick_ms(void)
{
	return HAL_GetTick();
}
