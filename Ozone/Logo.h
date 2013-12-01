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
/*								Logo.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "TextFont.h"
#include "Mesh.h"
#include "Timer.h"
#include "Video.h"

class CLogo
{
	public:
		CLogo(void);
		~CLogo(void);
		void Init(void);
		void Update(void);

		bool IsFinished(void) { return m_bFinished; };

	private:
		CMesh* m_pMeshGear1;
		CMesh* m_pMeshGear2;
		CMesh* m_pMeshTextGeardome;

		CTextFont* m_pFont;

		CTimer* m_pMainTimer;
		CVideo* m_pVideoManager;

		bool m_bFinished;
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Logo.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



