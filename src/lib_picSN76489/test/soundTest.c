/*******************************************************************************
 * @file      picTEST.c
 * @author    Jay Convertino
 * @date      2021.12.14
 * @brief     Test PICerino platform
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>

#include <sn76489.h>

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
  uint16_t freq = 0;
  uint8_t attn = 0;
  uint8_t shiftRate = 0;
  
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

  /* Port E set all to output */
  TRISE = 0;

  LATE = 0;

  g_porteBuffer = 1;

  /* wait for chip to be ready */
  __delay_ms(10);

  initSN76489port(&sn76489, &TRISA, &TRISD, &TRISC, 6, 7, 7);

  initSN76489(&sn76489, &LATA, &LATD, &PORTC);

  /* chip tests */
  /* voice one freq test */
  LATE = (1 << 0);
  
  setSN76489voice1_attn(&sn76489, 2);
  
  do
  {
    setSN76489voice1_freq(&sn76489, freq);
    
    __delay_ms(5);
    
    freq++;
    
  } while(freq < (1 << 10));
  
  setSN76489voice1_freq(&sn76489, 254);
  
  /* voice one attn test */
  do
  {
    setSN76489voice1_attn(&sn76489, attn);
    
    __delay_ms(500);
    
    attn++;
    
  } while(attn < (1 << 4));

  /* voice two freq test */
  freq = 0;
  attn = 0;
  
  LATE = (1 << 1);
  
  setSN76489voice2_attn(&sn76489, 2);
  
  do
  {
    setSN76489voice2_freq(&sn76489, freq);
    
    __delay_ms(5);
    
    freq++;
    
  } while(freq < (1 << 10));
  
  setSN76489voice2_freq(&sn76489, 254);
  
  /* voice two attn test */
  do
  {
    setSN76489voice2_attn(&sn76489, attn);
    
    __delay_ms(500);
    
    attn++;
    
  } while(attn < (1 << 4));
  
  /* voice three freq test */
  freq = 0;
  attn = 0;
  
  LATE = (1 << 2);
  
  setSN76489voice3_attn(&sn76489, 2);
  
  do
  {
    setSN76489voice3_freq(&sn76489, freq);
    
    __delay_ms(5);
    
    freq++;
    
  } while(freq < (1 << 10));
  
  setSN76489voice3_freq(&sn76489, 254);
  
  /* voice three attn test */
  do
  {
    setSN76489voice3_attn(&sn76489, attn);
    
    __delay_ms(500);
    
    attn++;
    
  } while(attn < (1 << 4));
  
  /* test noise */
  attn = 0;
  
  LATE = 3;
  
  setSN76489noise_attn(&sn76489, 2);
  
  /** Periodic noise **/
  do
  {
    setSN76489noiseCtrl(&sn76489, 0, shiftRate);
    
    __delay_ms(2000);
    
    shiftRate++;
    
  } while(shiftRate < (1 << 2));
  
  /* noise attn test */
  do
  {
    setSN76489noise_attn(&sn76489, attn);
    
    __delay_ms(500);
    
    attn++;
    
  } while(attn < (1 << 4));
  
  shiftRate = 0;
  
  LATE = 7;
  
  setSN76489noise_attn(&sn76489, 2);
  
  /** White noise **/
  do
  {
    setSN76489noiseCtrl(&sn76489, 1, shiftRate);
    
    __delay_ms(2000);
    
    shiftRate++;
    
  } while(shiftRate < (1 << 2));
  
  attn = 0;
  
  /* noise attn test */
  do
  {
    setSN76489noise_attn(&sn76489, attn);
    
    __delay_ms(500);
    
    attn++;
    
  } while(attn < (1 << 4));
  
  /* done testing */
  LATE = 0;
  
  for(;;)
  {
    LATE = g_porteBuffer;
    g_porteBuffer = (g_porteBuffer == 4 ? 1 : g_porteBuffer << 1);
    __delay_ms(100);
  }
}
