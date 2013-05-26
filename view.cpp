/*
  WiseGui Library

  file: view.cpp

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

#include "view.h"
#include "paraminfo.h"

#include "wg.h"

View::View(ControlListener *listener)
{
	_hWnd = NULL;
	_gbuf = NULL;
	_bmp = NULL;
	_w = _h = 0;
	_backcolor = Color::Black;
	_selected = NULL;
	_listener = listener;
	_dispose = false;
	_data = NULL;
}

View::~View()
{
	for (ClstPtr c = _cl.begin(); c != _cl.end(); c++)
		delete (*c);		
}

void *View::GetData() { return _data; } 
void View::SetData(void *data) { _data = data; }
HWND View::GetWindowHandle() { return _hWnd; }

void View::SetWindowHandle(HWND hWnd)
{
	_hWnd = hWnd;
	OnLoad();
}

void View::Add(ControlPtr c)
{
	int index = c->GetParamIndex();

	if (index > -1)
	{
		ParamInfo *pi = (ParamInfo *)_listener->GetParamInfo(index);
		if (pi != NULL)
		{
			c->SetMin(pi->GetMin());
			c->SetMax(pi->GetMax());
		}
	}

	c->_listener = _listener;
	c->Create();
	_cl.push_back(c);

	if (index > -1)
	{
		double value = _listener->GetParamValue(index);
		c->SetNormalValue(value);
	}
}

void View::UpdateControls()
{
	for (ClstPtr c = _cl.begin(); c != _cl.end(); c++)
	{
		ControlPtr p = *c;

		int index = p->GetParamIndex();
		double value = _listener->GetParamValue(index);
		p->SetNormalValue(value);
	}

	OnLoad();

	if (_hWnd)
		InvalidateRect(_hWnd, NULL, FALSE);
}

ControlListener *View::GetListener()
{
	return (ControlListener *)_listener;
}

void View::SetSize(int w, int h)
{
	if (w <= 0 || h <= 0)
		return;

	if (_w == w && _h == h)
		return;

	if (_gbuf != NULL)
		delete _gbuf;

	_gbuf = new Bitmap(w, h);
	_w = w; 
	_h = h;
}

void View::SetBackColor(int a, int r, int g, int b)
{
	_backcolor = Color(a, r, g, b);
}

void View::SetImageId(int id) { _bmp = g_BitmapPool.Load(id); }

void View::UpdateControl(int index, int update_type)
{
	if (update_type == UPDATE_REQ_TYPE_INDIRECT)
	{
		double value = _listener->GetParamValue(index);
		SetControlValue(index, value); 
		OnUpdate(index, value);
	}
	else if (update_type == UPDATE_REQ_TYPE_DIRECT)
	{
		ControlPtr c = GetControl(index);
		if (c != NULL)
		{
			Redraw(c);
			double value; c->GetNormalValue(&value);
			OnUpdate(index, value);
		}
	}
}

void View::SetControlValue(int index, double value, 
	bool normalize, bool redraw)
{
	for (ClstPtr c = _cl.begin(); c != _cl.end(); c++)
	{
		ControlPtr p = *c;
		if (p->GetParamIndex() == index)
		{
			if (normalize)
				p->SetNormalValue(value);
			else
				p->SetValue(value);

			if (p->_repaint && redraw)
				Redraw(p);
			break;
		}
	}
}

ControlPtr View::GetControl(int index)
{
	for (ClstPtr c = _cl.begin(); c != _cl.end(); c++)
	{
		ControlPtr p = *c;
		if (p->GetParamIndex() == index)
		{
			return p;
		}
	}

	return NULL;
}

void View::Paint(Graphics *g)
{
	if (_gbuf == NULL)
		return;

	Graphics *gg = Graphics::FromImage(_gbuf);
	gg->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	gg->Clear(_backcolor);
	if (_bmp)
	{
		gg->DrawImage(_bmp, 
			0, 0, _bmp->GetWidth(), _bmp->GetHeight());
	}

	for (ClstPtr c = _cl.begin(); c != _cl.end(); c++)
		(*c)->OnDraw(gg);

	delete gg;

	g->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g->DrawImage(_gbuf, 0, 0);
}

void View::MouseDown(int x, int y, int info)
{
	if (!IsLeftButton(info))
		return;

	if (_selected != NULL)
		if (_selected->HitTest(x, y))
		{
			_selected->OnMouseDown(x, y, info);
			if (_selected->_repaint)
				Redraw(_selected);
			return;
		}

	ControlPtr newsel = NULL;
	for (ClstPtr c = _cl.begin(); c != _cl.end(); c++)
		if ((*c)->HitTest(x, y))
		{   
			newsel = (*c);
			break;
		}

	if (newsel != _selected 
		&& _selected != NULL)
	{
		_selected->OnLostFocus();
		if (_selected->_repaint)
			Redraw(_selected);			
	}

	_selected = newsel;
	if (_selected != NULL)
	{
		_selected->OnMouseDown(x, y, info);
		if (_selected->_repaint)
			Redraw(_selected);
	}
}

void View::MouseMove(int x, int y, int info)
{
	if (IsLeftButton(info) && _selected != NULL)
	{
		if (_selected != NULL)
		{
			_selected->OnMouseMove(x, y, info);
			if (_selected->_repaint)
				Redraw(_selected);
		}
	}
}

void View::MouseUp(int x, int y, int info)
{
	if (_selected == NULL)
		return;

	_selected->OnMouseUp(x, y, IsLeftButton(info));
	if (_selected->_repaint)
		Redraw(_selected);
	//_selected = NULL;   
}

void View::Redraw(ControlPtr c)
{		
	if (_hWnd == NULL)
		return;

	HDC hdc = GetDC(_hWnd);
	Graphics g(hdc);

	Graphics *gg = Graphics::FromImage(_gbuf);

	SolidBrush b(_backcolor);
	gg->FillRectangle(&b, c->_dirty_rect);
	if (_bmp)
	{
		gg->DrawImage(_bmp, 
		c->_dirty_rect.X, c->_dirty_rect.Y,
		c->_dirty_rect.X, c->_dirty_rect.Y,
		c->_dirty_rect.Width, c->_dirty_rect.Height,
		UnitPixel);
	}

	for (ClstPtr cc = _cl.begin(); cc != _cl.end(); cc++)
		if ((*cc)->_dirty_rect.IntersectsWith(c->_dirty_rect))
		{   
			if (*cc != c)
				(*cc)->OnDraw(gg);
		}

	c->OnDraw(gg);

	delete gg;

	g.DrawImage(_gbuf, 
		c->_dirty_rect.X, c->_dirty_rect.Y,
		c->_dirty_rect.X, c->_dirty_rect.Y,
		c->_dirty_rect.Width, c->_dirty_rect.Height,
		UnitPixel
		);

	ReleaseDC(_hWnd, hdc);
	c->_repaint = false;
}


