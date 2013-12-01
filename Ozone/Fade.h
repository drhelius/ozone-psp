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
/*								Fade.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "Timer.h"

class CFade
{
	public:
		CFade(void);
		~CFade(void);

		void StartFade(u32 color, bool fadein, float time, float target = 255.0f);

		void Update(float dt);

		bool IsFinished(void) { return !m_bFadeActive; };

	private:

		struct VERT
		{		
			float x, y, z;
		};

		u32 m_iColor;
		float m_fFadeState;
		float m_fFadeTime;
		bool m_bFadeIn;
		bool m_bFadeActive;
		float m_fTarget;

};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*								End Fade.h							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


