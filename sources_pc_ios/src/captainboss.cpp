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

#include "captainboss.h"
#include "texturehelper.h"
#include "utils.h"
#include "game.h"

CaptainBoss::CaptainBoss()
{
	CIwResGroup *pGroup = IwGetResManager()->GetGroupNamed("ship");

	shipModel = new Model3D();
	shipModel->AddModel((CIwModel*)pGroup->GetResNamed("cannon_1", IW_GRAPHICS_RESTYPE_MODEL), (CIwAnimSkin*)pGroup->GetResNamed("cannon_1", IW_ANIM_RESTYPE_SKIN));
	shipModel->AddModel((CIwModel*)pGroup->GetResNamed("broken_cannon_1", IW_GRAPHICS_RESTYPE_MODEL), (CIwAnimSkin*)pGroup->GetResNamed("broken_cannon_1", IW_ANIM_RESTYPE_SKIN));
	shipModel->AddModel((CIwModel*)pGroup->GetResNamed("ship_1", IW_GRAPHICS_RESTYPE_MODEL), (CIwAnimSkin*)pGroup->GetResNamed("ship_1", IW_ANIM_RESTYPE_SKIN));
	shipModel->AddModel((CIwModel*)pGroup->GetResNamed("cannon_2", IW_GRAPHICS_RESTYPE_MODEL), (CIwAnimSkin*)pGroup->GetResNamed("cannon_2", IW_ANIM_RESTYPE_SKIN));
	shipModel->AddModel((CIwModel*)pGroup->GetResNamed("broken_cannon_2", IW_GRAPHICS_RESTYPE_MODEL), (CIwAnimSkin*)pGroup->GetResNamed("broken_cannon_2", IW_ANIM_RESTYPE_SKIN));
	shipModel->AddModel((CIwModel*)pGroup->GetResNamed("ship_2", IW_GRAPHICS_RESTYPE_MODEL), (CIwAnimSkin*)pGroup->GetResNamed("ship_2", IW_ANIM_RESTYPE_SKIN));
	shipModel->SetSkel((CIwAnimSkel*)pGroup->GetResNamed("bone_ship", IW_ANIM_RESTYPE_SKELETON));
	shipModel->AddAnim((CIwAnim*)pGroup->GetResNamed("bone_ship_idle", IW_ANIM_RESTYPE_ANIMATION));
	shipModel->AddAnim((CIwAnim*)pGroup->GetResNamed("bone_ship_fire", IW_ANIM_RESTYPE_ANIMATION));

	shipModel->HidModel(1);
	shipModel->HidModel(4);

	TextureData tmp;
	//Attack anim
	tmp = TextureHelper::GetTextureNamed("attack0000");	attackAnimRenderData[0] = TextureHelper::GetRenderData(tmp); attackAnimAtlasName[0] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("attack0001");	attackAnimRenderData[1] = TextureHelper::GetRenderData(tmp); attackAnimAtlasName[1] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("attack0002");	attackAnimRenderData[2] = TextureHelper::GetRenderData(tmp); attackAnimAtlasName[2] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("attack0002");	attackAnimRenderData[3] = TextureHelper::GetRenderData(tmp); attackAnimAtlasName[3] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("attack0002");	attackAnimRenderData[4] = TextureHelper::GetRenderData(tmp); attackAnimAtlasName[4] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("attack0002");	attackAnimRenderData[5] = TextureHelper::GetRenderData(tmp); attackAnimAtlasName[5] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("attack0002");	attackAnimRenderData[6] = TextureHelper::GetRenderData(tmp); attackAnimAtlasName[6] = TextureHelper::GetAtlasName(tmp);

	//Idle Anim
	tmp = TextureHelper::GetTextureNamed("idle0000");	idleAnimRenderData[0] = TextureHelper::GetRenderData(tmp); idleAnimAtlasName[0] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("idle0001");	idleAnimRenderData[1] = TextureHelper::GetRenderData(tmp); idleAnimAtlasName[1] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("idle0002");	idleAnimRenderData[2] = TextureHelper::GetRenderData(tmp); idleAnimAtlasName[2] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("idle0003");	idleAnimRenderData[3] = TextureHelper::GetRenderData(tmp); idleAnimAtlasName[3] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("idle0004");	idleAnimRenderData[4] = TextureHelper::GetRenderData(tmp); idleAnimAtlasName[4] = TextureHelper::GetAtlasName(tmp);

	//Pain Anim
	tmp = TextureHelper::GetTextureNamed("pain0000");	painAnimRenderData[0] = TextureHelper::GetRenderData(tmp); painAnimAtlasName[0] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("pain0001");	painAnimRenderData[1] = TextureHelper::GetRenderData(tmp); painAnimAtlasName[1] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("pain0002");	painAnimRenderData[2] = TextureHelper::GetRenderData(tmp); painAnimAtlasName[2] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("pain0003");	painAnimRenderData[3] = TextureHelper::GetRenderData(tmp); painAnimAtlasName[3] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("pain0004");	painAnimRenderData[4] = TextureHelper::GetRenderData(tmp); painAnimAtlasName[4] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("pain0005");	painAnimRenderData[5] = TextureHelper::GetRenderData(tmp); painAnimAtlasName[5] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("pain0006");	painAnimRenderData[6] = TextureHelper::GetRenderData(tmp); painAnimAtlasName[6] = TextureHelper::GetAtlasName(tmp);

	//Death Anim
	tmp = TextureHelper::GetTextureNamed("z_death0000");	deathAnimRenderData[0]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[0]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0001");	deathAnimRenderData[1]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[1]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0002");	deathAnimRenderData[2]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[2]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0003");	deathAnimRenderData[3]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[3]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0004");	deathAnimRenderData[4]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[4]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0005");	deathAnimRenderData[5]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[5]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0006");	deathAnimRenderData[6]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[6]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0007");	deathAnimRenderData[7]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[7]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0008");	deathAnimRenderData[8]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[8]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0009");	deathAnimRenderData[9]  = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[9]  = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0010");	deathAnimRenderData[10] = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[10] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0011");	deathAnimRenderData[11] = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[11] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0012");	deathAnimRenderData[12] = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[12] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0013");	deathAnimRenderData[13] = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[13] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0014");	deathAnimRenderData[14] = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[14] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0015");	deathAnimRenderData[15] = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[15] = TextureHelper::GetAtlasName(tmp);
	tmp = TextureHelper::GetTextureNamed("z_death0016");	deathAnimRenderData[16] = TextureHelper::GetRenderData(tmp); deathAnimAtlasName[16] = TextureHelper::GetAtlasName(tmp);

	curCaptainAnim = CaptainAnim_None;
	curCaptainState = CaptainState_Idle;
	bIsActive = false;
	curAnimFrame = 0;
	bIncreaseAnimFrame = true;
	enemyHolder = NULL;
	pGame = NULL;
	lastHealth = -1;
	curShipPosition = ShipPosition_None;

	animationDelay.SetTimeLimit(200);
	painAnimationDelay.SetTimeLimit(6000);
	waveDelay.SetTimeLimit(4000);
	timeBetweenWaves.SetTimeLimit(10000);
	restingTime.SetTimeLimit(finalBoss_time_resting);

	shipExplotionTime.SetTimeLimit(finalBoss_time_ship_explotion_time);
	shipExplotionDelay.SetTimeLimit(finalboss_time_ship_explotion_single_explotion);

	yellowTextTimer.SetTimeLimit(1500);
	/*
	numEnemiesLength[0] = 1;
	waveEnemyInfo[0][0] = ENEMY_Soldier;

	numEnemiesLength[1] = 2;
	waveEnemyInfo[1][0] = ENEMY_Soldier;
	waveEnemyInfo[1][1] = ENEMY_Varon;

	numEnemiesLength[2] = 3;
	waveEnemyInfo[2][0] = ENEMY_Soldier;
	waveEnemyInfo[2][1] = ENEMY_Varon;
	waveEnemyInfo[2][2] = ENEMY_Pirate;

	numEnemiesLength[3] = 4;
	waveEnemyInfo[3][0] = ENEMY_Soldier;
	waveEnemyInfo[3][1] = ENEMY_Varon;
	waveEnemyInfo[3][2] = ENEMY_Pirate;
	waveEnemyInfo[3][3] = ENEMY_Scientist;

	numEnemiesLength[4] = 5;
	waveEnemyInfo[4][0] = ENEMY_Soldier;
	waveEnemyInfo[4][1] = ENEMY_Varon;
	waveEnemyInfo[4][2] = ENEMY_Pirate;
	waveEnemyInfo[4][3] = ENEMY_Scientist;
	waveEnemyInfo[4][4] = ENEMY_Convict;

	numEnemiesLength[5] = 6;
	waveEnemyInfo[5][0] = ENEMY_Soldier;
	waveEnemyInfo[5][1] = ENEMY_Varon;
	waveEnemyInfo[5][2] = ENEMY_Pirate;
	waveEnemyInfo[5][3] = ENEMY_Scientist;
	waveEnemyInfo[5][4] = ENEMY_Convict;
	waveEnemyInfo[5][5] = ENEMY_Rabbit;

	numEnemiesLength[6] = 7;
	waveEnemyInfo[6][0] = ENEMY_Soldier;
	waveEnemyInfo[6][1] = ENEMY_Varon;
	waveEnemyInfo[6][2] = ENEMY_Pirate;
	waveEnemyInfo[6][3] = ENEMY_Scientist;
	waveEnemyInfo[6][4] = ENEMY_Convict;
	waveEnemyInfo[6][5] = ENEMY_Rabbit;
	waveEnemyInfo[6][6] = ENEMY_Tank;
	*/
	timerSpeakAtBackward.SetTimeLimit(5000);

	SetWaveSettings();

	backwardTime[1] = finalboss_cycle1_backward_time;
	backwardTime[2] = finalboss_cycle2_backward_time;
	backwardTime[3] = finalboss_cycle3_backward_time;

	waveTime[1] = finalboss_cycle1_wave_delay;
	waveTime[2] = finalboss_cycle2_wave_delay;
	waveTime[3] = finalboss_cycle3_wave_delay;

	nextWaveTime[1] = finalboss_cycle1_wave_time;
	nextWaveTime[2] = finalboss_cycle2_wave_time;
	nextWaveTime[3] = finalboss_cycle3_wave_time;

	timeBeforeSPAttack[1] = finalboss_cycle1_time_before_sp;
	timeBeforeSPAttack[2] = finalboss_cycle2_time_before_sp;
	timeBeforeSPAttack[3] = finalboss_cycle3_time_before_sp;

	delayBetweenSPShots[1] = finalboss_cycle1_time_between_sp_shots;
	delayBetweenSPShots[2] = finalboss_cycle2_time_between_sp_shots;
	delayBetweenSPShots[3] = finalboss_cycle3_time_between_sp_shots;

	whiteFade = new FadeScreen(255, 255, 255);

	shipCannonExplotionTime.SetTimeLimit(2400);
	shipCannonExplotionDelay.SetTimeLimit(600);
	bCannonExplotionScene = false;
	bStartInitialDialog = false;
	bEndGameAction = false;
	bEndingScenePlaying = false;

	entranceOffsetX = 120;
	entranceOffsetY = 0.5f * entranceOffsetX;

	bCaptainIgnorePauseStatus = false;
}

CaptainBoss::~CaptainBoss()
{
	delete shipModel;
	delete whiteFade;
}

void CaptainBoss::DestroyCannons()
{
	PlayCaptainAnim(CaptainState_Pain);

	bCannonExplotionScene = true;
	shipCannonExplotionTime.Reset();
	shipCannonExplotionTime.Start();
	
	//Utils::DrawExplotion(605, 390, 1);
	//Utils::DrawExplotion(695, 340, 1);

	shipModel->UnhidModel(1);
	shipModel->UnhidModel(4);
	shipModel->HidModel(0);
	shipModel->HidModel(3);
}

void CaptainBoss::RenderShipFragment(int index)
{
	shipModel->RenderSingleModel(index);
}

bool CaptainBoss::GetIsVisible()
{
	return bVisible;
}

void CaptainBoss::SetVisible(bool value)
{
	bVisible = value;

	if(!bVisible)
	{
		shipModel->HidModel(1);
		shipModel->HidModel(4);
		shipModel->HidModel(2);
		shipModel->HidModel(5);
	}
	else
	{
		shipModel->UnhidModel(1);
		shipModel->UnhidModel(4);
		shipModel->UnhidModel(2);
		shipModel->UnhidModel(5);
	}
}

bool CaptainBoss::IsVulnerable()
{
	return !shipModel->IsMoving() && curShipPosition == ShipPosition_Forward;
}

void CaptainBoss::Update()
{
	if(!bIsActive)
	{
		return;
	}

	UpdateAnimation();

	if(bIsDead)
	{
		UpdateEndingScene();
		return;
	}

	if(bStartInitialDialog && !IsMoving())
	{
		bStartInitialDialog = false;
		dialogController->StartDialog(25);
		PlayCaptainAnim(CaptainState_Attack);
	}

	DestroyCannonsSceneUpdate();

	if(!shipModel->IsMoving() && curCaptainAnim == CaptainAnim_None && !shipModel->IsAnimating())
	{
		curCaptainAnim = CaptainAnim_Idle;
		shipModel->PlayAnim(0, finalboss_ship_idle_speed);
	}

	shipModel->Update(bCaptainIgnorePauseStatus);

	if(curShipPosition == ShipPosition_Backward && !shipModel->IsMoving() && timerSpeakAtBackward.GetIsEnd())
	{
		timerSpeakAtBackward.Reset();
		timerSpeakAtBackward.Start();

		if(curCaptainCycle == 1) Utils::PlaySoundFX(FX_boss_comment_cycle01_backward_01);
		else if(curCaptainCycle == 2) Utils::PlaySoundFX(FX_Boss_comment_cycle02_backward_01);
		else if(curCaptainCycle == 3)
		{
			if(rand() % 2 == 0)
			{
				Utils::PlaySoundFX(FX_Boss_comment_cycle03_backward_02);
			}
			else
			{
				Utils::PlaySoundFX(FX_Boss_comment_cycle03_backward_01);
			}
		}
	}

	if(enemyHolder != NULL && enemyHolder->getInitialized())
	{
		enemyHolder->bIsVulnerable = IsVulnerable();

		painAnimationDelay.Update();
		if(painAnimationDelay.GetIsStop() || painAnimationDelay.GetIsEnd())
		{
			if(lastHealth != enemyHolder->GetCurrentHealth())
			{
				PlayCaptainAnim(CaptainState_Pain);
				painAnimationDelay.Reset();
				painAnimationDelay.Start();
			}
		}

		lastHealth = enemyHolder->GetCurrentHealth();
	}

	if(pGame != NULL)
	{
		if(!bPause && !bPauseGame && !dialogController->IsActive())
		{
			if(cycleTime.GetIsStop()) cycleTime.Resume();
			if(waveDelay.GetIsStop()) waveDelay.Resume();
			if(timeBetweenWaves.GetIsStop()) timeBetweenWaves.Resume();
			if(rechargingCannon.GetIsStop()) rechargingCannon.Resume();
			if(restingTime.GetIsStop()) restingTime.Resume();
			if(yellowTextTimer.GetIsStop()) yellowTextTimer.Resume();
			if(timerSpeakAtBackward.GetIsStop()) timerSpeakAtBackward.Resume();

			cycleTime.Update();
			waveDelay.Update();
			timeBetweenWaves.Update();
			rechargingCannon.Update();
			restingTime.Update();
			yellowTextTimer.Update();
			timerSpeakAtBackward.Update();
		}
		else
		{
			cycleTime.Stop();
			waveDelay.Stop();
			timeBetweenWaves.Stop();
			rechargingCannon.Stop();
			restingTime.Stop();
			yellowTextTimer.Stop();
			timerSpeakAtBackward.Stop();
		}

		if(Utils::GameIsPaused()) return;

		if(curCaptainCycle == 0 && inGameState == GSG_Attack)
		{
			numWave = 0;
			SetCycle(1);
		}

		if(lastShipPosition != curShipPosition)
		{
			if(curShipPosition == ShipPosition_Backward)
			{
				MoveBackward();
			}
			else if(curShipPosition == ShipPosition_Forward)
			{
				MoveForward();
			}

			lastShipPosition = curShipPosition;
		}

		if(inGameState != GSG_Attack) return;

		if(!shipModel->IsMoving())
		{
			if(curShipPosition == ShipPosition_Backward)
			{
				if(bStartBackwardTimer)
				{
					bStartBackwardTimer = false;
					cycleTime.Reset();
					cycleTime.Start();
				}
				
				if(cycleTime.GetIsEnd())
				{
					curShipPosition = ShipPosition_Forward;
					restingTime.SetTimeLimit(TIME_AFTER_FORWARD_MOVEMENT + timeBeforeSPAttack[curCaptainCycle]);
					restingTime.Reset();
					restingTime.Start();
					bStartForwardTimer = true;
					bPerformSpecialAttack = true;
				}

				if(rechargingCannon.GetIsEnd())
				{
					rechargingCannon.Reset();
					rechargingCannon.Start();

					if(curCaptainCycle == 1)
					{
						Utils::PlaySoundFX(FX_Boss_comment_cycle01_prefire);
						Shoot(finalboss_sp_markspot_duration);
						Shoot(finalboss_sp_markspot_duration);
					}
					else if(curCaptainCycle == 2)
					{
						Utils::PlaySoundFX(FX_Boss_comment_cycle02_prefire);
						Shoot(finalboss_sp_markspot_duration, Skill_Type_Poison);
						Shoot(finalboss_sp_markspot_duration, Skill_Type_Poison);
					}
					else if(curCaptainCycle == 3)
					{
						Utils::PlaySoundFX(FX_Boss_comment_cycle03_prefire);
						Shoot(finalboss_sp_markspot_duration, Skill_Type_Poison);
						Shoot(finalboss_sp_markspot_duration, Skill_Type_Slow);
					}

					PlayCaptainAnim(CaptainState_Attack);
				}

				if((!bPauseGame && !bPause) && (timeBetweenWaves.GetIsEnd() || bFirstWave))
				{
					if(waveDelay.GetIsEnd() || waveDelay.GetIsStop())
					{
						waveDelay.Reset();
						waveDelay.Start();
						for(int i = 0; pGame->initNodes[i]; i++)
						{
							SpawnEnemyUnit(i, i + 1);
						}
						numLoops++;

						if(numLoops > 2)
						{
							numWave++;
							
							if(numWave >= finalboss_num_waves[curCaptainCycle - 1])
							{
								numWave = 0;
							}

							numLoops = 0;
							bFirstWave = false; 
							timeBetweenWaves.Reset();
							timeBetweenWaves.Start();
						}
					}
				}
			}
			else if(curShipPosition == ShipPosition_Forward)
			{
				//restingTime.GetCurrTime_Reversed() <= timeBeforeSPAttack[curCaptainCycle]
				if(restingTime.GetIsEnd() || (enemyHolder->GetCurrentHealth() < healthLimit[currentHealtLimit]))
				{
					if((cycleTime.GetIsEnd() || cycleTime.GetIsStop()) || (enemyHolder->GetCurrentHealth() < healthLimit[currentHealtLimit]))
					{
						if(bStartForwardTimer)
						{
							bStartForwardTimer = false;
							cycleTime.SetTimeLimit(finalboss_forward_time);
							cycleTime.Reset();
							cycleTime.Start();
							bAffectedByRoot = true;

							if(curCaptainCycle == 3)
							{
								allyHitPercent = 20;
							}
							else
							{
								allyHitPercent = 0;
							}

							//Cancel normal cannon attacks.
							for(int i = 0; i < MAX_TILESROW; i++)
							{
								for(int j = 0; j < MAX_TILESROW; j++)
								{
									pGame->cannonWarningTile[i][j] = 0;
								}
							}
						}
						else
						{
							cycleTime.SetTimeLimit(backwardTime[curCaptainCycle]);
							curShipPosition = ShipPosition_Backward;
							bStartBackwardTimer = true;
							CancelSpecialAttack();
							if(currentHealtLimit <= 2 && (enemyHolder->GetCurrentHealth() <= healthLimit[currentHealtLimit]))
							{
								if(currentHealtLimit == 2 || enemyHolder->GetCurrentHealth() <= 0)
								{
									PlayEndingScene();
								}
								else
								{
									currentHealtLimit++;
									Utils::PlaySoundFX(FX_Dying_Baron);
									PlayCaptainAnim(CaptainState_Pain);

									if(currentHealtLimit > 1)
									{
										DestroyCannons();
									}

									if(currentHealtLimit > 0)
									{
										curCaptainCycle++;
										if(curCaptainCycle > 3) 
										{
											curCaptainCycle = 3;
										}
										else
										{
											Utils::PlaySoundFX(FX_Boss_angry_after_paincycle);
										}

										waveDelay.SetTimeLimit(waveTime[curCaptainCycle]);
										timeBetweenWaves.SetTimeLimit(nextWaveTime[curCaptainCycle]);

										if(curCaptainCycle == 2)
										{
											rechargingCannon.SetTimeLimit(finalBoss_cycle2_time_recharge);
										}
										else if(curCaptainCycle == 3)
										{
											rechargingCannon.SetTimeLimit(finalBoss_cycle3_time_recharge);
										}
									}
								}
							}
						}
					}
					else
					{
						if(bPerformSpecialAttack && cycleTime.GetCurrTime() >= timeBeforeSPAttack[curCaptainCycle])
						{
							bPerformSpecialAttack = false;
							PerformSpecialAttack();
						}
					}
				}
			}
		}
	}
}

void CaptainBoss::PlayEndingScene()
{
	Utils::MoveCamera(100, CIwFVec2(0, 0), CIwFVec2((float)(130 - xWindowOffset), (float)(50 - yWindowOffset)));

	bCaptainIgnorePauseStatus = true;
	bEndingScenePlaying = true;
	bIsDead = true;

	PlayCaptainAnim(CaptainState_Death);

	shipExplotionTime.Reset();
	shipExplotionTime.Start();

	shipExplotionBlinkDelay.SetTimeLimit(3000);
	shipExplotionBlinkDelay.Reset();
	shipExplotionBlinkDelay.Start();

	shipExplotionBlinkTime.SetTimeLimit(30);

	CancelSpecialAttack();
	for(int i = 1; i < MAX_ISO_ENEMIES; i++)
	{
		if(pGame->isoEnemyArr[i].GetEnemyType() != ENEMY_None)
		{
			pGame->isoEnemyArr[i].TakeDamage(10000);
		}
	}

	//bEndGameAction = true;
	dialogController->StartDialog(50);

	//Achievement nº 14.
	//Epic finale: Vence al malvado jefe pirata.
	achievementsHelper->SetAchievementCompletedById(13);
}

void CaptainBoss::EndGameAction()
{	
	bCaptainIgnorePauseStatus = false;
	bPauseGame = true;
	bBlockCamera = true;
	bEndGameAction = true;
	whiteFade->FadeInWithTimer(5000);
	fade->SetColor(255, 255, 255);
	pGame->loadingfade->SetColor(255, 255, 255);
}

void CaptainBoss::RenderSpecialForeground()
{
	if(!bIsActive) 
		return;

	whiteFade->Render();
}

void CaptainBoss::UpdateEndingScene()
{
	/*
	if(!Utils::GameIsPaused())
	{
		if(shipExplotionTime.GetIsStop()) shipExplotionTime.Resume();
		if(shipExplotionDelay.GetIsStop()) shipExplotionDelay.Resume();
		if(shipExplotionBlinkDelay.GetIsStop()) shipExplotionBlinkDelay.Resume();
		if(shipExplotionBlinkTime.GetIsStop()) shipExplotionBlinkTime.Resume();

		shipExplotionTime.Update();
		shipExplotionDelay.Update();
		shipExplotionBlinkDelay.Update();
		shipExplotionBlinkTime.Update();
	}
	else
	{
		shipExplotionTime.Stop();
		shipExplotionDelay.Stop();
		shipExplotionBlinkDelay.Stop();
		shipExplotionBlinkTime.Stop();
	}
	*/
	shipExplotionTime.Update();
	shipExplotionDelay.Update();
	shipExplotionBlinkDelay.Update();
	shipExplotionBlinkTime.Update();

	whiteFade->Update();

	//Credits.
	if(bEndGameAction && whiteFade->GetAlpha() == 255)
	{
		bPauseGame = false;
		bBlockCamera = false;
		bEndGameAction = false;
		SetActive(false);
		gameState = GS_Retry;
		inGameState = GSG_DynamicCredits;
		currentMap = 26;
	}

	//if(Utils::GameIsPaused())
	//{
	//	return;
	//}

	if(shipExplotionTime.GetIsEnd())
	{
		SetVisible(false);
		bEndingScenePlaying = false;
	}
	else
	{
		if(shipExplotionBlinkDelay.GetIsEnd())
		{
			if(shipExplotionBlinkTime.GetIsEnd() && !GetIsVisible())
			{
				SetVisible(true);

				shipExplotionBlinkDelay.SetTimeLimit((int)(shipExplotionBlinkDelay.GetTimeLimit() * 0.5f));
				shipExplotionBlinkDelay.Reset();
				shipExplotionBlinkDelay.Start();
				shipExplotionBlinkTime.Stop();
			}
			else
			{
				if(GetIsVisible())
				{
					SetVisible(false);
					shipExplotionBlinkTime.Reset();
					shipExplotionBlinkTime.Start();
				}
			}
		}

		if(shipExplotionDelay.GetIsEnd() || shipExplotionDelay.GetIsStop())
		{
			int centerX = 430;
			int centerY = 200;
			int numExplotions = rand() % 6 + 1;
			for(int i = 0; i < numExplotions; i++)
			{
				int offX = rand() % 220;
				int offY = rand() % 180;
				Utils::DrawExplotion((float)(centerX + offX), (float)(centerY + offY), 10);
			}
			shipExplotionDelay.Reset();
			shipExplotionDelay.Start();
		}
	}
}

void CaptainBoss::PerformSpecialAttack()
{
	srand(time(NULL));
	if(curCaptainCycle == 1)
	{
		Utils::PlaySoundFX(FX_Boss_comment_cycle01_prefire);
		for(int i = 0; i < finalboss_cycle1_sp_num_shots; i++)
		{
			Shoot(2000 + delayBetweenSPShots[curCaptainCycle] * i, Skill_Type_None, true, allyHitPercent);
		}
	}
	else if(curCaptainCycle == 2)
	{
		Utils::PlaySoundFX(FX_Boss_comment_cycle02_prefire);
		for(int i = 0; i < finalboss_cycle2_sp_num_shots; i++)
		{
			Shoot(2000 + delayBetweenSPShots[curCaptainCycle] * i, Skill_Type_Poison, true, allyHitPercent);
		}
	}
	else if(curCaptainCycle == 3)
	{
		Utils::PlaySoundFX(FX_Boss_comment_cycle03_prefire);
		for(int i = 0; i < finalboss_cycle3_sp_num_shots; i++)
		{
			Shoot(2000 + delayBetweenSPShots[curCaptainCycle] * i, Skill_Type_CountdownDeath, true, allyHitPercent);
		}
	}
}

bool CaptainBoss::IsMoving()
{
	return shipModel->IsMoving();
}

void CaptainBoss::SetCycle(int n)
{
	if(n == 1)
	{
		curShipPosition = ShipPosition_Backward;
		lastShipPosition = ShipPosition_Backward;
		cycleTime.SetTimeLimit(backwardTime[1]);
		cycleTime.Reset();
		cycleTime.Start();

		waveDelay.SetTimeLimit(waveTime[1]);
		timeBetweenWaves.SetTimeLimit(nextWaveTime[1]);
		rechargingCannon.SetTimeLimit(finalBoss_cycle1_time_recharge);
		
		numLoops = 0;
		waveDelay.Reset();
		waveDelay.Start();
		rechargingCannon.Reset();
		rechargingCannon.Start();

		timeBetweenWaves.Reset();
		timeBetweenWaves.Start();

		curCaptainCycle = 1;
		bFirstWave = true;
	}
}

void CaptainBoss::Init()
{
	srand(time(NULL));
	bStartBackwardTimer = false;

	currentHealtLimit = 0;
	healthLimit[0] = (int)(0.66f * enemyHolder->GetMaxHealth());
	healthLimit[1] = (int)(0.33f * enemyHolder->GetMaxHealth());
	healthLimit[2] = (int)(0.0f * enemyHolder->GetMaxHealth());

	curCaptainCycle = 0;

	//Level 4 features.
	Skill_Att_Poison[3].damage_per_interval = finalboss_poison_damage_per_interval;
	Skill_Att_Poison[3].interval_time       = finalboss_poison_interval_time;
	Skill_Att_Poison[3].num_intervals       = finalboss_poison_num_intervals;
	Skill_Att_Poison[3].level               = 4;
	
	Skill_Att_Slow[3].debuff_time        = 60;
	Skill_Att_Slow[3].debuff_slow_amount = 21;
	Skill_Att_Slow[3].level              = 4;

	pGame->setMPCoins(120);
	bPerformSpecialAttack = false;
	bIsDead = false;
	bAffectedByRoot = true;

	shipModel->HidModel(1);
	shipModel->HidModel(4);
	shipModel->UnhidModel(0);
	shipModel->UnhidModel(3);

	shipModel->UnhidModel(2);
	shipModel->UnhidModel(5);
	bVisible = true;

	currCannon = 0;

	//disminuir 0 mueve el projectil a la derecha.
	//disminuir 1 mueve el projectil hacia abajo

	cannonOffset[0][0] = -120;
	cannonOffset[0][1] = -70;

	cannonOffset[1][0] = -35;
	cannonOffset[1][1] = -125;

	curCaptainCycle = 0;
	numWave = 0;

	bFirstWave = true;

	PlayShipAnim(0, 0.15f);

	PlaceInitialSelknams();

	if(whiteFade != NULL)
	{
		whiteFade->SetAlpha(0);
	}

	bPauseGame = false;

	curShipPosition = ShipPosition_None;
}

void CaptainBoss::Render()
{
	if(!bIsActive)
	{
		return;
	}

	shipModel->Render();
}

AtlasName CaptainBoss::GetAtlasByCaptainState(CaptainState state)
{
	if(state == CaptainState_Attack)
	{
		return attackAnimAtlasName[curAnimFrame];
	}
	else if(state == CaptainState_Idle)
	{
		return idleAnimAtlasName[curAnimFrame];
	}
	else if(state == CaptainState_Death)
	{
		return deathAnimAtlasName[curAnimFrame];
	}
	else if(state == CaptainState_Pain)
	{
		return painAnimAtlasName[curAnimFrame];
	}

	return idleAnimAtlasName[0];
}

void CaptainBoss::CancelSpecialAttack()
{
	//IwTrace(BOSSBATTLE, ("CancelSpecialAttack called. CancelSpecialAttack called. CancelSpecialAttack called. CancelSpecialAttack called."));

	bPerformSpecialAttack = false;
	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			pGame->cannonWarningTile[i][j] = 0;
		}
	}
}

RenderData CaptainBoss::GetRenderDataByCaptainState(CaptainState state)
{
	if(state == CaptainState_Attack)
	{
		return attackAnimRenderData[curAnimFrame];
	}
	else if(state == CaptainState_Idle)
	{
		return idleAnimRenderData[curAnimFrame];
	}
	else if(state == CaptainState_Death)
	{
		return deathAnimRenderData[curAnimFrame];
	}
	else if(state == CaptainState_Pain)
	{
		return painAnimRenderData[curAnimFrame];
	}

	return deathAnimRenderData[0];
}

CIwFVec3 CaptainBoss::GetPosition()
{
	CIwFVec3 bonePosition = shipModel->GetBonePosition(0);
	CIwFVec3 shipPosition = shipModel->GetPosition();
	return CIwFVec3(shipPosition.x + bonePosition.x, shipPosition.y + bonePosition.y, shipPosition.z + bonePosition.z);
}

void CaptainBoss::PlayShipAnim(int index, float speed)
{
	shipModel->PlayAnim(index, speed);
}

bool CaptainBoss::GetIsActive()
{
	return bIsActive;
}

void CaptainBoss::MoveForward()
{
	if(curCaptainCycle == 1) Utils::PlaySoundFX(FX_Boss_comment_cycle01_moving_forward_01);
	else if(curCaptainCycle == 2) Utils::PlaySoundFX(FX_Boss_comment_cycle02_moving_forward_01);
	else if(curCaptainCycle == 3) Utils::PlaySoundFX(FX_Boss_comment_cycle03_moving_forward_01);

	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		if(pGame->isoProjectileArr[i].GetType() == Projectile_Cannon)
		{
			pGame->isoProjectileArr[i].CleanIso();
		}
	}

	Move(CIwFVec3(80, 40, 0), restingTime.GetTimeLimit());
}

void CaptainBoss::MoveBackward()
{
	Move(CIwFVec3(-80, -40, 0), restingTime.GetTimeLimit());
}

CaptainState CaptainBoss::GetState()
{
	return curCaptainState;
}

void CaptainBoss::SetActive(bool bIsActive)
{
	this->bIsActive = bIsActive;
	if(bIsActive)
	{
		curCaptainState = CaptainState_Idle;
		curAnimFrame = 0;
		bIncreaseAnimFrame = true;
		animationDelay.Reset();
		animationDelay.Start();

		isEnemyUnlocked[(int)ENEMY_Captain] = true;
	}
}

void CaptainBoss::Move(CIwFVec3 desp, int time)
{
	shipModel->Move(desp, time);
}

void CaptainBoss::SetPosition(CIwFVec3 position)
{
	shipModel->SetPosition(position);
}

void CaptainBoss::UpdateAnimation()
{
	if((!bPause && !bPauseGame && !dialogController->IsActive()) || bCaptainIgnorePauseStatus)
	{
		if(animationDelay.GetIsStop()) animationDelay.Resume();

		animationDelay.Update();
	}
	else
	{
		animationDelay.Stop();
	}

	if(animationDelay.GetIsEnd())
	{
		animationDelay.Reset();
		animationDelay.Start();

		if(bIncreaseAnimFrame)
		{
			curAnimFrame++;
		}
		else
		{
			curAnimFrame--;
		}

		if(curCaptainState == CaptainState_Attack) 
		{
			if(curAnimFrame >= CAPTAIN_NUM_ANIM_ATTACK)
			{
				bIncreaseAnimFrame = false;
				curAnimFrame = CAPTAIN_NUM_ANIM_ATTACK - 2;
			}
			else if(curAnimFrame < 0)
			{
				PlayCaptainAnim(CaptainState_Idle);
			}	
		} 
		else if(curCaptainState == CaptainState_Idle)
		{
			if(curAnimFrame >= CAPTAIN_NUM_ANIM_IDLE)
			{
				bIncreaseAnimFrame = false;
				curAnimFrame = CAPTAIN_NUM_ANIM_IDLE - 2;
			}
			else if(curAnimFrame < 0)
			{
				bIncreaseAnimFrame = true;
				curAnimFrame = 1;
			}
		}
		else if(curCaptainState == CaptainState_Pain && curAnimFrame >= CAPTAIN_NUM_ANIM_PAIN)
		{
			PlayCaptainAnim(CaptainState_Idle);
		}
		else if(curCaptainState == CaptainState_Death && curAnimFrame >= CAPTAIN_NUM_ANIM_DEATH) 
		{
			curAnimFrame = CAPTAIN_NUM_ANIM_DEATH - 1;
		}
	}
}

void CaptainBoss::PlayCaptainAnim(CaptainState state)
{
	curAnimFrame = 0;
	bIncreaseAnimFrame = true;
	curCaptainState = state;

	if(state == CaptainState_Pain)
	{
		int numPain = rand() % 3;
		if(numPain == 0) Utils::PlaySoundFX(FX_Boss_pain_01);
		else if(numPain == 1) Utils::PlaySoundFX(FX_Boss_pain_02);
		else if(numPain == 2) Utils::PlaySoundFX(FX_Boss_pain_02);

		animationDelay.SetTimeLimit(80);
	}
	else if(state == CaptainState_Idle)
	{
		animationDelay.SetTimeLimit(400);
	}
	else if(state == CaptainState_Death)
	{
		Utils::PlaySoundFX(FX_Boss_dying);
		animationDelay.SetTimeLimit(150);
	}
	else if(state == CaptainState_Attack)
	{
		animationDelay.SetTimeLimit(100);
	}
}


void CaptainBoss::SetEnemyObject(IsoEnemy* enemy)
{
	enemyHolder = enemy;
	lastHealth = enemyHolder->GetCurrentHealth();
}

void CaptainBoss::SetGameRef(CGame* gameRef)
{
	pGame = gameRef;
}

void CaptainBoss::SpawnEnemyUnit(int node, int dest)
{
	if(pGame->initNodes[node] == NULL) return;

	//Enemy Spawn
	IsoEnemy* CurrentEnemy = NULL;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		CurrentEnemy = &pGame->isoEnemyArr[i];

		if(CurrentEnemy == NULL) continue;

		if(CurrentEnemy->getInitialized()) continue;

		IsoEnemyType tmpType = finalboss_unit_to_spawn[curCaptainCycle - 1][numWave][node];
		int tmpLevel = 3;
		if(tmpType == ENEMY_Tank)
		{
			tmpLevel = 2;
		}

		CurrentEnemy->setEnemyType(finalboss_unit_to_spawn[curCaptainCycle - 1][numWave][node]);
		CurrentEnemy->setEnemyLevel(tmpLevel);

		CurrentEnemy->setInitPathNum(node + 1);
		CurrentEnemy->setTargetNum(dest);

		CurrentEnemy->InitIsoPawn(pGame->initNodes[node]);

		return;
	}
}

void CaptainBoss::Shoot(IsoTerrain* terrain, Skill_Type skillType)
{
	if(terrain==NULL)
		IwAssertMsg(APPASSERT, terrain == NULL, ("CaptainBoss::Shoot(), Illegal value terrain"));
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		if(pGame->isoProjectileArr[i].GetType() == Projectile_None)
		{
			shipModel->PlayAnim(1, 0.6f, false);
			Utils::PlaySoundFX(FX_Cannon_ship);
			curCaptainAnim = CaptainAnim_None;

			int damage = 0;
			if(curCaptainCycle == 1)      damage = finalboss_cycle1_cannon_damage;
			else if(curCaptainCycle == 2) damage = finalboss_cycle2_cannon_damage;
			else if(curCaptainCycle == 3) damage = finalboss_cycle3_cannon_damage;

			pGame->isoProjectileArr[i].InitInflictor(enemyHolder, enemyHolder->GetPawnAttributes(), damage, skillType);
			pGame->isoProjectileArr[i].InitTarget(terrain);

			pGame->isoProjectileArr[i].SetCorrection(cannonOffset[currCannon][0], cannonOffset[currCannon][1]);
			pGame->isoProjectileArr[i].SetTargetCorrection(0, 0);
			
			pGame->isoProjectileArr[i].SetAttackRange(1);
			pGame->isoProjectileArr[i].RangeAttackAffectsToEnemies(true);
			pGame->isoProjectileArr[i].InitProjectile(Projectile_Cannon);
			
			if(currCannon == 0)
			{
				if(curShipPosition == ShipPosition_Backward)
				{
					Utils::DrawCannonExplotion(605 + 45, 390 + 80, 1);
				}
				else if(curShipPosition == ShipPosition_Forward)
				{
					Utils::DrawCannonExplotion(605 + 40 + 80, 390 + 90 + 40, 1);
				}
			}
			else if(currCannon == 1)
			{
				if(curShipPosition == ShipPosition_Backward)
				{
					Utils::DrawCannonExplotion(695 + 40, 340 + 90, 1);
				}
				else if(curShipPosition == ShipPosition_Forward)
				{
					Utils::DrawCannonExplotion(695 + 40 + 80, 340 + 90 + 40, 1);
				}
			}

			currCannon++;
			if(currCannon > 1) currCannon = 0;

			return;
		}
	}
}

void CaptainBoss::DrawSpecialAttackCountdown()
{
	if(enemyHolder == NULL || !bPerformSpecialAttack)
		return;

	float barXOff = -38.0f;
	float barYOff = -74.0f;
	float barHeight = 4;
	float barWidth = 75.0f;

	IwGxLightingOn();

	char str[100];
	sprintf(str, "%.2f", (restingTime.GetCurrTime_Reversed()) * 0.001f);

	uint32 color = 0xffffffff;
	if(restingTime.GetCurrTime_Reversed() <= 1200)
	{
		color = 0xff2525e7;
	}
	else if(!yellowTextTimer.GetIsEnd() && !yellowTextTimer.GetIsStop())
	{
		color = 0xff35e3dd; //dde335
	}
	
	Utils::RenderText(str, CIwRect((int)(enemyHolder->getPosX() + barXOff - xWindowOffset), (int)(enemyHolder->getPosY() + barYOff - yWindowOffset), (int)(barWidth), 16), font_komikadisplay11, color, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP);
	IwGxLightingOff();
}

void CaptainBoss::MarkPlaceToShoot(IsoTerrain* terrain, int delay, Skill_Type skillType)
{
	pGame->cannonWarningTile[terrain->getXNumTile()][terrain->getYNumTile()] = delay;
	pGame->cannonWarningSkillType[terrain->getXNumTile()][terrain->getYNumTile()] = skillType;
}

void CaptainBoss::Shoot(int delay, Skill_Type skillType, bool bFriendlyDamage, int percent)
{
	bool bAttackSelknam = true;
	if(bFriendlyDamage)
	{
		int prob = rand() % 100 + 1;
		if(percent <= prob)
		{
			bAttackSelknam = false;
		}
	}

	IsoSelknam* lastSelknamSelected = NULL;
	IsoEnemy* lastEnemySelected = NULL;

	if(bAttackSelknam)
	{
		for(int i = 0; i < MAX_ISO_SELKNAM; i++)
		{
			IsoSelknam* tmp = &pGame->isoSelknamArr[i];
			if(tmp != NULL
				&& tmp->getInitialized()
				&& !tmp->GetIsDead())
			{
				int x = tmp->GetTile()->getXNumTile();
				int y = tmp->GetTile()->getYNumTile();

				if(pGame->cannonWarningTile[x][y] > 0) continue;

				if(tmp->GetSelknamType() == SELKNAM_Warrior_Tank)
				{
					MarkPlaceToShoot(tmp->GetTile(), delay, skillType);
					return;
				}
				else
				{
					int m = rand() % 10;
					if(m < 4 || lastSelknamSelected == NULL)
					{
						lastSelknamSelected = tmp;
					}
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < MAX_ISO_ENEMIES; i++)
		{
			IsoEnemy* tmp = &pGame->isoEnemyArr[i];
			if(tmp != NULL
				&& tmp->getInitialized()
				&& !tmp->GetIsDead())
			{
				int x = tmp->GetTile()->getXNumTile();
				int y = tmp->GetTile()->getYNumTile();

				if(pGame->cannonWarningTile[x][y] > 0) continue;

				int m = rand() % 10;
				if(m < 4 || lastEnemySelected == NULL)
				{
					lastEnemySelected = tmp;
				}
			}
		}
	}


	if(lastSelknamSelected != NULL)
	{
		MarkPlaceToShoot(lastSelknamSelected->GetTile(), delay, skillType);
	}
	else if(lastEnemySelected != NULL)
	{
		MarkPlaceToShoot(lastEnemySelected->GetTile(), delay, skillType);
	}
	else
	{
		/*
		int randObj = rand() % 4;
		int x = pGame->objectivesNodes[randObj]->getXNumTile();
		int y = pGame->objectivesNodes[randObj]->getYNumTile();

		if(pGame->cannonWarningTile[x][y] <= 0)
		{
			MarkPlaceToShoot(pGame->objectivesNodes[randObj], delay, skillType);
		}
		else
		{
			*/
		int rX = rand() % 7 + 9;
		int rY = rand() % 10 + 4;
		if(pGame->isoTerrainAstarGrid[rX][rY])
		{
			MarkPlaceToShoot(pGame->isoTerrainAstarGrid[rX][rY], delay, skillType);
		}
		//}
	}
}

void CaptainBoss::EndObject()
{
	if(bIsActive)
	{
		bIsActive = false;

		for(int i = 0; i < MAX_ISO_ENEMIES; i++)
		{
			pGame->isoEnemyArr[i].CleanIso();
		}

		shipModel->SetPosition(CIwFVec3(150, 100, 10));
	}
}

void CaptainBoss::ApplyDebuffToBoss(Skill_Type skillType)
{
	if(skillType == Skill_Type_Root)
	{
		if(bAffectedByRoot && !shipModel->IsMoving())
		{
			bAffectedByRoot = false;
			restingTime.SetTimeLimit(restingTime.GetCurrTime_Reversed() + 3000);
			restingTime.Reset();
			restingTime.Start();

			yellowTextTimer.Reset();
			yellowTextTimer.Start();
		}
		else
		{
			enemyHolder->has_skill_effect[(int)Skill_Type_Root] = false;
		}
	}
	else if(skillType == Skill_Type_Confusion)
	{
		if(curCaptainCycle == 1 || curCaptainCycle == 2)
		{
			allyHitPercent = 20;
			enemyHitPercent = 80;
		}
		else if(curCaptainCycle == 3)
		{
			allyHitPercent = 30;
			enemyHitPercent = 70;
		}
	}
}

void CaptainBoss::EntranceScene()
{
	SetActive(true);

	bCaptainIgnorePauseStatus = true;
	bStartInitialDialog = true;
	bPauseGame = true;
	bBlockCamera = true;

	Utils::MoveCamera(100, CIwFVec2(0, 0), CIwFVec2((float)(130 - xWindowOffset), (float)(50 - yWindowOffset)));
	Move(CIwFVec3(200 - 40 - entranceOffsetX, 120 - 20 - entranceOffsetY, 0), 5000);
	PlayCaptainAnim(CaptainState_Attack);
}

void CaptainBoss::EndEntranceScene()
{
	bPauseGame = false;
	bBlockCamera = false;
	bCaptainIgnorePauseStatus = false;
}

void CaptainBoss::SetEntrancePosition()
{
	SetPosition(CIwFVec3(150 + 40 + entranceOffsetX, 100 + 20 + entranceOffsetY, 300));
}

void CaptainBoss::SetWaveSettings()
{
	/*
	ENEMY_Soldier,
	ENEMY_Varon,
	ENEMY_Convict,
	ENEMY_Scientist,
	ENEMY_Tank,
	ENEMY_Rabbit,
	ENEMY_Pirate,
	*/

	//------------------------------STAGE 1-----------------------//
	finalboss_num_waves[0] = 5;
	finalboss_unit_to_spawn[0][0][0] = ENEMY_Soldier;
	finalboss_unit_to_spawn[0][0][1] = ENEMY_Soldier;
	finalboss_unit_to_spawn[0][0][2] = ENEMY_Soldier;
	finalboss_unit_to_spawn[0][0][3] = ENEMY_Soldier;

	finalboss_unit_to_spawn[0][1][0] = ENEMY_Tank;
	finalboss_unit_to_spawn[0][1][1] = ENEMY_Varon;
	finalboss_unit_to_spawn[0][1][2] = ENEMY_Varon;
	finalboss_unit_to_spawn[0][1][3] = ENEMY_Soldier;

	finalboss_unit_to_spawn[0][2][0] = ENEMY_Varon;
	finalboss_unit_to_spawn[0][2][1] = ENEMY_Soldier;
	finalboss_unit_to_spawn[0][2][2] = ENEMY_Varon;
	finalboss_unit_to_spawn[0][2][3] = ENEMY_Pirate;

	finalboss_unit_to_spawn[0][3][0] = ENEMY_Scientist;
	finalboss_unit_to_spawn[0][3][1] = ENEMY_Varon;
	finalboss_unit_to_spawn[0][3][2] = ENEMY_Scientist;
	finalboss_unit_to_spawn[0][3][3] = ENEMY_Tank;

	finalboss_unit_to_spawn[0][4][0] = ENEMY_Soldier;
	finalboss_unit_to_spawn[0][4][1] = ENEMY_Soldier;
	finalboss_unit_to_spawn[0][4][2] = ENEMY_Convict;
	finalboss_unit_to_spawn[0][4][3] = ENEMY_Soldier;

	//------------------------------STAGE 2-----------------------//
	finalboss_num_waves[1] = 5;
	finalboss_unit_to_spawn[1][0][0] = ENEMY_Tank;
	finalboss_unit_to_spawn[1][0][1] = ENEMY_Tank;
	finalboss_unit_to_spawn[1][0][2] = ENEMY_Tank;
	finalboss_unit_to_spawn[1][0][3] = ENEMY_Tank;

	finalboss_unit_to_spawn[1][1][0] = ENEMY_Rabbit;
	finalboss_unit_to_spawn[1][1][1] = ENEMY_Rabbit;
	finalboss_unit_to_spawn[1][1][2] = ENEMY_Rabbit;
	finalboss_unit_to_spawn[1][1][3] = ENEMY_Rabbit;

	finalboss_unit_to_spawn[1][2][0] = ENEMY_Pirate;
	finalboss_unit_to_spawn[1][2][1] = ENEMY_Pirate;
	finalboss_unit_to_spawn[1][2][2] = ENEMY_Varon;
	finalboss_unit_to_spawn[1][2][3] = ENEMY_Varon;

	finalboss_unit_to_spawn[1][3][0] = ENEMY_Pirate;
	finalboss_unit_to_spawn[1][3][1] = ENEMY_Pirate;
	finalboss_unit_to_spawn[1][3][2] = ENEMY_Convict;
	finalboss_unit_to_spawn[1][3][3] = ENEMY_Convict;

	finalboss_unit_to_spawn[1][4][0] = ENEMY_Convict;
	finalboss_unit_to_spawn[1][4][1] = ENEMY_Convict;
	finalboss_unit_to_spawn[1][4][2] = ENEMY_Pirate;
	finalboss_unit_to_spawn[1][4][3] = ENEMY_Pirate;

	//------------------------------STAGE 3-----------------------//
	finalboss_num_waves[2] = 5;
	finalboss_unit_to_spawn[2][0][0] = ENEMY_Tank;
	finalboss_unit_to_spawn[2][0][1] = ENEMY_Tank;
	finalboss_unit_to_spawn[2][0][2] = ENEMY_Tank;
	finalboss_unit_to_spawn[2][0][3] = ENEMY_Tank;

	finalboss_unit_to_spawn[2][1][0] = ENEMY_Convict;
	finalboss_unit_to_spawn[2][1][1] = ENEMY_Convict;
	finalboss_unit_to_spawn[2][1][2] = ENEMY_Convict;
	finalboss_unit_to_spawn[2][1][3] = ENEMY_Convict;

	finalboss_unit_to_spawn[2][2][0] = ENEMY_Tank;
	finalboss_unit_to_spawn[2][2][1] = ENEMY_Scientist;
	finalboss_unit_to_spawn[2][2][2] = ENEMY_Pirate;
	finalboss_unit_to_spawn[2][2][3] = ENEMY_Scientist;

	finalboss_unit_to_spawn[2][3][0] = ENEMY_Scientist;
	finalboss_unit_to_spawn[2][3][1] = ENEMY_Scientist;
	finalboss_unit_to_spawn[2][3][2] = ENEMY_Pirate;
	finalboss_unit_to_spawn[2][3][3] = ENEMY_Pirate;

	finalboss_unit_to_spawn[2][4][0] = ENEMY_Scientist;
	finalboss_unit_to_spawn[2][4][1] = ENEMY_Pirate;
	finalboss_unit_to_spawn[2][4][2] = ENEMY_Pirate;
	finalboss_unit_to_spawn[2][4][3] = ENEMY_Tank;
}

bool CaptainBoss::CanBeHit()
{
	return !IsMoving() && curShipPosition == ShipPosition_Forward && lastShipPosition == curShipPosition;
}

void CaptainBoss::DestroyCannonsSceneUpdate()
{
	if(!bCannonExplotionScene)
	{
		return;
	}

	shipCannonExplotionTime.Update();
	shipCannonExplotionDelay.Update();

	if(shipCannonExplotionTime.GetIsEnd())
	{
		bCannonExplotionScene = false;
	}
	else
	{
		if(shipCannonExplotionDelay.GetIsEnd() || shipCannonExplotionDelay.GetIsStop())
		{
			int centerX = 620;
			int centerY = 380;
			int numExplotions = rand() % 5 + 1;
			for(int i = 0; i < numExplotions; i++)
			{
				int offX = rand() % 140;
				int offY = rand() % 80;
				Utils::DrawCannonExplotion((float)(centerX + offX), (float)(centerY + offY), 10);
				Utils::PlaySoundFX(FX_Explosion);
			}
			shipCannonExplotionDelay.Reset();
			shipCannonExplotionDelay.Start();
		}
	}
}

void CaptainBoss::PlaceInitialSelknams()
{
	SpawnSelknam(SELKNAM_Warrior_Tank, pGame->isoTerrainAstarGrid[9][14]);
	SpawnSelknam(SELKNAM_Warrior_Tank, pGame->isoTerrainAstarGrid[10][11]);
	SpawnSelknam(SELKNAM_Warrior_Tank, pGame->isoTerrainAstarGrid[11][7]);
	SpawnSelknam(SELKNAM_Warrior_Tank, pGame->isoTerrainAstarGrid[12][5]);
	SpawnSelknam(SELKNAM_Warrior_Tank, pGame->isoTerrainAstarGrid[10][3]);
}

void CaptainBoss::SpawnSelknam(IsoSelknamType type, IsoTerrain* terrain)
{
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(pGame->isoSelknamArr[i].GetSelknamType() != SELKNAM_None)
			continue;

		pGame->isoSelknamArr[i].InitSelknam(type, false, 3);
		pGame->isoSelknamArr[i].InitIsoPawn(terrain);

		return;
	}
}