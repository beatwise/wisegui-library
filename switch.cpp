/*
  WiseGui Library

  file: switch.cpp

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

#include "switch.h"
#include "wg.h"

Switch::Switch() 
{ 
	_type = CTYPE_INT; 
	_value = 0;
}
Switch::~Switch() { }

void Switch::SetImageId(int id) { _bmp = g_BitmapPool.Load(id); }
void Switch::SetOptionsCount(int count) { _optionsCount = count; }

void Switch::Create() 
{
	_mouse_rect = Gdiplus::Rect(_x, _y, _w, _h);
	_dirty_rect = _mouse_rect;
}

void Switch::OnMouseUp(int x, int y, int info) {}
void Switch::OnMouseMove(int x, int y, int info) {}
void Switch::OnDraw(Graphics *g) 
{
	if (_bmp == NULL)
		return;

	Gdiplus::Rect r(_x, _y, _w, _h);
	g->DrawImage(_bmp, r, 0, _h * (int)_value, _w, _h, UnitPixel);	
}



void VSwitch::OnMouseDown(int x, int y, int info) 
{
	int hh = _h / _optionsCount;
	_value = (y - _y) / hh;
	_repaint = true;
	Notify();
}

void HSwitch::OnMouseDown(int x, int y, int info) 
{
	int ww = _w / _optionsCount;
	_value = (x - _x) / ww;
	_repaint = true;
	Notify();
}

