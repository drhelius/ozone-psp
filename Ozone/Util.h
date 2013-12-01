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
/*								Util.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <malloc.h>

#include <list>

#include <pspge.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psprtc.h>
#include <pspctrl.h>
#include <pspaudiolib.h>

#include <mikmod.h>

#define PSP_SLIM 0
//#define DEBUG_PSP_OZONE 0

//#define SPLASH_NEOFLASH 0
//#define SPLASH_GBAX 0

#define SafeDelete(pointer) if(pointer != NULL) {delete pointer; pointer = NULL;}
#define SafeDeleteArray(pointer) if(pointer != NULL) {delete [] pointer; pointer = NULL;}

#define InitPointer(pointer) ((pointer) = NULL)
#define IsValidPointer(pointer) ((pointer) != NULL)

#define PSP_MAX_PATH (128)
#define PSP_BUF_WIDTH (512)
#define PSP_SCR_WIDTH (480)
#define PSP_SCR_HEIGHT (272)

#define MAX_VELOCITY (50.0f)
#define TIME_LIGHT (1.0f)	///--- en segundos
#define BOUNCE_BRAKE (0.85f) ///--- tasa de desaceleración
#define MAX_SMOKE_PARTICLES (150)
#define MAX_PARTICLES (30)
#define MAX_PARTICLES_TYPE2 (300)

#define VRAM_SCRATCH_PAD_START (0x041F0000)
#define VRAM_UTIL_START (0x04154000)

#define VECTOR3 ScePspFVector3
#define VECTOR2 ScePspFVector2
#define MATRIX ScePspFMatrix4

#define M_PI_ENTRE_180 (0.017453293f)
#define M_180_ENTRE_PI (57.29577951f)

#define COLOR_ARGB(a,r,g,b) \
	((unsigned int)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

typedef unsigned int DWORD;

enum SOUNDS
{
	SOUND_LEVEL_END,
	SOUND_GEM_GREEN,
	SOUND_GEM_YELLOW,
	SOUND_GEM_RED,
	SOUND_GEM_BLUE,
	SOUND_GEM_BLUE2,
	SOUND_LEVEL_AIRPUMP,
	SOUND_LEVEL_IMP,
	SOUND_HURT,
	SOUND_MOVE,
	SOUND_DOORS,
	SOUND_TICK,
	SOUND_TRANS,
	SOUND_EXPLO,
	SOUND_ENE2_FIRE,
	SOUND_ENE3_FIRE,
	SOUND_ENE_EXPLO,
	SOUND_FIRE_1,
	SOUND_FIRE_2,
	SOUND_ENE4_BOUNCE
};

enum MUSICS
{
	MUSIC_AMBIENT1,
	MUSIC_AMBIENT2,
	MUSIC_AMBIENT3,
	MUSIC_AMBIENT4,
	MUSIC_AMBIENT5,
	MUSIC_LOGO,
	MUSIC_MENU	
};

struct VERTEX3D
{
	u16 u, v;
	float x,y,z;
};

struct VERTEX3D_NORMAL
{
	u16 u, v;	
	float nx,ny,nz;
	float x,y,z;
};

#define VERTEX3D_FORMAT (GU_TEXTURE_16BIT|GU_VERTEX_32BITF|GU_TRANSFORM_3D)
#define VERTEX3D_FORMAT_NORMAL (GU_TEXTURE_16BIT|GU_NORMAL_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D)

struct TEXTURE
{
	u8* pTextureData;
	int w, h;
	bool bInVRAM;
};

enum LEVEL_BLOCKS
{
	khCubes,					
	khSectors,	
	khEnemies,
	khObjects,
	khTransports,
	khMaxBlocks					
};

struct HEADER
{
	char ID[8];					
	int version;				
};

struct BLOCK	
{
	int offset;				// Desplazamiento en el archivo
	int length;				// Longitud en bytes
};

struct CUBE 
{
	int posX;
	int posY;

	int tex;
	int texLat;
	bool sides[4];				
};

struct SECTOR
{
	int numCubes;
	int numObjs;

	int IndCub[25];
	int IndObj[100];
};

struct LEVEL
{
	int NumSecX;
	int NumSecY;

	int IniX;
	int IniY;

	int EndX;
	int EndY;

	int Back;

	int Gems;
};

struct ENEMY_File
{
	int posX;
	int posY;

	int type;
	int rot;
};

struct ENEMY
{
	float posX;
	float posY;

	int type;
	int rot;

	float timeShot;

	float velX;
	float velY;

	bool active;
};

struct OBJECT
{
	int posX;
	int posY;

	int type;
	int rot;

	float timeShot;

	bool active;
};

struct OBJECT_File
{
	int posX;
	int posY;

	int type;
	int rot;
};

struct TRANS
{
	int posX;
	int posY;

	int id;
};

struct stCollisionData
{
	float x;
	float y;
	float velX;
	float velY;
	float radius;
};


//--------------------------------------------------------------------
// Función:    Log
// Propósito:  
// Fecha:      jueves, 09 de noviembre de 2006, 13:01:21
//--------------------------------------------------------------------
inline void Log(const char* const msg, ...)
{
#ifdef DEBUG_PSP_OZONE
	va_list args;
	
	va_start(args, msg);
	
	char szBuf[256];
	
	vsprintf(szBuf, msg, args);

	fprintf(stderr, szBuf);
	fprintf(stderr, "\n");

	va_end(args);
#endif
}


//--------------------------------------------------------------------
// Función:    MAT_RGBToBGR
// Propósito:  
// Fecha:      jueves, 30 de noviembre de 2006, 19:42:25
//--------------------------------------------------------------------
inline u32 MAT_RGBToBGR(u32 color)
{
	return (color & 0xFF00FF00) | ((color >> 16) & 0xFF) | ((color & 0xFF) << 16);
}


//--------------------------------------------------------------------
// Función:    MAT_ToRadians
// Propósito:  Pasa de Grados a Radianes
// Fecha:      martes, 28 de noviembre de 2006, 22:46:51
//--------------------------------------------------------------------
inline float MAT_ToRadians(float angle)
{
	return (angle * M_PI_ENTRE_180);
}


//--------------------------------------------------------------------
// Función:    MAT_ToDegrees
// Propósito:  Pasa de Radianes a Grados
// Fecha:      martes, 28 de noviembre de 2006, 22:46:41
//--------------------------------------------------------------------
inline float MAT_ToDegrees(float angle)
{
	return (angle * M_180_ENTRE_PI);
}


//--------------------------------------------------------------------
// Función:    MAT_VectorAdd
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 19:10:57
//--------------------------------------------------------------------
inline void MAT_VectorAdd(VECTOR3 *result, VECTOR3 *v1, VECTOR3 *v2)
{
	result->x = v1->x + v2->x;
	result->y = v1->y + v2->y;
	result->z = v1->z + v2->z;
}


//--------------------------------------------------------------------
// Función:    MAT_VectorSubtract
// Creador:    Nacho (AMD)
// Fecha:      Monday  11/06/2007  22:07:26
//--------------------------------------------------------------------
inline void MAT_VectorSubtract(VECTOR3 *result, VECTOR3 *v1, VECTOR3 *v2)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	result->z = v1->z - v2->z;
}


//--------------------------------------------------------------------
// Función:    MAT_Vector2Subtract
// Creador:    Nacho (AMD)
// Fecha:      Thursday  21/06/2007  17:05:49
//--------------------------------------------------------------------
inline void MAT_Vector2Subtract(VECTOR2 *result, VECTOR2 *v1, VECTOR2 *v2)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
}


//--------------------------------------------------------------------
// Función:    MAT_VectorScalar
// Propósito:  
// Fecha:      miércoles, 29 de noviembre de 2006, 19:11:35
//--------------------------------------------------------------------
inline void MAT_VectorScalar(VECTOR3 *result, VECTOR3 *v1, float scalar)
{
	result->x = v1->x * scalar;
	result->y = v1->y * scalar;
	result->z = v1->z * scalar;
}


//--------------------------------------------------------------------
// Función:    MAT_abs
// Creador:    Nacho (AMD)
// Fecha:      Monday  11/06/2007  22:05:57
//--------------------------------------------------------------------
inline float MAT_abs(float num)
{
	return (num < 0) ? -num : num;
}

//--------------------------------------------------------------------
// Función:    MAT_VectorQuadraticLength
// Creador:    Nacho (AMD)
// Fecha:      Monday  11/06/2007  22:01:54
//--------------------------------------------------------------------
inline float MAT_VectorQuadraticLength(VECTOR3 *v)
{
	return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}


//--------------------------------------------------------------------
// Función:    MAT_VectorLength
// Creador:    Nacho (AMD)
// Fecha:      Monday  11/06/2007  22:02:37
//--------------------------------------------------------------------
inline float MAT_VectorLength(VECTOR3 *v)
{
	return sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}


//--------------------------------------------------------------------
// Función:    MAT_Vector2Length
// Creador:    Nacho (AMD)
// Fecha:      Friday  15/06/2007  12:34:10
//--------------------------------------------------------------------
inline float MAT_Vector2Length(VECTOR2 *v)
{
	return sqrtf((v->x * v->x) + (v->y * v->y));
}

//------------------------------------------------------------------------------
// Función: MAT_InvRaizCuadrada
// Propósito: Calcula 1 / raiz cuadrada de un float
//------------------------------------------------------------------------------
inline float MAT_InvRaizCuadrada(float numero)
{
	long i;
	float x2, y;
	
	x2 = numero * 0.5f;
	y  = numero;
	i  = * ( long * ) &y;						
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( 1.5f - ( x2 * y * y ) );   
	y  = y * ( 1.5f - ( x2 * y * y ) );

	return y;
}


//--------------------------------------------------------------------
// Función:    MAT_Clamp
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:50:17
//--------------------------------------------------------------------
inline int MAT_Clamp(int num, int min, int max)
{
	if (num > max)
	{
		return max; 
	}
	if (num < min)
	{
		return min; 
	}
	return num;
}

//--------------------------------------------------------------------
// Función:    MAT_Clampf
// Creador:    Nacho (AMD)
// Fecha:      miércoles, 31 de enero de 2007, 19:50:17
//--------------------------------------------------------------------
inline float MAT_Clampf(float num, float min, float max)
{
	if (num > max)
	{
		return max; 
	}
	if (num < min)
	{
		return min; 
	}
	return num;
}

//--------------------------------------------------------------------
// Función:    MAT_Max
// Creador:    Nacho (AMD)
// Fecha:      Monday  11/06/2007  18:57:55
//--------------------------------------------------------------------
inline float MAT_Max(float num1, float num2)
{
	return (num1 > num2) ? num1 : num2;	
}

//--------------------------------------------------------------------
// Función:    MAT_Min
// Creador:    Nacho (AMD)
// Fecha:      Monday  11/06/2007  18:58:00
//--------------------------------------------------------------------
inline float MAT_Min(float num1, float num2)
{
	return (num1 < num2) ? num1 : num2;	
}

//--------------------------------------------------------------------
// Función:    MAT_RandomInt
// Creador:    Nacho (AMD)
// Propósito:  Devuelve un número entero aleatorio entre dos introducidos, el alto
//			   excluido y el bajo incluido.	
// Fecha:      martes, 06 de febrero de 2007, 19:29:34
//--------------------------------------------------------------------
inline int MAT_RandomInt(int low, int high)
{
	int range = high - low;
	int num = rand() % range;
	return (num + low);
}

//------------------------------------------------------------------------------
// Función: MAT_RaizCuadrada
// Propósito: Calcula la raiz cuadrada de un float
//------------------------------------------------------------------------------
inline float MAT_RaizCuadrada(float numero)
{
	//return 1.0f / MAT_InvRaizCuadrada(numero);

	return sqrtf(numero);
}

//------------------------------------------------------------------------------
// Función: MAT_NormalizarAngulo360
// Propósito: Devuelve un ángulo normalizado entre [0 <= angle < 360]
//------------------------------------------------------------------------------
inline float MAT_NormalizarAngulo360(float angulo)
{
	while (angulo>360.0f)
	{
		angulo-=360.0f;
	}

	while (angulo<0.0f)
	{
		angulo+=360.0f;
	}

	return angulo;
}

//------------------------------------------------------------------------------
// Función: MAT_NormalizarAngulo180
// Propósito: Devuelve un ángulo normalizado entre [-180 < angulo <= 180]
//------------------------------------------------------------------------------
inline float MAT_NormalizarAngulo180(float angulo)
{
	angulo = MAT_NormalizarAngulo360(angulo);
	if (angulo > 180.0f)
	{
		angulo-=360.0f;
	}
	return angulo;
}

//--------------------------------------------------------------------
// Función:    UTIL_NextWord
// Propósito:  Avanza dwPos hasta la próxima palabra de la cadena szBuff[dwPos]
// Fecha:      19/07/2004 18:59
//--------------------------------------------------------------------
inline void UTIL_NextWord(char *szBuff, DWORD &dwPos)
{
	//--- Va hasta el final de la palabra actual
	for(; szBuff[dwPos]!='\0' && szBuff[dwPos]!=' ' && szBuff[dwPos]!='\t' && szBuff[dwPos]!='('; dwPos++);
	//--- Se salta los espacios, tabs que haya
	for(; szBuff[dwPos]==' ' || szBuff[dwPos]=='\t'; dwPos++);
}


//--------------------------------------------------------------------
// Función:    UTIL_TakeNextWord
// Propósito:  Mete en 'szWord' la próxima palabra de la cadena szBuff[dwPos]
// Fecha:      19/07/2004 18:58
//--------------------------------------------------------------------
inline void UTIL_TakeNextWord(char *szBuff, DWORD &dwPos, char *szWord, const DWORD dwMaxSize)
{
	DWORD dwLen=(DWORD)strlen((char*)szBuff);

	for(;szBuff[dwPos]==' ' || szBuff[dwPos]=='\t' || szBuff[dwPos]=='(' || szBuff[dwPos]==','
		|| szBuff[dwPos]==')'; ){
			dwPos++;
	}

	DWORD dw;
	for(dw=0; 
		szBuff[dwPos]!='\0' && szBuff[dwPos]!=' ' && szBuff[dwPos]!='\t' && szBuff[dwPos]!='(' &&
		szBuff[dwPos]!=')' && szBuff[dwPos]!=',' && 
		dw<dwMaxSize-1 && dwPos<dwLen; 
	dwPos++, dw++){
		szWord[dw]=szBuff[dwPos];
	}
	szWord[dw]=0;
}


//--------------------------------------------------------------------
// Función:    UTIL_TakeNextName
// Propósito:  Coge el nombre que viene a partir de dwPos
//			   El nombre tiene que estar entre comillas y puede contener espacios
// Fecha:      19/07/2004 18:57
//--------------------------------------------------------------------
inline void UTIL_TakeNextName(const char *szLine, DWORD &dwPos, char *szName, const DWORD dwMaxSize)
{
	DWORD	dwLen=(DWORD)strlen(szLine);
	bool	bFnd;

	szName[0]='\0';
	for(bFnd=true; dwPos<dwLen; dwPos++){
		if(szLine[dwPos]=='\x22'){
			dwPos++;
			bFnd = true;
			break;
		}
	}
	if(!bFnd){
		//LOG.Write(LOG_APP, "#¡[E] Se esperaba un nombre entre comillas [%s]", szLine);
		return;
	}
	DWORD	i;
	for(i=0, bFnd=false; dwPos<dwLen && i<dwMaxSize-1; i++, dwPos++){
		if(szLine[dwPos]=='\x22'){
			bFnd=true;
			break;
		}
		szName[i]=szLine[dwPos];
	}
	szName[i]='\x0';
	if(!bFnd){
		//LOG.Write(LOG_APP, "#¡[E] No se han encontrado comillas finales o se ha llegado a dwMaxSize antes de las comillas");
		//LOG.Write(LOG_APP, "#¡[E] [%s]", szLine);
	}
}

//--------------------------------------------------------------------
// Función:    UTIL_ToUpper
// Propósito:  
// Fecha:      19/07/2004 18:49
//--------------------------------------------------------------------
inline void UTIL_ToUpper(char *txt, char *upptxt)
{
	int i;

	for(i=0; txt[i]!=0; i++)
	{
		if(txt[i]>='a' && txt[i]<='z')
			upptxt[i]=(char)(txt[i]-('a'-'A'));
		else
		{
			switch(txt[i])
			{
				case 'ñ':
					upptxt[i]='Ñ';
					break;

				case 'á':
					upptxt[i]='A';
					break;

				case 'é':
					upptxt[i]='E';
					break;

				case 'í':
					upptxt[i]='I';
					break;

				case 'ó':
					upptxt[i]='O';
					break;

				case 'ú':
					upptxt[i]='U';
					break;

				default:
					upptxt[i]=txt[i];
					break;
			}
		}
	}
	upptxt[i]=0;
}


//--------------------------------------------------------------------
// Función:    UTIL_GetExtension
// Propósito:  Coge la extensión del fichero de un path
// Fecha:      19/07/2004 18:51
//--------------------------------------------------------------------
inline void UTIL_GetExtension(const char *szPathName, char *szExt)
{
	DWORD i, dwLen=(DWORD)strlen(szPathName);
	szExt[0]='\0';
	for(i=dwLen; i>0; --i){
		if(szPathName[i]=='.'){
			memcpy(szExt, &szPathName[i+1], dwLen-i);
			break;
		}
	}
}


//--------------------------------------------------------------------
// Función:    UTIL_GetFileName
// Propósito:  Coge el nombre del fichero de un path completo
// Fecha:      19/07/2004 18:53
//--------------------------------------------------------------------
inline void UTIL_GetFileName(const char *szPathName, char *szName)
{
	szName[0]='\0';
	DWORD dwLen=(DWORD)strlen(szPathName);

	DWORD i;
	for(i=dwLen; i>0; i--){
		if(szPathName[i]=='\\' || szPathName[i]=='/'){
			i++;
			break;
		}
	}
	///--- dwLen+1 para que copie el 0 final
	for(DWORD j=0; i<dwLen+1; j++, i++){
		szName[j]=szPathName[i];
	}
}


//-------------------------------------------------------------------------------
// Función: UTIL_PreParseLine
// Propósito: 
//-------------------------------------------------------------------------------	
inline void UTIL_PreParseLine(char *szLine)
{
	///---------------------------------
	///--- Elimina espacios, tabs, etc.
	///---------------------------------
	DWORD	dwLen=DWORD(strlen(szLine));
	DWORD	dwPos;

	if(dwLen){
		///--- Si tiene, quita el retorno de carro, espacios finales, ...
		while(szLine[dwLen-1]<33 && szLine[dwLen-1]>0 && dwLen>0){
			--dwLen;
		}
		szLine[dwLen]='\0';
		///--- Recorre la línea
		for(dwPos=0; dwPos<dwLen;){
			///--- Si es un comentario pasa del resto de la línea
			if(szLine[dwPos]=='/' && szLine[dwPos+1]=='/'){
				szLine[dwPos]='\0';
				break;
			}
			///--- Elimina espacios y tabs
			if(szLine[dwPos]==' ' || szLine[dwPos]=='\t'){
				//XTRACEX("%d %d %s [%s]", dwPos, dwLen, &szLine[dwPos], szLine);
				memmove(&szLine[dwPos], &szLine[dwPos+1], dwLen-dwPos);
				--dwLen;
			}
			///--- Si es un nombre entre comillas lo conserva
			else if(szLine[dwPos]=='\x22'){
				for(; dwPos<dwLen; ){
					++dwPos;
					if(szLine[dwPos]=='\x22'){
						++dwPos;
						break;
					}
				}
			}
			else{
				///--- Es texto, sigue hasta espacio o tab
				for(; dwPos<dwLen;){
					++dwPos;
					if(szLine[dwPos]==' '){
						///--- deja un espacio entre palabras
						++dwPos;
						break;
					}
					else if(szLine[dwPos]=='\t'){
						///--- Convierte el tab a espacio
						szLine[dwPos]=' ';
						++dwPos;
						break;
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

/********************************************************************/
/********************************************************************/
/*							End Util.h								*/
/*																    */
/********************************************************************/
/********************************************************************/
