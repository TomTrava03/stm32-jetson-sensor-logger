# STM32 Sensor Jetson Logger

An embedded-to-edge learning project that collects environmental data on an
STM32 board and forwards it to an NVIDIA Jetson for logging and simple rain
likelihood experiments.

The project currently includes working STM32 bring-up, UART telemetry, and a
DHT11 driver. The Jetson logger and prediction model are planned next. A BMP280
pressure sensor may be added later to improve the available weather features.

## Data flow

`DHT11 -> STM32 -> USART2 / USB VCP -> Jetson logger -> dataset -> rain model`

The future model is intended as a portfolio and learning experiment, not as a
reliable weather forecasting system.

## Hardware

- ST NUCLEO-F446RE with STM32F446RET6
- KY-015 module with DHT11 temperature and humidity sensor
- NVIDIA Jetson Orin Nano Super for the planned edge pipeline
- Optional future BMP280 pressure sensor

DHT11 connections:

| DHT11 | NUCLEO-F446RE |
| --- | --- |
| `-` | `GND` |
| `+` | `3V3` |
| `S` | `PA9` |

## Current firmware

- STM32 HAL project generated with STM32CubeIDE
- 180 MHz system clock
- LED heartbeat and user-button interrupt with debounce
- USART2 telemetry over the ST-LINK Virtual COM Port at 115200 baud
- TIM6 microsecond time base
- DHT11 single-wire protocol, timeouts, checksum validation, conversion, and
  minimum sampling interval
- Separate sensor protocol and STM32-specific platform layers

## Directory overview

```text
.
|-- firmware/
|   `-- nucleo_f446re/stm32_sensor_node/
|       |-- Inc/                    STM32 application headers
|       |-- Src/                    STM32 application and generated sources
|       |-- platform/               STM32/HAL-specific DHT11 operations
|       `-- sensor_drivers/         Hardware-independent DHT11 protocol
|-- docs/
|   `-- learning-concepts-index.md  Topics encountered during development
`-- jetson/                         Planned edge-side software
    |-- logger/                     Planned serial collector and data logger
    `-- rain_model/                 Planned rain-likelihood experiment
```

The `jetson/` directories describe the intended next phase and are not present
in the repository yet.

## Build and run

1. Import `firmware/nucleo_f446re/stm32_sensor_node` into STM32CubeIDE.
2. Build the `Debug` configuration.
3. Connect the NUCLEO board through its ST-LINK USB port.
4. Flash or debug the firmware.
5. Open the ST-LINK Virtual COM Port at `115200 8N1` to read telemetry.

## Next milestones

1. Integrate DHT11 readings into the existing UART telemetry.
2. Implement the Jetson serial logger and structured local storage.
3. Collect and validate a time-series dataset.
4. Add BMP280 pressure measurements.
5. Train and evaluate a simple rain-likelihood model.
