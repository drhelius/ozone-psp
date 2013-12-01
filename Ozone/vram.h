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

#ifndef common_vram_h
#define common_vram_h

/*
	The following are a few helperfunctions to help manage vram in gu-examples.
	Do not use for your own code, it's better you manage it in your own way.
*/

/* make a static allocation of vram memory and return pointer relative to vram start */
void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm);
/* make a static allocation of vram memory and return absolute pointer */
void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm);

// the following is not yet implemented
/*
void beginDynamicVramFrame();
void endDynamicVramFrame();

void* getDynamicVramBuffer(unsigned int width, unsigned int height, unsigned int psm);
void* getDynamicVramTexture(unsigned int width, unsigned int height, unsigned int psm);
*/

#endif
