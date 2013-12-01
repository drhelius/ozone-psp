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
/*								Sound.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"

class CSound
{
	public:
		CSound(void);
		~CSound(void);
		void Init(void);
		int Play(SOUNDS theSound, int pan=127, bool loop=false);
		void PlayBounce(int i);
		void PlayMusic(MUSICS theMusic, bool loop=false);
		void End(void);
		void Stop(int sound);
		void FinishLoading(void);
		void LoadMusic(MUSICS theMusic, char* path);
		void FreeMusic(MUSICS theMusic);

	private:
		
		SAMPLE* LoadSound(char* path);
		void FreeSound(SAMPLE* pSound);

	private:

		int m_iMikmodThreadID;

		SAMPLE* m_pSounds[20];
		SAMPLE* m_pBounceSounds[30];
		SAMPLE* m_pMusics[7];

		bool m_bOutputEnabled;
		bool m_bFinishedLoading;

};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Sound.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


