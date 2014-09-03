/*
 *  Scrolling text for matrix displays
 *
 * Copyright (c) 2014, Daniel Swann <hs@dswann.co.uk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the owner nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include "MatrixText.h"

MatrixText::MatrixText(set_xy_fuct set_xy)
{
  _set_xy = set_xy;
  _scroll_speed = 50;
  _last_move = 0;

}

MatrixText::~MatrixText()
{

}

void MatrixText::set_scroll_speed(uint16_t speed)
{
  _scroll_speed = speed;
}

void MatrixText::set_character_spacing(uint8_t pixels)
{
  _character_spacing = pixels;
}

void MatrixText::show_text(const char *msg, uint16_t ul_x, uint16_t ul_y, uint16_t br_x, uint16_t br_y)
{
  _msg = msg;
  _msg_len = strlen(msg);
  _ul_x = ul_x;
  _ul_y = ul_y;
  _br_x = br_x;
  _br_y = br_y;
  _position = 0;
  _character_spacing = 1;
}


void MatrixText::loop()
{
  byte ch;

  if ( (millis()-_last_move) < _scroll_speed)
    return;
  else
    _last_move = millis();
  
  // Wipe buffer
  for (uint16_t x=_ul_x; x < _br_x; x++)
    for (uint16_t y = _ul_y; y < _br_y; y++)
      _set_xy(x, y, 0);

  for (uint16_t x = _ul_x; x < _br_x; x++)
  {
    uint16_t char_pos = (x+_position)/(SYSTEM5x8_WIDTH + _character_spacing);

    // Bounds check of message array
    if ( (char_pos >= _msg_len) || (char_pos < 0) )
      continue;

    // Check character is in the font
    if ( (_msg[char_pos] < 32) || (_msg[char_pos] > 126) )
      continue;
    
    // Get the character data to display.
    // If we're in the gap between characters, there is nothing to get.
    if (((x+_position)%(SYSTEM5x8_WIDTH+_character_spacing)) >= SYSTEM5x8_WIDTH)
    {
      ch = 0;
    } 
    else
    {      
      // Get character/font data from progmem
      uint16_t font_pos = ((_msg[char_pos]-32)*SYSTEM5x8_WIDTH) + ((x+_position)%(SYSTEM5x8_WIDTH+_character_spacing));
      //                     ^^^ Get start of character ^^^               ^^^ Get position in character ^^^
      ch = pgm_read_byte_near(System5x8 + font_pos);
    }

    // Output column of character
    for (uint8_t y = _ul_y; y < _ul_y+SYSTEM5x8_HEIGHT; y++)
    {
      if (y <= _br_y)
        _set_xy(x, y, ch & (1 << (y-_ul_y)));
    }
  }

  // Scroll the text. If it's scrolled off-screen, reset position to just off the right edge
  if ((int32_t)_position++ > (int32_t)(_msg_len * (SYSTEM5x8_WIDTH + _character_spacing))) 
    _position = -1*_br_x;
}

