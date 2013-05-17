/*
  WiseGui Library

  file: control.cpp

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

#include "control.h"

Control::Control() 
{ 
	_param_index = -1;
	_repaint = false; 
	_listener = NULL; 
	_type = 0;
	_bmp = NULL;
}

Control::~Control() { };

void Control::Notify()
{
	if (_param_index == -1)
		return;

	if (_listener != NULL)
	{
		double v;
		GetNormalValue(&v);
		_listener->ParamChanged(_param_index, v); 
	}
}

void Control::SetLocation(int x, int y) { _x = x; _y = y; }
void Control::SetSize(int w, int h) { _w = w; _h = h; }

void Control::SetMin(double min) {_min = min;}
void Control::SetMax(double max) {_max = max;}

double Control::GetValue()
{
	return _value;
}

void Control::SetValue(double value)
{
	_value = value;
	_repaint = true;
}

void Control::GetNormalValue(double *v) 
{ 
	if (_type == CTYPE_BOOL)
		*v = _value != 0.0;
	else 
	{
		*v = (double)(_value - _min)/ (double)(_max - _min) ; 
	}
}

void Control::SetNormalValue(double v) // v [0..1]
{ 
	if (v < 0 || v > 1)
		return;

	if (_type == CTYPE_BOOL)
		_value = v != 0.0;
	else if (_type == CTYPE_INT)
	{
		_value = (int)( _min + (_max - _min) * v + 0.5f);
	}

	else
	{
		_value = _min + (_max - _min) * v;
	}
	_repaint = true;
}

void Control::SetParamIndex(int index) { _param_index = index; }

int Control::GetParamIndex() { return _param_index ; }

void Control::OnLostFocus() {}

bool Control::HitTest(int x, int y) {return _mouse_rect.Contains(x, y);}	

