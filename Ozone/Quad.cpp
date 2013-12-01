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
/*								Quad.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Quad.h"

VERTEX3D* CQuad::ms_pVertexArray2D = NULL;
VERTEX3D* CQuad::ms_pVertexArray3D = NULL;
int CQuad::ms_iReferenceCounter2D = 0;
int CQuad::ms_iReferenceCounter3D = 0;

//--------------------------------------------------------------------
// Función:    CQuad::CQuad
// Propósito:  
// Fecha:      martes, 28 de noviembre de 2006, 19:27:08
//--------------------------------------------------------------------
CQuad::CQuad(bool bIs2D)
{
	m_bIs2D = bIs2D;

	InitPointer(m_pVertexArray);

	if (m_bIs2D)
		ms_iReferenceCounter2D++;
	else
		ms_iReferenceCounter3D++;
}


//--------------------------------------------------------------------
// Función:    CQuad::~CQuad
// Propósito:  
// Fecha:      martes, 28 de noviembre de 2006, 19:27:12
//--------------------------------------------------------------------
CQuad::~CQuad(void)
{
	if (m_bIs2D)
	{
		ms_iReferenceCounter2D--;

		if (ms_iReferenceCounter2D == 0)
		{
			//SafeDeleteArray(ms_pVertexArray2D);
			Log("Quad: Borrado VertexArray 2D");
		}
	}
	else
	{
		ms_iReferenceCounter3D--;

		if (ms_iReferenceCounter3D == 0)
		{
			//SafeDeleteArray(ms_pVertexArray3D);
			Log("Quad: Borrado VertexArray 3D");
		}
	}

	InitPointer(m_pVertexArray);

	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture);
}


//--------------------------------------------------------------------
// Función:    CQuad::Init
// Propósito:  
// Fecha:      martes, 28 de noviembre de 2006, 19:30:16
//--------------------------------------------------------------------
void CQuad::Init(const char* const szTexture, int iTexWidth, int iTexHeight, bool allocateInVRAM)
{
	Log("Cargando Quad");

	if (m_bIs2D)
	{
		if (ms_iReferenceCounter2D == 1)
		{
			ms_pVertexArray2D = (VERTEX3D*)CVideo::GetSingleton()->Allocate(sizeof(VERTEX3D) * 6);//new VERTEX3D[6];
			Log("Quad: Creado VertexArray 2D");

			ms_pVertexArray2D[1].u = 0;
			ms_pVertexArray2D[1].v = 65535;
			ms_pVertexArray2D[1].x = 0.0f;
			ms_pVertexArray2D[1].y = 1.0f;
			ms_pVertexArray2D[1].z = 0.0f;

			ms_pVertexArray2D[0].u = 0;
			ms_pVertexArray2D[0].v = 0;
			ms_pVertexArray2D[0].x = 0.0f;
			ms_pVertexArray2D[0].y = 0.0f;
			ms_pVertexArray2D[0].z = 0.0f;

			ms_pVertexArray2D[2].u = 65535;
			ms_pVertexArray2D[2].v = 65535;
			ms_pVertexArray2D[2].x = 1.0f;
			ms_pVertexArray2D[2].y = 1.0f;
			ms_pVertexArray2D[2].z = 0.0f;	

			ms_pVertexArray2D[4].u = 65535;
			ms_pVertexArray2D[4].v = 65535;
			ms_pVertexArray2D[4].x = 1.0f;
			ms_pVertexArray2D[4].y = 1.0f;
			ms_pVertexArray2D[4].z = 0.0f;

			ms_pVertexArray2D[3].u = 0;
			ms_pVertexArray2D[3].v = 0;
			ms_pVertexArray2D[3].x = 0.0f;
			ms_pVertexArray2D[3].y = 0.0f;
			ms_pVertexArray2D[3].z = 0.0f;			

			ms_pVertexArray2D[5].u = 65535;
			ms_pVertexArray2D[5].v = 0;
			ms_pVertexArray2D[5].x = 1.0f;
			ms_pVertexArray2D[5].y = 0.0f;
			ms_pVertexArray2D[5].z = 0.0f;
		}

		m_pVertexArray = ms_pVertexArray2D;
	}
	else
	{
		if (ms_iReferenceCounter3D == 1)
		{
			ms_pVertexArray3D = (VERTEX3D*)CVideo::GetSingleton()->Allocate(sizeof(VERTEX3D) * 6);//new VERTEX3D[6];
			Log("Quad: Creado VertexArray 3D");

			ms_pVertexArray3D[0].u = 0;
			ms_pVertexArray3D[0].v = 65535;
			ms_pVertexArray3D[0].x = 0.0f;
			ms_pVertexArray3D[0].y = 0.0f;
			ms_pVertexArray3D[0].z = 0.0f;

			ms_pVertexArray3D[2].u = 0;
			ms_pVertexArray3D[2].v = 0;
			ms_pVertexArray3D[2].x = 0.0f;
			ms_pVertexArray3D[2].y = 1.0f;
			ms_pVertexArray3D[2].z = 0.0f;

			ms_pVertexArray3D[1].u = 65535;
			ms_pVertexArray3D[1].v = 65535;
			ms_pVertexArray3D[1].x = 1.0f;
			ms_pVertexArray3D[1].y = 0.0f;
			ms_pVertexArray3D[1].z = 0.0f;

			ms_pVertexArray3D[4].u = 65535;
			ms_pVertexArray3D[4].v = 65535;
			ms_pVertexArray3D[4].x = 1.0f;
			ms_pVertexArray3D[4].y = 0.0f;
			ms_pVertexArray3D[4].z = 0.0f;

			ms_pVertexArray3D[3].u = 0;
			ms_pVertexArray3D[3].v = 0;
			ms_pVertexArray3D[3].x = 0.0f;
			ms_pVertexArray3D[3].y = 1.0f;
			ms_pVertexArray3D[3].z = 0.0f;

			ms_pVertexArray3D[5].u = 65535;
			ms_pVertexArray3D[5].v = 0;
			ms_pVertexArray3D[5].x = 1.0f;
			ms_pVertexArray3D[5].y = 1.0f;
			ms_pVertexArray3D[5].z = 0.0f;
		}

		m_pVertexArray = ms_pVertexArray3D;
	}	

	m_pTexture = CTextureManager::GetSingleton()->GetTexture(szTexture, iTexWidth, iTexHeight, allocateInVRAM);

	Log("Quad cargado");
}


//--------------------------------------------------------------------
// Función:    CQuad::Render
// Propósito:  
// Fecha:      martes, 28 de noviembre de 2006, 19:29:48
//--------------------------------------------------------------------
void CQuad::Render(void)
{	
	sceGumDrawArray(GU_TRIANGLES,VERTEX3D_FORMAT,6,0,m_pVertexArray);	
}

/********************************************************************/
/********************************************************************/
/*							End Quad.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

