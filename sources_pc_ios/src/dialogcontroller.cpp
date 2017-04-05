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

#include "common.h"
#include "localization.h"
#include "utils.h"
#include "captainboss.h"
#include "dialogcontroller.h"
#include "parserhelper.h"
#include "savedatahelper.h"
#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#endif

DialogController* dControllerRef = NULL;

void NextScene()
{
	if(dControllerRef != NULL)
		dControllerRef->GoToNextScene();
}

DialogController::DialogController()
{	
	dControllerRef = this;

	messageBoxWidth  = 768;
	messageBoxHeight = 170;
	messageBoxX      = (int)((currentGameWidth - messageBoxWidth) * 0.5f);
	messageBoxY      = currentGameHeight - messageBoxHeight - 30;
	textOffset       = 30;
	imageYOffset     = 0;
	bIsActive        = false;
	leftImageTextOffset  = 0;
	rightImageTextOffset = 0;
	messageBoxFadeTimer.SetTimeLimit(300);
	leftImagesMotionTimer[0].SetTimeLimit(300);
	rightImagesMotionTimer[0].SetTimeLimit(300);
	closingTimer.SetTimeLimit(500);
	bClosingAnimation = false;

	dialogSelknam1 = new RenderData();
	dialogEnemy1   = new RenderData();

	*dialogSelknam1 = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("dialog_selknam_1"));
	*dialogEnemy1   = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("dialog_enemy_1"));

	for(int i = 0; i < MAX_DIALOGS; i++)
	{
		activeDialogs[i] = false;
	}

	delayAllowTap.SetTimeLimit(200);

	//World 1 - Map 1
	RegisterDialog(1);
	CreateMessage(1, 0, loc_dialog_world_1_0);
	CreateMessage(1, 1, loc_dialog_world_1_1);
	CreateMessage(1, 2, loc_dialog_world_1_2);
	CreateMessage(1, 3, loc_dialog_world_1_3);
	CreateMessage(1, 4, loc_dialog_world_1_4);
	AddLeftImage(1, 2, DIALOG_SELKNAM_1);
	AddRightImage(1, 2, DIALOG_ENEMY_1);
	AddLeftImage(1, 3, DIALOG_SELKNAM_1);
	AddRightImage(1, 3, DIALOG_ENEMY_1);
	AddLeftImage(1, 4, DIALOG_SELKNAM_1);
	AddRightImage(1, 4, DIALOG_ENEMY_1);

	//World 2 - Map 1
	RegisterDialog(7);
	CreateMessage(7, 0, loc_dialog_world_2_0);
	CreateMessage(7, 1, loc_dialog_world_2_1);
	CreateMessage(7, 2, loc_dialog_world_2_2);
	CreateMessage(7, 3, loc_dialog_world_2_3);
	AddLeftImage(7, 0, DIALOG_ARCHER);
	AddRightImage(7, 0, DIALOG_BARON);
	AddLeftImage(7, 1, DIALOG_ARCHER);
	AddRightImage(7, 1, DIALOG_BARON);
	AddLeftImage(7, 2, DIALOG_ARCHER);
	AddRightImage(7, 2, DIALOG_BARON);
	AddLeftImage(7, 3, DIALOG_ARCHER);
	AddRightImage(7, 3, DIALOG_BARON);

	//World 3 - Map 1
	RegisterDialog(13);
	CreateMessage(13, 0, loc_dialog_world_3_0);
	CreateMessage(13, 1, loc_dialog_world_3_1);
	CreateMessage(13, 2, loc_dialog_world_3_2);
	AddLeftImage(13, 0, DIALOG_MAGE);
	AddRightImage(13, 0, DIALOG_SCIENTIST);
	AddLeftImage(13, 1, DIALOG_MAGE);
	AddRightImage(13, 1, DIALOG_SCIENTIST);
	AddLeftImage(13, 2, DIALOG_MAGE);
	AddRightImage(13, 2, DIALOG_SCIENTIST);

	//World 4 - Map 1
	RegisterDialog(19);
	CreateMessage(19, 0, loc_dialog_world_4_0);
	CreateMessage(19, 1, loc_dialog_world_4_1);
	CreateMessage(19, 2, loc_dialog_world_4_2);
	CreateMessage(19, 3, loc_dialog_world_4_3);
	CreateMessage(19, 4, loc_dialog_world_4_4);

	
	AddLeftImage(19, 0, DIALOG_BOWLER);
	AddRightImage(19, 0, DIALOG_TANK);

	AddLeftImage(19, 1, DIALOG_BOWLER);
	AddRightImage(19, 1, DIALOG_TANK);

	AddLeftImage(19, 2, DIALOG_BOWLER);
	AddRightImage(19, 2, DIALOG_TANK);

	AddLeftImage(19, 3, DIALOG_BOWLER);
	AddRightImage(19, 3, DIALOG_TANK);

	AddLeftImage(19, 4, DIALOG_BOWLER);
	AddRightImage(19, 4, DIALOG_TANK);

	//World 4 - Map 6 Ending.
	RegisterDialog(48);
	CreateMessage(48, 0, loc_dialog_world_4_5); //CHANCHANCHAN!

	//End Stage - Starting
	RegisterDialog(25);
	CreateMessage(25, 0, loc_dialog_world_end_0);
	CreateMessage(25, 1, loc_dialog_world_end_1);
	CreateMessage(25, 2, loc_dialog_world_end_2);

	AddRightImage(25, 0, DIALOG_CAPTAIN);
	AddRightImage(25, 1, DIALOG_CAPTAIN);
	AddRightImage(25, 2, DIALOG_CAPTAIN);

	//End Stage - Ending.
	RegisterDialog(50);
	CreateMessage(50, 0, loc_dialog_world_end_3);
	CreateMessage(50, 1, loc_dialog_world_end_4);
	CreateMessage(50, 2, loc_dialog_world_end_5);
	CreateMessage(50, 3, loc_dialog_world_end_6);
	AddRightImage(50, 0, DIALOG_CAPTAIN);
	AddLeftImage(50, 1, DIALOG_BOWLER);
	AddLeftImage(50, 1, DIALOG_ARCHER);
	AddLeftImage(50, 1, DIALOG_MAGE);
	AddLeftImage(50, 1, DIALOG_SELKNAM_1);

	//Easter Egg
	RegisterDialog(52);
	CreateMessage(52, 0, loc_dialog_world_altend_0);
	CreateMessage(52, 1, loc_dialog_world_altend_1);

#if defined DESKTOPPLATFORM
	xbox360Controller = new Xbox360Controller();
	xbox360Controller->SetCallback(Utils::GetActionButton1(), &NextScene);
	xbox360Controller->SetCallback(Utils::GetActionButton2(), &NextScene);
#endif
}

DialogController::~DialogController()
{
	delete dialogSelknam1;
	delete dialogEnemy1;
#if defined DESKTOPPLATFORM
	delete xbox360Controller;
#endif
}

bool DialogController::DialogExists(int dialogId)
{
	if(dialogId >= MAX_DIALOGS)
	{
		return false;
	}

	return activeDialogs[dialogId];
}

void DialogController::RegisterDialog(int dialogId)
{
	activeDialogs[dialogId] = true;
	lastScene[dialogId]     = 0;
}

void DialogController::StartDialog(int dialogId)
{
	if(!DialogExists(dialogId))
	{
		return;
	}

	currentActiveDialogId = dialogId;
	currentActiveScene    = 0;
	bIsActive             = true;

	//Boss appears.
	if(currentActiveDialogId == 25 && currentActiveScene == 0)
	{
		Utils::PlaySoundFX(FX_Boss_laugh);
	}
	
	storeLastInGameState = inGameState;
	//inGameState = GSG_Cinematic;
	bShakeCamera = true;

	ResizeTextSpace();

	messageBoxFadeTimer.Start();
	delayAllowTap.Reset();
	delayAllowTap.Start();

	Utils::PauseGame();
	Utils::FreezeCamera();
}

void DialogController::RenderMessageBox(string content)
{
	float factor         = messageBoxFadeTimer.GetCurrTime() / (float)messageBoxFadeTimer.GetTimeLimit();
	
	if(factor > 1.0f)
	{
		factor = 1.0f;
	}

	int   backgroundFade = (int)(168 * factor);

	if(bClosingAnimation)
	{
		backgroundFade =  (int)(((closingTimer.GetTimeLimit() - closingTimer.GetCurrTime()) * 168) / (float)closingTimer.GetTimeLimit());
	
		if(backgroundFade < 0)
		{
			backgroundFade = 0;
		}
	}

	Utils::RenderColor(0, 0, 0, backgroundFade, CIwSVec2(messageBoxX, messageBoxY),                        CIwSVec2(messageBoxWidth, messageBoxHeight));
	IwGxFlush();

	if(messageBoxFadeTimer.GetIsEnd() && !bClosingAnimation)
	{
		IwGxLightingOn();
		Utils::RenderText(content.c_str(), CIwRect(messageBoxX + textOffset + leftImageTextOffset, messageBoxY - 10, messageBoxWidth - 2 * textOffset - leftImageTextOffset - rightImageTextOffset, messageBoxHeight), font_SmartKid20, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		
		if(delayAllowTap.GetIsEnd() && !BossStageSpecialDialogCondition())
		{
#if !defined DESKTOPPLATFORM
			Utils::RenderText(loc_dialog_tapToContinue, CIwRect(messageBoxX + textOffset + leftImageTextOffset, messageBoxY + messageBoxHeight - 32, messageBoxWidth - 2 * textOffset - leftImageTextOffset - rightImageTextOffset, 30), font_SmartKid18, 0xffffffff);
#endif
#if defined DESKTOPPLATFORM
			Utils::RenderText(loc_dialog_tapToContinue_alt, CIwRect(messageBoxX + textOffset + leftImageTextOffset, messageBoxY + messageBoxHeight - 32, messageBoxWidth - 2 * textOffset - leftImageTextOffset - rightImageTextOffset, 30), font_SmartKid18, 0xffffffff);
#endif
		}
		
		IwGxLightingOff();
	}
}

void DialogController::Render()
{
	if(!bIsActive) 
	{
		return;
	}

	RenderMessageBox(dialogObject[currentActiveDialogId][currentActiveScene].GetContent());

	for(int i = dialogObject[currentActiveDialogId][currentActiveScene].GetNumLeftImages() - 1; i >= 0; i--)
	{
		//int offsetX = (int)(((leftImagesMotionTimer[i].GetTimeLimit() - leftImagesMotionTimer[i].GetCurrTime()) * leftInitialXOffset) / (float)leftImagesMotionTimer[i].GetTimeLimit());
		int offsetX = (int)(((leftImagesMotionTimer[0].GetTimeLimit() - leftImagesMotionTimer[0].GetCurrTime()) * leftInitialXOffset) / (float)leftImagesMotionTimer[0].GetTimeLimit());

		RenderData data = GetImageData(dialogObject[currentActiveDialogId][currentActiveScene].GetImage(i));
		CIwSVec2 dataDimension = TextureHelper::GetDimension(data);
		int x = messageBoxX - (int)(dataDimension.x * 0.5f) + 10 - offsetX - (12 * (dialogObject[currentActiveDialogId][currentActiveScene].GetNumLeftImages() - 1));
		int y = messageBoxY + messageBoxHeight - dataDimension.y + 15; 

		x += 90 * i;

		//IwTrace(DIALOGSYSTEM, ("%d %d", i, x));

		if(bClosingAnimation)
		{
			float closingFactor =  (int)((closingTimer.GetTimeLimit() - closingTimer.GetCurrTime()) * 255) / (float)closingTimer.GetTimeLimit();
			
			if(closingFactor < 0)
			{
				closingFactor = 0;
			}

			Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(data.atlasName), CIwFVec2((float)(x + imageXOffset), (float)(y + imageYOffset)), data, (int)(closingFactor));
		}
		else
		{
			float factor         = leftImagesMotionTimer[0].GetCurrTime() / (float)leftImagesMotionTimer[0].GetTimeLimit();

			if(factor > 1)
			{
				factor = 1;
			}
			
			Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(data.atlasName), CIwFVec2((float)(x + imageXOffset), (float)(y + imageYOffset)), data, (int)(255 * factor));
		}

		IwGxFlush();
	}

	for(int i = 0; i < dialogObject[currentActiveDialogId][currentActiveScene].GetNumRightImages(); i++)
	{
		int offsetX = (int)(((rightImagesMotionTimer[i].GetTimeLimit() - rightImagesMotionTimer[i].GetCurrTime()) * rightInitialXOffset) / (float)rightImagesMotionTimer[i].GetTimeLimit());

		RenderData data = GetImageData(dialogObject[currentActiveDialogId][currentActiveScene].GetRightImage(i));
		CIwSVec2 dataDimension = TextureHelper::GetDimension(data);
		int x = messageBoxX - (int)(dataDimension.x * 0.5f) + 10 - offsetX + messageBoxWidth - 60;
		int y = messageBoxY + messageBoxHeight - dataDimension.y + 50; 

		if(bClosingAnimation)
		{
			float closingFactor =  (int)((closingTimer.GetTimeLimit() - closingTimer.GetCurrTime()) * 255) / (float)closingTimer.GetTimeLimit();
			
			if(closingFactor < 0)
			{
				closingFactor = 0;
			}

			Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(data.atlasName), CIwFVec2((float)(x + imageXOffset), (float)(y + imageYOffset)), data, (int)(closingFactor));
		}
		else
		{
			float factor         = rightImagesMotionTimer[i].GetCurrTime() / (float)rightImagesMotionTimer[i].GetTimeLimit();

			if(factor > 1)
			{
				factor = 1;
			}

			Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(data.atlasName), CIwFVec2((float)(x + imageXOffset), (float)(y + imageYOffset)), data, (int)(255 * factor));
		}
	}
}

RenderData DialogController::GetImageData(DIALOG_IMAGE image)
{
	imageYOffset = 0;
	imageXOffset = 0;

	if(image == DIALOG_SELKNAM_1)
	{
		return TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("selknam_warrior"));
	}
	else if(image == DIALOG_ENEMY_1)
	{
		return TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("enemy_soldier"));
	}
	
	if(image == DIALOG_BARON) 
	{
		imageYOffset = -30;
		return TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("enemy_varon")), MIRROR_VERTICAL);
	}

	if(image == DIALOG_SCIENTIST) 
	{
		imageYOffset = -35;
		imageXOffset = 10;
		return TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("enemy_scientist")), MIRROR_VERTICAL);
	}

	if(image == DIALOG_TANK) 
	{
		imageYOffset = -10;
		imageXOffset = 5;
		return TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("enemy_tank")), MIRROR_VERTICAL);
	}

	if(image == DIALOG_CAPTAIN) 
	{
		imageYOffset = -30;
		return TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("enemy_boss")), MIRROR_VERTICAL);
	}

	if(image == DIALOG_ARCHER) 
	{
		imageYOffset = 25;
		return TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("selknam_archer"));
	}

	if(image == DIALOG_MAGE) 
	{
		return TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("selknam_mage"));
	}

	if(image == DIALOG_BOWLER) 
	{
		imageYOffset = 15;
		return TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("selknam_bowler")), MIRROR_VERTICAL);
	}

	return TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("selknam_warrior"));;
}

void DialogController::Update()
{
	if(!bIsActive)
	{
		return;
	}

#if defined DESKTOPPLATFORM
	xbox360Controller->Update();
#endif

	delayAllowTap.Update();

	if(bClosingAnimation && closingTimer.GetIsEnd())
	{
		Utils::UnpauseGame();

		if(!bShowTutorialLevel1
		   && currentMap == 1
		   && gameMode == GAMEMODE_CAMPAIGN 
		   || currentMap != 1 
		   || gameMode != GAMEMODE_CAMPAIGN)
		{
			Utils::UnfreezeCamera();
		}

		if(gameMode == GAMEMODE_CAMPAIGN)
		{
			if(currentMap == 24)
			{
				//if(bShakeCamera)
				//{
				//	bShakeCamera = false;
				//	Utils::ShakeCamera(120, 6, 350, 4);
				//}
				//else
				//{
					if(!Utils::IsCameraShaking())
					{
						gameState = GS_Retry;
						currentMap = 25;
						bIsActive = false;
						bClosingAnimation = false;
					}
				//}
			}
			else if(currentActiveDialogId == 52)
			{
				gameState = GS_MainMenu;
				mainMenuState = GSM_FinalScreen;
				Stars_By_Level[24] = 1;
				if(saveDataHelper)
					saveDataHelper->SaveGameDataCampaign();

				bIsActive = false;
				bClosingAnimation = false;
			}
			else if(currentActiveDialogId == 50)
			{
				captainBoss->EndGameAction();
				bIsActive = false;
				bClosingAnimation = false;
			}
			else
			{
				bClosingAnimation = false;
				bIsActive = false;
			}
		}
		else
		{
			bClosingAnimation = false;
			bIsActive = false;
		}
	}

	closingTimer.Update();
	messageBoxFadeTimer.Update();
	for(int i = 0; i < 4; i++)
	{
		leftImagesMotionTimer[i].Update();
	}

	rightImagesMotionTimer[0].Update();
}

void DialogController::CreateMessage(int dialogId, int sceneId, string content)
{
	if(sceneId > lastScene[dialogId])
	{
		lastScene[dialogId] = sceneId;
	}

	dialogObject[dialogId][sceneId].SetContent(content);
}

void DialogController::UpdateClickEvents(s3ePointerEvent *event)
{
	if(!bIsActive) 
	{
		return;
	}

	if(event->m_Pressed)
	{
		if(event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!bClosingAnimation)
			{
				GoToNextScene();
			}
		}
	}
}

void DialogController::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!bIsActive) 
	{
		return;
	}

	//nextButton->HandleMotion(event);
}

bool DialogController::IsActive()
{
	return bIsActive;
}

bool DialogController::BossStageSpecialDialogCondition()
{
	return currentMap == 25 && currentActiveDialogId == 50 && currentActiveScene == 1 && captainBoss != NULL && captainBoss->bEndingScenePlaying;
}

void DialogController::GoToNextScene()
{
	if(!delayAllowTap.GetIsEnd()) 
	{
		return;
	}

	//Special dialog 50 handling
	if(BossStageSpecialDialogCondition())
	{
		return;
	}

	currentActiveScene++;

	delayAllowTap.Reset();
	delayAllowTap.Start();

	

	if(currentActiveDialogId == 25 && currentActiveScene == 2)
	{
		captainBoss->EndEntranceScene();
	}

	if(currentActiveScene > lastScene[currentActiveDialogId])
	{
		currentActiveScene--;
		bClosingAnimation = true;
		closingTimer.Start();

		if(currentMap == 24)
		{
			fade->SetColor(255, 255, 255);
			Utils::ShakeCamera(50, 6, 180, 3);
			Utils::PlaySoundFX(FX_Boss_appear_explosion);
		}
	}
	else
	{
		ResizeTextSpace();
	}

	Utils::PlaySoundFX(FX_Click_01);
}

void DialogController::AddLeftImage(int dialogId, int sceneId, DIALOG_IMAGE image)
{
	dialogObject[dialogId][sceneId].AddLeftImage(image);
}

void DialogController::AddRightImage(int dialogId, int sceneId, DIALOG_IMAGE image)
{
	dialogObject[dialogId][sceneId].AddRightImage(image);
}

void DialogController::ResizeTextSpace()
{
	int leftOffset  = 115 + (dialogObject[currentActiveDialogId][currentActiveScene].GetNumLeftImages() - 1) * 120;
	int rightOffset = 120;

	if(dialogObject[currentActiveDialogId][currentActiveScene].GetNumLeftImages() > 0)
	{
		leftImageTextOffset = leftOffset;
		if(currentActiveScene == 0 || dialogObject[currentActiveDialogId][currentActiveScene - 1].GetImage(0) != dialogObject[currentActiveDialogId][currentActiveScene].GetImage(0))
		{
			leftImagesMotionTimer[0].Start();
		}
		leftInitialXOffset = 100;
	}
	else
	{
		leftImageTextOffset = 0;
	}

	if(dialogObject[currentActiveDialogId][currentActiveScene].GetNumRightImages() > 0)
	{
		rightImageTextOffset = rightOffset;
		if(currentActiveScene == 0 || dialogObject[currentActiveDialogId][currentActiveScene - 1].GetRightImage(0) != dialogObject[currentActiveDialogId][currentActiveScene].GetRightImage(0))
		{
			rightImagesMotionTimer[0].Start();
		}
		rightInitialXOffset = -100;
	}
	else
	{
		rightImageTextOffset = 0;
	}
}