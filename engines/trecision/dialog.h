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

#ifndef TRECISION_DIALOG_H
#define TRECISION_DIALOG_H

#include "common/scummsys.h"
#include "common/serializer.h"

namespace Trecision {
class TrecisionEngine;

#define MAXDISPCHOICES 5
#define MAXSUBTITLES 1500
#define MAXDIALOG 70
#define MAXCHOICE 1000
#define MAXNEWSMKPAL 40

struct Dialog {
	uint16 _flag;         // DONT_SKIP .. and more
	uint16 _interlocutor; // Person I'm talking to... Maybe it's not needed
	char _startAnim[14];  // aANIMATION or text table index by filename.
	uint16 _startLen;
	uint16 _firstChoice;
	uint16 _choiceNumb;
	uint16 _newPal[MAXNEWSMKPAL];
};

struct DialogSubTitle {
	uint16 _sentence;
	uint16 _x, _y, _color;       // you can compact this info using a bit field
	uint16 _startFrame, _length; // Frame at which the subtitle starts and its length
};

struct DialogChoice {
	uint16 _flag;                         // DLGCHOICE_HIDE|DLGCHOICE_ONETIME|DLGCHOICE_FRAUD...if used...
	uint16 _sentenceIndex;                // Index in the sentence array.
	uint16 _firstSubTitle, _subTitleNumb; // starting index and number of sub title sentences
	uint16 _on[MAXDISPCHOICES], _off[MAXDISPCHOICES];
	uint16 _startFrame; // Starting frame of the choice
	uint16 _nextDialog;
};

class DialogManager {
	TrecisionEngine *_vm;

	void showChoices(uint16 i);
	void playChoice(uint16 i);

	int16 _curPos;
	int16 _lastPos;
	uint16 _dispChoice[MAXDISPCHOICES];
	uint16 _curDispChoice;
	DialogSubTitle _subTitles[MAXSUBTITLES];
	uint16 _curSubTitle;

public:
	DialogManager(TrecisionEngine *vm);
	~DialogManager();

	void dialogPrint(int x, int y, int c, const Common::String &txt);
	void updateChoices(int16 dmx, int16 dmy);
	void selectChoice(int16 dmx, int16 dmy);
	void playDialog(uint16 i);
	void afterChoice();
	void dialogHandler(int numFrame);
	void doDialog();
	bool showCharacterAfterDialog() const;

	void syncGameStream(Common::Serializer &ser);
	void loadData(Common::File *file);

	Dialog _dialog[MAXDIALOG];
	DialogChoice _choice[MAXCHOICE];

	uint16 _curDialog;
	uint16 _curChoice;
};
; // end of class

} // End of namespace Trecision
#endif
