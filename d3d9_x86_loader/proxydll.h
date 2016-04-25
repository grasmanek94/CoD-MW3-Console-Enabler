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
// proxydll.h
#pragma once

#define SDX_STATIC_LIB
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#include "d3d9.h"
#include "myIDirect3D9.h"
#include "myIDirect3DDevice9.h"
#include "myIDirect3DSwapChain9.h"
#include <string>
#include <map>
#include "keyhook.h"

#include <sstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#define TW_STATIC
#include "anttweak/include/AntTweakBar.h"

#if defined _DEBUG
#pragma comment(lib, "./Debug/AntTweakBar.lib")
#else
#pragma comment(lib, "./Release/AntTweakBar.lib")
#endif
// Exported function
IDirect3D9* WINAPI Direct3DCreate9 (UINT SDKVersion);

// regular functions
void LoadOriginalDll(void);

extern myIDirect3DSwapChain9*  gl_pmyIDirect3DSwapChain9;
extern myIDirect3DDevice9*		gl_pmyIDirect3DDevice9;
extern myIDirect3D9*			gl_pmyIDirect3D9;
extern HINSTANCE				gl_hOriginalDll;

int Command(std::string command, bool single = true, int num = 0);
std::string string_format(const std::string fmt, ...);
void LoadOriginalDll(void);
void CustomTWTerminate();
extern bool WindowActive;