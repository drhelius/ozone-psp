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
/*								Smoke.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Quad.h"

class CSmoke
{
	private:

		struct stSmokeNode
		{	
			VECTOR3 pos;
			VECTOR2 vel;
			float radius;
			bool enable;
			int next;
		};

		stSmokeNode* m_pSmokeArray;

		int m_iFreeSlot;

		CQuad* m_pQuad;

	public:
		CSmoke(void);
		~CSmoke(void);

		void Init(void);
		void AddExplosion(VECTOR3 pos, float vel, bool big=false);
		void AddSmoke(VECTOR3 pos, VECTOR2 vel, float radius);
		void Update(float dt);
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Smoke.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


