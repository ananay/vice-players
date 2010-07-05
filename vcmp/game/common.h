//----------------------------------------------------
//
// GPL code license:
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author: kyeman
//
//-----------------------------------------------------------

#pragma once

#include "../../raknet/WindowsIncludes.h"

//-----------------------------------------------------------

#define MAX_PLAYERS		50
#define MAX_VEHICLES	200

//-----------------------------------------------------------

typedef struct _RGBA {
	unsigned char r,g,b,a;
} RGBA, *PRGBA;

typedef struct _VECTOR {
	float X,Y,Z;
} VECTOR, *PVECTOR;

typedef struct _MATRIX4X4 {
	VECTOR vLookRight;
	float  pad_r;
	VECTOR vLookUp;
	float  pad_u;
	VECTOR vLookAt;
	float  pad_a;
	VECTOR vPos;
	float  pad_p;
} MATRIX4X4, *PMATRIX4X4;

//-----------------------------------------------------------

#define _pad(x,y) BYTE x[y]

typedef struct _PLACEABLE {
	MATRIX4X4 matMatrix;
	DWORD     pMatrix;
	DWORD     dwHasMatrix;
} PLACEABLE;

typedef struct _ENTITY_TYPE {
	DWORD	  func_table;     // 00-04
	PLACEABLE placeable;      // 04-4C
	DWORD *   pRWObject;      // 4C-50
	BYTE	  nControlFlags;  // 50-51
	BYTE	  nControlFlags2; // 51-52
	BYTE      byteUnkFlags1;  // 52-53
	BYTE      byteUnkFlags2;  // 53-54
	_pad(__pad1, 0x8);        // 54-5C
	WORD	  wModelIndex;    // 5C-5E
	_pad(__pad2, 0x6);        // 5E-64
} ENTITY_TYPE;

typedef struct _PHYSICAL_TYPE {
	ENTITY_TYPE entity;          // 000-064
	_pad(__pad0a, 0xC);          // 064-070
	VECTOR      vecMoveSpeed;    // 070-07C
	VECTOR      vecTurnSpeed;    // 07C-088
	_pad(__pad1a, 0x92);         // 088-11A
	BYTE        byteSunkFlags;   // 11A-11B
	BYTE	    byteLockedFlags; // 11B-11C
	_pad(__pad2a, 0x4);          // 11C-120
} PHYSICAL_TYPE;

typedef struct _PED_TYPE {
	PHYSICAL_TYPE physical;        // 000-120
	_pad(__pad0b, 0x2C);           // 120-14C
	BYTE	      byteShootFlags;  // 14C-14D
	BYTE	      byteJumpFlags;   // 14D-14E
	_pad(__pad1b, 0x22);           // 14E-170
	DWORD *       pTargetEntity;   // 170-174
	_pad(__pad2b, 0xD0);           // 174-244
	BYTE	      byteAction;      // 244-245
	_pad(__pad3b, 0x10F);          // 245-354
	float	      fHealth;         // 354-358
	float	      fArmour;         // 358-35C
	_pad(__pad4b, 0x18);           // 35C-374
	float	      fRotation1;      // 374-378
	float	      fRotation2;      // 378-37C
	_pad(__pad5b, 0x2C);           // 37C-3A8
	DWORD *       pVehicle;        // 3A8-3AC
	BYTE	      byteIsInVehicle; // 3AC-3AD
	_pad(__pad6b, 0x27);           // 3AD-3D4
	BYTE	      bytePedType;     // 3D4-3D5
	_pad(__pad7b, 0x1C3);          // 3D5-598
	DWORD         dwWeaponUsed;    // 598-59C
	DWORD *       pDamageEntity;   // 59C-5A0
	_pad(__pad8b, 0x54);           // 5A0-5F4
} PED_TYPE;

typedef struct _VEHICLE_TYPE {
	PHYSICAL_TYPE physical;           // 000-120
	_pad(__pad0b, 0x80);              // 120-1A0
	BYTE          byteColors[4];      // 1A0-1A4
	_pad(__pad1b, 0x4);               // 1A4-1A8
	PED_TYPE *    pDriver;            // 1A8-1AC
	PED_TYPE *    pPassengers[8];     // 1AC-1CC
	BYTE	      bytePassengerCount; // 1CC-1CD
	_pad(__pad3b, 0x3);               // 1CD-1D0
	BYTE	      byteMaxPassengers;  // 1D0-1D1
	_pad(__pad4b, 0x17);              // 1D1-1E8
	float	      fSteerAngle1;       // 1E8-1EC
	float	      fSteerAngle2;       // 1EC-1F0
	float	      fAcceleratorPedal;  // 1F0-1F4
	float	      fBrakePedal;        // 1F4-1F8
	_pad(__pad5b, 0xC);               // 1F8-204
	float	      fHealth;            // 204-208
	_pad(__pad6b, 0x28);              // 208-230
	DWORD	      dwDoorsLocked;      // 230-234
	DWORD         dwWeaponUsed;       // 234-238
	DWORD *       pDamageEntity;      // 238-23C
	DWORD	      nRadio;             // 23C-240
	BYTE	      byteHorn;           // 240-241
	DWORD         dwUnk1;             // 241-245
	BYTE	      byteSiren;          // 245-246
	_pad(__pad7b, 0x5A);              // 246-2A0
	// End of CVehicle
	// CAutomobile stuff
	_pad(__pad8b, 0x310);             // 2A0-5B0
	float	      fSpecialWeaponRotation1; // 5B0-5B4 (following 2 are rhino turret and firetruck spray)
	float	      fSpecialWeaponRotation2; // 5B4-5B8
	/// ............
} VEHICLE_TYPE;

//-----------------------------------------------------------

#define WEATHER_SUNNY		0
#define WEATHER_CLOUDY		1
#define WEATHER_RAINING		2
#define WEATHER_FOGGY		3
#define WEATHER_EXTRASUNNY	4
#define WEATHER_STORM		5
#define WEATHER_INTERIOR	6

//-----------------------------------------------------------

#define INTERIOR_OUTSIDE	0
#define INTERIOR_HOTEL		1
#define INTERIOR_MANSION	2
#define INTERIOR_BANK		3
#define INTERIOR_MALL		4
#define INTERIOR_STRIPCLUB	5
#define INTERIOR_LAWYERS	6
#define INTERIOR_CAFEROBINA	7
#define INTERIOR_CONCERT	8
#define INTERIOR_STUDIO		9
#define INTERIOR_AMMUNATION	10
#define INTERIOR_APPARTMENT	11
#define INTERIOR_POLICEHQ	12
#define INTERIOR_UNKNOWN	12
#define INTERIOR_STADIUM1	14
#define INTERIOR_STADIUM2	15
#define INTERIOR_STADIUM3	16
#define INTERIOR_CLUB		17
#define INTERIOR_PRINTWORKS	18

//-----------------------------------------------------------

#define FADE_OUT			0
#define FADE_IN				1

//-----------------------------------------------------------

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5

//-----------------------------------------------------------

#define ACTION_NORMAL 1
#define ACTION_DRIVING_VEHICLE 50
#define ACTION_WASTED 55
#define ACTION_GETTING_IN_VEHICLE 58
#define ACTION_EXITING_VEHICLE 60

//-----------------------------------------------------------

#define WEAPON_MODEL_CELLPHONE			258
#define WEAPON_MODEL_BRASSKNUCKLE		259
#define WEAPON_MODEL_SCREWDRIVER		260
#define WEAPON_MODEL_GOLFCLUB			261
#define WEAPON_MODEL_NITESTICK			262
#define WEAPON_MODEL_KNIFECUR			263
#define WEAPON_MODEL_BASEBALL_BAT		264
#define WEAPON_MODEL_HAMMER				265
#define WEAPON_MODEL_CLEAVER			266
#define WEAPON_MODEL_MACHETE			267
#define WEAPON_MODEL_KATANA				268
#define WEAPON_MODEL_CHAINSAW			269
#define WEAPON_MODEL_GRENADE			270
#define WEAPON_MODEL_TEARGAS			271
#define WEAPON_MODEL_MOLOTOV			272
#define WEAPON_MODEL_MISSILE			273
#define WEAPON_MODEL_COLT45				274
#define WEAPON_MODEL_PYTHON				275
#define WEAPON_MODEL_RUGER				276
#define WEAPON_MODEL_CHROMEGUN			277
#define WEAPON_MODEL_SHOTGSPA			278
#define WEAPON_MODEL_BUDDYSHOT			279
#define WEAPON_MODEL_M4					280
#define WEAPON_MODEL_TEC9				281
#define WEAPON_MODEL_UZI				282
#define WEAPON_MODEL_INGRAMSL			283
#define WEAPON_MODEL_MP5LNG				284
#define WEAPON_MODEL_SNIPER				285
#define WEAPON_MODEL_LASER				286
#define WEAPON_MODEL_ROCKETLA			287
#define WEAPON_MODEL_FLAME				288
#define WEAPON_MODEL_M60				289
#define WEAPON_MODEL_MINIGUN			290
#define WEAPON_MODEL_BOMB				291
#define WEAPON_MODEL_CAMERA				292
#define WEAPON_MODEL_FINGERS			293
#define WEAPON_MODEL_MINIGUN2			294
#define WEAPON_UNARMED					0
#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_SCREWDRIVER				2
#define WEAPON_GOLFCLUB					3
#define WEAPON_NITESTICK				4
#define WEAPON_KNIFECUR					5
#define WEAPON_BASEBALL_BAT				6
#define WEAPON_HAMMER					7
#define WEAPON_CLEAVER					8
#define WEAPON_MACHETE					9
#define WEAPON_KATANA					10
#define WEAPON_CHAINSAW					11
#define WEAPON_GRENADE					12
#define WEAPON_TEARGAS					14
#define WEAPON_MOLOTOV					15
#define WEAPON_MISSILE					16
#define WEAPON_COLT45					17
#define WEAPON_PYTHON					18
#define WEAPON_CHROMEGUN				19
#define WEAPON_SHOTGSPA					20
#define WEAPON_BUDDYSHOT				21
#define WEAPON_TEC9						22
#define WEAPON_UZI						23
#define WEAPON_INGRAMSL					24
#define WEAPON_MP5LNG					25
#define WEAPON_M4						26
#define WEAPON_RUGER					27
#define WEAPON_SNIPER					28
#define WEAPON_LASER					29
#define WEAPON_ROCKETLA					30
#define WEAPON_FLAME					31
#define WEAPON_M60						32
#define WEAPON_MINIGUN					33
#define WEAPON_BOMB						34
#define WEAPON_HELICANNON				35
#define WEAPON_CAMERA					36
#define WEAPON_COLLISION				39
#define WEAPON_FALL						41
#define WEAPON_DRIVEBY					42
#define WEAPON_DROWN					43
#define WEAPON_WATER					50
#define WEAPON_EXPLOSION				51

//-----------------------------------------------------------
