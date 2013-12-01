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
/*							  Video.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "Singleton.h"

extern "C"
{
	#include "vram.h"
}

class CVideo : public CSingleton <CVideo>
{
	public:
		CVideo(void);
		~CVideo(void);
		void Init(void);
		void StartDrawing(void);
		void EndDrawing(void);
		void Clear(u32 iColor, int iFlags);
		void* Allocate(u32 iBytes);
		void ClearVRAM();
		void EnableTexture(TEXTURE* pTexture, bool bFromExternalRAM = false);
		void RenderQuad2D(int x, int y, int width, int height);

	private:

		unsigned int __attribute__((aligned(16))) m_DisplayList[262144];

		void* m_pFrameBuffer0;
		void* m_pFrameBuffer1;
		void* m_pDepthBuffer;

		u32 m_iCurrentVRAMpointer;
		
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Video.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
