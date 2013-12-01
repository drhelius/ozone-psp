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
/*						    TextureManager.h						*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "Singleton.h"
#include "Video.h"

class CTextureManager : public CSingleton <CTextureManager>
{
	private:
		
		struct stTexture
		{
			u32 iAccessCount;
			char strName[PSP_MAX_PATH];
			TEXTURE* pTexture;
		};
		
		u32 m_iTextureCount;

		std::list<stTexture*> m_theList;

		typedef std::list<stTexture*>::iterator LIST_ITER;


	public:
		CTextureManager(void);
		~CTextureManager(void);
		void Reset(void);
		TEXTURE* GetTexture(const char* const szFileName, int width, int height, bool allocateInVRAM = false);
		void ReleaseTexture(TEXTURE* data);
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*						End TextureManager.h						*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
