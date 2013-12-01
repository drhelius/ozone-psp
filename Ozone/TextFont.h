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
/*						    TextFont.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "TextureManager.h"

class CTextFont
{
	public:
		CTextFont(void);
		~CTextFont(void);
		void Draw(const char* const szString, int x, int y, u32 color);
		void Init(const char* const szTexture, const char* const szSizeFile);

	private:

		struct VERT
		{
			float s, t;
			unsigned int c;
			float x, y, z;
		};

		TEXTURE* m_pTexture;
		u8 m_SizeArray[64];		
};


//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End TextFont.h							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

