/*
 * File:   ADC.c
 * Author: Vincenzo
 *
 * Created on February 3, 2020, 12:07 PM
 */


#include <xc.h>
#include "ADC.h"

void ADC(void) {
  ADCON0bits.ADCS1 = 0;
  ADCON0bits.ADCS0 = 1;
  ADCON0bits.CHS3 = 0;
  ADCON0bits.CHS2 = 1;
  ADCON0bits.CHS1 = 1;
  ADCON0bits.CHS0 = 0;
  ADCON1bits.ADFM = 0;
  ADCON1bits.VCFG0 = 0;
  ADCON1bits.VCFG1 = 0;
  ADCON0bits.ADON = 1;
  
  PIE1bits.ADIE = 1;
  PIR1bits.ADIF = 0;
  
}
