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
/*						    Level.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Level.h"
#include "Game.h"


//--------------------------------------------------------------------
// Función:    CLevel::CLevel
// Propósito:  
// Fecha:      viernes, 27 de octubre de 2006, 17:04:27
//--------------------------------------------------------------------
CLevel::CLevel(void)
{
	InitVariables();		
}

//--------------------------------------------------------------------
// Función:    CLevel::InitVariables
// Creador:    Nacho (AMD)
// Fecha:      Saturday  16/06/2007  19:23:58
//--------------------------------------------------------------------
void CLevel::InitVariables(void)
{
	InitPointer(m_pMainTimer);
	InitPointer(m_pVideoManager);
	InitPointer(m_pInput);
	InitPointer(m_pCamera);	
	InitPointer(m_pCubes);
	InitPointer(m_pEnemies);
	InitPointer(m_pSectors);
	InitPointer(m_pFont);
	InitPointer(m_pTextureManager);
	InitPointer(m_pLayerManager);
	InitPointer(m_pNormalCube);
	InitPointer(m_pWireCube);
	InitPointer(m_pDoorCube);
	InitPointer(m_pInfoCubes);
	InitPointer(m_pParticles);
	InitPointer(m_pBall);
	InitPointer(m_pQuadBackground);
	InitPointer(m_pSmoke);
	InitPointer(m_pObjects);
	InitPointer(m_pQuadObjs);
	InitPointer(m_pTrans);
	InitPointer(m_pTransTexture);
	InitPointer(m_pEndTexture);
	InitPointer(m_pQuadGemItem);
	InitPointer(m_pArrow);

	for (int i=0; i<10; i++)
	{
		InitPointer(m_pObjTextures[i]);
		InitPointer(m_pObjMeshes[i]);
	}

	for (int i=0; i<4; i++)
	{
		InitPointer(m_pEnemiesTexture[i]);
		InitPointer(m_pEnemiesMeshes[i]);
	}

	for (int i=0; i<13; i++)
	{
		InitPointer(m_pMeshCube[i].texture);
		InitPointer(m_pMeshCube[i].textureAdd);
		InitPointer(m_pMeshCube[i].theMesh);
	}

	m_bGoingToDie = false;
	m_bEndLevel = false;
	m_bFinished = true;
	m_bFinishRequest = false;
	m_bDead = false;
	m_bPaused = false;
	m_bHasRedKey = false;
	m_bHasBlueKey = false;
	m_bTiltGems = false;
	m_bSoundAir = false;
	m_bSoundPump = false;
	m_bSoundImp = false;
	m_iVoiceSoundAir = -1;
	m_iVoiceSoundPump = -1;
	m_iVoiceSoundImp = -1;
	m_iNumCubes = 0;
	m_iNumEnemies = 0;
	m_iNumSectors = 0;
	m_iNumObjs = 0;	
	m_iRemainingGems = 0;
	m_iRedAmmo = 0;
	m_iYellowAmmo = 0;
	m_iLevel = 0;
	m_iPausedMenu = 0;
	m_iBounceSound = 0;
	m_fLastDeflateSmoke = 0.0f;
	m_fLastBounceSound = 0.0f;
	m_fPausedMenuAlpha = 0.0f;
	m_fLastYellowFire = 0.0f;
	m_fAngGems = 0.0f;
	m_fAngTrans = 0.0f;
	m_fAngTrans2 = 0.0f;
	m_fAngBlow = 0.0f;
	m_fOffsetArrow = 0.0f;
	m_fTimerTrans = 0.0f;
	m_fAngU = 0.0f;
	m_fDeltaTime = 0.0f;
	m_fActualTime = 0.0f;
	m_fLastSmokeUpdate = 0.0f;	
	m_fFanSpeed = 250.0f;
}

//--------------------------------------------------------------------
// Función:    CLevel::End
// Creador:    Nacho (AMD)
// Fecha:      Saturday  16/06/2007  18:31:44
//--------------------------------------------------------------------
void CLevel::End(void)
{
	if (m_bSoundAir)
	{
		m_bSoundAir = false;
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundAir);
	}

	if (m_bSoundPump)
	{
		m_bSoundPump = false;
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundPump);
	}

	if (m_bSoundImp)
	{
		m_bSoundImp = false;
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundImp);
	}

	for (int i=0; i<13; i++)
	{
		m_pTextureManager->ReleaseTexture(m_pMeshCube[i].texture);
		m_pTextureManager->ReleaseTexture(m_pMeshCube[i].textureAdd);
	}


	for (int i=0; i<10; i++)
	{
		m_pTextureManager->ReleaseTexture(m_pObjTextures[i]);
	}

	for (int i=0; i<4; i++)
	{
		m_pTextureManager->ReleaseTexture(m_pEnemiesTexture[i]);		
		SafeDelete(m_pEnemiesMeshes[i]);
	}

	m_pTextureManager->ReleaseTexture(m_pTransTexture);
	m_pTextureManager->ReleaseTexture(m_pEndTexture);

	SafeDelete(m_pQuadGemItem);	

	m_pVideoManager->ClearVRAM();

	SafeDelete(m_pCamera);	
	SafeDeleteArray(m_pCubes);	
	SafeDeleteArray(m_pEnemies);
	SafeDeleteArray(m_pSectors);
	SafeDelete(m_pFont);
	SafeDelete(m_pLayerManager);

	SafeDelete(m_pParticles);

	SafeDelete(m_pNormalCube);
	SafeDelete(m_pWireCube);
	SafeDelete(m_pDoorCube);

	SafeDeleteArray(m_pInfoCubes);

	SafeDelete(m_pBall);
	SafeDelete(m_pQuadBackground);

	SafeDelete(m_pSmoke);
	SafeDelete(m_pArrow);

	SafeDeleteArray(m_pObjects);		

	SafeDelete(m_pObjMeshes[0]);
	SafeDelete(m_pObjMeshes[4]);
	SafeDelete(m_pObjMeshes[5]);
	SafeDelete(m_pObjMeshes[7]);
	
	SafeDelete(m_pQuadObjs);

	SafeDelete(m_pTrans);

	m_bFinished = true;
}


//--------------------------------------------------------------------
// Función:    CLevel::~CLevel
// Propósito:  
// Fecha:      viernes, 27 de octubre de 2006, 17:04:32
//--------------------------------------------------------------------
CLevel::~CLevel(void)
{
	Log("CLevel: Destructor");

	if (!m_bFinished)
		End();
}


//--------------------------------------------------------------------
// Función:    CLevel::Init
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 20:38:11
//--------------------------------------------------------------------
void CLevel::Init(int iLevel, int record)
{
	Log("CLevel: Iniciando nivel %d\n", iLevel);

	m_iActualRecord = record;

	m_pMainTimer = CGame::GetMainTimer();
	m_pVideoManager = CVideo::GetSingleton();
	m_pInput = CInput::GetSingleton();
	m_pTextureManager = CTextureManager::GetSingleton();

	m_pMainTimer->Stop();

	m_iLevel = iLevel;

	m_pCamera = new CCamera();
	m_pFont = new CTextFont();

	m_pFont->Init("gfx/fonts/font.psptex", "gfx/fonts/font.dat");

	m_pNormalCube = new CMesh();
	m_pNormalCube->LoadMesh("gfx/cubes/normal.o3d", NULL, 0, 0);

	m_pWireCube = new CMesh();
	m_pWireCube->LoadMesh("gfx/cubes/wire.o3d", NULL, 0, 0);

	m_pDoorCube = new CMesh();
	m_pDoorCube->LoadMesh("gfx/cubes/gate.o3d", NULL, 0, 0);

	m_pBall = new CBall();
	m_pBall->Init();

	m_pQuadObjs = new CQuad(false);
	m_pQuadObjs->Init(NULL, 0, 0);

	m_pQuadBackground = new CQuad(false);
	m_pQuadBackground->Init("gfx/levels/backs/h_back_1.psptex", 128, 128, true);	

	m_pQuadGemItem = new CQuad(true);
	m_pQuadGemItem->Init("gfx/models/h_gem_item.psptex", 32, 32);

	m_pSmoke = new CSmoke();
	m_pSmoke->Init();

	m_pArrow = new CMesh();
	m_pArrow->LoadMesh("gfx/models/flecha.o3d", "gfx/models/flecha.psptex", 16, 16);

	m_pParticles = new CParticles();
	m_pParticles->Init();

	m_pObjMeshes[0] = new CMesh();
	m_pObjMeshes[0]->LoadMesh("gfx/models/gem.o3d", NULL, 0, 0);
	m_pObjMeshes[1] = m_pObjMeshes[0];
	m_pObjMeshes[2] = m_pObjMeshes[0];
	m_pObjMeshes[3] = m_pObjMeshes[0];
	m_pObjMeshes[4] = new CMesh();
	m_pObjMeshes[4]->LoadMesh("gfx/models/spike.o3d", NULL, 0, 0);
	m_pObjMeshes[5] = new CMesh();
	m_pObjMeshes[5]->LoadMesh("gfx/models/key.o3d", NULL, 0, 0);
	m_pObjMeshes[6] = m_pObjMeshes[5];
	m_pObjMeshes[7] = new CMesh();
	m_pObjMeshes[7]->LoadMesh("gfx/models/spitter.o3d", NULL, 0, 0);
	m_pObjMeshes[8] = NULL;
	m_pObjMeshes[9] = NULL;

	m_pObjTextures[0] = m_pTextureManager->GetTexture("gfx/models/h_blue_gem.psptex", 64, 64);
	m_pObjTextures[1] = m_pTextureManager->GetTexture("gfx/models/h_red_gem.psptex", 64, 64);
	m_pObjTextures[2] = m_pTextureManager->GetTexture("gfx/models/h_yellow_gem.psptex", 64, 64);
	m_pObjTextures[3] = m_pTextureManager->GetTexture("gfx/models/h_green_gem.psptex", 64, 64);
	m_pObjTextures[4] = m_pTextureManager->GetTexture("gfx/models/spike.psptex", 64, 64);
	m_pObjTextures[5] = m_pTextureManager->GetTexture("gfx/models/key1.psptex", 64, 64);
	m_pObjTextures[6] = m_pTextureManager->GetTexture("gfx/models/key2.psptex", 64, 64);
	m_pObjTextures[7] = m_pTextureManager->GetTexture("gfx/models/spitter.psptex", 64, 64);
	m_pObjTextures[8] = m_pTextureManager->GetTexture("gfx/models/impulsor.psptex", 64, 64);
	m_pObjTextures[9] = m_pTextureManager->GetTexture("gfx/models/inflador.psptex", 64, 64);

	m_pTransTexture = m_pTextureManager->GetTexture("gfx/models/espiral.psptex", 64, 64);
	m_pEndTexture = m_pTextureManager->GetTexture("gfx/models/fin.psptex", 64, 64);
	
	m_pEnemiesTexture[0] = m_pTextureManager->GetTexture("gfx/models/ene1.psptex", 64, 64);
	m_pEnemiesTexture[1] = m_pTextureManager->GetTexture("gfx/models/ene2.psptex", 64, 64);
	m_pEnemiesTexture[2] = m_pTextureManager->GetTexture("gfx/models/ene3.psptex", 64, 64);
	m_pEnemiesTexture[3] = m_pTextureManager->GetTexture("gfx/models/ene4.psptex", 64, 64);
	
	m_pEnemiesMeshes[0] = new CMesh();
	m_pEnemiesMeshes[0]->LoadMesh("gfx/models/ene1.o3d", NULL, 0, 0);
	m_pEnemiesMeshes[1] = new CMesh();
	m_pEnemiesMeshes[1]->LoadMesh("gfx/models/ene2.o3d", NULL, 0, 0);
	m_pEnemiesMeshes[2] = new CMesh();
	m_pEnemiesMeshes[2]->LoadMesh("gfx/models/ene3.o3d", NULL, 0, 0);
	m_pEnemiesMeshes[3] = new CMesh();
	m_pEnemiesMeshes[3]->LoadMesh("gfx/models/ene4.o3d", NULL, 0, 0);

	LoadLevel(iLevel);

	m_pLayerManager = new CLayerManager();
	m_pLayerManager->Init(35.0f, 20.0f, 19.0f, 9.0f, -1.0f, -35.0f);

	for(int fg=0; fg<3; fg++)
	{
		for (int kkk=0; kkk<100; kkk++)
		{
			float posx = (float)MAT_RandomInt(0, m_theLevel.NumSecX*32);
			float posy = (float)MAT_RandomInt(0, m_theLevel.NumSecY*32);

			float velx = MAT_RandomInt(-100, 100)/100.0f;
			float vely = MAT_RandomInt(-100, 100)/100.0f;

			m_pLayerManager->AddParticle(posx, posy, velx, vely, fg);
		}
	}

	VECTOR3 pos;
	pos.x = m_theLevel.IniX + 4.0f;
	pos.y = m_theLevel.IniY + 4.0f;
	pos.z = 4.0f;	

	m_pBall->SetPosition(pos);

	pos.x *= -1.0f;
	pos.z = -50.0f;

	m_pCamera->SetPosition(pos);	

	m_RenderedCubes.ResetAll();
	m_RenderedObjs.ResetAll();

	m_PausedTimer.Start();
	m_SpikeTimer.Start();

	m_bFinished = false;

	m_RecordTimer.Start();
	m_pMainTimer->Continue();		
}

static int theLevel;
static int total_seconds;

//--------------------------------------------------------------------
// Función:    SavingThread
// Creador:    Nacho (AMD)
// Fecha:      Saturday  23/06/2007  13:04:57
//--------------------------------------------------------------------
static int SavingThread(SceSize args, void *argp)
{
	CGame::UpdateRecords(theLevel, total_seconds);

	return (0);
}


//--------------------------------------------------------------------
// Función:    CLevel::StartSavingRecords
// Creador:    Nacho (AMD)
// Fecha:      Saturday  23/06/2007  13:04:55
//--------------------------------------------------------------------
void CLevel::StartSavingRecords(int seconds)
{
	theLevel = m_iLevel;
	total_seconds = seconds;	

	int id=0;

	if ((id = sceKernelCreateThread("saving_thread", SavingThread, 0x11, 0xFA0, 0, 0)) > 0)
	{
		sceKernelStartThread(id, 0 , NULL);
	}
	else
	{
		Log("******* CLevel: Error en StartSavingRecords\n");
	}	
}


//--------------------------------------------------------------------
// Función:    CLevel::Update
// Propósito:  
// Fecha:      lunes, 27 de noviembre de 2006, 20:41:07
//--------------------------------------------------------------------
void CLevel::Update(CFade* pFade)
{
	m_pMainTimer->Update();
	m_PausedTimer.Update();

	m_fDeltaTime = m_pMainTimer->GetDeltaTime();
	m_fActualTime = m_pMainTimer->GetActualTime();

	UpdateInput(pFade);

	/////////////////////
	/// SONIDO LOOP

	if (m_bSoundAir && (m_bGoingToDie || m_bPaused))
	{
		m_bSoundAir = false;
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundAir);
	}

	if (m_bSoundPump && (m_bGoingToDie || m_bPaused))
	{
		m_bSoundPump = false;
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundPump);
	}

	if (m_bSoundImp && (m_bGoingToDie || m_bPaused))
	{
		m_bSoundImp = false;
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundImp);
	}

	if (m_bPaused && (!m_bGoingToDie))
	{
	}
	else
	{
		UpdateLogic(pFade);			

		UpdateCamera();
	}
	
	RenderLevel(pFade);
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderArrow
// Creador:    Nacho (AMD)
// Fecha:      Thursday  21/06/2007  14:02:32
//--------------------------------------------------------------------
void CLevel::RenderArrow(void)
{
	int best_i = -1;
	float best_length = 0.0;;

	for (int i=0; i<m_iNumObjs; i++)
	{
		if ((m_pObjects[i].type == 0) && m_pObjects[i].active)
		{
			VECTOR3 vDis;
			VECTOR3 vecBallPos = m_pBall->GetPosition();
			vecBallPos.z = 4.0f;
			VECTOR3 vTemp = {m_pObjects[i].posX + 4.0f, m_pObjects[i].posY + 4.0f, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			float length = MAT_VectorQuadraticLength(&vDis);

			if (best_i < 0)
			{
				best_length = length;
				best_i = i;
			}
			else
			{
				if (length < best_length)
				{
					best_length = length;
					best_i = i;
				}
			}		
		}
	}

	if (best_i >= 0)
	{
	

		

		VECTOR2 posArrow = { -m_pCamera->GetPosition().x, m_pCamera->GetPosition().y - 10.0f };
		VECTOR2 posGem = { m_pObjects[best_i].posX + 4.0f, m_pObjects[best_i].posY + 4.0f};
		VECTOR2 vDir;
		VECTOR2 vLeft = { -1.0f, 0.0f };

		MAT_Vector2Subtract(&vDir, &posGem, &posArrow);

		float dot = (vDir.x * vLeft.x) + (vDir.y * vLeft.y);

		float magnitud = MAT_Vector2Length(&vDir) * MAT_Vector2Length(&vLeft);

		float angulo = acosf(dot/magnitud);

		if (isnan(angulo))
			angulo = 0.0f;

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{			
			
			VECTOR3 scale = { 0.08f, 0.08f, 0.08f };
			VECTOR3 pos =  { posArrow.x, -posArrow.y, 10.0f };

			sceGumTranslate(&pos);				
			sceGumRotateZ((posArrow.y <= posGem.y) ? angulo : -angulo);
			sceGumRotateY(MAT_ToRadians(-20.0f));
			sceGumScale(&scale);	
		}	

		m_pVideoManager->EnableTexture(m_pArrow->GetTexture());

		sceGuDisable(GU_DEPTH_TEST);
	

		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);


		VECTOR3 posLight;
		posLight.z = -0.7f;
		posLight.y = 0.0f;
		posLight.x = 0.0f;

		sceGuEnable(GU_LIGHTING);
		sceGuEnable(GU_LIGHT0);
		sceGuLight(0, GU_DIRECTIONAL, GU_DIFFUSE, &posLight);
		sceGuLightColor(0,GU_DIFFUSE, 0xffffffff);			
		sceGuLightAtt(0, 1.0f, 0.0f, 0.0f);	

		sceGuSpecular(10.0f);
		sceGuAmbient(0x00101010);

		sceGuColor(0xffffffff);		

		m_pArrow->Render();

		sceGuLight(0, GU_DIRECTIONAL, GU_DIFFUSE_AND_SPECULAR, &posLight);
		sceGuLightColor(0,GU_DIFFUSE, 0x00000000);
		sceGuLightColor(0,GU_SPECULAR, 0xffffffff);

		sceGuEnable(GU_BLEND);
		sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, COLOR_ARGB(255, 255, 255, 255), COLOR_ARGB(255, 255, 255, 255));

		m_pArrow->Render();

		sceGuDisable(GU_BLEND);

		sceGuAmbientColor(0xffffffff);
		sceGuAmbient(0xffffffff);
		sceGuDisable(GU_LIGHT0);
		sceGuDisable(GU_LIGHTING);		

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

		sceGuEnable(GU_DEPTH_TEST);

		
	}	
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderBackgroundQuad
// Creador:    Nacho (AMD)
// Fecha:      martes, 30 de enero de 2007, 19:22:31
//--------------------------------------------------------------------
void CLevel::RenderBackgroundQuad(int cx, int cy, float fZ)
{
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		
		ScePspFVector3 pos =  {-(cx * 64.0f), -(cy * 64.0f) - 64.0f, fZ};
		ScePspFVector3 scale = { 64.1f, 64.1f, 0.0f };
		
		sceGumTranslate(&pos);
		sceGumScale(&scale);
		
	}

	m_pQuadBackground->Render();	
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderBackground
// Creador:    Nacho (AMD)
// Fecha:      martes, 30 de enero de 2007, 19:37:38
//--------------------------------------------------------------------
void CLevel::RenderBackground(float fZ)
{	
	int centerSectorX = ((int)m_pCamera->GetPosition().x) /64;
	int centerSectorY = ((int)m_pCamera->GetPosition().y) /64;	

	m_pVideoManager->EnableTexture(m_pQuadBackground->GetTexture());
		
	for (int cx=(centerSectorX-1); cx<=(centerSectorX+1); cx++)
	{
		for (int cy=(centerSectorY-1); cy<=(centerSectorY+1); cy++)
		{
			RenderBackgroundQuad(cx, cy, fZ);
		}
	}
}


//--------------------------------------------------------------------
// Función:    CLevel::UpdateInput
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 21:21:35
//--------------------------------------------------------------------
void CLevel::UpdateInput(CFade* pFade)
{
	if (m_bFinishRequest)
		return;

	m_pInput->Update();

	VECTOR2 vecBallVel = m_pBall->GetVelocity();	
	VECTOR3 vecBallPos = m_pBall->GetPosition();

	if (!m_bGoingToDie && m_pInput->ButtonPressed(PSP_CTRL_START))
	{
		m_bPaused = !m_bPaused;

		if (m_bPaused)
		{
			m_iPausedMenu = (m_bGoingToDie ? 1 : 0);
			m_fPausedMenuAlpha = 0.0f;
			m_pMainTimer->Stop();
			m_RecordTimer.Stop();
			pFade->StartFade(0xff000000, false, 0.3f, 150.0f);			
		}
		else
		{
			m_pMainTimer->Continue();
			m_RecordTimer.Continue();
			pFade->StartFade(0xff000000, true, 0.1f, 150.0f);
		}		
	}

	if (m_bPaused || m_bDead)
	{
		if (m_pInput->ButtonPressed(PSP_CTRL_UP))
		{
			m_iPausedMenu--;

			if (m_bDead)
			{
				if (m_iPausedMenu == 0)
				{
					m_iPausedMenu = 2;
				}
			}
			else
			{
				if (m_iPausedMenu < 0)
					m_iPausedMenu = 2;
			}

			m_fPausedMenuAlpha = 0.0f;

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}
		else if (m_pInput->ButtonPressed(PSP_CTRL_DOWN))
		{			
			m_iPausedMenu++;
			m_iPausedMenu %= 3;
			m_fPausedMenuAlpha = 0.0f;

			if (m_bDead && (m_iPausedMenu == 0))
			{
				m_iPausedMenu = 1;
			}

			CGame::GetSoundPlayer()->Play(SOUND_TICK);
		}

		if (m_pInput->ButtonPressed(PSP_CTRL_CROSS) || m_pInput->ButtonPressed(PSP_CTRL_CIRCLE))
		{
			if (m_iPausedMenu == 0)
			{
				m_bPaused = false;
				m_RecordTimer.Continue();
				m_pMainTimer->Continue();
				pFade->StartFade(0xff000000, true, 0.1f, 150.0f);
			}
			else if (m_iPausedMenu == 1)
			{
				m_pVideoManager->StartDrawing();
				m_pVideoManager->Clear(0x0, GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

				m_pFont->Draw("LOADING...", 395, 250, COLOR_ARGB(255, 255, 255, 255));

				m_pVideoManager->EndDrawing();

				bool isend = m_bEndLevel;

				int level = m_iLevel;				
				End();	
				InitVariables();

				Init(level, isend ? m_iFinishedRecord : m_iActualRecord);
												
				pFade->StartFade(0xff000000, true, 1.8f);				
			}
			else if (m_iPausedMenu == 2)
			{
				m_bFinishRequest = true;
			}

			//CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
		}

		return;
	}

	if (m_bGoingToDie)
		return;

	if ((m_iYellowAmmo > 0) && (m_pInput->ButtonPressed(PSP_CTRL_RIGHT) ||
		m_pInput->ButtonPressed(PSP_CTRL_LEFT) || m_pInput->ButtonPressed(PSP_CTRL_UP) ||
		m_pInput->ButtonPressed(PSP_CTRL_DOWN)))
	{
		VECTOR2 vel = {vecBallVel.x * 1.5f, vecBallVel.y * 1.5f};
		
		m_pParticles->AddParticle(vecBallPos, vel, 1.0f, 0);

		m_iYellowAmmo--;

		CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
	}

	if ((m_iRedAmmo > 0) && (m_pInput->ButtonPressed(PSP_CTRL_CROSS) ||
		m_pInput->ButtonPressed(PSP_CTRL_CIRCLE)))
	{
		VECTOR2 vel = {vecBallVel.x * 1.5f, vecBallVel.y * 1.5f};

		m_pParticles->AddParticle(vecBallPos, vel, 1.0f, 1);

		m_iRedAmmo--;

		CGame::GetSoundPlayer()->Play(SOUND_FIRE_2);
	}
	
	if (m_pInput->ButtonPress(PSP_CTRL_TRIANGLE))
	{
		if (!m_bSoundAir)
		{
			m_bSoundAir = true;
			m_iVoiceSoundAir = CGame::GetSoundPlayer()->Play(SOUND_MOVE, 127, true);
		}

		m_pBall->Deflate(m_fDeltaTime * 7.0f);

		if ((m_fActualTime - m_fLastDeflateSmoke) > 0.2f)
		{
			m_fLastDeflateSmoke = m_fActualTime;

			m_pSmoke->AddExplosion(vecBallPos, 7.0f, true);
		}
	}

	if (m_pInput->ButtonPress(PSP_CTRL_SQUARE))
	{
		float len = MAT_Vector2Length(&vecBallVel);

		if (len < 0.1f)
		{
			len = 0.0f;
		}
		else
		{
			vecBallVel.x /= len;
			vecBallVel.y /= len;

			len -= 15.0f * m_fDeltaTime;
		}

		vecBallVel.x *= len;
		vecBallVel.y *= len;

		m_pBall->SetVelocity(vecBallVel);
	}

	bool startPlayingWind = false;
	
	if (m_pInput->ButtonPress(PSP_CTRL_RIGHT))
	{
		startPlayingWind = true;
		
		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x += (vecBallVel.x - 8.0f);
			vel.y = vecBallVel.y;
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);
		}

		vecBallVel.x += (40.0f * m_fDeltaTime);	

		if (vecBallVel.x > 100.0f)
			vecBallVel.x = 100.0f;

		m_pBall->Deflate(m_fDeltaTime);
	} 
	else if (m_pInput->ButtonPress(PSP_CTRL_LEFT))
	{
		startPlayingWind = true;

		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x += (vecBallVel.x + 8.0f);
			vel.y = vecBallVel.y;
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);
		}

		vecBallVel.x -= (40.0f * m_fDeltaTime);

		if (vecBallVel.x < -100.0f)
			vecBallVel.x = -100.0f;

		m_pBall->Deflate(m_fDeltaTime);
	}
	
	if (m_pInput->ButtonPress(PSP_CTRL_UP))
	{
		startPlayingWind = true;

		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x = vecBallVel.x;
			vel.y += (vecBallVel.y + 8.0f);
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);
		}

		vecBallVel.y -= (40.0f * m_fDeltaTime);

		if (vecBallVel.y < -100.0f)
			vecBallVel.y = -100.0f;

		m_pBall->Deflate(m_fDeltaTime);
	}		
	else if (m_pInput->ButtonPress(PSP_CTRL_DOWN))
	{
		startPlayingWind = true;

		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x = vecBallVel.x;
			vel.y += (vecBallVel.y - 8.0f);
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);
		}

		vecBallVel.y += (40.0f * m_fDeltaTime);

		if (vecBallVel.y > 100.0f)
			vecBallVel.y = 100.0f;

		m_pBall->Deflate(m_fDeltaTime);
	}
	
	//////////////////////////////////////////////////////////////////////////

	if (m_pInput->GetAnalogX() > 178)
	{
		startPlayingWind = true;

		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x += (vecBallVel.x - 8.0f);
			vel.y = vecBallVel.y;
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);

			if ((m_iYellowAmmo > 0) && ((m_fActualTime - m_fLastYellowFire) > 0.5f))
			{
				m_fLastYellowFire = m_fActualTime;

				vel.x = vecBallVel.x * 1.5f;
				vel.y = vecBallVel.y * 1.5f;

				m_pParticles->AddParticle(vecBallPos, vel, 1.0f, 0);

				m_iYellowAmmo--;

				CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
			}
		}

		vecBallVel.x += (25.0f * m_fDeltaTime);	

		if (vecBallVel.x > 100.0f)
			vecBallVel.x = 100.0f;

		m_pBall->Deflate(m_fDeltaTime);
		
	}
	else if (m_pInput->GetAnalogX() < 78)
	{
		startPlayingWind = true;

		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x += (vecBallVel.x + 8.0f);
			vel.y = vecBallVel.y;
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);

			if ((m_iYellowAmmo > 0) && ((m_fActualTime - m_fLastYellowFire) > 0.5f))
			{
				m_fLastYellowFire = m_fActualTime;

				vel.x = vecBallVel.x * 1.5f;
				vel.y = vecBallVel.y * 1.5f;

				m_pParticles->AddParticle(vecBallPos, vel, 1.0f, 0);

				m_iYellowAmmo--;

				CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
			}
		}

		vecBallVel.x -= (25.0f * m_fDeltaTime);

		if (vecBallVel.x < -100.0f)
			vecBallVel.x = -100.0f;

		m_pBall->Deflate(m_fDeltaTime);
	}

	if (m_pInput->GetAnalogY() < 78)
	{
		startPlayingWind = true;

		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x = vecBallVel.x;
			vel.y += (vecBallVel.y + 8.0f);
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);

			if ((m_iYellowAmmo > 0) && ((m_fActualTime - m_fLastYellowFire) > 0.5f))
			{
				m_fLastYellowFire = m_fActualTime;

				vel.x = vecBallVel.x * 1.5f;
				vel.y = vecBallVel.y * 1.5f;

				m_pParticles->AddParticle(vecBallPos, vel, 1.0f, 0);

				m_iYellowAmmo--;

				CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
			}
		}

		vecBallVel.y -= (25.0f * m_fDeltaTime);

		if (vecBallVel.y < -100.0f)
			vecBallVel.y = -100.0f;

		m_pBall->Deflate(m_fDeltaTime);
	}
	else if (m_pInput->GetAnalogY() > 178)
	{		
		startPlayingWind = true;		

		if ((m_fActualTime - m_fLastSmokeUpdate) > 0.1f)
		{
			m_fLastSmokeUpdate = m_fActualTime;

			VECTOR2 vel = {0};
			vel.x = vecBallVel.x;
			vel.y += (vecBallVel.y - 8.0f);
			m_pSmoke->AddSmoke(vecBallPos, vel, 4.0f);

			if ((m_iYellowAmmo > 0) && ((m_fActualTime - m_fLastYellowFire) > 0.5f))
			{
				m_fLastYellowFire = m_fActualTime;

				vel.x = vecBallVel.x * 1.5f;
				vel.y = vecBallVel.y * 1.5f;

				m_pParticles->AddParticle(vecBallPos, vel, 1.0f, 0);

				m_iYellowAmmo--;

				CGame::GetSoundPlayer()->Play(SOUND_FIRE_1);
			}
		}

		vecBallVel.y += (25.0f * m_fDeltaTime);

		if (vecBallVel.y > 100.0f)
			vecBallVel.y = 100.0f;

		m_pBall->Deflate(m_fDeltaTime);
	}

	//////////////////////////////////////////////////////////////////////////	

	if (startPlayingWind && !m_bSoundAir)
	{
		m_bSoundAir = true;
		m_iVoiceSoundAir = CGame::GetSoundPlayer()->Play(SOUND_MOVE, 127, true);
	}

	if (!m_pInput->ButtonPress(PSP_CTRL_RIGHT) && !m_pInput->ButtonPress(PSP_CTRL_LEFT) && 
		!m_pInput->ButtonPress(PSP_CTRL_UP) && !m_pInput->ButtonPress(PSP_CTRL_DOWN) &&
		(m_pInput->GetAnalogX() <= 178) && (m_pInput->GetAnalogX() >= 78) &&
		(m_pInput->GetAnalogY() <= 178) && (m_pInput->GetAnalogY() >= 78) &&
		!m_pInput->ButtonPress(PSP_CTRL_TRIANGLE))
	{
		if (m_bSoundAir)
		{
			m_bSoundAir = false;
			CGame::GetSoundPlayer()->Stop(m_iVoiceSoundAir);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	
	m_pBall->SetVelocity(vecBallVel);

	m_pBall->Update(m_fDeltaTime, m_iRedAmmo > 0);

	vecBallPos = m_pBall->GetPosition();

	stCollisionData data;

	data.radius = m_pBall->GetRadius();
	data.x = vecBallPos.x;
	data.y = vecBallPos.y;
	data.velX = vecBallVel.x;
	data.velY = vecBallVel.y;

	if (TestCollision(&data, true))
	{		
		vecBallPos.x = data.x;
		vecBallPos.y = data.y;
		vecBallVel.x = data.velX;
		vecBallVel.y = data.velY;

		m_pBall->SetPosition(vecBallPos);
		m_pBall->SetVelocity(vecBallVel);

		if ((m_fActualTime - m_fLastBounceSound) > 0.15f)
		{
			m_fLastBounceSound = m_fActualTime;
			CGame::GetSoundPlayer()->PlayBounce(m_iBounceSound);
			m_iBounceSound++;
			m_iBounceSound %= 30;
		}		
	}	
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderEnemies
// Creador:    Nacho (AMD)
// Fecha:      Saturday  10/02/2007  14:16:01
//--------------------------------------------------------------------
void CLevel::RenderEnemies(void)
{
	std::list<int> renderEnemyList[4];

	float camPosX = -m_pCamera->GetPosition().x;
	float camPosY = m_pCamera->GetPosition().y;

	for (int i=0; i< m_iNumEnemies; i++)
	{
		if (m_pEnemies[i].active)
		{
			float eneX = (float)m_pEnemies[i].posX;
			float eneY = (float)m_pEnemies[i].posY;

			if ((eneX < (camPosX + 40.0f)) && (eneX > (camPosX - 45.0f)))
			{
				if ((eneY < (camPosY + 20.0f)) && (eneY > (camPosY - 30.0f)))
				{						
					renderEnemyList[m_pEnemies[i].type].push_back(i);
				}
			}	
		}
	}

	for (int i=0; i<4; i++)
	{
		if (renderEnemyList[i].empty())
		{
			continue;
		}
		else
		{
			m_pVideoManager->EnableTexture(m_pEnemiesTexture[i]);

			INT_LIST_ITER itor;

			for(itor = renderEnemyList[i].begin(); itor != renderEnemyList[i].end(); ++itor)
			{
				int enemy = (*itor);

				switch (i)
				{
					///--- sierra
					case 0:
					{
						VECTOR3 pos = { m_pEnemies[enemy].posX + 4.0f, (float)-m_pEnemies[enemy].posY -4.0f, 4.0f };						
						VECTOR3 scale = { 0.05f, 0.05f, 0.05f };

						sceGumMatrixMode(GU_MODEL);
						sceGumLoadIdentity();
						{
							sceGumTranslate(&pos);	
							sceGumRotateZ(MAT_ToRadians(-m_fAngU));
							sceGumScale(&scale);				
						}				
						
						break;
					}
					///--- dirigidos
					case 1:
					{
						VECTOR3 scale = { 0.073f, 0.073f, 0.073f };

						sceGumMatrixMode(GU_MODEL);
						sceGumLoadIdentity();
						{
							switch 	(m_pEnemies[enemy].rot)
							{
								case 0:
								{	
									VECTOR3 pos = { m_pEnemies[enemy].posX, (float)-m_pEnemies[enemy].posY -4.0f, 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									sceGumRotateZ(MAT_ToRadians(270.0f));
									break;
								}
								case 1:
								{
									VECTOR3 pos = { m_pEnemies[enemy].posX + 4.0f, (float)-m_pEnemies[enemy].posY -8.0f, 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									break;
								}
								case 2:
								{
									VECTOR3 pos = { m_pEnemies[enemy].posX + 8.0f, (float)-m_pEnemies[enemy].posY -4.0f, 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									sceGumRotateZ(MAT_ToRadians(90.0f));
									break;
								}
								case 3:
								{	
									VECTOR3 pos = { m_pEnemies[enemy].posX + 4.0f, (float)-m_pEnemies[enemy].posY , 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									sceGumRotateZ(MAT_ToRadians(180.0f));
									break;
								}
							}					
						}	

						break;
					}
					///--- rectos
					case 2:
					{
						VECTOR3 scale = { 0.48f, 0.48f, 0.48f };

						sceGumMatrixMode(GU_MODEL);
						sceGumLoadIdentity();
						{
							switch 	(m_pEnemies[enemy].rot)
							{
								case 0:
								{	
									VECTOR3 pos = { m_pEnemies[enemy].posX, (float)-m_pEnemies[enemy].posY -4.0f, 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									sceGumRotateZ(MAT_ToRadians(270.0f));
									break;
								}
								case 1:
								{
									VECTOR3 pos = { m_pEnemies[enemy].posX + 4.0f, (float)-m_pEnemies[enemy].posY -8.0f, 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									break;
								}
								case 2:
								{
									VECTOR3 pos = { m_pEnemies[enemy].posX + 8.0f, (float)-m_pEnemies[enemy].posY -4.0f, 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									sceGumRotateZ(MAT_ToRadians(90.0f));
									break;
								}
								case 3:
								{	
									VECTOR3 pos = { m_pEnemies[enemy].posX + 4.0f, (float)-m_pEnemies[enemy].posY , 4.0f };						

									sceGumTranslate(&pos);							
									sceGumScale(&scale);
									sceGumRotateZ(MAT_ToRadians(180.0f));
									break;
								}
							}		
						}	

						break;
					}
					///--- rebota
					case 3:
					{
						VECTOR3 pos = { m_pEnemies[enemy].posX + 4.0f, (float)-m_pEnemies[enemy].posY -4.0f, 4.0f };						
						VECTOR3 scale = { 0.05f, 0.05f, 0.05f };

						sceGumMatrixMode(GU_MODEL);
						sceGumLoadIdentity();
						{
							sceGumTranslate(&pos);				
							sceGumRotateZ(MAT_ToRadians(m_fAngU));
							sceGumScale(&scale);	
						}

						break;
					}
				}

				m_pEnemiesMeshes[i]->Render();
			}
		}
	}
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderTrans
// Creador:    Nacho (AMD)
// Fecha:      viernes, 09 de febrero de 2007, 16:52:23
//--------------------------------------------------------------------
void CLevel::RenderTrans(void)
{
	float camPosX = -m_pCamera->GetPosition().x;
	float camPosY = m_pCamera->GetPosition().y;

	m_pVideoManager->EnableTexture(m_pTransTexture);

	for (int i=0; i<m_iNumTrans; i++)
	{
		float objX = (float)m_pTrans[i].posX;
		float objY = (float)m_pTrans[i].posY;

		if ((objX < (camPosX + 40.0f)) && (objX > (camPosX - 45.0f)))
		{
			if ((objY < (camPosY + 30.0f)) && (objY > (camPosY - 30.0f)))
			{
				VECTOR3 pos = { objX + 4.0f, (float)-objY -4.0f, 0.0f };
				VECTOR3 pos2 = { -4.0f, -4.0f, 0.0f };
				VECTOR3 scale = { 8.0f, 8.0f, 8.0f };

				sceGumMatrixMode(GU_MODEL);
				sceGumLoadIdentity();
				{
					sceGumTranslate(&pos);				
					sceGumRotateZ(MAT_ToRadians(m_fAngTrans));
					sceGumTranslate(&pos2);
					sceGumScale(&scale);				
				}		

				sceGuEnable(GU_BLEND);
				sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

				m_pQuadObjs->Render();

				sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);

				sceGumMatrixMode(GU_MODEL);
				sceGumLoadIdentity();
				{
					sceGumTranslate(&pos);				
					sceGumRotateZ(MAT_ToRadians(m_fAngTrans2));
					sceGumTranslate(&pos2);
					sceGumScale(&scale);				
				}

				m_pQuadObjs->Render();

				sceGuDisable(GU_BLEND);
			}
		}
	}
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderEnd
// Creador:    Nacho (AMD)
// Fecha:      viernes, 09 de febrero de 2007, 17:17:32
//--------------------------------------------------------------------
void CLevel::RenderEnd(void)
{
	float objX = (float)m_theLevel.EndX;
	float objY = (float)m_theLevel.EndY;
	float camPosX = -m_pCamera->GetPosition().x;
	float camPosY = m_pCamera->GetPosition().y;

	if ((objX < (camPosX + 40.0f)) && (objX > (camPosX - 45.0f)))
	{
		if ((objY < (camPosY + 30.0f)) && (objY > (camPosY - 30.0f)))
		{	
			m_pVideoManager->EnableTexture(m_pEndTexture);
			
			VECTOR3 pos = { objX, -objY -8.0f, 0.0f };
			VECTOR3 scale = { 8.0f, 8.0f, 8.0f };

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				sceGumTranslate(&pos);
				sceGumScale(&scale);				
			}		

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

			m_pQuadObjs->Render();		

			sceGuDisable(GU_BLEND);	
		}
	}
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderSectors
// Propósito:  
// Fecha:      jueves, 30 de noviembre de 2006, 17:51:14
//--------------------------------------------------------------------
void CLevel::RenderSectors(void)
{
	float camPosX = -m_pCamera->GetPosition().x;
	float camPosY = m_pCamera->GetPosition().y;

	int cx = ((int)camPosX) >> 5; 
	int cy = ((int)camPosY) >> 5;

	int minX = MAT_Clamp((cx - 2), 0, m_theLevel.NumSecX);
	int maxX = MAT_Clamp((cx + 3), 0, m_theLevel.NumSecX);
	int minY = MAT_Clamp((cy - 1), 0, m_theLevel.NumSecY);
	int maxY = MAT_Clamp((cy + 2), 0, m_theLevel.NumSecY);	

	std::list<int> renderCubeList[13];
	std::list<int> renderObjList[10];	

	m_RenderedCubes.ResetAll();
	m_RenderedObjs.ResetAll();

	//////////////////////////////////////////////////////////////////////

	for (int sx=minX; sx<maxX; sx++)
	{
		for (int sy=minY; sy<maxY; sy++)
		{		
			int sector = (sy * m_theLevel.NumSecX) + sx;

			//////////////////////////////////////////////////////////////////////

			for (int i=0; i< m_pSectors[sector].numCubes; i++)
			{
				int cube = m_pSectors[sector].IndCub[i];					

				if (m_RenderedCubes.IsSet(cube))
				{
					continue;
				}
				else
				{
					float cubeX = (float)m_pCubes[cube].posX;
					float cubeY = (float)m_pCubes[cube].posY;

					m_RenderedCubes.Set(cube);

					if (m_bHasRedKey)
					{
						if (m_pCubes[cube].texLat == 18)
							continue;
					}
					if (m_bHasBlueKey)
					{
						if (m_pCubes[cube].texLat == 19)
							continue;
					}

					if ((cubeX < (camPosX + 40.0f)) && (cubeX > (camPosX - 45.0f)))
					{
						if ((cubeY < (camPosY + 20.0f)) && (cubeY > (camPosY - 30.0f)))
						{						
							renderCubeList[m_pInfoCubes[cube].iMeshCube].push_back(cube);
						}
					}	
				}
			}


			//////////////////////////////////////////////////////////////////////

			for (int i=0; i< m_pSectors[sector].numObjs; i++)
			{			
				int obj = m_pSectors[sector].IndObj[i];
				
				if (m_RenderedObjs.IsSet(obj))
				{
					continue;
				}
				else
				{	
					if (!m_pObjects[obj].active)
						continue;

					float objX = (float)m_pObjects[obj].posX;
					float objY = (float)m_pObjects[obj].posY;

					m_RenderedObjs.Set(obj);

					if ((objX < (camPosX + 40.0f)) && (objX > (camPosX - 45.0f)))
					{
						if ((objY < (camPosY + 30.0f)) && (objY > (camPosY - 30.0f)))
						{								
							renderObjList[m_pObjects[obj].type].push_back(obj);
						}
					}	
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////

	for (int i=0; i<13; i++)
	{	
		if (renderCubeList[i].empty())
		{
			continue;
		}
		else
		{
			m_pVideoManager->EnableTexture(m_pMeshCube[i].texture);

			INT_LIST_ITER itor;

			for(itor = renderCubeList[i].begin(); itor != renderCubeList[i].end(); ++itor)
			{
				int cube = (*itor);

				sceGumMatrixMode(GU_MODEL);
				sceGumLoadIdentity();
				{
					ScePspFVector3 pos = { (float)m_pCubes[cube].posX+4.0f, (float)-m_pCubes[cube].posY -4.0f, 4.0f };
					sceGumTranslate(&pos);
				}

				m_pInfoCubes[cube].pMeshCube->theMesh->Render();

				if (m_pInfoCubes[cube].timeRemaining > 0.0f)
				{
					if (m_pMeshCube[i].textureAdd)
					{					
						int ambient = (int)((m_pInfoCubes[cube].timeRemaining / TIME_LIGHT) * 255.0f);

						sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

						sceGuAmbientColor(COLOR_ARGB(255, ambient, ambient, ambient));

						sceGuEnable(GU_BLEND);
						sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);


						m_pVideoManager->EnableTexture(m_pMeshCube[i].textureAdd);

						m_pInfoCubes[cube].pMeshCube->theMesh->Render();

						sceGuDisable(GU_BLEND);
						sceGuAmbientColor(0xffffffff);

						sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);

						m_pVideoManager->EnableTexture(m_pMeshCube[i].texture);
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////

	for (int i=0; i<10; i++)
	{	
		if (renderObjList[i].empty())
		{
			continue;
		}
		else
		{		
			m_pVideoManager->EnableTexture(m_pObjTextures[i]);

			INT_LIST_ITER itor;

			for(itor = renderObjList[i].begin(); itor != renderObjList[i].end(); ++itor)
			{
				RenderObject(*itor);				
			}
		}
	}	
}


//--------------------------------------------------------------------
// Función:    CLevel::RenderObject
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 07 de febrero de 2007, 21:00:42
//--------------------------------------------------------------------
void CLevel::RenderObject(int object)
{
	int type = m_pObjects[object].type;

	switch (type)
	{
		case 0:
		case 1:
		case 2:
		case 3:		///--- gemas
		{
			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos = { (float)m_pObjects[object].posX+4.0f, (float)-m_pObjects[object].posY -4.0f, 6.0f };
				VECTOR3 scale = { 0.08f, 0.08f, 0.08f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);
				sceGumRotateY(MAT_ToRadians(m_fAngGems + m_pObjects[object].rot));
			}

			sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

		
			VECTOR3 posLight = m_pBall->GetPosition();
			posLight.z = 1.0f;
			posLight.y = 0.0f;
			posLight.x = 0.0f;

			sceGuEnable(GU_LIGHTING);
			sceGuEnable(GU_LIGHT0);
			sceGuLight(0, GU_DIRECTIONAL, GU_DIFFUSE, &posLight);
			sceGuLightColor(0,GU_DIFFUSE, 0xffffffff);			
			sceGuLightAtt(0, 1.0f, 0.0f, 0.0f);	

			sceGuSpecular(10.0f);
			sceGuAmbient(0x00202020);

			sceGuColor(0xffffffff);

			m_pObjMeshes[type]->Render();

			sceGuLight(0, GU_DIRECTIONAL, GU_DIFFUSE_AND_SPECULAR, &posLight);
			sceGuLightColor(0,GU_DIFFUSE, 0x00000000);
			sceGuLightColor(0,GU_SPECULAR, 0xffffffff);

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, COLOR_ARGB(255, 255, 255, 255), COLOR_ARGB(255, 255, 255, 255));

			m_pObjMeshes[type]->Render();

			sceGuDisable(GU_BLEND);

			sceGuDisable(GU_LIGHT0);
			sceGuDisable(GU_LIGHTING);	
			sceGuAmbient(0xffffffff);			

			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
			
			break;
		}
		case 4:		///--- pincho
		{
			if (m_pObjects[object].posY >= 0)
			{
				sceGumMatrixMode(GU_MODEL);
				sceGumLoadIdentity();
				{
					VECTOR3 pos = { (float)m_pObjects[object].posX+1.0f, (float)-m_pObjects[object].posY -1.0f, 4.0f };
					VECTOR3 scale = { 0.08f, 0.08f, 0.08f };
					sceGumTranslate(&pos);
					sceGumScale(&scale);
				}	

				m_pObjMeshes[type]->Render();
			}
			break;
		}
		case 5:		///--- llaves		
		case 6:		///--- llave 2
		{
			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos = { (float)m_pObjects[object].posX+1.0f, (float)-m_pObjects[object].posY -1.0f, 4.0f };
				VECTOR3 scale = { 0.05f, 0.04f, 0.04f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);
				sceGumRotateY(MAT_ToRadians(m_fAngGems));
			}	

			m_pObjMeshes[type]->Render();
			break;
		}
		case 7:		///--- lanzador
		{
			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos = { (float)m_pObjects[object].posX+1.0f, (float)-m_pObjects[object].posY -1.0f, 4.0f };
				VECTOR3 scale = { 0.08f, 0.08f, 0.08f };
				
				sceGumTranslate(&pos);
				sceGumScale(&scale);

				switch 	(m_pObjects[object].rot)
				{
					case 0:
					{							
						sceGumRotateZ(MAT_ToRadians(270.0f));
						break;
					}					
					case 2:
					{
						sceGumRotateZ(MAT_ToRadians(90.0f));
						break;
					}
					case 3:
					{							
						sceGumRotateZ(MAT_ToRadians(180.0f));
						break;
					}
				}				
			}

			m_pObjMeshes[type]->Render();
			break;
		}
		case 8:		///--- impulsor
		{
			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				
				VECTOR3 scale = { 8.0f, 8.0f, 8.0f };				

				switch 	(m_pObjects[object].rot)
				{	
					case 0:
					{	
						VECTOR3 pos = { (float)m_pObjects[object].posX, (float)-m_pObjects[object].posY-8.0f, 0.0f };
						sceGumTranslate(&pos);
						sceGumScale(&scale);						
						break;
					}
					case 1:
					{	
						VECTOR3 pos = { (float)m_pObjects[object].posX + 8.0f, (float)-m_pObjects[object].posY-8.0f, 0.0f };
						sceGumTranslate(&pos);
						sceGumScale(&scale);
						sceGumRotateZ(MAT_ToRadians(90.0f));
						break;
					}
					case 2:
					{
						VECTOR3 pos = { (float)m_pObjects[object].posX + 8.0f, (float)-m_pObjects[object].posY, 0.0f };
						sceGumTranslate(&pos);
						sceGumScale(&scale);
						sceGumRotateZ(MAT_ToRadians(180.0f));
						break;
					}
					case 3:
					{
						VECTOR3 pos = { (float)m_pObjects[object].posX, (float)-m_pObjects[object].posY, 0.0f };
						sceGumTranslate(&pos);
						sceGumScale(&scale);
						sceGumRotateZ(MAT_ToRadians(270.0f));
						break;
					}
				}
			}

			sceGuTexMapMode(GU_TEXTURE_MATRIX, 1, 1);

			sceGumMatrixMode(GU_TEXTURE);
			sceGumLoadIdentity();
			{
				VECTOR3 pos = { m_fOffsetArrow, 0.0f, 1.0f };
				sceGumTranslate(&pos);
			}

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

			m_pQuadObjs->Render();

			sceGuDisable(GU_BLEND);

			sceGuTexMapMode(GU_TEXTURE_COORDS, 0, 0);
			break;
		}
		case 9:		///--- inflador
		{
			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos = { (float)m_pObjects[object].posX + 4.0f, (float)-m_pObjects[object].posY -4.0f, 0.0f };
				VECTOR3 pos2 = { -4.0f, -4.0f, 0.0f };
				VECTOR3 scale = { 8.0f, 8.0f, 8.0f };

				
				sceGumTranslate(&pos);				
				sceGumRotateZ(MAT_ToRadians(m_fAngBlow));
				sceGumTranslate(&pos2);
				sceGumScale(&scale);				
			}		

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

			m_pQuadObjs->Render();

			sceGuDisable(GU_BLEND);
			break;
		}
	}		
}


//--------------------------------------------------------------------
// Función:    CLevel::TestSimpleCollision
// Creador:    Nacho (AMD)
// Fecha:      Tuesday  12/06/2007  23:20:45
//--------------------------------------------------------------------
bool CLevel::TestSimpleCollision(stCollisionData* pData)
{	
	int sectorx = ((int)(pData->x - pData->radius)) >> 5;
	int sectory = ((int)(pData->y - pData->radius)) >> 5;

	int sectorx1 = ((int)(pData->x + pData->radius)) >> 5;		
	int sectory1 = ((int)(pData->y + pData->radius)) >> 5;

	int sectors[4];

	sectors[0] = (sectory * m_theLevel.NumSecX) + sectorx;

	if (sectorx1 != sectorx)
	{
		sectors[1] = (sectory * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[1] = -1;
	}

	if (sectory1 != sectory)
	{
		sectors[2] = (sectory1 * m_theLevel.NumSecX) + sectorx;	
	}
	else
	{
		sectors[2] = -1;
	}

	if ((sectorx1 != sectorx) && (sectory1 != sectory))
	{
		sectors[3] = (sectory1 * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[3] = -1;
	}

	for (int a=0; a<4; a++)
	{
		int sector = sectors[a];
		
		if (sector < 0)
			continue;
			
		for (int i=0; i< m_pSectors[sector].numCubes; i++)
		{
			int iCube = m_pSectors[sector].IndCub[i];
			CUBE* pCube = &m_pCubes[iCube];

			if (m_bHasRedKey)
			{
				if (pCube->texLat == 18)
					continue;
			}
			if (m_bHasBlueKey)
			{
				if (pCube->texLat == 19)
					continue;
			}
			
			float cubox = pCube->posX;
			float cuboy = pCube->posY;		

			float cubox2 = cubox + 8.0f;
			float cuboy2 = cuboy + 8.0f;

			float objectx = pData->x - pData->radius;
			float objecty = pData->y - pData->radius;

			float objectx2 = pData->x + pData->radius;
			float objecty2 = pData->y + pData->radius;			

			if ((cubox2 > objectx) && (cuboy2 > objecty) && (objectx2 > cubox) && (objecty2 > cuboy))
			{						
				if (pData->velY != 0.0f)
				{		
					pData->velY = -pData->velY;
					return true;			
				}
				else if (pData->velX != 0.0f)
				{	
					pData->velX = -pData->velX;
					return true; 				
				}	
			}
		}
	}

	return false;   
}


//--------------------------------------------------------------------
// Función:    CLevel::TestCollisionInflate
// Creador:    Nacho (AMD)
// Fecha:      Friday  15/06/2007  18:08:20
//--------------------------------------------------------------------
bool CLevel::TestCollisionInflate(stCollisionData* pData)
{	
	bool ret = false;

	int sectorx = ((int)(pData->x - pData->radius)) >> 5;
	int sectory = ((int)(pData->y - pData->radius)) >> 5;

	int sectorx1 = ((int)(pData->x + pData->radius)) >> 5;		
	int sectory1 = ((int)(pData->y + pData->radius)) >> 5;

	int sectors[4];

	sectors[0] = (sectory * m_theLevel.NumSecX) + sectorx;

	if (sectorx1 != sectorx)
	{
		sectors[1] = (sectory * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[1] = -1;
	}

	if (sectory1 != sectory)
	{
		sectors[2] = (sectory1 * m_theLevel.NumSecX) + sectorx;	
	}
	else
	{
		sectors[2] = -1;
	}

	if ((sectorx1 != sectorx) && (sectory1 != sectory))
	{
		sectors[3] = (sectory1 * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[3] = -1;
	}

	for (int a=0; a<4; a++)
	{
		int sector = sectors[a];

		if (sector < 0)
			continue;

		for (int i=0; i< m_pSectors[sector].numCubes; i++)
		{
			int iCube = m_pSectors[sector].IndCub[i];
			CUBE* pCube = &m_pCubes[iCube];

			if (m_bHasRedKey)
			{
				if (pCube->texLat == 18)
					continue;
			}
			if (m_bHasBlueKey)
			{
				if (pCube->texLat == 19)
					continue;
			}

			float cubox = pCube->posX;
			float cuboy = pCube->posY;		

			float cubox2 = cubox + 8.0f;
			float cuboy2 = cuboy + 8.0f;

			bool colLeft = !pCube->sides[0];
			bool colRight = !pCube->sides[2];
			bool colTop = !pCube->sides[1];
			bool colDown = !pCube->sides[3];

			if ((pData->x >= cubox) && (pData->x <= cubox2))
			{
				/// esta arriba
				if (colTop)
				{
					if (((pData->y - cuboy) > -pData->radius) && (pData->y < cuboy2))
					{						
						pData->y -= (pData->y + pData->radius - cuboy);						
						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						ret = true; 
					}
				}
				/// esta abajo
				else if (colDown)
				{
					if (((cuboy2 - pData->y) > -pData->radius) && (pData->y > cuboy))
					{						
						pData->y += (cuboy2 - (pData->y - pData->radius));
						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						ret = true; 
					}			
				}
			}
			else if ((pData->y >= cuboy) && (pData->y <= cuboy2))
			{	
				/// esta izqda
				if (colLeft)
				{
					if (((pData->x - cubox) > -pData->radius) && (pData->x < cubox2))
					{
						
						pData->x -= (pData->x + pData->radius - cubox );
						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;					
						ret = true; 
					}   
				}
				/// esta dcha
				else if (colRight)
				{
					if (((cubox2 - pData->x) > -pData->radius) && (pData->x > cubox))
					{						
						pData->x += (pData->radius - (pData->x - cubox2));  
						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						ret = true; 
					}		
				}
			}	

			else if ((pData->y < cuboy) && (pData->x < cubox))
			{	
				if (colTop && colLeft)
				{
					float disy = cuboy - pData->y;
					float disx = cubox - pData->x;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (-disx) / longdis;
						float disynor = (-disy) / longdis;
						
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));
						
						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;						

						ret = true; 
					}        
				}
			}
			/// esta arriba dcha
			else if ((pData->y < cuboy) && (pData->x > cubox2))
			{
				if (colTop && colRight)
				{
					float disy = cuboy - pData->y;
					float disx = pData->x - cubox2;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (disx) / longdis;
						float disynor = (-disy) / longdis;
						
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));
						
						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;						

						ret = true; 
					}
				}
			}
			/// esta abajo izqda
			else if ((pData->y > cuboy2) && (pData->x < cubox))
			{
				if (colDown && colLeft)
				{
					float disy = pData->y - cuboy2;
					float disx = cubox - pData->x;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (-disx) / longdis;
						float disynor = (disy) / longdis;
						
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));
						
						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;						

						ret = true; 
					}        
				}
			}
			/// esta abajo dcha
			else if ((pData->y > cuboy2) && (pData->x > cubox2))
			{
				if (colDown && colRight)
				{
					float disy = pData->y - cuboy2;
					float disx = pData->x - cubox2;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (disx) / longdis;
						float disynor = (disy) / longdis;
						
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));

						m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;					

						ret = true; 
					}
				}
			}			
		}
	}

	return ret;    
}

//--------------------------------------------------------------------
// Función:    CLevel::TestCollision
// Creador:    Nacho (AMD)
// Fecha:      viernes, 02 de febrero de 2007, 23:31:56
//--------------------------------------------------------------------
bool CLevel::TestCollision(stCollisionData* pData, bool bIsTheBall)
{	
	bool ret = false;

	int sectorx = ((int)(pData->x - pData->radius)) >> 5;
	int sectory = ((int)(pData->y - pData->radius)) >> 5;

	int sectorx1 = ((int)(pData->x + pData->radius)) >> 5;		
	int sectory1 = ((int)(pData->y + pData->radius)) >> 5;

	int sectors[4];

	sectors[0] = (sectory * m_theLevel.NumSecX) + sectorx;

	if (sectorx1 != sectorx)
	{
		sectors[1] = (sectory * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[1] = -1;
	}

	if (sectory1 != sectory)
	{
		sectors[2] = (sectory1 * m_theLevel.NumSecX) + sectorx;	
	}
	else
	{
		sectors[2] = -1;
	}

	if ((sectorx1 != sectorx) && (sectory1 != sectory))
	{
		sectors[3] = (sectory1 * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[3] = -1;
	}

	for (int a=0; a<4; a++)
	{
		int sector = sectors[a];
		
		if (sector < 0)
			continue;
			
		for (int i=0; i< m_pSectors[sector].numCubes; i++)
		{
			int iCube = m_pSectors[sector].IndCub[i];
			CUBE* pCube = &m_pCubes[iCube];

			if (m_bHasRedKey)
			{
				if (pCube->texLat == 18)
					continue;
			}
			if (m_bHasBlueKey)
			{
				if (pCube->texLat == 19)
					continue;
			}
			
			float cubox = pCube->posX;
			float cuboy = pCube->posY;		

			float cubox2 = cubox + 8.0f;
			float cuboy2 = cuboy + 8.0f;

			bool colLeft = !pCube->sides[0];
			bool colRight = !pCube->sides[2];
			bool colTop = !pCube->sides[1];
			bool colDown = !pCube->sides[3];

			bool dirLeft = (pData->velX < 0.0f);
			bool dirRight = (pData->velX > 0.0f);
			bool dirTop = (pData->velY < 0.0f);
			bool dirDown = (pData->velY > 0.0f);			

			if ((pData->x >= cubox) && (pData->x <= cubox2))
			{
				/// esta arriba
				if (dirDown && colTop)
				{
					if (((pData->y - cuboy) > -pData->radius) && (pData->y < cuboy2))
					{
						pData->velY = -pData->velY;
						pData->y -= (pData->y + pData->radius - cuboy) ;

						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}
				}
				/// esta abajo
				else if (dirTop && colDown)
				{
					if (((cuboy2 - pData->y) > -pData->radius) && (pData->y > cuboy))
					{
						pData->velY = -pData->velY;
						pData->y += (cuboy2 - (pData->y - pData->radius)) ;

						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}			
				}
			}

			else if ((pData->y >= cuboy) && (pData->y <= cuboy2))
			{	
				/// esta izqda
				if (dirRight && colLeft)
				{
					if (((pData->x - cubox) > -pData->radius) && (pData->x < cubox2))
					{
						pData->velX = -pData->velX;
						pData->x -= (pData->x + pData->radius - cubox ) ;

						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}   
				}
				/// esta dcha
				else if (dirLeft && colRight)
				{
					if (((cubox2 - pData->x) > -pData->radius) && (pData->x > cubox))
					{
						pData->velX = -pData->velX;
						pData->x += (pData->radius - (pData->x - cubox2)) ;  
	  
						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}		
				}
			}
			/// esta arriba izqda
			else if ((pData->y < cuboy) && (pData->x < cubox))
			{	
				if ((dirDown || dirRight) && colTop && colLeft)
				{
					float disy = cuboy - pData->y;
					float disx = cubox - pData->x;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (-disx) / longdis;
						float disynor = (-disy) / longdis;
						float vel = MAT_RaizCuadrada((pData->velX * pData->velX) + (pData->velY * pData->velY));
						pData->velX = disxnor * vel;
						pData->velY = disynor * vel;

						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));

						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}        
				}
			}
			/// esta arriba dcha
			else if ((pData->y < cuboy) && (pData->x > cubox2))
			{
				if ((dirDown || dirLeft) && colTop && colRight)
				{
					float disy = cuboy - pData->y;
					float disx = pData->x - cubox2;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (disx) / longdis;
						float disynor = (-disy) / longdis;
						float vel = MAT_RaizCuadrada((pData->velX * pData->velX) + (pData->velY * pData->velY));
						pData->velX = disxnor * vel;
						pData->velY = disynor * vel;

						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));

						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}
				}
			}
			/// esta abajo izqda
			else if ((pData->y > cuboy2) && (pData->x < cubox))
			{
				if ((dirTop || dirRight) && colDown && colLeft)
				{
					float disy = pData->y - cuboy2;
					float disx = cubox - pData->x;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (-disx) / longdis;
						float disynor = (disy) / longdis;
						float vel = MAT_RaizCuadrada((pData->velX * pData->velX) + (pData->velY * pData->velY));
						pData->velX = disxnor * vel;
						pData->velY = disynor * vel;

						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));

						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}        
				}
			}
			/// esta abajo dcha
			else if ((pData->y > cuboy2) && (pData->x > cubox2))
			{
				if ((dirTop || dirLeft) && colDown && colRight)
				{
					float disy = pData->y - cuboy2;
					float disx = pData->x - cubox2;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (disx) / longdis;
						float disynor = (disy) / longdis;
						float vel = MAT_RaizCuadrada((pData->velX * pData->velX) + (pData->velY * pData->velY));
						pData->velX = disxnor * vel;
						pData->velY = disynor * vel;

						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));

						if (bIsTheBall)
						{
							pData->velX *= BOUNCE_BRAKE;
							pData->velY *= BOUNCE_BRAKE;
							m_pInfoCubes[iCube].timeRemaining = TIME_LIGHT;
						}

						ret = true;
					}
				}
			}			
		}
	}

	return ret;    
}


//--------------------------------------------------------------------
// Función:    CLevel::TestCollisionEnemies
// Creador:    Nacho (AMD)
// Fecha:      Friday  15/06/2007  13:40:47
//--------------------------------------------------------------------
bool CLevel::TestCollisionEnemies(stCollisionData* pData)
{	
	bool ret = false;

	int sectorx = ((int)(pData->x - pData->radius)) >> 5;
	int sectory = ((int)(pData->y - pData->radius)) >> 5;

	int sectorx1 = ((int)(pData->x + pData->radius)) >> 5;		
	int sectory1 = ((int)(pData->y + pData->radius)) >> 5;

	int sectors[4];

	sectors[0] = (sectory * m_theLevel.NumSecX) + sectorx;

	if (sectorx1 != sectorx)
	{
		sectors[1] = (sectory * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[1] = -1;
	}

	if (sectory1 != sectory)
	{
		sectors[2] = (sectory1 * m_theLevel.NumSecX) + sectorx;	
	}
	else
	{
		sectors[2] = -1;
	}

	if ((sectorx1 != sectorx) && (sectory1 != sectory))
	{
		sectors[3] = (sectory1 * m_theLevel.NumSecX) + sectorx1;	
	}
	else
	{
		sectors[3] = -1;
	}

	for (int a=0; a<4; a++)
	{
		int sector = sectors[a];

		if (sector < 0)
			continue;

		for (int i=0; i< m_pSectors[sector].numCubes; i++)
		{
			int iCube = m_pSectors[sector].IndCub[i];
			CUBE* pCube = &m_pCubes[iCube];

			if (m_bHasRedKey)
			{
				if (pCube->texLat == 18)
					continue;
			}
			if (m_bHasBlueKey)
			{
				if (pCube->texLat == 19)
					continue;
			}

			float cubox = pCube->posX;
			float cuboy = pCube->posY;		

			float cubox2 = cubox + 8.0f;
			float cuboy2 = cuboy + 8.0f;

			bool colLeft = !pCube->sides[0];
			bool colRight = !pCube->sides[2];
			bool colTop = !pCube->sides[1];
			bool colDown = !pCube->sides[3];

			bool dirLeft = (pData->velX < 0.0f);
			bool dirRight = (pData->velX > 0.0f);
			bool dirTop = (pData->velY < 0.0f);
			bool dirDown = (pData->velY > 0.0f);			

				

			/// esta arriba izqda
			if ((pData->y < cuboy) && (pData->x < cubox))
			{	
				if ((dirDown || dirRight) && colTop && colLeft)
				{
					float disy = cuboy - pData->y;
					float disx = cubox - pData->x;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (-disx) / longdis;
						float disynor = (-disy) / longdis;
					
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));

						ret = true;						
					}        
				}
			}
			/// esta arriba dcha
			if ((pData->y < cuboy) && (pData->x > cubox2))
			{
				if ((dirDown || dirLeft) && colTop && colRight)
				{
					float disy = cuboy - pData->y;
					float disx = pData->x - cubox2;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (disx) / longdis;
						float disynor = (-disy) / longdis;
					
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));					

						ret = true;					
					}
				}
			}
			/// esta abajo izqda
			if ((pData->y > cuboy2) && (pData->x < cubox))
			{
				if ((dirTop || dirRight) && colDown && colLeft)
				{
					float disy = pData->y - cuboy2;
					float disx = cubox - pData->x;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (-disx) / longdis;
						float disynor = (disy) / longdis;
					
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));						

						ret = true;					
					}        
				}
			}
			/// esta abajo dcha
			if ((pData->y > cuboy2) && (pData->x > cubox2))
			{
				if ((dirTop || dirLeft) && colDown && colRight)
				{
					float disy = pData->y - cuboy2;
					float disx = pData->x - cubox2;
					float distanciaVertice = (disx * disx) + (disy * disy); 

					if (distanciaVertice < (pData->radius * pData->radius))
					{
						float longdis = MAT_RaizCuadrada(distanciaVertice);
						float disxnor = (disx) / longdis;
						float disynor = (disy) / longdis;
					
						pData->x += (disxnor * ( pData->radius - longdis));
						pData->y += (disynor * ( pData->radius - longdis));					

						ret = true;					
					}
				}
			}	

			if ((pData->x >= cubox) && (pData->x <= cubox2))
			{
				/// esta arriba
				if (dirDown && colTop)
				{
					if (((pData->y - cuboy) >= -pData->radius) && (pData->y < cuboy2))
					{
						pData->y -= (pData->y + pData->radius - cuboy);

						ret = true; 						
					}
				}
				/// esta abajo
				if (dirTop && colDown)
				{
					if (((cuboy2 - pData->y) >= -pData->radius) && (pData->y > cuboy))
					{
						pData->y += (cuboy2 - (pData->y - pData->radius));

						ret = true;						
					}			
				}
			}

			if ((pData->y >= cuboy) && (pData->y <= cuboy2))
			{	
				/// esta izqda
				if (dirRight && colLeft)
				{
					if (((pData->x - cubox) >= -pData->radius) && (pData->x < cubox2))
					{
						pData->x -= (pData->x + pData->radius - cubox );			

						ret = true; 						
					}   
				}
				/// esta dcha
				if (dirLeft && colRight)
				{
					if (((cubox2 - pData->x) >= -pData->radius) && (pData->x > cubox))
					{
						pData->x += (pData->radius - (pData->x - cubox2));			

						ret = true;
					}		
				}
			}	
		}
	}

	return ret;    
}


//--------------------------------------------------------------------
// Función:    CLevel::LoadLevel
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 17:21:34
//--------------------------------------------------------------------
void CLevel::LoadLevel(int iLevel)
{
	iLevel++;

	char strLevel[PSP_MAX_PATH];

	sprintf(strLevel, "gfx/levels/nhe/nivel%d.nhe", iLevel);

	int fd = sceIoOpen(strLevel, PSP_O_RDONLY, 0777); 

	if(fd <= 0)
	{ 
		Log("CLevel: Error abriendo fichero de nivel\n");	

		return;
	} 

	HEADER header;
	BLOCK blocks[khMaxBlocks];

	sceIoRead(fd, &header, sizeof(HEADER)); 

	if(strcmp(header.ID,"HYDRIUM")!=0)
	{
		Log("CLevel: ID de cabecera incorrecto\n");

		return;
	}

	if(header.version!=27)
	{
		Log("CLevel: Version de cabecera incorrecta\n");
		return;
	}

	sceIoRead(fd, &blocks, sizeof(BLOCK) * 5); 	
	sceIoRead(fd, &m_theLevel, sizeof(LEVEL));

	Log("CLevel: SX %d, SY %d, BG %d, GM %d\n", m_theLevel.NumSecX, m_theLevel.NumSecY, m_theLevel.Back, m_theLevel.Gems);	

	m_iNumCubes = blocks[khCubes].length / sizeof(CUBE);
	m_RenderedCubes.Init(m_iNumCubes);
	Log("CLevel: Leyendo %d cubos\n", m_iNumCubes);	
	m_pCubes = new CUBE[m_iNumCubes];
	m_pInfoCubes = new stInfoCube[m_iNumCubes];
	sceIoLseek(fd, blocks[khCubes].offset, SEEK_SET);
	sceIoRead(fd, m_pCubes, sizeof(CUBE) * m_iNumCubes);

	m_iNumSectors = blocks[khSectors].length / sizeof(SECTOR);
	Log("CLevel: Leyendo %d sectores\n", m_iNumSectors);	
	m_pSectors = new SECTOR[m_iNumSectors];
	sceIoLseek(fd, blocks[khSectors].offset, SEEK_SET);
	sceIoRead(fd, m_pSectors, sizeof(SECTOR) * m_iNumSectors);

	m_iNumObjs = blocks[khObjects].length / sizeof(OBJECT_File);
	m_RenderedObjs.Init(m_iNumObjs);
	Log("CLevel: Leyendo %d objetos\n", m_iNumObjs);
	m_pObjects = new OBJECT[m_iNumObjs];
	OBJECT_File *pObjects = new OBJECT_File[m_iNumObjs];
	sceIoLseek(fd, blocks[khObjects].offset, SEEK_SET);
	sceIoRead(fd, pObjects, sizeof(OBJECT_File) * m_iNumObjs);

	m_iNumTrans = blocks[khTransports].length / sizeof(TRANS);
	Log("CLevel: Leyendo %d teletransporters\n", m_iNumTrans);
	m_pTrans = new TRANS[m_iNumTrans];
	sceIoLseek(fd, blocks[khTransports].offset, SEEK_SET);
	sceIoRead(fd, m_pTrans, sizeof(TRANS) * m_iNumTrans);

	m_iNumEnemies = blocks[khEnemies].length / sizeof(ENEMY_File);
	Log("CLevel: Leyendo %d enemigos\n", m_iNumEnemies);
	m_pEnemies = new ENEMY[m_iNumEnemies];
	ENEMY_File *pEnemies = new ENEMY_File[m_iNumEnemies];
	sceIoLseek(fd, blocks[khEnemies].offset, SEEK_SET);
	sceIoRead(fd, pEnemies, sizeof(ENEMY_File) * m_iNumEnemies);
	
	sceIoClose(fd);
	
	//////////////////////////////////////////////////////////////////////
	
	m_iRemainingGems = 0; 

	for (int i=0;i < m_iNumObjs; i++)
	{
		m_pObjects[i].active = true;
		m_pObjects[i].posX = pObjects[i].posX;
		m_pObjects[i].posY = pObjects[i].posY;

		if ((pObjects[i].type>=0) && (pObjects[i].type<4))	///--- gemas
		{
			if (pObjects[i].type==0)
			{
				m_iRemainingGems++;
			}
			m_pObjects[i].rot = MAT_RandomInt(1,350);
		}
		else
			m_pObjects[i].rot = pObjects[i].rot;
		
		m_pObjects[i].type = pObjects[i].type;
		m_pObjects[i].timeShot = 0.0f;
	}

	SafeDeleteArray(pObjects);

	//////////////////////////////////////////////////////////////////////

	for (int i=0;i < m_iNumEnemies; i++)
	{
		m_pEnemies[i].posX = (float)pEnemies[i].posX;
		m_pEnemies[i].posY = (float)pEnemies[i].posY;
		m_pEnemies[i].rot = pEnemies[i].rot;
		m_pEnemies[i].type = pEnemies[i].type;
		m_pEnemies[i].velX = 0.0f;
		m_pEnemies[i].velY = 0.0f;
		m_pEnemies[i].timeShot = 0.0f;
		m_pEnemies[i].active = true;

		///--- rebota
		if (m_pEnemies[i].type == 3)
		{
			if (m_pEnemies[i].rot==0)
			{
				m_pEnemies[i].velX = 15.0f;						
			}
			else if (m_pEnemies[i].rot==3)
			{
				m_pEnemies[i].velY = 15.0f;							
			}
			else if (m_pEnemies[i].rot==2)
			{
				m_pEnemies[i].velX = -15.0f;						
			}
			else if (m_pEnemies[i].rot==1)
			{
				m_pEnemies[i].velY = -15.0f;								
			}
		}
	}

	SafeDeleteArray(pEnemies);

	//////////////////////////////////////////////////////////////////////
	
	for (int i=0; i<m_iNumCubes; i++)
	{
		m_pInfoCubes[i].timeRemaining=0.0f;

		if (m_pCubes[i].tex == 2 && m_pCubes[i].texLat == 0)
		{
			if (m_pMeshCube[0].texture == NULL)
			{
				m_pMeshCube[0].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_01.psptex", 64, 64);
				m_pMeshCube[0].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_01_add.psptex", 64, 64);
				m_pMeshCube[0].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[0];
			m_pInfoCubes[i].iMeshCube = 0;
		}
		else if (m_pCubes[i].tex == 3 && m_pCubes[i].texLat == 0)
		{
			if (m_pMeshCube[1].texture == NULL)
			{
				m_pMeshCube[1].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_02.psptex", 64, 64);
				m_pMeshCube[1].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_02_add.psptex", 64, 64);
				m_pMeshCube[1].theMesh = m_pWireCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[1];
			m_pInfoCubes[i].iMeshCube = 1;
		}
		else if (m_pCubes[i].tex == 10 && m_pCubes[i].texLat == 11)
		{
			if (m_pMeshCube[2].texture == NULL)
			{
				m_pMeshCube[2].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_03.psptex", 64, 64);
				m_pMeshCube[2].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_03_add.psptex", 64, 64);
				m_pMeshCube[2].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[2];
			m_pInfoCubes[i].iMeshCube = 2;
		}
		else if (m_pCubes[i].tex == 12 && m_pCubes[i].texLat == 17)
		{
			if (m_pMeshCube[3].texture == NULL)
			{
				m_pMeshCube[3].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_04.psptex", 64, 64);
				m_pMeshCube[3].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_04_add.psptex", 64, 64);
				m_pMeshCube[3].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[3];
			m_pInfoCubes[i].iMeshCube = 3;
		}
		else if (m_pCubes[i].tex == 14 && m_pCubes[i].texLat == 13)
		{
			if (m_pMeshCube[4].texture == NULL)
			{
				m_pMeshCube[4].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_05.psptex", 64, 64);
				m_pMeshCube[4].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_05_add.psptex", 64, 64);
				m_pMeshCube[4].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[4];
			m_pInfoCubes[i].iMeshCube = 4;
		}
		else if (m_pCubes[i].tex == 16 && m_pCubes[i].texLat == 13)
		{
			if (m_pMeshCube[5].texture == NULL)
			{
				m_pMeshCube[5].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_05.psptex", 64, 64);
				m_pMeshCube[5].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_06_add.psptex", 64, 64);
				m_pMeshCube[5].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[5];
			m_pInfoCubes[i].iMeshCube = 5;
		}
		else if (m_pCubes[i].tex == 18 && m_pCubes[i].texLat == 18) //puerta roja
		{
			if (m_pMeshCube[6].texture == NULL)
			{
				m_pMeshCube[6].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_07.psptex", 64, 64);
				m_pMeshCube[6].textureAdd = NULL;
				m_pMeshCube[6].theMesh = m_pDoorCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[6];
			m_pInfoCubes[i].iMeshCube = 6;
		}
		else if (m_pCubes[i].tex == 19 && m_pCubes[i].texLat == 19) //puerta azul
		{
			if (m_pMeshCube[7].texture == NULL)
			{
				m_pMeshCube[7].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_08.psptex", 64, 64);
				m_pMeshCube[7].textureAdd = NULL;
				m_pMeshCube[7].theMesh = m_pDoorCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[7];
			m_pInfoCubes[i].iMeshCube = 7;
		}
		else if (m_pCubes[i].tex == 8 && m_pCubes[i].texLat == 7)
		{
			if (m_pMeshCube[8].texture == NULL)
			{
				m_pMeshCube[8].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_02.psptex", 64, 64);
				m_pMeshCube[8].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_09_add.psptex", 64, 64);
				m_pMeshCube[8].theMesh = m_pWireCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[8];
			m_pInfoCubes[i].iMeshCube = 8;
		}
		else if (m_pCubes[i].tex == 9 && m_pCubes[i].texLat == 9)
		{
			if (m_pMeshCube[9].texture == NULL)
			{
				m_pMeshCube[9].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_02.psptex", 64, 64);
				m_pMeshCube[9].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_09_add.psptex", 64, 64);
				m_pMeshCube[9].theMesh = m_pWireCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[9];
			m_pInfoCubes[i].iMeshCube = 9;
		}
		else if (m_pCubes[i].tex == 16 && m_pCubes[i].texLat == 15)
		{
			if (m_pMeshCube[10].texture == NULL)
			{
				m_pMeshCube[10].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_02.psptex", 64, 64);
				m_pMeshCube[10].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_11_add.psptex", 64, 64);
				m_pMeshCube[10].theMesh = m_pWireCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[10];
			m_pInfoCubes[i].iMeshCube = 10;
		}
		else if (m_pCubes[i].tex == 13 && m_pCubes[i].texLat == 15)
		{
			if (m_pMeshCube[11].texture == NULL)
			{
				m_pMeshCube[11].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_05.psptex", 64, 64);
				m_pMeshCube[11].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_12_add.psptex", 64, 64);
				m_pMeshCube[11].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[11];
			m_pInfoCubes[i].iMeshCube = 11;
		}
		else if (m_pCubes[i].tex == 5 && m_pCubes[i].texLat == 4)
		{
			if (m_pMeshCube[12].texture == NULL)
			{
				m_pMeshCube[12].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_13.psptex", 64, 64);
				m_pMeshCube[12].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_13_add.psptex", 64, 64);
				m_pMeshCube[12].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[12];
			m_pInfoCubes[i].iMeshCube = 12;
		}
		else
		{
			if (m_pMeshCube[2].texture == NULL)
			{
				m_pMeshCube[2].texture = m_pTextureManager->GetTexture("gfx/cubes/h_cube_03.psptex", 64, 64);
				m_pMeshCube[2].textureAdd = m_pTextureManager->GetTexture("gfx/cubes/h_cube_03_add.psptex", 64, 64);
				m_pMeshCube[2].theMesh = m_pNormalCube;
			}
			m_pInfoCubes[i].pMeshCube = &m_pMeshCube[2];
			m_pInfoCubes[i].iMeshCube = 2;
		}
	}

	Log("CLevel: Datos de nivel leidos corectamente\n");
}


//--------------------------------------------------------------------
// Función:    CLevel::Render2D
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  13/06/2007  12:00:13
//--------------------------------------------------------------------
void CLevel::Render2D(void)
{

	int color = (int)(((cosf(m_fActualTime * 10.0f) + 1.0f) / 2.0f) * 255.0f);

	if (m_iRemainingGems != 0)
	{
		if (!m_bTiltGems)
			color = 255;

		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);		

		sceGuAmbientColor(COLOR_ARGB((int)color, 255, 255, 255));		
		
		sceGuEnable(GU_BLEND);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

		m_pVideoManager->EnableTexture(m_pQuadGemItem->GetTexture());
		
		VECTOR3 scale = { 64.0f, 32.0f, 1.0f };	

		VECTOR3 pos = { 420.0f, 0.0f, 1.0f };

		scale.x = 32.0f;	

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			sceGumTranslate(&pos);
			sceGumScale(&scale);				
		}

		m_pQuadGemItem->Render();

		sceGuDisable(GU_BLEND);

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);	

		sceGuAmbientColor(0xffffffff);

		char gems[5];
		sprintf(gems, "%d", m_iRemainingGems);

		m_pFont->Draw(gems, 455, 9, COLOR_ARGB((int)color, 0, 0, 0));
		m_pFont->Draw(gems, 454, 8, COLOR_ARGB((int)color, 0xac, 0xd7, 0xff));	
	}
	else
	{
		if (!m_bEndLevel)
		{
			m_pFont->Draw("FIND EXIT", 398, 9, COLOR_ARGB((int)color, 0, 0, 0));
			m_pFont->Draw("FIND EXIT", 397, 8, COLOR_ARGB((int)color, 0xac, 0xd7, 0xff));
		}
	}


	if (m_iActualRecord != 1245986)
	{	
		if (m_RecordTimer.GetActualTime() < 7.0f)
		{
			char best_time[32];
			int seconds = m_iActualRecord % 60;
			int minutes = m_iActualRecord / 60;
			
			if (seconds < 10)
			{
				sprintf(best_time, "BEST TIME: %d:0%d", minutes, seconds);	
			}
			else
			{
				sprintf(best_time, "BEST TIME: %d:%d", minutes, seconds);	
			}

			float alpha = 255.0f;

			if (m_RecordTimer.GetActualTime() > 6.0f)
			{
				alpha = (7.0f - m_RecordTimer.GetActualTime()) * 255.0f;
			}

			m_pFont->Draw(best_time, 9, 9, COLOR_ARGB((int)alpha, 0, 0, 0));
			m_pFont->Draw(best_time, 8, 8, COLOR_ARGB((int)alpha, 255, 216, 0));
		}
	}

	
#ifdef DEBUG_PSP_OZONE
	char str[PSP_MAX_PATH];

	sprintf(str, "FPS: %f", m_pMainTimer->GetFPS());

	m_pFont->Draw(str, 0, 0, 0xff00ff00);

	sprintf(str, "X: %f  Y: %f Z: %f", m_pCamera->GetPosition().x,  m_pCamera->GetPosition().y, m_pCamera->GetPosition().z);

	m_pFont->Draw(str, 0, 20, 0xff00ff00);
#endif

}


//--------------------------------------------------------------------
// Función:    CLevel::RenderLevel
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 17:21:39
//--------------------------------------------------------------------
void CLevel::RenderLevel(CFade* pFade)
{
	m_pVideoManager->StartDrawing();	

	m_pVideoManager->Clear(0xffff0000, GU_DEPTH_BUFFER_BIT);	

	m_pCamera->SetMode(true);

	RenderBackground(-40.0f);

	m_pLayerManager->Render(2, m_pCamera);

	RenderSectors();	

	RenderTrans();

	RenderEnd();

	RenderEnemies();

	m_pParticles->Update(m_fDeltaTime, this, CGame::GetSoundPlayer());	

	m_pSmoke->Update(m_fDeltaTime);

	if (!m_bGoingToDie)
		m_pBall->RenderBall(m_iRedAmmo > 0);	

	m_pLayerManager->Render(1, m_pCamera);
	m_pLayerManager->Render(0, m_pCamera);

	if (m_iRemainingGems < 6)
		RenderArrow();	

	m_pCamera->SetMode(false);

	Render2D();

	if (m_bPaused || m_bDead)
	{
		pFade->Update(m_bDead ? m_fDeltaTime : m_PausedTimer.GetDeltaTime());
		
		int color = (int)(((cosf(m_fPausedMenuAlpha * 8.0f) + 1.0f) / 2.0f) * 255.0f);

		if (!m_bGoingToDie)
		{
			m_pFont->Draw("CONTINUE PLAYING", 169, 101, COLOR_ARGB((m_iPausedMenu == 0) ? color : 255, 0, 0, 0));
			m_pFont->Draw("CONTINUE PLAYING", 168, 100, COLOR_ARGB((m_iPausedMenu == 0) ? color : 255, 255, 255, 255));
		}
		m_pFont->Draw("RESTART THE LEVEL", 165, 131 - (m_bGoingToDie ? 15 : 0), COLOR_ARGB((m_iPausedMenu == 1) ? color : 255, 0, 0, 0));
		m_pFont->Draw("RESTART THE LEVEL", 164, 130 - (m_bGoingToDie ? 15 : 0), COLOR_ARGB((m_iPausedMenu == 1) ? color : 255, 255, 255, 255));
		m_pFont->Draw("BACK TO MAIN MENU", 159, 161 - (m_bGoingToDie ? 15 : 0), COLOR_ARGB((m_iPausedMenu == 2) ? color : 255, 0, 0, 0));
		m_pFont->Draw("BACK TO MAIN MENU", 158, 160 - (m_bGoingToDie ? 15 : 0), COLOR_ARGB((m_iPausedMenu == 2) ? color : 255, 255, 255, 255));

		//m_pBall->Update2D(m_PausedTimer.GetDeltaTime());

		m_pBall->RenderBall2D(125.0f - (cosf((m_bDead ? m_fActualTime : m_PausedTimer.GetActualTime()) * 6.0f) * 4.0f), 107.0f + (m_iPausedMenu * 30.0f) - (m_bGoingToDie ? 15 : 0), 15.0f);
		m_pBall->RenderBall2D(355.0f + (cosf((m_bDead ? m_fActualTime : m_PausedTimer.GetActualTime()) * 6.0f) * 4.0f), 107.0f + (m_iPausedMenu * 30.0f) - (m_bGoingToDie ? 15 : 0), 15.0f);

		

		m_fPausedMenuAlpha += m_PausedTimer.GetDeltaTime();

	}
	else
	{
		pFade->Update(m_fDeltaTime);
	}	

	if (m_bEndLevel)
	{	
		int total_seconds = (int)m_RecordTimer.GetActualTime();
		int minutes = total_seconds / 60;
		int seconds = total_seconds % 60;

		if (m_iActualRecord <= total_seconds)
		{
			m_pFont->Draw("YOU HAVE FINISHED!!!", 156, 31, COLOR_ARGB(255, 0, 0, 0));
			m_pFont->Draw("YOU HAVE FINISHED!!!", 155, 30, COLOR_ARGB(255, 255, 216, 0));
		}
		else
		{
			m_pFont->Draw("YOU HAVE A NEW RECORD!!!", 131, 31, COLOR_ARGB(255, 0, 0, 0));
			m_pFont->Draw("YOU HAVE A NEW RECORD!!!", 130, 30, COLOR_ARGB(255, 255, 216, 0));
		}	

		char time[32];

		if (seconds < 10)
		{
			sprintf(time, "YOUR TIME IS %d:0%d", minutes, seconds);
		}
		else
		{
			sprintf(time, "YOUR TIME IS %d:%d", minutes, seconds);
		}
		m_pFont->Draw(time, 166, 51, COLOR_ARGB(255, 0, 0, 0));
		m_pFont->Draw(time, 165, 50, COLOR_ARGB(255, 255, 216, 0));
	}

	m_pVideoManager->EndDrawing();
}


//--------------------------------------------------------------------
// Función:    CLevel::UpdateCamera
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 17:21:37
//--------------------------------------------------------------------
void CLevel::UpdateCamera(void)
{
	VECTOR3 vecBallPos = m_pBall->GetPosition();	
	VECTOR3 vecCamPos = m_pCamera->GetPosition();

	float resx = (-vecBallPos.x) - vecCamPos.x;
	float resy = vecBallPos.y - vecCamPos.y;

	vecCamPos.x += (resx * m_fDeltaTime * 3.0f);
	vecCamPos.y += (resy * m_fDeltaTime * 3.0f);

/*
	float ballVel = MAT_Max(m_pBall->GetVelocity().x, m_pBall->GetVelocity().y);

	if (ballVel <= 1.5f)
	{
		vecCamPos.z += 1.0f * m_pMainTimer->GetDeltaTime();

		if (vecCamPos.z > -40.0f)
			vecCamPos.z = -40.0f;
	}
	else if (ballVel >= 4.0f)
	{
		vecCamPos.z -= 2.0f * m_pMainTimer->GetDeltaTime();

		if (vecCamPos.z < -54.0f)
			vecCamPos.z = -54.0f;
	}
*/
	m_pCamera->SetPosition(vecCamPos);	
}


//--------------------------------------------------------------------
// Función:    CLevel::UpdateLogicEnemies
// Creador:    Nacho (AMD)
// Fecha:      Thursday  15/02/2007  18:53:40
//--------------------------------------------------------------------
void CLevel::UpdateLogicEnemies(void)
{
	VECTOR3 vecBallPos = m_pBall->GetPosition();
	VECTOR2 vecBallVel = m_pBall->GetVelocity();
	float fBallRadius = m_pBall->GetRadius();

	for (int i=0; i< m_iNumEnemies; i++)
	{
		if (m_pEnemies[i].active)
		{
			switch (m_pEnemies[i].type)
			{
				///--- sierra
				case 0:
				{
					if (!m_bGoingToDie)
					{
						VECTOR3 vDis;
						VECTOR3 vTemp;
						vTemp.x = m_pEnemies[i].posX +4.0f;
						vTemp.y = m_pEnemies[i].posY +4.0f;
						vTemp.z = 4.0f;

						MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

						float length = MAT_VectorLength(&vDis);

						if(length > 100.0f)
						{
							continue;
						}
						else if (length < fBallRadius +2.0f)
						{
							m_pEnemies[i].active = false;

							m_pBall->SetRadius(fBallRadius - 0.2f);

							fBallRadius = m_pBall->GetRadius();

							vecBallVel.x += vDis.x * 3.0f;
							vecBallVel.y += vDis.y * 3.0f;

							m_pBall->SetVelocity(vecBallVel);

							m_pSmoke->AddExplosion(vTemp, 20.0f, true);

							m_pSmoke->AddExplosion(vecBallPos, 4.0f);

							CGame::GetSoundPlayer()->Play(SOUND_ENE_EXPLO);

							break;
						}

						vDis.x /= length;
						vDis.y /= length;

						m_pEnemies[i].velX = vDis.x * 10.0f;
						m_pEnemies[i].velY = vDis.y * 10.0f;

						float oldx=m_pEnemies[i].posX;
						float oldy=m_pEnemies[i].posY;

						m_pEnemies[i].posX += m_pEnemies[i].velX * m_fDeltaTime;
						m_pEnemies[i].posY += m_pEnemies[i].velY * m_fDeltaTime;

						stCollisionData data;

						data.radius = 2.0f;
						data.x = m_pEnemies[i].posX + 4.0f;
						data.y = m_pEnemies[i].posY + 4.0f;
						data.velX = m_pEnemies[i].velX;
						data.velY = m_pEnemies[i].velY;

						if (TestCollisionEnemies(&data))
						{								
							m_pEnemies[i].posX = data.x - 4.0f;
							m_pEnemies[i].posY = data.y - 4.0f;
							m_pEnemies[i].velX = data.velX;
							m_pEnemies[i].velY = data.velY;	
						}

						for (int a=0; a< m_iNumEnemies; a++)
						{
							if (m_pEnemies[a].active)
							{
								if (m_pEnemies[a].type==0)
								{
									if (a!=i)
									{
										VECTOR3 vDis2;
										VECTOR3 vTempEnemy = {m_pEnemies[a].posX+4.0f, m_pEnemies[a].posY+4.0f, 4.0f};
										VECTOR3 vTemp2 = {m_pEnemies[i].posX+4.0f, m_pEnemies[i].posY+4.0f, 4.0f};

										MAT_VectorSubtract(&vDis2, &vTempEnemy, &vTemp2);

										float length = MAT_VectorQuadraticLength(&vDis2);

										if (length < 16.0f) 
										{
											m_pEnemies[i].posX = oldx;
											m_pEnemies[i].posY = oldy;
										}
									}
								}
							}
						}												
					}

					break;
				}
				///--- dirigidos
				case 1:
				{
					if (!m_bGoingToDie)
					{
						VECTOR3 vDis = {0};
						VECTOR3 vTemp = {0};

						if (m_pEnemies[i].rot==0)
						{
							vTemp.x = m_pEnemies[i].posX+2.0f;
							vTemp.y = m_pEnemies[i].posY+4.0f;
						}						
						else if (m_pEnemies[i].rot==3)
						{
							vTemp.x = m_pEnemies[i].posX+4.0f;
							vTemp.y = m_pEnemies[i].posY+2.0f;
						}
						else if (m_pEnemies[i].rot==2)
						{
							vTemp.x = m_pEnemies[i].posX+6.0f;
							vTemp.y = m_pEnemies[i].posY+4.0f;
						}
						else if (m_pEnemies[i].rot==1)
						{
							vTemp.x = m_pEnemies[i].posX+4.0f;
							vTemp.y = m_pEnemies[i].posY+6.0f;						
						}	

						vTemp.z = 4.0f;

						MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

						float length = MAT_VectorLength(&vDis);
						if(length > 60.0f)
						{
							continue;
						}
						else if (length < fBallRadius +2.0f)
						{
							m_pEnemies[i].active = false;

							m_pBall->SetRadius(fBallRadius - 0.5f);

							fBallRadius = m_pBall->GetRadius();

							vecBallVel.x += vDis.x * 3.0f;
							vecBallVel.y += vDis.y * 3.0f;

							m_pBall->SetVelocity(vecBallVel);

							m_pSmoke->AddExplosion(vTemp, 30.0f, true);

							m_pSmoke->AddExplosion(vecBallPos, 6.0f);

							CGame::GetSoundPlayer()->Play(SOUND_ENE_EXPLO);

							break;
						}

						float disx = vDis.x;

						vDis.x /= length;
						vDis.y /= length;

						vDis.x *= 7.0f;
						vDis.y *= 7.0f;

						if ((m_fActualTime - m_pEnemies[i].timeShot) >= 1.2f)
						{
							
							m_pEnemies[i].timeShot = m_fActualTime;

							VECTOR2 vel = {vDis.x, vDis.y};
							if (m_pEnemies[i].rot==0)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+2.0f, m_pEnemies[i].posY+4.0f, 4.0f };							
								m_pParticles->AddParticle(pos, vel, 2.0f, 3);
							}
							else if (m_pEnemies[i].rot==3)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+4.0f, m_pEnemies[i].posY +2.0f, 4.0f };							
								m_pParticles->AddParticle(pos, vel, 2.0f, 3);
							}
							else if (m_pEnemies[i].rot==2)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+6.0f, m_pEnemies[i].posY+4.0f, 4.0f };							
								m_pParticles->AddParticle(pos, vel, 2.0f, 3);
							}
							else if (m_pEnemies[i].rot==1)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+4.0f, m_pEnemies[i].posY+6.0f, 4.0f };							
								m_pParticles->AddParticle(pos, vel, 2.0f, 3);
							}					

							int pan = 127;

							if (disx < 0.0f)
							{
								if (disx < -32.0f)
								{
									pan = 255;
								}
								else
								{
									pan = MAT_Clamp(128 + (int)(((-disx) / 32.0f) * 127.0f), 128, 255);									
								}
							}
							else

							{
								if (disx > 32.0f)
								{
									pan = 0;
								}
								else
								{ 
									pan = MAT_Clamp((int)(127.0f - ((disx / 32.0f) * 127.0f)), 0, 127);
								}
							}

							//CGame::GetSoundPlayer()->Play(SOUND_ENE2_FIRE, pan);
						}
					}

					break;
				}
				///--- rectos
				case 2:
				{
					if (!m_bGoingToDie)
					{
						VECTOR3 vDis;
						VECTOR3 vTemp = {0};

						if (m_pEnemies[i].rot==0)
						{
							vTemp.x = m_pEnemies[i].posX+2.0f;
							vTemp.y = m_pEnemies[i].posY+4.0f;
						}						
						else if (m_pEnemies[i].rot==3)
						{
							vTemp.x = m_pEnemies[i].posX+4.0f;
							vTemp.y = m_pEnemies[i].posY+2.0f;
						}
						else if (m_pEnemies[i].rot==2)
						{
							vTemp.x = m_pEnemies[i].posX+6.0f;
							vTemp.y = m_pEnemies[i].posY+4.0f;
						}
						else if (m_pEnemies[i].rot==1)
						{
							vTemp.x = m_pEnemies[i].posX+4.0f;
							vTemp.y = m_pEnemies[i].posY+6.0f;						
						}	

						vTemp.z = 4.0f;
						
						MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

						float len = MAT_VectorQuadraticLength(&vDis);

						float disx = vDis.x;

						if(len > 3600.0f)
						{
							continue;
						}						
						else if (len < ((fBallRadius +2.0f) * (fBallRadius +2.0f)))
						{
							m_pEnemies[i].active = false;

							m_pBall->SetRadius(fBallRadius - 0.5f);

							fBallRadius = m_pBall->GetRadius();

							vecBallVel.x += vDis.x * 3.0f;
							vecBallVel.y += vDis.y * 3.0f;

							m_pBall->SetVelocity(vecBallVel);

							m_pSmoke->AddExplosion(vTemp, 30.0f, true);

							m_pSmoke->AddExplosion(vecBallPos, 6.0f);

							CGame::GetSoundPlayer()->Play(SOUND_ENE_EXPLO);

							break;
						}

						if ((m_fActualTime - m_pEnemies[i].timeShot) >= 0.8f)
						{
							//m_pSonido[SON_ENE3_DISP]->Play(0, PosBola);
							m_pEnemies[i].timeShot = m_fActualTime;

							if (m_pEnemies[i].rot==0)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+2.0f, m_pEnemies[i].posY+4.0f, 4.0f };
								VECTOR2 vel = {10.0f, 0.0f};
								m_pParticles->AddParticle(pos, vel, 2.0f, 4);
							}
							else if (m_pEnemies[i].rot==3)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+4.0f, m_pEnemies[i].posY +2.0f, 4.0f };
								VECTOR2 vel = {0.0f, 10.0f};
								m_pParticles->AddParticle(pos, vel, 2.0f, 4);
							}
							else if (m_pEnemies[i].rot==2)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+6.0f, m_pEnemies[i].posY+4.0f, 4.0f };
								VECTOR2 vel = {-10.0f, 0.0f};
								m_pParticles->AddParticle(pos, vel, 2.0f, 4);
							}
							else if (m_pEnemies[i].rot==1)
							{
								VECTOR3 pos = { m_pEnemies[i].posX+4.0f, m_pEnemies[i].posY+6.0f, 4.0f };
								VECTOR2 vel = {0.0f, -10.0f};
								m_pParticles->AddParticle(pos, vel, 2.0f, 4);
							}

							int pan = 127;

							if (disx < 0.0f)
							{
								if (disx < -32.0f)
								{
									pan = 255;
								}
								else
								{
									pan = MAT_Clamp(128 + (int)(((-disx) / 32.0f) * 127.0f), 128, 255);									
								}
							}
							else
							{
								if (disx > 32.0f)
								{
									pan = 0;
								}
								else
								{ 
									pan = MAT_Clamp((int)(127.0f - ((disx / 32.0f) * 127.0f)), 0, 127);
								}
							}

							CGame::GetSoundPlayer()->Play(SOUND_ENE3_FIRE, pan);
						}
					}
					break;
				}
				///--- rebota
				case 3:
				{
					VECTOR3 vDis;
					VECTOR3 vTemp = {m_pEnemies[i].posX+4.0f, m_pEnemies[i].posY+4.0f, 4.0f};
					
					MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

					float disx = vDis.x;

					float len = MAT_VectorQuadraticLength(&vDis);

					if(len > 3600.0f)
					{
						continue;
					}
					else if ((!m_bGoingToDie) && (len < ((fBallRadius +2.0f) * (fBallRadius +2.0f))))
					{
						m_pEnemies[i].active = false;

						m_pBall->SetRadius(fBallRadius - 0.5f);

						fBallRadius = m_pBall->GetRadius();

						vecBallVel.x += vDis.x * 3.0f;
						vecBallVel.y += vDis.y * 3.0f;

						m_pBall->SetVelocity(vecBallVel);

						m_pSmoke->AddExplosion(vTemp, 30.0f, true);

						m_pSmoke->AddExplosion(vecBallPos, 6.0f);

						CGame::GetSoundPlayer()->Play(SOUND_ENE_EXPLO);

						break;
					}

					m_pEnemies[i].posX += m_pEnemies[i].velX * m_fDeltaTime;
					m_pEnemies[i].posY += m_pEnemies[i].velY * m_fDeltaTime;	
				
					stCollisionData data;

					data.radius = 2.2f;
					data.x = m_pEnemies[i].posX + 4.0f;
					data.y = m_pEnemies[i].posY + 4.0f;
					data.velX = m_pEnemies[i].velX;
					data.velY = m_pEnemies[i].velY;

					if (TestSimpleCollision(&data))
					{		
						m_pEnemies[i].posX = data.x-4.0f;
						m_pEnemies[i].posY = data.y-4.0f;
						m_pEnemies[i].velX = data.velX;
						m_pEnemies[i].velY = data.velY;

						int pan = 127;

						if (disx < 0.0f)
						{
							if (disx < -32.0f)
							{
								pan = 255;
							}
							else
							{
								pan = MAT_Clamp(128 + (int)(((-disx) / 32.0f) * 127.0f), 128, 255);									
							}
						}
						else

						{
							if (disx > 32.0f)
							{
								pan = 0;
							}
							else
							{ 
								pan = MAT_Clamp((int)(127.0f - ((disx / 32.0f) * 127.0f)), 0, 127);
							}
						}

						CGame::GetSoundPlayer()->Play(SOUND_ENE4_BOUNCE, pan);
					}	
					
					break;
				}
			}
		}
	}

}


//--------------------------------------------------------------------
// Función:    CLevel::UpdateLogic
// Creador:    Nacho (AMD)
// Fecha:      sábado, 03 de febrero de 2007, 17:50:42
//--------------------------------------------------------------------
void CLevel::UpdateLogic(CFade* pFade)
{
	VECTOR3 vecBallPos = m_pBall->GetPosition();
	VECTOR2 vecBallVel = m_pBall->GetVelocity();
	float fBallRadius = m_pBall->GetRadius();

	/////////////////////
	/// AIRE

	if (!m_bGoingToDie)
	{
		if ((fBallRadius < 0.5f)||(fBallRadius > 3.0f))
		{
			
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 60.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 40.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 20.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 10.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 8.0f);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 15.0f);
			
			m_bGoingToDie = true;

			m_DeadTimer.Start();

			CGame::GetSoundPlayer()->Play(SOUND_EXPLO);
		}
	}

	if (m_bGoingToDie && !m_bDead && (m_DeadTimer.GetActualTime() > 2.0f)) 
	{
		m_iPausedMenu = 1;
		m_fPausedMenuAlpha = 0.0f;
		pFade->StartFade(0xff000000, false, 0.6f, 150.0f);		
		m_bDead = true;
	}	

	/////////////////////
	/// FINAL

	if ((vecBallPos.x > m_theLevel.EndX)
		&& (vecBallPos.x < (m_theLevel.EndX + 8.0f))
		&& (vecBallPos.y > m_theLevel.EndY)
		&& (vecBallPos.y < (m_theLevel.EndY + 8.0f))		
		&& (!m_bEndLevel))
	{
		if (m_iRemainingGems == 0)
		{
			m_RecordTimer.Stop();

			m_bEndLevel = true;
			m_bGoingToDie = true;

			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 60.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 40.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 20.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 10.0f, true);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 8.0f);
			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 15.0f);

			CGame::GetSoundPlayer()->Play(SOUND_LEVEL_END);

			int seconds = (int)m_RecordTimer.GetActualTime();

			if (seconds < m_iActualRecord)
			{
				m_iFinishedRecord = seconds;
				StartSavingRecords(seconds);
			}
			else
			{
				m_iFinishedRecord = m_iActualRecord;
			}

			m_DeadTimer.Start();

			m_bTiltGems = false;
		}
		else
		{
			m_bTiltGems = true;
		}
	}
	else
		m_bTiltGems = false;

	/////////////////////
	/// VELOCIDAD

	if (vecBallVel.x > MAX_VELOCITY)
	{
		vecBallVel.x = MAX_VELOCITY;
		m_pBall->SetVelocity(vecBallVel);
	}
	else if (vecBallVel.x < -MAX_VELOCITY)
	{
		vecBallVel.x = -MAX_VELOCITY;
		m_pBall->SetVelocity(vecBallVel);
	}

	if (vecBallVel.y > MAX_VELOCITY)
	{
		vecBallVel.y = MAX_VELOCITY;
		m_pBall->SetVelocity(vecBallVel);
	}
	else if (vecBallVel.y < -MAX_VELOCITY)
	{
		vecBallVel.y = -MAX_VELOCITY;
		m_pBall->SetVelocity(vecBallVel);
	}

	/////////////////////
	/// ENEMIGOS

	UpdateLogicEnemies();

	/////////////////////
	/// PARPADEO CUBOS

	for (int i=0; i<m_iNumCubes; i++)
	{
		if (m_pInfoCubes[i].timeRemaining > 0.0f)
		{
			m_pInfoCubes[i].timeRemaining -= m_fDeltaTime;

			if (m_pInfoCubes[i].timeRemaining < 0.0f)
				m_pInfoCubes[i].timeRemaining = 0.0f;
		}
	}

	/////////////////////
	/// ROTACIONES
	
	m_fAngGems += (-20.0f * m_fDeltaTime);
	m_fAngGems = MAT_NormalizarAngulo360(m_fAngGems);

	m_fAngTrans += (-150.0f * m_fDeltaTime);
	m_fAngTrans = MAT_NormalizarAngulo360(m_fAngTrans);
	
	m_fAngTrans2 += (-60.0f * m_fDeltaTime);
	m_fAngTrans2 = MAT_NormalizarAngulo360(m_fAngTrans2);

	m_fAngU += (270.0f * m_fDeltaTime);
	m_fAngU = MAT_NormalizarAngulo360(m_fAngU);

	m_fOffsetArrow -= 1.7f * m_fDeltaTime;
	m_fAngBlow -= (m_fFanSpeed * m_fDeltaTime);

	m_pLayerManager->Frame(m_fDeltaTime, float(m_theLevel.NumSecX*32), float(m_theLevel.NumSecY*32));

	
	/////////////////////
	/// TRANSPORTADORES

	if (!m_bGoingToDie)
	{		
		for (int i=0; i< m_iNumTrans; i++)
		{
			if ((vecBallPos.x > m_pTrans[i].posX)
				&& (vecBallPos.x < (m_pTrans[i].posX + 8.0f))
				&& (vecBallPos.y > m_pTrans[i].posY)
				&& (vecBallPos.y < (m_pTrans[i].posY + 8.0f)))
			{
				if ((m_fActualTime-m_fTimerTrans) > 3.0f)
				{
					//m_pSonido[SON_NIVEL_TRANS]->Play(0, PosBola);
					
					int idactual = m_pTrans[i].id;
					
					if (i>0)
					{
						if (m_pTrans[i-1].id == idactual)
						{
							//TELETRANSPORTA i-1
							VECTOR3 v = { m_pTrans[i-1].posX+4.0f, m_pTrans[i-1].posY+4.0f, 4.0f };
							m_pBall->SetPosition(v);
						}
						else
						{
							//TELETRANSPORTA i+1							
							VECTOR3 v = { m_pTrans[i+1].posX+4.0f, m_pTrans[i+1].posY+4.0f, 4.0f };
							m_pBall->SetPosition(v);
						}
					}
					else if (i < (m_iNumTrans-1))
					{
						if (m_pTrans[i+1].id == idactual)
						{
							//TELETRANSPORTA i+1							
							VECTOR3 v = { m_pTrans[i+1].posX+4.0f, m_pTrans[i+1].posY+4.0f, 4.0f };
							m_pBall->SetPosition(v);
						}
						else
						{
							//TELETRANSPORTA i-1							
							VECTOR3 v = { m_pTrans[i-1].posX+4.0f, m_pTrans[i-1].posY+4.0f, 4.0f };
							m_pBall->SetPosition(v);
						}
					}

					CGame::GetSoundPlayer()->Play(SOUND_TRANS);
					
					m_pSmoke->AddExplosion(m_pBall->GetPosition(), 8.0f);

					//m_pSistema->PonerExplosionPeq(m_pBola->DaPos());
					m_fTimerTrans = m_fActualTime;
				}
			}
		}
	}
	/////////////////////
	/// OBJETOS

	int imp=0;
	int pump=0;

	for (int i =0; i<m_iNumObjs; i++)
	{
		if (!m_pObjects[i].active)
			continue;

		static bool flip=true;

		if (m_pObjects[i].type==0) //GEMA AZUL
		{
			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pObjects[i].posX + 4.0f, m_pObjects[i].posY + 4.0f, 4.0f};
			
			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > ((1.0f + fBallRadius) * (1.0f + fBallRadius)))
			{
				continue;
			}

			m_pObjects[i].active = false;

			vTemp.z = 4.0f;

			m_pSmoke->AddExplosion(vTemp, 4.0f);

			m_iRemainingGems--;

			if (flip)
			{				
				CGame::GetSoundPlayer()->Play(SOUND_GEM_BLUE);
			}
			else
			{
				CGame::GetSoundPlayer()->Play(SOUND_GEM_BLUE2);
			}

			flip = !flip;

			continue;
		}
		else if (m_pObjects[i].type==1) //GEMA ROJA
		{
			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pObjects[i].posX + 4.0f, m_pObjects[i].posY + 4.0f, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > ((1.0f + fBallRadius) * (1.0f + fBallRadius)))
			{
				continue;
			}

			m_pObjects[i].active = false;

			vTemp.z = 4.0f;

			m_pSmoke->AddExplosion(vTemp, 4.0f);

			m_iRedAmmo = 20;

			CGame::GetSoundPlayer()->Play(SOUND_GEM_RED);

			continue;
		}
		else if (m_pObjects[i].type==2) //GEMA AMARILLA
		{
			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pObjects[i].posX + 4.0f, m_pObjects[i].posY + 4.0f, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > ((1.0f + fBallRadius) * (1.0f + fBallRadius)))
			{
				continue;
			}

			m_pObjects[i].active = false;

			vTemp.z = 4.0f;

			m_pSmoke->AddExplosion(vTemp, 4.0f);

			m_iYellowAmmo = 70;

			CGame::GetSoundPlayer()->Play(SOUND_GEM_YELLOW);

			continue;
		}
		else if (m_pObjects[i].type==3) //GEMA VERDE
		{
			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pObjects[i].posX + 4.0f, m_pObjects[i].posY + 4.0f, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > ((1.0f + fBallRadius) * (1.0f + fBallRadius)))
			{
				continue;
			}

			m_pSmoke->AddExplosion(m_pBall->GetPosition(), 10.0f);

			vTemp.z = 4.0f;

			m_pSmoke->AddExplosion(vTemp, 4.0f);

			if (m_pBall->GetRadius() < 2.5f)
			{
				m_pBall->SetRadius(2.5f);
				fBallRadius = 2.5f;

				stCollisionData data;

				data.radius = fBallRadius;
				data.x = vecBallPos.x;
				data.y = vecBallPos.y;
				data.velX = vecBallVel.x;
				data.velY = vecBallVel.y;

				if (TestCollisionInflate(&data))
				{		
					vecBallPos.x = data.x;
					vecBallPos.y = data.y;

					m_pBall->SetPosition(vecBallPos);					
				}	
			}

			m_pObjects[i].active = false;	

			CGame::GetSoundPlayer()->Play(SOUND_GEM_GREEN);

			continue;
		}
		else if (m_pObjects[i].type==4 && !m_bGoingToDie) //PINCHO
		{
			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pObjects[i].posX + 1.0f, m_pObjects[i].posY + 1.0f, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > ((1.0f + fBallRadius) * (1.0f + fBallRadius)))
			{
				continue;
			}

			fBallRadius = m_pBall->GetRadius() - (2.0f * m_fDeltaTime);

			m_pBall->SetRadius(fBallRadius);

	
			if (m_SpikeTimer.GetActualTime() > 0.1f)
			{					
				m_SpikeTimer.Start();

				m_pSmoke->AddExplosion(m_pBall->GetPosition(), 8.0f); 					
			}
				
			continue;

		}
		else if (m_pObjects[i].type==9 && !m_bGoingToDie) //INFLADOR
		{
			if((vecBallPos.x>m_pObjects[i].posX)&&(vecBallPos.x<m_pObjects[i].posX+8.0f)&&(vecBallPos.y>m_pObjects[i].posY)&&(vecBallPos.y<m_pObjects[i].posY+8.0f))
			{
				pump++;
				
				m_pBall->Inflate(m_fDeltaTime * 10.0f);

				fBallRadius = m_pBall->GetRadius();

				stCollisionData data;

				data.radius = fBallRadius;
				data.x = vecBallPos.x;
				data.y = vecBallPos.y;
				data.velX = vecBallVel.x;
				data.velY = vecBallVel.y;

				if (TestCollisionInflate(&data))
				{		
					vecBallPos.x = data.x;
					vecBallPos.y = data.y;
					
					m_pBall->SetPosition(vecBallPos);					
				}	

				continue;
			}
		}
		else if (m_pObjects[i].type==7) //LANZADOR DE PIEDRAS
		{

			VECTOR3 vDis;

			VECTOR3 vTemp = {m_pObjects[i].posX + 1.0f, m_pObjects[i].posY + 1.0f, 4.0f};
			
			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > 3600.0f)
			{
				continue;
			}

			if ((m_fActualTime - m_pObjects[i].timeShot) >= 0.1f)
			{
				m_pObjects[i].timeShot = m_fActualTime;

				float velx = 0.0f;
				float vely = 0.0f;
				float ang = 0;					

				if (m_pObjects[i].rot==0)
				{
					ang = MAT_RandomInt(-40, 40);						
				}
				else if (m_pObjects[i].rot==1)
				{
					ang = MAT_RandomInt(230, 310);							
				}
				else if (m_pObjects[i].rot==2)
				{
					ang = MAT_RandomInt(140, 220);							
				}
				else if (m_pObjects[i].rot==3)
				{
					ang = MAT_RandomInt(50, 130);
				}

				ang = MAT_ToRadians(ang);

				velx = cosf(ang) * 20.0f;
				vely = sinf(ang) * 20.0f;

				VECTOR2 vel = {velx, vely};
				VECTOR3 pos = {vTemp.x, vTemp.y, 4.0f };							
				m_pParticles->AddParticle(pos, vel, 0.8f, 2);
			}

			continue;
		}
		else if (m_pObjects[i].type==8) //IMPULSOR
		{
			if((vecBallPos.x>m_pObjects[i].posX)&&(vecBallPos.x<m_pObjects[i].posX+8.0f)&&(vecBallPos.y>m_pObjects[i].posY)&&(vecBallPos.y<m_pObjects[i].posY+8.0f))
			{
				imp++;

				if (m_pObjects[i].rot==0)
				{
					VECTOR2 newVel = {vecBallVel.x + (60.0f * m_fDeltaTime), vecBallVel.y};
					m_pBall->SetVelocity(newVel);
				}
				else if (m_pObjects[i].rot==3)
				{
					VECTOR2 newVel = {vecBallVel.x, vecBallVel.y + (60.0f * m_fDeltaTime)};
					m_pBall->SetVelocity(newVel);
				}
				else if (m_pObjects[i].rot==2)
				{
					VECTOR2 newVel = {vecBallVel.x - (60.0f * m_fDeltaTime), vecBallVel.y};
					m_pBall->SetVelocity(newVel);
				}
				else if (m_pObjects[i].rot==1)
				{
					VECTOR2 newVel = {vecBallVel.x, vecBallVel.y - (60.0f * m_fDeltaTime)};
					m_pBall->SetVelocity(newVel);	
				}
			}

			continue;
		}
		else if (m_pObjects[i].type==5) //LLAVE ROJA
		{	
			VECTOR3 vDis;

			VECTOR3 vTemp = {m_pObjects[i].posX + 1.0f, m_pObjects[i].posY + 1.0f, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > ((0.8f + fBallRadius) * (0.8f + fBallRadius)))
			{
				
				continue;
			}

			m_pObjects[i].active = false;
			m_bHasRedKey = true;

			CGame::GetSoundPlayer()->Play(SOUND_DOORS);

			continue;			
		}
		else if (m_pObjects[i].type==6) //LLAVE ROJA
		{
			VECTOR3 vDis;

			VECTOR3 vTemp = {m_pObjects[i].posX + 1.0f, m_pObjects[i].posY + 1.0f, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if(MAT_VectorQuadraticLength(&vDis) > ((0.8f + fBallRadius) * (0.8f + fBallRadius)))
			{

				continue;
			}

			m_pObjects[i].active = false;
			m_bHasBlueKey = true;

			CGame::GetSoundPlayer()->Play(SOUND_DOORS);

			continue;
		}			
	}

	if (pump > 0)
	{
		if (m_fFanSpeed < 600.0f)
		{
			m_fFanSpeed += 350.0f * m_fDeltaTime;
		}
		else
		{
			m_fFanSpeed = 600.0f;
		}
	}
	else
	{
		if (m_fFanSpeed > 250.0f)
		{
			m_fFanSpeed -= 80.0f * m_fDeltaTime;
		}
		else
		{
			m_fFanSpeed = 250.0f;
		}
	}


	if (pump>0 && !m_bSoundPump)
	{
		m_iVoiceSoundPump = CGame::GetSoundPlayer()->Play(SOUND_LEVEL_AIRPUMP, 127, true);		
		m_bSoundPump=true;
	}

	if (pump==0 && m_bSoundPump)
	{
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundPump);
		m_bSoundPump=false;
	}

	if (imp>0 && !m_bSoundImp)
	{
		m_iVoiceSoundImp = CGame::GetSoundPlayer()->Play(SOUND_LEVEL_IMP, 127, true);		
		m_bSoundImp=true;
	}

	if (imp==0 && m_bSoundImp)
	{
		CGame::GetSoundPlayer()->Stop(m_iVoiceSoundImp);
		m_bSoundImp=false;
	}
}

/********************************************************************/
/********************************************************************/
/*							End Level.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////




