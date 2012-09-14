/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef NEVERHOOD_GRAPHICS_H
#define NEVERHOOD_GRAPHICS_H

#include "common/array.h"
#include "common/file.h"
#include "graphics/surface.h"
#include "neverhood/neverhood.h"

namespace Neverhood {

struct NPoint {
	int16 x, y;
};

typedef Common::Array<NPoint> NPointArray;

struct NDimensions {
	int16 width, height;
};

struct NRect {
	int16 x1, y1, x2, y2;

	NRect() : x1(0), y1(0), x2(0), y2(0) {}

	NRect(int16 x01, int16 y01, int16 x02, int16 y02) : x1(x01), y1(y01), x2(x02), y2(y02) {}

	void set(int16 x01, int16 y01, int16 x02, int16 y02) {
		x1 = x01;
		y1 = y01;
		x2 = x02;
		y2 = y02;
	}

	bool contains(int16 x, int16 y) const {
		return x >= x1 && x <= x2 && y >= y1 && y <= y2;
	}

};

typedef Common::Array<NRect> NRectArray;

// TODO: Use Common::Rect 
struct NDrawRect {
	int16 x, y, width, height;
	NDrawRect() : x(0), y(0), width(0), height(0) {}
	NDrawRect(int16 x0, int16 y0, int16 width0, int16 height0) : x(x0), y(y0), width(width0), height(height0) {}
	int16 x2() { return x + width; } 
	int16 y2() { return y + height; }
	void set(int16 x0, int16 y0, int16 width0, int16 height0) {
		x = x0;
		y = y0;
		width = width0;
		height = height0;
	}
};

class AnimResource;
class SpriteResource;
class MouseCursorResource;

// NOTE: "Restore" methods aren't need in the reimplementation as they're DirectDraw-specific

class BaseSurface {
public:
	BaseSurface(NeverhoodEngine *vm, int priority, int16 width, int16 height);
	virtual ~BaseSurface();
	virtual void draw();
	virtual void addDirtyRect();
	void clear();
	void drawSpriteResource(SpriteResource &spriteResource);
	void drawSpriteResourceEx(SpriteResource &spriteResource, bool flipX, bool flipY, int16 width, int16 height);
	void drawAnimResource(AnimResource &animResource, uint frameIndex, bool flipX, bool flipY, int16 width, int16 height);
	void drawMouseCursorResource(MouseCursorResource &mouseCursorResource, int frameNum);
	void copyFrom(Graphics::Surface *sourceSurface, int16 x, int16 y, NDrawRect &sourceRect, bool transparent);
	int getPriority() const { return _priority; }
	void setPriority(int priority) { _priority = priority; }
	NDrawRect& getDrawRect() { return _drawRect; }
	NDrawRect& getSysRect() { return _sysRect; }
	NRect& getClipRect() { return _clipRect; }
	void setClipRect(NRect clipRect) { _clipRect = clipRect; }
	bool getVisible() const { return _visible; }
	void setVisible(bool value) { _visible = value; }
	void setTransparent(bool value) { _transparent = value; }
	Graphics::Surface *getSurface() { return _surface; }
protected:
	NeverhoodEngine *_vm;
	int _priority;
	bool _visible;
	Graphics::Surface *_surface;
	NDrawRect _drawRect;
	NDrawRect _sysRect;
	NRect _clipRect;
	bool _transparent;
};

class ShadowSurface : public BaseSurface {
public:
	ShadowSurface(NeverhoodEngine *vm, int priority, int16 width, int16 height, BaseSurface *shadowSurface);
	virtual void draw();
protected:
	BaseSurface *_shadowSurface;
};

class FontSurface : public BaseSurface {
public:
	FontSurface(NeverhoodEngine *vm, NPointArray *tracking, uint16 numRows, byte firstChar, uint16 charWidth, uint16 charHeight);
	void drawChar(BaseSurface *destSurface, int16 x, int16 y, byte chr);
	void drawString(BaseSurface *destSurface, int16 x, int16 y, const byte *string);
protected:
	NPointArray *_tracking;
	uint16 _numRows;
	byte _firstChar;
	uint16 _charWidth;
	uint16 _charHeight;
};

// Misc

void parseBitmapResource(byte *sprite, bool *rle, NDimensions *dimensions, NPoint *position, byte **palette, byte **pixels);
void unpackSpriteRle(byte *source, int width, int height, byte *dest, int destPitch, bool flipX, bool flipY);
void unpackSpriteRleRepl(byte *source, int width, int height, byte *dest, int destPitch, byte oldColor, byte newColor, bool flipX, bool flipY);
void unpackSpriteNormal(byte *source, int width, int height, byte *dest, int destPitch, bool flipX, bool flipY);
int calcDistance(int16 x1, int16 y1, int16 x2, int16 y2);

} // End of namespace Neverhood

#endif /* NEVERHOOD_GRAPHICS_H */
