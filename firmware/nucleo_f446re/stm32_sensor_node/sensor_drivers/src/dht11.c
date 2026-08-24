#include "dht11.h"
#include "dht11_port.h"

#include <stdbool.h>

#define DHT11_START_LOW_US 20000U
#define DHT11_RELEASE_WAIT_US 30U
#define DHT11_RESPONSE_TIMEOUT_US 100U
#define DHT11_BIT_SAMPLE_US 40U
#define DHT11_BIT_TIMEOUT_US 100U
#define DHT11_BITS_PER_BYTE 8U
#define DHT11_DECIMAL_SCALE 10U
#define DHT11_TEMPERATURE_SIGN_MASK 0x80U
#define DHT11_TEMPERATURE_DECIMAL_MASK 0x7FU
#define DHT11_MIN_REQUEST_INTERVAL_MS 2000U

static bool dht11_is_initialized;
static bool dht11_has_previous_request;
static uint32_t dht11_last_request_ms;

DHT11_Status dht11_init(void)
{
	int ret;

	if (dht11_is_initialized)
		return DHT11_OK;

	ret = dht11_port_tim6_init();
	if (ret != 0)
		return DHT11_FAILED_OPERATION;

	dht11_is_initialized = true;

	return DHT11_OK;
}

static void dht11_send_start_signal(void)
{
	dht11_port_downgrade_bus();
	dht11_port_wait_us(DHT11_START_LOW_US);

	dht11_port_release_bus();
	dht11_port_wait_us(DHT11_RELEASE_WAIT_US);
}

static bool dht11_wait_for_bus_level(
		DHT11_BUS_LEVEL expected_level, uint16_t timeout_us)
{
	uint16_t start_us;
	uint16_t elapsed_us;

	start_us = (uint16_t)dht11_port_read_counter();

	if (timeout_us == 0U)
		return dht11_port_read_bus_level() == expected_level;

	while (dht11_port_read_bus_level() != expected_level) {
		elapsed_us = (uint16_t)(dht11_port_read_counter() - start_us);
		if (elapsed_us >= timeout_us)
			return false;
	}
	return true;
}

static DHT11_Status dht11_wait_for_response(void)
{
	dht11_send_start_signal();
	if (!dht11_wait_for_bus_level(DHT11_LOW_LEVEL, DHT11_RESPONSE_TIMEOUT_US))
		return DHT11_REQUEST_TIMEOUT;

	if (!dht11_wait_for_bus_level(DHT11_HIGH_LEVEL, DHT11_RESPONSE_TIMEOUT_US))
		return DHT11_RESPONSE_TIMEOUT;

	if (!dht11_wait_for_bus_level(DHT11_LOW_LEVEL, DHT11_RESPONSE_TIMEOUT_US))
		return DHT11_RESPONSE_TIMEOUT;

	return DHT11_OK;
}

static DHT11_Status dht11_read_bit(uint8_t *bit)
{
	if (!bit)
		return DHT11_INVALID_ARGUMENT;

	if (!dht11_wait_for_bus_level(DHT11_HIGH_LEVEL, DHT11_BIT_TIMEOUT_US))
		return DHT11_TRANSMISSION_TIMEOUT;

	dht11_port_wait_us(DHT11_BIT_SAMPLE_US);

	*bit = dht11_port_read_bus_level() == DHT11_HIGH_LEVEL ? 1U : 0U;

	if (!dht11_wait_for_bus_level(DHT11_LOW_LEVEL, DHT11_BIT_TIMEOUT_US))
		return DHT11_TRANSMISSION_TIMEOUT;

	return DHT11_OK;
}

static DHT11_Status dht11_read_byte(uint8_t *byte)
{
	DHT11_Status status;
	uint8_t value;
	uint8_t bit;
	uint8_t bit_index;

	if (!byte)
		return DHT11_INVALID_ARGUMENT;

	value = 0U;
	bit_index = 0U;

	while (bit_index < DHT11_BITS_PER_BYTE) {
		status = dht11_read_bit(&bit);
		if (status != DHT11_OK)
			return status;

		value <<= 1U;
		value |= bit;

		bit_index++;
	}

	*byte = value;

	return DHT11_OK;
}

static DHT11_Status dht11_validate_checksum(const dht11_msg_t *message)
{
	uint8_t calculated_checksum;

	if (!message)
		return DHT11_INVALID_ARGUMENT;

	calculated_checksum = (uint8_t)(message->raw_dec_humidity +
			message->raw_dec_temperature + message->raw_int_humidity
			+ message->raw_int_temperature);

	if (message->checksum != calculated_checksum)
		return DHT11_CHECKSUM_ERROR;

	return DHT11_OK;
}

static DHT11_Status dht11_read_message(dht11_msg_t *message)
{
	DHT11_Status status;
	dht11_msg_t received;

	if (!message)
		return DHT11_INVALID_ARGUMENT;

	status = dht11_wait_for_response();
	if (status != DHT11_OK)
		return status;

	status = dht11_read_byte(&received.raw_int_humidity);
	if (status != DHT11_OK)
		return status;

	status = dht11_read_byte(&received.raw_dec_humidity);
	if (status != DHT11_OK)
		return status;

	status = dht11_read_byte(&received.raw_int_temperature);
	if (status != DHT11_OK)
		return status;

	status = dht11_read_byte(&received.raw_dec_temperature);
	if (status != DHT11_OK)
		return status;

	status = dht11_read_byte(&received.checksum);
	if (status != DHT11_OK)
		return status;

	status = dht11_validate_checksum(&received);
	if (status != DHT11_OK)
		return status;

	*message = received;

	return DHT11_OK;
}

static DHT11_Status dht11_convert_message(const dht11_msg_t *message,
		dht11_data_t *data)
{
	dht11_data_t converted;

	if (!message || !data)
		return DHT11_INVALID_ARGUMENT;

	converted.humidity_dec =
			message->raw_int_humidity * DHT11_DECIMAL_SCALE
			+ message->raw_dec_humidity;
	converted.temperature_dec =
			(int16_t)(message->raw_int_temperature * DHT11_DECIMAL_SCALE +
			(message->raw_dec_temperature
			& DHT11_TEMPERATURE_DECIMAL_MASK));

	if ((message->raw_dec_temperature & DHT11_TEMPERATURE_SIGN_MASK) != 0U)
		converted.temperature_dec = -converted.temperature_dec;

	*data = converted;

	return DHT11_OK;
}

DHT11_Status dht11_read(dht11_data_t *data)
{
	DHT11_Status status;
	dht11_msg_t message;
	uint32_t now_ms;

	if (!data)
		return DHT11_INVALID_ARGUMENT;

	if (!dht11_is_initialized)
		return DHT11_FAILED_OPERATION;

	now_ms = dht11_port_read_tick_ms();
	if (dht11_has_previous_request &&
		(uint32_t)(now_ms - dht11_last_request_ms)
		< DHT11_MIN_REQUEST_INTERVAL_MS)
		return DHT11_REQUEST_TOO_EARLY;

	dht11_last_request_ms = now_ms;
	dht11_has_previous_request = true;

	status = dht11_read_message(&message);
	if (status != DHT11_OK)
		return status;

	status = dht11_convert_message(&message, data);
	if (status != DHT11_OK)
		return status;

	return DHT11_OK;
}
