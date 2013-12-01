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
/*								Quad.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "TextureManager.h"

class CQuad
{
	public:
		CQuad(bool bIs2D);
		~CQuad(void);
		void Init(const char* const szTexture, int iTexWidth,
			int iTexHeight, bool allocateInVRAM = false);
		void Render(void);
		inline TEXTURE* GetTexture(void) { return m_pTexture; };

	private:

		TEXTURE* m_pTexture;
		static VERTEX3D* ms_pVertexArray3D;
		static VERTEX3D* ms_pVertexArray2D;
		static int ms_iReferenceCounter3D;
		static int ms_iReferenceCounter2D;
		VERTEX3D* m_pVertexArray;
		bool m_bIs2D;

};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*								End Quad.h							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



