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
/*						    Input.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Input.h"


//--------------------------------------------------------------------
// Función:    CInput::CInput
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:37:28
//--------------------------------------------------------------------
CInput::CInput(void)
{
}


//--------------------------------------------------------------------
// Función:    CInput::~CInput
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:37:30
//--------------------------------------------------------------------
CInput::~CInput(void)
{
	Log("CInput: Destructor\n");
}


//--------------------------------------------------------------------
// Función:    CInput::Init
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:50:59
//--------------------------------------------------------------------
void CInput::Init(void)
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	Log("CInput: Inicializado\n");
}


//--------------------------------------------------------------------
// Función:    CInput::Update
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:50:52
//--------------------------------------------------------------------
void CInput::Update(void)
{
	m_OldPad = m_ActualPad;

	sceCtrlPeekBufferPositive(&m_ActualPad, 1);
}


//--------------------------------------------------------------------
// Función:    CInput::ButtonPress
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:50:31
//--------------------------------------------------------------------
bool CInput::ButtonPress(PspCtrlButtons theButton)
{
	return (m_ActualPad.Buttons & theButton);
}


//--------------------------------------------------------------------
// Función:    CInput::ButtonPressed
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 19:50:29
//--------------------------------------------------------------------
bool CInput::ButtonPressed(PspCtrlButtons theButton)
{
	return ((m_ActualPad.Buttons & theButton) && !(m_OldPad.Buttons & theButton));
}


//--------------------------------------------------------------------
// Función:    CInput::AnyButtonPress
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 20:01:40
//--------------------------------------------------------------------
bool CInput::AnyButtonPress(void)
{
	if (m_ActualPad.Buttons & PSP_CTRL_SQUARE)
	{
		return true;
	}
	if (m_ActualPad.Buttons & PSP_CTRL_TRIANGLE)
	{
		return true;
	} 
	if (m_ActualPad.Buttons & PSP_CTRL_CIRCLE)
	{
		return true;
	} 
	if (m_ActualPad.Buttons & PSP_CTRL_CROSS)
	{
		return true;
	} 

	if (m_ActualPad.Buttons & PSP_CTRL_UP)
	{
		return true;
	} 
	if (m_ActualPad.Buttons & PSP_CTRL_DOWN)
	{
		return true;
	} 
	if (m_ActualPad.Buttons & PSP_CTRL_LEFT)
	{
		return true;
	} 
	if (m_ActualPad.Buttons & PSP_CTRL_RIGHT)
	{
		return true;
	}      

	if (m_ActualPad.Buttons & PSP_CTRL_START)
	{
		return true;
	}
	if (m_ActualPad.Buttons & PSP_CTRL_SELECT)
	{
		return true;
	}
	if (m_ActualPad.Buttons & PSP_CTRL_LTRIGGER)
	{
		return true;
	}
	if (m_ActualPad.Buttons & PSP_CTRL_RTRIGGER)
	{
		return true;
	} 

	return false;
}


//--------------------------------------------------------------------
// Función:    CInput::AnyButtonPressed
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 20:01:42
//--------------------------------------------------------------------
bool CInput::AnyButtonPressed(void)
{
	if ((m_ActualPad.Buttons & PSP_CTRL_SQUARE) && !(m_OldPad.Buttons & PSP_CTRL_SQUARE))
	{
		return true;
	}
	if ((m_ActualPad.Buttons & PSP_CTRL_TRIANGLE) && !(m_OldPad.Buttons & PSP_CTRL_TRIANGLE))
	{
		return true;
	} 
	if ((m_ActualPad.Buttons & PSP_CTRL_CIRCLE) && !(m_OldPad.Buttons & PSP_CTRL_CIRCLE))
	{
		return true;
	} 
	if ((m_ActualPad.Buttons & PSP_CTRL_CROSS) && !(m_OldPad.Buttons & PSP_CTRL_CROSS))
	{
		return true;
	} 

	if ((m_ActualPad.Buttons & PSP_CTRL_UP) && !(m_OldPad.Buttons & PSP_CTRL_UP))
	{
		return true;
	} 
	if ((m_ActualPad.Buttons & PSP_CTRL_DOWN) && !(m_OldPad.Buttons & PSP_CTRL_DOWN))
	{
		return true;
	} 
	if ((m_ActualPad.Buttons & PSP_CTRL_LEFT) && !(m_OldPad.Buttons & PSP_CTRL_LEFT))
	{
		return true;
	} 
	if ((m_ActualPad.Buttons & PSP_CTRL_RIGHT) && !(m_OldPad.Buttons & PSP_CTRL_RIGHT))
	{
		return true;
	}      

	if ((m_ActualPad.Buttons & PSP_CTRL_START) && !(m_OldPad.Buttons & PSP_CTRL_START))
	{
		return true;
	}
	if ((m_ActualPad.Buttons & PSP_CTRL_SELECT) && !(m_OldPad.Buttons & PSP_CTRL_SELECT))
	{
		return true;
	}
	if ((m_ActualPad.Buttons & PSP_CTRL_LTRIGGER) && !(m_OldPad.Buttons & PSP_CTRL_LTRIGGER))
	{
		return true;
	}
	if ((m_ActualPad.Buttons & PSP_CTRL_RTRIGGER) && !(m_OldPad.Buttons & PSP_CTRL_RTRIGGER))
	{
		return true;
	} 

	return false;
}


//--------------------------------------------------------------------
// Función:    CInput::GetAnalogX
// Creador:    Nacho (AMD)
// Fecha:      viernes, 02 de febrero de 2007, 23:42:12
//--------------------------------------------------------------------
unsigned char CInput::GetAnalogX(void)
{
	return m_ActualPad.Lx;
}


//--------------------------------------------------------------------
// Función:    CInput::GetAnalogY
// Creador:    Nacho (AMD)
// Fecha:      viernes, 02 de febrero de 2007, 23:42:14
//--------------------------------------------------------------------
unsigned char CInput::GetAnalogY(void)
{
	return m_ActualPad.Ly;
}

/********************************************************************/
/********************************************************************/
/*							End Input.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////







