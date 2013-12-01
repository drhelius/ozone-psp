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
/*								Bitset.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Bitset.h"


//--------------------------------------------------------------------
// Función:    CBitset::Init
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:30:45
//--------------------------------------------------------------------
void CBitset::Init(int iBitNumber) 
{ 
	// Obtenemos el numero de integers necesarios
	m_iIntNum = (iBitNumber >> 5) + 1;

	// Borramos si ya tuvieramos una tabla
	SafeDeleteArray(m_pBits);

	// Creamos la tabla y la inicilizamos a 0
	m_pBits = new unsigned int[m_iIntNum];
	
	ResetAll();
}


//--------------------------------------------------------------------
// Función:    CBitset::Set
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:30:53
//--------------------------------------------------------------------
void CBitset::Set(int i) 
{
	m_pBits[i >> 5] |= (1 << (i & 31));
}


//--------------------------------------------------------------------
// Función:    CBitset::IsSet
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:30:50
//--------------------------------------------------------------------
bool CBitset::IsSet(int i) 
{
	return ((m_pBits[i >> 5] & (1 << (i & 31 ))) != 0);
}


//--------------------------------------------------------------------
// Función:    CBitset::Reset
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:30:49
//--------------------------------------------------------------------
void CBitset::Reset(int i) 
{
	m_pBits[i >> 5] &= ~(1 << (i & 31));	
}


//--------------------------------------------------------------------
// Función:    CBitset::ResetAll
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:30:47
//--------------------------------------------------------------------
void CBitset::ResetAll(void) 
{
	memset(m_pBits, 0, sizeof(unsigned int) * m_iIntNum);
}


/********************************************************************/
/********************************************************************/
/*							End Bitset.cpp							*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////
