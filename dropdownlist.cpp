/*
  WiseGui Library

  file: dropdownlist.cpp

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

#include "dropdownlist.h"
#include "wg.h"

#define DDL_ST_DEFAULT		0
#define DDL_ST_LIST			1


DropDownList::DropDownList() 
{ 
	_type = CTYPE_INT; 
	_visible_items = 0;
	_first_visible_item = 0;
	_selectedIndex = -1;
	_scrolling = false;
	_state = DDL_ST_DEFAULT;
	_font_color = Color::White;
}

DropDownList::~DropDownList() {}

void DropDownList::SetImageId(int id) { _bmp = g_BitmapPool.Load(id); }
void DropDownList::SetFontFamily(char *ff) {_fontFamily = ff;}
void DropDownList::SetFontSize(int fs) {_fontSize = fs;}
void DropDownList::SetFontColor(int a, int r, int g, int b)
{
	_font_color = Color(a, r, g, b);
}

void DropDownList::SetItems(char *items[], int count) 
{
	for (int i = 0; i < count; i++)
		_items.push_back(items[i]);
}	

void DropDownList::Create() 
{	
	int bmpw = _bmp->GetWidth();
	int bmph = _bmp->GetHeight();

	_default_rect = Gdiplus::Rect(_x, _y, _w, _h);

	_all_rect = Gdiplus::Rect(
		_x, _y, bmpw, bmph);

	_button_rect = Gdiplus::Rect(
		_x + bmpw - _h, _y, _h, _h);

	_list_rect = Gdiplus::Rect(
		_x, _y, bmpw - _h, bmph);

	_scrollbar_rect = Gdiplus::Rect(
		_x + bmpw - _h, _y + _h, _h, bmph - _h);


	_mouse_rect = _default_rect;
	_dirty_rect = _default_rect;

	_first_visible_item = 0;
	_visible_items = _bmp->GetHeight() / _h - 1;

	if (_items.size() > 0)
		_selectedIndex = 0;
	else
		_selectedIndex = -1;

	_scrolling = false;	
}

void DropDownList::OnMouseUp(int x, int y, int info) 
{
	_scrolling = false;
}

void DropDownList::OnLostFocus() 
{
	_state = DDL_ST_DEFAULT;
	_mouse_rect = _default_rect;
	_dirty_rect = _all_rect;
	_repaint = true;
}

void DropDownList::OnMouseDown(int x, int y, int info) 
{
	if (_state == DDL_ST_DEFAULT)
	{
		if (_button_rect.Contains(x, y))
		{
			_state = DDL_ST_LIST;
			_mouse_rect = _all_rect;
			_dirty_rect = _all_rect;
			_repaint = true;
		}
	}		
	else if (_state == DDL_ST_LIST)
	{
		if (_button_rect.Contains(x, y))
		{
			_state = DDL_ST_DEFAULT;
			_mouse_rect = _default_rect;
			_dirty_rect = _all_rect;
			_repaint = true;
		}			
		else if (_list_rect.Contains(x, y))
		{
			int index = (y - _list_rect.Y) / _h ;
			index = index - 1 + _first_visible_item;
			if (index >= _items.size())
				return;
			_selectedIndex = index;
			_state = DDL_ST_DEFAULT;
			_mouse_rect = _default_rect;
			_dirty_rect = _all_rect;
			_repaint = true;
			_value = index;
			Notify();
		}
		else if (_scrollbar_rect.Contains(x, y))
		{
			if (IsLeftButton(info))
			{
				_mouse_rect = _all_rect;
				_dirty_rect = _all_rect;
				_gy = y;
				_scrolling = true;
			}
		}
	}
}

void DropDownList::OnMouseMove(int x, int y, int info) 
{
	if (!IsLeftButton(info))
		return;

	if (_scrolling)
	{
		int delta = y - _gy;
		
		if (delta < -4)
		{
			if (_first_visible_item > 0)
				_first_visible_item--;
			_gy = y;				
		}
		else if (delta > 4)
		{
			if (_items.size() >= _visible_items)
				if (_first_visible_item < 
					_items.size() - _visible_items)
					_first_visible_item++;
			_gy = y;				
		}

		_repaint = true;
	}
}

void DropDownList::OnDraw(Graphics *g) 
{
	if (_bmp == NULL)
		return;

	if (_state == DDL_ST_DEFAULT)
	{
		g->DrawImage(_bmp, _default_rect, 0, 0, _w, _h, 
			Gdiplus::Unit::UnitPixel);	
	}
	else if (_state == DDL_ST_LIST) 
	{
		g->DrawImage(_bmp, _all_rect, 0, 0, 
			_w, _bmp->GetHeight(), 
			Gdiplus::Unit::UnitPixel);	
	}

	wstring wffname;
	wffname.assign(_fontFamily.begin(), _fontFamily.end());
	FontFamily ff(wffname.c_str());

	StringFormat sf(0, LANG_NEUTRAL);
	sf.SetLineAlignment(StringAlignment::StringAlignmentCenter);

	SolidBrush b(_font_color);

	if (_selectedIndex >= 0 && _items.size() > 0)
	{
		wstring witem;
		string item = _items[_selectedIndex];			
		witem.assign(item.begin(), item.end());
		RectF r = RectF(_x + 2, _y, _bmp->GetWidth() - _h, _h);

		Gdiplus::Font font(&ff, _fontSize,
			FontStyle::FontStyleRegular, UnitPixel);
		g->DrawString(witem.c_str(), witem.length(), 
			&font, r, &sf, &b);
	}

	if ( (_state == DDL_ST_LIST) 
		&& _items.size() > 0)
	{
		int y = 0;
		int n = _items.size() > _visible_items ? 
			_visible_items : _items.size();

		for (int i = 0; i < n; i++)
		{
			wstring witem;
			string item = _items[i + _first_visible_item];
			witem.assign(item.begin(), item.end());
			RectF r = RectF(
				_x + 2, _y + _h + y, 
				_bmp->GetWidth() - _h, _h);

			Gdiplus::Font font(&ff, _fontSize,
				FontStyle::FontStyleRegular, UnitPixel);
			g->DrawString(witem.c_str(), witem.length(), 
				&font, r, &sf, &b);

			y += _h;
		}
	}
}

void DropDownList::SetNormalValue(double v)
{ 
	Control::SetNormalValue(v);
	_selectedIndex = _value;
}

