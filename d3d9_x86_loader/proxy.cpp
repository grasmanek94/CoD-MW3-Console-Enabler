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
	Made for MW3 version 1.4.382 Sinlgeplayer.EXE, multiplayer.exe execute command address is: 0x4C1030 (instead of 0x426190)
*/

// proxydll.cpp
#include "proxydll.h"

typedef void(__cdecl *lpSendCommandToConsole)(int v1, char* command);
lpSendCommandToConsole SendCommandToConsole = (lpSendCommandToConsole)(0x426190);//singleplayer
//lpSendCommandToConsole SendCommandToConsole = (lpSendCommandToConsole)(0x4C1030);//multiplayer

// global variables
#pragma data_seg (".d3d9_shared")
myIDirect3DSwapChain9*  gl_pmyIDirect3DSwapChain9;
myIDirect3DDevice9*		gl_pmyIDirect3DDevice9;
myIDirect3D9*			gl_pmyIDirect3D9;
HINSTANCE				gl_hOriginalDll;
#pragma data_seg ()

//HMODULE Injected = NULL;
//bool DoTheInject = false;

#pragma warning(disable: 4996)

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return true;
}

void LoadOriginalDll(void)
{
    char buffer[MAX_PATH];
    
    // Getting path to system dir and to d3d8.dll
	::GetSystemDirectory(buffer,MAX_PATH);

	// Append dll name
	strcat(buffer,"\\d3d9.dll");
	
	// try to load the system's d3d9.dll, if pointer empty
	if (!gl_hOriginalDll) gl_hOriginalDll = ::LoadLibrary(buffer);

	// Debug
	if (!gl_hOriginalDll)
	{
		OutputDebugString("PROXYDLL: Original d3d9.dll not loaded ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}
}

void ExitInstance() 
{    

    OutputDebugString("PROXYDLL: ExitInstance called.\r\n");
	
	// Release the system's d3d9.dll
	if (gl_hOriginalDll)
	{
		::FreeLibrary(gl_hOriginalDll);
	    gl_hOriginalDll = NULL;  
	}
}

int Command(std::string command, bool single, int num)
{
	if (single)
	{
		SendCommandToConsole(num, (char *)((command + "\n").c_str()));
	}
	else
	{
		for (int i = 0; i < 2; ++i)
			SendCommandToConsole(i, (char *)((command + "\n").c_str()));
	}

	return 0;
}