/* pdf.h
   Header file for pdf.c

   Part of the swftools package.

   Copyright (c) 2009 Matthias Kramm <kramm@quiss.org> 
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef __gfxdevice_pdf_h__
#define __gfxdevice_pdf_h__

#include "../gfxdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

gfxfontlist_t* get_fontlist(gfxdevice_t*dev);
void pdf_drawchars(gfxdevice_t*dev, gfxfont_t*font, int* chars, int len, gfxcolor_t*color, gfxmatrix_t*matrixes);
void gfxdevice_pdf_init(gfxdevice_t*dev);

#ifdef __cplusplus
}
#endif

#endif //__gfxdevice_pdf_h__
