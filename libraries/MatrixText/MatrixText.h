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


#ifndef MATRIXTEXT_H
#define MATRIXTEXT_H

#include <stdint.h>
#include <Arduino.h>

#include "System5x7.h"

typedef unsigned char byte;
typedef void (*set_xy_fuct)(byte,byte,byte);


/*
   Class: MatrixText
   Class that can be used to generate scroll text on matrix displays
*/
class MatrixText
{
  public:
    
    /*
       Constructor: MatrixText
         Create MatrixText object, and set the function used to write to the display.
       
       Parameters:
         set_xy - Pointer to function used to set a pixel on the display. Should be of type "void (byte x, byte y, byte val)". 
         Function set_xy will be called with val=0 to clear a pixel, non-zero otherwise.
    */
    MatrixText(set_xy_fuct set_xy);
    
    ~MatrixText();
    
    /*
       Function: show_text
       Show scrolling text on the display.
       
       Parameters:
       The first parameter is the message to display, the next four specify the box to draw the text within.
       
        msg - message text to display. This is not copied, so must not be changed after calling this function.
        ul_x - Upper left x
        ul_y - Upper left y
        br_x - Bottom right x
        br_y - Bottom right y
    */
    void show_text(const char *msg, uint16_t ul_x, uint16_t ul_y, uint16_t br_x, uint16_t br_y);
    
    /*
       Function: set_scroll_speed
         Set how fast the text should scroll. 
       
       Parameters:
         speed - how long to wait (in ms) before advancing the text (default 50ms). Setting to zero causes the text to advance every time <loop> is called.
    */
    void set_scroll_speed(uint16_t speed);

    /*
       Function: set_character_spacing
         Set the spacing between each character. Defaults to 1.
       
       Parameters:
         pixels - Sets the amount of space, in pixels, between each character. Defaults to 1 if not set.
    */
    void set_character_spacing(uint8_t pixels);
    
    /*
       Function: loop
         Call loop repeatedly to keep the text scrolling. Will only update the display if it's required based on the speed set using <set_scroll_speed>.
    */    
    void loop();


  private:
    set_xy_fuct _set_xy;
    const char *_msg;
    uint16_t _msg_len;
    uint16_t _ul_x;
    uint16_t _ul_y;
    uint16_t _br_x; 
    uint16_t _br_y;
    
    int16_t _position;
    uint16_t _scroll_speed;
    unsigned long _last_move;
    uint8_t _character_spacing;
};

#endif
