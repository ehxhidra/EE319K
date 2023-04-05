// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// 8/24/2022

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "Fifo.h"
#include "UART1.h"
#include "../inc/tm4c123gh6pm.h"


#define CR  0x0D

// Initialize UART1
// Baud rate is 2000 bits/sec
void UART1_Init(void){
  // write this
    int32_t delay = 0;
    SYSCTL_RCGCUART_R |= 0x00000002;  // activate UART1
    SYSCTL_RCGCGPIO_R |= 0x00000004;  // activate port C
    delay = SYSCTL_RCGCADC_R;
    delay = SYSCTL_RCGCADC_R;
    delay = SYSCTL_RCGCADC_R;
    delay = SYSCTL_RCGCADC_R;
    UART1_CTL_R &= ~0x00000001;    // disable UART
    UART1_IBRD_R = 2500;     // IBRD = int(80,000,000/(16*2000)) = int(2500)
    UART1_FBRD_R = 0;     // FBRD = round(0.00 * 64) = 0
    UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
    UART1_CTL_R |= 0x00000001;     // enable UART
    UART1_IM_R |= 0x10; // Arm RTRIS
    GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
    GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1
    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
    GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF00FFFF) | 0x00400000;
    NVIC_EN0_R = 0x40; //enable interrupt 6 in NVIC

}
//// Assumes a 80 MHz bus clock, creates 115200 baud rate
//
//void UART_Init(void){            // should be called only once
//
//  SYSCTL_RCGCUART_R |= 0x00000002;  // activate UART1
//
//  SYSCTL_RCGCGPIO_R |= 0x00000004;  // activate port C
//
//  UART1_CTL_R &= ~0x00000001;    // disable UART
//
//  UART1_IBRD_R = 43;     // IBRD = int(80,000,000/(16*115,200)) = int(43.40278)
//
//  UART1_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26
//
//  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
//
//  UART1_CTL_R |= 0x00000001;     // enable UART
//
//  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
//
//  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1
//
//  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
//
//  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
//
//}

//------------UART1_InChar------------
// Wait for new input, interrupt driven
// Use your FIFO
// then return ASCII code
// Input: none
// Output: char read from UART

char UART1_InChar(void){
  // write this
//    while((UART1_FR_R&0x0010) != 0); // wait until RXFE is 0
    while(Fifo_Get() == 0) {};
    return Fifo_Get();
}
// Wait for new input, then return ASCII code

//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until > is typed
//    or until max length of the string is reached.
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART1_InMessage(char *bufPt){
  // write this, if needed

}
//------------UART1_OutChar------------
// Output 8-bit to serial port
// Do not use interrupts or FIFO, use busy wait
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART1_OutChar(char data){
  // write this
    while((UART1_FR_R&0x0020) != 0); // wait until TXFF is 0
    UART1_DR_R = data;
}
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// UART1 receiver has timed out
// Use your FIFO
uint32_t RxCounter = 0;
void UART1_Handler(void){
  char x;
  PF2 ^= 0x04;  // Heartbeat toggle
  PF2 ^= 0x04;  // Heartbeat toggle
  while((UART1_FR_R&0x0010) == 0) { // wait until RXFE bit in UART1_FR_R is 0 (should be 8 bytes)
      x = (UART1_DR_R & 0xFF); // read last byte from UART1_DR_R
      Fifo_Put(x);
  }
  RxCounter++;
  UART1_ICR_R = 0x40;
  PF2 ^= 0x04;  // Heartbeat toggle
}

//------------UART1_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART1_OutString(char *pt){
  // write if needed

}



