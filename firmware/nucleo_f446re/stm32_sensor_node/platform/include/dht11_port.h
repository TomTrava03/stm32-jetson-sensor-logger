#ifndef DHT11_PORT_H
#define DHT11_PORT_H

#include <stdint.h>

typedef enum {
	DHT11_HIGH_LEVEL,
	DHT11_LOW_LEVEL,
} DHT11_BUS_LEVEL;

/*
 * @brief: start TIM6 and release bus
 */
int dht11_port_tim6_init(void);

/*
 * @brief: PA9 set S pin to 0
 */
void dht11_port_downgrade_bus(void);

/*
 * @brief: stop open-drain, pin S to HIGH
 */
void dht11_port_release_bus(void);

/*
 * @brief: return bus level (HIGH or LOW)
 */
DHT11_BUS_LEVEL dht11_port_read_bus_level(void);

/*
 * @brief: return TIM6 in microseconds
 */
uint32_t dht11_port_read_counter(void);

/*
 * @brief: wait time (in microseconds), 0 to 65535
 */
void dht11_port_wait_us(uint16_t time_us);

/*
 * @brief: returns time (in milliseconds) to avoid too close requests
 */
uint32_t dht11_port_read_tick_ms(void);

#endif /* DHT11_PORT_H */
