/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Graphics maniuplation routines - private header file

#ifndef SAGA_GFX_H
#define SAGA_GFX_H

#include "common/rect.h"
#include "graphics/surface.h"

namespace Saga {

using Common::Point;
using Common::Rect;

enum CursorType {
	kCursorNormal,
	kCursorBusy
};

struct ClipData {
	// input members
	Rect sourceRect;
	Rect destRect;
	Point destPoint;

	// output members
	Point drawSource;
	Point drawDest;
	int drawWidth;
	int drawHeight;

	bool calcClip() {
		Common::Rect s;

		// Adjust the rect to draw to its screen coordinates
		s = sourceRect;
		s.left += destPoint.x;
		s.right += destPoint.x;
		s.top += destPoint.y;
		s.bottom += destPoint.y;

		s.clip(destRect);

		if ((s.width() <= 0) || (s.height() <= 0)) {
			return false;
		}

		drawSource.x = s.left - sourceRect.left - destPoint.x;
		drawSource.y = s.top - sourceRect.top - destPoint.y;
		drawDest.x = s.left;
		drawDest.y = s.top;
		drawWidth = s.width();
		drawHeight = s.height();

		return true;
	}
};

#include "common/pack-start.h"	// START STRUCT PACKING

struct PalEntry {
	byte red;
	byte green;
	byte blue;
} PACKED_STRUCT;

#include "common/pack-end.h"	// END STRUCT PACKING

struct Color {
	int red;
	int green;
	int blue;
	int alpha;
};

struct Surface : Graphics::Surface {

	void transitionDissolve(const byte *sourceBuffer, const Common::Rect &sourceRect, int flags, double percent);
	void drawPalette();
	void drawPolyLine(const Point *points, int count, int color);
	void blit(const Common::Rect &destRect, const byte *sourceBuffer);

	void getRect(Common::Rect &rect) {
		rect.left = rect.top = 0;
		rect.right = w;
		rect.bottom = h;
	}

	void drawRect(const Common::Rect &destRect, int color) {
		Common::Rect rect(w , h);
		rect.clip(destRect);

		if (rect.isValidRect()) {
			fillRect(rect, color);
		}
	}

	void clearRect2x(Common::Rect r) {
	// This clears a 2x scaled rect (used for Japanese font removal).
	// The pixels buffer only gets allocated for game versions that actually require it.
		if (!pixels)
			return;
		fillRect(Common::Rect(r.left << 1, r.top << 1, r.right << 1, r.bottom << 1), 0);
	}
};

#define PAL_ENTRIES 256

#define CURSOR_W 7
#define CURSOR_H 7
#define CURSOR_PC98_W 16
#define CURSOR_PC98_H 16

#define CURSOR_ORIGIN_X 4
#define CURSOR_ORIGIN_Y 4

bool hitTestPoly(const Point *points, unsigned int npoints, const Point& test_point);
class SagaEngine;

class Gfx {
public:

	Gfx(SagaEngine *vm, OSystem *system, int width, int height);
	~Gfx();

	void initPalette();
	void setPalette(const PalEntry *pal, bool full = false);
	void loadECSExtraPalettes();
	void setPaletteColor(int n, int r, int g, int b);
	void getCurrentPal(PalEntry *src_pal);
	void savePalette() { getCurrentPal(_savedPalette); }
	void restorePalette() { setPalette(_savedPalette, true); }
	void palToBlack(PalEntry *src_pal, double percent);
	void blackToPal(PalEntry *src_pal, double percent);
	void palFade(PalEntry *srcPal, int16 from, int16 to, int16 start, int16 numColors, double percent);
	void showCursor(bool state);
	void setCursor(CursorType cursorType = kCursorNormal);

	// Back buffer access methods. These all take care of adding the necessary dirty rectangles
	// APART FROM setPixelColor() and getBackBufferPixels()

	// This method adds a dirty rectangle automatically
	void drawFrame(const Common::Point &p1, const Common::Point &p2, int color);

	// This method adds a dirty rectangle automatically
	void drawRect(const Common::Rect &destRect, int color);

	// This method adds a dirty rectangle automatically
	void fillRect(const Common::Rect &destRect, uint32 color);

	// This method adds a dirty rectangle automatically
	void drawRegion(const Common::Rect &destRect, const byte *sourceBuffer);

	// This method does not add a dirty rectangle automatically
	void drawBgRegion(const Common::Rect &destRect, const byte *sourceBuffer);

	// Used for testing
	void drawPalette() {
		_backBuffer.drawPalette();
	}

	// WARNING: This method does not add a dirty rectangle automatically.
	// Whenever it gets called, the corresponding caller must take care
	// to add the corresponding dirty rectangle itself
	void hLine(int x, int y, int x2, uint32 color) {
		_backBuffer.hLine(x, y, x2, color);
		// Clear corresponding area of the sjis text layer (if the pixels buffer was actually created)
		_sjisBackBuffer.clearRect2x(Common::Rect(x, y, x2, y + 1));
	}

	// WARNING: This method does not add a dirty rectangle automatically.
	// Whenever it gets called, the corresponding caller must take care
	// to add the corresponding dirty rectangle itself
	void vLine(int x, int y, int y2, uint32 color) {
		_backBuffer.vLine(x, y, y2, color);
		// Clear corresponding area of the sjis text layer (if the pixels buffer was actually created)
		_sjisBackBuffer.clearRect2x(Common::Rect(x, y, x + 1, y2));
	}

	// WARNING: This method does not add a dirty rectangle automatically.
	// Whenever it gets called, the corresponding caller must take care
	// to add the corresponding dirty rectangle itself
	void setPixelColor(int x, int y, byte color) {
		((byte *)_backBuffer.getBasePtr(x, y))[0] = color;
		// Clear corresponding area of the sjis text layer (if the pixels buffer was actually created)
		if (_sjisBackBuffer.getPixels()) {
			*((uint16 *)_sjisBackBuffer.getBasePtr(x << 1, y << 1)) = 0;
			*((uint16 *)_sjisBackBuffer.getBasePtr(x << 1, (y << 1) + 1)) = 0;
		}
	}

	// WARNING: This method does not add a dirty rectangle automatically.
	// Whenever it gets called, the corresponding caller must take care
	// to add the corresponding dirty rectangle itself
	void drawPolyLine(const Common::Point *points, int count, int color) {
		_backBuffer.drawPolyLine(points, count, color);
	}

	// WARNING: This method allows direct modification of the back buffer
	// Whenever it gets called, the corresponding caller must take care
	// to add the corresponding dirty rectangle itself
	byte *getBackBufferPixels() {
		return (byte *)_backBuffer.getPixels();
	}

	// Same as getBackBufferPixels(), but for the hires sjis buffer
	byte *getSJISBackBufferPixels() {
		return (byte *)_sjisBackBuffer.getPixels();
	}

	// Expose the sjis buffer directly. One of the two implementations of Graphics::FontSJIS::drawChar()
	// allows a Common::Surface as a parameter which makes the rendering a bit nicer compared to using
	// the raw pixel buffer.
	Surface &getSJISBackBuffer() {
		return _sjisBackBuffer;
	}

	uint16 getBackBufferWidth() {
		return _backBuffer.w;
	}

	uint16 getSJISBackBufferWidth() {
		return _sjisBackBuffer.w;
	}

	uint16 getBackBufferHeight() {
		return _backBuffer.h;
	}

	uint16 getSJISBackBufferHeight() {
		return _sjisBackBuffer.h;
	}

	uint16 getBackBufferPitch() {
		return _backBuffer.pitch;
	}

	uint16 getSJISBackBufferPitch() {
		return _sjisBackBuffer.pitch;
	}

	void getBackBufferRect(Common::Rect &rect) {
		_backBuffer.getRect(rect);
	}

private:
	Surface _backBuffer;
	Surface _sjisBackBuffer;
	byte _currentPal[PAL_ENTRIES * 3];
	OSystem *_system;
	SagaEngine *_vm;

	PalEntry _globalPalette[PAL_ENTRIES];
	PalEntry _savedPalette[PAL_ENTRIES];
};

} // End of namespace Saga

#endif
