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
/*								Ball.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Mesh.h"
#include "Quad.h"

class CBall : public CMesh
{	
	public:
		CBall(void);
		~CBall(void);
		
		void SetPosition(VECTOR3& pos) { m_vecPosition = pos; };
		VECTOR3 GetPosition(void) { return m_vecPosition; };

		void SetVelocity(VECTOR2& vel) { m_vecVelocity = vel; };
		VECTOR2 GetVelocity(void) { return m_vecVelocity; };

		float GetRadius(void) { return m_fRadius; };
		void SetRadius(float radius) { m_fRadius = radius; };

		void Init(void);

		void RenderBall(bool halo);
		void RenderBall2D(float x, float y, float s);

		void Update(float dt, bool halo);
		void Update2D(float dt);

		void Deflate(float fDeltaTime);
		void Inflate(float fDeltaTime);

	private:

		CVideo* m_pVideoManager;

		VECTOR2 m_vecVelocity;
		VECTOR3 m_vecPosition;		
		
		TEXTURE* m_pTexture1;
		TEXTURE* m_pTexture2;

		CQuad* m_pQuadHalo;

		float m_fRadius;
		float m_fRot1;
		float m_fRot2;
		float m_fHaloRotation;
		float m_fHaloAlphaTime;

		bool m_bHasHalo;

};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Ball.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



