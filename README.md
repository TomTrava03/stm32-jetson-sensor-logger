# STM32 Jetson Sensor Logger

Progetto didattico e portfolio basato su NUCLEO-F446RE, STM32F446RET6 e
NVIDIA Jetson Orin Nano Super.

Stato attuale: preparazione dell'ambiente Windows. STM32CubeIDE 2.2.0 è
installato. La scheda non è ancora collegata e non esiste ancora firmware.

## Roadmap

1. Installare e verificare gli strumenti ufficiali ST su Windows.
2. Collegare la NUCLEO da sola e verificare ST-LINK e Virtual COM Port.
3. Creare con CubeMX il progetto minimo per NUCLEO-F446RE.
4. Imparare build, flash e debug con un programma vuoto generato da ST.
5. Aggiungere LED e pulsante integrati, un passo alla volta.
6. Inviare un contatore testuale tramite USART2 e Virtual COM Port.
7. Solo dopo il bring-up, separare application, platform e transport.
8. Identificare e verificare un sensore prima di qualsiasi cablaggio.
9. Aggiungere il driver del sensore.
10. Creare successivamente collector Jetson, analisi Python e cartelle dati.

Le sottocartelle verranno create soltanto quando inizierà la milestone che le
usa, spiegando prima il loro scopo.
