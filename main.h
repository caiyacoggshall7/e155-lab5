// main.h
//Caiya Coggshall and Josh Brake
//ccoggshall@g.hmc.edu & jdbrake@hmc.edu
//10/5/25 & 10/31/22 
// This code is the header file for my main

#ifndef MAIN_H
#define MAIN_H

#include "lib/STM32L432KC.h"
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Custom defines
///////////////////////////////////////////////////////////////////////////////

//#define LED_PIN PA5
//#define BUTTON_PIN PA4
#define PPR       408 // datasheet for motor says 120 PPR but Kavi sent out new for 408
#define PIN_A      PA1 // !!
#define PIN_B      PA2 // !!
#define DELAY_TIM TIM2

void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void computevelocity(void); 

#endif // MAIN_H