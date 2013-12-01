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
/*						    Sound.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Sound.h"

static bool sound_done = false;
//extern int _mm_errno;
//extern BOOL _mm_critical;
//extern char *_mm_errmsg[];
//int mikModThreadID = -1;


//--------------------------------------------------------------------
// Función:    my_error_handler
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  16:43:21
//--------------------------------------------------------------------
static void my_error_handler(void)
{
	//printf("_mm_critical %d\n", MikMod_critical);
	//printf("_mm_errno %d\n", MikMod_errno);
	//printf("%s\n", MikMod_strerror(MikMod_errno));
	return;
}


//--------------------------------------------------------------------
// Función:    AudioChannelThread
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  16:43:26
//--------------------------------------------------------------------
static int AudioChannelThread(SceSize args, void *argp)
{
	while (!sound_done)
	{
		MikMod_Update();
		// We have to sleep here to allow other threads a chance to process.
		// with no sleep this thread will take over when the output is disabled via MikMod_DisableOutput()
		// co-operative threading sucks bigtime...
		sceKernelDelayThread(1);
	}
	return (0);
}


//--------------------------------------------------------------------
// Función:    CSound::CSound
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  16:44:14
//--------------------------------------------------------------------
CSound::CSound(void)
{
	for (int i=0; i<20; i++)
		InitPointer(m_pSounds[i]);

	for (int i=0; i<30; i++)
		InitPointer(m_pBounceSounds[i]);

	for (int i=0; i<7; i++)
		InitPointer(m_pMusics[i]);

	m_bOutputEnabled = false;
	m_bFinishedLoading = false;
}


//--------------------------------------------------------------------
// Función:    CSound::~CSound
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  16:44:13
//--------------------------------------------------------------------
CSound::~CSound(void)
{
	Log("CSound: Destructor");

	End();
}


//--------------------------------------------------------------------
// Función:    CSound::End
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  16:49:45
//--------------------------------------------------------------------
void CSound::End(void)
{
	sound_done = true;

	if (m_iMikmodThreadID > 0)
	{
		SceUInt timeout = 100000;
		sceKernelWaitThreadEnd(m_iMikmodThreadID, &timeout);
		// not 100% sure if this is necessary after a clean exit, but just to make sure any resources are freed:
		sceKernelDeleteThread(m_iMikmodThreadID);
	}

	Player_Stop();

	for (int i=0; i<20; i++)
	{
		FreeSound(m_pSounds[i]);
		InitPointer(m_pSounds[i]);
	}

	for (int i=0; i<30; i++)
	{
		FreeSound(m_pBounceSounds[i]);
		InitPointer(m_pBounceSounds[i]);
	}

	for (int i=0; i<7; i++)
	{
		if (IsValidPointer(m_pMusics[i]))
		{
			FreeSound(m_pMusics[i]);
			InitPointer(m_pMusics[i]);
		}		
	}

	m_bFinishedLoading = false;

	MikMod_Exit();
}


//--------------------------------------------------------------------
// Función:    CSound::Init
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  16:44:11
//--------------------------------------------------------------------
void CSound::Init(void)
{
	MikMod_RegisterErrorHandler(my_error_handler);
	MikMod_RegisterAllDrivers();	
	MikMod_RegisterAllLoaders();

	md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_SNDFX|DMODE_SOFT_MUSIC; 
	md_reverb = 0;
	md_pansep = 128;
	md_volume = 255;
	

	if (MikMod_Init(""))
	{
		Log("***** Could not initialize sound, reason: %s\n", MikMod_strerror(MikMod_errno));
		sceKernelExitGame();
		return;
	}

	MikMod_SetNumVoices(-1, 24);

	m_pSounds[SOUND_LEVEL_END] = LoadSound("sounds/nivel_fin.wav");
	m_pSounds[SOUND_GEM_GREEN] = LoadSound("sounds/nivel_gema_verde.wav");
	m_pSounds[SOUND_GEM_YELLOW] = LoadSound("sounds/nivel_gema_amarilla.wav");
	m_pSounds[SOUND_GEM_RED] = LoadSound("sounds/nivel_gema_roja.wav");
	m_pSounds[SOUND_GEM_BLUE] = LoadSound("sounds/nivel_gema_azul.wav");
	m_pSounds[SOUND_GEM_BLUE2] = LoadSound("sounds/nivel_gema_azul2.wav");
	m_pSounds[SOUND_LEVEL_AIRPUMP] = LoadSound("sounds/nivel_inflador.wav");
	m_pSounds[SOUND_LEVEL_IMP] = LoadSound("sounds/nivel_impulsor.wav");
	m_pSounds[SOUND_HURT] = LoadSound("sounds/nivel_golpe.wav");
	m_pSounds[SOUND_MOVE] = LoadSound("sounds/nivel_mover.wav");
	m_pSounds[SOUND_DOORS] = LoadSound("sounds/nivel_puertas.wav");
	m_pSounds[SOUND_TICK] = LoadSound("sounds/disp_lanzador.wav");
	m_pSounds[SOUND_TRANS] = LoadSound("sounds/nivel_trans.wav");
	m_pSounds[SOUND_EXPLO] = LoadSound("sounds/nivel_explosion.wav");
	//m_pSounds[SOUND_ENE2_FIRE] = LoadSound("sounds/ene2_disp.wav");
	m_pSounds[SOUND_ENE3_FIRE] = LoadSound("sounds/ene3_disp.wav");
	m_pSounds[SOUND_ENE_EXPLO] = LoadSound("sounds/ene_explosion.wav");
	m_pSounds[SOUND_FIRE_1] = LoadSound("sounds/disp_1.wav");
	m_pSounds[SOUND_FIRE_2] = LoadSound("sounds/disp_2.wav");
	m_pSounds[SOUND_ENE4_BOUNCE] = LoadSound("sounds/ene4_rebota.wav");

	MikMod_EnableOutput();

	m_bOutputEnabled = true;

	if ((m_iMikmodThreadID = sceKernelCreateThread("MikMod" ,AudioChannelThread,0x12,0x10000,0,NULL)) > 0)
	{
		sceKernelStartThread(m_iMikmodThreadID, 0 , NULL);
	}
	else
	{
		Log("***** Play thread create failed!\n");
	}
}

//--------------------------------------------------------------------
// Función:    CSound::FinishLoading
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  20:58:02
//--------------------------------------------------------------------
void CSound::FinishLoading(void)
{
	if (!m_bFinishedLoading)
	{
		m_bFinishedLoading = true;

		for (int i=0; i<30; i++)
		{
			char temp[PSP_MAX_PATH];
			sprintf (temp, "sounds/nivel_toque%d.wav", i+1);
			m_pBounceSounds[i] = LoadSound(temp);		
		}
	}
}

//--------------------------------------------------------------------
// Función:    LoadSound
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  17:01:06
//--------------------------------------------------------------------
SAMPLE* CSound::LoadSound(char* path)
{
	Log("SOUND: Loading... %s", path);

	SAMPLE* temp = Sample_Load(path);

	if (temp == NULL)
		Log("***** SOUND: failed loading: %s\n", path);
		
	return temp;
}


//--------------------------------------------------------------------
// Función:    FreeSound
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  17:01:09
//--------------------------------------------------------------------
void CSound::FreeSound(SAMPLE* pSound)
{
	Sample_Free(pSound);
}


//--------------------------------------------------------------------
// Función:    CSound::Stop
// Creador:    Nacho (AMD)
// Fecha:      Friday  22/06/2007  16:05:47
//--------------------------------------------------------------------
void CSound::Stop(int sound)
{
	Voice_Stop(sound);
}


//--------------------------------------------------------------------
// Función:    CSound::Play
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  16:43:54
//--------------------------------------------------------------------
int CSound::Play(SOUNDS theSound, int pan, bool loop) 
{	
	if (!IsValidPointer(m_pSounds[theSound]))
		return 0;

	if (loop)
	{
		m_pSounds[theSound]->loopstart = 0;
		m_pSounds[theSound]->loopend = m_pSounds[theSound]->length;
		m_pSounds[theSound]->flags = SF_LOOP;
	}
	else
	{
		m_pSounds[theSound]->flags = 0;
	}

	int voice = Sample_Play(m_pSounds[theSound], 0, loop ? SFX_CRITICAL : 0);
	Voice_SetPanning(voice, pan);
	Voice_SetVolume(voice, 256);

	return voice;
}


//--------------------------------------------------------------------
// Función:    CSound::PlayBounce
// Creador:    Nacho (AMD)
// Fecha:      Monday  18/06/2007  17:37:53
//--------------------------------------------------------------------
void CSound::PlayBounce(int i) 
{	
	if (!IsValidPointer(m_pBounceSounds[i]))
		return;

	if ((i<0) || (i> 29))
		return;

	int voice = Sample_Play(m_pBounceSounds[i], 0, 0);
	Voice_SetPanning(voice, 127);
	Voice_SetVolume(voice, 256);
}


//--------------------------------------------------------------------
// Función:    CSound::PlayMusic
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  20/06/2007  12:48:18
//--------------------------------------------------------------------
void CSound::PlayMusic(MUSICS theMusic, bool loop) 
{	
	if (!IsValidPointer(m_pMusics[theMusic]))
		return;

	if (loop)
	{
		m_pMusics[theMusic]->loopstart = 0;
		m_pMusics[theMusic]->loopend = m_pMusics[theMusic]->length;
		m_pMusics[theMusic]->flags = SF_LOOP;
	}
	else
	{
		m_pMusics[theMusic]->flags = 0;
	}

	int voice = Sample_Play(m_pMusics[theMusic], 0, SFX_CRITICAL);
	Voice_SetPanning(voice, 127);
	Voice_SetVolume(voice, 256);
}


//--------------------------------------------------------------------
// Función:    CSound::LoadMusic
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  20/06/2007  13:20:02
//--------------------------------------------------------------------
void CSound::LoadMusic(MUSICS theMusic, char* path)
{
	Log("MUSIC: Loading... %s", path);
#ifdef DEBUG_PSP_OZONE
	m_pMusics[theMusic] = NULL;
#else
	m_pMusics[theMusic] = Sample_Load(path);
#endif

	if (m_pMusics[theMusic] == NULL)
		Log("***** MUSIC: failed loading: %s\n", path);
}


//--------------------------------------------------------------------
// Función:    CSound::FreeMusic
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  20/06/2007  13:20:05
//--------------------------------------------------------------------
void CSound::FreeMusic(MUSICS theMusic)
{	
	if (IsValidPointer(m_pMusics[theMusic]))
	{
		Sample_Free(m_pMusics[theMusic]);
		InitPointer(m_pMusics[theMusic]);
	}	
}


/********************************************************************/
/********************************************************************/
/*							End Sound.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
