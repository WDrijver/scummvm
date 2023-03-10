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

#ifndef CHEWY_ROOMS_ROOM73_H
#define CHEWY_ROOMS_ROOM73_H

namespace Chewy {
namespace Rooms {

class Room73 : public JungleRoom {
public:
	static void entry(int16 eib_nr);
	static void xit(int16 eib_nr);
	static int procMoveBushes();
	static int procPickupMachete();
private:
	static void moveBushes();
	static void pickupMachete();
};

} // namespace Rooms
} // namespace Chewy

#endif
