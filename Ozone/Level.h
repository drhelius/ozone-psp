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
/*						    Level.h									*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "Video.h"
#include "Timer.h"
#include "Input.h"
#include "Camera.h"
#include "Mesh.h"
#include "TextFont.h"
#include "TextureManager.h"
#include "Ball.h"
#include "Quad.h"
#include "Bitset.h"
#include "Smoke.h"
#include "LayerManager.h"
#include "Particles.h"
#include "Fade.h"

class CLevel
{
	public:
		CLevel(void);	
		~CLevel(void);		

		void Init(int iLevel, int record);
		void InitVariables(void);
		void End(void);
		void Update(CFade* pFade);
		bool TestCollision(stCollisionData* pData, bool bIsTheBall);
		bool TestCollisionEnemies(stCollisionData* pData);
		bool TestSimpleCollision(stCollisionData* pData);
		bool TestCollisionInflate(stCollisionData* pData);
		CBall* GetBall(void) { return m_pBall; };
		CSmoke* GetSmoke(void) { return m_pSmoke; };
		bool IsBallDead(void) { return m_bGoingToDie; };

		int GetNumEnemies(void) { return m_iNumEnemies; };
		ENEMY* GetEnemies(void) { return m_pEnemies; };

		bool FinishRequested(void) { return m_bFinishRequest; };

	private:
		struct stMeshCube
		{
			CMesh *theMesh;
			TEXTURE* texture;
			TEXTURE* textureAdd;
		};

		struct stInfoCube
		{
			float timeRemaining;
			stMeshCube* pMeshCube;
			int iMeshCube;
		};				

		CParticles* m_pParticles;
		CTimer* m_pMainTimer;
		CVideo* m_pVideoManager;
		CInput* m_pInput;
		CCamera* m_pCamera;
		CTextFont* m_pFont;
		CTextureManager* m_pTextureManager;
		CQuad* m_pQuadBackground;
		CQuad* m_pQuadObjs;

		CUBE *m_pCubes;
		SECTOR *m_pSectors;
		ENEMY *m_pEnemies;
		int m_iNumCubes;
		int m_iNumSectors;
		int m_iNumTrans;
		int m_iNumEnemies;

		LEVEL m_theLevel;

		stMeshCube m_pMeshCube[13];

		stInfoCube *m_pInfoCubes;

		CMesh* m_pNormalCube;
		CMesh* m_pWireCube;
		CMesh* m_pDoorCube;
		
		CMesh* m_pObjMeshes[10];

		CMesh* m_pArrow;

		CBall* m_pBall;

		CLayerManager* m_pLayerManager;

		CBitset m_RenderedCubes;
		CBitset m_RenderedObjs;

		CSmoke* m_pSmoke;

		OBJECT* m_pObjects;
		TRANS* m_pTrans;

		CQuad* m_pQuadGemItem;

		TEXTURE* m_pTransTexture;
		TEXTURE* m_pEndTexture;

		TEXTURE* m_pEnemiesTexture[4];
		CMesh* m_pEnemiesMeshes[4];

		int	m_iNumObjs;		

		TEXTURE* m_pObjTextures[10];

		typedef std::list<int>::iterator INT_LIST_ITER;

		float m_fDeltaTime;
		float m_fActualTime;

		float m_fLastSmokeUpdate;

		float m_fAngTrans;	
		float m_fAngTrans2;
		float m_fAngGems;
		float m_fAngBlow;
		float m_fAngU;
		float m_fOffsetArrow;
		float m_fTimerTrans;

		int m_iRemainingGems;

		int m_iRedAmmo;
		int m_iYellowAmmo;

		int m_iLevel;

		bool m_bDead;

		CTimer m_SpikeTimer;
		CTimer m_PausedTimer;

		CTimer m_DeadTimer;

		bool m_bPaused;

		bool m_bHasRedKey;
		bool m_bHasBlueKey;

		int m_iPausedMenu;
		float m_fPausedMenuAlpha;

		bool m_bFinishRequest;
		bool m_bFinished;

		bool m_bEndLevel;
		bool m_bGoingToDie;

		bool m_bTiltGems;

		bool m_bSoundAir;
		bool m_bSoundPump;
		bool m_bSoundImp;

		int m_iVoiceSoundAir;
		int m_iVoiceSoundPump;
		int m_iVoiceSoundImp;

		int m_iBounceSound;

		float m_fLastYellowFire;
		float m_fLastBounceSound;
		float m_fLastDeflateSmoke;

		int m_iActualRecord;
		int m_iFinishedRecord;

		float m_fFanSpeed;

		CTimer m_RecordTimer;

	
	private:
		void UpdateInput(CFade* pFade);
		void RenderSectors(void);
		void RenderBackgroundQuad(int cx, int cy, float fZ);
		void RenderBackground(float fZ);
		void RenderObject(int object);
		void RenderTrans(void);
		void RenderEnd(void);
		void RenderEnemies(void);
		void Render2D(void);
		void RenderArrow(void);

		void LoadLevel(int iLevel);
		void RenderLevel(CFade* pFade);
		void UpdateCamera(void);
		void UpdateLogic(CFade* pFade);
		void UpdateLogicEnemies(void);
		void StartSavingRecords(int seconds);
};

/********************************************************************/
/********************************************************************/
/*							End Level.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
