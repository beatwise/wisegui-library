/*
  WiseGui Library

  file: wisegui.cpp

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

#include <windows.h>
#include <map>
#include <gdiplus.h>
using namespace Gdiplus;

#include "wg.h"



int g_users_count = 0;

HINSTANCE g_hInstance;
BitmapPool g_BitmapPool;

GdiplusStartupInput gdiplusStartupInput;
GdiplusStartupOutput gdiplusStartupOutput;
ULONG_PTR gdiplusBGThreadToken = NULL;
ULONG_PTR gdiplusToken = NULL;

void WiseGui_Init()
{
	

	g_users_count++;
	if (g_users_count > 1)
		return;

	Window::Init();

	if (gdiplusToken == NULL)
	{
		gdiplusStartupInput.SuppressBackgroundThread = TRUE;

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, 
			&gdiplusStartupOutput);	

		Status stat = gdiplusStartupOutput
			.NotificationHook(&gdiplusBGThreadToken);
	}
}

void WiseGui_Terminate()
{
	g_users_count--;
	if (g_users_count > 0)
		return;

	g_BitmapPool.Clear();

	Window::Deinit();

	if (gdiplusToken != NULL)
	{
		gdiplusStartupOutput.NotificationUnhook(gdiplusBGThreadToken);
		GdiplusShutdown(gdiplusToken);
	}		
}

