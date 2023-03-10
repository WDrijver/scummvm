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

#include "engines/advancedDetector.h"
#include "common/translation.h"
#include "made/made.h"
#include "made/detection.h"


namespace Made {

static const ADExtraGuiOptionsMap optionsList[] = {
	{
		GAMEOPTION_INTRO_MUSIC_DIGITAL,
		{
			_s("Play a digital soundtrack during the opening movie"),
			_s("If selected, the game will use a digital soundtrack during the introduction. Otherwise, it will play MIDI music."),
			"intro_music_digital",
			true,
			0,
			0
		}
	},
	AD_EXTRA_GUI_OPTIONS_TERMINATOR
};

uint32 MadeEngine::getGameID() const {
	return _gameDescription->gameID;
}

uint32 MadeEngine::getFeatures() const {
	return _gameDescription->features;
}

Common::Platform MadeEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

uint16 MadeEngine::getVersion() const {
	return _gameDescription->version;
}

} // End of namespace Made

class MadeMetaEngine : public AdvancedMetaEngine {
public:
	const char *getName() const override {
		return "made";
	}

	const ADExtraGuiOptionsMap *getAdvancedExtraGuiOptions() const override {
		return Made::optionsList;
	}

	bool hasFeature(MetaEngineFeature f) const override;
	Common::Error createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const override;
};

bool MadeMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		false;
}

bool Made::MadeEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsReturnToLauncher);
}

Common::Error MadeMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	*engine = new Made::MadeEngine(syst, (const Made::MadeGameDescription *)desc);
	return Common::kNoError;
}

#if PLUGIN_ENABLED_DYNAMIC(MADE)
	REGISTER_PLUGIN_DYNAMIC(MADE, PLUGIN_TYPE_ENGINE, MadeMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(MADE, PLUGIN_TYPE_ENGINE, MadeMetaEngine);
#endif
