// Taken from button_interrupt.c and modified with interrupts for a Quadrature Encoder
// Caiya Coggshall
// ccoggshall@hmc.edu
// 10/8/25

#include "main.h"
int    directioncounter; // + means clockwise and - means counterclockwise


int main(void) {

    // Enable button as input
    gpioEnable(GPIO_PORT_A);
    pinMode(PIN_A, GPIO_INPUT);
    pinMode(PIN_B, GPIO_INPUT);

    // Need these to work
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD1, 0b10); // Set PA1 as pull-down
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD2, 0b10); // Set PA2 as pull-down

    // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);


    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // 2. Configure EXTICR for the input button interrupt
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI1, 0b000); // Select PA1
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI2, 0b000); // Select PA2 

    // Enable interrupts globally
    __enable_irq();

    // FOR PIN_A
    // Configure interrupt for rising and falling edge of GPIO pin for button
    EXTI->IMR1 |= (1 << gpioPinOffset(PIN_A)); // Configure the mask bit
    EXTI->RTSR1 |= (1 << gpioPinOffset(PIN_A));// Enable rising edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(PIN_A));// Enable falling edge trigger
    
    NVIC->ISER[0] |= (1 << EXTI1_IRQn); // Turn on EXTI interrupt in NVIC_ISER

    
    // FOR PIN_B
    // Configure interrupt for rising and falling edge of GPIO pin for button
    EXTI->IMR1 |= (1 << gpioPinOffset(PIN_B)); // Configure the mask bit
    EXTI->RTSR1 |= (1 << gpioPinOffset(PIN_B));// Enable rising edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(PIN_B));// Enable falling edge trigger
    
    NVIC->ISER[0] |= (1 << EXTI2_IRQn); // Turn on EXTI interrupt in NVIC_ISER
  

    // Interrupts
    while(1){  
        delay_millis(TIM2, 1000);
        computevelocity();
    }

    // Manually Polling
    //int volatile cur_button_state = digitalRead(PIN_A);
    //int volatile prev_button_state = cur_button_state;

    // while(1){
    //    prev_button_state = cur_button_state;
    //    cur_button_state = digitalRead(PIN_A);
    //    if ((prev_button_state == 1) && (cur_button_state == 0)) {
    //        digitalWrite(PIN_A, led_state);
    //    }
    //    delay_millis(TIM2, 1000);
    //}


}

// THIS IS THE INTERUPT for PIN A so on rising/falling edge
void EXTI1_IRQHandler(void){
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(PIN_A))){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(PIN_A));

        if (digitalRead(PIN_A) == digitalRead(PIN_B)) {
            directioncounter--; // CCW
        }

        else {//if (digitalRead(PIN_A) != digitalRead(PIN_B)) {
            directioncounter++; // CW 
        }

        //else if (!digitalRead(PIN_A) && digitalRead(PIN_B)) {
        //    directioncounter++; // CW 
        //}

        //else if (!digitalRead(PIN_A) && !digitalRead(PIN_B)) {
        //    directioncounter--; // CCW 
        //}
        //else {
        //}
    }

    
}


// THIS IS THE INTERUPT for PIN B so on rising/falling edge
void EXTI2_IRQHandler(void){
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(PIN_B))){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(PIN_B));

        if (digitalRead(PIN_B) == digitalRead(PIN_A)) {
            directioncounter++; // CW
        }

        else { //if (digitalRead(PIN_B) != digitalRead(PIN_A)) {
            directioncounter--; // CCW 
        }
  
        //else if (!digitalRead(PIN_B) && digitalRead(PIN_A)) {
        //    directioncounter--; // CCW 
        //}

        //else if (!digitalRead(PIN_B) && !digitalRead(PIN_A)) {
        //    directioncounter++; // CW 
        //}
        //else {
        //}
    }
   
}



// Find velocity once a second
void computevelocity(void){
    float revpersec = (((float)directioncounter) / (PPR*4) ); // divide by 4 because counting on rising and falling edge of both pins
    // doing float / int to get a float bc rev could be not an int, but you want directioncounter to be a integer before
    
    printf("Direction counter: %d \n", directioncounter);
    // clear counter
    directioncounter = 0;
  
    
    printf("The motor is going at %f revolutions per second \n", revpersec);
    // %f is for decimal float, %d is an signed decimal int
}


