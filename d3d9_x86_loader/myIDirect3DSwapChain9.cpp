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

#include "proxydll.h"

std::string string_format(const std::string fmt, ...)
{
	int size = 512;
	std::string str;
	va_list ap;
	while (1) {
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {
			str.resize(n);
			return str;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return str;
}

// Exported function (faking d3d9.dll's one-and-only export)
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
	bool Load = false;
	if (!gl_hOriginalDll)
	{
		LoadOriginalDll(); // looking for the "right d3d9.dll"
		Load = true;
	}

	// Hooking IDirect3D Object from Original Library
	typedef IDirect3D9 *(WINAPI* D3D9_Type)(UINT SDKVersion);
	D3D9_Type D3DCreate9_fn = (D3D9_Type)GetProcAddress(gl_hOriginalDll, "Direct3DCreate9");

	// Debug
	if (!D3DCreate9_fn)
	{
		OutputDebugString("PROXYDLL: Pointer to original D3DCreate9 function not received ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}

	// Request pointer from Original Dll. 
	IDirect3D9 *pIDirect3D9_orig = D3DCreate9_fn(SDKVersion);

	// Create my IDirect3D8 object and store pointer to original object there.
	// note: the object will delete itself once Ref count is zero (similar to COM objects)
	gl_pmyIDirect3D9 = new myIDirect3D9(pIDirect3D9_orig);

	// Return pointer to hooking Object instead of "real one"
	return (gl_pmyIDirect3D9);
}

extern D3DPRESENT_PARAMETERS*   gl_myPresentationParams;
extern HWND __hFocusWindow;

//////////////////////////////////////////////////////////////////////////////////////////////
// Function called to copy the content of a std::string (souceString) handled 
// by the AntTweakBar library to destinationClientString handled by our application
void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

// Callback function to create a bar with a given title
void TW_CALL CreateBarCB(void *clientData)
{
	const std::string *data = (const std::string *)(clientData);
	Command(data->c_str());
}
//////////////////////////////////////////////////////////////////////////////////////////////

bool TwExist = false;
std::string cmdlinetosend = "bind U \"+gostand\"";
void OnInit()
{
	if (TwExist)
		return;
	//load everything here
	if (!TwInit(TW_DIRECT3D9, gl_pmyIDirect3DDevice9))
	{
		//MessageBox(__hFocusWindow, TwGetLastError(), "Cannot initialize cxx", MB_OK | MB_ICONERROR);
		//return;
	}

	TwBar *bar = TwNewBar("Main");
	//TwDefine(string_format(" Main label='Console' fontSize=3 position='0 0' size='%d %d' valueswidth=%d resizable=false movable=false fontstyle=fixed iconifiable=false ", gl_myPresentationParams->BackBufferWidth, gl_myPresentationParams->BackBufferHeight / 6, ((gl_myPresentationParams->BackBufferWidth / 4) * 3)).c_str());
	TwDefine(string_format(" Main label='Console (use F1, ENTER to type command and F2, ENTER to execute) ' fontSize=3 position='0 0' size='640 140' valueswidth=440 resizable=false movable=false fontstyle=fixed iconifiable=false ").c_str());
	TwCopyStdStringToClientFunc(CopyStdStringToClient);
	TwAddVarRW(bar, "CommandLine", TW_TYPE_STDSTRING, &cmdlinetosend," label='Command to Execute' key=F1");
	TwAddButton(bar, "Execute", CreateBarCB, &cmdlinetosend," label='Execute' key=F2");
	TwAddSeparator(bar, "Sep1", "");
	TwExist = true;
}

void CustomTWTerminate()
{
	//unload everything here
	TwExist = false;
	TwTerminate();
}

void ActivateApp()
{
	CustomTWTerminate();
}

bool show = false;
//POINT pt;
HRESULT myIDirect3DSwapChain9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	if (keyhook_maybe_install(__hFocusWindow))
	{
		if (WindowActive)
			TwWindowSize(gl_myPresentationParams->BackBufferWidth, gl_myPresentationParams->BackBufferHeight);
	}
	if (WindowActive)
	{
		if (!TwExist)
			OnInit();
		// Some games use this one for presenting. They ignore the Device's present routine		

		// we may want to draw own things here before flipping surfaces
		if (KEY_PRESSED(VK_OEM_3))
		{
			if (show)
				show = false;
			else
				show = true;
			SetInputLock(show);
			//Command("bind U \"+gostand\"");
		}
		if (KEY_PRESSED(VK_F4) && KEY_DOWN(VK_MENU))
		{
			::exit(0);
		}

		// ... draw own stuff ...
		if (show)
		{
			if (TwExist)
			{
				TwDraw();
				//pt.x = 600;
				//pt.y = 100;
				//ClientToScreen(__hFocusWindow, &pt);
				//SetCursorPos(pt.x, pt.y);
			}
		}
	}
	// call original routine
	HRESULT hres = m_pIDirect3DSwapChain9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	return (hres);
}

ULONG myIDirect3DSwapChain9::Release(void)
{
	// ATTENTION: This is a booby-trap ! Watch out !
	// If we create our own sprites, surfaces, etc. (thus increasing the ref counter
	// by external action), we need to delete that objects before calling the original
	// Release function	

	// global var
	extern myIDirect3DSwapChain9*  gl_pmyIDirect3DSwapChain9;

	// release/delete own objects
	CustomTWTerminate();
	//unload

	// Calling original function now
	ULONG count = m_pIDirect3DSwapChain9->Release();

	if (count == 0)
	{
		// now, the Original Object has deleted itself, so do we here
		gl_pmyIDirect3DSwapChain9 = NULL;
		delete(this);  // destructor will be called automatically
	}

	return (count);
}


myIDirect3DSwapChain9::myIDirect3DSwapChain9(IDirect3DSwapChain9* pOriginal, IDirect3DDevice9* pDevice)
{
	m_pIDirect3DSwapChain9 = pOriginal; // store the pointer to original object
	m_pIDirect3DDevice9 = pDevice; 
}

myIDirect3DSwapChain9::~myIDirect3DSwapChain9(void)
{
	m_pIDirect3DSwapChain9 = NULL;
}

HRESULT myIDirect3DSwapChain9::QueryInterface (REFIID riid, void** ppvObj)
{
	// check if original dll can provide interface. then send *our* address
	*ppvObj = NULL;

	HRESULT hRes = m_pIDirect3DSwapChain9->QueryInterface(riid, ppvObj); 

	if (hRes == NOERROR)
	{
		*ppvObj = this;
	}
	
	return hRes;
}

ULONG myIDirect3DSwapChain9::AddRef(void)
{
	return(m_pIDirect3DSwapChain9->AddRef());
}

HRESULT myIDirect3DSwapChain9::GetFrontBufferData(IDirect3DSurface9* pDestSurface)
{
	return(m_pIDirect3DSwapChain9->GetFrontBufferData(pDestSurface));
}

HRESULT myIDirect3DSwapChain9::GetBackBuffer(UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	return(m_pIDirect3DSwapChain9->GetBackBuffer(iBackBuffer,Type,ppBackBuffer));
}

HRESULT myIDirect3DSwapChain9::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	return(m_pIDirect3DSwapChain9->GetRasterStatus(pRasterStatus));
}

HRESULT myIDirect3DSwapChain9::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	return(m_pIDirect3DSwapChain9->GetDisplayMode(pMode));
}

HRESULT myIDirect3DSwapChain9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return(m_pIDirect3DSwapChain9->GetDevice(ppDevice));
}

HRESULT myIDirect3DSwapChain9::GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return(m_pIDirect3DSwapChain9->GetPresentParameters(pPresentationParameters));
}
/////////////////////////
