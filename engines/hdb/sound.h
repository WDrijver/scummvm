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

#ifndef HDB_SOUND_H
#define HDB_SOUND_H

#define	SFNAME_TITLE		"jungle.mp3"
#define	SFNAME_MENU			"on_a_mission.mp3"
#define	SFNAME_ROBO			"low_techno.mp3"
#define	SFNAME_MEXI			"ambient_mexi_howl.mp3"
#define	SFNAME_BASSO		"amb_walkin_basso.mp3"
#define	SFNAME_WIND			"dangerous_wind.mp3"
#define	SFNAME_INDUSTRO		"industro_hunch.mp3"
#define	SFNAME_JACKIN		"jaround.mp3"
#define	SFNAME_SNEAKERS		"lost_my_sneakers.mp3"
#define	SFNAME_QUIET		"mysterious_quiet.mp3"
#define	SFNAME_JEEBIES		"jeebies5B.mp3"
#define	SFNAME_VIBRACIOUS	"vibracious_(dope_delivery_mix).mp3"
#define	SFNAME_ROMANTIC		"romantic.mp3"
#define	SFNAME_ARETHERE		"AreWeThereYet_(extended).mp3"

#define	SFNAME_CORRIDOR		"corridor(extended).mp3"
#define SFNAME_MOKE			"moke_a_doke_doke.mp3"
#define	SFNAME_TILES		"tilesache(extended).mp3"
#define SFNAME_DARKVIB		"vibracious_(Dark_Delivery_Mix).mp3"
#define SFNAME_EXPER		"experimentatus_(extended).mp3"

#define	FIRST_VOICE		V00_AIRLOCK_01
#define	NUM_VOICES		( LAST_SOUND - FIRST_VOICE )

#include "audio/audiostream.h"
#include "audio/mixer.h"
#include "audio/decoders/wave.h"
#include "audio/decoders/mp3.h"
#include "audio/decoders/vorbis.h"

namespace HDB {

enum {
	kMaxSNDChannels = 32,
	kMaxSounds = 5000,
	kLaserChannel = kMaxSNDChannels - 1
};

enum SoundType {
	SONG_NONE,
	SND_GUI_INPUT,
	SND_MAIL_PROCESS,
	SND_MONKEY_OOHOOH,
	SND_GET_GEM,
	SND_MENU_ACCEPT,
	SND_MENU_BACKOUT,
	SND_MENU_SLIDER,
	SND_DIALOG_CLOSE,
	SND_CRATE_SLIDE,
	SND_LIGHT_SLIDE,
	SND_HEAVY_SLIDE,
	SND_POP,
	SND_TELEPORT,
	SND_FOOTSTEPS,
	SND_SPLASH,
	SND_CELLHOLDER_USE_REJECT,
	SND_CHICKEN_AMBIENT,
	SND_FERRET_SQUEAK,
	SND_SWITCH_USE,
	SND_MOVE_SELECTION,
	SND_NOTICE,
	SND_MAINTBOT_WHOOSH1,
	SND_MAINTBOT_WHOOSH2,
	SND_SHIPMOVING_INTRO,
	SND_DIALOG_OPEN,
	SND_TOUCHPLATE_CLICK,
	SND_DOOR_OPEN_CLOSE,
	SND_MBOT_HYEAH,
	SND_MBOT_YEAH,
	SND_MBOT_WHISTLE1,
	SND_CLUB_MISS,
	SND_CLUB_HIT_METAL,
	SND_CLUB_HIT_FLESH,
	SND_FROG_LICK,
	SND_ROBOT_STUNNED,
	SND_QUEST_FAILED,
	SND_GET_MONKEYSTONE,
	SND_INSERT_CELL,
	SND_CABINET_OPEN,
	SND_CABINET_CLOSE,
	SND_MAILSORTER_HAPPY,
	SND_QUEST_COMPLETE,
	SND_TRY_AGAIN,
	SND_AIRLOCK_CLOSE,
	SND_BYE,
	SND_FART,
	SND_FART2,
	SND_GUY_UHUH,
	SND_GUY_DYING,
	SND_GEM_THROW,
	SND_INV_SELECT,
	SND_INFOCOMP,
	SND_CLOCK_BONK,
	SND_HDB,
	SND_VORTEX_SAVE,
	SND_GET_GOO,
	SND_MANNY_CRASH,
	SND_BARREL_EXPLODE,
	SND_BARREL_MELTING,
	SND_CHICKEN_BAGAWK,
	SND_CHICKEN_DEATH,
	SND_GET_THING,
	SND_STEPS_ICE,
	SND_FOURFIRE_TURN,
	SND_FOUR_FIRE,
	SND_SHOCKBOT_AMBIENT,
	SND_SHOCKBOT_SHOCK,
	SND_RAILRIDER_ONTRACK,
	SND_RAILRIDER_TASTE,
	SND_RAILRIDER_EXIT,
	SND_GUY_FRIED,
	SND_MAILSORTER_UNHAPPY,
	SND_GET_CLUB,
	SND_BUZZFLY_FLY,
	SND_BUZZFLY_STUNNED,
	SND_BUZZFLY_STING,
	SND_FATFROG_STUNNED,
	SND_NOPUSH_SIZZLE,
	SND_OMNIBOT_FIRE,
	SND_RIGHTBOT_TURN,
	SND_RIGHTBOT_STUNNED,
	SND_MONKEY_WIN,
	SND_FALL_DOWN_HOLE,
	SND_MBOT_HMMM,
	SND_MBOT_HMMM2,
	SND_MBOT_DEATH,
	SND_MBOT_WHISTLE2,
	SND_MBOT_WHISTLE3,
	SND_DEADEYE_AMB01,
	SND_DEADEYE_AMB02,
	SND_DEADEYE_ATTACK01,
	SND_DEADEYE_ATTACK02,
	SND_FROG_RIBBIT1,
	SND_FROG_RIBBIT2,
	SND_MEERKAT_BITE,
	SND_BRIDGE_EXTEND,
	SND_BRIDGE_START,
	SND_BRIDGE_END,
	SND_MACHINE_AMBIENT1,
	SND_GET_STUNNER,
	SND_GET_SLUG,
	SND_GUY_DROWN,
	SND_GUY_GRABBED,
	SND_PANIC,
	SND_PANIC_COUNT,
	SND_PANIC_DEATH,
	SND_LASER_LOOP,
	SND_SLOT_WIN,
	SND_SLOT_SPIN,
	SND_SLOT_STOP,
	SND_GOOD_FAERIE_AMBIENT,
	SND_GOOD_FAERIE_SPELL,
	SND_GOOD_FAERIE_STUNNED,
	SND_ICEPUFF_WARNING,
	SND_ICEPUFF_THROW,
	SND_ICEPUFF_STUNNED,
	SND_ICEPUFF_APPEAR,
	SND_GUY_PLUMMET,
	SND_PUSH_DIVERTER,
	SND_TURNBOT_TURN,
	SND_PUSHBOT_STRAIN,
	SND_MONKEYSTONE_SECRET_STAR,
	SND_OMNIBOT_AMBIENT,
	SND_PUSHBOT_STUNNED,
	SND_MEERKAT_WARNING,
	SND_MEERKAT_APPEAR,
	SND_MEERKAT_STUNNED,
	SND_TURNBOT_STUNNED,
	SND_DRAGON_WAKE,
	SND_DRAGON_FALLASLEEP,
	SND_DRAGON_BREATHEFIRE,
	SND_BADFAIRY_AMBIENT,
	SND_BADFAIRY_SPELL,
	SND_BADFAIRY_STUNNED,
	SND_DEMIGOD_AMBIENT,
	SND_DEMIGOD_HOLYSPEECH,
	SND_DEMIGOD_UNHAPPY,
	SND_GATEPUDDLE_AMBIENT,
	SND_GATEPUDDLE_DISSIPATE,
	SND_GATEPUDDLE_SPAWN,
	SND_REALSLOT_SPIN,
	SND_REALSLOT_STOP,
	SND_REALSLOT_WIN,
	SND_SLUG_FIRE,
	SND_SLUG_HIT,
	SND_STUNNER_FIRE,
	SND_UNLOCKED_ITEM,

	SONG_TITLE,
	SONG_MENU,
	SONG_ROBO,
	SONG_MEXI,
	SONG_BASSO,
	SONG_WIND,
	SONG_INDUSTRO,
	SONG_JACKIN,
	SONG_SNEAKERS,
	SONG_QUIET,
	SONG_JEEBIES,
	SONG_VIBRACIOUS,
	SONG_ROMANTIC,
	SONG_ARETHERE,

	SONG_CORRIDOR,
	SONG_MOKE,
	SONG_TILES,
	SONG_DARKVIB,
	SONG_EXPER,

	SND_BEEPBEEPBEEP,

	V00_AIRLOCK_01,
	V00_ARNIE_01,

	V00_BRAD_01,
	V00_BRAD_02,
	V00_BRAD_03,

	V00_BUSTER_01,
	V00_BUSTER_02,
	V00_BUSTER_03,
	V00_BUSTER_04,
	V00_BUSTER_05,
	V00_BUSTER_06,
	V00_BUSTER_07,
	V00_BUSTER_08,
	V00_BUSTER_09,
	V00_BUSTER_10,
	V00_BUSTER_11,
	V00_BUSTER_12,
	V00_BUSTER_13A,
	V00_BUSTER_13B,
	V00_BUSTER_14,
	V00_BUSTER_15,
	V00_BUSTER_16,
	V00_BUSTER_17,
	V00_BUSTER_18,
	V00_BUSTER_19,

	V00_CHICKEN_01,

	V00_CHUTEGUARD_01,
	V00_CHUTEGUARD_02,
	V00_CHUTEGUARD_03,
	V00_CHUTEGUARD_04,
	V00_CHUTEGUARD_05,

	V00_COOPER_01,
	V00_COOPER_02,
	V00_COOPER_03,
	V00_COOPER_04,
	V00_COOPER_05,

	V00_DINERS_01,
	V00_DINERS_02,
	V00_DINERS_03,
	V00_DINERS_04,
	V00_DINERS_05,
	V00_DINERS_06,

	V00_DOLLY_01,
	V00_DOLLY_02,
	V00_DOLLY_03,
	V00_DOLLY_04,
	V00_DOLLY_05,
	V00_DOLLY_06,
	V00_DOLLY_07,
	V00_DOLLY_08,
	V00_DOLLY_09,

	V00_DOREK_01,
	V00_DOREK_02,
	V00_DOREK_03,
	V00_DOREK_04,

	V00_ENGCOMPUTER_01,
	V00_ENGCOMPUTER_02,

	V00_FARBLE_01,
	V00_FARBLE_02,
	V00_FARBLE_03,
	V00_FARBLE_04,
	V00_FARBLE_05,
	V00_FARBLE_06,
	V00_FARBLE_07,
	V00_FARBLE_08,

	V00_GRUNETT_01,
	V00_GRUNETT_02,
	V00_GRUNETT_03,
	V00_GRUNETT_04,

	V00_GUY_01,
	V00_GUY_02,
	V00_GUY_03,
	V00_GUY_04,
	V00_GUY_05,
	V00_GUY_06,
	V00_GUY_07,
	V00_GUY_08,
	V00_GUY_09,
	V00_GUY_10A,
	V00_GUY_10B,
	V00_GUY_11,
	V00_GUY_12,
	V00_GUY_13,
	V00_GUY_14,
	V00_GUY_15,
	V00_GUY_16,
	V00_GUY_17,
	V00_GUY_18,

	V00_HUMBERT_01,
	V00_HUMBERT_02,

	V00_ILLIAC_01,
	V00_INFOCOMP_01,

	V00_JAQUES_01,
	V00_JAQUES_03,
	V00_JAQUES_04,
	V00_JAQUES_05,

	V00_LUMBAR_01,

	V00_MAILSORTER_01,
	V00_MAILSORTER_02,
	V00_MAILSORTER_03,
	V00_MAILSORTER_04,
	V00_MAILSORTER_05,
	V00_MAILSORTER_06,
	V00_MAILSORTER_07,
	V00_MAILSORTER_08,
	V00_MAILSORTER_09,
	V00_MAILSORTER_10,
	V00_MAILSORTER_11,
	V00_MAILSORTER_12,
	V00_MAILSORTER_13,
	V00_MAILSORTER_14,
	V00_MAILSORTER_15,

	V00_MANNY_01,
	V00_MANNY_02,
	V00_MANNY_03,
	V00_MANNY_04,
	V00_MANNY_05,
	V00_MANNY_06,
	V00_MANNY_07,
	V00_MANNY_08,
	V00_MANNY_09,
	V00_MANNY_10,
	V00_MANNY_11,
	V00_MANNY_12,
	V00_MANNY_13,
	V00_MANNY_14,
	V00_MANNY_15,
	V00_MANNY_16,
	V00_MANNY_17,
	V00_MANNY_18,

	V00_NOTEBRICK_01,

	V00_PILOTS_01,
	V00_PILOTS_02,

	V00_POLONEY_01,
	V00_POLONEY_02,
	V00_POLONEY_03,
	V00_POLONEY_04,
	V00_POLONEY_05,
	V00_POLONEY_06,

	V00_SARGE_01,
	V00_SARGE_02,
	V00_SARGE_03,
	V00_SARGE_04,
	V00_SARGE_05,
	V00_SARGE_06,
	V00_SARGE_07,
	V00_SARGE_08,
	V00_SARGE_09,
	V00_SARGE_10,
	V00_SARGE_11,
	V00_SARGE_12,
	V00_SARGE_13,
	V00_SARGE_14,
	V00_SARGE_15,
	V00_SARGE_16,

	V01_ALABASTER_01,
	V01_ALABASTER_02,
	V01_ALABASTER_03,
	V01_ALABASTER_04,
	V01_ALABASTER_05,
	V01_ALABASTER_06,
	V01_BRAKER_01,
	V01_BRAKER_02,
	V01_BRAKER_03,
	V01_BRAKER_04,
	V01_CHISTE_01,
	V01_CHISTE_02,
	V01_CHISTE_03,
	V01_CHISTE_04,
	V01_CHISTE_05,
	V01_CHISTE_06,
	V01_CHISTE_07,
	V01_CHISTE_08,
	V01_CHISTE_09,
	V01_CHISTE_10,
	V01_CHISTE_11,
	V01_CHISTE_12,
	V01_CHISTE_13,
	V01_CHISTE_14,
	V01_COVERT_01,
	V01_COVERT_02,
	V01_COVERT_03,
	V01_COVERT_04,
	V01_COVERT_05,
	V01_COVERT_06,
	V01_COVERT_07,
	V01_COVERT_08,
	V01_COVERT_09,
	V01_COVERT_10,
	V01_COVERT_11,
	V01_COVERT_12,
	V01_COVERT_13,
	V01_DAGEAUBE_01,
	V01_DAGEAUBE_02,
	V01_DAGEAUBE_03,
	V01_DAGEAUBE_04,
	V01_DAGEAUBE_05,
	V01_EMC2_01,
	V01_EMC2_02,
	V01_GIZZARD_01,
	V01_GIZZARD_02,
	V01_GUY_01,
	V01_GUY_02,
	V01_GUY_03,
	V01_GUY_04,
	V01_GUY_05,
	V01_HIDROW_01,
	V01_HIDROW_02,
	V01_HIDROW_03,
	V01_KEBOTTLE_01,
	V01_KEBOTTLE_02,
	V01_KEBOTTLE_03,
	V01_KEBOTTLE_04,
	V01_KEBOTTLE_05,
	V01_KEBOTTLE_06,
	V01_KEBOTTLE_07,
	V01_PONTE_01,
	V01_PONTE_02,
	V01_PONTE_03,
	V01_PONTE_04,
	V01_PONTE_05,
	V01_PONTE_06,
	V01_PONTE_07,
	V01_PONTE_08,
	V01_PONTE_09,
	V01_PONTE_10,
	V01_POPOPOLIS_01,
	V01_POPOPOLIS_02,
	V01_POPOPOLIS_03,
	V01_POPOPOLIS_04,
	V01_POPOPOLIS_05,
	V01_POPOPOLIS_06,
	V01_POPOPOLIS_07,
	V01_POPOPOLIS_08,
	V01_POPOPOLIS_09,
	V01_POPOPOLIS_10,
	V01_POPOPOLIS_11,
	V01_POPOPOLIS_12,
	V01_POPOPOLIS_13,
	V01_POPOPOLIS_14,
	V01_POPOPOLIS_15,
	V01_POPOPOLIS_16,
	V01_POPOPOLIS_17,
	V01_POPOPOLIS_18,
	V01_POPOPOLIS_19,
	V01_POPOPOLIS_20,
	V01_POPOPOLIS_21,
	V01_POPOPOLIS_22,
	V01_POPOPOLIS_23,
	V01_POPOPOLIS_24,
	V01_POPOPOLIS_25,
	V01_POPOPOLIS_26,
	V01_POPOPOLIS_27,
	V01_POPOPOLIS_28,
	V01_POPOPOLIS_29,
	V01_POPOPOLIS_30,
	V01_POPOPOLIS_31,
	V01_POPOPOLIS_32,
	V01_POPOPOLIS_33,
	V01_POPOPOLIS_34,
	V01_POPOPOLIS_35,
	V01_POPOPOLIS_36,
	V01_POPOPOLIS_37,
	V01_SARGE_01,
	V01_SARGE_02,
	V01_SARGE_03,
	V01_SARGE_04,
	V01_SOBREDELLE_01,
	V01_SOBREDELLE_02,
	V01_SOBREDELLE_03,
	V01_WAYNE_01,
	V01_WAYNE_02,
	V01_ZIQUOZ_01,
	V01_ZIQUOZ_02,
	V01_ZIQUOZ_03,

	V02_AGUSTO_01,
	V02_AGUSTO_02,
	V02_ALBONDIGAS_01,
	V02_ALBONDIGAS_02,
	V02_BUMPER_01,
	V02_BUMPER_02,
	V02_BUMPER_03,
	V02_BUMPER_04,
	V02_BUMPER_05,
	V02_BUMPER_06,
	V02_BUMPER_07,
	V02_DELGADO_01,
	V02_FEBRARO_01,
	V02_GUY_01,
	V02_GUY_02,
	V03_ACROPHO_01,
	V03_ACROPHO_02,
	V03_ACROPHO_03,
	V03_ACROPHO_04,
	V03_ENERO_01,
	V03_ENERO_02,
	V03_ENERO_03,
	V03_ENERO_04,
	V03_ENERO_05,
	V03_GAIA_01,
	V03_GAIA_02,
	V03_GAIA_03,
	V03_GAIA_04,
	V03_GAIA_05,
	V03_GAIA_06,
	V03_LAGO_01,
	V03_LAGO_02,
	V03_LAGO_04,
	V03_LAGO_05,
	V03_LAGO_06,
	V03_LAGO_07,
	V04_BONIFACE_01,
	V04_BONIFACE_02,
	V04_BONIFACE_03,
	V04_BONIFACE_04,
	V04_BONIFACE_05,
	V04_BONIFACE_06,
	V04_BONIFACE_07,
	V04_JULIO_01,
	V04_JULIO_02,
	V04_JULIO_04,
	V04_MARZO_01,
	V04_OCTUBRE_01,
	V04_OCTUBRE_02,
	V04_OCTUBRE_03,

	V05_ABRIL_01,
	V05_ABRIL_02,
	V05_ABRIL_03,
	V05_ABRIL_04,
	V05_ABRIL_05,
	V05_GELIGELLA_01,
	V05_GELIGELLA_02,
	V05_RAILRIDER_01,
	V05_RAILRIDER_02,
	V05_RAILRIDER_03,
	V05_RAILRIDER_04,
	V05_EVERLOO_01,
	V05_EVERLOO_02,
	V05_EVERLOO_03,
	V05_EVERLOO_04,
	V06_AZUL_01,
	V06_AZUL_02,
	V06_AZUL_03,
	V06_AZUL_04,
	V06_AZUL_05,
	V06_AZUL_06,
	V06_AZUL_07,
	V06_AZUL_08,
	V06_GUY_01,
	V06_GUY_02,
	V06_GUY_03,
	V06_GUY_04,
	V06_HANSON_01,
	V06_HANSON_03,
	V06_HANSON_04,
	V06_HANSON_05,
	V06_HANSON_06,
	V06_HANSON_07,
	V06_HANSON_08,
	V06_HANSON_09,
	V06_HANSON_10,
	V06_HANSON_11,
	V06_HANSON_12,
	V06_JUNIO_01,
	V06_JUNIO_02,
	V06_JUNIO_03,
	V06_JUNIO_04,
	V06_JUNIO_05,
	V06_JUNIO_06,
	V06_JUNIO_07,
	V06_JUNIO_08,
	V06_JUNIO_09,
	V06_NARANJA_01,
	V06_NARANJA_02,
	V06_NARANJA_03,
	V06_NARANJA_04,
	V06_NARANJA_05,
	V06_SEPTEMBRE_01,
	V06_SEPTEMBRE_02,
	V06_SEPTEMBRE_03,
	V06_SEPTEMBRE_04,
	V06_SEPTEMBRE_05,
	V06_SEPTEMBRE_06,
	V06_SEPTEMBRE_07,
	V06_SEPTEMBRE_08,
	V06_SEPTEMBRE_09,
	V06_WORKER_01,
	V06_WORKER_02,
	V06_WORKER_03,
	V06_WORKER_04,
	V07_CERCO_01,
	V07_CERCO_02,
	V07_COMPUTER_01,
	V07_MAYO_01,
	V07_MAYO_02,
	V07_NOVIEMBRE_01,
	V07_NOVIEMBRE_02,
	V07_NOVIEMBRE_03,
	V07_PANITO_01,
	V07_PANITO_02,
	V07_PANITO_03,
	V07_PANITO_04,
	V07_PANITO_05,
	V08_COOKIE_01,
	V08_COOKIE_02,
	V08_DICIEMBRE_01,
	V08_GUY_01,
	V08_GUY_02,
	V08_GUY_03,
	V08_MACHINA_01,
	V08_MACHINA_02,
	V08_MACHINA_03,
	V08_MACHINA_04,
	V08_MACHINA_05,
	V08_MACHINA_06,
	V08_MACHINA_07,
	V08_MACHINA_08,
	V08_MACHINA_09,
	V08_MESA_01,
	V08_MESA_02,
	V08_MESA_03,
	V08_MESA_04,
	V08_SHAKIR_01,
	V08_SHAKIR_02,
	V08_SHAKIR_03,
	V09_VIOLETA_01,
	V09_VIOLETA_02,
	V09_VIOLETA_03,
	V09_VIOLETA_04,
	V10_CHICKEN_01,
	V10_COOPER_01,
	V10_COOPER_02,
	V10_COOPER_03,
	V10_COOPER_04,
	V10_COOPER_05,
	V10_COOPER_06,
	V10_COOPER_07,
	V10_COOPER_08,
	V10_COOPER_09,
	V10_DOLLY_01,
	V10_DOLLY_02,
	V10_DOLLY_03,
	V10_DOLLY_04,
	V10_DOLLY_05,
	V10_DOLLY_06,
	V10_DOLLY_07,
	V10_DOLLY_08,
	V10_DOLLY_09,
	V10_DOLLY_10,
	V10_ENGCOMPUTER_01,
	V10_FARBLE_01,
	V10_FARBLE_02,
	V10_FARBLE_03,
	V10_FARBLE_04,
	V10_FARBLE_05,
	V10_FARBLE_06,
	V10_GRUNETT_01,
	V10_GRUNETT_02,
	V10_GRUNETT_03,
	V10_GRUNETT_04,
	V10_GRUNETT_05,
	V10_GRUNETT_06,
	V10_GUY_01,
	V10_GUY_02,
	V10_GUY_03,
	V10_GUY_04,
	V10_GUY_05,
	V10_GUY_06,
	V10_GUY_07,
	V10_GUY_08,
	V10_INFOCOMP_01,
	V10_NOTEBRICK_01,
	V10_PILOTS_01,
	V10_PILOTS_02,
	V10_PILOTS_03,
	V10_PILOTS_04,
	V10_PILOTS_05,
	V10_SARGE_01,
	V10_SARGE_02,
	V10_SARGE_03,
	V10_SARGE_04,
	V10_SARGE_05,
	V11_ANCHOVY_01,
	V11_ANCHOVY_02,
	V11_ANCHOVY_03,
	V11_ANCHOVY_04,
	V11_ANCHOVY_05,
	V11_ARBIVALL_01,
	V11_ARBIVALL_02,
	V11_ARBIVALL_03,
	V11_ARBIVALL_04,
	V11_ARBIVALL_05,
	V11_ARBIVALL_06,
	V11_ARBIVALL_07,
	V11_ARNIE_01,
	V11_BELINDA_01,
	V11_BLOCK_01,
	V11_BLOCK_02,
	V11_BLOCK_03,
	V11_BRAD_01,
	V11_BRAD_02,
	V11_BRAD_03,
	V11_BUSTER_01,
	V11_BUSTER_02,
	V11_BUSTER_03,
	V11_DINERS_01,
	V11_DINERS_02,
	V11_DOPPLER_01,
	V11_DOPPLER_02,
	V11_DOPPLER_03,
	V11_DOPPLER_04,
	V11_DOPPLER_05,
	V11_DOREK_01,
	V11_DOREK_02,
	V11_DOREK_03,
	V11_DOZER_01,
	V11_DOZER_02,
	V11_DOZER_03,
	V11_DOZER_04,
	V11_GNAPPE_01,
	V11_GNAPPE_02,
	V11_GNAPPE_03,
	V11_GNAPPE_04,
	V11_GNAPPE_05,
	V11_GNODIMY_01,
	V11_GNODIMY_02,
	V11_GNODIMY_03,
	V11_GNODIMY_04,
	V11_GUY_01,
	V11_GUY_02,
	V11_GUY_03,
	V11_GUY_04,
	V11_GUY_05,
	V11_GUY_06,
	V11_GUY_07,
	V11_GUY_08,
	V11_GUY_09,
	V11_GUY_10,
	V11_GUY_11,
	V11_GUY_12,
	V11_GUY_13,
	V11_GUY_15,
	V11_GUY_16,
	V11_GUY_17,
	V11_HAMBRE_01,
	V11_HAMBRE_02,
	V11_HAMBRE_03,
	V11_HAMBRE_04,
	V11_HAMBRE_05,
	V11_HUMBERT_01,
	V11_ILLIAC_01,
	V11_JAQUES_01,
	V11_JAQUES_02,
	V11_JAQUES_03,
	V11_JAQUES_04,
	V11_LUMBAR_01,
	V11_MAN_IN_STALL_01,
	V11_MAN_IN_STALL_02,
	V11_MAN_IN_STALL_03,
	V11_MAN_IN_STALL_04,
	V11_MAN_IN_STALL_05,
	V11_MANNY_01,
	V11_MUMPS_01,
	V11_MUMPS_02,
	V11_MUMPS_03,
	V11_MUMPS_04,
	V11_MUMPS_05,
	V11_MUMPS_06,
	V11_MUMPS_07,
	V11_MUMPS_08,
	V11_MUMPS_09,
	V11_MUMPS_10,
	V11_MUMPS_11,
	V11_MUMPS_12,
	V11_MUMPS_13,
	V11_MUMPS_14,
	V11_MUMPS_15,
	V11_MUMPS_16,
	V11_MUMPS_17,
	V11_MUMPS_18,
	V11_MUMPS_19,
	V11_MUMPS_20,
	V11_MUMPS_22,
	V11_MUMPS_23,
	V11_MUMPS_24,
	V11_MUMPS_25,
	V11_MUMPS_26,
	V11_PEDIMENT_01,
	V11_PEDIMENT_02,
	V11_PEDIMENT_03,
	V11_PEDIMENT_04,
	V11_PEDIMENT_05,
	V11_PEDIMENT_06,
	V11_PEDIMENT_07,
	V11_PEDIMENT_08,
	V11_POLONEY_01,
	V11_POLONEY_02,
	V11_POLONEY_03,
	V11_SAUSAGE_01,
	V11_SAUSAGE_02,
	V11_SAUSAGE_03,
	V11_SAUSAGE_04,
	V11_SAUSAGE_05,
	V11_SAUSAGE_06,
	V11_SAUSAGE_07,
	V11_SAUSAGE_08,
	V11_SAUSAGE_09,
	V11_SAUSAGE_10,
	V12_ABLE_01,
	V12_ABLE_02,
	V12_ABLE_03,
	V12_BUSY_01,
	V12_BUSY_02,
	V12_BUSY_03,
	V12_BUSY_04,
	V12_BUSY_05,
	V12_BUSY_06,
	V12_BUSY_07,
	V12_BUSY_08,
	V12_CHARLIE_01,
	V12_CHARLIE_02,
	V12_CONUNDRUM_01,
	V12_CONUNDRUM_02,
	V12_CONUNDRUM_03,
	V12_CONUNDRUM_04,
	V12_CONUNDRUM_05,
	V12_FOXTROT_01,
	V12_FOXTROT_02,
	V12_GUY_01,
	V12_GUY_02,
	V12_GUY_03,
	V12_HAUTE_01,
	V12_HAUTE_02,
	V12_HAUTE_03,
	V12_HAUTE_04,
	V12_HAUTE_05,
	V12_HAUTE_06,
	V12_HOVER_01,
	V12_HOVER_02,
	V12_RUBE_01,
	V12_RUBE_02,
	V12_RUBE_03,
	V12_RUBE_04,
	V12_RUBE_05,
	V12_RUBE_06,
	V12_RUBE_07,
	V12_RUBE_08,
	V12_RUBE_09,
	V12_RUBE_10,
	V12_TEKT_01,
	V12_TEKT_02,
	V12_TEKT_03,
	V12_TEKT_04,
	V12_TEKT_05,
	V12_TEKT_06,
	V12_TEKT_07,
	V13_ANIBLE_01,
	V13_ANIBLE_02,
	V13_FLOWER_01,
	V13_FLOWER_02,
	V13_FOOCHANE_01,
	V13_FOOCHANE_02,
	V13_FOOCHANE_03,
	V13_FOOCHANE_04,
	V13_GHULABUL_01,
	V13_GHULABUL_02,
	V13_GHULABUL_03,
	V13_GHULABUL_04,
	V13_GHULABUL_05,
	V13_GHULABUL_06,
	V13_GHULABUL_07,
	V13_GUY_01,
	V13_GUY_02,
	V13_HAVENGIN_01,
	V13_MELO_01,
	V13_MELO_02,
	V13_MELO_03,
	V13_MITAKO_01,
	V13_MITAKO_02,
	V13_MITAKO_03,
	V13_SPOONIE_01,
	V13_SPOONIE_02,
	V13_SPOONIE_03,
	V13_SPOONIE_04,
	V13_SPOONIE_05,
	V13_ZYGOTE_01,
	V13_ZYGOTE_02,
	V13_ZYGOTE_03,
	V13_ZYGOTE_04,
	V13_ZYGOTE_05,
	V14_LAMORTE_01,
	V14_LAMORTE_02,
	V15_COPROLITE_01,
	V15_COPROLITE_02,
	V15_COPROLITE_03,
	V15_COPROLITE_04,
	V15_COPROLITE_05,
	V15_COPROLITE_06,
	V15_COPROLITE_07,
	V15_COPROLITE_08,
	V15_COPROLITE_09,
	V15_COPROLITE_10,
	V15_COPROLITE_11,
	V15_COPROLITE_12,
	V15_COPROLITE_13,
	V15_COPROLITE_14,
	V15_COPROLITE_15,
	V15_COPROLITE_16,
	V15_COPROLITE_17,
	V15_COPROLITE_18,
	V15_GUY_01,
	V15_GUY_02,
	V15_REED_01,
	V15_REED_02,
	V15_REED_03,
	V16_DYING_01,
	V16_DYING_02,
	V16_GUY_01,
	V16_GUY_02,
	V16_GUY_03,
	V17_ANDERSON_01,
	V17_BARBER_01,
	V17_CAMPBELL_01,
	V17_DEVONSHIRE_01,
	V17_EGGER_01,
	V17_EMERALD_01,
	V17_FLINGBESTER_01,
	V17_GERSHWIN_01,
	V17_GUY_01,
	V17_GUY_02,
	V17_GUY_03,
	V17_GUY_04,
	V17_HEPHASTUS_01,
	V17_INIWA_01,
	V17_INIWA_02,
	V17_INIWA_03,
	V17_KEYCODE_01,
	V17_KEYCODE_02,
	V17_KEYCODE_03,
	V17_KEYCODE_04,
	V17_TOERIG_01,
	V17_TOERIG_02,
	V17_WARNER_01,
	V17_WARNER_02,
	V17_WARNER_03,
	V17_WARNER_04,
	V17_YORP_01,
	V17_YORP_02,
	V18_DERECHA_01,
	V18_GUY_01,
	V18_IZQUIERDA_01,
	V18_IZQUIERDA_02,
	V18_IZQUIERDA_03,
	V18_IZQUIERDA_04,
	V18_IZQUIERDA_05,
	V18_IZQUIERDA_06,
	V19_CAVEAT_01,
	V19_CAVEAT_02,
	V19_ENDER_01,
	V19_ENDER_02,
	V19_FRUSTRATO_01,
	V19_FRUSTRATO_02,
	V19_FRUSTRATO_03,
	V19_FRUSTRATO_04,
	V19_FRUSTRATO_05,
	V19_FRUSTRATO_06,
	V19_FRUSTRATO_07,
	V19_FRUSTRATO_08,
	V19_FRUSTRATO_09,
	V19_FRUSTRATO_10,
	V19_GUY_01,
	V19_GUY_02,
	V19_GUY_03,
	V19_INTER_01,
	V19_INTER_02,
	V19_LONE_01,
	V19_LONE_02,
	V19_LONE_03,
	V19_PHILLIPS_01,
	V19_PHILLIPS_02,
	V19_PHILLIPS_03,
	V19_PHILLIPS_04,
	V19_PHILLIPS_05,
	V19_RAILRIDER_01,
	V19_RAILRIDER_02,
	V19_RAILRIDER_03,
	V19_RAILRIDER_04,
	V19_REDRUM_01,
	V19_REDRUM_02,
	V19_REDRUM_03,
	V19_REDRUM_04,
	V19_REDRUM_05,
	V19_REDRUM_06,
	V19_STANDARD_01,
	V19_STANDARD_02,
	V20_ARNIE_01,
	V20_ARNIE_02,
	V20_BRAD_01,
	V20_BRAD_02,
	V20_BUSTER_01,
	V20_BUSTER_02,
	V20_BUSTER_03,
	V20_COOPER_01,
	V20_COOPER_02,
	V20_DINERS_01,
	V20_DINERS_02,
	V20_DOLLY_01,
	V20_DOREK_01,
	V20_DOREK_02,
	V20_DOREK_03,
	V20_FARBLE_01,
	V20_FARBLE_02,
	V20_GRUNETT_01,
	V20_GRUNETT_02,
	V20_GUY_01,
	V20_GUY_02,
	V20_GUY_03,
	V20_GUY_04,
	V20_GUY_05,
	V20_GUY_06,
	V20_HUMBERT_01,
	V20_ILLIAC_01,
	V20_JACQUES_01,
	V20_JACQUES_02,
	V20_LUMBAR_01,
	V20_MANNY_01,
	V20_POLONEY_01,
	V20_POLONEY_02,
	V20_SARGE_01,
	V20_SARGE_02,
	V20_SARGE_03,
	V20_SARGE_04,
	V20_SARGE_05,
	V21_ANTIMA_01,
	V21_ANTIMA_02,
	V21_ANTIMA_03,
	V21_ANTIMA_04,
	V21_ANTIMA_05,
	V21_ANTIMA_06,
	V21_ANTIMA_07,
	V21_BRAN_ONOMI_01,
	V21_BRAN_ONOMI_02,
	V21_BRAN_ONOMI_03,
	V21_CONTENNE_01,
	V21_CONTENNE_02,
	V21_CONTENNE_03,
	V21_CONTENNE_04,
	V21_CONTENNE_05,
	V21_CONTENNE_06,
	V21_CONTENNE_07,
	V21_COUHOUSE_01,
	V21_COUHOUSE_02,
	V21_COUHOUSE_03,
	V21_DEBENSHIRE_01,
	V21_DEBENSHIRE_02,
	V21_DEBENSHIRE_03,
	V21_DEBENSHIRE_04,
	V21_FEMURIA_01,
	V21_FEMURIA_02,
	V21_FEMURIA_03,
	V21_FEMURIA_04,
	V21_FEMURIA_05,
	V21_GLARATTE_01,
	V21_GLARATTE_02,
	V21_GLARATTE_03,
	V21_GLARATTE_04,
	V21_GUY_01,
	V21_GUY_02,
	V21_GUY_03,
	V21_GUY_04,
	V21_GUY_05,
	V21_GUY_06,
	V21_GUY_07,
	V21_GUY_08,
	V21_GUY_09,
	V21_HYPERBOLE_01,
	V21_KREN_KREN_01,
	V21_KREN_KREN_02,
	V21_KREN_KREN_03,
	V21_KREN_KREN_04,
	V21_KREN_KREN_05,
	V21_LITOTE_01,
	V21_PERIOUS_01,
	V21_PERIOUS_02,
	V21_PERIOUS_03,
	V21_PHAEDRUS_01,
	V21_PHAEDRUS_02,
	V21_PHAEDRUS_04,
	V21_PHAEDRUS_05,
	V21_PHAEDRUS_06,
	V21_PHAEDRUS_07,
	V21_PHAEDRUS_08,
	V21_PHAEDRUS_09,
	V21_PHAEDRUS_10,
	V21_PHAEDRUS_11,
	V21_PHAEDRUS_12,
	V21_PHAEDRUS_13,
	V21_PHAEDRUS_14,
	V21_PHAEDRUS_15,
	V21_PHAEDRUS_16,
	V21_PHAEDRUS_17,
	V21_PHAEDRUS_18,
	V21_PHAEDRUS_19,
	V21_PHAEDRUS_20,
	V21_PHAEDRUS_21,
	V21_PHAEDRUS_22,
	V21_PHAEDRUS_23,
	V21_PHAEDRUS_24,
	V21_PHAEDRUS_25,
	V21_PHAEDRUS_26,
	V21_PHIPOLOSSI_01,
	V21_PHIPOLOSSI_02,
	V21_PHIPOLOSSI_03,
	V21_PHIPOLOSSI_04,
	V21_PHIPOLOSSI_05,
	V21_PHIPOLOSSI_06,
	V21_PLODDER_01,
	V21_PLODDER_02,
	V21_PLODDER_03,
	V21_PLODDER_04,
	V21_SARGE_01,
	V21_SARGE_02,
	V21_SARGE_03,
	V21_SARGE_04,
	V21_SARGE_05,
	V21_SARGE_06,
	V21_SARGE_07,
	V21_SARGE_08,
	V21_SATIRE_01,
	V21_SATIRE_02,
	V21_SHELLEY_01,
	V21_SHELLEY_02,
	V21_SHELLEY_03,
	V21_SHELLEY_04,
	V21_SHELLEY_05,
	V21_VERILITH_01,
	V21_VERILITH_02,
	V21_VERILITH_03,
	V21_VERILITH_04,
	V21_VERILITH_05,
	V22_GUY_01,
	V22_OSCURO_01,
	V22_OSCURO_02,
	V22_VERILITH_01,
	V22_VERILITH_02,
	V22_VERILITH_03,
	V22_VERILITH_04,
	V23_GUY_01,
	V23_GUY_02,
	V23_GUY_03,
	V23_INSANITO_01,
	V23_INSANITO_02,
	V23_INSANITO_03,
	V23_INSANITO_04,
	V23_INSANITO_05,
	V23_VERILITH_01,
	V24_GUY_01,
	V24_GUY_02,
	V24_GUY_03,
	V24_MANAGOTA_01,
	V24_MANAGOTA_02,
	V24_MANAGOTA_03,
	V24_MANAGOTA_04,
	V25_BBOW_01,
	V25_BBOW_02,
	V25_BBOW_03,
	V25_BBOW_04,
	V25_GUY_01,
	V25_GUY_02,
	V25_VERILITH_01,
	V26_GUY_01,
	V26_GUY_02,
	V26_GUY_03,
	V26_GUY_04,
	V26_GUY_05,
	V26_FDOE_01,
	V26_FDOE_02,
	V26_FDOE_03,
	V26_FDOE_04,
	V27_AYONN_01,
	V27_AYONN_02,
	V27_AYONN_03,
	V27_AYONN_04,
	V27_BALE_01,
	V27_BALE_02,
	V27_BALE_03,
	V27_GUY_01,
	V27_GUY_02,
	V27_GUY_03,
	V27_ONABILDEN_01,
	V27_ONABILDEN_02,
	V27_ONABILDEN_03,
	V27_ONABILDEN_04,
	V28_FINN_01,
	V28_FINN_02,
	V28_FINN_03,
	V28_GUY_01,
	V28_GUY_02,
	V28_GUY_03,
	V29_ALERE_01,
	V29_ALERE_02,
	V29_ALERE_03,
	V29_AOLOS_01,
	V29_AOLOS_02,
	V29_AOLOS_03,
	V29_AOLOS_04,
	V29_AOLOS_05,
	V29_GUY_01,
	V29_GUY_02,
	V29_GUY_03,
	V29_GUY_04,
	V29_GUY_05,
	V29_GUY_06,
	V29_GUY_07,
	V29_GUY_08,
	V29_GUY_09,
	V29_GUY_10,
	V29_GUY_11,
	V29_GUY_12,
	V29_GUY_13,
	V29_GUY_14,
	V29_GUY_15,
	V29_KORENAAR_01,
	V29_KORENAAR_02,
	V29_KORENAAR_03,
	V29_KORENAAR_04,
	V29_KORENAAR_05,
	V29_PHARIBOS_01,
	V29_PHARIBOS_02,
	V29_VELEGAL_01,
	V29_VELEGAL_02,
	V30_AOLOS_01,
	V30_AOLOS_02,
	V30_AOLOS_03,
	V30_AOLOS_04,
	V30_AOLOS_05,
	V30_AOLOS_06,
	V30_AOLOS_07,
	V30_AOLOS_08,
	V30_AOLOS_09,
	V30_AOLOS_11,
	V30_AOLOS_12,
	V30_AOLOS_13,
	V30_AOLOS_14,
	V30_AOLOS_15,
	V30_AOLOS_16,
	V30_AOLOS_17,
	V30_AOLOS_18,
	V30_AOLOS_19,
	V30_AOLOS_20,
	V30_CASINOX_01,
	V30_CASINOX_02,
	V30_CASINOX_03,
	V30_CASINOX_04,
	V30_CASINOX_05,
	V30_CASINOX_06,
	V30_CHICKEN_EXPLOSION_01,
	V30_CHICKEN_EXPLOSION_02,
	V30_DOPEFISH_01,
	V30_DOPEFISH_02,
	V30_DOPEFISH_03,
	V30_DOPEFISH_04,
	V30_GUY_01,
	V30_GUY_02,
	V30_JOHN_01,
	V30_JOHN_02,
	V30_JOHN_03,
	V30_JOHN_04,
	V30_JOHN_05,
	V30_LUCAS_01,
	V30_LUCAS_02,
	V30_LUCAS_03,
	V30_LUCAS_04,
	V30_STEVIE_01,
	V30_STEVIE_02,
	V30_STEVIE_03,
	V30_STEVIE_04,
	V30_STEVIE_05,
	V30_STEVIE_06,
	V30_STEVIE_07,
	V30_STEVIE_08,
	V30_STEVIE_09,
	V30_STEVIE_10,
	V30_TOM_01,
	V30_TOM_02,
	V30_TOM_03,
	V30_TOM_04,
	V30_TOM_05,
	V30_TOM_06,
	V30_VERILITH_01,
	V30_VERILITH_02,
	V30_VERILITH_03,
	V30_VERILITH_04,
	V30_VERILITH_05,
	V30_VERILITH_06,
	V30_VERILITH_07,
	OUTRO_DOLLY_01,
	OUTRO_DOLLY_02,
	OUTRO_DOLLY_03,
	OUTRO_DOLLY_04,
	OUTRO_DOLLY_05,
	OUTRO_DOLLY_06,
	OUTRO_DOLLY_07,
	OUTRO_DOLLY_08,
	OUTRO_DOLLY_09,
	OUTRO_DOLLY_10,
	OUTRO_DOLLY_11,
	OUTRO_DOLLY_12,
	OUTRO_DOLLY_13,
	OUTRO_DOLLY_14,
	OUTRO_DOLLY_15,
	OUTRO_DOLLY_16,
	OUTRO_DOLLY_17,
	OUTRO_DOLLY_18,
	OUTRO_DOREK_01,
	OUTRO_DOREK_02,
	OUTRO_DOREK_03,
	OUTRO_DOREK_04,
	OUTRO_DOREK_05,
	OUTRO_DOREK_06,
	OUTRO_GUY_01,
	OUTRO_GUY_02,
	OUTRO_GUY_03,
	OUTRO_GUY_04,
	OUTRO_GUY_05,
	OUTRO_GUY_06,
	OUTRO_JENKINS_01,
	OUTRO_JENKINS_02,
	OUTRO_JENKINS_03,
	OUTRO_JENKINS_04,
	OUTRO_JENKINS_05,
	OUTRO_SARGE_01,
	OUTRO_SARGE_02,
	OUTRO_SARGE_03,
	OUTRO_SARGE_04,
	OUTRO_SARGE_05,
	OUTRO_SARGE_06,
	GUY_GOT_A_JOB,
	GUY_WINS,

	GUY_ENERGY_CELL,
	GUY_COMPLETED,
	GUY_GOT_SOMETHING,
	HDB_INTRO,

	// Demo files
	DEMO_SARGE_01,
	DEMO_SARGE_02,
	DEMO_SARGE_03,
	DEMO_SARGE_04,
	DEMO_SARGE_05,
	DEMO_SARGE_06,
	DEMO_GUY_01,
	DEMO_PILOT_01,
	DEMO_SCIENTIST_01,
	DEMO_SCIENTIST_02,

	LAST_SOUND
};

struct SoundLookUp {
	SoundType idx;
	const char *name;			// name from MSD .h file
	const char *luaName;		// name for Lua code to use
};

struct Voice {
	bool active;
	Audio::SoundHandle handle;

	Voice() : active(false) {}
};

enum SndMem {
	SNDMEM_FREEABLE = -1,
	SNDMEM_NOTCACHED = 0,
	SNDMEM_LOADED = 1
};

enum SndType {
	SNDTYPE_NONE = 0,
	SNDTYPE_WAV = -1,
	SNDTYPE_MP3 = 1,
	SNDTYPE_OGG = 2
};

struct SoundCache {
	SndMem loaded;			// -1 = freeable; in memory, 0 = not cached, 1 = cached
	int size;				// size of sound
	const char *name;		// filename / MSD name
	const char *luaName;	// name used by Lua for i.d.
	SndType ext;			// 0 = Uninitialized, -1 = WAV, 1 = MP3
	byte *data;

	SoundCache() : loaded(SNDMEM_NOTCACHED), size(0), name(nullptr), luaName(nullptr), ext(SNDTYPE_NONE), data(nullptr) {}
};

class Song {
public:
	Song() : _playing(false), _song(SONG_NONE),
		fadingOut(false), fadeOutVol(0), fadeOutRamp(0),
		fadingIn(false), fadeInVol(0), fadeInRamp(0) {}

	void playSong(SoundType song, bool fadeIn, int ramp);
	void fadeOut(int ramp);
	void stop();

	bool isPlaying() const;
	SoundType getSong() const;

	// The update() method is to be used for cross fading, fade-in and fade-out of music
	// However, it's currently unused and only partially implemented.
//	void update();

private:
	static Common::String getFileName(SoundType song);
	Audio::AudioStream* createStream(Common::String fileName);

	Audio::SoundHandle handle;

	bool _playing;
	SoundType _song;

	bool fadingOut;
	int fadeOutVol;
	int	fadeOutRamp;

	bool fadingIn;
	int	fadeInVol;
	int	fadeInRamp;
};

class Sound {
public:

	Sound();
	~Sound();

	void test(); // FIXME. Remove

	void init();
	void save(Common::OutSaveFile *out);
	void loadSaveFile(Common::InSaveFile *in);
	void setVoiceStatus(bool value) {
		_voicesOn = value;
	}
	bool getVoiceStatus() {
		return _voicesOn;
	}
	void clearPersistent() {
		memset(_voicePlayed, 0, NUM_VOICES * sizeof(byte));
	}

	void playSound(int index);
	void playSoundEx(int index, int channel, bool loop);
	void playVoice(int index, int actor);
	void stopVoices() {
		if (_voices[0].active) {
			g_hdb->_mixer->stopHandle(_voices[0].handle);
			_voices[0].active = false;
		}
		if (_voices[1].active) {
			g_hdb->_mixer->stopHandle(_voices[1].handle);
			_voices[1].active = false;
		}
	}
	void startMusic(SoundType song);
	void fadeInMusic(SoundType song, int ramp);
	void fadeOutMusic(int ramp);
	void stopMusic();
	void beginMusic(SoundType song, bool fadeIn, int ramp);
	// The updateMusic() method is to be used for cross fading, fade-in and fade-out of music
	// However, it's currently unused and only partially implemented.
//	void updateMusic();
	bool songPlaying(SoundType song);
	void stopChannel(int channel);
	int registerSound(const char *name);
	void freeSound(int index);
	const char *getSNDLuaName(int index);
	int getSNDIndex(const char *name);
	int getNumSounds() {
		return _numSounds;
	}

	SoundType whatSongIsPlaying();

	void markSoundCacheFreeable();

	// Voice System Variables

	enum {
		GUY,
		OTHER,
		MAX_VOICES
	};

	Voice _voices[MAX_VOICES];

	bool _voicesOn;
	byte _voicePlayed[NUM_VOICES];

	// Music System Variables

	Song _song1, _song2;

	// Sound Caching System Variables

	SoundCache _soundCache[kMaxSounds];
	int _numSounds;
	Audio::SoundHandle _handles[kMaxSNDChannels];

};

} // End of Namespace

#endif // !HDB_SOUND_H
