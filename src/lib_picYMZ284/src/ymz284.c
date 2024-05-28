/*******************************************************************************
 * @file    ymz284.c
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2024.05.27
 * @brief   Library for YMZ284 sound chip.
 * 
 * @license mit
 * 
 * Copyright 2024 Johnathan Convertino
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

#include <ymz284.h>

/** DEFINES **/
#define CHAN_A_FREQ_L   0
#define CHAN_A_FREQ_H   1
#define CHAN_B_FREQ_L   2
#define CHAN_B_FREQ_H   3
#define CHAN_C_FREQ_L   4
#define CHAN_C_FREQ_H   5
#define NOISE_FREQ      6
#define MIXER_SETTING   7
#define CHAN_A_LEVEL    8
#define CHAN_B_LEVEL    9
#define CHAN_C_LEVEL    10
#define ENVELOPE_FREQ_L 11
#define ENVELOPE_FREQ_H 12
#define ENVELOPE_SHAPE  13
#define NON_VALID_ADDR  14
#define CONTROL_POWER   15

/** SEE MY PRIVATES **/
/*** send address to port and control chip ***/
void sendAddr(struct s_ymz284 *p_ymz284, uint8_t addr);
/*** send data to port and control chip ***/
void sendData(struct s_ymz284 *p_ymz284, uint8_t data);

/** INITIALIZE AND FREE MY STRUCTS **/

/*** Initialize ymz284 tris port direction and struct pins ***/
void initYMZ284port(struct s_ymz284 *p_ymz284, volatile unsigned char *p_dataTRIS, volatile unsigned char *p_ctrlTRIS, uint8_t nCSWR, uint8_t A0)
{
  if(!p_dataTRIS) return;
  
  if(!p_ctrlTRIS) return;
  
  if(!p_ymz284) return;
  
  /**** All outputs for data port ****/
  *p_dataTRIS = 0;
  
  /**** nCSWR, and A0 are output ****/
  *p_ctrlTRIS &= ~(((unsigned)1 << nCSWR) | ((unsigned)1 << A0));
  
  /**** setup struct with pin locations ****/
  p_ymz284->nCSWR = nCSWR;
  
  p_ymz284->A0 = A0;
}

/*** Initialize ymz284 struct ports ***/
void initYMZ284(struct s_ymz284 *p_ymz284, volatile unsigned char *p_dataPortW, volatile unsigned char *p_ctrlPortW)
{
  if(!p_dataPortW) return;
  
  if(!p_ctrlPortW) return;
  
  if(!p_ymz284)  return;
  
  /**** setup ports for library usage ****/
  p_ymz284->p_dataPortW = p_dataPortW;
  
  p_ymz284->p_ctrlPortW = p_ctrlPortW;
  
  *(p_ymz284->p_ctrlPortW) = ~0;
  
  *(p_ymz284->p_dataPortW) = 0;
  
  /**** mute all ****/
  
  setYMZ284channel_attn(p_ymz284, 'A', 0, 0);
  
  setYMZ284channel_attn(p_ymz284, 'B', 0, 0);
  
  setYMZ284channel_attn(p_ymz284, 'C', 0, 0);
}

/*** calculate freqDiv ***/
uint16_t getYMZ284_FreqDiv(uint32_t refClk, uint32_t channelFreq)
{
  return (refClk/(channelFreq << 5)) & 0x0FFF;
}

/*** calculate envelope freqDiv ***/
uint16_t getYMZ284_EnvFreqDiv(uint32_t refClk, uint32_t channelFreq)
{
  return (refClk/(channelFreq << 9));
}

/** SET YOUR DATA **/

/*** set ymz284 channel frequency ***/
void setYMZ284channel_freq(struct s_ymz284 *p_ymz284, char channel, uint16_t freqDiv)
{
  uint8_t addr_l;
  uint8_t addr_h;
  
  /**** NULL check ****/
  if(!p_ymz284) return;
  
  /**** Select the correct register for the selected channel ****/
  switch(channel)
  {
    case 'A':
      addr_l = CHAN_A_FREQ_L;
      addr_h = CHAN_A_FREQ_H;
      break;
    case 'B':
      addr_l = CHAN_B_FREQ_L;
      addr_h = CHAN_B_FREQ_H;
      break;
    case 'C':
      addr_l = CHAN_C_FREQ_L;
      addr_h = CHAN_C_FREQ_H;
      break;
    default:
      return;
  }
  
  sendAddr(p_ymz284, addr_l);
  sendData(p_ymz284, freqDiv & 0x00FF);

  sendAddr(p_ymz284, addr_h);
  sendData(p_ymz284, (freqDiv >> 8) & 0x000F);
}

/*** set ymz284 channel attenuation ***/
void setYMZ284channel_attn(struct s_ymz284 *p_ymz284, char channel, uint8_t attenuate, uint8_t select)
{
  uint8_t addr;


  /**** NULL check ****/
  if(!p_ymz284) return;

  /**** Select the correct register for the selected channel ****/
  switch(channel)
  {
    case 'A':
      addr = CHAN_A_LEVEL;
      break;
    case 'B':
      addr = CHAN_B_LEVEL;
      break;
    case 'C':
      addr = CHAN_C_LEVEL;
      break;
    default:
      return;
  }
  
  sendAddr(p_ymz284, addr);
  sendData(p_ymz284, (attenuate & (unsigned)0x0F) | (unsigned)(select << 4));
}

void setYMZ284mixer(struct s_ymz284 *p_ymz284, uint8_t noise, uint8_t tone)
{
  /**** NULL check ****/
  if(!p_ymz284) return;

  sendAddr(p_ymz284, MIXER_SETTING);
  sendData(p_ymz284, (unsigned)(noise << 3) | tone);
}

/*** set ymz284 noise frequency ***/
void setYMZ284noise_freq(struct s_ymz284 *p_ymz284, uint8_t freqDiv)
{
  /**** NULL check ****/
  if(!p_ymz284) return;
  
  sendAddr(p_ymz284, NOISE_FREQ);
  sendData(p_ymz284, freqDiv & (unsigned)0x1F);
}

/*** set ymz284 envelope frequency***/
void setYMZ284env_freq(struct s_ymz284 *p_ymz284, uint16_t freqDiv)
{
  /**** NULL check ****/
  if(!p_ymz284) return;
  
  sendAddr(p_ymz284, ENVELOPE_FREQ_L);
  sendData(p_ymz284, freqDiv & 0x00FF);

  sendAddr(p_ymz284, ENVELOPE_FREQ_H);
  sendData(p_ymz284, (freqDiv >> 8) & 0x00FF);
}

void setYMZ284env_shape(struct s_ymz284 *p_ymz284, uint8_t shape)
{
  /**** NULL check ****/
  if(!p_ymz284) return;

  sendAddr(p_ymz284, ENVELOPE_SHAPE);
  sendData(p_ymz284, shape & (unsigned)0x0F);
}

/*** send address to chip ***/
void sendAddr(struct s_ymz284 *p_ymz284, uint8_t addr)
{
  /**** NULL check ****/
  if(!p_ymz284) return;

  *(p_ymz284->p_ctrlPortW) &= ~((unsigned)1 << p_ymz284->A0);

  sendData(p_ymz284, addr);

  *(p_ymz284->p_ctrlPortW) |= ((unsigned)1 << p_ymz284->A0);

}

/*** send data to chip ***/
void sendData(struct s_ymz284 *p_ymz284, uint8_t data)
{
  /**** NULL check ****/
  if(!p_ymz284) return;
  
  di();
  
  *(p_ymz284->p_dataPortW) = data;

  *(p_ymz284->p_ctrlPortW) &= ~((unsigned)1 << p_ymz284->nCSWR);

  *(p_ymz284->p_ctrlPortW) |= ((unsigned)1 << p_ymz284->nCSWR);
  
  ei();
}
