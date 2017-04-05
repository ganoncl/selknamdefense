//Copyright 2017 Sebastian Gana, Rodrigo Alarcon, Walter Berendsen y Javier Gonzalez
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include "IwGx.h"
#include "common.h"
#include "parserhelper.h"
#include "stdio.h"
#include "isoterrain.h"
#include "isoprop.h"
#include "game.h"
#include "iostream.h"
#include "survivalmodeconfig.h"
using namespace std;

ParserHelper::ParserHelper()
{

}

ParserHelper::~ParserHelper()
{

}

//Unit Atributes Parser.
void ParserHelper::RetrievePawnsAttributesFromFile()
{
	//unit properties
	int tmpType = -1;
	int tmpSelknamBehavior = -1;
	int tmpEnemyBehavior = -1;
	int tmpSkillType = -1;

	PawnAttributes				tmpPawnAtributes;

	int tmpTotalPawns = -1;

	//skill properties
	Skill_Poison				tmpSkill_Att_Poison;
	Skill_Slow					tmpSkill_Att_Slow;
	Skill_Stun					tmpSkill_Att_Stun;
	Skill_Root					tmpSkill_Att_Root;
	Skill_Confusion				tmpSkill_Att_Confusion;
	Skill_Explode				tmpSkill_Att_Explode;
	Skill_PoisonOnDeath			tmpSkill_Att_PoisonOnDeath;
	Skill_Heal					tmpSkill_Att_Heal;
	Skill_Might					tmpSkill_Att_Might;
	Skill_Haste					tmpSkill_Att_Haste;
	int tmpTotalSkillsLevel = -1;

	s3eFile* fileHandle = s3eFileOpen("unit_atributes.sdm", "rb");
	if(!fileHandle)
		return;
	static int32 fileSize = s3eFileGetSize(fileHandle);
	int dataIntToRead = 0;
	int readerPos = 0;
	
	while (readerPos < fileSize)
	{

		//total pawns num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalPawns = dataIntToRead;
		IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalPawns %i", tmpTotalPawns));
		readerPos += sizeof(int);
		//handling pawns atributes
		for (int i = 0; i < tmpTotalPawns; i++)
		{
			//type
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpType = dataIntToRead;
			tmpType++;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpType %i", tmpType));
			readerPos += sizeof(int);

			//selknam behavior
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSelknamBehavior = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSelknamBehavior %i", tmpSelknamBehavior));
			readerPos += sizeof(int);
			tmpPawnAtributes.sBehavior = Pawn_SelknamBehavior(tmpSelknamBehavior);
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.sBehavior %i", tmpPawnAtributes.sBehavior));


			//enemy behavior
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpEnemyBehavior = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpEnemyBehavior %i", tmpEnemyBehavior));
			readerPos += sizeof(int);
			tmpPawnAtributes.eBehavior = Pawn_EnemyBehavior(tmpEnemyBehavior);
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.eBehavior %i", tmpPawnAtributes.eBehavior));

			//level
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.level = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.level %i", tmpPawnAtributes.level));
			readerPos += sizeof(int);

			//health
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.health = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.health %i", tmpPawnAtributes.health));
			readerPos += sizeof(int);

			//damage
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.damage = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.damage %i", tmpPawnAtributes.damage));
			readerPos += sizeof(int);

			//speed
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.speed = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.speed %i", tmpPawnAtributes.speed));
			readerPos += sizeof(int);

			//attack speed
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.attackspeed = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.attackspeed %i", tmpPawnAtributes.attackspeed));
			readerPos += sizeof(int);

			//attack cooldown
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.attack_cooldown = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.attack_cooldown %i", tmpPawnAtributes.attack_cooldown));
			readerPos += sizeof(int);

			//range
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.range = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.range %i", tmpPawnAtributes.range));
			readerPos += sizeof(int);

			//range alt
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.rangealt = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.rangealt %i", tmpPawnAtributes.rangealt));
			readerPos += sizeof(int);

			//cost
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.cost = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.cost %i", tmpPawnAtributes.cost));
			readerPos += sizeof(int);

			//move cost
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.move_cost = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.move_cost %i", tmpPawnAtributes.move_cost));
			readerPos += sizeof(int);

			//sold value
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.sold_value = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.sold_value %i", tmpPawnAtributes.sold_value));
			readerPos += sizeof(int);

			//coins per kill
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.coins_perkill = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.coins_perkill %i", tmpPawnAtributes.coins_perkill));
			readerPos += sizeof(int);

			//casting speed
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.casting_speed = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.casting_speed %i", tmpPawnAtributes.casting_speed));
			readerPos += sizeof(int);

			//cast cooldown
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPawnAtributes.cast_cooldown = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpPawnAtributes.cast_cooldown %i", tmpPawnAtributes.cast_cooldown));
			readerPos += sizeof(int);

			//skill type
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkillType = dataIntToRead;
			IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkillType %i", tmpSkillType));
			readerPos += sizeof(int);
			tmpPawnAtributes.skill = Skill_Type(tmpSkillType);

			//archer base
			if(tmpType == 1 && tmpPawnAtributes.level == 1)
			{
				SelknamAtt_Archer_Base[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
				IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, SelknamAtt_Archer_Base.damage %i", SelknamAtt_Archer_Base[0].damage));
			}
			else if(tmpType == 1 && tmpPawnAtributes.level == 2)
			{
				SelknamAtt_Archer_Base[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
				IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, SelknamAtt_Archer_Base.damage %i", SelknamAtt_Archer_Base[1].damage));
			}
			else if(tmpType == 1 && tmpPawnAtributes.level == 3)
			{
				SelknamAtt_Archer_Base[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
				IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, SelknamAtt_Archer_Base.damage %i", SelknamAtt_Archer_Base[2].damage));
			}
			//archer poison
			else if(tmpType == 2 && tmpPawnAtributes.level == 1)
				SelknamAtt_Archer_Poisoner[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 2 && tmpPawnAtributes.level == 2)
				SelknamAtt_Archer_Poisoner[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 2 && tmpPawnAtributes.level == 3)
				SelknamAtt_Archer_Poisoner[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//archer retarder
			else if(tmpType == 3 && tmpPawnAtributes.level == 1)
				SelknamAtt_Archer_Retarder[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 3 && tmpPawnAtributes.level == 2)
				SelknamAtt_Archer_Retarder[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 3 && tmpPawnAtributes.level == 3)
				SelknamAtt_Archer_Retarder[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//warrior base
			else if(tmpType == 4 && tmpPawnAtributes.level == 1)
				SelknamAtt_Warrior_Base[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 4 && tmpPawnAtributes.level == 2)
				SelknamAtt_Warrior_Base[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 4 && tmpPawnAtributes.level == 3)
				SelknamAtt_Warrior_Base[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//warrior tank
			else if(tmpType == 5 && tmpPawnAtributes.level == 1)
				SelknamAtt_Warrior_Tank[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 5 && tmpPawnAtributes.level == 2)
				SelknamAtt_Warrior_Tank[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 5 && tmpPawnAtributes.level == 3)
				SelknamAtt_Warrior_Tank[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//warrior berserk
			else if(tmpType == 6 && tmpPawnAtributes.level == 1)
				SelknamAtt_Warrior_Berserk[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 6 && tmpPawnAtributes.level == 2)
				SelknamAtt_Warrior_Berserk[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 6 && tmpPawnAtributes.level == 3)
				SelknamAtt_Warrior_Berserk[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//shaman healer
			else if(tmpType == 7 && tmpPawnAtributes.level == 1)
				SelknamAtt_Shaman_Healer[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 7 && tmpPawnAtributes.level == 2)
				SelknamAtt_Shaman_Healer[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 7 && tmpPawnAtributes.level == 3)
				SelknamAtt_Shaman_Healer[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//shaman might
			else if(tmpType == 8 && tmpPawnAtributes.level == 1)
				SelknamAtt_Shaman_Might[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 8 && tmpPawnAtributes.level == 2)
				SelknamAtt_Shaman_Might[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 8 && tmpPawnAtributes.level == 3)
				SelknamAtt_Shaman_Might[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//shaman haste
			else if(tmpType == 9 && tmpPawnAtributes.level == 1)
				SelknamAtt_Shaman_Haste[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 9 && tmpPawnAtributes.level == 2)
				SelknamAtt_Shaman_Haste[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 9 && tmpPawnAtributes.level == 3)
				SelknamAtt_Shaman_Haste[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//bowler Stuner
			else if(tmpType == 10 && tmpPawnAtributes.level == 1)
				SelknamAtt_Bowler_Stuner[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 10 && tmpPawnAtributes.level == 2)
				SelknamAtt_Bowler_Stuner[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 10 && tmpPawnAtributes.level == 3)
				SelknamAtt_Bowler_Stuner[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//bowler rooter
			else if(tmpType == 11 && tmpPawnAtributes.level == 1)
				SelknamAtt_Bowler_Rooter[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 11 && tmpPawnAtributes.level == 2)
				SelknamAtt_Bowler_Rooter[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 11 && tmpPawnAtributes.level == 3)
				SelknamAtt_Bowler_Rooter[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//bowler confuser
			else if(tmpType == 12 && tmpPawnAtributes.level == 1)
				SelknamAtt_Bowler_Confuser[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 12 && tmpPawnAtributes.level == 2)
				SelknamAtt_Bowler_Confuser[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 12 && tmpPawnAtributes.level == 3)
				SelknamAtt_Bowler_Confuser[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//enemy soldier
			else if(tmpType == 13 && tmpPawnAtributes.level == 1)
			{
				Enemy_Att_Soldier[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
				IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, Enemy_Att_Soldier.damage %i", Enemy_Att_Soldier[0].health));
			}
			else if(tmpType == 13 && tmpPawnAtributes.level == 2)
			{
				Enemy_Att_Soldier[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
				IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, Enemy_Att_Soldier.damage %i", Enemy_Att_Soldier[1].health));
			}
			else if(tmpType == 13 && tmpPawnAtributes.level == 3)
			{
				Enemy_Att_Soldier[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
				IwTrace(PAWNATRIBUTES, ("GetPawnAttributesFromExternalFile() called, Enemy_Att_Soldier.damage %i", Enemy_Att_Soldier[2].health));
			}
			//enemy varon
			else if(tmpType == 14 && tmpPawnAtributes.level == 1)
				Enemy_Att_Varon[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 14 && tmpPawnAtributes.level == 2)
				Enemy_Att_Varon[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 14 && tmpPawnAtributes.level == 3)
				Enemy_Att_Varon[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//enemy convict
			else if(tmpType == 15 && tmpPawnAtributes.level == 1)
				Enemy_Att_Convict[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 15 && tmpPawnAtributes.level == 2)
				Enemy_Att_Convict[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 15 && tmpPawnAtributes.level == 3)
				Enemy_Att_Convict[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//enemy Scientist
			else if(tmpType == 16 && tmpPawnAtributes.level == 1)
				Enemy_Att_Scientist[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 16 && tmpPawnAtributes.level == 2)
				Enemy_Att_Scientist[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 16 && tmpPawnAtributes.level == 3)
				Enemy_Att_Scientist[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//enemy Tank
			else if(tmpType == 17 && tmpPawnAtributes.level == 1)
				Enemy_Att_Tank[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 17 && tmpPawnAtributes.level == 2)
				Enemy_Att_Tank[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 17 && tmpPawnAtributes.level == 3)
				Enemy_Att_Tank[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//enemy Rabbit
			else if(tmpType == 18 && tmpPawnAtributes.level == 1)
				Enemy_Att_Rabbit[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 18 && tmpPawnAtributes.level == 2)
				Enemy_Att_Rabbit[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 18 && tmpPawnAtributes.level == 3)
				Enemy_Att_Rabbit[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			//enemy Pirate
			else if(tmpType == 19 && tmpPawnAtributes.level == 1)
				Enemy_Att_Pirate[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 19 && tmpPawnAtributes.level == 2)
				Enemy_Att_Pirate[tmpPawnAtributes.level - 1] = tmpPawnAtributes;
			else if(tmpType == 19 && tmpPawnAtributes.level == 3)
				Enemy_Att_Pirate[tmpPawnAtributes.level - 1] = tmpPawnAtributes;

		}

		//total skill poison num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel poison %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Poison.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Poison.level %i", tmpSkill_Att_Poison.level));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Poison.damage_per_interval = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Poison.damage_per_interval %i", tmpSkill_Att_Poison.damage_per_interval));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Poison.interval_time = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Poison.interval_time %i", tmpSkill_Att_Poison.interval_time));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Poison.num_intervals = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Poison.num_intervals %i", tmpSkill_Att_Poison.num_intervals));
			readerPos += sizeof(int);

			Skill_Att_Poison[i] = tmpSkill_Att_Poison;
		}

		//total skill slow num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel slow %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Slow.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Slow.level %i", tmpSkill_Att_Slow.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Slow.debuff_time = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Slow.debuff_time %i", tmpSkill_Att_Slow.debuff_time));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Slow.debuff_slow_amount = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Slow.debuff_slow_amount %i", tmpSkill_Att_Slow.debuff_slow_amount ));
			readerPos += sizeof(int);

			Skill_Att_Slow[i] = tmpSkill_Att_Slow;
		}

		//total skill stun num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel stun %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Stun.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Stun.level %i", tmpSkill_Att_Stun.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Stun.debuff_time = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Stun.debuff_time %i", tmpSkill_Att_Stun.debuff_time));
			readerPos += sizeof(int);

			Skill_Att_Stun[i] = tmpSkill_Att_Stun;
		}

		//total skill root num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel root %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Root.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Root.level %i", tmpSkill_Att_Root.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Root.debuff_time = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Root.debuff_time %i", tmpSkill_Att_Root.debuff_time));
			readerPos += sizeof(int);

			Skill_Att_Root[i] = tmpSkill_Att_Root;
		}

		//total skill confusion num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel confusion %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Confusion.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Confusion.level %i", tmpSkill_Att_Confusion.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Confusion.debuff_time = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Confusion.debuff_time %i", tmpSkill_Att_Confusion.debuff_time));
			readerPos += sizeof(int);

			Skill_Att_Confusion[i] = tmpSkill_Att_Confusion;
		}

		//total skill explode num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel explode %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Explode.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Explode.level %i", tmpSkill_Att_Explode.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Explode.range = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Explode.range %i", tmpSkill_Att_Explode.range ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Explode.damage = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Explode.damage %i", tmpSkill_Att_Explode.damage ));
			readerPos += sizeof(int);

			Skill_Att_Explode[i] = tmpSkill_Att_Explode;
		}

		//total skill poison on death num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel poison on death %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_PoisonOnDeath.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_PoisonOnDeath.level %i", tmpSkill_Att_PoisonOnDeath.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_PoisonOnDeath.range = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_PoisonOnDeath.range %i", tmpSkill_Att_PoisonOnDeath.range ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_PoisonOnDeath.damage_per_interval = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_PoisonOnDeath.damage_per_interval %i", tmpSkill_Att_PoisonOnDeath.damage_per_interval ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_PoisonOnDeath.interval_time = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_PoisonOnDeath.interval_time %i", tmpSkill_Att_PoisonOnDeath.interval_time ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_PoisonOnDeath.num_intervals = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_PoisonOnDeath.num_intervals %i", tmpSkill_Att_PoisonOnDeath.num_intervals ));
			readerPos += sizeof(int);

			Skill_Att_PoisonOnDeath[i] = tmpSkill_Att_PoisonOnDeath;
		}

		//total skill heal num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel heal %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Heal.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Heal.level %i", tmpSkill_Att_Heal.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Heal.health_amount = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Heal.health_amount %i", tmpSkill_Att_Heal.health_amount ));
			readerPos += sizeof(int);

			Skill_Att_Heal[i] = tmpSkill_Att_Heal;
		}

		//total skill might num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel might %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Might.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Might.level %i", tmpSkill_Att_Might.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Might.buff_might_amount = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Might.health_amount %i", tmpSkill_Att_Might.buff_might_amount ));
			readerPos += sizeof(int);

			Skill_Att_Might[i] = tmpSkill_Att_Might;
		}

		//total skill haste num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalSkillsLevel = dataIntToRead;
		IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpTotalSkillsLevel haste %i", tmpTotalSkillsLevel));
		readerPos += sizeof(int);

		for (int i = 0; i < tmpTotalSkillsLevel; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Haste.level = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Haste.level %i", tmpSkill_Att_Haste.level ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpSkill_Att_Haste.buff_haste_amount = dataIntToRead;
			IwTrace(SKILLATRIBUTES, ("GetPawnAttributesFromExternalFile() called, tmpSkill_Att_Haste.buff_haste_amount %i", tmpSkill_Att_Haste.buff_haste_amount ));
			readerPos += sizeof(int);

			Skill_Att_Haste[i] = tmpSkill_Att_Haste;
		}
	}

	s3eFileClose(fileHandle);
}

//Unit Atributes Parser.
void ParserHelper::RetrievePawnsAnimsFromFile()
{
	s3eFile* fileHandle = s3eFileOpen("anim_controller.sdm", "rb");
	if(!fileHandle)
		return;
	static int32 fileSize = s3eFileGetSize(fileHandle);
	int16 dataInt16ToRead = 0;
	int readerPos = 0;
	bool bIsEOF = false;


	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				for (int l = 0; l < MAX_ANIMATION; l++)
				{
					s3eFileRead(&dataInt16ToRead, sizeof(int16), 1, fileHandle);
					Anim_Pawns[i][j][k][l] = dataInt16ToRead;
					readerPos += sizeof(int16);
					if(readerPos >= fileSize)
					{
						bIsEOF = true;
						break;
					}
				}
				if(bIsEOF) {break;}
			}
			if(bIsEOF) {break;}
		}
		if(bIsEOF) {break;}
	}	
	
	s3eFileClose(fileHandle);
}

//Parse Map From Binary File
void ParserHelper::RetrieveMapFromFile(CGame* gameRef)
{
	IsoTerrain *CurrentIsoTerrain = NULL;
	IsoProp *CurrentIsoProp = NULL;
	IsoEnemy *CurrentIsoEnemy = NULL;

	//map properties
	int tmpLifes = -1;
	int tmpCoins = -1;
	int tmpCinematic = -1;
	int tmpHowToPlayPage = -1;
	int tmpHowToPlayMaxPages = -1;
	int tmpTimeDefense = -1;
	int tmpTimeToWaves[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	int tmpTimeToWavesSpawn[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	int tmpTileType = -1;

	//tiles
	int tmpTotalTiles = -1;

	int tmpTileNum = -1;
	int tmpLayer = -1;
	int tmpX = -1;
	int tmpY = -1;
	int tmpZ = -1;
	int tmpXNumTile = -1;
	int tmpYNumTile = -1;

	bool bTmpIsWalkeable = false;
	bool bTmpIsWalkeableFromNorth = false;
	bool bTmpIsWalkeableFromSouth = false;
	bool bTmpIsWalkeableFromEast = false;
	bool bTmpIsWalkeableFromWest = false;

	bool bTmpIsRamp;
	bool bTmpRampConnOnNorth;
	bool bTmpRampConnOnSouth;
	bool bTmpRampConnOnEast;
	bool bTmpRampConnOnWest;

	bool bTmpIsInitPoint = false;
	int TmpInitPointNum = -1;
	bool bTmpIsFinalPoint = false;
	int TmpFinalPointNum = -1;

	bool bTmpIsInitCursorPoint;
	bool bTmpCanNavigate;
	bool bTmpCanPutUnit;
	bool bTmpIntentinalCantPutUnit;

	bool bTmpIsGreen;

	bool bTmpIsWater_a;
	bool bTmpIsWater_b;

	//props
	int tmpTotalProps = -1;

	int tmpPropNum = -1;
	int tmpPropLayer = -1;
	int tmpPropX = -1;
	int tmpPropY = -1;
	int tmpPropZ = -1;
	int tmpPropWidth = -1;
	int tmpPropHeight = -1;
	int tmpPropXNumTile = -1;
	int tmpPropYNumTile = -1;

	//enemies
	int tmpTotalEnemies = -1;

	int tmpEnemyType = -1;
	int tmpEnemyLevel = -1;
	int tmpInitPathNum = -1;
	int tmpWaveNum = -1;
	int tmpSec= -1;
	int tmpTargetNum = -1;

	s3eFile* fileHandle = NULL;

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		fileHandle = s3eFileOpen(Utils::TransformToMapFilename(currentMap, bExtraMapMode).c_str(), "rb");
	}
	else if(gameMode == GAMEMODE_SURVIVAL)
	{
		fileHandle = s3eFileOpen("survival_maps/survival.sdm", "rb");
	}
	else if(gameMode == GAMEMODE_VSEUROPE)
	{
		fileHandle = s3eFileOpen("survival_maps/selknams_vs_european.sdm", "rb");
	}
	
	if(!fileHandle)
	{
		return;
	}

	int32 fileSize = s3eFileGetSize(fileHandle);
	int dataIntToRead = 0;
	char dataByteRead = 0;
	bool dataBoolToRead = 0;
	int readerPos = 0;
	int structCounter = 0;
	while (readerPos < fileSize)
	{
		//map properties.
		s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
		tmpLifes = int(dataByteRead);
		IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, tmpLifes %i", tmpLifes));
		readerPos += sizeof(char);

		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpCoins = dataIntToRead;
		IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, tmpCoins %i", tmpCoins));
		readerPos += sizeof(int);

		s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
		tmpCinematic = int(dataByteRead);
		IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, tmpCinematic %i", tmpCinematic));
		readerPos += sizeof(char);

		s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
		tmpHowToPlayPage = int(dataByteRead);
		IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, tmpHowToPlayPage %i", tmpHowToPlayPage));
		readerPos += sizeof(char);

		s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
		tmpHowToPlayMaxPages = int(dataByteRead);
		IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, tmpHowToPlayMaxPages %i", tmpHowToPlayMaxPages));
		readerPos += sizeof(char);

		s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
		tmpTimeDefense = int(dataByteRead);
		IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, tmpTimeDefense %i", tmpTimeDefense));
		readerPos += sizeof(char);

		for (int i = 0; i < 9; i++)
		{
			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpTimeToWaves[i] = int(dataByteRead);
			IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, timetowave%i , %i", i, tmpTimeToWaves[i]));
			readerPos += sizeof(char);
		}

		for (int i = 0; i < 9; i++)
		{
			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpTimeToWavesSpawn[i] = int(dataByteRead);
			IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, timetowave%i , %i", i, tmpTimeToWavesSpawn[i]));
			readerPos += sizeof(char);
		}

		s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
		tmpTileType = int(dataByteRead);
		IwTrace(MAPPROPERTIES_PARSER, ("InitMap() called, tmpTileType %i", tmpTileType));
		readerPos += sizeof(char);

		//Assign Map properties to current level.
		if(tmpLifes <= 0)//lifes check
			tmpLifes = 8;
		gameRef->setMPLifes(tmpLifes);
		gameRef->setMPCoins(tmpCoins);
		gameRef->setMPCinematic(tmpCinematic);
		gameRef->setMPHowToplay(tmpHowToPlayPage);
		gameRef->setMPHowToplayMaxPages(tmpHowToPlayMaxPages);
		gameRef->setMPTimeDefense(tmpTimeDefense);
		gameRef->setMPTimeToWaves(tmpTimeToWaves);
		gameRef->setMPTimeToWavesSpawn(tmpTimeToWavesSpawn);
		gameRef->setMPTileType(tmpTileType);

		//total tiles num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalTiles = dataIntToRead;
		IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpTotalTiles %i", tmpTotalTiles));
		readerPos += sizeof(int);
		//handling tiles
		structCounter = 0;
		for (int i = 0; i < tmpTotalTiles; i++)
		{
			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpTileNum = int(dataByteRead);
			IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpTileNum %i", tmpTileNum));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpLayer = int(dataByteRead);
			IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpLayer %i", tmpLayer));
			readerPos += sizeof(char);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpX = dataIntToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpX %i", tmpX));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpY = dataIntToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpY %i", tmpY));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpZ = dataIntToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpZ %i", tmpZ));
			readerPos += sizeof(int);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpXNumTile = int(dataByteRead);
			IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpXNumTile %i", tmpXNumTile));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpYNumTile = int(dataByteRead);
			IwTrace(MAPTILE_PARSER, ("InitMap() called, tmpYNumTile %i", tmpYNumTile));
			readerPos += sizeof(char);



			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsWalkeable = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsWalkeable %i", bTmpIsWalkeable));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsWalkeableFromNorth = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsWalkeableFromNorth %i", bTmpIsWalkeableFromNorth));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsWalkeableFromSouth = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsWalkeableFromSouth %i", bTmpIsWalkeableFromSouth));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsWalkeableFromEast = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsWalkeableFromEast %i", bTmpIsWalkeableFromEast));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsWalkeableFromWest = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsWalkeableFromWest %i", bTmpIsWalkeableFromWest));
			readerPos += sizeof(bool);



			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsRamp = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsRamp %i", bTmpIsRamp));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpRampConnOnNorth = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpRampConnOnNorth %i", bTmpRampConnOnNorth));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpRampConnOnSouth = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpRampConnOnSouth %i", bTmpRampConnOnSouth));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpRampConnOnEast = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpRampConnOnEast %i", bTmpRampConnOnEast));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpRampConnOnWest = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpRampConnOnWest %i", bTmpRampConnOnWest));
			readerPos += sizeof(bool);



			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsInitPoint = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsInitPoint %i", bTmpIsInitPoint));
			readerPos += sizeof(bool);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			TmpInitPointNum = int(dataByteRead);
			IwTrace(MAPTILE_PARSER, ("InitMap() called, TmpInitPointNum %i", TmpInitPointNum));
			readerPos += sizeof(char);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsFinalPoint = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsFinalPoint %i", bTmpIsFinalPoint));
			readerPos += sizeof(bool);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			TmpFinalPointNum = int(dataByteRead);
			IwTrace(MAPTILE_PARSER, ("InitMap() called, TmpFinalPointNum %i", TmpFinalPointNum));
			readerPos += sizeof(char);


			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsInitCursorPoint = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsInitCursorPoint %i", bTmpIsInitCursorPoint));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpCanNavigate = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpCanNavigate %i", bTmpCanNavigate));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpCanPutUnit = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpCanPutUnit %i", bTmpCanPutUnit));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIntentinalCantPutUnit = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIntentinalCantPutUnit %i", bTmpIntentinalCantPutUnit));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsGreen = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bIsGreen %i", bTmpIsGreen));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsWater_a = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsWater_a %i", bTmpIsWater_a));
			readerPos += sizeof(bool);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			bTmpIsWater_b = dataBoolToRead;
			IwTrace(MAPTILE_PARSER, ("InitMap() called, bTmpIsWater_b %i", bTmpIsWater_b));
			readerPos += sizeof(bool);

			if(structCounter <= MAX_TERRAIN_TILES)
			{
				CurrentIsoTerrain = &gameRef->isoTerrainArr[structCounter];
				
				//SCREEN FIX
				//set positions relative to screen width
				float tmpXFloat = float(tmpX);
				//set offset by new width screen.
				tmpXFloat = tmpXFloat + mapXOffset;
				float tmpYFloat = float(tmpY);
				//set offset by height.
				tmpYFloat = tmpYFloat + mapYOffset;

				CurrentIsoTerrain->setPosition(tmpXFloat, tmpYFloat, float(tmpZ));
				CurrentIsoTerrain->setRender(true);
				CurrentIsoTerrain->SetTileNum(tmpTileNum);
				CurrentIsoTerrain->setLayer(tmpLayer);
				CurrentIsoTerrain->setXNumTile(tmpXNumTile);
				CurrentIsoTerrain->setYNumTile(tmpYNumTile);
				CurrentIsoTerrain->setIsoPosition();

				CurrentIsoTerrain->setIsWalkeable(bTmpIsWalkeable);
				CurrentIsoTerrain->setIsWalkeableFromNorth(bTmpIsWalkeableFromNorth);
				CurrentIsoTerrain->setIsWalkeableFromSouth(bTmpIsWalkeableFromSouth);
				CurrentIsoTerrain->setIsWalkeableFromEast(bTmpIsWalkeableFromEast);
				CurrentIsoTerrain->setIsWalkeableFromWest(bTmpIsWalkeableFromWest);

				CurrentIsoTerrain->setIsRamp(bTmpIsRamp);
				CurrentIsoTerrain->setRampConnOnNorth(bTmpRampConnOnNorth);
				CurrentIsoTerrain->setRampConnOnSouth(bTmpRampConnOnSouth);
				CurrentIsoTerrain->setRampConnOnEast(bTmpRampConnOnEast);
				CurrentIsoTerrain->setRampConnOnWest(bTmpRampConnOnWest);

				CurrentIsoTerrain->setIsInitPoint(bTmpIsInitPoint);			
				CurrentIsoTerrain->setInitPointNum(TmpInitPointNum);
				CurrentIsoTerrain->setIsFinalPoint(bTmpIsFinalPoint);
				CurrentIsoTerrain->setFinalPointNum(TmpFinalPointNum);

				CurrentIsoTerrain->setInitCursorPoint(bTmpIsInitCursorPoint);
				CurrentIsoTerrain->setCanNavigate(bTmpCanNavigate);
				CurrentIsoTerrain->setCanPutUnit(bTmpCanPutUnit);
				CurrentIsoTerrain->SetCanPutUnitInitValue(bTmpCanPutUnit);
				CurrentIsoTerrain->setIntentionalCantPutUnit(bTmpIntentinalCantPutUnit);
				CurrentIsoTerrain->setEnemyCanStopHere(true);

				CurrentIsoTerrain->setIsGreen(bTmpIsGreen);
				if(!bTmpIsGreen)
					CurrentIsoTerrain->setAStarUniqueCost(ASTAR_WALKABLETILECOST);
				else
					CurrentIsoTerrain->setAStarUniqueCost(ASTAR_WALKABLEGREENTILECOST);

				CurrentIsoTerrain->setIsWater_a(bTmpIsWater_a);
				CurrentIsoTerrain->setIsWater_b(bTmpIsWater_b);

				CurrentIsoTerrain->InitIso();

				//IwTrace(APPLOG, ("InitMap() called, posX %i, posY %i, posZ %i layer %i, tileNum %i, isRamp %i", tmpX, tmpY, tmpZ, tmpLayer, tmpTileNum, bTmpIsRamp));

				structCounter++;
			}
		}

		////total props num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalProps = dataIntToRead;
		IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpTotalProps %i", tmpTotalProps));
		readerPos += sizeof(int);
		//handling props
		structCounter = 0;
		for (int i = 0; i < tmpTotalProps; i++)
		{
			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpPropNum = int(dataByteRead);
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropNum %i", tmpPropNum));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpPropLayer = int(dataByteRead);
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropLayer %i", tmpPropLayer));
			readerPos += sizeof(char);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPropX = dataIntToRead;
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropX %i", tmpPropX));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPropY = dataIntToRead;
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropY %i", tmpPropY));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPropZ = dataIntToRead;
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropZ %i", tmpPropZ));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPropWidth = dataIntToRead;
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropWidth %i", tmpPropWidth));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpPropHeight = dataIntToRead;
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropHeight %i", tmpPropHeight));
			readerPos += sizeof(int);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpPropXNumTile = int(dataByteRead);
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropXNumTile %i", tmpPropXNumTile));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpPropYNumTile = int(dataByteRead);
			IwTrace(MAPROPS_PARSER, ("InitMap() called, tmpPropYNumTile %i", tmpPropYNumTile));
			readerPos += sizeof(char);

			if(structCounter <= MAX_ISO_PROPS)
			{
				CurrentIsoProp = &gameRef->isoPropArr[structCounter];
				//SCREEN FIX
				//set positions relative to screen width
				float tmpXFloat = float(tmpPropX);
				//set offset by new width screen.
				tmpXFloat = tmpXFloat + mapXOffset;
				float tmpYFloat = float(tmpPropY);
				//set offset by height.
				tmpYFloat = tmpYFloat + mapYOffset;

				CurrentIsoProp->setPosition(tmpXFloat, tmpYFloat, float(tmpPropZ));
				CurrentIsoProp->setRender(true);
				CurrentIsoProp->SetPropNum(tmpPropNum);
				CurrentIsoProp->setLayer(tmpPropLayer);
				CurrentIsoProp->setSize((float)tmpPropWidth, (float)tmpPropHeight);
				CurrentIsoProp->setXNumTile(tmpPropXNumTile);
				CurrentIsoProp->setYNumTile(tmpPropYNumTile);
				CurrentIsoProp->setIsoPosition();
				CurrentIsoProp->InitIso();

				//IwTrace(APPLOG, ("InitMap() called, PROP, tmpPropX %i, tmpPropY %i, tmpPropZ %i tmpPropLayer %i, tmpPropNum %i", tmpPropX, tmpPropY, tmpPropZ, tmpPropLayer, tmpPropNum));

				structCounter++;
			}

		}

		//total enemies num
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpTotalEnemies = dataIntToRead;
		IwTrace(MAPENEMIES_PARSER, ("InitMap() called, tmpTotalEnemies %i", tmpTotalEnemies));
		readerPos += sizeof(int);

		structCounter = 0;
		//handling enemies
		for (int i = 0; i < tmpTotalEnemies; i++)
		{
			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpEnemyType = int(dataByteRead);
			IwTrace(MAPENEMIES_PARSER, ("InitMap() called, tmpEnemyType %i", tmpEnemyType));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpEnemyLevel = int(dataByteRead);
			IwTrace(MAPENEMIES_PARSER, ("InitMap() called, tmpEnemyLevel %i", tmpEnemyLevel));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpInitPathNum = int(dataByteRead);
			IwTrace(MAPENEMIES_PARSER, ("InitMap() called, tmpInitPathNum %i", tmpInitPathNum));
			readerPos += sizeof(char);
			
			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpWaveNum = int(dataByteRead);
			IwTrace(MAPENEMIES_PARSER, ("InitMap() called, tmpWaveNum %i", tmpWaveNum));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpSec = int(dataByteRead);
			IwTrace(MAPENEMIES_PARSER, ("InitMap() called, tmpSec %i", tmpSec));
			readerPos += sizeof(char);

			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			tmpTargetNum = int(dataByteRead);
			IwTrace(MAPENEMIES_PARSER, ("InitMap() called, tmpTargetNum %i", tmpTargetNum));
			readerPos += sizeof(char);

			if(structCounter < MAX_ISO_ENEMIES)
			{
				CurrentIsoEnemy = &gameRef->isoEnemyArr[structCounter];

				CurrentIsoEnemy->setEnemyType((IsoEnemyType)tmpEnemyType);
				CurrentIsoEnemy->setEnemyLevel(tmpEnemyLevel);
				CurrentIsoEnemy->setInitPathNum(tmpInitPathNum);
				CurrentIsoEnemy->setWaveToSpawn(tmpWaveNum);
				CurrentIsoEnemy->setSecToSpawn(tmpSec);
				CurrentIsoEnemy->setTargetNum(tmpTargetNum);

				structCounter++;
			}
		}
	}

	s3eFileClose(fileHandle);
}
