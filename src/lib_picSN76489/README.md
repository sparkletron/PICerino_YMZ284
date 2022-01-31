# SN76489 Sound IC library

Software Library for TI SN76489 sound chips.  

author: Jay Convertino  

data: 2022.01.31  

license: MIT

## Release Versions
### Current
  - release_v0.0.1

### Past
  - none
  
## Requirements
  - xc8-cc V2.32
  - PIC18F45K50 (MCU can be changed in makefile)
  - PICerino development board

## Building
  - make : builds all
  - make dox_gen : doxygen only
  - make test : test only
  - make libSN76489.a : static library only
  - make clean : remove all build outputs.
  
## Documentation
  - See doxygen generated document
  - Three methods for ready check when setting data to output port.
    1. Uncomment macro __DELAY_READY for timed delay.
        - MUST SET __XTAL_FREQ IN HEADER FOR THIS TO WORK.
    2. Uncomment macro __LOOP_READY for while loop that checks ready state.
    3. Leave both commented for general delay in system. Works with no optimizations.
  
### Example Code
```c
void main(void) 
{
  struct s_sn76489 sn76489;

  /* OSCCON SETUP */
  OSCCONbits.IRCF = 0x7;
  OSCCONbits.OSTS = 0;
  OSCCONbits.SCS  = 0x3;

  OSCCON2bits.PLLEN = 1;

  /* PORT E SETUP */
  INTCON2bits.nRBPU = 1;

  /* disable analog inputs */
  ANSELA = 0;
  ANSELC = 0;
  ANSELD = 0;
  ANSELE = 0;

  /* wait for chip to be ready */
  __delay_ms(10);

  initSN76489port(&sn76489, &TRISA, &TRISD, &TRISC, 6, 7, 7);

  initSN76489(&sn76489, &LATA, &LATD, &PORTC);

  /* voice one freq */
  
  /* set attenuation */
  setSN76489voice1_attn(&sn76489, 2);
  
  /* set frequency to 440 hz */
  setSN76489voice1_freq(&sn76489, 254);
  
  /* play this lovely tune forever */
  for(;;);
  
}
```
