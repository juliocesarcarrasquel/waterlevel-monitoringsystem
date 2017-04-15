# Development of a Tank water level & temperature monitoring solution based on the STM32L476RG Microcontroller

Università degli Studi di Roma La Sapienza
Dipartimento di Ingegneria Informatica, Automatica 
e Gestionale Antonio Ruberti (DIAG)
Contact: carrasquelgamez.1726154@studenti.uniroma1.it
Last update: April 2017

STMicroelectronics provides the STM32L4 family of low-power microcontrollers based on the ARM Cortex M4 architecture. This project particularly works with the the STM32L476RG microcontroller as the core piece for the management of a tank water-level & temperature monitoring system. For the detection of the level of water in a given tank is used the HCSR04 ultra-sonic ranging device whereas for the temperature is used the water-proof DS18B20 thermometer which goes immersed below the water. The system also includes an U-Blox NEO-6M GPS receiver which keeps track of the location where the system is operating. In order to carry out the development tasks it was used the STM32CubeMX framework and the System Workbench 4 IDE which provide an easy and professional environment. On the other hand, it was taken into advantage the several microcontroller capabilities such as the different clock sources, the UART interfaces and the management of different general input/output ports among others in order to make a correct system configuration.
