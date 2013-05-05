/*
  WiseGui Library

  file: window.h

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

#ifndef __WINDOW_
#define __WINDOW_

#include <windows.h>
#include "view.h"

#define IsLeftButton(a)		(MK_LBUTTON & a)
#define IsRightButton(a)	(MK_RBUTTON & a)

#define WM_UPDATE_CONTROL	WM_APP + 0
#define WM_UPDATE_ALL		WM_APP + 1

class Window
{
private:
	View *_view, *_newview;
	HWND _hWnd;

	int _width;
	int _height;

public:
	Window(int w, int h);
	virtual ~Window();

	void Create(void *ptr);
	void Destroy();

	View *GetView();
	void SetView(View *pView);
	void RequestControlUpdate(int index, double value);
	void RequestViewUpdate();

	LRESULT Messages(UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK wp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void Init();
	static void Deinit();
};


#endif
