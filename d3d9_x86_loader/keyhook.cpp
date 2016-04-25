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

bool	key_LockInput = false;


void SetInputLock(bool lock)
{
	key_LockInput = lock;
}

bool GetInputLock()
{
	return key_LockInput;
}

void ActivateApp();
bool WindowActive = true;
union KeyState
{
	LPARAM lparam;

	struct
	{
		unsigned nRepeatCount : 16;
		unsigned nScanCode : 8;
		unsigned nExtended : 1;
		unsigned nReserved : 4;
		unsigned nContext : 1;
		unsigned nPrev : 1;
		unsigned nTrans : 1;
	};
};
KeyState ks;
extern bool show;
static LRESULT CALLBACK wnd_proc(HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (show)
	{
		if (TwEventWin(wnd, umsg, wparam, lparam))
		{
			return 0;   // Event has been handled by AntTweakBar
		}
	}
	switch (umsg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		//process_key((umsg == WM_LBUTTONDOWN), VK_LBUTTON, 0, 0, 0, wnd);
		//break;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		//process_key((umsg == WM_RBUTTONDOWN), VK_RBUTTON, 0, 0, 0, wnd);
		//break;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		//process_key((umsg == WM_MBUTTONDOWN), VK_MBUTTON, 0, 0, 0, wnd);
		//break;

		/* :D */
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_MOUSEMOVE:
	{
		/*unsigned long	p = (unsigned long)lparam;
		int				down = (umsg == WM_KEYDOWN || umsg == WM_SYSKEYDOWN);
		int				vkey = (int)wparam;

		unsigned int	repeat = (p >> 0) & 0x7FFF;
		unsigned int	scancode = (p >> 16) & 0x00FF;
		unsigned int	extended = (p >> 24) & 0x0001;

		switch (vkey)
		{
		case VK_SHIFT:
			if (scancode == MapVirtualKey(VK_LSHIFT, 0))
				vkey = VK_LSHIFT;
			else if (scancode == MapVirtualKey(VK_RSHIFT, 0))
				vkey = VK_RSHIFT;
			break;

		case VK_CONTROL:
			if (scancode == MapVirtualKey(VK_LCONTROL, 0))
				vkey = VK_LCONTROL;
			else if (scancode == MapVirtualKey(VK_RCONTROL, 0))
				vkey = VK_RCONTROL;
			break;

		case VK_MENU:
			if (scancode == MapVirtualKey(VK_LMENU, 0))
				vkey = VK_LMENU;
			else if (scancode == MapVirtualKey(VK_RMENU, 0))
				vkey = VK_RMENU;
			break;
		}

		if (KEY_DOWN(VK_LMENU) && vkey == VK_LMENU && down)
			break;
		if (KEY_UP(VK_LMENU) && vkey == VK_LMENU && !down)
			break;
	
		ks.lparam = lparam;
		if (GetForegroundWindow() == wnd)
		{
			//process_key(vkey, down, !down, ks.nPrev == down);
		}*/
		if (key_LockInput)
			return 0;

	}
		break;
	case WM_ACTIVATEAPP:
		ActivateApp();
		WindowActive = (bool)wparam;
		break;
	}
//wnd_proc_original:
	return CallWindowProc(orig_wndproc, wnd, umsg, wparam, lparam);
}

bool keyhook_maybe_install(HWND wnd)
{
	if (orig_wndproc == NULL || wnd != orig_wnd)
	{
		keyhook_uninstall();
		orig_wndproc = (WNDPROC)(UINT_PTR)SetWindowLong(wnd, GWL_WNDPROC, (LONG)(UINT_PTR)wnd_proc);
		orig_wnd = wnd;
		return true;
	}
	return false;
}

void keyhook_uninstall(void)
{
	if (orig_wnd != NULL)
	{
		SetWindowLong(orig_wnd, GWL_WNDPROC, (LONG)(UINT_PTR)orig_wndproc);
		orig_wnd = NULL;
	}
}

bool KEY_DOWN(int v)
{
	if (WindowActive)
	{
		short value = GetAsyncKeyState(v);
		byte pressed = (byte)(value & 0xFFu);
		byte down = (byte)((value >> 8) & 0xFFu);
		return (bool)GetAsyncKeyState(v);
	}
	else return false;
}

bool KEY_UP(int v)
{
	if (WindowActive)
	{
		short value = GetAsyncKeyState(v);
		byte pressed = (byte)(value & 0xFFu);
		byte down = (byte)((value >> 8) & 0xFFu);
		return (bool)down;
	}
	else return true;
}

bool KEY_PRESSED(int v)
{
	if (WindowActive)
	{
		short value = GetAsyncKeyState(v);
		byte pressed = (byte)(value & 0xFFu);
		byte down = (byte)((value >> 8) & 0xFFu);
		return (bool)pressed;
	}
	else return false;
}

bool KEY_RELEASED(int v)
{
	if (WindowActive)
	{
		short value = GetAsyncKeyState(v);
		byte pressed = (byte)(value & 0xFFu);
		byte down = (byte)((value >> 8) & 0xFFu);
		return (bool)pressed;
	}
	else return false;
}

/*static void process_key(int down, int vkey, int repeat, int scancode, int extended, HWND wnd)
{
if (GetForegroundWindow() == wnd)
{

return;
}
}*/
