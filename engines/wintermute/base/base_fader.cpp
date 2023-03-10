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

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/base/base_fader.h"
#include "engines/wintermute/base/base_engine.h"
#include "engines/wintermute/base/timer.h"
#include "engines/wintermute/base/gfx/base_renderer.h"
#include "common/util.h"

namespace Wintermute {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_PERSISTENT(BaseFader, false)

//////////////////////////////////////////////////////////////////////////
BaseFader::BaseFader(BaseGame *inGame) : BaseObject(inGame) {
	_active = false;
	_red = _green = _blue = 0;
	_currentAlpha = 0x00;
	_sourceAlpha = 0;
	_targetAlpha = 0;
	_duration = 1000;
	_startTime = 0;
	_system = false;
}


//////////////////////////////////////////////////////////////////////////
BaseFader::~BaseFader() {

}


//////////////////////////////////////////////////////////////////////////
bool BaseFader::update() {
	if (!_active) {
		return STATUS_OK;
	}

	int alphaDelta = _targetAlpha - _sourceAlpha;

	uint32 time;

	if (_system) {
		time = g_system->getMillis() - _startTime;
	} else {
		time = BaseEngine::getTimer()->getTime() - _startTime;
	}

	if (time >= _duration) {
		_currentAlpha = _targetAlpha;
	} else {
		_currentAlpha = (byte)(_sourceAlpha + (float)time / (float)_duration * alphaDelta);
	}
	_currentAlpha = MIN((byte)255, MAX(_currentAlpha, (byte)0));

	_ready = time >= _duration;
	if (_ready && _currentAlpha == 0x00) {
		_active = false;
	}

	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
bool BaseFader::display() {
	if (!_active) {
		return STATUS_OK;
	}

	if (_currentAlpha > 0x00) {
		BaseEngine::getRenderer()->fadeToColor(_red, _green, _blue, _currentAlpha);
	}
	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
bool BaseFader::deactivate() {
	_active = false;
	_ready = true;
	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
bool BaseFader::fadeIn(uint32 sourceColor, uint32 duration, bool system) {
	_ready = false;
	_active = true;

	_red   = RGBCOLGetR(sourceColor);
	_green = RGBCOLGetG(sourceColor);
	_blue  = RGBCOLGetB(sourceColor);

	_sourceAlpha = RGBCOLGetA(sourceColor);
	_targetAlpha = 0;

	_duration = duration;
	_system = system;

	if (_system) {
		_startTime = g_system->getMillis();
	} else {
		_startTime = BaseEngine::getTimer()->getTime();
	}

	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
bool BaseFader::fadeOut(uint32 targetColor, uint32 duration, bool system) {
	_ready = false;
	_active = true;

	_red   = RGBCOLGetR(targetColor);
	_green = RGBCOLGetG(targetColor);
	_blue  = RGBCOLGetB(targetColor);

	//_sourceAlpha = 0;
	_sourceAlpha = _currentAlpha;
	_targetAlpha = RGBCOLGetA(targetColor);

	_duration = duration;
	_system = system;

	if (_system) {
		_startTime = g_system->getMillis();
	} else {
		_startTime = BaseEngine::getTimer()->getTime();
	}


	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
uint32 BaseFader::getCurrentColor() const {
	return BYTETORGBA(_red, _green, _blue, _currentAlpha);
}



//////////////////////////////////////////////////////////////////////////
bool BaseFader::persist(BasePersistenceManager *persistMgr) {
	BaseObject::persist(persistMgr);

	persistMgr->transferBool(TMEMBER(_active));
	persistMgr->transferByte(TMEMBER(_blue));
	persistMgr->transferByte(TMEMBER(_currentAlpha));
	persistMgr->transferUint32(TMEMBER(_duration));
	persistMgr->transferByte(TMEMBER(_green));
	persistMgr->transferByte(TMEMBER(_red));
	persistMgr->transferByte(TMEMBER(_sourceAlpha));
	persistMgr->transferUint32(TMEMBER(_startTime));
	persistMgr->transferByte(TMEMBER(_targetAlpha));
	persistMgr->transferBool(TMEMBER(_system));

	if (_system && !persistMgr->getIsSaving()) {
		_startTime = 0;
	}

	return STATUS_OK;
}

} // End of namespace Wintermute
