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


#include "common/system.h"

#include "parallaction/parallaction.h"
#include "parallaction/debug.h"

namespace Parallaction {


Debugger::Debugger(Parallaction *vm)
	: GUI::Debugger() {
	_vm = vm;
	_mouseState = MOUSE_ENABLED_SHOW;

	registerCmd("continue",	WRAP_METHOD(Debugger, cmdExit));
	registerCmd("location",	WRAP_METHOD(Debugger, Cmd_Location));
	registerCmd("give",		WRAP_METHOD(Debugger, Cmd_Give));
	registerCmd("zones",		WRAP_METHOD(Debugger, Cmd_Zones));
	registerCmd("animations",	WRAP_METHOD(Debugger, Cmd_Animations));
	registerCmd("globalflags",WRAP_METHOD(Debugger, Cmd_GlobalFlags));
	registerCmd("toggleglobalflag",WRAP_METHOD(Debugger, Cmd_ToggleGlobalFlag));
	registerCmd("localflags",	WRAP_METHOD(Debugger, Cmd_LocalFlags));
	registerCmd("locations",	WRAP_METHOD(Debugger, Cmd_Locations));
	registerCmd("gfxobjects",	WRAP_METHOD(Debugger, Cmd_GfxObjects));
	registerCmd("programs",	WRAP_METHOD(Debugger, Cmd_Programs));
	registerCmd("showmouse",	WRAP_METHOD(Debugger, Cmd_ShowMouse));
}


void Debugger::preEnter() {
	_mouseState = _vm->_input->getMouseState();
	GUI::Debugger::preEnter();
}


void Debugger::postEnter() {
	GUI::Debugger::postEnter();
	_vm->_input->setMouseState(_mouseState);
	_vm->_input->setArrowCursor();	// unselects the active item, if any
}

bool Debugger::Cmd_Location(int argc, const char **argv) {

	const char *character; // = _vm->_char.getName();
	const char *location; // = _vm->_location._name;

	char tmp[PATH_LEN];

	switch (argc) {
	case 3:
		character = const_cast<char *>(argv[2]);
		location = const_cast<char *>(argv[1]);
		Common::sprintf_s(tmp, "%s.%s", location, character);
		_vm->scheduleLocationSwitch(tmp);
		break;

	case 2:
		location = const_cast<char *>(argv[1]);
		_vm->scheduleLocationSwitch(location);
		break;

	case 1:
		debugPrintf("location <location name> [character name]\n");
		break;

	default:
		break;
	}

	return true;
}

bool Debugger::Cmd_Locations(int argc, const char **argv) {

	debugPrintf("+------------------------------+---------+\n"
				"| location name                |  flags  |\n"
				"+------------------------------+---------+\n");
	for (uint i = 0; i < _vm->_numLocations; i++) {
		debugPrintf("|%-30s| %08x|\n", _vm->_locationNames[i], _vm->_localFlags[i]);
	}
	debugPrintf("+------------------------------+---------+\n");

	return true;
}

bool Debugger::Cmd_GlobalFlags(int argc, const char **argv) {

	uint32 flags = g_globalFlags;

	debugPrintf("+------------------------------+---------+\n"
				"| flag name                    |  value  |\n"
				"+------------------------------+---------+\n");
	for (uint i = 0; i < _vm->_globalFlagsNames->count(); i++) {
		const char *value = ((flags & (1 << i)) == 0) ? "OFF" : "ON";
		debugPrintf("|%-30s|   %-6s|\n", _vm->_globalFlagsNames->item(i),  value);
	}
	debugPrintf("+------------------------------+---------+\n");

	return true;
}

bool Debugger::Cmd_ToggleGlobalFlag(int argc, const char **argv) {

	int i;

	switch (argc) {
	case 2:
		i = _vm->_globalFlagsNames->lookup(argv[1]);
		if (i == Table::notFound) {
			debugPrintf("invalid flag '%s'\n", argv[1]);
		} else {
			i--;
			if ((g_globalFlags & (1 << i)) == 0)
				g_globalFlags |= (1 << i);
			else
				g_globalFlags &= ~(1 << i);
		}
		break;

	default:
		debugPrintf("toggleglobalflag <flag name>\n");

	}

	return true;
}

bool Debugger::Cmd_LocalFlags(int argc, const char **argv) {

	uint32 flags = _vm->getLocationFlags();

	debugPrintf("+------------------------------+---------+\n"
				"| flag name                    |  value  |\n"
				"+------------------------------+---------+\n");
	for (uint i = 0; i < _vm->_localFlagNames->count(); i++) {
		const char *value = ((flags & (1 << i)) == 0) ? "OFF" : "ON";
		debugPrintf("|%-30s|   %-6s|\n", _vm->_localFlagNames->item(i),  value);
	}
	debugPrintf("+------------------------------+---------+\n");

	return true;
}

bool Debugger::Cmd_Give(int argc, const char **argv) {

	if (argc == 1) {
		debugPrintf("give <item name>\n");
	} else {
		int index = _vm->_objectsNames->lookup(argv[1]);
		if (index != Table::notFound)
			_vm->addInventoryItem(index + 4);
		else
			debugPrintf("invalid item name '%s'\n", argv[1]);
	}

	return true;
}


bool Debugger::Cmd_Zones(int argc, const char **argv) {

	ZoneList::iterator b = _vm->_location._zones.begin();
	ZoneList::iterator e = _vm->_location._zones.end();
	Common::Rect r;

	debugPrintf("+--------------------+---+---+---+---+--------+--------+\n"
				"| name               | l | t | r | b |  type  |  flag  |\n"
				"+--------------------+---+---+---+---+--------+--------+\n");
	for ( ; b != e; ++b) {
		ZonePtr z = *b;
		z->getRect(r);
		debugPrintf("|%-20s|%3i|%3i|%3i|%3i|%8x|%8x|\n", z->_name, r.left, r.top, r.right, r.bottom, z->_type, z->_flags );
	}
	debugPrintf("+--------------------+---+---+---+---+--------+--------+\n");


	return true;
}

Common::String Debugger::decodeZoneFlags(uint32 flags) {
	const char *descs[33] = {
		"none",     // 0
		"closed",   // 1
		"active",   // 2
		"remove",   // 4
		"acting",   // 8
		"locked",   // 0x10
		"fixed",    // 0x20
		"noname",   // 0x40
		"nomasked", // 0x80
		"looping",  // 0x100
		"added",    // 0x200
		"character",// 0x400
		"nowalk",   // 0x800
		"yourself", // 0x1000
		"scaled",   // 0x2000
		"selfuse",  // 0x4000
		"0x8000",   // 0x8000
		"0x10000",
		"0x20000",
		"0x40000",
		"0x80000",
		"0x100000",
		"0x200000",
		"0x400000",
		"0x800000",
		"isanimation",  // 0x1000000
		"animlinked",    // 0x2000000
		"0x4000000",
		"0x8000000",
		"0x10000000",
		"0x20000000",
		"0x40000000",
		"0x80000000"
	};

	uint32 mask = 1;
	const char *matches[32];
	uint numMatches = 0;
	for (uint32 i = 1; i < 32; i++, mask<<=1) {
		if (flags & mask) {
			matches[numMatches] = descs[i];
			numMatches++;
		}
	}
	if (numMatches == 0) {
		matches[0] = descs[0];
		numMatches = 1;
	}

	Common::String s(matches[0]);
	for (uint32 j = 1; j < numMatches; j++) {
		s += '+';
		s += matches[j];
	}
	return s;
}

bool Debugger::Cmd_Animations(int argc, const char **argv) {

	AnimationList::iterator b = _vm->_location._animations.begin();
	AnimationList::iterator e = _vm->_location._animations.end();
	Common::String flags;

	debugPrintf("+--------------------+----+----+----+---+--------+----------------------------------------+\n"
				"| name               | x  | y  | z  | f |  type  |                 flags                  | \n"
				"+--------------------+----+----+----+---+--------+----------------------------------------+\n");
	for ( ; b != e; ++b) {
		AnimationPtr a = *b;
		flags = decodeZoneFlags(a->_flags);
		debugPrintf("|%-20s|%4i|%4i|%4i|%3i|%8x|%-40s|\n", a->_name, a->getX(), a->getY(), a->getZ(), a->getF(), a->_type, flags.c_str() );
	}
	debugPrintf("+--------------------+---+---+---+---+--------+----------------------------------------+\n");


	return true;
}

bool Debugger::Cmd_GfxObjects(int argc, const char **argv) {

	const char *objType[] = { "DOOR", "GET", "ANIM" };

	debugPrintf("+--------------------+-----+-----+-----+-----+-----+-------+-----+--------+\n"
				"| name               |  x  |  y  |  w  |  h  |  z  | layer |  f  |  type  |\n"
				"+--------------------+-----+-----+-----+-----+-----+-------+-----+--------+\n");

	GfxObjArray::iterator b = _vm->_gfx->_sceneObjects.begin();
	GfxObjArray::iterator e = _vm->_gfx->_sceneObjects.end();
	Common::Rect r;

	for ( ; b != e; ++b) {
		GfxObj *obj = *b;
		obj->getRect(obj->frame, r);
		debugPrintf("|%-20s|%5i|%5i|%5i|%5i|%5i|%7i|%5i|%8s|\n", obj->getName(), r.left, r.top, r.width(), r.height(),
			obj->z, obj->layer, obj->frame, objType[obj->type]);
	}

	debugPrintf("+--------------------+-----+-----+-----+-----+-----+-------+-----+--------+\n");

	return true;
}

bool Debugger::Cmd_Programs(int argc, const char** argv) {

	ProgramList::iterator b = _vm->_location._programs.begin();
	ProgramList::iterator e = _vm->_location._programs.end();

	const char *status[] = { "idle", "running", "completed" };

	int i = 1;

	debugPrintf("+---+--------------------+--------+----------+\n"
				"| # | bound animation    |  size  |  status  |\n"
				"+---+--------------------+--------+----------+\n");
	for ( ; b != e; b++, i++) {
		ProgramPtr p = *b;
		debugPrintf("|%3i|%-20s|%8i|%-10s|\n", i, p->_anim->_name, p->_instructions.size(), status[p->_status] );
	}
	debugPrintf("+---+--------------------+--------+----------+\n");

	return true;
}

bool Debugger::Cmd_ShowMouse(int argc, const char** argv) {
	_mouseState = MOUSE_ENABLED_SHOW;
	return true;
}

} // namespace Parallaction
