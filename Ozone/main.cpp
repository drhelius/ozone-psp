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
/*						    main.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////

#include "Util.h"
#include "Video.h"
#include "Input.h"
#include "TextureManager.h"
#include "Game.h"

extern "C"
{
#include "callbacks.h"
}

#ifdef PSP_SLIM
PSP_MODULE_INFO("Ozone", 0, 1, 0);
#else
PSP_MODULE_INFO("Ozone", 0, 1, 1);
#endif
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);


///--------------------------------------------------------------------
/// Función:    main
/// Creador:    Nacho (AMD)
/// Fecha:      domingo, 28 de enero de 2007, 22:54:20
///--------------------------------------------------------------------
int main(int argc, char* argv[])
{
	Log("Ozone Main()\n"); 

#ifdef PSP_SLIM
	PSP_HEAP_SIZE_KB(20480);
#endif

	setupCallbacks();

	CGame* pTheGame = CGame::GetSingleton();

	pTheGame->Init();	

	while(running())
	{
		pTheGame->Go();
	}

	SafeDelete(pTheGame);

	CTextureManager* pTexMan = CTextureManager::GetSingleton();
	SafeDelete(pTexMan);

	CVideo* pVideoMan = CVideo::GetSingleton();
	SafeDelete(pVideoMan);

	CInput* pInput = CInput::GetSingleton();
	SafeDelete(pInput);

	sceKernelExitGame();

	return 0;
}

/********************************************************************/
/********************************************************************/
/*						End main.cpp								*/
/*																    */
/********************************************************************/
/********************************************************************/
//////////////////////////////////////////////////////////////////////



#if 0
static unsigned int __attribute__((aligned(16))) list[262144];
extern unsigned char logo_start[];

struct Vertex
{
	u16 u, v;
	//unsigned int color;
	float x,y,z;
};

struct Vertex __attribute__((aligned(16))) vertices[12*3] =
{
	{0, 0, -4,-4, 4}, // 0
	{255, 0, -4, 4, 4}, // 4
	{255, 255,  4, 4, 4}, // 5

	{0, 0, -4,-4, 4}, // 0
	{255, 255,  4, 4, 4}, // 5
	{0, 255,  4,-4, 4}, // 255

	{0, 0, -4,-4,-4}, // 3
	{255, 0,  4,-4,-4}, // 2
	{255, 255,  4, 4,-4}, // 6

	{0, 0, -4,-4,-4}, // 3
	{255, 255,  4, 4,-4}, // 6
	{0, 255, -4, 4,-4}, // 7

	{0, 0,  4,-4,-4}, // 0
	{255, 0,  4,-4, 4}, // 3
	{255, 255,  4, 4, 4}, // 7

	{0, 0,  4,-4,-4}, // 0
	{255, 255,  4, 4, 4}, // 7
	{0, 255,  4, 4,-4}, // 4

	{0, 0, -4,-4,-4}, // 0
	{255, 0, -4, 4,-4}, // 3
	{255, 255, -4, 4, 4}, // 7

	{0, 0, -4,-4,-4}, // 0
	{255, 255, -4, 4, 4}, // 7
	{0, 255, -4,-4, 4}, // 4

	{0, 0, -4, 4,-4}, // 0
	{255, 0,  4, 4,-4}, // 255
	{255, 255,  4, 4, 4}, // 2

	{0, 0, -4, 4,-4}, // 0
	{255, 255,  4, 4, 4}, // 2
	{0, 255, -4, 4, 4}, // 3

	{0, 0, -4,-4,-4}, // 4
	{255, 0, -4,-4, 4}, // 7
	{255, 255,  4,-4, 4}, // 6

	{0, 0, -4,-4,-4}, // 4
	{255, 255,  4,-4, 4}, // 6
	{0, 255,  4,-4,-4}, // 5
};

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

struct CABECERA
{
	char ID[8];					// Debe ser siempre 'HELIOUS'
	int version;				
};

struct BLOQUE
{
	int desplaz;				// Desplazamiento en el archivo
	int longitud;				// Longitud en bytes
};

struct CUBO 
{
	int posX;
	int posY;

	int tex;
	int texLat;
	char lados[4];				// Longitud en bytes
};

int done = 0;
extern int _mm_errno;
extern BOOL _mm_critical;
extern char *_mm_errmsg[];
int mikModThreadID = -1;

void my_error_handler(void)
{
	printf("_mm_critical %d\n", MikMod_critical);
	printf("_mm_errno %d\n", MikMod_errno);
	printf("%s\n", MikMod_strerror(MikMod_errno));
	return;
}
static int AudioChannelThread(SceSize args, void *argp)
{
	while (!done)
	{
		MikMod_Update();
		// We have to sleep here to allow other threads a chance to process.
		// with no sleep this thread will take over when the output is disabled via MikMod_DisableOutput()
		// co-operative threading sucks bigtime...
		sceKernelDelayThread(1);
	}
	return (0);
}

extern UWORD md_mode;
extern UBYTE md_reverb;
extern UBYTE md_pansep;

int main(int argc, char* argv[])
{
	int maxchan = 128;
	MODULE *mf = NULL; // for mod
	SAMPLE *sf = NULL; // for wav
	int voice = 0; 	   // for wav
	int pan = 127;
	int vol = 127;
	int freq = 22000;

	fprintf(stderr, "start"); 
	float camX = 0.0f;
	float camY = 0.0f;
	float hei = 0.0f;

	setupCallbacks();

	SceCtrlData pad;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	// setup GU

	void* fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
	sceGuDepthBuffer(zbp,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	sceGuDepthRange(65535,0);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CCW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);

#if 0
	u8* miTextura = new u8[65536];

	int fd = sceIoOpen("cube05.psptex", PSP_O_RDONLY, 0777); 

	int readBytes = sceIoRead(fd, miTextura, 65536); 

	sceIoClose(fd);


	fd = sceIoOpen("nivel1.nhe", PSP_O_RDONLY, 0777); 
    
	if(fd <= 0) { 
		printf("Error opening file."); 
	 sceGuTerm();

	sceKernelExitGame();
	return -1;
	} 

	CABECERA m_Cabecera;
	BLOQUE m_Bloque[5];

	//unsigned char *buffer = (unsigned char *)malloc(28420*sizeof(unsigned char));

	readBytes = sceIoRead(fd, &m_Cabecera, sizeof(CABECERA)); 

	if(strcmp(m_Cabecera.ID,"HYDRIUM")!=0)
	{
		printf("Error reading file."); 
	  sceGuTerm();

	sceKernelExitGame();
	return -1;
	}

	if(m_Cabecera.version!=27)
	{
		printf("Error version file."); 
	  sceGuTerm();

	sceKernelExitGame();

	return -1;
	}

	readBytes += sceIoRead(fd, &m_Bloque, sizeof(BLOQUE) * 5); 

	int m_iNumCubos = m_Bloque[0].longitud / sizeof(CUBO);

	CUBO *m_pCubos = new CUBO[m_iNumCubos];//(CUBO*)malloc(m_iNumCubos * sizeof(CUBO));
	
	int i;
	int saltarse = (m_Bloque[0].desplaz - readBytes);
	for (i=0; i< saltarse; i++)
	{
		unsigned char temp;
		sceIoRead(fd, &temp, sizeof(unsigned char)); 
	}
	
	readBytes = sceIoRead(fd, m_pCubos, sizeof(CUBO) * m_iNumCubos);
#else

	u8* miTextura = new u8[262144];

	int fd = sceIoOpen("h_logo.psptex", PSP_O_RDONLY, 0777); 

	int readBytes = sceIoRead(fd, miTextura, 262144); 

	sceIoClose(fd);

	fd = sceIoOpen("cubo.O3D", PSP_O_RDONLY, 0777);

	u32 tris = 0;
	u32 verts = 0;

	sceIoRead(fd, &tris, 4); 
	sceIoRead(fd, &verts, 4); 

	printf("Verts: %d\n", verts);
	printf("Tris: %d\n", tris);

	printf("malloc: %d\n", sizeof(VERTEX3D));

	VERTEX3D* pModelo = new VERTEX3D[verts];

	printf("desp malloc");

	for (u32 ve=0; ve<verts; ve++)
	{
		sceIoRead(fd, &pModelo[ve], sizeof(VERTEX3D)); 

		//printf("v: %f %f %f %d %d\n", x, y, z, pModelo[ve].u, pModelo[ve].v);
	}
printf("carga modelo");

	sceIoClose(fd);
#endif
	// run sample





	//pspAudioInit();

	//MP3_Init(1);
	//MP3_Load("ambient.mp3");
	//MP3_Play(); 

/*
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(1);

	if (!MikMod_InitThreads())
	{
		printf("MikMod thread init failed\n");
	}
*/
	MikMod_RegisterErrorHandler(my_error_handler);
	/* register all the drivers */
	MikMod_RegisterAllDrivers();
	/* register all the module loaders */
	MikMod_RegisterAllLoaders();

	/* initialize the library */
	md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_SNDFX|DMODE_SOFT_MUSIC; 
	md_reverb = 0;
	md_pansep = 128;
	if (MikMod_Init(""))
	{
		printf("Could not initialize sound, reason: %s\n", MikMod_strerror(MikMod_errno));
		sceKernelExitGame();
		return 0;
	}

	MikMod_SetNumVoices(-1, 8);
	/* get ready to play */

	sf = Sample_Load("nivel_gema_roja2.wav");

	printf("Starting.\n");
	MikMod_EnableOutput();
	BOOL outputEnabled = true;

	

	if ((mikModThreadID = sceKernelCreateThread("MikMod" ,AudioChannelThread,0x12,0x10000,0,NULL)) > 0)
	{
		sceKernelStartThread(mikModThreadID, 0 , NULL);
	}
	else
	{
		printf("Play thread create failed!\n");
	}

	while(running())
	{
		sceCtrlPeekBufferPositive(&pad, 1); 

		if (pad.Buttons != 0){		

			if (pad.Buttons & PSP_CTRL_UP){
				camY--;
			} 
			if (pad.Buttons & PSP_CTRL_DOWN){
				camY++;
			} 
			if (pad.Buttons & PSP_CTRL_LEFT){
				camX++;
			} 
			if (pad.Buttons & PSP_CTRL_RIGHT){
				
				camX--;
			}  
			
			if (pad.Buttons & PSP_CTRL_CIRCLE){
				hei--;
			} 
			if (pad.Buttons & PSP_CTRL_CROSS){
				hei++;
			} 
			if(pad.Buttons & PSP_CTRL_TRIANGLE)
			{
				voice = Sample_Play(sf,0,0);
				Voice_SetPanning(voice, pan);
			}
		}

		sceGuStart(GU_DIRECT,list);

		// clear screen

		sceGuClearColor(0xffffffff);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

		// setup matrices for cube

		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(40.0f,16.0f/9.0f,0.5f,1000.0f);

		sceGumMatrixMode(GU_VIEW);
		
		sceGumLoadIdentity();
		
		
#if 0
		// setup texture

		sceGuTexMode(GU_PSM_8888,0,0,GU_TRUE);
		sceGuTexImage(0,128,128,128,miTextura);
		sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGB);
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		sceGuTexScale(0.5f,0.5f);
		sceGuTexOffset(0.0f,0.0f);
		sceGuAmbientColor(0xffffffff);


		// draw cube

		
		for (i=0; i<m_iNumCubos; i++)
		{
			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
			{
				ScePspFVector3 pos = { (float)m_pCubos[i].posX + camX, (float)-m_pCubos[i].posY + camY, -40.0f - hei };
				//ScePspFVector3 rot = { val * 0.79f * (GU_PI/180.0f), val * 0.98f * (GU_PI/180.0f), val * 1.32f * (GU_PI/180.0f) };
				sceGumTranslate(&pos);
				//sceGumRotateXYZ(&rot);
			}

			sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_8BIT|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);

		}
#else
		// setup texture

		sceGuTexMode(GU_PSM_8888,0,0,GU_TRUE);
		sceGuTexImage(0,256,256,256,miTextura);
		sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGB);
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		sceGuTexScale(0.5f,0.5f);
		sceGuTexOffset(0.0f,0.0f);
		sceGuAmbientColor(0xffffffff);

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			ScePspFVector3 pos = { camX, camY, -40.0f - hei };
			//ScePspFVector3 rot = { val * 0.79f * (GU_PI/180.0f), val * 0.98f * (GU_PI/180.0f), val * 1.32f * (GU_PI/180.0f) };
			sceGumTranslate(&pos);
			//sceGumRotateXYZ(&rot);
		}

		//sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_8BIT|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);
		sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_16BIT|GU_VERTEX_32BITF|GU_TRANSFORM_3D,verts,0,pModelo);


#endif
		sceGuFinish();
		sceGuSync(0,0);
		//sceDisplayWaitVblankStart();
		sceGuSwapBuffers();

		//val++;
	}

	sceGuTerm();

	//MP3_Stop();
	//MP3_FreeTune();

	// allow audio thread to terminate cleanly
	done = true;
	if (mikModThreadID > 0)
	{
		SceUInt timeout = 100000;
		sceKernelWaitThreadEnd(mikModThreadID, &timeout);
		// not 100% sure if this is necessary after a clean exit, but just to make sure any resources are freed:
		sceKernelDeleteThread(mikModThreadID);
	}
	Player_Stop();
	Player_Free(mf);
	MikMod_Exit();

	sceKernelExitGame();
	return 0;
}

#endif
