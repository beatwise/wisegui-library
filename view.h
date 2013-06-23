/*
  WiseGui Library

  file: view.h

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

#ifndef __VIEW_
#define __VIEW_

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

#include <list>
using namespace std;

#include "controls.h"

//old defs
#define UPDATE_REQ_TYPE_INDIRECT	0
#define UPDATE_REQ_TYPE_DIRECT		1
//current defs
#define URT_INDIRECT	0
#define URT_DIRECT		1
#define URT_DIRECT2		2

#define MOUSE_DOWN		0
#define MOUSE_UP		1
#define MOUSE_MOVE		2

typedef list<ControlPtr> ControlsList;
typedef ControlsList::iterator ClstPtr;

class View
{
private:

	ControlsList _cl;	
	Bitmap *_gbuf;
	int _w, _h;
	Color _backcolor;
	Bitmap *_bmp;
	ControlPtr _selected;
	HWND _hWnd;
	void *_data;

protected:
	ControlListener *_listener;

public:
	void *GetData();
	void SetData(void *);
	HWND GetWindowHandle();

	bool _dispose;
	View(ControlListener *listener);
	virtual ~View();
	void Add(ControlPtr c);
	ControlPtr GetControl(int index);
	void SetWindowHandle(HWND hWnd);
	void UpdateControls();

	ControlListener *GetListener();

	void Paint(Graphics *g);
	void MouseDown(int x, int y, int info);
	void MouseMove(int x, int y, int info);
	void MouseUp(int x, int y, int info);
	void Redraw(ControlPtr c);

	void SetSize(int w, int h);
	void SetBackColor(int a, int r, int g, int b);
	void SetImageId(int id);
	void SetControlValue(int index, double value, 
		bool normalize = true, bool redraw = true); 
	void UpdateControl(int index, int update_type); 

	virtual void OnLoad() {}
	virtual void OnUpdate(int index, double value) {}
	virtual void OnMouse(int index, int event, int x, int y, int info) {}
};

#endif
