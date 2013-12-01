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
/*								Fade.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Fade.h"


//--------------------------------------------------------------------
// Función:    CFade::CFade
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  13/06/2007  14:38:15
//--------------------------------------------------------------------
CFade::CFade(void)
{
	m_iColor = 0;;
	m_fFadeState = 0.0f;
	m_bFadeIn = true;
	m_bFadeActive = false;
	m_fFadeTime = 0.0f;
	m_fTarget = 0.0f;
}


//--------------------------------------------------------------------
// Función:    CFade::~CFade
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  13/06/2007  14:38:17
//--------------------------------------------------------------------
CFade::~CFade(void)
{
}


//--------------------------------------------------------------------
// Función:    CFade::StartFade
// Creador:    Nacho (AMD)
// Fecha:      Thursday  14/06/2007  11:43:04
//--------------------------------------------------------------------
void CFade::StartFade(u32 color, bool fadein, float time, float target)
{
	m_fTarget = target;
	m_bFadeActive = true;
	m_bFadeIn = fadein;
	m_fFadeState = 0.0f;
	m_fFadeTime = time;
	m_iColor = color;
}


//--------------------------------------------------------------------
// Función:    CFade::Update
// Creador:    Nacho (AMD)
// Fecha:      Thursday  14/06/2007  11:56:53
//--------------------------------------------------------------------
void CFade::Update(float dt)
{
	if (m_fFadeTime > 0.0f)
	{
		m_fFadeState += dt;	

		if (m_fFadeState >= m_fFadeTime)
		{
			m_bFadeActive = false;
		}

		float alpha = 0.0f;

		///--- fade in
		if (m_bFadeIn)
		{
			alpha = 255.0f - ((m_fFadeState * 255.0f) / m_fFadeTime);
		}
		///--- fade out
		else
		{
			alpha = ((m_fFadeState * 255.0f) / m_fFadeTime);
		}

		alpha = MAT_Clampf(alpha, 0.0f, m_fTarget);		

		if (alpha > 0.0f)
		{		
			VERT* v = (VERT*)sceGuGetMemory(sizeof(VERT) * 2);
			
			VERT* v0 = &v[0];
			VERT* v1 = &v[1];

			int alpha_integer = (((int)alpha) & 0xFF) << 24;

			v0->x = -1.0f;
			v0->y = -1.0f;
			v0->z = 0.0f;
			
			v1->x = 481.0f;
			v1->y = 273.0f;
			v1->z = 0.0f;	

			sceGuColor((m_iColor & 0x00ffffff) | alpha_integer);

			sceGuDisable(GU_TEXTURE_2D);

			sceGuDisable(GU_DEPTH_TEST);		

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
			
			sceGumDrawArray(GU_SPRITES, GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, v);		

			sceGuEnable(GU_DEPTH_TEST);
			
			sceGuDisable(GU_BLEND);

			sceGuEnable(GU_TEXTURE_2D);
			sceGuColor(0xffffffff);
		}
	}
}

/********************************************************************/
/********************************************************************/
/*							End Fade.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


