/*
* Ozone - PSP Edition
* Copyright (C) 2009-2013 Ignacio Sanchez

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see http://www.gnu.org/licenses/
*
*/

/********************************************************************/
/********************************************************************/
/*						    callbacks.c								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

static int exitRequest = 0;


///--------------------------------------------------------------------
/// Función:    running
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:53:46
///--------------------------------------------------------------------
int running()
{
	return !exitRequest;
}


///--------------------------------------------------------------------
/// Función:    exitCallback
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:53:49
///--------------------------------------------------------------------
int exitCallback(int arg1, int arg2, void *common)
{
	exitRequest = 1;

	sceDisplayWaitVblankStart();

	return 0;
}


///--------------------------------------------------------------------
/// Función:    callbackThread
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:53:51
///--------------------------------------------------------------------
int callbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}


///--------------------------------------------------------------------
/// Función:    setupCallbacks
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:53:54
///--------------------------------------------------------------------
int setupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

/********************************************************************/
/********************************************************************/
/*							End callbacks.c							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
