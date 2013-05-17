/*
  WiseGui Library

  file: label.cpp

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

#include "label.h"
#include "wg.h"

Label::Label() 
{ 
	_type = CTYPE_STRING; 
	_font_family = "Verdana";
	_font_size = 10;
	_font_color = Color::White;
	_align = TEXT_ALIGN_CENTER;

	_text = "";
}

Label::~Label() { }

void Label::SetImageId(int id) { _bmp = g_BitmapPool.Load(id); }

void Label::Create() 
{
	_mouse_rect = Gdiplus::Rect(_x, _y, _w, _h);
	_dirty_rect = _mouse_rect;
}

void Label::SetAlignment(int align) {_align = align;}
void Label::SetFontFamily(char *ff) {_font_family = ff;}
void Label::SetFontSize(int fs) {_font_size = fs;}
void Label::SetFontColor(int a, int r, int g, int b)
{
	_font_color = Color(a, r, g, b);
}


void Label::OnMouseUp(int x, int y, int info) {}

void Label::OnMouseDown(int x, int y, int info) 
{
	//Notify();
}

void Label::OnMouseMove(int x, int y, int info) {}

void Label::OnDraw(Graphics *g) 
{
	if (_bmp == NULL)
		return;

	Rect r(_x, _y, _w, _h);
	g->DrawImage(_bmp, r, 0, 0, _w, _h, UnitPixel);	


	wstring wtext;	
	wtext.assign(_text.begin(), _text.end());

	SolidBrush b(_font_color);

	wstring wffname;
	wffname.assign(_font_family.begin(), _font_family.end());
	FontFamily ff(wffname.c_str());

	StringFormat sf(0, LANG_NEUTRAL);
	sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	if (_align == TEXT_ALIGN_LEFT)
		sf.SetAlignment(Gdiplus::StringAlignmentNear);
	else if (_align == TEXT_ALIGN_CENTER)
		sf.SetAlignment(Gdiplus::StringAlignmentCenter);
	else if (_align == TEXT_ALIGN_RIGHT)
		sf.SetAlignment(Gdiplus::StringAlignmentFar);

	RectF rf(_x, _y, _w, _h);
	Font font(&ff, _font_size, Gdiplus::FontStyleRegular, UnitPixel);
	g->DrawString(wtext.c_str(), wtext.length(), &font, rf, &sf, &b);

}

void Label::SetNormalValue(double v)
{
	int a = (int)v;
	char *str = (char *)a;
	if (str != NULL)
	{
		_text = str;
		_repaint = true;
	}
}
