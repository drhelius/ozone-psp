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
/*						    TextFont.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "TextFont.h"


//--------------------------------------------------------------------
// Función:    CTextFont::CTextFont
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:37:23
//--------------------------------------------------------------------
CTextFont::CTextFont(void)
{
}


//--------------------------------------------------------------------
// Función:    CTextFont::~CTextFont
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:37:21
//--------------------------------------------------------------------
CTextFont::~CTextFont(void)
{
	Log("CTextFont: Destructor");
	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture);
}


//--------------------------------------------------------------------
// Función:    CTextFont::Draw
// Propósito:  
// Fecha:      sábado, 11 de noviembre de 2006, 14:54:38
//--------------------------------------------------------------------
void CTextFont::Draw(const char* const szString, int x, int y, u32 color)
{	
	int len = (int)strlen(szString);
	
	if(!len) {
		return;
	}

	color = MAT_RGBToBGR(color);

	VERT* v = (VERT*)sceGuGetMemory(sizeof(VERT) * 2 * len);

	int i;
	for(i = 0; i < len; i++) {
		unsigned char c = (unsigned char)szString[i];
		if(c < 32) {
			c = 0;
		} else if(c >= 96) {
			c = 0;
		}
		else if (c == 165)
		{
			c=63;
		}
		else if (c == 169)
		{
			c=62;
		}
		else
		{
			c-=32;
		}

		float tx = (float)((c & 0xf) << 4);
		float ty = (float)((c >> 4) << 4);

		VERT* v0 = &v[i*2+0];
		VERT* v1 = &v[i*2+1];

		v0->s = tx;
		v0->t = ty;
		v0->c = color;
		v0->x = (float)(x);
		v0->y = (float)(y);
		v0->z = 0.0f;

		v1->s = tx + 16.0f;
		v1->t = ty + 16.0f;
		v1->c = color;
		v1->x = (float)(x + 16.0f);
		v1->y = (float)(y + 16.0f);
		v1->z = 0.0f;

		x += m_SizeArray[c];
	}

	sceGuDisable(GU_DEPTH_TEST);

	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

	sceGuTexImage(0,m_pTexture->w, m_pTexture->h, m_pTexture->w, m_pTexture->pTextureData);

	sceGumDrawArray(GU_SPRITES, 
		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
		len * 2, 0, v
		);

	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDisable(GU_BLEND);
}


//--------------------------------------------------------------------
// Función:    CTextFont::Init
// Propósito:  
// Fecha:      sábado, 11 de noviembre de 2006, 14:49:59
//--------------------------------------------------------------------
void CTextFont::Init(const char* const szTexture, const char* const szSizeFile)
{
	Log("Cargando TextFont: %s %s", szTexture, szSizeFile);
	
	m_pTexture = CTextureManager::GetSingleton()->GetTexture(szTexture, 256, 64);	

	int fd = sceIoOpen(szSizeFile, PSP_O_RDONLY, 0777);

	if(fd <= 0)
	{ 
		Log("Imposible abrir font size: %s", szSizeFile);
		return;
	} 

	sceIoRead(fd, m_SizeArray, sizeof(u8) * 64); 

	m_SizeArray[0] = 10;
	
	sceIoClose(fd);

	Log("TextFont cargada");
}

/********************************************************************/
/********************************************************************/
/*						End TextFont.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


