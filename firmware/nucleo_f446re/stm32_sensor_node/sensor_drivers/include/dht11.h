#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

typedef enum {
	DHT11_OK,
	DHT11_FAILED_OPERATION,
	DHT11_REQUEST_TOO_EARLY,
	DHT11_REQUEST_TIMEOUT,
	DHT11_TRANSMISSION_TIMEOUT,
	DHT11_RESPONSE_TIMEOUT,
	DHT11_CHECKSUM_ERROR,
	DHT11_INVALID_ARGUMENT
} DHT11_Status;

typedef struct {
	uint8_t raw_int_humidity;
	uint8_t raw_dec_humidity;
	uint8_t raw_int_temperature;
	uint8_t raw_dec_temperature;
	uint8_t checksum;
} dht11_msg_t;

typedef struct {
	int16_t temperature_dec;
	uint16_t humidity_dec;
} dht11_data_t;

DHT11_Status dht11_init(void);

DHT11_Status dht11_read(dht11_data_t *data);

#endif /* DHT11_H */
