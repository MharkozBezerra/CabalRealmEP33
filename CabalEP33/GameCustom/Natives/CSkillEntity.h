#pragma once
#include "Base.h"

struct CSkillStuff
{
	int vTable;                                   //  0x00000
	int var_00004;                                   //  0x00004
	int var_00008;                                   //  0x00008
	int var_0000C;                                   //  0x0000C
	int var_00010;                                   //  0x00010
	int var_00014;                                   //  0x00014
	int var_00018;                                   //  0x00018
	int var_0001C;                                   //  0x0001C
	int var_00020;                                   //  0x00020
	int var_00024;                                   //  0x00024
	int var_00028;                                   //  0x00028
	int var_0002C;                                   //  0x0002C
	int var_00030;                                   //  0x00030
	int var_00034;                                   //  0x00034
	int var_00038;                                   //  0x00038
	int var_0003C;                                   //  0x0003C
	int var_00040;                                   //  0x00040
	int var_00044;                                   //  0x00044
	int var_00048;                                   //  0x00048
	int var_0004C;                                   //  0x0004C
	int var_00050;                                   //  0x00050
	int var_00054;                                   //  0x00054
	int var_00058;                                   //  0x00058
	int var_0005C;                                   //  0x0005C
	int var_00060;                                   //  0x00060
	int var_00064;                                   //  0x00064
	int var_00068;                                   //  0x00068
	int var_0006C;                                   //  0x0006C
	int var_00070;                                   //  0x00070
	int var_00074;                                   //  0x00074
	int var_00078;                                   //  0x00078
	int var_0007C;                                   //  0x0007C
	int var_00080;                                   //  0x00080
	int var_00084;                                   //  0x00084
	int var_00088;                                   //  0x00088
	int var_0008C;                                   //  0x0008C
	int var_00090;                                   //  0x00090
	int var_00094;                                   //  0x00094
	int var_00098;                                   //  0x00098
	int var_0009C;                                   //  0x0009C
	int var_000A0;                                   //  0x000A0
	int value_ref;                                   //  0x000A4
	int var_000A8;                                   //  0x000A8
	int var_000AC;                                   //  0x000AC
	int var_000B0;                                   //  0x000B0
	int iSkillIdx;                                   //  0x000B4
	int iGroup;										 //  0x000B8
	int iItensity;                                   //  0x000BC
	int var_000C0;                                   //  0x000C0
	unsigned short cooltimeIdx;                      //  0x000C4
	unsigned short var_000C6;                        //  0x000C6
	int var_000C8;                                   //  0x000C8
	int var_000CC;                                   //  0x000CC
	int var_000D0;                                   //  0x000D0
	int durCoef[2];									 //  0x000D4
	int var_000DC;                                   //  0x000DC
	int var_000E0;                                   //  0x000E0
	int var_000E4;                                   //  0x000E4
	int var_000E8;                                   //  0x000E8
	int target;										 //  0x000EC
	int max_target;                                  //  0x000F0
	int var_000F4;                                   //  0x000F4
	int anicode;                                     //  0x000F8
	int iFxcode_on;                                  //  0x000FC
	int iFxcode_keep;                                //  0x00100
	int expCoef[2];                                  //  0x00104
	int var_0010C;                                   //  0x0010C
	int var_00110;                                   //  0x00110
	int var_00114;                                   //  0x00114
	int var_00118;                                   //  0x00118
	int mpCoef[2];                                   //  0x0011C
	int sp;											 //  0x00124
	int var_00128;                                   //  0x00128
	int var_0012C;                                   //  0x0012C
	int var_00130;                                   //  0x00130
	int var_00134;                                   //  0x00134
	int var_00138;                                   //  0x00138
	int var_0013C;                                   //  0x0013C
	int var_00140;                                   //  0x00140
	int var_00144;                                   //  0x00144
	int var_00148;                                   //  0x00148
	int var_0014C;                                   //  0x0014C
	int usecase;                                     //  0x00150
	int reach;										 //  0x00154
	int mreach;                                   //  0x00158
	int range;										 //  0x0015C
	int iTerm;										 //  0x00160
	int iBlendlimit;                                 //  0x00164
	int iHit_frame;                                  //  0x00168
	int iFiring_frame;                               //  0x0016C
	int iAni_f;                                      //  0x00170
	int iLocal_f;                                    //  0x00174
	int var_00178;                                   //  0x00178
	int var_0017C;                                   //  0x0017C
	int var_00180;                                   //  0x00180

	int EncodeSkillData(int data) { return (data + 0x0059077C) ^ 0x2147E4; }
	int DecodeSkillData(int data) { return (data ^ 0x2147E4) - 0x0059077C; }
	int GetRange() { return DecodeSkillData(range); }
	int GetReach() { return DecodeSkillData(reach); }
	int GetMReach() { return DecodeSkillData(mreach); }
};