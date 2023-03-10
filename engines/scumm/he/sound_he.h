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

#ifndef SCUMM_HE_SOUND_HE_H
#define SCUMM_HE_SOUND_HE_H

#include "common/scummsys.h"
#include "scumm/sound.h"
#include "audio/audiostream.h"

namespace Scumm {

class ScummEngine_v60he;

class SoundHE : public Sound {
protected:
	ScummEngine_v60he *_vm;

	int _overrideFreq;

	struct HEMusic {
		int32 id;
		int32 offset;
		int32 size;
	};
	HEMusic *_heMusic;
	int16 _heMusicTracks;

	Audio::SoundHandle *_heSoundChannels;

public: // Used by createSound()
	struct {
		int sound;
		int codeOffs;
		int priority;
		int rate;
		int timer;
		int sbngBlock;
		int soundVars[27];
	} _heChannel[8];

public:
	SoundHE(ScummEngine *parent, Audio::Mixer *mixer);
	~SoundHE() override;

	void addSoundToQueue(int sound, int heOffset = 0, int heChannel = 0, int heFlags = 0, int heFreq = 0, int hePan = 0, int heVol = 0) override;
	void addSoundToQueue2(int sound, int heOffset = 0, int heChannel = 0, int heFlags = 0, int heFreq = 0, int hePan = 0, int heVol = 0) override;

	int isSoundRunning(int sound) const override;
	void stopSound(int sound) override;
	void stopAllSounds() override;
	void setupSound() override;

	bool getHEMusicDetails(int id, int &musicOffs, int &musicSize);
	int findFreeSoundChannel();
	int isSoundCodeUsed(int sound);
	int getSoundPos(int sound);
	int getSoundVar(int sound, int var);
	void setSoundVar(int sound, int var, int val);
	void playHESound(int soundID, int heOffset, int heChannel, int heFlags, int heFreq, int hePan, int heVol);
	void processSoundCode();
	void processSoundOpcodes(int sound, byte *codePtr, int *soundVars);
	void setOverrideFreq(int freq);
	void setupHEMusicFile();
	void startHETalkSound(uint32 offset);
	void stopSoundChannel(int chan);

protected:
	void processSoundQueues() override;

private:
	int _heTalkOffset;

	Audio::RewindableAudioStream *tryLoadAudioOverride(int soundID, int *duration = nullptr);
};


} // End of namespace Scumm

#endif
