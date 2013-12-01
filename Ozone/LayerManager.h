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

#pragma once

#include "Quad.h"
#include "Camera.h"

class CLayerManager
{
	private:

		float m_fMax1, m_fMin1, m_fMax2, m_fMin2, m_fMax3, m_fMin3; 

		CQuad* m_pQuad;
		CVideo* m_pVideoManager;

		struct stNodeLayer
		{
			VECTOR3 pos;
			VECTOR2 vel;
			float rot;
			float velrot;
		};

		TEXTURE* m_pTexture[3];
	
		typedef std::list<stNodeLayer*>::iterator Itor;

		std::list<stNodeLayer*> m_Lista[3];

	public:

		CLayerManager(void);
		~CLayerManager(void);

		void AddParticle(float posx, float posy, float velx, float vely, int layer);
		void Init(float max1, float min1, float max2, float min2, float max3, float min3);

		void End(void);
		void Render(int layer, CCamera* pCamera);
		void Frame(float delta, float maxx, float maxy);

};
