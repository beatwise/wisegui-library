/*
  WiseGui Library

  file: label.h

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

#ifndef __LABEL_
#define __LABEL_

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

#include <string>
using namespace std;

#include "control.h"

#define TEXT_ALIGN_LEFT			0
#define TEXT_ALIGN_CENTER		1
#define TEXT_ALIGN_RIGHT		2

class Label 
	: public Control
{
private:
	string _text;
	string _font_family;
	int _font_size;
	Color _font_color;
	int _align;

public:
	Label(); 
	~Label();

	void Create();
	void OnMouseUp(int x, int y, int info); 
	void OnMouseDown(int x, int y, int info);
	void OnMouseMove(int x, int y, int info);
	void OnDraw(Graphics *g);

	void SetImageId(int id);

	void SetFontFamily(char *ff);
	void SetFontSize(int fs);
	void SetFontColor(int a, int r, int g, int b);
	void SetAlignment(int align);

	void SetNormalValue(double v);
};

#endif