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
/*								 Mesh.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Mesh.h"


//--------------------------------------------------------------------
// Función:    CMesh::CMesh
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:19:21
//--------------------------------------------------------------------
CMesh::CMesh(void)
{
	InitPointer(m_pVertexArray);
	InitPointer(m_pVertexArrayNormals);
	InitPointer(m_pTexture);

	m_bUsingNormals = false;
}


//--------------------------------------------------------------------
// Función:    CMesh::~CMesh
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:19:23
//--------------------------------------------------------------------
CMesh::~CMesh(void)
{
	Log("CMesh: Destructor");
	m_pVertexArray = NULL; //SafeDeleteArray(m_pVertexArray);
	m_pVertexArrayNormals = NULL;
	CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture);
}


//--------------------------------------------------------------------
// Función:    CMesh::LoadMesh
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 22:19:26
//--------------------------------------------------------------------
void CMesh::LoadMesh(const char* const szFileName, const char* const szTexture, int iTexWidth, int iTexHeight, bool allocateInVRAM)
{
	Log("Cargando Mesh: %s", szFileName);

	int fd = sceIoOpen(szFileName, PSP_O_RDONLY, 0777);

	if(fd <= 0)
	{ 
		Log("*** CMesh::LoadMesh() Imposible abrir Mesh: %s", szFileName);
		return;
	} 

	sceIoRead(fd, &m_iTriangleCount, sizeof(u32)); 
	sceIoRead(fd, &m_iVertexCount, sizeof(u32)); 	

	if ((m_iTriangleCount == 0) && (m_iVertexCount == 0))
	{
		m_bUsingNormals = true;
		sceIoRead(fd, &m_iTriangleCount, sizeof(u32)); 
		sceIoRead(fd, &m_iVertexCount, sizeof(u32));

		Log("!!!Mesh con normales...");
	}
	else
	{
		m_bUsingNormals = false;
	}	

	//SafeDeleteArray(m_pVertexArray);	

	if (m_bUsingNormals)
	{
		m_pVertexArrayNormals = (VERTEX3D_NORMAL*)CVideo::GetSingleton()->Allocate(sizeof(VERTEX3D_NORMAL) * m_iVertexCount);//new VERTEX3D[m_iVertexCount];
		
		for (u32 ve=0; ve<m_iVertexCount; ve++)
		{
			sceIoRead(fd, &m_pVertexArrayNormals[ve], sizeof(VERTEX3D_NORMAL)); 
		}
	}
	else
	{
		m_pVertexArray = (VERTEX3D*)CVideo::GetSingleton()->Allocate(sizeof(VERTEX3D) * m_iVertexCount);//new VERTEX3D[m_iVertexCount];
		
		for (u32 ve=0; ve<m_iVertexCount; ve++)
		{
			sceIoRead(fd, &m_pVertexArray[ve], sizeof(VERTEX3D)); 
		}
	}
	
	sceIoClose(fd);

	m_pTexture = CTextureManager::GetSingleton()->GetTexture(szTexture, iTexWidth, iTexHeight, allocateInVRAM);
	
	Log("Mesh cargada: %s", szFileName);
}


//--------------------------------------------------------------------
// Función:    CMesh::Render
// Propósito:  
// Fecha:      viernes, 10 de noviembre de 2006, 00:05:25
//--------------------------------------------------------------------
void CMesh::Render(void)
{	
	if (m_bUsingNormals)	
	{
		sceGumDrawArray(GU_TRIANGLES,VERTEX3D_FORMAT_NORMAL,m_iVertexCount,0,m_pVertexArrayNormals);
	}
	else
	{
		sceGumDrawArray(GU_TRIANGLES,VERTEX3D_FORMAT,m_iVertexCount,0,m_pVertexArray);
	}
}

/********************************************************************/
/********************************************************************/
/*							End Mesh.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////




