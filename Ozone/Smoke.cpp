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
/*								Smoke.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Smoke.h"


//--------------------------------------------------------------------
// Función:    CSmoke::CSmoke
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 11:18:15
//--------------------------------------------------------------------
CSmoke::CSmoke(void)
{
	InitPointer(m_pSmokeArray);
	InitPointer(m_pQuad);
}


//--------------------------------------------------------------------
// Función:    CSmoke::~CSmoke
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 11:18:17
//--------------------------------------------------------------------
CSmoke::~CSmoke(void)
{
	Log("CSmoke: destructor\n");

	SafeDeleteArray(m_pSmokeArray);	
	SafeDelete(m_pQuad);
}


//--------------------------------------------------------------------
// Función:    Init
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 11:51:15
//--------------------------------------------------------------------
void CSmoke::Init(void)
{
	m_pSmokeArray = new stSmokeNode[MAX_SMOKE_PARTICLES];

	for (int i=0; i<MAX_SMOKE_PARTICLES; i++)
	{		
		m_pSmokeArray[i].enable = false;
		m_pSmokeArray[i].next = i+1;
	}

	m_pSmokeArray[MAX_SMOKE_PARTICLES - 1].next = -1;

	m_iFreeSlot = 0;

	m_pQuad = new CQuad(false);
	m_pQuad->Init("gfx/models/smoke.psptex", 32, 32);
}


//--------------------------------------------------------------------
// Función:    CSmoke::AddExplosion
// Creador:    Nacho (AMD)
// Fecha:      viernes, 09 de febrero de 2007, 17:42:24
//--------------------------------------------------------------------
void CSmoke::AddExplosion(VECTOR3 pos, float vel, bool big)
{
	int pass = 0;

	if (big)
		pass = 22;
	else
		pass = 45;

	for (int i=0; i<360; i+=pass)
	{
		if (m_iFreeSlot < 0)
		{
			return;
		}

		VECTOR2 v;
		float rad = MAT_ToRadians((float)(i));
		v.x = cosf(rad) * vel;
		v.y = sinf(rad) * vel;	

		m_pSmokeArray[m_iFreeSlot].pos = pos;
		m_pSmokeArray[m_iFreeSlot].vel = v;
		m_pSmokeArray[m_iFreeSlot].radius = 4.0f;
		m_pSmokeArray[m_iFreeSlot].enable = true;

		m_iFreeSlot = m_pSmokeArray[m_iFreeSlot].next;
	}
}


//--------------------------------------------------------------------
// Función:    CSmoke::AddSmoke
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 11:59:25
//--------------------------------------------------------------------
void CSmoke::AddSmoke(VECTOR3 pos, VECTOR2 vel, float radius)
{
	if (m_iFreeSlot < 0)
	{
		return;
	}

	m_pSmokeArray[m_iFreeSlot].pos = pos;
	m_pSmokeArray[m_iFreeSlot].vel = vel;
	m_pSmokeArray[m_iFreeSlot].radius = radius;
	m_pSmokeArray[m_iFreeSlot].enable = true;

	m_iFreeSlot = m_pSmokeArray[m_iFreeSlot].next;  
}


//--------------------------------------------------------------------
// Función:    CSmoke::Update
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 12:01:18
//--------------------------------------------------------------------
void CSmoke::Update(float dt)
{	
	CVideo::GetSingleton()->EnableTexture(m_pQuad->GetTexture());

	sceGuDisable(GU_DEPTH_TEST);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

	for (int i=0; i<MAX_SMOKE_PARTICLES; i++)
	{
		if (m_pSmokeArray[i].enable)
		{
			m_pSmokeArray[i].pos.x += m_pSmokeArray[i].vel.x * dt;
			m_pSmokeArray[i].pos.y += m_pSmokeArray[i].vel.y * dt;
			m_pSmokeArray[i].radius -= 3.5f * dt;

			if (m_pSmokeArray[i].radius < 0.5f)
			{
				m_pSmokeArray[i].enable = false;
				m_pSmokeArray[i].next = m_iFreeSlot;
				m_iFreeSlot = i;
			}
			else
			{
				///--- renderizar

				sceGumMatrixMode(GU_MODEL);
				sceGumLoadIdentity();
				{
					VECTOR3 pos =  { m_pSmokeArray[i].pos.x -(m_pSmokeArray[i].radius/2.0f),
						-(m_pSmokeArray[i].pos.y + (m_pSmokeArray[i].radius/2.0f)), 4.0f };
					VECTOR3 scale = { m_pSmokeArray[i].radius, m_pSmokeArray[i].radius, 0.0f };
					//ScePspFVector3 rot = { val * 0.79f * (GU_PI/180.0f), val * 0.98f * (GU_PI/180.0f), val * 1.32f * (GU_PI/180.0f) };
					sceGumTranslate(&pos);
					sceGumScale(&scale);

					//sceGumRotateXYZ(&rot);
				}

				sceGuEnable(GU_BLEND);
				sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

				if (m_pSmokeArray[i].radius <= 2.5f)
				{
					float alpha = MAT_Clampf(((m_pSmokeArray[i].radius - 0.5f) / 2.0f) * 255.0f, 0.0f, 255.0f);

					sceGuAmbientColor(COLOR_ARGB((int)alpha, 255, 255, 255));
				}
				else
				{
					sceGuAmbientColor(0xffffffff);
				}

				m_pQuad->Render();	

				sceGuAmbientColor(0xffffffff);							
				
				sceGuDisable(GU_BLEND);
			}
		}
	}
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

	sceGuEnable(GU_DEPTH_TEST);
}


/********************************************************************/
/********************************************************************/
/*							End Smoke.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

