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

#include "LayerManager.h"

//--------------------------------------------------------------------
// Función:    CLayerManager::CLayerManager
// Propósito:  
// Fecha:      viernes, 22 julio, 2005, 12:38:44
//--------------------------------------------------------------------
CLayerManager::CLayerManager(void)
{
	m_fMax1 = 0.0f;
	m_fMin1 = 0.0f;
	m_fMax2 = 0.0f;
	m_fMin2 = 0.0f;
	m_fMax3 = 0.0f;
	m_fMin3 = 0.0f;

	InitPointer(m_pQuad);
	InitPointer(m_pTexture[0]);
	InitPointer(m_pTexture[1]);
	InitPointer(m_pTexture[2]);

	m_pVideoManager = CVideo::GetSingleton();
}


//--------------------------------------------------------------------
// Función:    CLayerManager::~CLayerManager
// Propósito:  
// Fecha:      viernes, 22 julio, 2005, 12:38:48
//--------------------------------------------------------------------
CLayerManager::~CLayerManager(void)
{
	Log("CLayerManager: destructor\n");

	End();
}


//--------------------------------------------------------------------
// Función:    CLayerManager::End
// Propósito:  
// Fecha:      lunes, 25 de julio de 2005, 13:00:11
//--------------------------------------------------------------------
void CLayerManager::End(void)
{
	SafeDelete(m_pQuad);

	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture[0]);
	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture[1]);
	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture[2]);

	for (int i=0; i<3;i++)
	{
		for (Itor itor = m_Lista[i].begin(); itor != m_Lista[i].end();)
		{			
			delete (*itor);
			itor = m_Lista[i].erase(itor);
		}
	}
}


//--------------------------------------------------------------------
// Función:    CLayerManager::Init
// Propósito:  
// Fecha:      lunes, 25 de julio de 2005, 12:54:09
//--------------------------------------------------------------------
void CLayerManager::Init(float max1, float min1, float max2, float min2, 
								float max3, float min3)
{
	m_fMax1=max1;
	m_fMin1=min1;
	m_fMax2=max2;
	m_fMin2=min2;
	m_fMax3=max3;
	m_fMin3=min3;

	m_pQuad = new CQuad(false);
	m_pQuad->Init(NULL, 0, 0);

	m_pTexture[0] = CTextureManager::GetSingleton()->GetTexture("gfx/levels/layers/h_back_1.psptex", 64, 64);
	m_pTexture[1] = CTextureManager::GetSingleton()->GetTexture("gfx/levels/layers/h_back_2.psptex", 64, 64);
	m_pTexture[2] = CTextureManager::GetSingleton()->GetTexture("gfx/levels/layers/h_back_3.psptex", 64, 64);
}


//--------------------------------------------------------------------
// Función:    CLayerManager::AddParticle
// Propósito:  
// Fecha:      lunes, 25 de julio de 2005, 12:56:44
//--------------------------------------------------------------------
void CLayerManager::AddParticle(float posx, float posy, float velx, float vely, int layer)
{
	float posz = 0.0f;

	if (layer==0)
		posz = (float)MAT_RandomInt((int)m_fMin1, (int)m_fMax1);
	else if (layer==1)
		posz = (float)MAT_RandomInt((int)m_fMin2, (int)m_fMax2);
	else if (layer==2)
		posz = (float)MAT_RandomInt((int)m_fMin3, (int)m_fMax3);

	posz+=((float)MAT_RandomInt(-100, 100))/1000.0f;

	stNodeLayer* pTemp = new stNodeLayer;

	pTemp->vel.x = velx;
	pTemp->vel.y = vely;

	pTemp->pos.x = posx;
	pTemp->pos.y = posy;
	pTemp->pos.z = posz;	
	
	pTemp->rot = (float)MAT_RandomInt(0, 1000);
	pTemp->velrot = (float)MAT_RandomInt(8, 20) * (pTemp->vel.x + pTemp->vel.y) * 1.2f;

	if (MAT_RandomInt(0, 2)==1)
		pTemp->velrot *= -1;

	if (m_Lista[layer].empty())
		m_Lista[layer].push_back(pTemp);
	else
	{
		for (Itor itor = m_Lista[layer].begin(); itor != m_Lista[layer].end(); ++itor)
		{
			if ((*itor)->pos.z > posz)
			{
				m_Lista[layer].insert(itor, pTemp);
				return;
			}
		}

		m_Lista[layer].push_back(pTemp);
	}
}


//--------------------------------------------------------------------
// Función:    CLayerManager::Render
// Propósito:  
// Fecha:      lunes, 25 de julio de 2005, 13:22:22
//--------------------------------------------------------------------
void CLayerManager::Render(int layer, CCamera* pCamera)
{
	float camPosX = -pCamera->GetPosition().x;
	float camPosY = pCamera->GetPosition().y;

	m_pVideoManager->EnableTexture(m_pTexture[layer]);

	for (Itor itor = m_Lista[layer].begin(); itor != m_Lista[layer].end(); ++itor)
	{

		if (((*itor)->pos.x > camPosX + 100.0f) || ((*itor)->pos.x < camPosX - 100.0f))
		{
			continue;			
		}

		if (((*itor)->pos.y > camPosY + 100.0f) || ((*itor)->pos.y < camPosY - 100.0f))
		{
			continue;			
		}

/*
		MATRIZ mat, matescal, mattras, matrot, mattras1, mattras2;

		MAT_MatrizTraslacion(&mattras1, -(*itor)->sizex / 40.0f, (*itor)->sizey / 40.0f, 0);
		MAT_MatrizTraslacion(&mattras2, (*itor)->sizex / 40.0f, (*itor)->sizey / 40.0f, 0);

		MAT_MatrizEscalacion(&matescal, (*itor)->sizex / 20.0f, (*itor)->sizey / 20.0f, 0);
		MAT_MatrizTraslacion(&mattras, (*itor)->pos.x, (*itor)->pos.y, (*itor)->pos.z);
		MAT_MatrizRotacionZ(&matrot, (*itor)->rot);

		mat = matescal * mattras1 * matrot * mattras2 * mattras;

		m_pDispD3D->SetTransform(D3DTS_WORLD, &mat);

		m_Panel.Renderizar((*itor)->textura);*/

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			ScePspFVector3 pos =  { (*itor)->pos.x, -(*itor)->pos.y, (*itor)->pos.z };
			if (layer == 2)
			{
				ScePspFVector3 scale = { 7.0f, 7.0f, 7.0f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);
			}
			else
			{
				ScePspFVector3 scale = { 13.0f, 13.0f, 13.0f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);
			}		
			sceGumRotateZ(MAT_ToRadians((*itor)->rot));
		}

		sceGuEnable(GU_BLEND);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

		
		m_pQuad->Render();

		sceGuDisable(GU_BLEND);
	}
}


//--------------------------------------------------------------------
// Función:    CLayerManager::Frame
// Propósito:  
// Fecha:      lunes, 25 de julio de 2005, 13:36:27
//--------------------------------------------------------------------
void CLayerManager::Frame(float delta, float maxx, float maxy)
{
	for (int i=0; i<3;i++)
	{
		for (Itor itor = m_Lista[i].begin(); itor != m_Lista[i].end(); ++itor)
		{
			(*itor)->rot += (*itor)->velrot*delta;
			
			(*itor)->pos.x = (*itor)->pos.x + ((*itor)->vel.x*delta);			
			(*itor)->pos.y = (*itor)->pos.y + ((*itor)->vel.y*delta);			
			
			
			if (((*itor)->pos.x < -50.0f) || ((*itor)->pos.x > maxx))
			{
				(*itor)->vel.x = -(*itor)->vel.x;
			}
			
			if (((*itor)->pos.y < -50.0f) || ((*itor)->pos.y > maxy))
			{
				(*itor)->vel.y = -(*itor)->vel.y;
			}			
		}
	}	
}
