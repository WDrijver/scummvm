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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "ultima/ultima8/misc/pent_include.h"
#include "ultima/ultima8/filesys/file_system.h"
#include "ultima/shared/std/string.h"
#include "ultima/ultima8/ultima8.h"
#include "common/system.h"
#include "common/memstream.h"
#include "common/savefile.h"

namespace Ultima {
namespace Ultima8 {

using Std::string;

FileSystem *FileSystem::filesystem = 0;

FileSystem::FileSystem(bool noforced)
	: noforcedvpaths(noforced), allowdataoverride(true) {
	con->Print(MM_INFO, "Creating FileSystem...\n");

	filesystem = this;
	AddVirtualPath("@home", "");
}

FileSystem::~FileSystem() {
	con->Print(MM_INFO, "Destroying FileSystem...\n");

	filesystem = 0;
}


// Open a streaming file as readable. Streamed (0 on failure)
IDataSource *FileSystem::ReadFile(const string &vfn, bool is_text) {
	string filename = vfn;

	IDataSource *data = checkBuiltinData(vfn, is_text);

	// allow data-override?
	if (!allowdataoverride && data)
		return data;

	Common::SeekableReadStream *readStream;
	if (!rawOpen(readStream, filename))
		return 0;

	return new IFileDataSource(readStream);
}

// Open a streaming file as writeable. Streamed (0 on failure)
ODataSource *FileSystem::WriteFile(const string &vfn, bool is_text) {
	string filename = vfn;
	Common::WriteStream *writeStream;

	if (!rawOpen(writeStream, filename))
		return 0;

	return new OFileDataSource(writeStream);
}

bool FileSystem::rawOpen(Common::SeekableReadStream *&in, const string &fname) {
	string name = fname;
	Common::File *f;

	// Handle reading files from the ultima.dat data
	if (name.hasPrefix("@data/")) {
		// It's a file specifically from the ultima.dat file
		f = new Common::File();
		if (f->open(Common::String::format("data/%s", name.substr(6).c_str()),
			*Ultima8Engine::get_instance()->getDataArchive())) {
			in = f;
			return true;
		}

		f->close();
		delete f;
	}

	// Handle opening savegames
	if (name.hasPrefix("@save/")) {
		int slotNumber = Std::atoi(name.c_str() + 6);
		Std::string saveFilename = Ultima8Engine::get_instance()->getSaveFilename(slotNumber);

		in = g_system->getSavefileManager()->openForLoading(saveFilename);
		return in != 0;
	}

	if (!rewrite_virtual_path(name))
		return false;

	switch_slashes(name);

	int uppercasecount = 0;
	f = new Common::File();
	do {
		if (f->open(name)) {
			in = f;
			return true;
		}
	} while (base_to_uppercase(name, ++uppercasecount));

	// file not found
	delete f;
	return false;
}


bool FileSystem::rawOpen(Common::WriteStream *&out,  const string &fname) {
	string name = fname;
	switch_slashes(name);

	if (name.hasPrefix("@save/")) {
		int slotNumber = Std::atoi(name.c_str() + 6);
		Std::string saveFilename = Ultima8Engine::get_instance()->getSaveFilename(slotNumber);

		out = g_system->getSavefileManager()->openForSaving(saveFilename, false);
		return out != 0;
	} else {
		return false;
	}

#if 0
	if (!rewrite_virtual_path(name)) {
		con->Print_err(MM_MAJOR_WARN, "Illegal file access\n");
		return false;
	}

	int uppercasecount = 0;
	do {
		if (out.open(name))
			return true;

	} while (base_to_uppercase(name, ++uppercasecount));

	// file not found
	return false;
#endif
}

void FileSystem::switch_slashes(string &name) {
	for (string::iterator X = name.begin(); X != name.end(); ++X) {
		if (*X == '\\')
			*X =  '/';
	}
}

/*
 *  Convert just the last 'count' parts of a filename to uppercase.
 *  returns false if there are less than 'count' parts
 */

bool FileSystem::base_to_uppercase(string &str, int count) {
	if (count <= 0) return true;

	int todo = count;
	// Go backwards.
	string::reverse_iterator X;
	for (X = str.rbegin(); X != str.rend(); ++X) {
		// Stop at separator.
		if (*X == '/' || *X == '\\' || *X == ':')
			todo--;
		if (todo <= 0)
			break;

		*X = static_cast<char>(Std::toUpper(*X));
	}
	if (X == str.rend())
		todo--; // start of pathname counts as separator too

	// false if it didn't reach 'count' parts
	return (todo <= 0);
}

bool FileSystem::AddVirtualPath(const string &vpath, const string &realpath, const bool create) {
	string vp = vpath, rp = realpath;

	// remove trailing slash
	if (vp.rfind('/') == vp.size() - 1)
		vp.erase(vp.rfind('/'));

	if (rp.rfind('/') == rp.size() - 1)
		rp.erase(rp.rfind('/'));

	if (rp.find("..") != string::npos) {
		con->Printf_err(MM_MINOR_ERR,
		               "Error mounting virtual path \"%s\": \"..\" not allowed.\n",
		               vp.c_str());
		return false;
	}

	// Finding Reserved Virtual Path Names
	// memory path is reserved
	if (vp == "@memory" || vp.substr(0, 8) == "@memory/") {
		con->Printf_err(MM_MINOR_ERR,
		               "Error mounting virtual path \"%s\": %s\"@memory\" is a reserved virtual path name.\n",
		               vp.c_str());
		return false;
	}

	string fullpath = rp;
	rewrite_virtual_path(fullpath);
	// When mounting a memory file, it wont exist, so don't attempt to create the dir
#ifdef DEBUG
	con->Printf(MM_INFO, "virtual path \"%s\": %s\n", vp.c_str(), fullpath.c_str());
#endif
	if (!(fullpath.substr(0, 8) == "@memory/")) {
		if (!IsDir(fullpath)) {
			if (!create) {
#ifdef DEBUG
				con->Printf_err(MM_MINOR_WARN,
				               "Problem mounting virtual path \"%s\": directory not found: %s\n",
				               vp.c_str(), fullpath.c_str());
#endif
				return false;
			} else {
				MkDir(fullpath);
			}
		}
	}

	virtualpaths[vp] = rp;
	return true;
}

bool FileSystem::RemoveVirtualPath(const string &vpath) {
	string vp = vpath;

	// remove trailing slash
	if (vp.rfind('/') == vp.size() - 1)
		vp.erase(vp.rfind('/'));

	Std::map<Common::String, string>::iterator i = virtualpaths.find(vp);

	if (i == virtualpaths.end()) {
		return false;
	} else {
		virtualpaths.erase(vp);
		return true;
	}
}

IDataSource *FileSystem::checkBuiltinData(const Std::string &vfn, bool is_text) {
	// Is it a Memory file?
	Std::map<Common::String, MemoryFile *>::iterator mf = memoryfiles.find(vfn);

	if (mf != memoryfiles.end())
		return new IBufferDataSource(mf->_value->data,
		                             mf->_value->len, is_text);

	return 0;
}

bool FileSystem::rewrite_virtual_path(string &vfn) {
	bool ret = false;
	string::size_type pos = vfn.size();

	while ((pos = vfn.rfind('/', pos)) != Std::string::npos) {
//		perr << vfn << ", " << vfn.substr(0, pos) << ", " << pos << Std::endl;
		Std::map<Common::String, string>::iterator p = virtualpaths.find(
		            vfn.substr(0, pos));

		if (p != virtualpaths.end()) {
			ret = true;
			// rewrite first part of path
			vfn = p->_value + vfn.substr(pos + 1);
			pos = string::npos;
		} else {
			if (pos == 0)
				break;
			--pos;
		}
	}

	// We will allow all paths to work
	if (noforcedvpaths) ret = true;

	return ret;
}


bool FileSystem::IsDir(const string &path) {
	Common::FSNode node(path);
	return node.isDirectory();
}

/*
 *  Create a directory
 */

bool FileSystem::MkDir(const string &path) {
	Common::FSNode newDir(path);
	return newDir.createDirectory();
}

/*
 *  Get the current users pentagram home path
 */

Std::string FileSystem::getHomePath() {
	Common::FSNode gameDir = Ultima8Engine::get_instance()->getGameDirectory();
	return gameDir.getPath();
}

} // End of namespace Ultima8
} // End of namespace Ultima
