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
/*						    Particles.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Quad.h"
#include "Mesh.h"
#include "Sound.h"

class CLevel;

class CParticles
{
	private:
		struct stParticleNode
		{	
			VECTOR3 pos;
			VECTOR2 vel;
			float radius;
			bool enable;
			float timeStamp;
			int next;
		};

		stParticleNode* m_pParticleArray[5];

		int m_iFreeSlot[5];

		CQuad* m_pQuad;

		CMesh* m_Type4;

		TEXTURE* m_pTexture[4];

		float m_fAngType3;

	public:
		CParticles(void);
		~CParticles(void);

		void Init(void);
		void AddParticle(VECTOR3 pos, VECTOR2 vel, float radius, int type);
		void Update(float dt, CLevel* pTheLevel, CSound* pSoundPlayer);
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Particles.h							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

