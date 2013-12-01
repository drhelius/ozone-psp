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
/*						    Ball.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Ball.h"


//--------------------------------------------------------------------
// Función:    CBall::CBall
// Creador:    Nacho (AMD)
// Fecha:      lunes, 29 de enero de 2007, 19:47:15
//--------------------------------------------------------------------
CBall::CBall(void)
{
	InitPointer(m_pTexture1);
	InitPointer(m_pTexture2);
	InitPointer(m_pVideoManager);
	InitPointer(m_pQuadHalo);

	m_vecVelocity.x = 0.0f;
	m_vecVelocity.y = 0.0f;

	m_vecPosition.x = 0.0f;
	m_vecPosition.y = 0.0f;
	m_vecPosition.z = 0.0f;

	m_fHaloRotation = 0;

	m_fRot1 = 0.0f;
	m_fRot2 = 0.0f;

	m_fRadius = 2.5f;

	m_fHaloAlphaTime = 0.0f;

	m_bHasHalo = false;
}


//--------------------------------------------------------------------
// Función:    CBall::~CBall
// Creador:    Nacho (AMD)
// Fecha:      lunes, 29 de enero de 2007, 19:47:19
//--------------------------------------------------------------------
CBall::~CBall(void)
{
	Log("CBall: destructor\n");

	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture1);
	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture2);

	SafeDelete(m_pQuadHalo);
}


//--------------------------------------------------------------------
// Función:    CBall::Init
// Creador:    Nacho (AMD)
// Fecha:      lunes, 29 de enero de 2007, 20:15:21
//--------------------------------------------------------------------
void CBall::Init(void)
{
	m_pVideoManager = CVideo::GetSingleton();

	this->LoadMesh("gfx/models/ball.o3d", NULL, 0, 0);

	m_pTexture1 = CTextureManager::GetSingleton()->GetTexture("gfx/models/h_ball_1.psptex", 64, 64);
	m_pTexture2 = CTextureManager::GetSingleton()->GetTexture("gfx/models/h_ball_2.psptex", 64, 64);

	m_pQuadHalo = new CQuad(false);

	m_pQuadHalo->Init("gfx/models/h_gem_redglow.psptex", 64, 64);
}


//--------------------------------------------------------------------
// Función:    CBall::RenderBall2D
// Creador:    Nacho (AMD)
// Fecha:      Saturday  16/06/2007  12:47:07
//--------------------------------------------------------------------
void CBall::RenderBall2D(float x, float y, float s)
{
	sceGuDisable(GU_DEPTH_TEST);
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);	

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		ScePspFVector3 pos =  { x, y, 0.0f };
		ScePspFVector3 scale = { s, s, s };
		sceGumTranslate(&pos);
		sceGumRotateZ(m_fRot1);
		sceGumRotateY(MAT_ToRadians(180.0f));
		sceGumScale(&scale);
	}

	m_pVideoManager->EnableTexture(m_pTexture2);
	this->Render();

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		ScePspFVector3 pos =  { x, y, 0.0f };
		ScePspFVector3 scale = { s, s, s };
		sceGumTranslate(&pos);
		sceGumRotateZ(m_fRot2);
		sceGumRotateY(MAT_ToRadians(180.0f));
		sceGumScale(&scale);
	}

	m_pVideoManager->EnableTexture(m_pTexture1);

	this->Render();	

	sceGuDisable(GU_BLEND);
	sceGuEnable(GU_DEPTH_TEST);
}


//--------------------------------------------------------------------
// Función:    CBall::RenderBall
// Creador:    Nacho (AMD)
// Fecha:      lunes, 29 de enero de 2007, 20:43:10
//--------------------------------------------------------------------
void CBall::RenderBall(bool halo)
{
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	if (halo || (m_fHaloAlphaTime > 0.0f))
	{
		int alpha = 255;

		if (m_fHaloAlphaTime <= 1.0f)
		{
			alpha = (int)(MAT_Clampf(255.0f * m_fHaloAlphaTime, 0.0f, 225.0f));
		}

		sceGuAmbientColor(COLOR_ARGB((int)alpha, 255, 255, 255));


		///--- render

		m_bHasHalo = true;

		float scale_n = m_fRadius * 4.0f;
		float half_scale_n = scale_n * 0.5f;

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{			
			VECTOR3 pos2 = { -half_scale_n, -half_scale_n, 0.0f };
			VECTOR3 scale = { scale_n, scale_n, 1.0f };

			VECTOR3 pos =  { m_vecPosition.x + half_scale_n - (m_fRadius * 2.0f),
				-(m_vecPosition.y) + half_scale_n - (m_fRadius * 2.0f), 4.15f };

			sceGumTranslate(&pos);				
			sceGumRotateZ(MAT_ToRadians(m_fHaloRotation));
			sceGumTranslate(&pos2);
			sceGumScale(&scale);	
		}	
		
		m_pVideoManager->EnableTexture(m_pQuadHalo->GetTexture());

		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

		m_pQuadHalo->Render();

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

		sceGuAmbientColor(0xffffffff);
	}

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		ScePspFVector3 pos =  { m_vecPosition.x, -m_vecPosition.y, 4.0f };
		ScePspFVector3 scale = { m_fRadius, m_fRadius, m_fRadius };
		sceGumTranslate(&pos);
		sceGumRotateZ(m_fRot1);
		sceGumScale(&scale);
	}

	m_pVideoManager->EnableTexture(m_pTexture2);
	this->Render();

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		ScePspFVector3 pos =  { m_vecPosition.x, -m_vecPosition.y, 4.1f };
		ScePspFVector3 scale = { m_fRadius, m_fRadius, m_fRadius };
		sceGumTranslate(&pos);
		sceGumRotateZ(m_fRot2);
		sceGumScale(&scale);
	}

	m_pVideoManager->EnableTexture(m_pTexture1);
	this->Render();	

	

	sceGuDisable(GU_BLEND);
}


//--------------------------------------------------------------------
// Función:    CBall::Deflate
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  14/02/2007  19:33:29
//--------------------------------------------------------------------
void CBall::Deflate(float fDeltaTime)
{
	m_fRadius -= (0.03f * fDeltaTime);
}


//--------------------------------------------------------------------
// Función:    CBall::Inflate
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  14/02/2007  19:33:30
//--------------------------------------------------------------------
void CBall::Inflate(float fDeltaTime)
{
	m_fRadius += (0.03f * fDeltaTime);
}


//--------------------------------------------------------------------
// Función:    CBall::Update
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:46:44
//--------------------------------------------------------------------
void CBall::Update(float dt, bool halo)
{
	if (halo)
	{
		m_fHaloAlphaTime = 1.0f;
	}
	else
	{
		if (m_fHaloAlphaTime > 0.0f)
		{
			m_fHaloAlphaTime -= dt;
		}
		else
		{
			m_fHaloAlphaTime = 0.0f;
		}
	}

	m_vecPosition.x += (m_vecVelocity.x * dt);
	m_vecPosition.y += (m_vecVelocity.y * dt);	

	m_fRot1 -= dt * 0.07f;
	m_fRot1 = MAT_NormalizarAngulo360(m_fRot1);
	m_fRot2 += dt * 0.12f;
	m_fRot2 = MAT_NormalizarAngulo360(m_fRot2);

	m_fHaloRotation += 15.0f * dt;
}


//--------------------------------------------------------------------
// Función:    CBall::Update2D
// Creador:    Nacho (AMD)
// Fecha:      Saturday  16/06/2007  12:55:08
//--------------------------------------------------------------------
void CBall::Update2D(float dt)
{	
	m_fRot1 -= dt * 0.07f;
	m_fRot1 = MAT_NormalizarAngulo360(m_fRot1);
	m_fRot2 += dt * 0.12f;
	m_fRot2 = MAT_NormalizarAngulo360(m_fRot2);
}


/********************************************************************/
/********************************************************************/
/*							End Ball.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



