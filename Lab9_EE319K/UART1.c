// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// 1/2/2023

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "Fifo.h"
#include "UART1.h"
#include "../inc/tm4c123gh6pm.h"

// Initialize UART1
// Baud rate is 2000 bits/sec
void UART1_Init(void){
	Fifo_Init();
  SYSCTL_RCGCUART_R |= 0x0002; // activate UART1
	
	while((SYSCTL_RCGCUART_R&0x02) == 0){};
  SYSCTL_RCGCGPIO_R |= 0x0004; // activate port C
  while((SYSCTL_RCGCGPIO_R&0x04) == 0){};
  UART1_CTL_R &= ~0x0002;      // disable UART
	UART1_IM_R |= 0x40;
  UART1_IBRD_R = 2500; 
		//IBRD=int(80000000/(16*2000)) = int(2500)
  UART1_FBRD_R = 0;  
// FBRD = round(0.1267 * 64) = 8
  UART1_LCRH_R = 0x0070;  // 8-bit length, enable FIFO
	NVIC_EN0_R |= 0x40;  
  UART1_CTL_R = 0x0301;   // enable RXE, TXE and UART
	GPIO_PORTC_AMSEL_R &= ~0x30; // No analog on PC5-4
  GPIO_PORTC_AFSEL_R |= 0x30; // alt funct on PC5-4 0011 0000
  GPIO_PORTC_PCTL_R = 
            (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_DEN_R |= 0x30;   // digital I/O on PC5-4

}

//------------UART1_InChar------------
// Wait for new input, interrupt driven
// Use your FIFO
// then return ASCII code
// Input: none
// Output: char read from UART
char UART1_InChar(void){
  // write this
//    while((UART1_FR_R&0x0010) != 0); // wait until RXFE is 0
    while (Fifo_Get() == 0) {};
    char temp = Fifo_Get();
    return temp;
}
// Wait for new input, then return ASCII code

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
	
uint32_t rxc = 0;
// RTRIS UART1 receiver has timed out
// Put data into your FIFO
void UART1_Handler(void){char letter;
  // write this
//1) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
	PF2 ^= 0x04;      // Heartbeat
//2) as long as the RXFE bit in the UART1_FR_R is zero (should be exactly 8 bytes, but could be more or less)
//   -Read a byte from UART1_DR_R 
//   -Put the byte into your software FIFO
	 while((UART1_FR_R&0x010) == 0) {	//check if RXFE is 0new
		 Fifo_Put(UART1_DR_R);
	 }
	
	/*for (uint8_t i = 0; i<8; i++) {
		letter = UART1_DR_R & 0xFF;
		Fifo_Put(letter);
	} */
	//3) Increment a RxCounter, used as debugging monitor of the number of UART messages received
	rxc++;
//4) acknowledge the interrupt by clearing the flag which requested the interrupt 
	UART1_FR_R &= ~0x10;
  UART1_ICR_R = ~0x40;   // this clears bit 6 (RTRIS) in the RIS register  ICHANGEDTHIS I CHANGED THIS I ADDED ~
// 5) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
	PF2 ^= 0x04;      // Heartbeat
}