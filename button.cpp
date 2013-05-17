/*
  WiseGui Library

  file: button.cpp

  Copyright (c) 2013, Alessandro De Santis
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of its contributors may 
      be used to endorse or promote products derived from this software 
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "button.h"
#include "wg.h"

Button::Button() 
{ 
	_type = CTYPE_NONE; 
	_pressed = false;
}

Button::~Button() {}

void Button::SetImageId(int id) { _bmp = g_BitmapPool.Load(id); }

void Button::Create() 
{	
	_mouse_rect = Gdiplus::Rect(_x, _y, _w, _h);
	_dirty_rect = _mouse_rect;
	_type = CTYPE_NONE;
}

void Button::OnMouseUp(int x, int y, int info) 
{
	_pressed = false;
	_repaint = true;
	Notify();
}

void Button::OnMouseDown(int x, int y, int info) 
{
	_pressed = true;
	_repaint = true;	
}

void Button::OnMouseMove(int x, int y, int info) {}

void Button::OnDraw(Graphics *g) 
{
	if (_bmp == NULL)
		return;

	Gdiplus::Rect r(_x, _y, _w, _h);
	if (_pressed == 0)
		g->DrawImage(_bmp, r, 0, 0, _w, _h, UnitPixel);	
	else 
		g->DrawImage(_bmp, r, 0, _h, _w, _h, UnitPixel);	
}

