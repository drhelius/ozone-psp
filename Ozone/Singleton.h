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

/************************* Hydra Engine *****************************/
/********************************************************************/
/*							Singleton.h								*/
/*																	*/
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"

template<typename T>
class CSingleton
{
	private:
		
		static T* ms_pSingleton;
		
	public:

		~CSingleton()
		{
			ms_pSingleton = NULL;
		}		

		static inline T* GetSingleton(void)
		{
			if (ms_pSingleton == NULL)
				ms_pSingleton = new T;

			return ms_pSingleton;
		}
};

template <typename T> T* CSingleton <T>::ms_pSingleton = NULL;

//////////////////////////////////////////////////////////////////////


/************************* Hydra Engine *****************************/
/********************************************************************/
/*							End Singleton.h							*/
/*																	*/
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
