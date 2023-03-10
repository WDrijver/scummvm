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

#ifndef ULTIMA8_WORLD_ITEMSELECTIONPROCESS_H
#define ULTIMA8_WORLD_ITEMSELECTIONPROCESS_H

#include "ultima/ultima8/kernel/process.h"
#include "ultima/ultima8/misc/classtype.h"

namespace Ultima {
namespace Ultima8 {

class Item;

/**
 * A process to select items for use.
 */
class ItemSelectionProcess : public Process {
public:
	ItemSelectionProcess();
	~ItemSelectionProcess();

	ENABLE_RUNTIME_CLASSTYPE()

	void run() override;

	//! Select the next item. If grab is true, pick up loose items now.
	bool selectNextItem(bool grab);

	//! Clear the selector sprite
	void clearSelection();

	//! Avatar moved - clear the selector if needed.
	void avatarMoved();

	//! Use the selected item (if any)
	void useSelectedItem();

	bool loadData(Common::ReadStream *rs, uint32 version);
	void saveData(Common::WriteStream *ws) override;

	static ItemSelectionProcess *get_instance() {
		return _instance;
	}

	uint16 getSelectedItem() const {
		return _selectedItem;
	}

private:
	void putItemSelectionOnItem(Item *item);

	uint16 _selectedItem;
	int32 _ax;
	int32 _ay;
	int32 _az;

	static ItemSelectionProcess *_instance;
};

} // End of namespace Ultima8
} // End of namespace Ultima

#endif
