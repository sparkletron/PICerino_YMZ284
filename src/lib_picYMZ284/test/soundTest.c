/*******************************************************************************
 * @file      picTEST.c
 * @author    Jay Convertino
 * @date      2024.05.27
 * @brief     Test PICerino platform
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>

#include <ymz284.h>

//setup to XTAL freq, for _delay macros
#define _XTAL_FREQ  48000000

/* configuration bits */
#pragma config PLLSEL   = PLL3X
#pragma config CFGPLLEN = ON
#pragma config CPUDIV   = NOCLKDIV
#pragma config LS48MHZ  = SYS48X8
#pragma config FOSC     = INTOSCIO
#pragma config PCLKEN   = OFF
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config nPWRTEN  = OFF
#pragma config BOREN    = OFF
#pragma config BORV     = 190
#pragma config nLPBOR   = OFF
#pragma config WDTEN    = OFF
#pragma config WDTPS    = 32768
#pragma config CCP2MX   = RC1
#pragma config PBADEN   = OFF
#pragma config MCLRE    = OFF

//needed since chip is read-modify write only, and read in the same line doesn't work
uint8_t g_porteBuffer = 0;

void main(void) 
{
  uint8_t  index = 0;
  uint16_t freq = 0;
  uint8_t  attn = 0;
  uint8_t  shiftRate = 0;
  
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

  /* Port E set all to output */
  TRISE = 0;

  LATE = 0;

  g_porteBuffer = 1;

  /* wait for chip to be ready */
  __delay_ms(100);

  initYMZ284port(&ymz284, &TRISA, &TRISD, 0, 1);

  initYMZ284(&ymz284, &LATA, &LATD);

  /* voice tests */
  for(index = 65; index <= 67; index++)
  {
    attn = 16;
    freq = 0;
    
    LATE = (unsigned)(1 << (index-65));

    setYMZ284mixer(&ymz284, ~0, ~(1 << (index-65)));

    setYMZ284channel_attn(&ymz284, (char)index, 15, 0);
    
    /* voice freq test */
    do
    {
      setYMZ284channel_freq(&ymz284, (char)index, freq);
      
      __delay_ms(5);
      
      freq++;
      
    } while(freq < (1 << 12));
    
    setYMZ284channel_freq(&ymz284, (char)index, 254);
    
    /* voice attn test */
    do
    {
      attn--;

      setYMZ284channel_attn(&ymz284, (char)index, attn, 0);

      __delay_ms(500);
      
    } while(attn > 0);
  }
  
  for(;;)
  {
    LATE = g_porteBuffer;
    g_porteBuffer = (g_porteBuffer == 4 ? 1 : (unsigned)g_porteBuffer << 1);
    __delay_ms(100);
  }
}
