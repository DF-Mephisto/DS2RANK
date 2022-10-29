#pragma once

#define NONE	0

//STATS
#define STAT_offset	0x390
#define VGR_offset	0xf8
#define END_offset	0xfa
#define VIT_offset	0xfc
#define ATN_offset	0xfe
#define STR_offset	0x100
#define DEX_offset	0x102
#define ADP_offset	0x108
#define INT_offset	0x104
#define FTH_offset	0x106

//WEAPONS
#define WEP_offset		0x480
#define R1_offset		0x390
#define R2_offset		0x3b8
#define R3_offset		0x3e0
#define L1_offset		0x37c
#define L2_offset		0x3a4
#define L3_offset		0x3cc

//ENCHANTMENTS
#define R1_ench_offset	0x39a
#define R2_ench_offset	0x3c2
#define R3_ench_offset	0x3ea
#define L1_ench_offset	0x386
#define L2_ench_offset	0x3ae
#define L3_ench_offset	0x3d6

//INFUSIONS
#define R1_inf_offset	0x39b
#define R2_inf_offset	0x3c3
#define R3_inf_offset	0x3eb
#define L1_inf_offset	0x387
#define L2_inf_offset	0x3af
#define L3_inf_offset	0x3d7

//ARMOR
#define ARM_offset		0x7f0
#define HEAD_offset		0x84
#define BODY_offset		0x98
#define HANDS_offset	0xac
#define LEGS_offset		0xc0

//ARMOR ENCHANTMENTS
#define HEAD_ench_offset		0x8e
#define BODY_ench_offset		0xa2
#define HANDS_ench_offset		0xb6
#define LEGS_ench_offset		0xca

//RINGS
#define RING_offset		0x480
#define RING1_offset	0x4bc
#define RING2_offset	0x4d0
#define RING3_offset	0x4e4
#define RING4_offset	0x4f8

//SPELLS
#define SPELL_offset	0x7e8
#define SP1_offset		0x294
#define SPINCR_offset	0x14

//CONSUMABLES
#define CONS_offset		0x7f0
#define CONS1_offset	0x19c
#define CONSINCR_offset	0x14

//RANKS
#define BLOODBRO_offset		0x1c8
#define BLOODBRO_offset_v	0x1c4
#define SENTINELS_offset	0x1c6
#define SENTINELS_offset_v	0x1c2
#define DRAGON_offset		0x1d0
#define DRAGON_offset_v		0x1cc
#define BELLBONGS_offset	0x1ce
#define BELLBONGS_offset_v	0x1ca

//RINGS COUNT
#define THORN_offset		0x1e8
#define DRAGONCREST_offset	0x1e0

enum EnemyType
{
	errorHostil = 30000,
	arenaHostil = 4113,
	hostHostil = 0
};
