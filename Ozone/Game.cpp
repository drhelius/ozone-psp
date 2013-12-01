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
/*						    Game.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Game.h"

CTimer* CGame::ms_pMainTimer = NULL;
CSound* CGame::m_pSoundPlayer = NULL;
int CGame::m_iRecords[8];

//--------------------------------------------------------------------
// Función:    CGame::CGame
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 12:40:15
//--------------------------------------------------------------------
CGame::CGame(void)
{
	InitPointer(m_pInput);
	InitPointer(m_pAuxTimer);
	InitPointer(CurrentStateFunction);
	InitPointer(m_pCamera);
	InitPointer(m_pLogo);
	InitPointer(m_pLevel);
	InitPointer(m_pFont);

	InitPointer(m_pQuadSelectBack);
	InitPointer(m_pQuadSelectBackButton);
	InitPointer(m_pQuadSelectHighlightSmall);
	InitPointer(m_pQuadSelectHighlightBig);

	for (int i=0; i<8; i++)
	{
		InitPointer(m_pQuadSelectNumber[i]);
	}
	
	InitPointer(m_pQuadSplashControlsBack);
	InitPointer(m_pQuadSplashBack);		
	InitPointer(m_pQuadSplashControls);
	InitPointer(m_pQuadSplashCredits);
	InitPointer(m_pQuadSplashStart);
	InitPointer(m_pQuadSplashOzone);
	InitPointer(m_pQuadSplashHighlight);
	InitPointer(m_pQuadSplashCreditsLeft);
	InitPointer(m_pQuadSplashCreditsRight);
	InitPointer(m_pQuadSplashCreditsBack);

	InitPointer(m_pSoundPlayer);

	InitPointer(ms_pMainTimer);

	m_pVideoManager = CVideo::GetSingleton();
}


//--------------------------------------------------------------------
// Función:    CGame::~CGame
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 12:40:13
//--------------------------------------------------------------------
CGame::~CGame(void)
{
	Log("CGame: Destructor");

	End();
}


//--------------------------------------------------------------------
// Función:    CGame::DoStateLogo
// Propósito:  
// Fecha:      viernes, 10 de noviembre de 2006, 14:12:40
//--------------------------------------------------------------------
void CGame::DoStateLogo(void)
{
	ms_pMainTimer->Update();

	m_pInput->Update();

	m_pVideoManager->StartDrawing();
	m_pVideoManager->Clear(0xffffffff, GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

	m_pLogo->Update();	

	if (m_pLogo->IsFinished())
	{

		static float timecolor = 0.52359879f;

		int color = (int)(((sinf(timecolor * 3.0f) + 1.0f) / 2.0f) * 255.0f);

		m_pFont->Draw("PRESS ANY KEY TO CONTINUE", 125, 252, COLOR_ARGB(255, color, color, color));

		timecolor += ms_pMainTimer->GetDeltaTime();
	}

	m_Fade.Update(ms_pMainTimer->GetDeltaTime());

	m_pVideoManager->EndDrawing();	
#ifdef DEBUG_PSP_OZONE
	if (m_pInput->AnyButtonPressed() && !m_bFinishingState)
#else
	if (m_pLogo->IsFinished() && m_pInput->AnyButtonPressed() && !m_bFinishingState)	
#endif
	{
		m_pSoundPlayer->PlayMusic(MUSIC_MENU, true);

		m_bFinishingState = true;

		m_Fade.StartFade(0xffffffff, false, 0.5f);		
	}

	if (m_bFinishingState && m_Fade.IsFinished())
	{
		m_bFinishingState = false;

		SafeDelete(m_pLogo);		

		m_pVideoManager->ClearVRAM();
		InitSplash();

		m_Fade.StartFade(0xffffffff, true, 1.0f);

		SetState(STATE_SPLASH);
		
	}
}


//--------------------------------------------------------------------
// Función:    CGame::ReadRecords
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  20/06/2007  19:56:30
//--------------------------------------------------------------------
void CGame::ReadRecords(void)
{
	Log("CGame: Leyendo records...");

	int fd = sceIoOpen("data/records.dat", PSP_O_RDONLY, 0777); 

	if(fd <= 0)
	{ 
		Log("***** CGame: Error abriendo fichero de records\n");	

		return;
	} 	

	sceIoRead(fd, m_iRecords, sizeof(int) * 8); 

	sceIoClose(fd);

	for (int i=0; i<8; i++)
	{
		m_iRecords[i] ^= 0x71fa35b1;
	}

	Log("CGame: Records leidos.");
}


//--------------------------------------------------------------------
// Función:    CGame::SaveRecords
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  20/06/2007  20:42:59
//--------------------------------------------------------------------
void CGame::SaveRecords(void)
{
	Log("CGame: Salvando records...");

	FILE* fd = fopen("data/records.dat", "wb");

	//int fd = sceIoOpen("data/records.dat", PSP_O_WRONLY | PSP_O_CREAT, 0777); 

	//if(fd <= 0)
	if(!IsValidPointer(fd))
	{ 
		Log("***** CGame: Error abriendo fichero de records\n");	

		return;
	} 

	for (int i=0; i<8; i++)
	{
		m_iRecords[i] ^= 0x71fa35b1;
	}

	fwrite(m_iRecords, 8, sizeof(int), fd);

	fclose(fd);
	
	//sceIoWrite(fd, m_iRecords, sizeof(int) * 8); 

	//sceIoClose(fd);

	for (int i=0; i<8; i++)
	{
		m_iRecords[i] ^= 0x71fa35b1;
	}

	Log("CGame: Records salvados.");
}

//--------------------------------------------------------------------
// Función:    CGame::DoStateSplash
// Creador:    Nacho (AMD)
// Fecha:      Friday  16/03/2007  20:39:27
//--------------------------------------------------------------------
void CGame::DoStateSplash(void)
{	
	ms_pMainTimer->Update();

	m_pInput->Update();	

	///--- splash
	if (m_iCurrentState == 0)
	{	
		if (m_bFinishingState && m_Fade.IsFinished())
		{
			switch (m_iCurrentSelection)
			{
				case 0:
				{						
					m_bFinishingState = false;
					EndSplash();
					m_pVideoManager->ClearVRAM();
					InitSelect();
					SetState(STATE_MENU);
					m_Fade.StartFade(0xffffffff, true, 0.6f);
					return;					
				}			
				case 1:
				{						
					m_bFinishingState = false;
					m_iCurrentState = 1;
					m_Fade.StartFade(0xffffffff, true, 0.6f);
					return;						
				}				
				case 2:			
				{					
					m_bFinishingState = false;
					m_iCurrentState = 2;
					m_fCreditsOffset = 460.0f;
					m_Fade.StartFade(0xffffffff, true, 0.6f);
					return;					
				}			
			}	
		}

		if (!m_bFinishingState && m_Fade.IsFinished() && (m_pInput->ButtonPressed(PSP_CTRL_CIRCLE) ||
			m_pInput->ButtonPressed(PSP_CTRL_CROSS)))
		{
			m_Fade.StartFade(0xffffffff, false, 0.25f);
			m_bFinishingState = true;

			CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
		}

		if (!m_bFinishingState && (m_iCurrentSelection < 2) && (m_pInput->ButtonPressed(PSP_CTRL_DOWN) || m_pInput->ButtonPressed(PSP_CTRL_RIGHT)))
		{
			m_iCurrentSelection++;

			m_pAuxTimer->Start();

			m_pSoundPlayer->Play(SOUND_TICK);
		} 
		else if (!m_bFinishingState && (m_iCurrentSelection > 0) && (m_pInput->ButtonPressed(PSP_CTRL_UP) || m_pInput->ButtonPressed(PSP_CTRL_LEFT)))
		{
			m_iCurrentSelection--;

			m_pAuxTimer->Start();

			m_pSoundPlayer->Play(SOUND_TICK);
		}

		m_pVideoManager->StartDrawing();

		m_pVideoManager->Clear(0x0, GU_DEPTH_BUFFER_BIT);	

		m_pCamera->SetMode(false);

		//////////////////////////////////////////////////////////////////////////

		m_pVideoManager->EnableTexture(m_pQuadSplashBack->GetTexture(), true);

		VECTOR3 scale = { 512.0f, 272.0f, 1.0f };

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{		
			sceGumScale(&scale);				
		}	

		m_pQuadSplashBack->Render();

		//////////////////////////////////////////////////////////////////////////

		sceGuEnable(GU_BLEND);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

		m_pVideoManager->EnableTexture(m_pQuadSplashHighlight->GetTexture());

		scale.x = 100.0f;
		scale.y = 100.0f;

		VECTOR3 pos = { 0.0f, 0.0f, 0.0f };

		switch (m_iCurrentSelection)
		{
			case 0:
				pos.x = 1.0f;
				pos.y = 13.0f;
				break;
			case 1:
				pos.x = 39.0f;
				pos.y = 104.0f;
				break;
			case 2:
				pos.x = 133.0f;
				pos.y = 159.0f;
				break;
		}

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		sceGuAmbientColor(COLOR_ARGB((int)((sinf(m_pAuxTimer->GetActualTime() * 3.0f) + 1.0f) * 255.0f / 2.0f), 255, 255, 255));

		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

		m_pQuadSplashHighlight->Render();

		sceGuAmbientColor(0xffffffff);

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

		m_pVideoManager->EnableTexture(m_pQuadSplashStart->GetTexture());

		scale.x = 64.0f;
		scale.y = 32.0f;

		//(m_iCurrentSelection == 0) ? (sinf(ms_pMainTimer->GetActualTime) * 100.0f) : 0;

		pos.x = 42.0f + ((m_iCurrentSelection == 0) ? (sinf(m_pAuxTimer->GetActualTime() * 10.0f) * 5.0f) : 0);
		pos.y = 48.0f;
		
		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	
		
		m_pQuadSplashStart->Render();
	
		m_pVideoManager->EnableTexture(m_pQuadSplashControls->GetTexture());

		scale.x = 128.0f;
		scale.y = 32.0f;

		pos.x = 70.0f + ((m_iCurrentSelection == 1) ? (sinf(m_pAuxTimer->GetActualTime() * 10.0f) * 5.0f) : 0);
		pos.y = 140.0f;

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		m_pQuadSplashControls->Render();

		
		m_pVideoManager->EnableTexture(m_pQuadSplashCredits->GetTexture());

		pos.x = 147.0f + ((m_iCurrentSelection == 2) ? (sinf(m_pAuxTimer->GetActualTime() * 10.0f) * 5.0f) : 0);
		pos.y = 195.0f;

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		m_pQuadSplashCredits->Render();


		m_pVideoManager->EnableTexture(m_pQuadSplashOzone->GetTexture());

		scale.x = 256.0f;
		scale.y = 64.0f;

		pos.x = 213.0f;
		pos.y = 30.0f;

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		float opacity = MAT_Clampf(sinf(ms_pMainTimer->GetActualTime()) + 1.3f, 0.0f, 1.0f);
		sceGuAmbientColor(COLOR_ARGB((int)(opacity * 255.0f), 255, 255, 255));

		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

		m_pQuadSplashOzone->Render();

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

		sceGuAmbientColor(0xffffffff);

		sceGuDisable(GU_BLEND);

		//////////////////////////////////////////////////////////////////////////
	}
	///--- controls
	else if (m_iCurrentState == 1)
	{
		if (m_bFinishingState && m_Fade.IsFinished())
		{			
			m_bFinishingState = false;
			m_iCurrentState = 0;
			m_iCurrentSelection = 1;
			m_pAuxTimer->Start();
			m_Fade.StartFade(0xffffffff, true, 0.6f);
			return;
		}

		if (!m_bFinishingState && m_pInput->AnyButtonPressed() && m_Fade.IsFinished())
		{			
			m_Fade.StartFade(0xffffffff, false, 0.25f);
			m_bFinishingState = true;
			CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
		}

		m_pVideoManager->StartDrawing();

		m_pVideoManager->Clear(0x0, GU_DEPTH_BUFFER_BIT);	

		m_pCamera->SetMode(false);

		//////////////////////////////////////////////////////////////////////////

		m_pVideoManager->EnableTexture(m_pQuadSplashControlsBack->GetTexture(), true);

		VECTOR3 scale = { 512.0f, 272.0f, 1.0f };

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{		
			sceGumScale(&scale);				
		}	

		m_pQuadSplashControlsBack->Render();
	
	}
	///--- credits
	else
	{
		if (m_bFinishingState && m_Fade.IsFinished())
		{			
			m_bFinishingState = false;
			m_iCurrentState = 0;
			m_iCurrentSelection = 2;
			m_pAuxTimer->Start();
			m_Fade.StartFade(0xffffffff, true, 0.6f);
			return;
		}

		if (!m_bFinishingState && m_pInput->AnyButtonPressed() && m_Fade.IsFinished())
		{			
			m_Fade.StartFade(0xffffffff, false, 0.25f);
			m_bFinishingState = true;
			CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
		}

		m_pVideoManager->StartDrawing();

		m_pVideoManager->Clear(0x0, GU_DEPTH_BUFFER_BIT);	

		m_pCamera->SetMode(false);

		//////////////////////////////////////////////////////////////////////////

		m_pVideoManager->EnableTexture(m_pQuadSplashCreditsBack->GetTexture(), true);

		VECTOR3 scale = { 512.0f, 272.0f, 1.0f };

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{		
			sceGumScale(&scale);				
		}	

		m_pQuadSplashCreditsBack->Render();		

		/////////////

		m_fCreditsOffset -= 50.0f * ms_pMainTimer->GetDeltaTime();

		if (m_fCreditsOffset < -1600.0f)
			m_fCreditsOffset = 460.0f;

		m_pFont->Draw("GEARDOME 2007              PROGRAMMING: NACHO SANCHEZ              ART: CESAR SAMPEDRO              AUDIO: DAVID GARCIA              SPECIAL THANKS TO: GORKA SUAREZ, DAVID SANCHEZ", (int)m_fCreditsOffset, 210, 0xff000000);

		////

		sceGuEnable(GU_BLEND);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

		m_pVideoManager->EnableTexture(m_pQuadSplashCreditsLeft->GetTexture(), true);

		scale.x = 256.0f;
		scale.y = 128.0f;

		VECTOR3 pos = { 0.0f, 144.0f, 1.0f };

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		m_pQuadSplashCreditsLeft->Render();

		m_pVideoManager->EnableTexture(m_pQuadSplashCreditsRight->GetTexture(), true);
		
		pos.x = 224.0f;

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		m_pQuadSplashCreditsRight->Render();

		////
		
		m_pVideoManager->EnableTexture(m_pQuadSplashOzone->GetTexture());

		scale.x = 256.0f;
		scale.y = 64.0f;

		pos.x = 112.0f;
		pos.y = 65.0f;

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		float opacity = MAT_Clampf(sinf(ms_pMainTimer->GetActualTime()) + 1.3f, 0.0f, 1.0f);
		sceGuAmbientColor(COLOR_ARGB((int)(opacity * 255.0f), 255, 255, 255));

		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

		m_pQuadSplashOzone->Render();

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

		sceGuAmbientColor(0xffffffff);

		sceGuDisable(GU_BLEND);

		
	}

	m_Fade.Update(ms_pMainTimer->GetDeltaTime());

	m_pVideoManager->EndDrawing();	
}


//--------------------------------------------------------------------
// Función:    CGame::DoStateMenu
// Creador:    Nacho (AMD)
// Fecha:      Friday  16/03/2007  20:29:37
//--------------------------------------------------------------------
void CGame::DoStateMenu(void)
{
	ms_pMainTimer->Update();

	m_pInput->Update();

	if (!m_bFinishingState && m_pInput->ButtonPressed(PSP_CTRL_DOWN))
	{
		if (m_iCurrentSelection < 4)
		{
			m_iCurrentSelection++;			

			m_pAuxTimer->Start();

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}
		else if (m_iCurrentSelection > 4)
		{
			m_iCurrentSelection--;
			
			m_pAuxTimer->Start();

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}
	} 
	else if (!m_bFinishingState && m_pInput->ButtonPressed(PSP_CTRL_UP))
	{
		if ((m_iCurrentSelection < 4) && (m_iCurrentSelection > 0))
		{
			m_iCurrentSelection--;
			if (m_iCurrentSelection < 0)
				m_iCurrentSelection = 0;

			m_pAuxTimer->Start();	

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}
		else if ((m_iCurrentSelection > 4) && (m_iCurrentSelection < 8)) 
		{
			m_iCurrentSelection++;
			
			m_pAuxTimer->Start();

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}		
	}
	else if (!m_bFinishingState && m_pInput->ButtonPressed(PSP_CTRL_RIGHT))
	{
		if (m_iCurrentSelection < 8)
		{
			m_iCurrentSelection++;
			m_pAuxTimer->Start();

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}	
	}
	else if (!m_bFinishingState && m_pInput->ButtonPressed(PSP_CTRL_LEFT))
	{
		if (m_iCurrentSelection > 0)
		{
			m_iCurrentSelection--;
			m_pAuxTimer->Start();

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}	
	}
	
	if (m_bFinishingState && m_Fade.IsFinished())
	{	
		m_bFinishingState = false;

		if (m_iCurrentSelection == 4)
		{
			EndSelect();
			m_pVideoManager->ClearVRAM();
			InitSplash();
			SetState(STATE_SPLASH);
			m_Fade.StartFade(0xffffffff, true, 0.6f);
		}
		else
		{
			ms_pMainTimer->Stop();

			m_pVideoManager->StartDrawing();
			m_pVideoManager->Clear(0x0, GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

			m_pFont->Draw("LOADING...", 395, 250, COLOR_ARGB(255, 255, 255, 255));

			m_pVideoManager->EndDrawing();

			m_pSoundPlayer->FreeMusic(MUSIC_LOGO);
			m_pSoundPlayer->FreeMusic(MUSIC_MENU);

			int level = m_iCurrentSelection;

			if (m_iCurrentSelection > 4)
			{
				level--;
			}		

			EndSelect();

			m_pVideoManager->ClearVRAM();
			m_pSoundPlayer->FinishLoading();

			char path[PSP_MAX_PATH];
			sprintf(path, "sounds/h_ambient_%d.wav", m_iCurrentMusic+1);
			m_pSoundPlayer->LoadMusic((MUSICS)m_iCurrentMusic, path);

			int temp = m_iCurrentMusic;

			m_iCurrentMusic++;
			m_iCurrentMusic %= 5;

			m_pLevel->InitVariables();
			m_pLevel->Init(level, m_iRecords[level]);
			SetState(STATE_GAME);	
			m_Fade.StartFade(0xff000000, true, 1.8f);			
			m_pSoundPlayer->PlayMusic((MUSICS)temp, true);
		}

		return;		
	}

	if (!m_bFinishingState && m_Fade.IsFinished() && (m_pInput->ButtonPressed(PSP_CTRL_CIRCLE) ||
		m_pInput->ButtonPressed(PSP_CTRL_CROSS)))
	{	
		if (m_iCurrentSelection == 4)
		{
			m_Fade.StartFade(0xffffffff, false, 0.25f);
			m_bFinishingState = true;			
		}
		else
		{
			m_Fade.StartFade(0xff000000, false, 0.3f);
			m_bFinishingState = true;			
		}

		CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
	}


	m_pVideoManager->StartDrawing();

	m_pVideoManager->Clear(0x0, GU_DEPTH_BUFFER_BIT);	

	m_pCamera->SetMode(false);

	m_pVideoManager->EnableTexture(m_pQuadSelectBack->GetTexture(), true);

	VECTOR3 scale = { 512.0f, 272.0f, 1.0f };

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{		
		sceGumScale(&scale);				
	}	

	m_pQuadSelectBack->Render();

	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	//////////////////////////////////////////////////////////////////////////

	
	m_pVideoManager->EnableTexture(m_pQuadSelectHighlightSmall->GetTexture());

	scale.x = 78.0f;
	scale.y = 78.0f;

	for (int i=0; i<9; i++)
	{
		if (i==4)
			continue;

		int j = i;

		if (i>4)
		{
			j = i-1;
		}

		if (m_iRecords[j] == 1245986)
			continue;
	

		VECTOR3 pos = { 0.0f, 0.0f, 0.0f };

		switch (i)
		{
		case 0:
			pos.x = -3.0f;
			pos.y = -3.0f;
			break;
		case 1:
			pos.x = -1.0f;
			pos.y = 70.0f;
			break;
		case 2:
			pos.x = 51.0f;
			pos.y = 135.0f;
			break;
		case 3:
			pos.x = 110.0f;
			pos.y = 184.0f;
			break;	
		case 5:
			pos.x = 289.0f;
			pos.y = 185.0f;
			break;
		case 6:
			pos.x = 349.0f;
			pos.y = 135.0f;
			break;
		case 7:
			pos.x = 399.0f;
			pos.y = 70.0f;
			break;
		case 8:
			pos.x = 400.0f;
			pos.y = -3.0f;
			break;
		}

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}	

		sceGuAmbientColor(COLOR_ARGB(255, 100, 160, 255));

		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

		m_pQuadSelectHighlightSmall->Render();
		
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

		sceGuAmbientColor(0xffffffff);

	}

	//////////////////////////////////////////////////////////////////////////

	if (m_iCurrentSelection == 4)
	{
		m_pVideoManager->EnableTexture(m_pQuadSelectHighlightBig->GetTexture());

		scale.x = 140.0f;
		scale.y = 70.0f;
	}
	else
	{
		m_pVideoManager->EnableTexture(m_pQuadSelectHighlightSmall->GetTexture());

		scale.x = 78.0f;
		scale.y = 78.0f;
	}	

	VECTOR3 pos = { 0.0f, 0.0f, 0.0f };

	switch (m_iCurrentSelection)
	{
	case 0:
		pos.x = -3.0f;
		pos.y = -3.0f;
		break;
	case 1:
		pos.x = -1.0f;
		pos.y = 70.0f;
		break;
	case 2:
		pos.x = 51.0f;
		pos.y = 135.0f;
		break;
	case 3:
		pos.x = 110.0f;
		pos.y = 184.0f;
		break;
	case 4:
		pos.x = 170.0f;
		pos.y = 209.0f;
		break;
	case 5:
		pos.x = 289.0f;
		pos.y = 185.0f;
		break;
	case 6:
		pos.x = 349.0f;
		pos.y = 135.0f;
		break;
	case 7:
		pos.x = 399.0f;
		pos.y = 70.0f;
		break;
	case 8:
		pos.x = 400.0f;
		pos.y = -3.0f;
		break;
	}

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		sceGumTranslate(&pos);
		sceGumScale(&scale);				
	}	

	sceGuAmbientColor(COLOR_ARGB((int)((sinf(m_pAuxTimer->GetActualTime() * 3.0f) + 1.0f) * 255.0f / 2.0f), 255, 255, 255));

	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

	if (m_iCurrentSelection == 4)
	{
		m_pQuadSelectHighlightBig->Render();
	}
	else
	{
		m_pQuadSelectHighlightSmall->Render();
	}

	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

	sceGuAmbientColor(0xffffffff);

	//////////////////////////////////////////////////////////////////////////

	for (int i=0; i<8; i++)
	{

		m_pVideoManager->EnableTexture(m_pQuadSelectNumber[i]->GetTexture());

		scale.x = 32.0f;
		scale.y = 32.0f;
	

		VECTOR3 pos = { 0.0f, 0.0f, 0.0f };

		switch (i)
		{
		case 0:
			pos.x = 20.0f;
			pos.y = 20.0f;
			break;
		case 1:
			pos.x = 22.0f;
			pos.y = 93.0f;
			break;
		case 2:
			pos.x = 74.0f;
			pos.y = 159.0f;
			break;
		case 3:
			pos.x = 133.0f;
			pos.y = 208.0f;
			break;		
		case 4:
			pos.x = 312.0f;
			pos.y = 208.0f;
			break;
		case 5:
			pos.x = 372.0f;
			pos.y = 159.0f;
			break;
		case 6:
			pos.x = 423.0f;
			pos.y = 94.0f;
			break;
		case 7:
			pos.x = 423.0f;
			pos.y = 20.0f;
			break;
		}

		if (m_iCurrentSelection > 4)
		{
			pos.x += ((m_iCurrentSelection == (i+1)) ? (sinf(m_pAuxTimer->GetActualTime() * 6.0f) * 4.0f) : 0);
		}
		else if (m_iCurrentSelection < 4)
		{
			pos.x += ((m_iCurrentSelection == i) ? (sinf(m_pAuxTimer->GetActualTime() * 6.0f) * 4.0f) : 0);
		}

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}

		m_pQuadSelectNumber[i]->Render();
	}

	m_pVideoManager->EnableTexture(m_pQuadSelectBackButton->GetTexture());

	pos.x = 208.0f + ((m_iCurrentSelection == 4) ? (sinf(m_pAuxTimer->GetActualTime() * 6.0f) * 4.0f) : 0);
	pos.y = 229.0f;

	scale.x = 64.0f;

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		sceGumTranslate(&pos);
		sceGumScale(&scale);				
	}	

	m_pQuadSelectBackButton->Render();

	//////////////////////////////////////////////////////////////////////////

	sceGuDisable(GU_BLEND);

	m_Fade.Update(ms_pMainTimer->GetDeltaTime());

	m_pVideoManager->EndDrawing();	
}


//--------------------------------------------------------------------
// Función:    CGame::DoStateGame
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 12:40:09
//--------------------------------------------------------------------
void CGame::DoStateGame(void)
{
	m_pLevel->Update(&m_Fade);

	if (m_pLevel->FinishRequested())
	{
		m_pVideoManager->StartDrawing();
		m_pVideoManager->Clear(0x0, GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

		m_pFont->Draw("LOADING...", 395, 250, COLOR_ARGB(255, 255, 255, 255));

		m_pVideoManager->EndDrawing();

		m_pLevel->End();	

		int temp = m_iCurrentMusic-1;
		if (temp < 0)
			temp = 4;
		m_pSoundPlayer->FreeMusic((MUSICS)(temp));
		m_bFinishingState = false;		
		m_pVideoManager->ClearVRAM();
		InitSelect();
		m_pSoundPlayer->LoadMusic(MUSIC_MENU, "sounds/h_menu_music.wav");
		m_pSoundPlayer->PlayMusic(MUSIC_MENU, true);
		SetState(STATE_MENU);
		m_Fade.StartFade(0xffffffff, true, 0.6f);
		ms_pMainTimer->Continue();		
	}
}


//--------------------------------------------------------------------
// Función:    CGame::SetState
// Propósito:  
// Fecha:      martes, 31 de octubre de 2006, 12:01:45
//--------------------------------------------------------------------
void CGame::SetState(eSTATES theNewState)
{
	switch(theNewState)
	{
		case STATE_LOGO:
		{
			Log("CGame: Seleccionado estado STATE_LOGO\n");
			CurrentStateFunction=&CGame::DoStateLogo;
			break;
		}
		case STATE_SPLASH:
		{
			Log("CGame: Seleccionado estado STATE_SPLASH\n");
			CurrentStateFunction=&CGame::DoStateSplash;
			break;
		}
		case STATE_MENU:
		{
			Log("CGame: Seleccionado estado STATE_MENU\n");
			CurrentStateFunction=&CGame::DoStateMenu;
			break;
		}
		case STATE_GAME:
		{
			Log("CGame: Seleccionado estado STATE_GAME\n");
			CurrentStateFunction=&CGame::DoStateGame;
			break;
		}
	}
}


//--------------------------------------------------------------------
// Función:    CGame::Init
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 12:39:27
//--------------------------------------------------------------------
void CGame::Init(void)
{	
	Log("CGame: Iniciando...\n");

	m_pVideoManager->Init();

	m_pFont = new CTextFont();

	m_pFont->Init("gfx/fonts/font.psptex", "gfx/fonts/font.dat");

	m_pCamera = new CCamera();

	m_pVideoManager->StartDrawing();
	m_pVideoManager->Clear(0x0, GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

#ifdef SPLASH_GBAX

	CQuad* pQuadSplash = new CQuad(true);
	pQuadSplash->Init("gfx/gbax.psptex", 512, 256);	

	m_pCamera->SetMode(false);

	//////////////////////////////////////////////////////////////////////////

	m_pVideoManager->EnableTexture(pQuadSplash->GetTexture(), true);

	VECTOR3 scale = { 512.0f, 272.0f, 1.0f };

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{		
		sceGumScale(&scale);				
	}	

	pQuadSplash->Render();
	
	SafeDelete(pQuadSplash);

	m_pFont->Draw("LOADING...", 396, 251, COLOR_ARGB(255, 0, 0, 0));
	m_pFont->Draw("LOADING...", 395, 250, COLOR_ARGB(255, 255, 255, 255));

#else
	#ifdef SPLASH_NEOFLASH

		CQuad* pQuadSplash = new CQuad(true);
		pQuadSplash->Init("gfx/neoflash.psptex", 512, 256);	

		m_pCamera->SetMode(false);

		//////////////////////////////////////////////////////////////////////////

		m_pVideoManager->EnableTexture(pQuadSplash->GetTexture(), true);

		VECTOR3 scale = { 512.0f, 272.0f, 1.0f };

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{		
			sceGumScale(&scale);				
		}	

		pQuadSplash->Render();

		SafeDelete(pQuadSplash);

		m_pFont->Draw("LOADING...", 4, 4, COLOR_ARGB(255, 0, 0, 0));
		m_pFont->Draw("LOADING...", 3, 3, COLOR_ARGB(255, 255, 255, 255));

	#else

		m_pFont->Draw("LOADING...", 396, 251, COLOR_ARGB(255, 0, 0, 0));
		m_pFont->Draw("LOADING...", 395, 250, COLOR_ARGB(255, 255, 255, 255));

	#endif
#endif

	m_pVideoManager->EndDrawing();

	m_bFinishingState = false;

	m_iCurrentSelection = 0;	

	srand((unsigned)time(NULL));

	m_iCurrentMusic = MAT_RandomInt(0, 5);

	ReadRecords();	

	SetState(STATE_LOGO);

	ms_pMainTimer = new CTimer();	

	m_pAuxTimer = new CTimer();	

	m_pLogo = new CLogo();
	m_pLogo->Init();

	m_pInput = CInput::GetSingleton();
	m_pInput->Init();

	m_pLevel = new CLevel();

	//m_pMusicPlayer = new CMusic();
	//m_pMusicPlayer->Init();
	//m_pMusicPlayer->LoadSong("sounds/h_ambient_1.mp3");

	m_pSoundPlayer = new CSound();
	m_pSoundPlayer->Init();

	m_pSoundPlayer->LoadMusic(MUSIC_LOGO, "sounds/h_logo.wav");
	m_pSoundPlayer->LoadMusic(MUSIC_MENU, "sounds/h_menu_music.wav");
	m_pSoundPlayer->PlayMusic(MUSIC_LOGO);

	//m_pMusicPlayer->Play();

	m_Fade.StartFade(0xffffffff, true, 2.8f);	

	m_pAuxTimer->Start();
	ms_pMainTimer->Start();

	Log("CGame: Iniciado\n");
}


//--------------------------------------------------------------------
// Función:    CGame::InitSelect
// Creador:    Nacho (AMD)
// Fecha:      Friday  16/03/2007  20:28:48
//--------------------------------------------------------------------
void CGame::InitSelect(void)
{
	Log("CGame: InitSelect");
	m_pQuadSelectBack = new CQuad(true);
	m_pQuadSelectBack->Init("gfx/menu/select/Psp_Final.psptex", 512, 256);	

	m_pQuadSelectBackButton = new CQuad(true);
	m_pQuadSelectBackButton->Init("gfx/menu/select/Texto_back.psptex", 64, 32);	

	m_pQuadSelectHighlightSmall = new CQuad(true);
	m_pQuadSelectHighlightSmall->Init("gfx/menu/select/Boton_level.psptex", 64, 64);	

	m_pQuadSelectHighlightBig = new CQuad(true);
	m_pQuadSelectHighlightBig->Init("gfx/menu/select/Boton_back.psptex", 128, 64);	

	for (int i=0; i<8; i++)
	{
		m_pQuadSelectNumber[i] = new CQuad(true);

		char temp[PSP_MAX_PATH];

		sprintf(temp, "gfx/menu/select/%d.psptex", i+1);

		m_pQuadSelectNumber[i]->Init(temp, 32, 32);
	}

	//m_iCurrentSelection = 0;
}


//--------------------------------------------------------------------
// Función:    CGame::EndSelect
// Creador:    Nacho (AMD)
// Fecha:      Friday  16/03/2007  20:28:51
//--------------------------------------------------------------------
void CGame::EndSelect(void)
{
	Log("CGame: EndSelect");
	SafeDelete(m_pQuadSelectBack);
	SafeDelete(m_pQuadSelectBackButton);
	SafeDelete(m_pQuadSelectHighlightSmall);
	SafeDelete(m_pQuadSelectHighlightBig);

	for (int i=0; i<8; i++)
	{
		SafeDelete(m_pQuadSelectNumber[i]);
	}

	//m_iCurrentSelection = 0;
}


//--------------------------------------------------------------------
// Función:    CGame::InitSplash
// Creador:    Nacho (AMD)
// Fecha:      Friday  16/03/2007  20:38:26
//--------------------------------------------------------------------
void CGame::InitSplash(void)
{
	Log("CGame: InitSplash");
	m_pQuadSplashBack = new CQuad(true);
	m_pQuadSplashBack->Init("gfx/menu/main/PSP_Menu.psptex", 512, 256);		

	m_pQuadSplashControlsBack = new CQuad(true);
	m_pQuadSplashControlsBack->Init("gfx/menu/controls.psptex", 512, 256);	

	m_pQuadSplashCreditsBack = new CQuad(true);
	m_pQuadSplashCreditsBack->Init("gfx/menu/credits_back.psptex", 512, 256);

	m_pQuadSplashCreditsLeft = new CQuad(true);
	m_pQuadSplashCreditsLeft->Init("gfx/menu/credits_front2.psptex", 256, 128);

	m_pQuadSplashCreditsRight = new CQuad(true);
	m_pQuadSplashCreditsRight->Init("gfx/menu/credits_front.psptex", 256, 128);

	m_pQuadSplashControls = new CQuad(true);
	m_pQuadSplashControls->Init("gfx/menu/main/Boton_controls.psptex", 128, 32);	

	m_pQuadSplashCredits = new CQuad(true);
	m_pQuadSplashCredits->Init("gfx/menu/main/Boton_credits.psptex", 128, 32);	

	m_pQuadSplashStart = new CQuad(true);
	m_pQuadSplashStart->Init("gfx/menu/main/Boton_start.psptex", 64, 32);	

	m_pQuadSplashOzone = new CQuad(true);
	m_pQuadSplashOzone->Init("gfx/menu/main/Logo_ozone.psptex", 256, 64);	

	m_pQuadSplashHighlight = new CQuad(true);
	m_pQuadSplashHighlight->Init("gfx/menu/select/Boton_level.psptex", 64, 64);	

	m_iCurrentSelection = 0;
	m_iCurrentState = 0;
}


//--------------------------------------------------------------------
// Función:    CGame::EndSplash
// Creador:    Nacho (AMD)
// Fecha:      Friday  16/03/2007  20:38:29
//--------------------------------------------------------------------
void CGame::EndSplash(void)
{
	Log("CGame: EndSplash");
	SafeDelete(m_pQuadSplashBack);
	SafeDelete(m_pQuadSplashControlsBack);
	SafeDelete(m_pQuadSplashControls);
	SafeDelete(m_pQuadSplashCredits);
	SafeDelete(m_pQuadSplashStart);
	SafeDelete(m_pQuadSplashOzone);
	SafeDelete(m_pQuadSplashHighlight);
	SafeDelete(m_pQuadSplashCreditsLeft);
	SafeDelete(m_pQuadSplashCreditsRight);
	SafeDelete(m_pQuadSplashCreditsBack);

	m_iCurrentSelection = 0;
}

//--------------------------------------------------------------------
// Función:    CGame::End
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 13:08:16
//--------------------------------------------------------------------
void CGame::End(void)
{
	Log("Game: Finalizando...\n");

	SafeDelete(m_pAuxTimer);

	InitPointer(CurrentStateFunction);
	SafeDelete(m_pCamera);
	SafeDelete(ms_pMainTimer);
	SafeDelete(m_pLogo);
	SafeDelete(m_pLevel);
	SafeDelete(m_pFont);

	SafeDelete(m_pQuadSelectBack);
	SafeDelete(m_pQuadSelectBackButton);
	SafeDelete(m_pQuadSelectHighlightSmall);
	SafeDelete(m_pQuadSelectHighlightBig);

	for (int i=0; i<8; i++)
	{
		SafeDelete(m_pQuadSelectNumber[i]);
	}

	SafeDelete(m_pQuadSplashBack);
	SafeDelete(m_pQuadSplashControls);
	SafeDelete(m_pQuadSplashControlsBack);
	SafeDelete(m_pQuadSplashCredits);
	SafeDelete(m_pQuadSplashStart);
	SafeDelete(m_pQuadSplashOzone);
	SafeDelete(m_pQuadSplashHighlight);
	SafeDelete(m_pQuadSplashCreditsLeft);
	SafeDelete(m_pQuadSplashCreditsRight);
	SafeDelete(m_pQuadSplashCreditsBack);

	SafeDelete(m_pSoundPlayer);

	m_iCurrentSelection = 0;
	m_iCurrentState = 0;

	Log("Game: Finalizado\n");
}


/********************************************************************/
/********************************************************************/
/*							End Game.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
