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
/*								vram.c								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "vram.h"

#include <pspge.h>
#include <pspgu.h>

static unsigned int staticOffset = 0;


///--------------------------------------------------------------------
/// Función:    getMemorySize
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:52:50
///--------------------------------------------------------------------
static unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int psm)
{
	switch (psm)
	{
		case GU_PSM_T4:
			return (width * height) >> 1;

		case GU_PSM_T8:
			return width * height;

		case GU_PSM_5650:
		case GU_PSM_5551:
		case GU_PSM_4444:
		case GU_PSM_T16:
			return 2 * width * height;

		case GU_PSM_8888:
		case GU_PSM_T32:
			return 4 * width * height;

		default:
			return 0;
	}
}


///--------------------------------------------------------------------
/// Función:    getStaticVramBuffer
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:53:01
///--------------------------------------------------------------------
void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm)
{
	unsigned int memSize = getMemorySize(width,height,psm);
	void* result = (void*)staticOffset;
	staticOffset += memSize;
	return result;
}


///--------------------------------------------------------------------
/// Función:    getStaticVramTexture
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:53:14
///--------------------------------------------------------------------
void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm)
{
	void* result = getStaticVramBuffer(width,height,psm);
	return (void*)(((unsigned int)result) + ((unsigned int)sceGeEdramGetAddr()));
}


/********************************************************************/
/********************************************************************/
/*							End vram.c								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
