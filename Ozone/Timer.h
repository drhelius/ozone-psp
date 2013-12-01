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
/*						    Timer.h									*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"

#define FPS_REFRESH_TIME	0.5f

class CTimer
{

	public:

		CTimer(void);
		~CTimer(void);
		void Start(void);
		void Stop(void);
		void Continue(void);

		float GetActualTime(void);
		float GetFrameTime(void);
		float GetDeltaTime(void);

		void Update(void);
		float GetFPS(void);

		bool IsRunning(void);


	private:
		
		bool   m_bIsRunning;	

		u64 m_i64BaseTicks;
		u64 m_i64StopedTicks;

		u64 m_i64TicksPerSecond;

		float  m_fFrameTime;
		float  m_fDeltaTime;

		u32  m_iFrameCount;
		float  m_fLastUpdate;
		float  m_fFPS;
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*						End Timer.h									*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
