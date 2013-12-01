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
/*						    Timer.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Timer.h"


//--------------------------------------------------------------------
// Función:    CTimer::CTimer
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 13:27:10
//--------------------------------------------------------------------
CTimer::CTimer(void)
{
	m_i64TicksPerSecond = sceRtcGetTickResolution();

	m_fFPS = 0;
	m_bIsRunning = false;
}


//--------------------------------------------------------------------
// Función:    CTimer::~CTimer
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 13:32:11
//--------------------------------------------------------------------
CTimer::~CTimer(void)
{
	Log("CTimer: Destructor");
}


//--------------------------------------------------------------------
// Función:    CTimer::Start
// Propósito:  Resetea el contador e inicia el cronómetro.
// Fecha:      29/07/2004 12:41
//--------------------------------------------------------------------
void CTimer::Start(void)
{
	// Tomamos el tiempo actual para saber en qué momento se
	// activó el cronómetro...probamos primero con el reloj
	// de alta resolución del PC. Si no existe, usamos timeGetTime

	sceRtcGetCurrentTick(&m_i64BaseTicks); 
    
	m_bIsRunning = true;

	m_fLastUpdate = 0;
	m_iFrameCount = 0;

	m_fFrameTime = 0;
	m_fDeltaTime = 0;
}


//--------------------------------------------------------------------
// Función:    CTimer::Stop
// Propósito:  Para el cronómetro.
// Fecha:      29/07/2004 12:41
//--------------------------------------------------------------------
void CTimer::Stop(void)
{
	if(m_bIsRunning)
	{
		// Recuerda cuando se ha parado para poder sacar el tiempo
		// transcurrido en detencion.
		// Probamos primero con el reloj de alta resolución del PC 
		// Si no existe usamos timeGetTime
	
		sceRtcGetCurrentTick(&m_i64StopedTicks); 
		
		m_bIsRunning = false;
	}
}


//--------------------------------------------------------------------
// Función:    CTimer::Continue
// Propósito:  Iniciamos el cronómetro pero sin resetear las variables.
// Fecha:      29/07/2004 12:41
//--------------------------------------------------------------------
void CTimer::Continue(void)
{
	if(!m_bIsRunning)
	{
		u64 Ticks;

		sceRtcGetCurrentTick(&Ticks); 

		// Incrementamos m_i64BaseTicks para saber 
		// cuánto tiempo hemos estado pausados.
		m_i64BaseTicks += Ticks - m_i64StopedTicks;

		m_bIsRunning = true;
	}
}


//--------------------------------------------------------------------
// Función:    CTimer::GetActualTime
// Propósito:  Devuelve el tiempo actual en segundos.
// Fecha:      29/07/2004 12:41
//--------------------------------------------------------------------
float CTimer::GetActualTime(void)
{
	u64 Ticks;

	if(m_bIsRunning)
	{
		sceRtcGetCurrentTick(&Ticks); 
	}
	else
		Ticks = m_i64StopedTicks;

	// Restamos el tiempo cuando comanzamos para
	// calcular el tiempo que nuestro cronómetro
	// ha estado en marcha
	Ticks -= m_i64BaseTicks;

	// Devolvemos el tiempo en segundos.
	return ((float) Ticks) / ((float)m_i64TicksPerSecond);
}


//--------------------------------------------------------------------
// Función:    CTimer::Update
// Propósito:  Calcula el frame-rate. Esta función debe llamarse una vez por frame.
// Fecha:      29/07/2004 12:42
//--------------------------------------------------------------------
void CTimer::Update(void)
{
	m_fDeltaTime = GetActualTime() - m_fFrameTime;
	m_fFrameTime += m_fDeltaTime;

	// Aumentamos el nº de frames
	m_iFrameCount++;
	// Calculamos el frame-rate
	if(m_fFrameTime - m_fLastUpdate > FPS_REFRESH_TIME)
	{
		m_fFPS = m_iFrameCount / (m_fFrameTime - m_fLastUpdate);
		m_iFrameCount = 0;
		m_fLastUpdate = m_fFrameTime;
	}
}


//--------------------------------------------------------------------
// Función:    CTimer::GetFPS
// Propósito:  Devuelve el frame-rate.
// Fecha:      29/07/2004 12:43
//--------------------------------------------------------------------
float CTimer::GetFPS(void)
{
	return m_fFPS;
}


//--------------------------------------------------------------------
// Función:    CTimer::GetFrameTime
// Propósito:  Devuelve el tiempo cuando Frame() fue llamado por última vez.
// Fecha:      29/07/2004 12:43
//--------------------------------------------------------------------
float CTimer::GetFrameTime(void)
{
	return m_fFrameTime;
}


//--------------------------------------------------------------------
// Función:    CTimer::GetDeltaTime
// Propósito:  Devuelve el tiempo que ha transcurrido entre dos llamadas a Frame().
// Fecha:      29/07/2004 12:43
//--------------------------------------------------------------------
float CTimer::GetDeltaTime(void)
{
	return m_fDeltaTime;
}


//--------------------------------------------------------------------
// Función:    CTimer::IsRunning
// Propósito:  Nos dice si el cronómetro está en marcha o no.
// Fecha:      29/07/2004 12:43
//--------------------------------------------------------------------
bool CTimer::IsRunning(void)
{
	return m_bIsRunning;
}

/********************************************************************/
/********************************************************************/
/*						End Timer.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


