OBJS = Ball.o Bitset.o callbacks.o Camera.o Fade.o Game.o Input.o LayerManager.o Level.o Logo.o main.o Mesh.o Particles.o Quad.o Smoke.o Sound.o TextFont.o TextureManager.o Timer.o Video.o vram.o 
TARGET = game
INCDIR =
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LDFLAGS =
LIBS= -lmad -lmikmod -lpspaudiolib -lpspaudio -lpsppower -lpspgum -lpspgu -lpsprtc -lm -lstdc++
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Ozone
PSP_EBOOT_PIC1 = ./Menu/PIC1.PNG
PSP_EBOOT_ICON = ./Menu/ICON0.PNG
PSP_EBOOT_SND0 = ./Menu/SND0.AT3
PSPSDK=$(shell psp-config --pspsdk-path)
include e:/pspdev/PSP/SDK/lib/build.mak

