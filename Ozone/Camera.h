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
/*								Camera.h							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"

class CCamera
{
	public:
		CCamera(void);
		~CCamera(void);
		
		void Update(void);
		void SetMode(bool bPerspective);
		void GoForward(float speed, float deltaTime);
		void GoStrafe(float speed, float deltaTime);
		void GoUp(float speed, float deltaTime);
		VECTOR3 GetPosition() const { return m_vPosition; }
		void SetPosition(VECTOR3 &val) { m_vPosition = val; }
		VECTOR3 GetRight() const { return m_vRight; }
		void SetRight(VECTOR3 &val) { m_vRight = val; }
		VECTOR3 GetUp() const { return m_vUp; }
		void SetUp(VECTOR3 &val) { m_vUp = val; }
		VECTOR3 GetLook() const { return m_vLook; }
		void SetLook(VECTOR3 &val) { m_vLook = val; }
		VECTOR3 GetTarget() const { return m_vTarget; }
		void SetTarget(VECTOR3 &val) { m_vTarget = val; }
		float GetFov() const { return m_fFov; }
		void SetFov(float val) { m_fFov = val; }
		void SetTargetMode(bool bTargetMode) { m_bTargeting = bTargetMode; };

	private:
		VECTOR3 m_vTarget;
		
		float m_fFov;
		
		float m_fPitch;
		float m_fYaw;
		float m_fRoll;

		float m_fNearPlane;
		float m_fFarPlane;

		VECTOR3 m_vPosition;
		
		VECTOR3 m_vRight;		
		VECTOR3 m_vUp;		
		VECTOR3 m_vLook;		
		
		MATRIX m_matView;
		MATRIX m_matProjection;

		bool m_bPerspective;
		bool m_bTargeting;
		
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Camera.h							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



