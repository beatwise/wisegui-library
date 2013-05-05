/*
  WiseGui Library

  file: slider.cpp

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

#include "slider.h"
#include "wg.h"

void Slider::Create() 
{
	_mouse_rect = Gdiplus::Rect(_x, _y, _w, _h);
	_dirty_rect = _mouse_rect;

	_val = 0;
}

Slider::Slider() 
{ 
	_type = CTYPE_DOUBLE;
	_bmpHandle = NULL;
	_moving = false;
}

Slider::~Slider() { }
void Slider::SetImageId(int id) { _bmp = g_BitmapPool.Load(id); }
void Slider::SetHandleId(int id) { _bmpHandle = g_BitmapPool.Load(id); }
void Slider::SetOffset(int v) { _offset = v; }
void Slider::SetLength(int v) { _length = v; }
void Slider::OnMouseUp(int x, int y, int info)
{
	_moving = false;
}



void VSlider::Create() 
{
	Slider::Create();
	_hx = _x + (_w - _bmpHandle->GetWidth()) / 2;
}

void VSlider::OnMouseDown(int x, int y, int info) 
{
	int pos = _y + _h - _offset 
		- _bmpHandle->GetHeight() / 2 - _val;

	Gdiplus::Rect hr(_hx, pos, 
		_bmpHandle->GetWidth(), 
		_bmpHandle->GetHeight());

	if (hr.Contains(x, y))
	{
		_gy = y;
		_ref = _val;
		_moving = true;
	}
}

void VSlider::OnMouseMove(int x, int y, int info) 
{
	if (!_moving)
		return;

	int delta = _gy - y;
	_ref += delta;
	_gy = y;

	if (_ref < 0)
	{
		_val = 0;
	}
	else if (_ref > _length)
	{
		_val = _length;
	}
	else
	{
		_val = _ref;
	}

	_value = _min + (_max - _min) * 
		((double)_val / _length);

	_repaint = true;

	Notify();
}

void VSlider::OnDraw(Graphics *g) 
{
	if (_bmp == NULL)
		return;

	int pos = _y + _h - _offset 
		- _bmpHandle->GetHeight() / 2 - _val;
	
	Gdiplus::Rect r(_x, _y, _w, _h);
	g->DrawImage(_bmp, r, 0, 0, _w, _h, 
		UnitPixel);	

	if (_bmpHandle == NULL)
		return;

	Gdiplus::Rect hr(_hx, pos, 
		_bmpHandle->GetWidth(), 
		_bmpHandle->GetHeight());
	g->DrawImage(_bmpHandle, hr, 0, 0, 
		_bmpHandle->GetWidth(), 
		_bmpHandle->GetHeight(),
		UnitPixel);	
}

void VSlider::SetNormalValue(double v)
{ 
	Control::SetNormalValue(v);
	_val = (_length) 
		* (_value - _min) / (_max - _min);
}



void HSlider::Create() 
{
	Slider::Create();
	_hy = _y + (_h - _bmpHandle->GetHeight()) / 2;
}

void HSlider::OnMouseDown(int x, int y, int info) 
{
	int pos = _x + _offset + _val 
		- _bmpHandle->GetWidth() / 2;

	Gdiplus::Rect hr(pos, _hy,
		_bmpHandle->GetWidth(), 
		_bmpHandle->GetHeight());

	if (hr.Contains(x, y))
	{
		_gx = x;
		_ref = _val;
		_moving = true;
	}
}

void HSlider::OnMouseMove(int x, int y, int info) 
{
	if (!_moving)
		return;

	int delta = x - _gx ;
	_ref += delta;
	_gx = x;

	if (_ref < 0)
	{
		_val = 0;
	}
	else if (_ref > _length)
	{
		_val = _length;
	}
	else
		_val = _ref;

	_value = _min + (_max - _min) * 
		((double)_val / _length);


	_repaint = true;

	Notify();
}

void HSlider::OnDraw(Graphics *g) 
{
	if (_bmp == NULL)
		return;

	int pos = _x + _offset + _val 
		- _bmpHandle->GetWidth() / 2;

	Rect r(_x, _y, _w, _h);
	g->DrawImage(_bmp, r, 0, 0, _w, _h, UnitPixel);	

	if (_bmpHandle == NULL)
		return;

	Rect hr(pos, _hy,
		_bmpHandle->GetWidth(), 
		_bmpHandle->GetHeight());
	g->DrawImage(_bmpHandle, hr, 0, 0, 
		_bmpHandle->GetWidth(), 
		_bmpHandle->GetHeight(),
		Gdiplus::UnitPixel);	
}

void HSlider::SetNormalValue(double v)
{ 
	Control::SetNormalValue(v);
	_val = _length * (_value - _min) / (_max - _min);
}

