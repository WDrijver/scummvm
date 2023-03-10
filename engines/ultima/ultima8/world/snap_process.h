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

#ifndef ULTIMA8_WORLD_SNAPPROCESS_H
#define ULTIMA8_WORLD_SNAPPROCESS_H

#include "ultima/ultima8/kernel/process.h"
#include "ultima/ultima8/misc/rect.h"

namespace Ultima {
namespace Ultima8 {

class Item;

class SnapProcess : public Process {
public:
	SnapProcess();
	~SnapProcess();

	ENABLE_RUNTIME_CLASSTYPE()

	void run() override;

	bool loadData(Common::ReadStream *rs, uint32 version);
	void saveData(Common::WriteStream *ws) override;

	void addEgg(Item *item);
	void removeEgg(Item *item);
	void clearEggs();

	static SnapProcess *get_instance();

protected:
	static SnapProcess *_instance;

	void getSnapEggRange(const Item *egg, Rect &rect) const;
	void updateCurrentEgg();
	bool isNpcInRangeOfCurrentEgg() const;

	ObjId   _currentSnapEgg;
	Rect	_currentSnapEggRange;
	Std::list<ObjId> _snapEggs;
};

} // End of namespace Ultima8
} // End of namespace Ultima

#endif
