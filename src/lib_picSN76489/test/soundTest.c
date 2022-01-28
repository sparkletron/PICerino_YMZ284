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
    ANSELE = 0;
    ANSELD = 0;
    
    /* Port E set all to output */
    TRISE = 0;
    
    LATE = 0;
    
    g_porteBuffer = 1;
    
    //wait for chip to be ready
    __delay_ms(10);
    
    initSN76489port(&sn76489, &TRISA, &TRISD, 6, 7, 5);
    
    initSN76489(&sn76489, &LATA, &PORTD, &LATD);
    
//     setSN76489voice1_freq(&sn76489, 112);
    
//     setSN76489voice1_attn(&sn76489, 8);
    
    for(;;)
    {
      LATE = g_porteBuffer;
      g_porteBuffer = (g_porteBuffer == 4 ? 1 : g_porteBuffer << 1);
      __delay_ms(1000);
    }
}
