/*
  WiseGui Library

  file: bitmap_pool.cpp

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

#include "bitmap_pool.h"

BitmapPool::~BitmapPool() { Clear(); }

void BitmapPool::Clear()
{
	//map<wstring, Bitmap *>::iterator i;
	//for (i = _map.begin(); i != _map.end(); i++)
	//{
	//	Bitmap *b = (*i).second;
	//	delete b;
	//}

	//_map.clear();

	map<int, ResEntry>::iterator j;
	for (j = _map2.begin(); j != _map2.end(); j++)
	{
		ResEntry re = (*j).second;
		delete re.bmp;
		re.pStream->Release();
		//GlobalUnlock(re.hMem);
		//GlobalFree(re.hMem);
	}

	_map2.clear();

	list<Bitmap *>::iterator bmp;
	for (bmp = _privateBmps.begin(); bmp != _privateBmps.end(); bmp++)
	{
		::delete *bmp;
	}

	_privateBmps.clear();

}

Bitmap *BitmapPool::Load(int id)
{
	map<int, ResEntry>::iterator i;
	i = _map2.find(id);
	if (i != _map2.end())
	{
		return (*i).second.bmp;
	}

	ResEntry re = {NULL, NULL, NULL};

	HRSRC hRes = 
		FindResource(g_hInstance, MAKEINTRESOURCE(id), RT_RCDATA);
	if (!hRes)
		return NULL;

	DWORD size = SizeofResource(g_hInstance, hRes);
	if (size == 0)
		return NULL;

	HGLOBAL hGRes = LoadResource(g_hInstance, hRes);
	LPVOID pResData = LockResource(hGRes);

	if (!pResData)
		return NULL;

	re.pStream = NULL;
	//if (CreateStreamOnHGlobal(NULL, FALSE, &re.pStream) != S_OK)
	if (CreateStreamOnHGlobal(NULL, TRUE, &re.pStream) != S_OK)
		return NULL;

	// todo: verify the HRESULTs
	ULARGE_INTEGER lsize = {size, 0};
	re.pStream->SetSize(lsize);
	re.pStream->Write(pResData, size, NULL);

	LARGE_INTEGER pos = {0, 0};
	re.pStream->Seek(pos, STREAM_SEEK_SET, NULL);
	GetHGlobalFromStream(re.pStream, &re.hMem);

	re.bmp = Gdiplus::Bitmap::FromStream(re.pStream);
	if (re.bmp == NULL)
	{
		re.pStream->Release();
		//GlobalUnlock(re.hMem);
		//GlobalFree(re.hMem);
		return NULL;
	}
	
	_map2.insert(pair<int, ResEntry>(id, re)); 

	return re.bmp;
}

Bitmap *BitmapPool::CreatePrivateBitmap(int w, int h)
{
	Bitmap *pb = ::new Bitmap(w, h);
	_privateBmps.push_back(pb);
	return pb;
}
