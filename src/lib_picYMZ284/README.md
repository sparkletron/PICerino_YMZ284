# YMZ284 Sound IC library

Software Library for Yamaha YMZ284 sound chips.

author: Jay Convertino  

data: 2024.05.27

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
  - make libYMZ284.a : static library only
  - make clean : remove all build outputs.
  
## Documentation
  - See doxygen generated document
  - Method for ready check is universal, NOT efficent. Optimize send data for your application! 
  
### Example Code
```c
void main(void)
{
  struct s_ymz284 ymz284;

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
  __delay_ms(100);

  initYMZ284port(&ymz284, &TRISA, &TRISD, 0, 1);

  initYMZ284(&ymz284, &LATA, &LATD);

  setYMZ284mixer(&ymz284, ~0, ~1);

  setYMZ284channel_attn(&ymz284, 'A', 15, 0);

  setYMZ284channel_freq(&ymz284, 'A', 254);

  /* play this lovely tune forever */
  for(;;);
}
```
