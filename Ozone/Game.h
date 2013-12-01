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
/*								Game.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "Singleton.h"
#include "Video.h"
#include "Timer.h"
#include "Input.h"
#include "Level.h"
#include "Logo.h"
#include "Quad.h"
#include "Fade.h"
#include "Sound.h"

class CGame : public CSingleton <CGame>
{	
	public:
		CGame(void);
		~CGame(void);
		void (CGame::*CurrentStateFunction)(void);		
		inline void Go(void) { (*this.*CurrentStateFunction)(); };
		void Init(void);
		void End(void);
		static inline CTimer* GetMainTimer(void) { return ms_pMainTimer; };
		static inline CSound* GetSoundPlayer(void) { return m_pSoundPlayer; };
		static inline void UpdateRecords(int level, int record) { m_iRecords[level]=record; SaveRecords(); };
		
	private:
		enum eSTATES
		{
			STATE_LOGO,
			STATE_SPLASH,
			STATE_MENU,			
			STATE_GAME
		};

	private:
		void DoStateLogo(void);
		void DoStateSplash(void);
		void DoStateMenu(void);
		void DoStateGame(void);
		void SetState(eSTATES theNewState);	
		void InitSelect(void);
		void EndSelect(void);
		void InitSplash(void);
		void EndSplash(void);
		static void ReadRecords(void);
		static void SaveRecords(void);

	private:
		static CTimer* ms_pMainTimer;
		CVideo* m_pVideoManager;
		CLogo* m_pLogo;
		CInput* m_pInput;
		CLevel* m_pLevel;
		CTextFont* m_pFont;

		CQuad* m_pQuadSelectBack;
		CQuad* m_pQuadSelectHighlightSmall;
		CQuad* m_pQuadSelectHighlightBig;
		CQuad* m_pQuadSelectNumber[8];
		CQuad* m_pQuadSelectBackButton;

		CQuad* m_pQuadSplashCreditsBack;
		CQuad* m_pQuadSplashCreditsRight;
		CQuad* m_pQuadSplashCreditsLeft;
		CQuad* m_pQuadSplashBack;
		CQuad* m_pQuadSplashControls;
		CQuad* m_pQuadSplashCredits;
		CQuad* m_pQuadSplashStart;
		CQuad* m_pQuadSplashOzone;
		CQuad* m_pQuadSplashHighlight;
		CQuad* m_pQuadSplashControlsBack;

		CTimer* m_pAuxTimer;

		CCamera* m_pCamera;

		int m_iCurrentSelection;

		int m_iCurrentState;

		float m_fCreditsOffset;

		CFade m_Fade;

		bool m_bFinishingState;

		static CSound* m_pSoundPlayer;

		int m_iCurrentMusic;

		static int m_iRecords[8];
};

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Game.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////


