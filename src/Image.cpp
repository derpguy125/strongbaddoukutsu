// THIS IS CODE.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "Image.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "WindowsWrapper.h"

#include "Draw.h"
#include "Escape.h"
#include "Flags.h"
#include "Generic.h"
#include "KeyControl.h"
#include "Main.h"
#include "MycParam.h"
#ifdef EXTRA_SOUND_FORMATS
#include "ExtraSoundFormats.h"
#endif
#include "Organya.h"
#include "Pause.h"
#include "Stage.h"
#include "TextScr.h"

// Draw illustration
void PutImg(void)
{
	RECT rcIllust = {0, 0, 320, 240};
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240 // TODO - Move this to CSE2EX
	// Widescreen edit
	RECT rcClip = {(WINDOW_WIDTH - 320) / 2, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	PutBitmap3(&rcClip, SubpixelToScreenCoord(0) + PixelToScreenCoord((WINDOW_WIDTH - 320) / 2), PixelToScreenCoord((WINDOW_HEIGHT - 240) / 2), &rcIllust, SURFACE_ID_IMG_OVERLAYS);
#else
	PutBitmap3(&grcFull, SubpixelToScreenCoord(0) + PixelToScreenCoord((WINDOW_WIDTH - 320) / 2), PixelToScreenCoord((WINDOW_HEIGHT - 240) / 2), &rcIllust, SURFACE_ID_IMG_OVERLAYS);
#endif
}

// Load illustration
void ReloadImg(int a)
{
	char name[16];
	sprintf(name, "Img%04d", a);
	std::string path = std::string("Images/") + name;
	ReloadBitmap_File(path.c_str(), SURFACE_ID_IMG_OVERLAYS);
}