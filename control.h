/*
  WiseGui Library

  file: control.h

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

#ifndef __CONTROL_
#define __CONTROL_

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

#include "control_types.h"
#include "listeners.h"
#include "bitmap_pool.h"

extern BitmapPool g_BitmapPool;

class Control
{
protected:	
	Gdiplus::Rect _mouse_rect;
	int _x, _y, _w, _h;
	double _min, _max, _value;
	int _type, _param_index;
	Bitmap *_bmp;

public:
	ControlListener *_listener;
	Gdiplus::Rect _dirty_rect;
	bool _repaint;

	Control();
	virtual ~Control();
	void Notify();

	virtual void SetLocation(int x, int y);
	virtual void SetSize(int w, int h);

	void SetMin(double);
	void SetMax(double);

	double GetValue(); 
	virtual void GetNormalValue(double *v); 
	virtual void SetNormalValue(double v);
	
	void SetParamIndex(int index);
	int GetParamIndex();

	virtual void Create() = 0;
	virtual void OnMouseDown(int x, int y, int info) = 0;
	virtual void OnMouseUp(int x, int y, int info) = 0;
	virtual void OnMouseMove(int x, int y, int info) = 0;
	virtual void OnDraw(Graphics *g) = 0;
	virtual void OnLostFocus();
	virtual bool HitTest(int x, int y);
};

typedef Control* ControlPtr;

#endif
