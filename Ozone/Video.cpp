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
/*								Video.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Video.h"


//--------------------------------------------------------------------
// Función:    CVideo::CVideo
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:38:10
//--------------------------------------------------------------------
CVideo::CVideo(void)
{
	InitPointer(m_pFrameBuffer0);
	InitPointer(m_pFrameBuffer1);
	InitPointer(m_pDepthBuffer);	
}


//--------------------------------------------------------------------
// Función:    CVideo::~CVideo
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:38:12
//--------------------------------------------------------------------
CVideo::~CVideo(void)
{	
	Log("CVideo: Destructor");
	sceGuTerm();
}


//--------------------------------------------------------------------
// Función:    CVideo::Init
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:38:08
//--------------------------------------------------------------------
void CVideo::Init(void)
{
	Log("CVideo: Init");

	m_pFrameBuffer0 = getStaticVramBuffer(PSP_BUF_WIDTH, PSP_SCR_HEIGHT, GU_PSM_8888);
	m_pFrameBuffer1 = getStaticVramBuffer(PSP_BUF_WIDTH, PSP_SCR_HEIGHT, GU_PSM_8888);
	m_pDepthBuffer = getStaticVramBuffer(PSP_BUF_WIDTH, PSP_SCR_HEIGHT,GU_PSM_4444);

	ClearVRAM();

	sceGuInit();

	sceGuStart(GU_DIRECT, m_DisplayList);
	sceGuDrawBuffer(GU_PSM_8888, m_pFrameBuffer0, PSP_BUF_WIDTH);
	sceGuDispBuffer(PSP_SCR_WIDTH, PSP_SCR_HEIGHT, m_pFrameBuffer1, PSP_BUF_WIDTH);
	sceGuDepthBuffer(m_pDepthBuffer, PSP_BUF_WIDTH);
	sceGuOffset(2048 - (PSP_SCR_WIDTH/2), 2048 - (PSP_SCR_HEIGHT/2));
	sceGuViewport(2048, 2048, PSP_SCR_WIDTH, PSP_SCR_HEIGHT);
	sceGuDepthRange(65535, 0);
	sceGuScissor(0, 0, PSP_SCR_WIDTH, PSP_SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CCW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuDisable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);

	sceGuTexMode(GU_PSM_8888,0,0,GU_TRUE);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	sceGuTexScale(0.5f,0.5f);
	sceGuTexOffset(0.0f,0.0f);	
	sceGuAmbientColor(0xffffffff);
	sceGuTexEnvColor(0xffffffff);
	sceGuColor(0xffffffff);

	sceGuFinish();
	sceGuSync(0, 0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
}


//--------------------------------------------------------------------
// Función:    CVideo::Allocate
// Propósito:  
// Fecha:      sábado, 16 de diciembre de 2006, 19:33:18
//--------------------------------------------------------------------
void* CVideo::Allocate(u32 iBytes)
{
	void* ret = (void*)(m_iCurrentVRAMpointer);
		
	m_iCurrentVRAMpointer += iBytes;

	Log("CVideo: VRAM Allocate %d bytes (Libres: %d)", iBytes, VRAM_SCRATCH_PAD_START - (int)m_iCurrentVRAMpointer);

	return ret;
}


//--------------------------------------------------------------------
// Función:    CVideo::ClearVRAM
// Propósito:  
// Fecha:      sábado, 16 de diciembre de 2006, 19:34:27
//--------------------------------------------------------------------
void CVideo::ClearVRAM()
{
	Log("CVideo: VRAM Clear (Libres: %d)", VRAM_SCRATCH_PAD_START - VRAM_UTIL_START);

	m_iCurrentVRAMpointer = VRAM_UTIL_START; ///--- justo después de los 3 buffers
}


//--------------------------------------------------------------------
// Función:    CVideo::StartDrawing
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:58:34
//--------------------------------------------------------------------
void CVideo::StartDrawing(void)
{
	sceGuStart(GU_DIRECT, m_DisplayList);
}


//--------------------------------------------------------------------
// Función:    CVideo::EndDrawing
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:58:37
//--------------------------------------------------------------------
void CVideo::EndDrawing(void)
{
	sceGuFinish();
	sceGuSync(0, 0);

#ifndef DEBUG_PSP_OZONE
	sceDisplayWaitVblankStart();
#endif

	sceGuSwapBuffers();
}


//--------------------------------------------------------------------
// Función:    CVideo::Clear
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 23:52:56
//--------------------------------------------------------------------
void CVideo::Clear(u32 iColor, int iFlags)
{
	sceGuClearColor(iColor);
	sceGuClearDepth(0);
	sceGuClear(iFlags);
}


//--------------------------------------------------------------------
// Función:    CVideo::EnableTexture
// Propósito:  
// Fecha:      sábado, 27 de enero de 2007, 14:38:39
//--------------------------------------------------------------------
void CVideo::EnableTexture(TEXTURE* pTexture, bool bFromExternalRAM)
{
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);

	if (pTexture->bInVRAM || bFromExternalRAM)	
	{		
		sceGuTexImage(0,pTexture->w,pTexture->h,pTexture->w,pTexture->pTextureData);
	}
	else
	{
		sceGuCopyImage(GU_PSM_8888,0,0,pTexture->w,pTexture->h,pTexture->w,pTexture->pTextureData,0,0,pTexture->w,(void*)(VRAM_SCRATCH_PAD_START));
		sceGuTexSync();

		///--- sólo cabe una textura como máximo de 128x128x32bit a partir de VRAM_SCRATCH_PAD_START

		sceGuTexImage(0,pTexture->w,pTexture->h,pTexture->w,(void*)(VRAM_SCRATCH_PAD_START));
	}
}


//--------------------------------------------------------------------
// Función:    CVideo::RenderQuad2D
// Creador:    Nacho (AMD)
// Fecha:      Thursday  15/02/2007  18:39:14
//--------------------------------------------------------------------
void CVideo::RenderQuad2D(int x, int y, int width, int height)
{
	VERTEX3D* v = (VERTEX3D*)sceGuGetMemory(sizeof(VERTEX3D) * 2);

	v[0].u = 0;
	v[0].v = 0;
	v[0].x = (float)(x);
	v[0].y = (float)(y);
	v[0].z = 0.0f;

	v[1].u = 255;
	v[1].v = 255;	
	v[1].x = (float)(x + width);
	v[1].y = (float)(y + height);
	v[1].z = 0.0f;

	sceGumDrawArray(GU_SPRITES, GU_TEXTURE_16BIT|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 2, 0, v);
}

/********************************************************************/
/********************************************************************/
/*							End Video.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////






