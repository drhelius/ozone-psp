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
/*						    Camera.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Camera.h"


//--------------------------------------------------------------------
// Función:    CCamera::CCamera
// Propósito:  
// Fecha:      martes, 28 de noviembre de 2006, 22:18:14
//--------------------------------------------------------------------
CCamera::CCamera(void)
{
	Log("CCamera: constructor\n");

	m_vTarget.x = 0.0f;
	m_vTarget.y = 0.0f;
	m_vTarget.z = 0.0f;

	m_fFov = 40.0f;

	m_fPitch = m_fYaw = m_fRoll = 0.0f;

	m_fNearPlane = 2.0f;
	m_fFarPlane = 100.0f;

	m_vPosition.x = 0.0f;
	m_vPosition.y = 0.0f;
	m_vPosition.z = 0.0f;	

	m_vLook.x = 0.0f;
	m_vLook.y = 0.0f;
	m_vLook.z = 1.0f;

	m_vRight.x = 1.0f;
	m_vRight.y = 0.0f;
	m_vRight.z = 0.0f;

	m_vUp.x = 0.0f;
	m_vUp.y = 1.0f;
	m_vUp.z = -1.0f;

	m_bPerspective = true;
	m_bTargeting = false;
}

//--------------------------------------------------------------------
// Función:    CCamera::~CCamera
// Propósito:  
// Fecha:      martes, 28 de noviembre de 2006, 22:18:16
//--------------------------------------------------------------------
CCamera::~CCamera(void)
{
	Log("CCamera: destructor\n");
}


//--------------------------------------------------------------------
// Función:    CCamera::Update
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 20:08:12
//--------------------------------------------------------------------
void CCamera::Update(void)
{
	if (m_bPerspective)
	{

		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(m_fFov,16.0f/9.0f,m_fNearPlane,m_fFarPlane);

		if (m_bTargeting)
		{
			VECTOR3 up;
			up.x = 0.0f;
			up.y = 1.0f;
			up.z = 0.0f;
			gumLoadIdentity(&m_matView);
			gumLookAt(&m_matView, &m_vPosition, &m_vTarget, &up);
		}                
		else
		{
			MATRIX T, Rx, Ry, Rz;
		
			gumLoadIdentity(&m_matView);
			gumLoadIdentity(&T);
			gumLoadIdentity(&Rx);
			gumLoadIdentity(&Ry);
			gumLoadIdentity(&Rz);
			
			gumTranslate(&T, &m_vPosition);
			
			gumRotateX(&Rx, -m_fPitch);
			gumRotateY(&Ry, -m_fYaw);
			gumRotateZ(&Rz, -m_fRoll);

			gumMultMatrix(&m_matView, &Rz, &Ry);
			gumMultMatrix(&m_matView, &Rx, &m_matView);
			gumMultMatrix(&m_matView, &T, &m_matView);
		}		

		m_vRight.x = m_matView.x.x;
		m_vRight.y = m_matView.y.x;
		m_vRight.z = m_matView.z.x;

		m_vUp.x = m_matView.x.y;
		m_vUp.y = m_matView.y.y;
		m_vUp.z = m_matView.z.y;

		m_vLook.x = m_matView.x.z;
		m_vLook.y = m_matView.y.z;
		m_vLook.z = m_matView.z.z;				

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadMatrix(&m_matView);
	}
}


//--------------------------------------------------------------------
// Función:    CCamera::SetMode
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 19:17:46
//--------------------------------------------------------------------
void CCamera::SetMode(bool bPerspective)
{
	m_bPerspective = bPerspective;

	if (m_bPerspective)
	{
		Update();
	}
	else
	{
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();

		
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumOrtho(0.0f, PSP_SCR_WIDTH, PSP_SCR_HEIGHT, 0.0f, -100.0f, 100.0f);
	}
}


//--------------------------------------------------------------------
// Función:    CCamera::GoForward
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 19:13:59
//--------------------------------------------------------------------
void CCamera::GoForward(float speed, float deltaTime)
{
	VECTOR3 scaledLook = VECTOR3();

	MAT_VectorScalar(&scaledLook, &m_vLook, speed * deltaTime);

	MAT_VectorAdd(&m_vPosition, &m_vPosition, &scaledLook);	
}


//--------------------------------------------------------------------
// Función:    CCamera::GoStrafe
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 19:14:01
//--------------------------------------------------------------------
void CCamera::GoStrafe(float speed, float deltaTime)
{
	VECTOR3 scaledRight;

	MAT_VectorScalar(&scaledRight, &m_vRight, speed * deltaTime);

	MAT_VectorAdd(&m_vPosition, &m_vPosition, &scaledRight);
}


//--------------------------------------------------------------------
// Función:    CCamera::GoUp
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 19:13:56
//--------------------------------------------------------------------
void CCamera::GoUp(float speed, float deltaTime)
{
	VECTOR3 scaledUp = VECTOR3();

	MAT_VectorScalar(&scaledUp, &m_vUp, speed * deltaTime);

	MAT_VectorAdd(&m_vPosition, &m_vPosition, &scaledUp);
}

/********************************************************************/
/********************************************************************/
/*							End Camera.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



