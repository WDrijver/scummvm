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

#ifndef SCUMM_PLAYERS_PLAYER_V2_H
#define SCUMM_PLAYERS_PLAYER_V2_H

#include "scumm/players/player_v2base.h"
#include "audio/audiostream.h"
#include "audio/mixer.h"

namespace Scumm {

/**
 * Scumm V2 PC-Speaker MIDI driver.
 * This simulates the pc speaker sound, which is driven  by the 8253 (square
 * wave generator) and a low-band filter.
 */
class Player_V2 : public Audio::AudioStream, public Player_V2Base {
public:
	Player_V2(ScummEngine *scumm, Audio::Mixer *mixer, bool pcjr);
	~Player_V2() override;

	// MusicEngine API
	void setMusicVolume(int vol) override;
	void startSound(int sound) override;
	void stopSound(int sound) override;
	void stopAllSounds() override;
//	virtual int  getMusicTimer();
	int  getSoundStatus(int sound) const override;

	// AudioStream API
	int readBuffer(int16 *buffer, const int numSamples) override;
	bool isStereo() const override { return true; }
	bool endOfData() const override { return false; }
	int getRate() const override { return _sampleRate; }

protected:
	enum {
		FIXP_SHIFT = 16
	};

	unsigned int _update_step;
	unsigned int _decay;
	int _level;
	unsigned int _RNG;
	unsigned int _volumetable[16];

	int _timer_count[4];
	int _timer_output;

	Audio::Mixer *_mixer;
	Audio::SoundHandle _soundHandle;
	const uint32 _sampleRate;

	Common::Mutex _mutex;

	uint32 _next_tick;
	uint32 _tick_len;

protected:
	virtual void generateSpkSamples(int16 *data, uint len);
	virtual void generatePCjrSamples(int16 *data, uint len);

	void lowPassFilter(int16 *data, uint len);
	void squareGenerator(int channel, int freq, int vol,
						int noiseFeedback, int16 *sample, uint len);
};

} // End of namespace Scumm

#endif
