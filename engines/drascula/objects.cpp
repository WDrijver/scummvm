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

#include "common/textconsole.h"

#include "drascula/drascula.h"

namespace Drascula {

void DrasculaEngine::pickObject(int object) {
	if (currentChapter == 6)
		loadPic("iconsp.alg", backSurface);
	else if (currentChapter == 4)
		loadPic("icons2.alg", backSurface);
	else if (currentChapter == 5)
		loadPic("icons3.alg", backSurface);
	else
		loadPic("icons.alg", backSurface);
	chooseObject(object);
	if (currentChapter == 2)
		loadPic(menuBackground, backSurface);
	else
		loadPic(99, backSurface);
}

void DrasculaEngine::chooseObject(int object) {
	if (currentChapter == 5) {
		if (takeObject == 1 && !_menuScreen && pickedObject != 16)
			addObject(pickedObject);
	} else {
		if (takeObject == 1 && !_menuScreen)
			addObject(pickedObject);
	}
	for (int i = 0; i < OBJHEIGHT; i++)
		memcpy(mouseCursor + i * OBJWIDTH, backSurface + _x1d_menu[object] + (_y1d_menu[object] + i) * 320, OBJWIDTH);
	setCursor(kCursorCurrentItem);
	takeObject = 1;
	pickedObject = object;
}

void DrasculaEngine::walkToPoint(Common::Point pos) {
	bool cursorVisible = isCursorVisible();
	hideCursor();

	if (currentChapter == 5 || currentChapter == 6) {
		if (!_characterVisible) {
			curX = roomX;
			curY = roomY;
			updateRoom();
			updateScreen();
			return;
		}
	}
	roomX = pos.x;
	roomY = pos.y;
	startWalking();

	while (!shouldQuit()) {
		updateRoom();
		updateScreen();
		updateEvents();
		if (!_characterMoved)
			break;

		pause(3);
	}

	if (_walkToObject) {
		_walkToObject = false;
		trackProtagonist = trackFinal;
	}
	updateRoom();
	updateScreen();

	// _roomNumber -2 is end credits. Do not show cursor there
	if (cursorVisible && _roomNumber != -2)
		showCursor();
}

void DrasculaEngine::checkObjects() {
	int l;

	_hasName = false;

	for (l = 0; l < numRoomObjs; l++) {
		if (_objectRect[l].contains(Common::Point(_mouseX, _mouseY)) && visible[l] == 1 && isDoor[l] == 0) {
			Common::strcpy_s(textName, objName[l]);
			_hasName = true;
		}
	}

	if (_mouseX > curX + 2 && _mouseY > curY + 2
			&& _mouseX < curX + curWidth - 2 && _mouseY < curY + curHeight - 2) {
		if (currentChapter == 2 || !_hasName) {
			Common::strcpy_s(textName, _textmisc[3]); // "hacker"
			_hasName = true;
		}
	}
}

/**
 * Remove an item from the inventory, namely the item in the slot
 * over which the mouse cursor currently hovers.
 */
void DrasculaEngine::removeObject() {
	int obj = 0, n;

	updateRoom();

	n = whichObject();
	if (n != 0) {
		obj = inventoryObjects[n];
		inventoryObjects[n] = 0;
		if (obj != 0)
			takeObject = 1;
	}

	updateEvents();

	if (takeObject == 1)
		chooseObject(obj);
}

/**
 * Remove the item with the given id from the inventory.
 * Returns 0 if was in the inventory, 1 otherwise.
 */
int DrasculaEngine::removeObject(int obj) {
	for (int n = 1; n < ARRAYSIZE(inventoryObjects); n++) {
		if (inventoryObjects[n] == obj) {
			inventoryObjects[n] = 0;
			return 0;
		}
	}

	return 1;
}


bool DrasculaEngine::pickupObject() {
	int obj = pickedObject;
	checkFlags = 1;

	updateRoom();
	showMenu();
	updateScreen();

	// Objects with an ID smaller than 7 are the inventory verbs
	if (pickedObject >= 7) {
		int n = whichObject();
		if (n != 0 && inventoryObjects[n] == 0) {
			inventoryObjects[n] = obj;
			takeObject = 0;
			checkFlags = 0;
		}
	}

	if (checkFlags == 1) {
		if (checkMenuFlags())
			return true;
	}
	updateEvents();
	if (takeObject == 0)
		selectVerb(kVerbNone);

	return false;
}

/**
 * Add the object with the given id to the inventory.
 */
void DrasculaEngine::addObject(int obj) {
	int n;

	// Check whether obj is already in the inventory.
	// If it is, just do nothing and return.
	for (n = 1; n < ARRAYSIZE(inventoryObjects); n++) {
		if (inventoryObjects[n] == obj)
			return;
	}

	// Otherwise, look for a free slot and add the object there.
	for (n = 1; n < ARRAYSIZE(inventoryObjects); n++) {
		if (inventoryObjects[n] == 0) {
			inventoryObjects[n] = obj;
			return;
		}
	}

	// If we get here, then we failed to add the object to the inventory.
	error("DrasculaEngine::addObject: Failed to add object %d to inventory", obj);
}

/**
 * Return the id of the inventory slot under the mouse cursor right now.
 * If no inventory slot is under the mouse cursor, return 0.
 */
int DrasculaEngine::whichObject() {
	int n;

	for (n = 1; n < ARRAYSIZE(inventoryObjects); n++) {
		if (_mouseX > _itemLocations[n].x && _mouseY > _itemLocations[n].y &&
			_mouseX < _itemLocations[n].x + OBJWIDTH &&
			_mouseY < _itemLocations[n].y + OBJHEIGHT) {
			return n;
		}
	}

	return 0;
}

void DrasculaEngine::updateVisible() {
	if (currentChapter == 1) {
		// nothing
	} else if (currentChapter == 2) {
		if (_roomNumber == 2 && flags[40] == 0)
			visible[3] = 0;
		else if (_roomNumber == 3 && flags[3] == 1)
			visible[8] = 0;
		else if (_roomNumber == 6 && flags[1] == 1 && flags[10] == 0) {
			visible[2] = 0;
			visible[4] = 1;
		} else if (_roomNumber == 7 && flags[35] == 1)
			visible[3] = 0;
		else if (_roomNumber == 14 && flags[5] == 1)
			visible[4] = 0;
		else if (_roomNumber == 18 && flags[28] == 1)
			visible[2] = 0;
	} else if (currentChapter == 3) {
		// nothing
	} else if (currentChapter == 4) {
		if (_roomNumber == 23 && flags[0] == 0 && flags[11] == 0)
			visible[2] = 1;
		if (_roomNumber == 23 && flags[0] == 1 && flags[11] == 0)
			visible[2] = 0;
		if (_roomNumber == 21 && flags[10] == 1)
			visible[2] = 0;
		if (_roomNumber == 22 && flags[26] == 1) {
			visible[2] = 0;
			visible[1] = 1;
		}
		if (_roomNumber == 22 && flags[27] == 1)
			visible[3] = 0;
		if (_roomNumber == 26 && flags[21] == 0)
			Common::strlcpy(objName[2], _textmisc[0], 20);
		if (_roomNumber == 26 && flags[18] == 1)
			visible[2] = 0;
		if (_roomNumber == 26 && flags[12] == 1)
			visible[1] = 0;
		if (_roomNumber == 31 && flags[13] == 1)
			visible[1] = 0;
		if (_roomNumber == 35 && flags[14] == 1)
			visible[2] = 0;
		if (_roomNumber == 35 && flags[17] == 1)
			visible[3] = 1;
		if (_roomNumber == 35 && flags[15] == 1)
			visible[1] = 0;
	} else if (currentChapter == 5) {
		if (_roomNumber == 49 && flags[6] == 1)
			visible[2] = 0;
		if (_roomNumber == 49 && flags[6] == 0)
			visible[1] = 0;
		if (_roomNumber == 49 && flags[6] == 1)
			visible[1] = 1;
		if (_roomNumber == 45 && flags[6] == 1)
			visible[3] = 1;
		if (_roomNumber == 53 && flags[2] == 1)
			visible[3] = 0;
		if (_roomNumber == 54 && flags[13] == 1)
			visible[3] = 0;
		if (_roomNumber == 55 && flags[8] == 1)
			visible[1] = 0;
	} else if (currentChapter == 6) {
		if (_roomNumber == 58 && flags[8] == 0)
			isDoor[1] = 0;
		if (_roomNumber == 58 && flags[8] == 1)
			isDoor[1] = 1;
		if (_roomNumber == 59)
			isDoor[1] = 0;
		if (_roomNumber == 60) {
			trackDrascula = 0;
			drasculaX = 155;
			drasculaY = 69;
		}
	}
}

} // End of namespace Drascula
