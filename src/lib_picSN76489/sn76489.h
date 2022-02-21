/*******************************************************************************
 * @file    sn76489.h
 * @brief   Library for sn76489 sound chip.
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2022.01.23
 * @details OH SO MANY
 * @version 0.0.1
 * 
 * @TODO
 *  - Add in a better way to enable/disable delay setting
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

// __DELAY_READY define, MUST USE _XTAL_FREQ define as well in this header.
// #define __DELAY_READY
// #define _XTAL_FREQ  48000000

/**
 * @def __LOOP_READY
 * Use loop check of ready, default method. Its a bit slow, and has issues
 * with setting data multiple times.
 */
//#define __LOOP_READY

/**
 * @struct s_sn76489 
 * @brief Struct for containing sn76489 instances 
 */
struct s_sn76489
{
  /**
   * @var s_sn76489::p_dataPortW
   * data port to use for writing output data
   */
  volatile unsigned char *p_dataPortW;
  /**
   * @var s_sn76489::p_ctrlPortW
   * port to use for control lines nCE, nWE. Will not change other port pins.
   */
  volatile unsigned char *p_ctrlPortW;
  /**
   * @var s_sn76489::p_readyPortR
   * port to use to monitor for ready pin
   */
  volatile unsigned char *p_readyPortR;
  /**
   * @var s_sn76489::nWE
   * active low write enable pin number
   */
  uint8_t nWE;
  /**
   * @var s_sn76489::nCE
   * active low chip enable pin number
   */
  uint8_t nCE;
  /**
   * @var s_sn76489::ready
   * active high chip ready pin number
   */
  uint8_t ready;
};

/***************************************************************************//**
 * @brief   Initialize sn76489 ports for correct I/O and set pin numbers
 * 
 * @param   p_sn76489 pointer to struct to contain port data.
 * @param   p_dataTRIS pointer to data direction register.
 * @param   p_ctrlTRIS pointer to control direction register.
 * @param   p_readyTRIS pointer to ready direction register.
 * @param   nWE is the pin number(bit) nWE is connected to on control port.
 * @param   nCE is the pin number(bit) nCE is connected to on control port.
 * @param   ready is the pin number(bit) ready is connected to on ready port.
 ******************************************************************************/
void initSN76489port(struct s_sn76489 *p_sn76489, volatile unsigned char *p_dataTRIS, volatile unsigned char *p_ctrlTRIS, volatile unsigned char *p_readyTRIS, uint8_t nWE, uint8_t nCE, uint8_t ready);

/***************************************************************************//**
 * @brief   Initialize sn76489 struct with ports to use for input output, must match
 * direction registers above.
 * 
 * @param   p_sn76489 pointer to struct to contain port data.
 * @param   p_dataPortW pointer to data output port.
 * @param   p_ctrlPortW pointer to control output port.
 * @param   p_readyPortR pointer to ready output port.
 ******************************************************************************/
void initSN76489(struct s_sn76489 *p_sn76489, volatile unsigned char *p_dataPortW, volatile unsigned char *p_ctrlPortW, volatile unsigned char *p_readyPortR);

/***************************************************************************//**
 * @brief   Calculate frequency from hertz to binary value.
 * 
 * @param   refClk is the reference clock in hertz for the sound chip.
 * @param   voiceFreq is the target frequency in hertz.
 * 
 * @return  A unsigned 16 bit number that will result in the freqency wanted.
 ******************************************************************************/
uint16_t getSN76489_FreqDiv(uint32_t refClk, uint32_t voiceFreq);

/***************************************************************************//**
 * @brief   Set sn76489 voice frequency
 * 
 * @param   p_sn76489 pointer to a struct that contains chip data.
 * @param   voice the voice (1,2, or 3) to set the frequency to.
 * @param   freqDiv is binary number to set the frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setSN76489voice_freq(struct s_sn76489 *p_sn76489, uint8_t voice, uint16_t freqDiv);

/***************************************************************************//**
 * @brief   Set sn76489 voice attenuation
 * 
 * @param   p_sn76489 pointer to a struct that contains chip data.
 * @param   voice the voice (1,2, or 3) to set the attenuation to.
 * @param   attenuate : 8 = 16db, 4 = 8db, 2 = 4db, 1 = 2 db, 15 = Mute 
 ******************************************************************************/
void setSN76489voice_attn(struct s_sn76489 *p_sn76489, uint8_t voice, uint8_t attenuate);

/***************************************************************************//**
 * @brief   Set sn76489 noise attenuation
 * 
 * @param   p_sn76489 pointer to a struct that contains chip data.
 * @param   attenuate : 8 = 16db, 4 = 8db, 2 = 4db, 1 = 2 db, 15 = Mute 
 ******************************************************************************/
void setSN76489noise_attn(struct s_sn76489 *p_sn76489, uint8_t attenuate);

/***************************************************************************//**
 * @brief   Set sn76489 noise type
 * 
 * @param   p_sn76489 pointer to a struct that contains chip data.
 * @param   type : 0 = periodic, 1 = white.
 * @param   rate : 0 = N/512, 1 = N/2048, 2 = N/1024, 3 = voice 3 
 ******************************************************************************/
void setSN76489noiseCtrl(struct s_sn76489 *p_sn76489, uint8_t type, uint8_t rate);

#endif
