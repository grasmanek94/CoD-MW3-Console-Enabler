/*
	MW3 Console plugin by Gamer_Z a.k.a. grasmanek94

	Copyright (c) 2014, Rafal Grasman
	All rights reserved.
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	* Neither the name of the creator nor the
	names of its contributors may be used to endorse or promote products
	derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Bugs:
	-Sometimes GUI doesn't show.

	Works with TeknoMW3
	Made for MW3 version 1.4.382 Sinlgeplayer.EXE, multiplayer address is: 0x4C1030
*/
#pragma once

#ifndef MYIDIRECT3DSWAPCHAIN9
#define MYIDIRECT3DSWAPCHAIN9
class myIDirect3DSwapChain9 : public IDirect3DSwapChain9
{
public:
    myIDirect3DSwapChain9(IDirect3DSwapChain9* pOriginal, IDirect3DDevice9* pDevice);
    virtual ~myIDirect3DSwapChain9(void);

    // START: The original DX9 function definitions
	HRESULT __stdcall QueryInterface (REFIID riid, void** ppvObj);
    ULONG   __stdcall AddRef(void);
    ULONG   __stdcall Release(void);
    HRESULT __stdcall Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags);
    HRESULT __stdcall GetFrontBufferData(IDirect3DSurface9* pDestSurface);
    HRESULT __stdcall GetBackBuffer(UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
    HRESULT __stdcall GetRasterStatus(D3DRASTER_STATUS* pRasterStatus);
    HRESULT __stdcall GetDisplayMode(D3DDISPLAYMODE* pMode);
    HRESULT __stdcall GetDevice(IDirect3DDevice9** ppDevice);
    HRESULT __stdcall GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);
    // END: The original DX9 function definitions

private:
    IDirect3DSwapChain9   *m_pIDirect3DSwapChain9;
	IDirect3DDevice9	  *m_pIDirect3DDevice9;
};
#endif