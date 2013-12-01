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
/*								Mesh.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "TextureManager.h"
#include "Video.h"

class CMesh
{
	public:
		CMesh(void);
		~CMesh(void);
		void LoadMesh(const char* const szFileName, const char* const szTexture, int iTexWidth,
			int iTexHeight, bool allocateInVRAM = false);
		void Render(void);
		inline TEXTURE* GetTexture(void) { return m_pTexture; };

	private:

		TEXTURE* m_pTexture;
		VERTEX3D* m_pVertexArray;
		VERTEX3D_NORMAL* m_pVertexArrayNormals;

		u32 m_iVertexCount;
		u32 m_iTriangleCount;	

		bool m_bUsingNormals;
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Mesh.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
