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

#include "engines/wintermute/base/base_persistence_manager.h"
#include "engines/wintermute/base/file/base_save_thumb_file.h"

namespace Wintermute {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
BaseSaveThumbFile::BaseSaveThumbFile() {
	_data = nullptr;
}


//////////////////////////////////////////////////////////////////////////
BaseSaveThumbFile::~BaseSaveThumbFile() {
	close();
}


//////////////////////////////////////////////////////////////////////////
bool BaseSaveThumbFile::open(const Common::String &filename) {
	close();

	if (scumm_strnicmp(filename.c_str(), "savegame:", 9) != 0) {
		return STATUS_FAILED;
	}

	size_t filenameSize = strlen(filename.c_str()) - 9 + 1;
	char *tempFilename = new char[filenameSize];
	Common::strcpy_s(tempFilename, filenameSize, filename.c_str() + 9);
	for (uint32 i = 0; i < strlen(tempFilename); i++) {
		if (tempFilename[i] < '0' || tempFilename[i] > '9') {
			tempFilename[i] = '\0';
			break;
		}
	}

	// get slot number from name
	int slot = atoi(tempFilename);
	delete[] tempFilename;

	BasePersistenceManager *pm = new BasePersistenceManager();
	if (!pm) {
		return STATUS_FAILED;
	}

	Common::String slotFilename = pm->getFilenameForSlot(slot);

	if (DID_FAIL(pm->initLoad(slotFilename))) {
		delete pm;
		return STATUS_FAILED;
	}

	bool res;

	if (pm->_thumbnailDataSize != 0) {
		_data = new byte[pm->_thumbnailDataSize];
		memcpy(_data, pm->_thumbnailData, pm->_thumbnailDataSize);
		_size = pm->_thumbnailDataSize;
		res = STATUS_OK;
	} else {
		res = STATUS_FAILED;
	}
	delete pm;

	return res;
}


//////////////////////////////////////////////////////////////////////////
bool BaseSaveThumbFile::close() {
	delete[] _data;
	_data = nullptr;

	_pos = 0;
	_size = 0;

	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
bool BaseSaveThumbFile::read(void *buffer, uint32 size) {
	if (!_data || _pos + size > _size) {
		return STATUS_FAILED;
	}

	memcpy(buffer, (byte *)_data + _pos, size);
	_pos += size;

	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
bool BaseSaveThumbFile::seek(uint32 pos, int whence) {
	if (!_data) {
		return STATUS_FAILED;
	}

	uint32 newPos = 0;

	switch (whence) {
	case SEEK_SET:
	default:
		newPos = pos;
		break;
	case SEEK_END:
		newPos = _size + pos;
		break;
	case SEEK_CUR:
		newPos = _pos + pos;
		break;
	}

	if (newPos > _size) {
		return STATUS_FAILED;
	} else {
		_pos = newPos;
	}

	return STATUS_OK;
}

} // End of namespace Wintermute
