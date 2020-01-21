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

#ifndef ULTIMA_ULTIMA1_MAP_MAP_DUNGEON_H
#define ULTIMA_ULTIMA1_MAP_MAP_DUNGEON_H

#include "ultima/ultima1/map/map.h"
#include "ultima/shared/core/rect.h"
#include "common/random.h"

namespace Ultima {
namespace Ultima1 {
namespace Map {

#define DUNGEON_WIDTH 11
#define DUNGEON_HEIGHT 11

enum DungeonTile {
	DTILE_HALLWAY = 0, DTILE_WALL = 1, DTILE_SECRET_DOOR = 2, DTILE_DOOR = 3, DTILE_LADDER_DOWN = 6,
	DTILE_LADDER_UP = 7, DTILE_BEAMS = 8
};

class MapDungeon : public Ultima1Map::MapBase {
private:
	Common::RandomSource _random;
	uint _dungeonLevel;					// Dungeon level number
private:
	/**
	 * Sets up a deterministic random seed for generating dungeon data
	 */
	void setRandomSeed();

	/**
	 * Gets a deterministic random number based on a given seed. Used in dungeon generation
	 * so that a given dungeon and level will always be built the same
	 */
	uint getDeterministicRandomNumber(uint min, uint max) { return min + _random.getRandomNumber(max - min); }

	/**
	 * Spawns a monster at a given position in the dungeon map
	 */
	void spawnMonsterAt(const Point &pt);
public:
	MapDungeon(Ultima1Game *game) : Ultima1Map::MapBase(game), _dungeonLevel(0),
		_random("UltimaDungeons") {}
	virtual ~MapDungeon() {}

	/**
	 * Load the map
	 */
	virtual void load(Shared::MapId mapId);

	/**
	 * Changes the dungeon level by a given delta amount, and generates a new map
	 * @param delta		Delta to change dungeon level by
	 * @returns			False if dungeon left, true if still within dungeon
	 */
	virtual bool changeLevel(int delta);

	/**
	 * Get the current map level
	 */
	virtual uint getLevel() const { return _dungeonLevel; }

	/**
	 * Spawns a monster within dungeons
	 */
	void spawnMonster();
};

} // End of namespace Map
} // End of namespace Ultima1
} // End of namespace Ultima

#endif
