# Indice dei concetti da approfondire

## Architettura del progetto

- Separazione tra codice generato e codice scritto a mano
- Blocchi `USER CODE` di STM32CubeMX
- Layer `application`
- Layer `platform`
- Layer `transport`
- Layer `sensor_drivers`
- Header pubblici e sorgenti privati
- Cartelle `include` e `src`
- Source Location di STM32CubeIDE
- Include paths del compilatore
- Configurazioni Debug e Release
- Dipendenze tra layer
- Hardware abstraction layer
- Porting layer

## Linguaggio C

- File header `.h`
- File sorgente `.c`
- Include guard
- Direttiva `#include`
- Dichiarazione e definizione
- Dichiarazioni duplicate tra header e file sorgente
- Differenza tra prototipo terminato da `;` e corpo di funzione
- Prototipo di funzione
- Lista parametri `(void)`
- Valore di ritorno
- Convenzione `0` per successo e valore negativo per errore
- Convenzione Linux degli error code negativi
- Funzioni comando con ritorno error-code integer
- Funzioni predicato con ritorno booleano
- Traduzione degli errori tra layer di astrazione
- Codici errno `EIO`, `EBUSY` e `ETIMEDOUT`
- Header standard `<errno.h>`
- Differenza tra variabile globale `errno` e codici errno negativi
- Return diretto di un codice errno negativo
- Header di sistema tra parentesi angolari
- Header locali tra virgolette
- Codici errno standard e simboli non standard
- Costanti simboliche al posto dei magic number
- Codice irraggiungibile dopo `return`
- Differenza tra `return` e `break` in uno `switch`
- Allineamento di `case` e `default` con `switch`
- Operatore address-of `&`
- Oggetto e puntatore a oggetto
- Parametro handle passato per indirizzo
- Variabili locali in minuscolo
- Identificatori definiti dal progetto e simboli imposti da HAL/CubeMX
- Nomi ufficiali delle periferiche e nomi C definiti dall'applicazione
- Graffe delle definizioni di funzione su una nuova riga
- Graffe delle strutture di controllo sulla stessa riga
- Spazio dopo `if`, `for`, `while` e `switch`
- Return anticipato quando non serve cleanup condiviso
- Eliminazione di `else` dopo un `return`
- Uscita centralizzata e `goto` quando serve cleanup condiviso
- `struct`
- `enum`
- Enum tag
- `typedef`
- Alias di tipo
- Tipi interi a dimensione esplicita
- Header standard `<stdint.h>`
- Tipi signed e unsigned
- `uint8_t`
- `int8_t`
- `uint16_t`
- `int16_t`
- `uint32_t`
- Aritmetica unsigned e overflow
- Variabili globali definite negli header
- Errori di definizione multipla al linking
- Qualificatore `static`
- Qualificatore `volatile`
- Cast espliciti
- Rappresentazione fixed-point
- Unita espresse nel nome di campo o variabile
- Unita espresse nei nomi dei parametri di funzione
- Tipo del parametro coerente con il range supportato
- Temperatura in decimi di grado Celsius
- Umidita in decimi di percentuale
- Buffer di caratteri
- `sizeof`
- `snprintf`
- Controllo della lunghezza prodotta da `snprintf`
- Sequenza `\r\n`
- Prefissi coerenti nei simboli pubblici
- Tab reali larghi 8
- Commenti C `/* ... */`
- Differenza tra errore di compilazione e di linking
- Translation unit
- File oggetto `.o`

## STM32CubeMX e codice generato

- File `.ioc`
- Generazione del codice
- Preservazione dei blocchi `USER CODE`
- `MX_GPIO_Init`
- `MX_USART2_UART_Init`
- `MX_TIM6_Init`
- `SystemClock_Config`
- File `main.h`
- Handle delle periferiche
- Rigenerazione e regressioni
- Driver STM32 HAL generati su richiesta

## STM32 HAL e BSP

- STM32 Hardware Abstraction Layer
- Board Support Package
- `HAL_Init`
- `HAL_StatusTypeDef`
- `HAL_OK`
- `HAL_ERROR`
- `HAL_BUSY`
- `HAL_TIMEOUT`
- Valori non negativi di `HAL_StatusTypeDef`
- Confronto esplicito con `HAL_OK`
- Mapping degli stati HAL in errori del port
- Isolamento di `HAL_StatusTypeDef` nel layer platform
- `TIM_HandleTypeDef` e puntatore a handle
- `BSP_LED_Init`
- `BSP_LED_Toggle`
- `BSP_PB_Init`
- Differenza tra inizializzazione e avvio di una periferica

## Clock e temporizzazione

- HSI
- HSE bypass
- PLL
- SYSCLK a 180 MHz
- HCLK a 180 MHz
- PCLK1 a 45 MHz
- Clock timer APB1 a 90 MHz
- Prescaler
- Periodo del contatore
- Auto-reload
- Timer a 1 MHz
- Tick da 1 microsecondo
- Timer a 16 bit
- Overflow da `65535` a `0`
- Calcolo wrap-safe del tempo trascorso
- Aritmetica modulare unsigned a 16 bit
- Precondizione di timer gia avviato
- SysTick
- `HAL_GetTick`
- `SystemCoreClock`
- Scheduling non bloccante
- Intervalli temporali unsigned
- `TIM6`
- `htim6`
- `HAL_TIM_Base_Start`
- Macro `__HAL_TIM_GET_COUNTER`
- Differenza tra macro HAL e funzione HAL
- Busy wait a microsecondi
- Differenza tra millisecondi e microsecondi

## GPIO

- Pin STM32 e pin Arduino
- Arduino D8 e STM32 PA9
- Arduino D13 e STM32 PA5
- Arduino D0/D1 e STM32 PA3/PA2
- GPIO input
- GPIO output
- Output push-pull
- Output open-drain
- Stato high-impedance
- Pull-up esterna
- Pull-up interna
- `GPIO_NOPULL`
- Livello logico alto e basso
- GPIO Output Data Register
- GPIO Input Data Register
- Lettura dell'Input Data Register mentre il GPIO e configurato come output
- `HAL_GPIO_WritePin`
- Parametri porta, pin e stato di `HAL_GPIO_WritePin`
- `HAL_GPIO_ReadPin`
- `GPIO_PIN_SET`
- `GPIO_PIN_RESET`
- `GPIO_PIN_SET` come rilascio di un'uscita open-drain
- `GPIO_PIN_RESET` come pilotaggio basso di un'uscita open-drain
- Differenza tra livello scritto e livello fisico su open-drain
- Mapping esplicito tra enum applicativi e valori HAL
- Macro `DHT11_DATA_Pin`
- Macro `DHT11_DATA_GPIO_Port`
- User Label GPIO di STM32CubeMX
- Generazione delle macro `_Pin` e `_GPIO_Port` da un User Label
- Alimentazione logica a 3,3 V
- Massa comune

## Interrupt e pulsante

- EXTI
- Interrupt su PC13
- `HAL_GPIO_EXTI_Callback`
- NVIC
- Callback HAL
- Debounce temporale
- Contatore delle pressioni
- Condivisione di dati tra interrupt e main loop
- Uso di `volatile`

## LED heartbeat

- Heartbeat non bloccante
- Intervallo di toggle
- Periodo completo di lampeggio
- Variabile di ultimo aggiornamento
- Test visivo di regressione

## USART e Virtual COM Port

- USART2
- UART asincrona
- PA2 USART2 TX
- PA3 USART2 RX
- ST-LINK Virtual COM Port
- Baud rate 115200
- Configurazione 8N1
- Assenza di flow control
- Oversampling 16
- `UART_HandleTypeDef`
- `huart2`
- `HAL_UART_Transmit`
- Trasmissione bloccante
- Timeout di trasmissione
- Telemetria testuale
- Contatore monotono
- Tera Term
- Terminale seriale di Eclipse

## DHT11 e KY-015

- Sensore DHT11
- Modulo KY-015
- Pin `-`, `+` e `S`
- Alimentazione a 3,3 V
- Resistenza di pull-up integrata
- Misura di circa 10,15 kOhm
- Bus digitale bidirezionale a singolo filo
- Linea inattiva alta
- Pilotaggio open-drain
- Rilascio del bus
- Segnale di start del master
- Risposta del sensore
- Timing degli impulsi
- Frame da 40 bit
- Trasmissione MSB first
- Byte intero e decimale dell'umidita
- Byte intero e decimale della temperatura
- Byte di checksum
- Decodifica dei dati raw
- Checksum sugli ultimi 8 bit della somma
- Intervallo minimo tra letture
- Stabilizzazione dopo l'accensione
- Timeout di risposta
- Timeout di trasmissione
- Sensore scollegato
- Lettura richiesta troppo presto
- Dato non valido
- Codici di stato del driver
- Separazione tra protocollo e accesso hardware

## API del driver DHT11

- `dht11_msg_t`
- `dht11_data_t`
- Stato di operazione DHT11
- Dati raw e dati decodificati
- Argomento non valido
- Richiesta troppo anticipata
- Errore di checksum
- Contratto di `dht11_port`
- Inizializzazione del port
- Forzatura del bus a livello basso
- Rilascio del bus
- Lettura del livello del bus
- Lettura del contatore a microsecondi
- Attesa wrap-safe a microsecondi
- Header del driver indipendente da HAL
- Implementazione STM32 confinata nel layer platform

## Build, debug e verifica

- Clean build
- Build incrementale
- Errori e warning del compilatore
- Breakpoint in `main`
- Resume con GDB
- Hardware breakpoint
- Expressions view
- Osservazione di `SystemCoreClock`
- Flash tramite ST-LINK
- Reset e ripartenza
- Test di regressione
- Test hardware-in-the-loop
- Verifica della compilazione delle nuove source directory
- Verifica dei file oggetto generati
- Controllo di heartbeat, pulsante e telemetria
- Uso del multimetro in modalita resistenza
- Misura tra segnale e alimentazione

## Git e file di progetto

- Repository Windows condiviso con WSL
- Working tree
- Staging area
- Commit atomici
- `git status --short`
- `git diff --check`
- `git diff --cached --check`
- File generati e file locali
- `.gitignore`
- `.gitattributes`
- Normalizzazione LF e CRLF
- File `.cproject`
- File `.settings`
- Directory di build Debug e Release
- File locali di STM32CubeIDE
- `safe.directory`
- Remote SSH GitHub
- Branch `main`

## Hardware e strumenti

- NUCLEO-F446RE
- STM32F446RET6
- ST-LINK/V2-1
- Firmware ST-LINK
- STM32CubeIDE
- STM32CubeMX standalone
- STM32CubeProgrammer
- GNU Arm Embedded Toolchain
- GDB per `arm-none-eabi`
- Virtual COM Port
- Connettori Arduino
- Connettori Morpho
- Pin maschio e femmina
- Jumper femmina-maschio
- Alimentazione USB
- Assenza di alimentazioni esterne simultanee
- Verifica di cortocircuiti e surriscaldamento
