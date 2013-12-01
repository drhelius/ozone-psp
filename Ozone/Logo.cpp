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
/*						    Logo.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Logo.h"
#include "Game.h"


//--------------------------------------------------------------------
// Función:    CLogo::CLogo
// Propósito:  
// Fecha:      viernes, 10 de noviembre de 2006, 13:53:09
//--------------------------------------------------------------------
CLogo::CLogo(void)
{
	InitPointer(m_pMeshGear1);
	InitPointer(m_pMeshGear2);
	InitPointer(m_pMeshTextGeardome);
	InitPointer(m_pMainTimer);
	InitPointer(m_pVideoManager);
	InitPointer(m_pFont);
}


//--------------------------------------------------------------------
// Función:    CLogo::~CLogo
// Propósito:  
// Fecha:      viernes, 10 de noviembre de 2006, 13:53:10
//--------------------------------------------------------------------
CLogo::~CLogo(void)
{
	Log("CLogo: Destructor");

	SafeDelete(m_pMeshGear1);
	SafeDelete(m_pMeshGear2);
	SafeDelete(m_pMeshTextGeardome);
	SafeDelete(m_pFont);
}


//--------------------------------------------------------------------
// Función:    CLogo::Init
// Propósito:  
// Fecha:      viernes, 10 de noviembre de 2006, 14:10:04
//--------------------------------------------------------------------
void CLogo::Init(void)
{
	m_bFinished = false;

	m_pMeshGear1 = new CMesh();
	m_pMeshGear2 = new CMesh();
	m_pMeshTextGeardome = new CMesh();

	m_pFont = new CTextFont();

	m_pMainTimer = CGame::GetMainTimer();
	m_pVideoManager = CVideo::GetSingleton();

	m_pFont->Init("gfx/fonts/font.psptex", "gfx/fonts/font.dat");

	m_pMeshGear1->LoadMesh("gfx/geardome/h_gear_small.o3d", "gfx/geardome/h_gear.psptex", 256, 256);
	m_pMeshGear2->LoadMesh("gfx/geardome/h_gear_big.o3d", "gfx/geardome/h_gear.psptex", 256, 256);
	m_pMeshTextGeardome->LoadMesh("gfx/geardome/h_logo.o3d", "gfx/geardome/h_logo.psptex", 256, 256);
}


//--------------------------------------------------------------------
// Función:    CLogo::Update
// Propósito:  
// Fecha:      viernes, 10 de noviembre de 2006, 14:10:45
//--------------------------------------------------------------------
void CLogo::Update(void)
{
	static float startTime = m_pMainTimer->GetActualTime();

	float h = m_pMainTimer->GetActualTime() - startTime;

	if (h<9.3f)
	{
		float f=h+9.5f;

		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(37.0f,16.0f/9.0f,2.0f,1000.0f);

		VECTOR3 eye;

		eye.x = (400.0f * cosf(f/2.0f));
		eye.y = 90.0f +(30.0f*sinf(f/3.0f));
		eye.z = -(-200.0f+(200.0f*sinf(f/4.0f)));

		VECTOR3 look;

		look.x = (50.0f*cosf(f/3.0f))-130.0f;
		look.y = 60.0f;
		look.z = 0.0f;

		VECTOR3 up;

		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();
		sceGumLookAt(&eye, &look, &up);

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
	}
	else
	{
		m_bFinished = true;

		float f=9.3f+9.5f;

		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(37.0f,16.0f/9.0f,2.0f,1000.0f);

		VECTOR3 eye;

		eye.x = (400.0f * cosf(f/2.0f));
		eye.y = 90.0f +(30.0f*sinf(f/3.0f));
		eye.z = -(-200.0f+(200.0f*sinf(f/4.0f)));

		VECTOR3 look;

		look.x = (50.0f*cosf(f/3.0f))-130.0f;
		look.y = 60.0f;
		look.z = 0.0f;

		VECTOR3 up;

		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();
		sceGumLookAt(&eye, &look, &up);

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
	}


	TEXTURE* tex = m_pMeshTextGeardome->GetTexture();

	//m_pVideoManager->EnableTexture(m_pMeshTextGeardome->GetTexture());
	sceGuTexImage(0,tex->w, tex->h, tex->w, tex->pTextureData);
	
	sceGumMatrixMode(GU_MODEL);
	{
		sceGumLoadIdentity();
		VECTOR3 pos = { 0.0f,0.0f, 50.0f };
		
		sceGumTranslate(&pos);
	}

	m_pMeshTextGeardome->Render();

	float time = h * 20.0f;

	sceGumMatrixMode(GU_MODEL);
	{
		sceGumLoadIdentity();
		VECTOR3 pos = { -269.805f, 94.738f, 40.0f };
		VECTOR3 rot = { 0.0f, 0.0f, time / 15.0f };

		sceGumTranslate(&pos);
		sceGumRotateXYZ(&rot);
	}

	tex = m_pMeshGear2->GetTexture();

	sceGuTexImage(0,tex->w, tex->h, tex->w, tex->pTextureData);
	//m_pVideoManager->EnableTexture(m_pMeshGear2->GetTexture());
	
	m_pMeshGear2->Render();


	sceGumMatrixMode(GU_MODEL);
	{
		sceGumLoadIdentity();
		VECTOR3 pos = { -156.586f, 113.963f, 40.0f };
		VECTOR3 rot = { 0.0f, 0.0f, (time / 15.0f) * -1.5f };

		sceGumTranslate(&pos);
		sceGumRotateXYZ(&rot);
	}

	tex = m_pMeshGear1->GetTexture();

	sceGuTexImage(0,tex->w, tex->h, tex->w, tex->pTextureData);
	//m_pVideoManager->EnableTexture(m_pMeshGear1->GetTexture());

	m_pMeshGear1->Render();		
}


/********************************************************************/
/********************************************************************/
/*							End Logo.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


