// ADC.c
// Runs on TM4C123
// Provide functions that initialize ADC0
// Last Modified: 1/2/2023  
// Student names: Yunjae Jo, Hyukjoo Chung
// Last modification date: 3/27/2023
// This file is in the inc folder so it automatically will be applied to labs 8 9 and 10
// Labs 8, 9, and 10 specify PD2
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function using PD2 
// Input: none
// Output: none
void ADC_Init(void){ 
// write this
	SYSCTL_RCGCADC_R |= 0x0001;   // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x08;    // 2) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x08) != 0x08){};  // 3 for stabilization
  GPIO_PORTD_DIR_R &= ~0x04;    // 4) make PE4 input
  GPIO_PORTD_AFSEL_R |= 0x04;   // 5) enable alternate function on PE4
  GPIO_PORTD_DEN_R &= ~0x04;    // 6) disable digital I/O on PE4
  GPIO_PORTD_AMSEL_R |= 0x04;   // 7) enable analog functionality on PE4
  ADC0_PC_R &= ~0xF;
  ADC0_PC_R |= 0x1;             // 8) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;        // 9) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;      // 10) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;       // 11) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;
  ADC0_SSMUX3_R += 5;           // 12) set channel
  ADC0_SSCTL3_R = 0x0006;       // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;         // 14) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       // 15) enable sample sequencer 3
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	uint32_t result;
	ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}


