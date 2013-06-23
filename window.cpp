/*
  WiseGui Library

  file: window.cpp

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

#include "window.h"
#include "wg.h"

#define GUICLS	"bwgui"

extern HINSTANCE g_hInstance;

Window::Window(int w, int h)
{
	_width = w;
	_height = h;
	_view = NULL;
	_newview = NULL;
	_hWnd = NULL;
}

void Window::Create(void *ptr)
{
	if (_hWnd != NULL)
		return;

	_hWnd = CreateWindow(GUICLS, 
		0, WS_CHILD, 0, 0, _width, _height, 
		(HWND)ptr, NULL, g_hInstance, NULL);

	if (_view != NULL)
		_view->SetWindowHandle(_hWnd);

	SetWindowLong(_hWnd, GWL_USERDATA, (UINT)this);
	ShowWindow(_hWnd, SW_SHOW);
}

Window::~Window()
{
	Destroy();

	if (_view != NULL)
		delete _view;
	_view = NULL;	
}

void Window::Destroy()
{
	if (_hWnd != NULL)
		DestroyWindow(_hWnd);
	_hWnd = NULL;
}

View *Window::GetView()
{
	return _view;
}

void Window::SetView(View *pView)
{
	if (pView == NULL)
		return;

	if (_view != NULL)
	{
		_newview = pView;
		_newview->SetSize(_width, _height);
		_view->_dispose = true;
	}
	else
	{
		_view = pView;
		_view->SetSize(_width, _height);

		::InvalidateRect(_hWnd, NULL, FALSE);
	}
}
void Window::RequestControlUpdate(int index, double value, int req_type)
{
	if (req_type == URT_DIRECT)
		_view->SetControlValue(index, value, false, false);
	else if (req_type == URT_DIRECT2)
		_view->SetControlValue(index, value, false, false);

	PostMessage(_hWnd, WM_UPDATE_CONTROL, 
		(WPARAM)index, (LPARAM)req_type);
}

void Window::RequestViewUpdate()
{
	if (_view != NULL)
		_view->UpdateControls();
}

LRESULT Window::Messages(UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	bool handled = false;

	if (_view != NULL)
	{
		switch (message)
		{
			case WM_LBUTTONDOWN:
			{				
				SetCapture(_hWnd);

				short int x = LOWORD(lParam);
				short int y = HIWORD(lParam);
				_view->MouseDown(x, y, wParam);
				handled = true;
			}
			break;

			case WM_MOUSEMOVE:
			{
				short int x = LOWORD(lParam);
				short int y = HIWORD(lParam);
				_view->MouseMove(x, y, wParam);
				handled = true;
			}
			break;

			case WM_LBUTTONUP:
			{
				ReleaseCapture();

				short int x = LOWORD(lParam);
				short int y = HIWORD(lParam);
				_view->MouseUp(x, y, wParam);
				handled = true;
			}
			break;

			case WM_PAINT:
			{
				hdc = BeginPaint(_hWnd, &ps);

				Graphics g(hdc);
				_view->Paint(&g);

				EndPaint(_hWnd, &ps);
				handled = true;
			}
			break;

			case WM_UPDATE_CONTROL:
			{
				int index = (int)(wParam);
				int type = (int)(lParam); 
				_view->UpdateControl(index, type);
				handled = true;
			}
		}
	}

	if (!handled)
		return DefWindowProc(_hWnd, message, wParam, lParam);

	if (_view->_dispose)
	{
		delete _view;
		_view = _newview;
		_newview = NULL;
		_view->SetWindowHandle(_hWnd);
		::InvalidateRect(_hWnd, NULL, FALSE);
	}

	return 0;
}

LRESULT CALLBACK Window::wp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* obj = (Window*)GetWindowLong(hWnd, GWL_USERDATA);
	if (obj != NULL)
		return obj->Messages(message, wParam, lParam);
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::Init()
{
	WNDCLASS wc;
	wc.style			= 0;
	wc.lpfnWndProc		= wp;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= g_hInstance;
	wc.hIcon			= 0;
	wc.hCursor			= LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground	= 0;
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= GUICLS;

	RegisterClass(&wc);
}

void Window::Deinit()
{
	UnregisterClass (GUICLS, g_hInstance);
}

