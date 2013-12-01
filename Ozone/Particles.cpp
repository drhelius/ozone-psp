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
/*						    Particles.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Level.h"


//--------------------------------------------------------------------
// Función:    CParticles::CParticles
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  14/02/2007  19:34:00
//--------------------------------------------------------------------
CParticles::CParticles(void)
{
	for (int i=0; i<5; i++)
	{	
		InitPointer(m_pParticleArray[i]);
	}

	for (int i=0; i<4; i++)
	{	
		InitPointer(m_pTexture[i]);
	}
}


//--------------------------------------------------------------------
// Función:    CParticles::~CParticles
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  14/02/2007  19:34:02
//--------------------------------------------------------------------
CParticles::~CParticles(void)
{
	Log("CParticles: destructor\n");

	for (int i=0; i<5; i++)
	{
		SafeDeleteArray(m_pParticleArray[i]);	
	}

	for (int i=0; i<4; i++)
	{
		CTextureManager::GetSingleton()->ReleaseTexture(m_pTexture[i]);
	}

	SafeDelete(m_pQuad);
	SafeDelete(m_Type4);
}


//--------------------------------------------------------------------
// Función:    CParticles::Init
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  14/02/2007  19:37:34
//--------------------------------------------------------------------
void CParticles::Init(void)
{
	m_fAngType3 = 0.0f;

	for (int i=0; i<5; i++)
	{
		int max = MAX_PARTICLES;

		///--- piedras
		if (i == 2)
			max = MAX_PARTICLES_TYPE2;

		m_pParticleArray[i] = new stParticleNode[max];

		for (int j=0; j<max; j++)
		{		
			m_pParticleArray[i][j].enable = false;
			m_pParticleArray[i][j].next = j+1;
		}

		m_pParticleArray[i][max - 1].next = -1;

		m_iFreeSlot[i] = 0;
	}

	m_pTexture[0] = CTextureManager::GetSingleton()->GetTexture("gfx/models/disp1.psptex", 16, 16);
	m_pTexture[1] = CTextureManager::GetSingleton()->GetTexture("gfx/models/disp2.psptex", 16, 16);
	m_pTexture[2] = CTextureManager::GetSingleton()->GetTexture("gfx/models/h_stones.psptex", 16, 16);
	m_pTexture[3] = CTextureManager::GetSingleton()->GetTexture("gfx/models/disp3.psptex", 16, 16);

	m_pQuad = new CQuad(false);
	m_pQuad->Init(NULL, 0, 0);

	m_Type4 = new CMesh();
	m_Type4->LoadMesh("gfx/models/disp4.o3d", "gfx/models/ene3.psptex", 64, 64);
}


//--------------------------------------------------------------------
// Función:    CParticles::AddParticle
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  14/02/2007  19:38:36
//--------------------------------------------------------------------
void CParticles::AddParticle(VECTOR3 pos, VECTOR2 vel, float radius, int type)
{
	if (m_iFreeSlot[type] < 0)
	{
		return;
	}

	m_pParticleArray[type][m_iFreeSlot[type]].pos = pos;
	m_pParticleArray[type][m_iFreeSlot[type]].vel = vel;
	m_pParticleArray[type][m_iFreeSlot[type]].radius = radius;
	m_pParticleArray[type][m_iFreeSlot[type]].enable = true;
	m_pParticleArray[type][m_iFreeSlot[type]].timeStamp = 0.0f;

	m_iFreeSlot[type] = m_pParticleArray[type][m_iFreeSlot[type]].next;  
}


//--------------------------------------------------------------------
// Función:    CParticles::Update
// Creador:    Nacho (AMD)
// Fecha:      Wednesday  14/02/2007  19:48:41
//--------------------------------------------------------------------
void CParticles::Update(float dt, CLevel* pTheLevel, CSound* pSoundPlayer)
{	
	CVideo * pVideo = CVideo::GetSingleton();

	m_fAngType3 += (240.0f * dt);
	m_fAngType3 = MAT_NormalizarAngulo360(m_fAngType3);

	CBall* pBall = pTheLevel->GetBall();
	CSmoke* pSmoke = pTheLevel->GetSmoke();

	VECTOR3 vecBallPos = pBall->GetPosition();
	VECTOR2 vecBallVel = pBall->GetVelocity();
	float fBallRadius = pBall->GetRadius();

	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);

	ENEMY* pEnemies = pTheLevel->GetEnemies();
	int iNumEnemies = pTheLevel->GetNumEnemies();
	
	///--- TIPO 0: Disparo amarillo
	//////////////////////////////////////////////////////////////////////

	pVideo->EnableTexture(m_pTexture[0]);

	for (int i=0; i<MAX_PARTICLES; i++)
	{
		if (m_pParticleArray[0][i].enable)
		{			
			m_pParticleArray[0][i].timeStamp += dt;

			if (m_pParticleArray[0][i].timeStamp >= 5.0f)
			{
				m_pParticleArray[0][i].enable = false;
				m_pParticleArray[0][i].next = m_iFreeSlot[0];
				m_iFreeSlot[0] = i;

				continue;
			}

			m_pParticleArray[0][i].pos.x += m_pParticleArray[0][i].vel.x * dt;
			m_pParticleArray[0][i].pos.y += m_pParticleArray[0][i].vel.y * dt;


			stCollisionData data;

			data.radius = 1.0f;
			data.x = m_pParticleArray[0][i].pos.x;
			data.y = m_pParticleArray[0][i].pos.y;
			data.velX = m_pParticleArray[0][i].vel.x;
			data.velY = m_pParticleArray[0][i].vel.y;

			if (pTheLevel->TestCollision(&data, false))
			{		
				m_pParticleArray[0][i].pos.x = data.x;
				m_pParticleArray[0][i].pos.y = data.y;
				m_pParticleArray[0][i].vel.x = data.velX;
				m_pParticleArray[0][i].vel.y = data.velY;
			}

			for (int a=0; a<iNumEnemies; a++)
			{
				if (pEnemies[a].active)
				{					
					///--- sierra
					///--- rebota
					if ((pEnemies[a].type == 0) || (pEnemies[a].type == 3))
					{
						VECTOR3 vDis;
						VECTOR3 vTemp = {m_pParticleArray[0][i].pos.x, m_pParticleArray[0][i].pos.y, 4.0f};
						VECTOR3 vTemp2 = {pEnemies[a].posX + 4.0f, pEnemies[a].posY + 4.0f, 4.0f};

						MAT_VectorSubtract(&vDis, &vTemp2, &vTemp);

						float length = MAT_VectorQuadraticLength(&vDis);

						if (length < 6.25f)
						{
							m_pParticleArray[0][i].enable = false;
							m_pParticleArray[0][i].next = m_iFreeSlot[0];
							m_iFreeSlot[0] = i;

							pSmoke->AddExplosion(vTemp2, 20.0f, true);

							pEnemies[a].active = false;

							pSoundPlayer->Play(SOUND_ENE_EXPLO);

							break;
						}
					}
					///--- dirigidos
					///--- rectos
					else if ((pEnemies[a].type == 1) || (pEnemies[a].type == 2))
					{	
						VECTOR3 vTemp = {0};

						if (pEnemies[a].rot==0)
						{
							vTemp.x = pEnemies[a].posX+2.0f;
							vTemp.y = pEnemies[a].posY+4.0f;
						}						
						else if (pEnemies[a].rot==3)
						{
							vTemp.x = pEnemies[a].posX+4.0f;
							vTemp.y = pEnemies[a].posY+2.0f;
						}
						else if (pEnemies[a].rot==2)
						{
							vTemp.x = pEnemies[a].posX+6.0f;
							vTemp.y = pEnemies[a].posY+4.0f;
						}
						else if (pEnemies[a].rot==1)
						{
							vTemp.x = pEnemies[a].posX+4.0f;
							vTemp.y = pEnemies[a].posY+6.0f;						
						}	

						vTemp.z = 4.0f;

						VECTOR3 vDis;
						VECTOR3 vTemp2 = {m_pParticleArray[0][i].pos.x, m_pParticleArray[0][i].pos.y, 4.0f};
						
						MAT_VectorSubtract(&vDis, &vTemp, &vTemp2);

						float disx = vecBallPos.x - m_pParticleArray[0][i].pos.x,;

						float length = MAT_VectorQuadraticLength(&vDis);

						if (length < 6.0f)
						{
							m_pParticleArray[0][i].enable = false;
							m_pParticleArray[0][i].next = m_iFreeSlot[0];
							m_iFreeSlot[0] = i;

							pSmoke->AddExplosion(vTemp, 20.0f, true);

							pEnemies[a].active = false;

							pSoundPlayer->Play(SOUND_ENE_EXPLO);

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

							pSoundPlayer->Play(SOUND_ENE_EXPLO, pan);

							break;
						}						
					}				
				}
			}

			if (!m_pParticleArray[0][i].enable)
				continue;

			///--- render

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos =  { m_pParticleArray[0][i].pos.x -1.0f,
					-(m_pParticleArray[0][i].pos.y + 1.0f), 4.0f };
				VECTOR3 scale = { 2.0f, 2.0f, 2.0f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);			
			}			

			if (m_pParticleArray[0][i].timeStamp >= 4.0f)
			{
				int alpha = (int)MAT_Clampf((5.0f - m_pParticleArray[0][i].timeStamp) * 255.0f, 0.0f, 255.0f);

				sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

				sceGuAmbientColor(COLOR_ARGB(alpha, 255, 255, 255));

				m_pQuad->Render();

				sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, COLOR_ARGB(alpha, alpha, alpha, alpha), 0xffffffff);

				m_pQuad->Render();

				sceGuAmbientColor(0xffffffff);
			}
			else
			{
				sceGuAmbientColor(0xffffffff);

				m_pQuad->Render();

				sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);

				m_pQuad->Render();
			}

			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);			
		}
	}

	///--- TIPO 1: Disparo rojo
	//////////////////////////////////////////////////////////////////////

	pVideo->EnableTexture(m_pTexture[1]);

	for (int i=0; i<MAX_PARTICLES; i++)
	{
		if (m_pParticleArray[1][i].enable)
		{			
			m_pParticleArray[1][i].timeStamp += dt;

			if (m_pParticleArray[1][i].timeStamp >= 5.0f)
			{
				m_pParticleArray[1][i].enable = false;
				m_pParticleArray[1][i].next = m_iFreeSlot[1];
				m_iFreeSlot[1] = i;

				continue;
			}

			m_pParticleArray[1][i].pos.x += m_pParticleArray[1][i].vel.x * dt;
			m_pParticleArray[1][i].pos.y += m_pParticleArray[1][i].vel.y * dt;


			stCollisionData data;

			data.radius = 1.0f;
			data.x = m_pParticleArray[1][i].pos.x;
			data.y = m_pParticleArray[1][i].pos.y;
			data.velX = m_pParticleArray[1][i].vel.x;
			data.velY = m_pParticleArray[1][i].vel.y;

			if (pTheLevel->TestCollision(&data, false))
			{		
				m_pParticleArray[1][i].pos.x = data.x;
				m_pParticleArray[1][i].pos.y = data.y;
				m_pParticleArray[1][i].vel.x = data.velX;
				m_pParticleArray[1][i].vel.y = data.velY;
			}

			for (int a=0; a<iNumEnemies; a++)
			{
				if (pEnemies[a].active)
				{			
					float disx = vecBallPos.x - m_pParticleArray[1][i].pos.x,;

					///--- sierra
					///--- rebota
					if ((pEnemies[a].type == 0) || (pEnemies[a].type == 3))
					{
						VECTOR3 vDis;
						VECTOR3 vTemp = {m_pParticleArray[1][i].pos.x, m_pParticleArray[1][i].pos.y, 4.0f};
						VECTOR3 vTemp2 = {pEnemies[a].posX + 4.0f, pEnemies[a].posY + 4.0f, 4.0f};

						MAT_VectorSubtract(&vDis, &vTemp2, &vTemp);

						float length = MAT_VectorQuadraticLength(&vDis);						

						if (length < 6.25f)
						{
							m_pParticleArray[1][i].enable = false;
							m_pParticleArray[1][i].next = m_iFreeSlot[1];
							m_iFreeSlot[1] = i;

							pSmoke->AddExplosion(vTemp2, 20.0f, true);

							pEnemies[a].active = false;

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

							pSoundPlayer->Play(SOUND_ENE_EXPLO, pan);

							break;
						}
					}
					///--- dirigidos
					///--- rectos
					else if ((pEnemies[a].type == 1) || (pEnemies[a].type == 2))
					{	
						VECTOR3 vTemp = {0};

						if (pEnemies[a].rot==0)
						{
							vTemp.x = pEnemies[a].posX+2.0f;
							vTemp.y = pEnemies[a].posY+4.0f;
						}						
						else if (pEnemies[a].rot==3)
						{
							vTemp.x = pEnemies[a].posX+4.0f;
							vTemp.y = pEnemies[a].posY+2.0f;
						}
						else if (pEnemies[a].rot==2)
						{
							vTemp.x = pEnemies[a].posX+6.0f;
							vTemp.y = pEnemies[a].posY+4.0f;
						}
						else if (pEnemies[a].rot==1)
						{
							vTemp.x = pEnemies[a].posX+4.0f;
							vTemp.y = pEnemies[a].posY+6.0f;						
						}	

						vTemp.z = 4.0f;

						VECTOR3 vDis;
						VECTOR3 vTemp2 = {m_pParticleArray[1][i].pos.x, m_pParticleArray[1][i].pos.y, 4.0f};

						MAT_VectorSubtract(&vDis, &vTemp, &vTemp2);

						float length = MAT_VectorQuadraticLength(&vDis);

						if (length < 6.0f)
						{
							m_pParticleArray[1][i].enable = false;
							m_pParticleArray[1][i].next = m_iFreeSlot[1];
							m_iFreeSlot[1] = i;

							pSmoke->AddExplosion(vTemp, 20.0f, true);

							pEnemies[a].active = false;

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

							pSoundPlayer->Play(SOUND_ENE_EXPLO, pan);

							break;
						}						
					}				
				}
			}

			if (!m_pParticleArray[1][i].enable)
				continue;


			///--- render

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos =  { m_pParticleArray[1][i].pos.x -1.0f,
					-(m_pParticleArray[1][i].pos.y + 1.0f), 4.0f };
				VECTOR3 scale = { 2.0f, 2.0f, 2.0f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);			
			}			

			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

			if (m_pParticleArray[1][i].timeStamp >= 4.0f)
			{
				int alpha = (int)MAT_Clampf((5.0f - m_pParticleArray[1][i].timeStamp) * 255.0f, 0.0f, 255.0f);

				sceGuAmbientColor(COLOR_ARGB(alpha, 255, 255, 255));			
			}
			else
			{
	
				sceGuAmbientColor(0xffffffff);							
			}

			m_pQuad->Render();

			sceGuAmbientColor(0xffffffff);

			

					
		}
	}

	///--- TIPO 2: Disparo enemigo piedra
	//////////////////////////////////////////////////////////////////////

	pVideo->EnableTexture(m_pTexture[2]);

	for (int i=0; i<MAX_PARTICLES_TYPE2; i++)
	{
		if (m_pParticleArray[2][i].enable)
		{			
			m_pParticleArray[2][i].timeStamp += dt;

			if (m_pParticleArray[2][i].timeStamp >= 3.0f)
			{
				m_pParticleArray[2][i].enable = false;
				m_pParticleArray[2][i].next = m_iFreeSlot[2];
				m_iFreeSlot[2] = i;

				continue;
			}

			m_pParticleArray[2][i].pos.x += m_pParticleArray[2][i].vel.x * dt;
			m_pParticleArray[2][i].pos.y += m_pParticleArray[2][i].vel.y * dt;

			stCollisionData data;

			data.radius = 0.4f;
			data.x = m_pParticleArray[2][i].pos.x;
			data.y = m_pParticleArray[2][i].pos.y;
			data.velX = m_pParticleArray[2][i].vel.x;
			data.velY = m_pParticleArray[2][i].vel.y;

			if (pTheLevel->TestSimpleCollision(&data))
			{		
				m_pParticleArray[2][i].enable = false;
				m_pParticleArray[2][i].next = m_iFreeSlot[2];
				m_iFreeSlot[2] = i;

				continue;
			}	

			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pParticleArray[2][i].pos.x, m_pParticleArray[2][i].pos.y, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			float length = MAT_VectorLength(&vDis);

			///--- toque con la bola

			if (!pTheLevel->IsBallDead())
			{
				if(length < (0.4f + fBallRadius))
				{
					VECTOR2 oldVel = pBall->GetVelocity();
					VECTOR2 newVel = {oldVel.x + (m_pParticleArray[2][i].vel.x / 4.0f),
										oldVel.y + (m_pParticleArray[2][i].vel.y / 4.0f)};

					pBall->SetVelocity(newVel);

					m_pParticleArray[2][i].enable = false;
					m_pParticleArray[2][i].next = m_iFreeSlot[2];
					m_iFreeSlot[2] = i;

					pSoundPlayer->Play(SOUND_TICK);

					continue;				
				}	
			}

			///--- render

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos =  { m_pParticleArray[2][i].pos.x -0.4f,
					-(m_pParticleArray[2][i].pos.y + 0.4f), 4.1 };
				VECTOR3 scale = { 0.8f, 0.8f, 0.8f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);
			}			

			if (m_pParticleArray[2][i].timeStamp >= 2.0f)
			{
				float alpha = MAT_Clampf((3.0f - m_pParticleArray[2][i].timeStamp) * 255.0f, 0.0f, 255.0f);

				sceGuAmbientColor(COLOR_ARGB((int)alpha, 255, 255, 255));
			}
			else
			{
				sceGuAmbientColor(0xffffffff);
			}

			m_pQuad->Render();

			sceGuAmbientColor(0xffffffff);
		}
	}

	///--- TIPO 3: Disparo enemigo dirigido
	//////////////////////////////////////////////////////////////////////

	pVideo->EnableTexture(m_pTexture[3]);

	for (int i=0; i<MAX_PARTICLES; i++)
	{
		if (m_pParticleArray[3][i].enable)
		{			
			m_pParticleArray[3][i].timeStamp += dt;

			if (m_pParticleArray[3][i].timeStamp >= 4.0f)
			{
				m_pParticleArray[3][i].enable = false;
				m_pParticleArray[3][i].next = m_iFreeSlot[3];
				m_iFreeSlot[3] = i;

				continue;
			}

			m_pParticleArray[3][i].pos.x += m_pParticleArray[3][i].vel.x * dt;
			m_pParticleArray[3][i].pos.y += m_pParticleArray[3][i].vel.y * dt;


			stCollisionData data;

			data.radius = 1.0f;
			data.x = m_pParticleArray[3][i].pos.x;
			data.y = m_pParticleArray[3][i].pos.y;
			data.velX = m_pParticleArray[3][i].vel.x;
			data.velY = m_pParticleArray[3][i].vel.y;

			if (pTheLevel->TestCollision(&data, false))
			{		
				m_pParticleArray[3][i].pos.x = data.x;
				m_pParticleArray[3][i].pos.y = data.y;
				m_pParticleArray[3][i].vel.x = data.velX;
				m_pParticleArray[3][i].vel.y = data.velY;
			}	


			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pParticleArray[3][i].pos.x, m_pParticleArray[3][i].pos.y, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if (!pTheLevel->IsBallDead())
			{
				///--- toque con la bola
				if(MAT_VectorQuadraticLength(&vDis) < ((1.0f + fBallRadius) * (1.0f + fBallRadius)))
				{
					pBall->SetRadius(fBallRadius - 0.2f);

					fBallRadius = pBall->GetRadius();

					vecBallVel.x += vDis.x * 2.0f;
					vecBallVel.y += vDis.y * 2.0f;

					pBall->SetVelocity(vecBallVel);

					pSmoke->AddExplosion(vTemp, 4.0f);
					pSmoke->AddExplosion(vecBallPos, 8.0f);

					m_pParticleArray[3][i].enable = false;
					m_pParticleArray[3][i].next = m_iFreeSlot[3];
					m_iFreeSlot[3] = i;

					pSoundPlayer->Play(SOUND_HURT);

					continue;				
				}	
			}

			///--- render

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos2 = { -1.0f, -1.0f, 0.0f };
				VECTOR3 scale = { 2.0f, 2.0f, 2.0f };

				VECTOR3 pos =  { m_pParticleArray[3][i].pos.x,
					-(m_pParticleArray[3][i].pos.y), 4.15f };
				

				sceGumTranslate(&pos);				
				sceGumRotateZ(MAT_ToRadians(m_fAngType3));
				sceGumTranslate(&pos2);
				sceGumScale(&scale);				
			}		

			if (m_pParticleArray[3][i].timeStamp >= 3.0f)
			{
				float alpha = MAT_Clampf((4.0f - m_pParticleArray[3][i].timeStamp) * 255.0f, 0.0f, 255.0f);

				sceGuAmbientColor(COLOR_ARGB((int)alpha, 255, 255, 255));
			}
			else
			{
				sceGuAmbientColor(0xffffffff);
			}

			m_pQuad->Render();

			sceGuAmbientColor(0xffffffff);
		}
	}

	///--- TIPO 4: Disparo recto
	//////////////////////////////////////////////////////////////////////

	pVideo->EnableTexture(m_Type4->GetTexture());

	for (int i=0; i<MAX_PARTICLES; i++)
	{
		if (m_pParticleArray[4][i].enable)
		{
			m_pParticleArray[4][i].timeStamp += dt;

			if (m_pParticleArray[4][i].timeStamp >= 4.0f)
			{
				m_pParticleArray[4][i].enable = false;
				m_pParticleArray[4][i].next = m_iFreeSlot[4];
				m_iFreeSlot[4] = i;

				continue;
			}

			m_pParticleArray[4][i].pos.x += m_pParticleArray[4][i].vel.x * dt;
			m_pParticleArray[4][i].pos.y += m_pParticleArray[4][i].vel.y * dt;

			stCollisionData data;

			data.radius = 0.5f;
			data.x = m_pParticleArray[4][i].pos.x;
			data.y = m_pParticleArray[4][i].pos.y;
			data.velX = m_pParticleArray[4][i].vel.x;
			data.velY = m_pParticleArray[4][i].vel.y;

			if (pTheLevel->TestSimpleCollision(&data))
			{		
				m_pParticleArray[4][i].enable = false;
				m_pParticleArray[4][i].next = m_iFreeSlot[4];
				m_iFreeSlot[4] = i;

				continue;
			}	

			VECTOR3 vDis;
			VECTOR3 vTemp = {m_pParticleArray[4][i].pos.x, m_pParticleArray[4][i].pos.y, 4.0f};

			MAT_VectorSubtract(&vDis, &vecBallPos, &vTemp);

			if (!pTheLevel->IsBallDead())
			{
				///--- toque con la bola
				if(MAT_VectorQuadraticLength(&vDis) < ((1.0f + fBallRadius) * (1.0f + fBallRadius)))
				{
					pBall->SetRadius(fBallRadius - 0.2f);

					fBallRadius = pBall->GetRadius();

					vecBallVel.x += vDis.x * 2.0f;
					vecBallVel.y += vDis.y * 2.0f;

					pBall->SetVelocity(vecBallVel);

					pSmoke->AddExplosion(vTemp, 4.0f);				
					pSmoke->AddExplosion(vecBallPos, 8.0f);

					m_pParticleArray[4][i].enable = false;
					m_pParticleArray[4][i].next = m_iFreeSlot[4];
					m_iFreeSlot[4] = i;

					pSoundPlayer->Play(SOUND_HURT);

					continue;				
				}	
			}

			///--- render

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				VECTOR3 pos =  { m_pParticleArray[4][i].pos.x, -m_pParticleArray[4][i].pos.y, 4.0f };
				VECTOR3 scale = { 0.15f, 0.15f, 0.15f };
				sceGumTranslate(&pos);
				sceGumScale(&scale);				

				if (m_pParticleArray[4][i].vel.x != 0.0f)
					sceGumRotateZ(MAT_ToRadians(90.0f));
			}	

			if (m_pParticleArray[4][i].timeStamp >= 3.0f)
			{
				float alpha = MAT_Clampf((4.0f - m_pParticleArray[4][i].timeStamp) * 255.0f, 0.0f, 255.0f);

				sceGuAmbientColor(COLOR_ARGB((int)alpha, 255, 255, 255));
			}
			else
			{
				sceGuAmbientColor(0xffffffff);
			}

			m_Type4->Render();

			sceGuAmbientColor(0xffffffff);

		}
	}

	sceGuDisable(GU_BLEND);

	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
}

/********************************************************************/
/********************************************************************/
/*						End Particles.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

