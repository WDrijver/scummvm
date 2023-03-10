/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * Additional copyright for this file:
 * Copyright (C) 1995-1997 Presto Studios, Inc.
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

#ifndef PEGASUS_ITEMS_ITEMLIST_H
#define PEGASUS_ITEMS_ITEMLIST_H

#include "common/list.h"

#include "pegasus/types.h"

namespace Common {
	class ReadStream;
	class WriteStream;
}

namespace Pegasus {

class Item;

class ItemList : public Common::List<Item *> {
public:
	ItemList();
	virtual ~ItemList();

	virtual void writeToStream(Common::WriteStream *stream);
	virtual void readFromStream(Common::ReadStream *stream);

	Item *findItemByID(const ItemID id);
	void resetAllItems();
};

typedef ItemList::iterator ItemIterator;

#define g_allItems g_vm->getAllItems()

} // End of namespace Pegasus

#endif
