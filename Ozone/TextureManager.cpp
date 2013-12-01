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
/*						 TextureManager.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "TextureManager.h"


//--------------------------------------------------------------------
// Función:    CTextureManager::CTextureManager
// Propósito:  
// Fecha:      domingo, 05 de noviembre de 2006, 22:57:17
//--------------------------------------------------------------------
CTextureManager::CTextureManager(void)
{	
	m_iTextureCount = 0;
}


//--------------------------------------------------------------------
// Función:    CTextureManager::~CTextureManager
// Propósito:  
// Fecha:      domingo, 05 de noviembre de 2006, 22:57:18
//--------------------------------------------------------------------
CTextureManager::~CTextureManager(void)
{
	Log("CTextureManager: Destructor");
	Reset();
}


//--------------------------------------------------------------------
// Función:    CTextureManager::Reset
// Propósito:  
// Fecha:      domingo, 05 de noviembre de 2006, 22:57:15
//--------------------------------------------------------------------
void CTextureManager::Reset(void)
{
	Log("CTextureManager: Reset");

	LIST_ITER i;

	for(i = m_theList.begin(); i != m_theList.end(); ++i)
	{	
		stTexture* pTemp = (*i);

		Log("CTextureManager: Releasing... \"%s\"", pTemp->strName);

		if (!pTemp->pTexture->bInVRAM)
			SafeDeleteArray(pTemp->pTexture->pTextureData);

		SafeDelete(pTemp->pTexture);
		SafeDelete(pTemp);

		m_theList.erase(i);

		m_iTextureCount--;

		Log("CTextureManager: Released (%d)", m_iTextureCount);		
	}

	m_theList.clear();

	m_iTextureCount = 0;

	Log("CTextureManager: Reseted");
}


//--------------------------------------------------------------------
// Función:    const CTextureManager::GetTexture
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 14:47:36
//--------------------------------------------------------------------
TEXTURE* CTextureManager::GetTexture(const char* const szFileName, int width, int height, bool allocateInVRAM)
{	
	if (szFileName)
	{
		if (m_iTextureCount > 0)
		{
			LIST_ITER i;
			
			for(i = m_theList.begin(); i != m_theList.end(); ++i)
			{
				stTexture* pTemp = (*i);

				if (strcmp(pTemp->strName, szFileName) == 0)
				{
					pTemp->iAccessCount++;

					Log("CTextureManager: Ya Cargada (%d) \"%s\". %dx%d", pTemp->iAccessCount, pTemp->strName, pTemp->pTexture->w, pTemp->pTexture->h);

					return pTemp->pTexture;
				}		
			}
		}

		int size = width * height * 4;

		TEXTURE* returnValue = new TEXTURE;

		returnValue->w = width;
		returnValue->h = height;
		returnValue->bInVRAM = allocateInVRAM;

		int fd = sceIoOpen(szFileName, PSP_O_RDONLY, 0777);

		if (fd <= 0)
		{
			Log("*** CTextureManager::GetTexture()  No se pudo abrir: %s", szFileName);
		}

		if (allocateInVRAM)
		{		
			returnValue->pTextureData = (u8*)CVideo::GetSingleton()->Allocate(size);
			
			Log("CTextureManager: Cargando textura en VRAM \"%s\" (%#X)...", szFileName, returnValue->pTextureData);

			u8* pTemp = new u8[size];

			int read = sceIoRead(fd, pTemp, size);

			if (read != size)
			{
				Log("*** CTextureManager::GetTexture()  No se leyeron el número de bytes esperados (%d): %d", size, read);
			}

			memcpy(returnValue->pTextureData, pTemp, size);

			SafeDeleteArray(pTemp);
		}
		else
		{
			returnValue->pTextureData = new u8[size];

			int read = sceIoRead(fd, returnValue->pTextureData, size);

			if (read != size)
			{
				Log("*** CTextureManager::GetTexture()  No se leyeron el número de bytes esperados (%d): %d", size, read);
			}	
		}

		sceIoClose(fd);

		stTexture* pNew = new stTexture;

		pNew->iAccessCount = 1;	
		pNew->pTexture = returnValue;
		strcpy(pNew->strName, szFileName);

		m_theList.push_back(pNew);

		m_iTextureCount++;

		Log("CTextureManager: Cargada Textura \"%s\". %dx%d (%d)", szFileName, width, height, m_iTextureCount);		

		return returnValue;
	}
	else
	{
		return NULL;
	}
}


//--------------------------------------------------------------------
// Función:    CTextureManager::ReleaseTexture
// Propósito:  
// Fecha:      sábado, 11 de noviembre de 2006, 12:51:01
//--------------------------------------------------------------------
void CTextureManager::ReleaseTexture(TEXTURE* data)
{
	if (data)
	{
		LIST_ITER i;

		for(i = m_theList.begin(); i != m_theList.end(); ++i)
		{
			stTexture* pTemp = (*i);

			if (pTemp->pTexture == data)
			{
				if (pTemp->iAccessCount > 1)
				{
					pTemp->iAccessCount--;
					Log("CTextureManager: Release decount (%d)", pTemp->iAccessCount);		
				}
				else
				{
					Log("CTextureManager: Releasing... \"%s\"", pTemp->strName);

					if (!pTemp->pTexture->bInVRAM)
						SafeDeleteArray(pTemp->pTexture->pTextureData);

					SafeDelete(pTemp->pTexture);
					SafeDelete(pTemp);

					m_theList.erase(i);

					m_iTextureCount--;

					Log("CTextureManager: Released (%d)", m_iTextureCount);		
				}

				return;
			}		
		}

		Log("CTextureManager: ReleaseTexture() ERROR: textura no encontrada");		
	}
}



/********************************************************************/
/********************************************************************/
/*						End TextureManager.cpp						*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


