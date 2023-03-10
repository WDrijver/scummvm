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

#include "tinsel/noir/notebook_page.h"
#include "tinsel/dialogs.h"
#include "tinsel/dw.h"
#include "tinsel/film.h"
#include "tinsel/handle.h"
#include "tinsel/multiobj.h"
#include "tinsel/polygons.h"
#include "tinsel/timers.h"
#include "tinsel/tinsel.h"
#include "tinsel/noir/sysreel.h"

namespace Tinsel {

void NotebookLine::clear() {
	MultiDeleteObjectIfExists(FIELD_STATUS, &_obj);
}

bool HasReelFrame(SCNHANDLE pReel) {
	if (pReel) {
		const FREEL* reel = (const FREEL*)_vm->_handle->LockMem(pReel);
		if (reel && reel->mobj) {
			const MULTI_INIT* pmi = reel->GetMultiInit();
			if (pmi) {
				return pmi->GetFrame() != nullptr;
			}
		}
	}
	return false;
}

int FindReelIndexForEntry(const FILM *pFilm, int pageLine) {
	if (HasReelFrame(pFilm->reels[pageLine].mobj)) {
		return pageLine;
	}
	for (int i = pageLine; i < pFilm->numreels; i++) {
		if (HasReelFrame(pFilm->reels[i].mobj)) {
			return i;
		}
	}
	for (int i = pageLine; i >= 0; i--) {
		if (HasReelFrame(pFilm->reels[i].mobj)) {
			return i;
		}
	}
	return -1;
}

void NotebookLine::fillIn(int pageLine) {
	const FILM *pFilm = _vm->_dialogs->getObjectFilm(_id);
	if (!pFilm)
		return;

	int reelIndex = FindReelIndexForEntry(pFilm, pageLine);
	assert(reelIndex >= 0);
	const FREEL *reel = &pFilm->reels[reelIndex];
	MultiDeleteObjectIfExists(FIELD_STATUS, &_obj);
	_obj = InsertReelObj(reel);

	MultiSetZPosition(_obj, 17);
	InitStepAnimScript(&_anim, _obj, pFilm->reels[reelIndex].script, ONE_SECOND / pFilm->frate);

	if (_crossedOut) {
		auto scribbleFilm = GetSystemReelFilm(SysReel::SCRIBBLES);
		_scribbles = InsertReelObj(&scribbleFilm->reels[reelIndex]);
		MultiSetZPosition(_scribbles, 18);

		InitStepAnimScript(&_scribbleAnim, _scribbles, scribbleFilm->reels[reelIndex].script, ONE_SECOND / pFilm->frate);
	}
}

void NotebookLine::crossOut() {
	_crossedOut = true;
}

void NotebookPage::handlePointAtLine(int line) {
	auto objId = getClueForLine(line);
	if (objId != 0 && objId != _pointedClue) {
		auto obj = _vm->_dialogs->getInvObject(objId);
		_vm->_dialogs->invPointEvent(obj, -1);
		_pointedClue = objId;
	}
}

int NotebookPage::indexOfClue(int id) const {
	for (uint32 i = 0; i < _numLines; i++) {
		if (_lines[i]._id == id) {
			return i;
		}
	}
	return -1;
}

bool NotebookPage::containsClue(int id) {
	return indexOfClue(id) != -1;
}

void NotebookPage::crossClue(int id) {
	int index = indexOfClue(id);
	assert(index != -1);
	_lines[index].crossOut();
}

void NotebookPage::addLine(int id) {
	if (containsClue(id)) {
		return;
	}
	assert(_numLines < MAX_ENTRIES_PER_PAGE);
	_lines[_numLines++]._id = id;
}

void NotebookPage::setTitle(int id) {
	_lines[0]._id = id;
	if (_numLines == 0) {
		_numLines++;
	}
}
int32 NotebookPage::getTitle() const {
	return _lines[0]._id;
}

void NotebookPage::fillIn() {
	for (uint32 i = 0; i < _numLines; i++) {
		_lines[i].fillIn(i);
	}
}

void NotebookPage::clear() {
	for (uint32 i = 0; i < _numLines; i++) {
		_lines[i].clear();
	}
	_pointedClue = -1;
}

int NotebookPage::getClueForLine(int line) const {
	if (line >= (int)_numLines) {
		return 0;
	}
	return _lines[line]._id;
}

} // End of namespace Tinsel
