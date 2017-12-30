/* Copyright (c) 2013-2015 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <mgba/internal/gba/cheats.h>

#include "gba/cheats/gameshark.h"
#include "gba/cheats/parv3.h"
#include <mgba/internal/gba/gba.h>
#include <mgba-util/string.h>

const uint32_t GBACheatGameSharkSeeds[4] = { 0x09F4FBBD, 0x9681884A, 0x352027E9, 0xF3DEE5A7 };

static const uint8_t _gsa1T1[256] = {
	0x31, 0x1C, 0x23, 0xE5, 0x89, 0x8E, 0xA1, 0x37, 0x74, 0x6D, 0x67, 0xFC, 0x1F, 0xC0, 0xB1, 0x94,
	0x3B, 0x05, 0x56, 0x86, 0x00, 0x24, 0xF0, 0x17, 0x72, 0xA2, 0x3D, 0x1B, 0xE3, 0x17, 0xC5, 0x0B,
	0xB9, 0xE2, 0xBD, 0x58, 0x71, 0x1B, 0x2C, 0xFF, 0xE4, 0xC9, 0x4C, 0x5E, 0xC9, 0x55, 0x33, 0x45,
	0x7C, 0x3F, 0xB2, 0x51, 0xFE, 0x10, 0x7E, 0x75, 0x3C, 0x90, 0x8D, 0xDA, 0x94, 0x38, 0xC3, 0xE9,
	0x95, 0xEA, 0xCE, 0xA6, 0x06, 0xE0, 0x4F, 0x3F, 0x2A, 0xE3, 0x3A, 0xE4, 0x43, 0xBD, 0x7F, 0xDA,
	0x55, 0xF0, 0xEA, 0xCB, 0x2C, 0xA8, 0x47, 0x61, 0xA0, 0xEF, 0xCB, 0x13, 0x18, 0x20, 0xAF, 0x3E,
	0x4D, 0x9E, 0x1E, 0x77, 0x51, 0xC5, 0x51, 0x20, 0xCF, 0x21, 0xF9, 0x39, 0x94, 0xDE, 0xDD, 0x79,
	0x4E, 0x80, 0xC4, 0x9D, 0x94, 0xD5, 0x95, 0x01, 0x27, 0x27, 0xBD, 0x6D, 0x78, 0xB5, 0xD1, 0x31,
	0x6A, 0x65, 0x74, 0x74, 0x58, 0xB3, 0x7C, 0xC9, 0x5A, 0xED, 0x50, 0x03, 0xC4, 0xA2, 0x94, 0x4B,
	0xF0, 0x58, 0x09, 0x6F, 0x3E, 0x7D, 0xAE, 0x7D, 0x58, 0xA0, 0x2C, 0x91, 0xBB, 0xE1, 0x70, 0xEB,
	0x73, 0xA6, 0x9A, 0x44, 0x25, 0x90, 0x16, 0x62, 0x53, 0xAE, 0x08, 0xEB, 0xDC, 0xF0, 0xEE, 0x77,
	0xC2, 0xDE, 0x81, 0xE8, 0x30, 0x89, 0xDB, 0xFE, 0xBC, 0xC2, 0xDF, 0x26, 0xE9, 0x8B, 0xD6, 0x93,
	0xF0, 0xCB, 0x56, 0x90, 0xC0, 0x46, 0x68, 0x15, 0x43, 0xCB, 0xE9, 0x98, 0xE3, 0xAF, 0x31, 0x25,
	0x4D, 0x7B, 0xF3, 0xB1, 0x74, 0xE2, 0x64, 0xAC, 0xD9, 0xF6, 0xA0, 0xD5, 0x0B, 0x9B, 0x49, 0x52,
	0x69, 0x3B, 0x71, 0x00, 0x2F, 0xBB, 0xBA, 0x08, 0xB1, 0xAE, 0xBB, 0xB3, 0xE1, 0xC9, 0xA6, 0x7F,
	0x17, 0x97, 0x28, 0x72, 0x12, 0x6E, 0x91, 0xAE, 0x3A, 0xA2, 0x35, 0x46, 0x27, 0xF8, 0x12, 0x50
};

static const uint8_t _gsa1T2[256] = {
	0xD8, 0x65, 0x04, 0xC2, 0x65, 0xD5, 0xB0, 0x0C, 0xDF, 0x9D, 0xF0, 0xC3, 0x9A, 0x17, 0xC9, 0xA6,
	0xE1, 0xAC, 0x0D, 0x14, 0x2F, 0x3C, 0x2C, 0x87, 0xA2, 0xBF, 0x4D, 0x5F, 0xAC, 0x2D, 0x9D, 0xE1,
	0x0C, 0x9C, 0xE7, 0x7F, 0xFC, 0xA8, 0x66, 0x59, 0xAC, 0x18, 0xD7, 0x05, 0xF0, 0xBF, 0xD1, 0x8B,
	0x35, 0x9F, 0x59, 0xB4, 0xBA, 0x55, 0xB2, 0x85, 0xFD, 0xB1, 0x72, 0x06, 0x73, 0xA4, 0xDB, 0x48,
	0x7B, 0x5F, 0x67, 0xA5, 0x95, 0xB9, 0xA5, 0x4A, 0xCF, 0xD1, 0x44, 0xF3, 0x81, 0xF5, 0x6D, 0xF6,
	0x3A, 0xC3, 0x57, 0x83, 0xFA, 0x8E, 0x15, 0x2A, 0xA2, 0x04, 0xB2, 0x9D, 0xA8, 0x0D, 0x7F, 0xB8,
	0x0F, 0xF6, 0xAC, 0xBE, 0x97, 0xCE, 0x16, 0xE6, 0x31, 0x10, 0x60, 0x16, 0xB5, 0x83, 0x45, 0xEE,
	0xD7, 0x5F, 0x2C, 0x08, 0x58, 0xB1, 0xFD, 0x7E, 0x79, 0x00, 0x34, 0xAD, 0xB5, 0x31, 0x34, 0x39,
	0xAF, 0xA8, 0xDD, 0x52, 0x6A, 0xB0, 0x60, 0x35, 0xB8, 0x1D, 0x52, 0xF5, 0xF5, 0x30, 0x00, 0x7B,
	0xF4, 0xBA, 0x03, 0xCB, 0x3A, 0x84, 0x14, 0x8A, 0x6A, 0xEF, 0x21, 0xBD, 0x01, 0xD8, 0xA0, 0xD4,
	0x43, 0xBE, 0x23, 0xE7, 0x76, 0x27, 0x2C, 0x3F, 0x4D, 0x3F, 0x43, 0x18, 0xA7, 0xC3, 0x47, 0xA5,
	0x7A, 0x1D, 0x02, 0x55, 0x09, 0xD1, 0xFF, 0x55, 0x5E, 0x17, 0xA0, 0x56, 0xF4, 0xC9, 0x6B, 0x90,
	0xB4, 0x80, 0xA5, 0x07, 0x22, 0xFB, 0x22, 0x0D, 0xD9, 0xC0, 0x5B, 0x08, 0x35, 0x05, 0xC1, 0x75,
	0x4F, 0xD0, 0x51, 0x2D, 0x2E, 0x5E, 0x69, 0xE7, 0x3B, 0xC2, 0xDA, 0xFF, 0xF6, 0xCE, 0x3E, 0x76,
	0xE8, 0x36, 0x8C, 0x39, 0xD8, 0xF3, 0xE9, 0xA6, 0x42, 0xE6, 0xC1, 0x4C, 0x05, 0xBE, 0x17, 0xF2,
	0x5C, 0x1B, 0x19, 0xDB, 0x0F, 0xF3, 0xF8, 0x49, 0xEB, 0x36, 0xF6, 0x40, 0x6F, 0xAD, 0xC1, 0x8C
};

// http://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
void GBACheatDecryptGameShark(uint32_t* op1, uint32_t* op2, const uint32_t* seeds) {
	uint32_t sum = 0xC6EF3720;
	int i;
	for (i = 0; i < 32; ++i) {
		*op2 -= ((*op1 << 4) + seeds[2]) ^ (*op1 + sum) ^ ((*op1 >> 5) + seeds[3]);
		*op1 -= ((*op2 << 4) + seeds[0]) ^ (*op2 + sum) ^ ((*op2 >> 5) + seeds[1]);
		sum -= 0x9E3779B9;
	}
}

void GBACheatReseedGameShark(uint32_t* seeds, uint16_t params, const uint8_t* t1, const uint8_t* t2) {
	int x, y;
	int s0 = params >> 8;
	int s1 = params & 0xFF;
	for (y = 0; y < 4; ++y) {
		for (x = 0; x < 4; ++x) {
			uint8_t z = t1[(s0 + x) & 0xFF] + t2[(s1 + y) & 0xFF];
			seeds[y] <<= 8;
			seeds[y] |= z;
		}
	}
}

void GBACheatSetGameSharkVersion(struct GBACheatSet* cheats, enum GBACheatGameSharkVersion version) {
	cheats->gsaVersion = version;
	switch (version) {
	case GBA_GS_GSAV1:
	case GBA_GS_GSAV1_RAW:
		memcpy(cheats->gsaSeeds, GBACheatGameSharkSeeds, 4 * sizeof(uint32_t));
		break;
	case GBA_GS_PARV3:
	case GBA_GS_PARV3_RAW:
		memcpy(cheats->gsaSeeds, GBACheatProActionReplaySeeds, 4 * sizeof(uint32_t));
		break;
	default:
		break;
	}
}

bool GBACheatAddGameSharkRaw(struct GBACheatSet* cheats, uint32_t op1, uint32_t op2) {
	enum GBAGameSharkType type = op1 >> 28;
	struct mCheat* cheat = 0;

	if (cheats->incompleteCheat != COMPLETE) {
		struct mCheat* incompleteCheat = mCheatListGetPointer(&cheats->d.list, cheats->incompleteCheat);
		if (cheats->remainingAddresses > 0) {
			cheat = mCheatListAppend(&cheats->d.list);
			cheat->type = CHEAT_ASSIGN;
			cheat->width = 4;
			cheat->address = op1;
			cheat->operand = incompleteCheat->operand;
			cheat->repeat = 1;
			--cheats->remainingAddresses;
		}
		if (cheats->remainingAddresses > 0) {
			cheat = mCheatListAppend(&cheats->d.list);
			cheat->type = CHEAT_ASSIGN;
			cheat->width = 4;
			cheat->address = op2;
			cheat->operand = incompleteCheat->operand;
			cheat->repeat = 1;
			--cheats->remainingAddresses;
		}
		if (cheats->remainingAddresses == 0) {
			cheats->incompleteCheat = COMPLETE;
		}
		return true;
	}

	switch (type) {
	case GSA_ASSIGN_1:
		cheat = mCheatListAppend(&cheats->d.list);
		cheat->type = CHEAT_ASSIGN;
		cheat->width = 1;
		cheat->address = op1 & 0x0FFFFFFF;
		break;
	case GSA_ASSIGN_2:
		cheat = mCheatListAppend(&cheats->d.list);
		cheat->type = CHEAT_ASSIGN;
		cheat->width = 2;
		cheat->address = op1 & 0x0FFFFFFF;
		break;
	case GSA_ASSIGN_4:
		cheat = mCheatListAppend(&cheats->d.list);
		cheat->type = CHEAT_ASSIGN;
		cheat->width = 4;
		cheat->address = op1 & 0x0FFFFFFF;
		break;
	case GSA_ASSIGN_LIST:
		cheats->remainingAddresses = (op1 & 0xFFFF) - 1;
		cheat = mCheatListAppend(&cheats->d.list);
		cheat->type = CHEAT_ASSIGN;
		cheat->width = 4;
		cheat->address = op2;
		cheats->incompleteCheat = mCheatListIndex(&cheats->d.list, cheat);
		break;
	case GSA_PATCH:
		cheats->romPatches[0].address = BASE_CART0 | ((op1 & 0xFFFFFF) << 1);
		cheats->romPatches[0].newValue = op2;
		cheats->romPatches[0].applied = false;
		cheats->romPatches[0].exists = true;
		return true;
	case GSA_BUTTON:
		switch (op1 & 0x00F00000) {
		case 0x00100000:
			cheat = mCheatListAppend(&cheats->d.list);
			cheat->type = CHEAT_IF_BUTTON;
			cheat->repeat = 1;
			cheat->negativeRepeat = 0;
			cheat = mCheatListAppend(&cheats->d.list);
			cheat->type = CHEAT_ASSIGN;
			cheat->width = 1;
			cheat->address = op1 & 0x0F0FFFFF;
			break;
		case 0x00200000:
			cheat = mCheatListAppend(&cheats->d.list);
			cheat->type = CHEAT_IF_BUTTON;
			cheat->repeat = 1;
			cheat->negativeRepeat = 0;
			cheat = mCheatListAppend(&cheats->d.list);
			cheat->type = CHEAT_ASSIGN;
			cheat->width = 2;
			cheat->address = op1 & 0x0F0FFFFF;
			break;
		default:
			mLOG(CHEATS, STUB, "GameShark button type unimplemented");
			return false;
		}
		break;
	case GSA_IF_EQ:
		if (op1 == 0xDEADFACE) {
			GBACheatReseedGameShark(cheats->gsaSeeds, op2, _gsa1T1, _gsa1T2);
			return true;
		}
		cheat = mCheatListAppend(&cheats->d.list);
		cheat->type = CHEAT_IF_EQ;
		cheat->width = 2;
		cheat->address = op1 & 0x0FFFFFFF;
		break;
	case GSA_IF_EQ_RANGE:
		cheat = mCheatListAppend(&cheats->d.list);
		cheat->type = CHEAT_IF_EQ;
		cheat->width = 2;
		cheat->address = op2 & 0x0FFFFFFF;
		cheat->operand = op1 & 0xFFFF;
		cheat->repeat = (op1 >> 16) & 0xFF;
		cheat->negativeRepeat = 0;
		return true;
	case GSA_HOOK:
		if (cheats->hook) {
			return false;
		}
		cheats->hook = malloc(sizeof(*cheats->hook));
		cheats->hook->address = BASE_CART0 | (op1 & (SIZE_CART0 - 1));
		cheats->hook->mode = MODE_THUMB;
		cheats->hook->refs = 1;
		cheats->hook->reentries = 0;
		return true;
	default:
		return false;
	}
	cheat->operand = op2;
	cheat->repeat = 1;
	cheat->negativeRepeat = 0;
	return true;
}

bool GBACheatAddGameShark(struct GBACheatSet* set, uint32_t op1, uint32_t op2) {
	uint32_t o1 = op1;
	uint32_t o2 = op2;
	char line[18] = "XXXXXXXX XXXXXXXX";
	snprintf(line, sizeof(line), "%08X %08X", op1, op2);

	switch (set->gsaVersion) {
	default:
		GBACheatSetGameSharkVersion(set, GBA_GS_GSAV1);
	// Fall through
	case GBA_GS_GSAV1:
		GBACheatDecryptGameShark(&o1, &o2, set->gsaSeeds);
	// Fall through
	case GBA_GS_GSAV1_RAW:
		return GBACheatAddGameSharkRaw(set, o1, o2);
	}
	return false;
}

bool GBACheatAddGameSharkLine(struct GBACheatSet* cheats, const char* line) {
	uint32_t op1;
	uint32_t op2;
	line = hex32(line, &op1);
	if (!line) {
		return false;
	}
	while (*line == ' ') {
		++line;
	}
	line = hex32(line, &op2);
	if (!line) {
		return false;
	}
	return GBACheatAddGameShark(cheats, op1, op2);
}

int GBACheatGameSharkProbability(uint32_t op1, uint32_t op2) {
	int probability = 0;
	if (op2 == 0x001DC0DE) {
		return 0x100;
	}
	uint32_t address = op1 & 0x0FFFFFFF;
	switch (op1 >> 28) {
	case GSA_ASSIGN_1:
		probability += 0x20;
		if (op2 & 0xFFFFFF00) {
			probability -= 0x10;
		}
		probability += GBACheatAddressIsReal(address);
		break;
	case GSA_ASSIGN_2:
		probability += 0x20;
		if (op2 & 0xFFFF0000) {
			probability -= 0x10;
		}
		probability += GBACheatAddressIsReal(address);
		break;
	case GSA_ASSIGN_4:
		probability += 0x20;
		probability += GBACheatAddressIsReal(address);
		break;
	case GSA_PATCH:
		probability += 0x20;
		if (op2 & 0xCFFF0000) {
			probability -= 0x10;
		}
		break;
	case GSA_BUTTON:
		probability += 0x10;
		break;
	case GSA_IF_EQ:
		probability += 0x20;
		if (op2 & 0xFFFF0000) {
			probability -= 0x10;
		}
		probability += GBACheatAddressIsReal(address);
		break;
	case GSA_IF_EQ_RANGE:
		probability += 0x20;
		probability += GBACheatAddressIsReal(op2);
		if (op1 & 0x0F000000) {
			probability -= 0x10;
		}
		break;
	case GSA_HOOK:
		probability += 0x20;
		if (op2 & 0xFFFF0000) {
			probability -= 0x10;
		}
		break;
	default:
		probability -= 0x40;
	}
	return probability;
}
