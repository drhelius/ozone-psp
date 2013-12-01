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
/*								Input.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "Singleton.h"

class CInput : public CSingleton <CInput>
{
	public:
		CInput(void);
		~CInput(void);

		void Init(void);
		void Update(void);
		bool ButtonPress(PspCtrlButtons theButton);
		bool ButtonPressed(PspCtrlButtons theButton);
		bool AnyButtonPress(void);
		bool AnyButtonPressed(void);
		unsigned char GetAnalogX(void);
		unsigned char GetAnalogY(void);

	
	private:
		SceCtrlData m_ActualPad;
		SceCtrlData m_OldPad;
		
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Input.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



