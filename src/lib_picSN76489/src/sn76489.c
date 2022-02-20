/*******************************************************************************
 * @file    sn76489.c
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2022.01.23
 * @brief   Library for SN76489 sound chip.
 * 
 * @license mit
 * 
 * Copyright 2022 Johnathan Convertino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>

#include <sn76489.h>

/** DEFINES **/
#define VOICE1_FREQ 0
#define VOICE2_FREQ 2
#define VOICE3_FREQ 1
#define VOICE1_ATTN 4
#define VOICE2_ATTN 6
#define VOICE3_ATTN 5
#define NOISE_CTRL  3
#define NOISE_ATTN  7
#define FIRST_BYTE  0x01
#define SECOND_BYTE 0x00
#define REG_SHIFT   1

/** SEE MY PRIVATES **/
/*** send data to port and control chip ***/
void sendData(struct s_sn76489 *p_sn76489, uint8_t data);
/*** check rate against valid datasheet values ***/
uint8_t checkRate(uint8_t rate);

/** INITIALIZE AND FREE MY STRUCTS **/

/*** Initialize sn76489 tris port direction and struct pins ***/
void initSN76489port(struct s_sn76489 *p_sn76489, volatile unsigned char *p_dataTRIS, volatile unsigned char *p_ctrlTRIS, volatile unsigned char *p_readyTRIS, uint8_t nWE, uint8_t nCE, uint8_t ready)
{
  if(!p_dataTRIS) return;
  
  if(!p_ctrlTRIS) return;
  
  if(!p_readyTRIS) return;
  
  if(!p_sn76489) return;
  
  /**** All outputs for data port ****/
  *p_dataTRIS = 0;
  
  /**** nWE is output, nCE is output ****/
  *p_ctrlTRIS &= ~(((unsigned)1 << nWE) | ((unsigned)1 << nCE));
  
  /**** ready is input ****/
  *p_readyTRIS |= ((unsigned)1 << ready);
  
  /**** setup struct with pin locations ****/
  p_sn76489->nWE = nWE;
  
  p_sn76489->nCE = nCE;
  
  p_sn76489->ready = ready;
}

/*** Initialize sn76489 struct ports ***/
void initSN76489(struct s_sn76489 *p_sn76489, volatile unsigned char *p_dataPortW, volatile unsigned char *p_ctrlPortW, volatile unsigned char *p_readyPortR)
{
  if(!p_dataPortW) return;
  
  if(!p_ctrlPortW) return;
  
  if(!p_readyPortR) return;
  
  if(!p_sn76489)  return;
  
  /**** setup ports for library usage ****/
  p_sn76489->p_dataPortW = p_dataPortW;
  
  p_sn76489->p_readyPortR = p_readyPortR;
  
  p_sn76489->p_ctrlPortW = p_ctrlPortW;
  
  *(p_sn76489->p_ctrlPortW) = ~0;
  
  *(p_sn76489->p_dataPortW) = 0;
  
  /**** mute all ****/
  setSN76489voice1_attn(p_sn76489, 15);
  
  setSN76489voice2_attn(p_sn76489, 15);
  
  setSN76489voice3_attn(p_sn76489, 15);
  
  setSN76489noise_attn(p_sn76489, 15);
}

/*** calculate freqDiv ***/
uint16_t getSN76489_FreqDiv(uint32_t refClk, uint32_t voiceFreq)
{
  return (refClk/((unsigned)5 << voiceFreq)) & 0x03FF;
}

/** SET YOUR DATA **/

/*** set sn76489 voice 1 frequency ***/
void setSN76489voice1_freq(struct s_sn76489 *p_sn76489, uint16_t freqDiv)
{
  sendData(p_sn76489, ((VOICE1_FREQ << REG_SHIFT) | ((freqDiv & 0xF000) >> 8) | FIRST_BYTE));
  sendData(p_sn76489, (((freqDiv & 0x0FC0) >> 4) | SECOND_BYTE));
}

/*** set sn76489 voice 2 frequency ***/
void setSN76489voice2_freq(struct s_sn76489 *p_sn76489, uint16_t freqDiv)
{
  sendData(p_sn76489, ((VOICE2_FREQ << REG_SHIFT) | ((freqDiv & 0xF000) >> 8) | FIRST_BYTE));
  sendData(p_sn76489, (((freqDiv & 0x0FC0) >> 4) | SECOND_BYTE));
}

/*** set sn76489 voice 3 frequency ***/
void setSN76489voice3_freq(struct s_sn76489 *p_sn76489, uint16_t freqDiv)
{
  sendData(p_sn76489, ((VOICE3_FREQ << REG_SHIFT) | ((freqDiv & 0xF000) >> 8) | FIRST_BYTE));
  sendData(p_sn76489, (((freqDiv & 0x0FC0) >> 4) | SECOND_BYTE));
}

/*** set sn76489 voice 1 attenuation ***/
void setSN76489voice1_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate)
{
  sendData(p_sn76489, ((unsigned)VOICE1_ATTN << REG_SHIFT) | attenuate | FIRST_BYTE);
}

/*** set sn76489 voice 2 attenuation ***/
void setSN76489voice2_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate)
{
  sendData(p_sn76489, ((unsigned)VOICE2_ATTN << REG_SHIFT) | attenuate | FIRST_BYTE);
}

/*** set sn76489 voice 3 attenuation ***/
void setSN76489voice3_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate)
{
  sendData(p_sn76489, ((unsigned)VOICE3_ATTN << REG_SHIFT) | attenuate | FIRST_BYTE);
}

/*** set sn76489 noise attenuation ***/
void setSN76489noise_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate)
{
  sendData(p_sn76489, ((unsigned)NOISE_ATTN << REG_SHIFT) | attenuate | FIRST_BYTE);
}

/*** set sn76489 noise controls***/
void setSN76489noiseCtrl(struct s_sn76489 *p_sn76489, uint8_t type, uint8_t rate)
{
  sendData(p_sn76489, ((unsigned)NOISE_CTRL << REG_SHIFT) | ((unsigned)rate << 6) | ((unsigned)type << 5) | FIRST_BYTE);
}

/*** send data to chip ***/
void sendData(struct s_sn76489 *p_sn76489, uint8_t data)
{
  di();
  
  /*** make sure we are ready for data ***/
  while(!((*(p_sn76489->p_readyPortR) >> p_sn76489->ready) & 0x01));
  
  *(p_sn76489->p_dataPortW) = data;
  
  *(p_sn76489->p_ctrlPortW) &= ~((unsigned)1 << p_sn76489->nCE);
  
  *(p_sn76489->p_ctrlPortW) &= ~((unsigned)1 << p_sn76489->nWE);
  
#ifdef __DELAY_READY
  __delay_ms(10);
#endif
  
#ifdef __LOOP_READY
  while(!((*(p_sn76489->p_readyPortR) >> p_sn76489->ready) & 0x01));
#endif
  
  *(p_sn76489->p_ctrlPortW) |= ((unsigned)1 << p_sn76489->nCE);
  
  *(p_sn76489->p_ctrlPortW) |= ((unsigned)1 << p_sn76489->nWE);
  
  ei();
}

/*** check the rate against valid values ***/
uint8_t checkRate(uint8_t rate)
{
  switch(rate)
  {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
      return rate;
    default:
      return 0x00;
  }
}
