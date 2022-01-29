/*******************************************************************************
 * @file    sn76489.h
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2022.01.23
 * @brief   Library for sn76489 sound chip.
 * @details OH SO MANY
 * @version 0.0.0
 * 
 * @TODO
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

#ifndef __LIB_SN76489
#define __LIB_SN76489

#include <xc.h>
#include <stdint.h>

/* Define for adding delay to SN76489 data pulse. Not needed in testing */
// #define __DELAY_SN76489


/** Struct for containing multiple sn76489 instances **/
struct s_sn76489
{
  volatile unsigned char *p_dataPortW;
  volatile unsigned char *p_ctrlPortW;
  volatile unsigned char *p_readyPortR;
  uint8_t nWE;
  uint8_t nCE;
  uint8_t ready;
};

/** INITIALIZE AND FREE MY STRUCTS **/

/*******************************************************************************
 * @brief   Initialize sn76489 port struct with params
 * 
 * @param   struct s_sn76489 *p_sn76489
 * @param   volatile unsigned char *p_dataTRIS
 * @param   volatile unsigned char *p_ctrlTRIS
 * @param   volatile unsigned char *p_readyTRIS
 * @param   uint8_t nWE
 * @param   uint8_t nCE
 * @param   uint8_t ready
 ******************************************************************************/
void initSN76489port(struct s_sn76489 *p_sn76489, volatile unsigned char *p_dataTRIS, volatile unsigned char *p_ctrlTRIS, volatile unsigned char *p_readyTRIS, uint8_t nWE, uint8_t nCE, uint8_t ready);

/*******************************************************************************
 * @brief   Initialize sn76489 struct with params
 * 
 * @param   struct s_sn76489 *p_sn76489
 * @param   volatile unsigned char *p_dataPortW
 * @param   volatile unsigned char *p_ctrlPortw
 * @param   volatile unsigned char *p_readyPortR
 ******************************************************************************/
void initSN76489(struct s_sn76489 *p_sn76489, volatile unsigned char *p_dataPortW, volatile unsigned char *p_ctrlPortW, volatile unsigned char *p_readyPortR);


/*******************************************************************************
 * @brief   calculate freqDiv
 * 
 * @param   uint32_t  refClk
 * @param   uint32_t  voiceFreq
 * 
 * @return  uint16_t   freqDiv
 ******************************************************************************/
uint16_t getSN76489_FreqDiv(uint32_t refClk, uint32_t voiceFreq);

/** SET YOUR DATA **/

/*******************************************************************************
 * @brief   set sn76489 voice 1 frequency
 * 
 * @param   struct  s_sn76489 object
 * @param   uint16_t freqDiv binary number to generate frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setSN76489voice1_freq(struct s_sn76489 *p_sn76489, uint16_t freqDiv);

/*******************************************************************************
 * @brief   set sn76489 voice 2 frequency
 * 
 * @param   struct s_sn76489 *p_sn76489 object
 * @param   uint16_t freqDiv binary number to generate frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setSN76489voice2_freq(struct s_sn76489 *p_sn76489, uint16_t freqDiv);

/*******************************************************************************
 * @brief   set sn76489 voice 3 frequency
 * 
 * @param   struct s_sn76489 *p_sn76489 object
 * @param   uint16_t freqDiv binary number to generate frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setSN76489voice3_freq(struct s_sn76489 *p_sn76489, uint16_t freqDiv);

/*******************************************************************************
 * @brief   set sn76489 voice 1 attenuation
 * 
 * @param   struct s_sn76489 *p_sn76489 object
 * @param   uint8_t attenuation : 8 = 16db, 4 = 8db, 2 = 4db, 1 = 2 db, 15 = Mute 
 ******************************************************************************/
void setSN76489voice1_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate);

/*******************************************************************************
 * @brief   set sn76489 voice 2 attenuation
 * 
 * @param   struct s_sn76489 *p_sn76489 object
 * @param   uint8_t attenuation : 8 = 16db, 4 = 8db, 2 = 4db, 1 = 2 db, 15 = Mute 
 ******************************************************************************/
void setSN76489voice2_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate);

/*******************************************************************************
 * @brief   set sn76489 voice 3 attenuation
 * 
 * @param   struct s_sn76489 *p_sn76489 object
 * @param   uint8_t attenuation : 8 = 16db, 4 = 8db, 2 = 4db, 1 = 2 db, 15 = Mute 
 ******************************************************************************/
void setSN76489voice3_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate);

/*******************************************************************************
 * @brief   set sn76489 noise attn
 * 
 * @param   struct s_sn76489 *p_sn76489 object

 ******************************************************************************/
void setSN76489noise_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate);

/*******************************************************************************
 * @brief   set sn76489 noise type
 * 
 * @param   struct s_sn76489 *p_sn76489 object
 * @param   uint8_t type : 0 = periodic, 1 = white.
 * @param   uint8_t rate : 0 = N/512, 1 = N/2048, 2 = N/1024, 3 = voice 3 
 ******************************************************************************/
void setSN76489noiseCtrl(struct s_sn76489 *p_sn76489, uint8_t type, uint8_t rate);

#endif
