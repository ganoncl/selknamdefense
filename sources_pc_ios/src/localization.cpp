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
#include "SimpleIni.h"

CSimpleIniCaseA simpleIniApi;

CIwGxFont* font_komikadisplaybold25;
CIwGxFont* font_komikadisplay11;
CIwGxFont* font_komikadisplay28;
CIwGxFont* font_komikadisplay14;
CIwGxFont* font_komikadisplay18;
CIwGxFont* font_komikadisplaybold18;
CIwGxFont* font_komikadisplay22;
CIwGxFont* font_komikadisplaybold22;
CIwGxFont* font_komikadisplaybold48;
CIwGxFont* font_SmartKid12;
CIwGxFont* font_SmartKid16;
CIwGxFont* font_SmartKid18;
CIwGxFont* font_SmartKid20;
CIwGxFont* font_SmartKid22;
CIwGxFont* font_SmartKid24;
CIwGxFont* font_EpsonJap18;

const char* loc_splash_loading = "   LOADING...";
const char* loc_splash_version = "VERSION : ";
const char* loc_splash_fullVersion = "FULL  VERSION : ";
const char* loc_splash_freeVersion = "FREE  VERSION : ";
const char* loc_ui_back = "BACK";
const char* loc_ui_close = "CLOSE";
const char* loc_ui_ok = "OK";
const char* loc_ui_accept = "ACCEPT";
const char* loc_ui_cancel = "CANCEL";
const char* loc_ui_next = "NEXT";
const char* loc_ui_skip = "SKIP";
const char* loc_ui_finish = "FINISH";
const char* loc_ui_playGame = "PLAY GAME";
const char* loc_ui_removeAds = "REMOVE ADS";
const char* loc_ui_newCampaign = "NEW CAMPAIGN";
const char* loc_ui_playCampaign = "PLAY CAMPAING";
const char* loc_ui_almanac = "ALMANAC";
const char* loc_ui_options = "OPTIONS";
const char* loc_ui_restore = "RESTORE PURCHASES";
const char* loc_ui_settings = "SETTINGS";
const char* loc_ui_credits = "CREDITS";
const char* loc_ui_survival = "SURVIVAL";
const char* loc_ui_quitGame = "QUIT GAME";
const char* loc_ui_exitGame = "EXIT GAME";
const char* loc_ui_pause = "PAUSE";
const char* loc_ui_resume = "RESUME";
const char* loc_ui_howToPlay = "HOW TO PLAY";
const char* loc_ui_retry = "RETRY";
const char* loc_ui_worldMap = "WORLD MAP";
const char* loc_ui_playAgain = "PLAY AGAIN";
const char* loc_ui_continue = "CONTINUE";
const char* loc_ui_upgrades = "UPGRADES";
const char* loc_ui_selknams = "SELKNAMS";
const char* loc_ui_enemies = "ENEMIES";
const char* loc_ui_time = "TIME: ";
const char* loc_ui_wave = "WAVE: ";
const char* loc_ui_congratulations = "CONGRATULATIONS!!";
const char* loc_ui_newRecord = "NEW RECORD:";
const char* loc_ui_yourRecord = "YOUR RECORD:";
const char* loc_ui_bestRecord = "BEST RECORD:";
const char* loc_ui_noRecord = "NOT ENOUGH FOR THE BEST RECORD!\nTRY AGAIN NEXT TIME.";
const char* loc_ui_victory = "VICTORY";
const char* loc_ui_defeat = "DEFEAT";
const char* loc_ui_scoreHeartBonus = "HEART BONUS : ";
const char* loc_ui_scoreCoinsBonus = "COINS BONUS : ";
const char* loc_ui_scoreWaveBonus = "WAVE BONUS : ";
const char* loc_ui_scoreTotal = "TOTAL : ";
const char* loc_ui_onlyFullVersion = "ONLY FULL\nVERSION";
const char* loc_ui_leaderboards = "LEADERBOARDS";
const char* loc_ui_gamecenter = "GAME CENTER";
const char* loc_ui_music = "MUSIC";
const char* loc_ui_fxSound = "FX SOUND";
const char* loc_ui_fullscreen = "FULL SCREEN";
const char* loc_ui_muteAll = "MUTE ALL";

//popups
const char* loc_popup_savedData = "SAVED DATA WILL BE DELETED, DO YOU WANT TO CONTINUE?";
const char* loc_popup_quitGame = "THE GAME WILL BE CLOSED.\nARE YOU SURE?";
const char* loc_popup_noFeature = "THIS FEATURE IS NOT EMPLEMENTED YET.";
const char* loc_popup_retry = "ARE YOU SURE YOU WANT TO START AGAIN?";
const char* loc_popup_returnToMenu = "ARE YOU SURE YOU WANT TO RETURN TO MAIN MENU?";
const char* loc_popup_returnToWorldMap = "ARE YOU SURE YOU WANT TO RETURN TO WORLD MAP?";
const char* loc_popup_upgrades = "THE UPGRADES WILL BE DELETED BUT THE STARS WILL REMAIN. DO YOU WANT TO CONTINUE?";
const char* loc_popup_survival = "TO PLAY THIS MODE YOU MUST COMPLETE THE THREE FIRST WORLDS IN THE CAMPAIGN MODE.";
const char* loc_popup_trial = "TO PLAY THIS MODE YOU MUST HAVE THE FULL VERSION.";
const char* loc_popup_thanks = "THANKS FOR SHARING.";
const char* loc_popup_error = "ERROR.";
const char* loc_popup_leaderboarderror = "LEADERBOARDS ERROR";
const char* loc_popup_PopupInternetConnection = "YOU NEED TO BE CONNECTED TO INTERNET TO PLAY.";

//Selknam Upgrade
const char* loc_upgrade_selknamUpgrade = "SELKNAM UPGRADE";
const char* loc_portrait_archerBase = "SNIPER";
const char* loc_portrait_archerSlower = "SLOWER";
const char* loc_portrait_archerPoisoner = "POISONER";
const char* loc_portrait_bowlerStunner = "STUNNER";
const char* loc_portrait_bowlerConfuser = "CONFUSER";
const char* loc_portrait_bowlerRooter = "ROOTER";
const char* loc_portrait_warriorBase = "WARRIOR";
const char* loc_portrait_warriorBerserk = "BERSERK";
const char* loc_portrait_warriorTank = "TANK";
const char* loc_portrait_shamanHealer = "HEALER";
const char* loc_portrait_shamanSpeeder = "SPEEDER";
const char* loc_portrait_shamanMighter = "MIGHTER";
const char* loc_upgrade_attributes = "ATTRIBUTES";
const char* loc_upgrade_health = "HEALTH";
const char* loc_upgrade_damage = "DAMAGE";
const char* loc_upgrade_speed = "SPEED";
const char* loc_upgrade_attackSpeed = "ATTACK SP";
const char* loc_upgrade_castSpeed = "CAST SP";
const char* loc_upgrade_range = "RANGE";
const char* loc_upgrade_cost = "COST";;
const char* loc_upgrade_skill = "SKILL";
const char* loc_upgrade_skillSlow = "SLOW";
const char* loc_upgrade_skillPoison = "POISON";
const char* loc_upgrade_skillStun = "STUN";
const char* loc_upgrade_skillConfusion = "CONFUSION";
const char* loc_upgrade_skillRoot = "ROOT";
const char* loc_upgrade_skillHeal = "HEAL";
const char* loc_upgrade_skillHaste = "HASTE";
const char* loc_upgrade_skillMight = "MIGHT";

const char* loc_description_skillSlow = "SLOW EXPLANATION";
const char* loc_description_skillPoison = "POISON EXPLANATION";
const char* loc_description_skillStun = "STUN EXPLANATION";
const char* loc_description_skillConfusion = "CONFUSION EXPLANATION";
const char* loc_description_skillRoot = "ROOT EXPLANATION";
const char* loc_description_skillHeal = "HEAL EXPLANATION";
const char* loc_description_skillHaste = "HASTE EXPLANATION";
const char* loc_description_skillMight = "MIGHT EXPLANATION";

//Leaderboars
const char* loc_leaderboards_title = "LEADERBOARDS";
const char* loc_leaderboards_rank = "RANK";
const char* loc_leaderboards_playername = "NAME";
const char* loc_leaderboards_score = "TOTAL";
const char* loc_leaderboards_time = "TIME";
const char* loc_leaderboards_prevpage = "PREV PAGE";
const char* loc_leaderboards_nextpage = "NEXT PAGE";
const char* loc_leaderboards_loading = "LOADING LEADERBOARDS...";
const char* loc_leaderboards_overall = "OVERALL";
const char* loc_leaderboards_friends = "FRIENDS";
const char* loc_leaderboards_myrank  = "MY RANK";
const char* loc_leaderboards_stage   = "STAGE";
const char* loc_leaderboards_survival = "SURVIVAL";
const char* loc_leaderboards_campaign = "CAMPAIGN";
const char* loc_leaderboards_notAvailable = "NOT AVAILABLE";

//Selknam Almanac
const char* loc_almanac_baseArcherTitle                = "BASE ARCHER";
const char* loc_almanac_baseArcherInformation          = "They were trained to hunt with their camouflage and long range arrows, skills that would give them advantage against the enemies.";
const char* loc_almanac_poisonArcherTitle              = "POISONER ARCHER";
const char* loc_almanac_poisonArcherInformation        = "Trained as well to hunt, but they are quicker to shoot a more effective kind of arrow that has poison on the tip extracted from poisonous animals and plants to hunt bigger prey. They are not killers, but under threat they WILL defend their territory.";
const char* loc_almanac_unitDelayerTitle               = "UNIT DELAYER ARCHER";   
const char* loc_almanac_unitDelayerInformation         = "High Ranked Archer. In quest for better and more effective hunting methods, the Shamans from the tribe called upon the forces of nature to favorite their people and obtained ice arrows that make prey slow and easy to kill. They will not hesitate to use their ice arrows against enemies to slow them down and let the warriors damage them longer.";
const char* loc_almanac_bolasThrowerBaseTitle          = "BOLAS THROWER BASE";
const char* loc_almanac_bolasThrowerBaseInformation    = "Teamed up with archers to trap their prey. They use their throwing skills and strength to damage and kill their prey as fast and as painless as possible, out of respect for the animals given by nature to them as food. But they will not be as merciful with the settlers invading their territory. They are not as assertive as archers.";
const char* loc_almanac_bolasThrowerFloorTitle         = "BOLAS THROWER THAT ROOT\nTO THE FLOOR";
const char* loc_almanac_bolasThrowerFloorInformation   = "As well as delayer archers they were trained to hunt but were given Shamanic powers to use mother nature in their favor teaming up with the plants and trees to entrap their pray by throwing bolas that root them for a while, so archers can shoot their deadly arrows while they are delayed in their advance. Along with their people they will use their skills to defeat the attacking settlers.";
const char* loc_almanac_bolasThrowerConfuseTitle       = "BOLAS THROWER THAT CONFUSES";
const char* loc_almanac_bolasThrowerConfuseInformation = "Only the strongest members of the Selknam tribe were trained to become confusers, they throw the bolas at the head of their prey to knock them out or confuse them so they have more time to trap their prey. They are more aggressive than your average Selknam.";
const char* loc_almanac_baseWarriorTitle               = "BASE WARRIOR";
const char* loc_almanac_baseWarriorInformation         = "These are average Selknam that usually wrestle as entertainment and amusement of the rest of the tribe, but their skills will be useful at the time of conflict and contention of enemy forces.";
const char* loc_almanac_resistantWarriorTitle          = "RESISTANT WARRIOR";
const char* loc_almanac_resistantWarriorInformation    = "Before the menace of the attacking settlers, the warriors have implemented artifacts as armor to improve their resistance and contention. They have better chances to defeat their enemies with their tenacity.";
const char* loc_almanac_aggressiveWarriorTitle         = "AGGRESIVE WARRIOR";
const char* loc_almanac_aggressiveWarriorInformation   = "These Selknam have lost their peaceful side at the sight of the settlers taking over their land, and they will not wait to be attacked, they will be offensive in their attacks. Armed with weapons, they will inflict as much damage as possible.";
const char* loc_almanac_baseMageTitle                  = "BASE MAGE";
const char* loc_almanac_baseMageInformation            = "Shaman of the tribe, with the ability to heal the members of his tribe that bravely go into battle. He calls upon the ancestor spirits to bring health to his allies and continue the fight to free their people from extinction.";
const char* loc_almanac_speederMageTitle               = "SPEEDER MAGE";
const char* loc_almanac_speederMageInformation         = "This Shaman has remained in contact with the fastest animals of this environment, learning the skills of nature's spirit to improve the attack speed of his people to help them achieve victory on the battles and eradicate the settlers for good."; 
const char* loc_almanac_mightierMageTitle              = "MIGHTER MAGE";
const char* loc_almanac_mightierMageInformation        = "Shaman of the tribe that has chosen the skills of strengthening, learning from rocks, water and mighty animals. With his wisdom and magic, he casts spells on his people to give them extra stamina in their attacks becoming mightier and more difficult to defeat.";

const char* loc_almanac_soldierTitle		= "SOLDIER";
const char* loc_almanac_soldierInfo			= "Your average untrained but aggressive European gold digger, he's engaged in bar fights and is skilled with his knife, but he's not used to confronting organized battles. He is ruthless and will not hesitate to kill.";
const char* loc_almanac_baronTitle			= "BARON";
const char* loc_almanac_baronInfo			= "Educated and noble European that has come to find new richness in the new unconquered lands, and is willing to participate in the battle to obtain lands and become the landlord. He will fearlessly fight for his ambition. He is skilled with guns, but the damage he inflicts is average.";
const char* loc_almanac_convictTitle		= "CONVICT";
const char* loc_almanac_convictInfo			= "Not many people were willing to journey to these lands out of fear of savages and the unknown. But convicts had nothing to live for and were not frightened, so it was a better option than staying in prison, even though they will sacrifice themselves for the good of the conquering mission by carrying gun powder and exploding along with whatever is around them.";
const char* loc_almanac_chemistTitle		= "THE CHEMIST";
const char* loc_almanac_chemistInfo			= "This chemist had become an alcoholic and was no good anymore to the European society and so he chose to offer himself for the quest, offering his chemistry knowledge to give the troops some advantage with chemical weapons. He himself will carry the poisonous chemical and additionally will attack, combating melee. But if he is killed he'll release the poison he has created.";
const char* loc_almanac_tankTitle			= "TANK";
const char* loc_almanac_tankInfo			= "This is a resistance soldier , trained for battle,  he won't attack the Selknam but will endure their attacks for a long time even long enough to arrive to their sacred place making them vulnerable to attacks. While Selknam try to bring these soldiers down, the other enemies will take advantage from the distraction.";
const char* loc_almanac_rabbitTitle			= "RABBIT";
const char* loc_almanac_rabbitInfo			= "This mad scientist has acquired much technology in his research and has developed a super speed quality and will evade attacks and quickly arrive to the Selknam danger zone. Since they don't have any armor they are more vulnerable to attacks but their super speed makes them a difficult target.";
const char* loc_almanac_pirateTitle			= "PIRATE";
const char* loc_almanac_pirateInfo			= "The Captain's right arm. This ruthless pirate has been to many battles and has won them all he has no loyalty and no remorse he will only try to benefit from the occasion, the captain has promised them all a juicy bounty once they eradicate the Selknam or take them as slaves. He is Stronger than the Gold Digger and will fight with his hook and sword.";
const char* loc_almanac_captainTitle		= "THE CAPTAIN";
const char* loc_almanac_captainInfo			= "His ambition is bigger than any of his troops, and he will not be defeated by savages, if he has to, he will take matter into his own hands and as a good captain will sink with his Ship. He will be strong and armed and has minions to his disposal. He Considers that these lands are already his.";


const char* loc_credits_title0 = "Team Lead & Programmer";
const char* loc_credits_title1 = "Game Design / Art";
const char* loc_credits_title2 = "Senior Programmer";
const char* loc_credits_title3 = "Game Design / Concept";
const char* loc_credits_title4 = "Music Production / Composer";
const char* loc_credits_title5 = "SPECIAL THANKS";
const char* loc_credits_name0 = "Sebastian Gana";
const char* loc_credits_name1 = "Rodrigo Alarcon";
const char* loc_credits_name2 = "Javier Gonzalez";
const char* loc_credits_name3 = "Walter Berendsen";
const char* loc_credits_name4 = "Matias Castro";
const char* loc_credits_name5 = "Leo Benaducci, Luis Santander";
const char* loc_credits_name6 = "Sebastian Suarez, Ignacio Correa";
const char* loc_credits_name7 = "ACE Team members";
const char* loc_credits_contact = "Contact us: contact@bad-games.com";
const char* loc_credits_marmalade = "www.madewithmarmalade.com";

const char* loc_tutorial_part0 = "WELCOME TO SELKNAM TUTORIAL!";
const char* loc_tutorial_part1 = "THIS IS THE BASE OF YOUR SELKNAM ALLIES. \nYOU MUST DEFEND IT!";
const char* loc_tutorial_part2 = "THE ENEMIES WILL COME THROUGH HERE. \nYOU HAVE TO STOP THEM FROM REACHING THE BASE.";
const char* loc_tutorial_part3 = "WHENEVER AN ENEMY REACHES YOUR BASE YOU LOSE A LIFE.";
const char* loc_tutorial_part4 = "TO DEFEND YOUR BASE YOU HAVE TO DEPLOY SELKNAMS\nTO ATTACK THE ENEMIES.";
const char* loc_tutorial_part5 = "YOU WILL EARN COINS FOR EACH DESTROYED ENEMY.";
const char* loc_tutorial_part6 = "A SELKNAMS ATTACKS ENEMIES AUTOMATICALLY\nIF AN ENEMY IS WITHIN SELKNAM RANGE.";
const char* loc_tutorial_part7 = "ENEMIES WILL ATTACK IN WAVES IN A CERTAIN PERIOD OF TIME.";
const char* loc_tutorial_part8 = "MOVE THE MAP BY TOUCHING THE SCREEN, \nAND CLICK ON A TILE TO PLACE A UNIT WHERE AVAILABLE.";
const char* loc_tutorial_part9 = "TOUCH THE UNIT MENU TO CHOOSE YOUR DEFENDERS, \nTHEN TOUCH AGAIN TO PLACE THE SELECTED UNIT.";
const char* loc_tutorial_part10 = "YOU GET STARS FROM WINNING BATTLES!\nGO TO UPGRADES TO IMPROVE YOUR UNITS.";
const char* loc_tutorial_part11 = "SURVIVAL MODE UNLOCKED. YOU CAN GO TO MAIN MENU TO PLAY THIS MODE.";
const char* loc_tutorial_part12 = "DOUBLE TAP TO UNLOCK/LOCK CHARACTERS AND UPGRADES";
const char* loc_tutorial_part13 = "DOUBLE CLICK TO UNLOCK/LOCK CHARACTERS AND UPGRADES";
const char* loc_tutorial_part14 = "DOUBLE CLICK TO UNLOCK/LOCK CHARACTERS AND UPGRADES";
const char* loc_tutorial_part15 = "DOUBLE CLICK TO UNLOCK/LOCK CHARACTERS AND UPGRADES";
const char* loc_tutorial_part16 = "DOUBLE CLICK TO UNLOCK/LOCK CHARACTERS AND UPGRADES";
const char* loc_tutorial_part17 = "DOUBLE CLICK TO UNLOCK/LOCK CHARACTERS AND UPGRADES";
const char* loc_tutorial_part18 = "DOUBLE CLICK TO UNLOCK/LOCK CHARACTERS AND UPGRADES";

//world dialogs
const char* loc_dialog_tapToContinue = "Tap to continue";
const char* loc_dialog_tapToContinue_alt = "Click to continue";
const char* loc_dialog_enemy = "Enemy:";
const char* loc_dialog_selknam = "Selknam:";
const char* loc_dialog_world_1_0 = "The Selknam men and women lived peacefully in the lands far down south of the Americas. That was until the Europeans, in quest for acknowledgment, land and power arrived to Tierra del Fuego.";
const char* loc_dialog_world_1_1 = "Where the Selknam had to protect their lives and territory from the foreigners.";
const char* loc_dialog_world_1_2 = "Enemy:\nAt last! We've arrived to these lands. The Captain will give us a good reward after we kick these indians out!";
const char* loc_dialog_world_1_3 = "Selknam:\nYou will never move us from here, we've lived in these lands even before you existed...You will not defeat us!";
const char* loc_dialog_world_1_4 = "Selknam:\nYou will never move us from here, we've lived in these lands even before you existed...You will not defeat us!";
const char* loc_dialog_world_2_0 = "Enemy:\nThese Indians are tougher than we thought, but it doesn't matter,";
const char* loc_dialog_world_2_1 = "Enemy:\nWith the Captain and his allies on our side, their time has come to an end. It's now or never!";
const char* loc_dialog_world_2_2 = "Selknam:\nThese Mountains and Lands are our Mother, we've lived here in peace for many centuries.";
const char* loc_dialog_world_2_3 = "Selknam:\nWe will not allow you to take away our home. We will not give you the land peacefully; we will fight to defend our lives!";
const char* loc_dialog_world_3_0 = "Enemy:\nBrrr, this place is cold! BRING MORE RUM! We will need it.\nAnnoying ONAS!";
const char* loc_dialog_world_3_1 = "Selknam:\nNature is son our side, the cold is our ally. You fool Europeans don't even deserve a piece of ice.";
const char* loc_dialog_world_3_2 = "Selknam:\nGO BACK OR FREEZE TO DEATH!";
const char* loc_dialog_world_4_0 = "Enemy:\nHahaha! Finally, they are trapped, you have\nnowhere to go.";
const char* loc_dialog_world_4_1 = "Enemy:\nBRING THE SPICES! We will be seasoning some Indians.";
const char* loc_dialog_world_4_2 = "Enemy:\nPrepare your stomachs for a victory feast! These fools are cooked...Hahahahaha!";
const char* loc_dialog_world_4_3 = "Selknam:\nWe are the Children of these Lands of Fire and Ice!";
const char* loc_dialog_world_4_4 = "Selknam:\nWE ARE THE SPIRIT OF TIERRA DEL FUEGO!";
const char* loc_dialog_world_4_5 = "And this is how the Selknam were able to defeat the Settler forces, but only few endured bravely the destruction waves of the old continent. \nNow they can live quietly and continue with... WHAT'S HAPPENING!!!";
const char* loc_dialog_world_end_0 = "HAHAHA!, useless indians! You can't win against progress!";
const char* loc_dialog_world_end_1 = "You will be conquered! Do not underestimate the power of the dark side!!! \n(I think that phrase should be immortalized or something)...";
const char* loc_dialog_world_end_2 = "Fi fai fou fuuu!!!! I smash ugly natives!";
const char* loc_dialog_world_end_3 = "Nooooo!!!! My Titan, you've destroyed it! Retreat... we've been... defeated....";
const char* loc_dialog_world_end_4 = "YAAAYY!";
const char* loc_dialog_world_end_5 = "And this is how the Selknam people obtained victory and outlived these horrible battles being able to take the environmental conditions to their advantage and fight for their land.";
const char* loc_dialog_world_end_6 = "Nowadays we may only know about them in History books, currently there are no Selknam among us. But what we do know is that we will always have their legend and brave style of life in our World History.";
const char* loc_dialog_world_altend_0 = "Even though the tribes fought bravely from the waves of Europeans, the Selknam were outnumbered and fell to the enemy. The peaceful people were enslaved and moved away from their homeland...";
const char* loc_dialog_world_altend_1 = "...where they died from starvation and sickness. We will never forget the great battles of the Selknam who gave their lives to defend the people from Tierra del Fuego!";

//social
const char* loc_socialMenu = "Try the awesome #SelknamDefense, a unique Tower Defense/RTS iOS game from Bad Games! Download it at http://bit.ly/MFea7x";
const char* loc_socialSurvival_part0 = "I got ";
const char* loc_socialSurvival_part1 = " in Survival Mode in #SelknamDefense. Join the resistance downloading it from http://bit.ly/MFea7x";
const char* loc_socialStage_part0 = "Try to beat my score in #SelknamDefense, I got ";
const char* loc_socialStage_part1 = " points in stage ";
const char* loc_socialStage_part2 = "!. You don't have it? Download it at http://bit.ly/MFea7x";

void LocalizationSystem::RetrieveLocalization(GameLang selknamLanguage)
{
	s3eResult bResult = S3E_RESULT_ERROR;
	
	simpleIniApi.SetUnicode();
	simpleIniApi.SetMultiLine(true);
	
	if(selknamLanguage == GAMELANG_INT)
		simpleIniApi.LoadFile("localization/sd_loc.int");
	else if(selknamLanguage == GAMELANG_ESN)
		simpleIniApi.LoadFile("localization/sd_loc.esn");

	loc_splash_loading = simpleIniApi.GetValue("UI", "SplashLoading", "_no_string_");
	loc_splash_version = simpleIniApi.GetValue("UI", "SplashVersion", "_no_string_");
	loc_splash_fullVersion = simpleIniApi.GetValue("UI", "SplashFullVersion", "_no_string_");
	loc_splash_freeVersion = simpleIniApi.GetValue("UI", "SplashFreeVersion", "_no_string_");
	loc_ui_back = simpleIniApi.GetValue("UI", "UIBack", "_no_string_");
	loc_ui_close = simpleIniApi.GetValue("UI", "UIClose", "_no_string_");
	loc_ui_ok = simpleIniApi.GetValue("UI", "UIOk", "_no_string_");
	loc_ui_accept = simpleIniApi.GetValue("UI", "UIAccept", "_no_string_");
	loc_ui_cancel = simpleIniApi.GetValue("UI", "UICancel", "_no_string_");
	loc_ui_next = simpleIniApi.GetValue("UI", "UINext", "_no_string_");
	loc_ui_skip = simpleIniApi.GetValue("UI", "UISkip", "_no_string_");
	loc_ui_finish = simpleIniApi.GetValue("UI", "UIFinish", "_no_string_");
	loc_ui_playGame = simpleIniApi.GetValue("UI", "UIPlayGame", "_no_string_");
	loc_ui_removeAds = simpleIniApi.GetValue("UI", "UIRemoveAds", "_no_string_");
	loc_ui_newCampaign = simpleIniApi.GetValue("UI", "UINewCampaign", "_no_string_");
	loc_ui_playCampaign = simpleIniApi.GetValue("UI", "UIPlayCampaign", "_no_string_");
	loc_ui_almanac = simpleIniApi.GetValue("UI", "UIAlmanac", "_no_string_");
	loc_ui_options = simpleIniApi.GetValue("UI", "UIOptions", "_no_string_");
	loc_ui_restore = simpleIniApi.GetValue("UI", "UIRestore", "_no_string_");
	loc_ui_settings = simpleIniApi.GetValue("UI", "UISettings", "_no_string_");
	loc_ui_credits = simpleIniApi.GetValue("UI", "UICredits", "_no_string_");
	loc_ui_survival = simpleIniApi.GetValue("UI", "UISurvival", "_no_string_");
	loc_ui_quitGame = simpleIniApi.GetValue("UI", "UIQuitGame", "_no_string_");
	loc_ui_exitGame = simpleIniApi.GetValue("UI", "UIExitGame", "_no_string_");
	loc_ui_pause = simpleIniApi.GetValue("UI", "UIPause", "_no_string_");
	loc_ui_resume = simpleIniApi.GetValue("UI", "UIResume", "_no_string_");
	loc_ui_howToPlay = simpleIniApi.GetValue("UI", "UIHowToPlay", "_no_string_");
	loc_ui_retry = simpleIniApi.GetValue("UI", "UIRetry", "_no_string_");
	loc_ui_worldMap = simpleIniApi.GetValue("UI", "UIWorldMap", "_no_string_");
	loc_ui_playAgain = simpleIniApi.GetValue("UI", "UIPlayAgain", "_no_string_");
	loc_ui_continue = simpleIniApi.GetValue("UI", "UIContinue", "_no_string_");
	loc_ui_upgrades = simpleIniApi.GetValue("UI", "UIUpgrades", "_no_string_");
	loc_ui_selknams = simpleIniApi.GetValue("UI", "UISelknams", "_no_string_");
	loc_ui_enemies = simpleIniApi.GetValue("UI", "UIEnemies", "_no_string_");
	loc_ui_time = simpleIniApi.GetValue("UI", "UITime", "_no_string_");
	loc_ui_wave = simpleIniApi.GetValue("UI", "UIWave", "_no_string_");
	loc_ui_congratulations = simpleIniApi.GetValue("UI", "UICongratulations", "_no_string_");
	loc_ui_newRecord = simpleIniApi.GetValue("UI", "UINewRecord", "_no_string_");
	loc_ui_yourRecord = simpleIniApi.GetValue("UI", "UIYourRecord", "_no_string_");
	loc_ui_bestRecord = simpleIniApi.GetValue("UI", "UIBestRecord", "_no_string_");
	loc_ui_noRecord = simpleIniApi.GetValue("UI", "UINoRecord", "_no_string_");
	loc_ui_victory = simpleIniApi.GetValue("UI", "UIVictory", "_no_string_");
	loc_ui_defeat = simpleIniApi.GetValue("UI", "UIDefeat", "_no_string_");
	loc_ui_scoreHeartBonus = simpleIniApi.GetValue("UI", "UIScoreHeartBonus", "_no_string_");
	loc_ui_scoreCoinsBonus = simpleIniApi.GetValue("UI", "UIScoreCoinsBonus", "_no_string_");
	loc_ui_scoreWaveBonus = simpleIniApi.GetValue("UI", "UIScoreWaveBonus", "_no_string_");
	loc_ui_scoreTotal = simpleIniApi.GetValue("UI", "UIScoreTotal", "_no_string_");
	loc_ui_onlyFullVersion = simpleIniApi.GetValue("UI", "UIOnlyFullVersion", "_no_string_");
	loc_ui_leaderboards = simpleIniApi.GetValue("UI", "UILeaderboards", "_no_string_");
	loc_ui_gamecenter = simpleIniApi.GetValue("UI", "UIGameCenter", "_no_string_");
	loc_ui_music = simpleIniApi.GetValue("UI", "UIMusic", "_no_string_");
	loc_ui_fxSound = simpleIniApi.GetValue("UI", "UIFXSound", "_no_string_");
	loc_ui_fullscreen = simpleIniApi.GetValue("UI", "UIFullscreen", "_no_string_");
	loc_ui_muteAll = simpleIniApi.GetValue("UI", "UIMuteAll", "_no_string_");

	loc_popup_savedData = simpleIniApi.GetValue("Popups", "PopupSavedData", "_no_string_");
	loc_popup_quitGame = simpleIniApi.GetValue("Popups", "PopupQuitGame", "_no_string_");
	loc_popup_noFeature = simpleIniApi.GetValue("Popups", "PopupNoFeature", "_no_string_");
	loc_popup_retry = simpleIniApi.GetValue("Popups", "PopupRetry", "_no_string_");
	loc_popup_returnToMenu = simpleIniApi.GetValue("Popups", "PopupReturnToMenu", "_no_string_");
	loc_popup_returnToWorldMap = simpleIniApi.GetValue("Popups", "PopupReturnToWorldMap", "_no_string_");
	loc_popup_upgrades = simpleIniApi.GetValue("Popups", "PopupUpgrades", "_no_string_");
	loc_popup_survival = simpleIniApi.GetValue("Popups", "PopupSurvival", "_no_string_");
	loc_popup_trial = simpleIniApi.GetValue("Popups", "PopupTrial", "_no_string_");
	loc_popup_thanks = simpleIniApi.GetValue("Popups", "PopupThanksSharing", "_no_string_");
	loc_popup_error = simpleIniApi.GetValue("Popups", "PopupErrorGeneric", "_no_string_");
	loc_popup_leaderboarderror = simpleIniApi.GetValue("Popups", "PopupErrorLeaderboard", "_no_string_");
	loc_popup_PopupInternetConnection = simpleIniApi.GetValue("Popups", "PopupInternetConnection", "_no_string_");
	
	loc_upgrade_selknamUpgrade = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSelknamUpgrade", "_no_string_");
	loc_portrait_archerBase = simpleIniApi.GetValue("SelknamUpgrade", "PortraitArcherBase", "_no_string_");
	loc_portrait_archerSlower = simpleIniApi.GetValue("SelknamUpgrade", "PortraitArcherSlower", "_no_string_");
	loc_portrait_archerPoisoner = simpleIniApi.GetValue("SelknamUpgrade", "PortraitArcherPoisoner", "_no_string_");
	loc_portrait_bowlerStunner = simpleIniApi.GetValue("SelknamUpgrade", "PortraitBowlerStunner", "_no_string_");
	loc_portrait_bowlerConfuser = simpleIniApi.GetValue("SelknamUpgrade", "PortraitBowlerConfuser", "_no_string_");
	loc_portrait_bowlerRooter = simpleIniApi.GetValue("SelknamUpgrade", "PortraitBowlerRooter", "_no_string_");
	loc_portrait_warriorBase = simpleIniApi.GetValue("SelknamUpgrade", "PortraitWarriorBase", "_no_string_");
	loc_portrait_warriorBerserk = simpleIniApi.GetValue("SelknamUpgrade", "PortraitWarriorBerserk", "_no_string_");
	loc_portrait_warriorTank = simpleIniApi.GetValue("SelknamUpgrade", "PortraitWarriorTank", "_no_string_");
	loc_portrait_shamanHealer = simpleIniApi.GetValue("SelknamUpgrade", "PortraitShamanHealer", "_no_string_");
	loc_portrait_shamanSpeeder = simpleIniApi.GetValue("SelknamUpgrade", "PortraitShamanSpeeder", "_no_string_");
	loc_portrait_shamanMighter = simpleIniApi.GetValue("SelknamUpgrade", "PortraitShamanMighter", "_no_string_");
	loc_upgrade_attributes = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeAttributes", "_no_string_");
	loc_upgrade_health = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeHealth", "_no_string_");
	loc_upgrade_damage = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeDamage", "_no_string_");
	loc_upgrade_speed = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSpeed", "_no_string_");
	loc_upgrade_attackSpeed = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeAttackSpeed", "_no_string_");
	loc_upgrade_castSpeed = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeCastSpeed", "_no_string_");
	loc_upgrade_range = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeRange", "_no_string_");
	loc_upgrade_cost = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeCost", "_no_string_");
	loc_upgrade_skill = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkill", "_no_string_");
	loc_upgrade_skillSlow = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillSlow", "_no_string_");
	loc_upgrade_skillPoison = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillPoison", "_no_string_");
	loc_upgrade_skillStun = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillStun", "_no_string_");
	loc_upgrade_skillConfusion = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillConfusion", "_no_string_");
	loc_upgrade_skillRoot = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillRoot", "_no_string_");
	loc_upgrade_skillHeal = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillHeal", "_no_string_");
	loc_upgrade_skillHaste = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillHaste", "_no_string_");
	loc_upgrade_skillMight = simpleIniApi.GetValue("SelknamUpgrade", "UpgradeSkillMight", "_no_string_");

	loc_leaderboards_title = simpleIniApi.GetValue("Leaderboards", "LeaderboardsTitle", "_no_string_");
	loc_leaderboards_rank = simpleIniApi.GetValue("Leaderboards", "LeaderboardsRank", "_no_string_");
	loc_leaderboards_playername = simpleIniApi.GetValue("Leaderboards", "LeaderboardsPlayername", "_no_string_");
	loc_leaderboards_score = simpleIniApi.GetValue("Leaderboards", "LeaderboardsScore", "_no_string_");
	loc_leaderboards_time = simpleIniApi.GetValue("Leaderboards", "LeaderboardsTime", "_no_string_");
	loc_leaderboards_prevpage = simpleIniApi.GetValue("Leaderboards", "LeaderboardsPrevPage", "_no_string_");
	loc_leaderboards_nextpage = simpleIniApi.GetValue("Leaderboards", "LeaderboardsNextPage", "_no_string_");
	loc_leaderboards_loading = simpleIniApi.GetValue("Leaderboards", "LeaderboardsLoading", "_no_string_");
	loc_leaderboards_overall = simpleIniApi.GetValue("Leaderboards", "LeaderboardsOverall", "_no_string_");
	loc_leaderboards_friends = simpleIniApi.GetValue("Leaderboards", "LeaderboardsFriends", "_no_string_");
	loc_leaderboards_myrank = simpleIniApi.GetValue("Leaderboards", "LeaderboardsMyRank", "_no_string_");
	loc_leaderboards_stage   = simpleIniApi.GetValue("Leaderboards", "LeaderboardsStage", "_no_string_");
	loc_leaderboards_survival = simpleIniApi.GetValue("Leaderboards", "LeaderboardsSurvival", "_no_string_");
	loc_leaderboards_campaign = simpleIniApi.GetValue("Leaderboards", "LeaderboardsCampaign", "_no_string_");
	loc_leaderboards_notAvailable = simpleIniApi.GetValue("Leaderboards", "LeaderboardsNotAvailable", "_no_string_");

	loc_almanac_baseArcherTitle                = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBaseArcherTitle", "_no_string_");
	loc_almanac_baseArcherInformation          = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBaseArcherInformation", "_no_string_");
	loc_almanac_poisonArcherTitle              = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacPoisonArcherTitle", "_no_string_");
	loc_almanac_poisonArcherInformation        = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacPoisonArcherInformation", "_no_string_");
	loc_almanac_unitDelayerTitle               = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacUnitDelayerTitle", "_no_string_");
	loc_almanac_unitDelayerInformation         = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacUnitDelayerInformation", "_no_string_");
	loc_almanac_bolasThrowerBaseTitle          = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBolasThrowerBaseTitle", "_no_string_");
	loc_almanac_bolasThrowerBaseInformation    = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBolasThrowerBaseInformation", "_no_string_");
	loc_almanac_bolasThrowerFloorTitle         = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBolasThrowerFloorTitle", "_no_string_");
	loc_almanac_bolasThrowerFloorInformation   = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBolasThrowerFloorInformation", "_no_string_");
	loc_almanac_bolasThrowerConfuseTitle       = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBolasThrowerConfuseTitle", "_no_string_");
	loc_almanac_bolasThrowerConfuseInformation = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBolasThrowerConfuseInformation", "_no_string_");
	loc_almanac_baseWarriorTitle               = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBaseWarriorTitle", "_no_string_");
	loc_almanac_baseWarriorInformation         = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBaseWarriorInformation", "_no_string_");
	loc_almanac_resistantWarriorTitle          = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacResistantWarriorTitle", "_no_string_");
	loc_almanac_resistantWarriorInformation    = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacResistantWarriorInformation", "_no_string_");
	loc_almanac_aggressiveWarriorTitle         = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacAggressiveWarriorTitle", "_no_string_");
	loc_almanac_aggressiveWarriorInformation   = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacAggressiveWarriorInformation", "_no_string_");
	loc_almanac_baseMageTitle                  = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBaseMageTitle", "_no_string_");
	loc_almanac_baseMageInformation            = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacBaseMageInformation", "_no_string_");
	loc_almanac_speederMageTitle               = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacSpeederMageTitle", "_no_string_");
	loc_almanac_speederMageInformation         = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacSpeederMageInformation", "_no_string_");
	loc_almanac_mightierMageTitle              = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacMighterMageTitle", "_no_string_");
	loc_almanac_mightierMageInformation        = simpleIniApi.GetValue("SelknamAlmanac", "AlmanacMighterMageInformation", "_no_string_");
	
	loc_almanac_soldierTitle		= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacSoldierTitle", "_no_string_");
	loc_almanac_soldierInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacSoldierInfo", "_no_string_");
	loc_almanac_baronTitle			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacBaronTitle", "_no_string_");
	loc_almanac_baronInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacBaronInfo", "_no_string_");
	loc_almanac_convictTitle		= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacConvictTitle", "_no_string_");
	loc_almanac_convictInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacConvictInfo", "_no_string_");
	loc_almanac_chemistTitle		= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacChemistTitle", "_no_string_");
	loc_almanac_chemistInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacChemistInfo", "_no_string_");
	loc_almanac_tankTitle			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacTankTitle", "_no_string_");
	loc_almanac_tankInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacTankInfo", "_no_string_");
	loc_almanac_rabbitTitle			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacRabbitTitle", "_no_string_");
	loc_almanac_rabbitInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacRabbitInfo", "_no_string_");
	loc_almanac_pirateTitle			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacPirateTitle", "_no_string_");
	loc_almanac_pirateInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacPirateInfo", "_no_string_");
	loc_almanac_captainTitle		= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacCaptainTitle", "_no_string_");
	loc_almanac_captainInfo			= simpleIniApi.GetValue("EnemiesAlmanac", "AlmanacCaptainInfo", "_no_string_");

	loc_credits_title0 = simpleIniApi.GetValue("Credits", "CreditsTitle00", "_no_string_");
	loc_credits_title1 = simpleIniApi.GetValue("Credits", "CreditsTitle01", "_no_string_");
	loc_credits_title2 = simpleIniApi.GetValue("Credits", "CreditsTitle02", "_no_string_");
	loc_credits_title3 = simpleIniApi.GetValue("Credits", "CreditsTitle03", "_no_string_");
	loc_credits_title4 = simpleIniApi.GetValue("Credits", "CreditsTitle04", "_no_string_");
	loc_credits_title5 = simpleIniApi.GetValue("Credits", "CreditsTitle05", "_no_string_");
	loc_credits_name0 = simpleIniApi.GetValue("Credits", "CreditsName00", "_no_string_");
	loc_credits_name1 = simpleIniApi.GetValue("Credits", "CreditsName01", "_no_string_");
	loc_credits_name2 = simpleIniApi.GetValue("Credits", "CreditsName02", "_no_string_");
	loc_credits_name3 = simpleIniApi.GetValue("Credits", "CreditsName03", "_no_string_");
	loc_credits_name4 = simpleIniApi.GetValue("Credits", "CreditsName04", "_no_string_");
	loc_credits_name5 = simpleIniApi.GetValue("Credits", "CreditsName05", "_no_string_");
	loc_credits_name6 = simpleIniApi.GetValue("Credits", "CreditsName06", "_no_string_");
	loc_credits_name7 = simpleIniApi.GetValue("Credits", "CreditsName07", "_no_string_");
	loc_credits_contact = simpleIniApi.GetValue("Credits", "CreditsContact", "_no_string_");
	loc_credits_marmalade = simpleIniApi.GetValue("Credits", "CreditsMarmalade", "_no_string_");

	loc_tutorial_part0 = simpleIniApi.GetValue("Tutorial", "TutorialPart00", "_no_string_");
	loc_tutorial_part1 = simpleIniApi.GetValue("Tutorial", "TutorialPart01", "_no_string_");
	loc_tutorial_part2 = simpleIniApi.GetValue("Tutorial", "TutorialPart02", "_no_string_");
	loc_tutorial_part3 = simpleIniApi.GetValue("Tutorial", "TutorialPart03", "_no_string_");
	loc_tutorial_part4 = simpleIniApi.GetValue("Tutorial", "TutorialPart04", "_no_string_");
	loc_tutorial_part5 = simpleIniApi.GetValue("Tutorial", "TutorialPart05", "_no_string_");
	loc_tutorial_part6 = simpleIniApi.GetValue("Tutorial", "TutorialPart06", "_no_string_");
	loc_tutorial_part7 = simpleIniApi.GetValue("Tutorial", "TutorialPart07", "_no_string_");
	loc_tutorial_part8 = simpleIniApi.GetValue("Tutorial", "TutorialPart08", "_no_string_");
	loc_tutorial_part9 = simpleIniApi.GetValue("Tutorial", "TutorialPart09", "_no_string_");
	loc_tutorial_part10 = simpleIniApi.GetValue("Tutorial", "TutorialPart10", "_no_string_");
	loc_tutorial_part11 = simpleIniApi.GetValue("Tutorial", "TutorialPart11", "_no_string_");
	loc_tutorial_part12 = simpleIniApi.GetValue("Tutorial", "TutorialPart12", "_no_string_");
	loc_tutorial_part13 = simpleIniApi.GetValue("Tutorial", "TutorialPart13", "_no_string_");
	loc_tutorial_part14 = simpleIniApi.GetValue("Tutorial", "TutorialPart14", "_no_string_");
	loc_tutorial_part15 = simpleIniApi.GetValue("Tutorial", "TutorialPart15", "_no_string_");
	loc_tutorial_part16 = simpleIniApi.GetValue("Tutorial", "TutorialPart16", "_no_string_");
	loc_tutorial_part17 = simpleIniApi.GetValue("Tutorial", "TutorialPart17", "_no_string_");
	loc_tutorial_part18 = simpleIniApi.GetValue("Tutorial", "TutorialPart18", "_no_string_");

	loc_dialog_tapToContinue = simpleIniApi.GetValue("Dialogs", "DialogTapToContinue", "_no_string_");
	loc_dialog_tapToContinue_alt = simpleIniApi.GetValue("Dialogs", "DialogClickToContinue", "_no_string_");
	loc_dialog_enemy = simpleIniApi.GetValue("Dialogs", "DialogEnemy", "_no_string_");
	loc_dialog_selknam = simpleIniApi.GetValue("Dialogs", "DialogSelknam", "_no_string_");
	loc_dialog_world_1_0 = simpleIniApi.GetValue("Dialogs", "DialogWorld_1_0", "_no_string_");
	loc_dialog_world_1_1 = simpleIniApi.GetValue("Dialogs", "DialogWorld_1_1", "_no_string_");
	loc_dialog_world_1_2 = simpleIniApi.GetValue("Dialogs", "DialogWorld_1_2", "_no_string_");
	loc_dialog_world_1_3 = simpleIniApi.GetValue("Dialogs", "DialogWorld_1_3", "_no_string_");
	loc_dialog_world_1_4 = simpleIniApi.GetValue("Dialogs", "DialogWorld_1_4", "_no_string_");
	loc_dialog_world_2_0 = simpleIniApi.GetValue("Dialogs", "DialogWorld_2_0", "_no_string_");
	loc_dialog_world_2_1 = simpleIniApi.GetValue("Dialogs", "DialogWorld_2_1", "_no_string_");
	loc_dialog_world_2_2 = simpleIniApi.GetValue("Dialogs", "DialogWorld_2_2", "_no_string_");
	loc_dialog_world_2_3 = simpleIniApi.GetValue("Dialogs", "DialogWorld_2_3", "_no_string_");
	loc_dialog_world_3_0 = simpleIniApi.GetValue("Dialogs", "DialogWorld_3_0", "_no_string_");
	loc_dialog_world_3_1 = simpleIniApi.GetValue("Dialogs", "DialogWorld_3_1", "_no_string_");
	loc_dialog_world_3_2 = simpleIniApi.GetValue("Dialogs", "DialogWorld_3_2", "_no_string_");
	loc_dialog_world_4_0 = simpleIniApi.GetValue("Dialogs", "DialogWorld_4_0", "_no_string_");
	loc_dialog_world_4_1 = simpleIniApi.GetValue("Dialogs", "DialogWorld_4_1", "_no_string_");
	loc_dialog_world_4_2 = simpleIniApi.GetValue("Dialogs", "DialogWorld_4_2", "_no_string_");
	loc_dialog_world_4_3 = simpleIniApi.GetValue("Dialogs", "DialogWorld_4_3", "_no_string_");
	loc_dialog_world_4_4 = simpleIniApi.GetValue("Dialogs", "DialogWorld_4_4", "_no_string_");
	loc_dialog_world_4_5 = simpleIniApi.GetValue("Dialogs", "DialogWorld_4_5", "_no_string_");
	loc_dialog_world_end_0 = simpleIniApi.GetValue("Dialogs", "DialogWorldEnd_0", "_no_string_");
	loc_dialog_world_end_1 = simpleIniApi.GetValue("Dialogs", "DialogWorldEnd_1", "_no_string_");
	loc_dialog_world_end_2 = simpleIniApi.GetValue("Dialogs", "DialogWorldEnd_2", "_no_string_");
	loc_dialog_world_end_3 = simpleIniApi.GetValue("Dialogs", "DialogWorldEnd_3", "_no_string_");
	loc_dialog_world_end_4 = simpleIniApi.GetValue("Dialogs", "DialogWorldEnd_4", "_no_string_");
	loc_dialog_world_end_5 = simpleIniApi.GetValue("Dialogs", "DialogWorldEnd_5", "_no_string_");
	loc_dialog_world_end_6 = simpleIniApi.GetValue("Dialogs", "DialogWorldEnd_6", "_no_string_");
	loc_dialog_world_altend_0 = simpleIniApi.GetValue("Dialogs", "DialogWorldAltEnd_0", "_no_string_");
	loc_dialog_world_altend_1 = simpleIniApi.GetValue("Dialogs", "DialogWorldAltEnd_1", "_no_string_");

	loc_socialMenu = simpleIniApi.GetValue("Social", "SocialMenu", "_no_string_");
	loc_socialSurvival_part0 = simpleIniApi.GetValue("Social", "SocialSurvivalPart0", "_no_string_");
	loc_socialSurvival_part1 = simpleIniApi.GetValue("Social", "SocialSurvivalPart1", "_no_string_");
	loc_socialStage_part0 = simpleIniApi.GetValue("Social", "SocialStagePart0", "_no_string_");
	loc_socialStage_part1 = simpleIniApi.GetValue("Social", "SocialStagePart1", "_no_string_");
	loc_socialStage_part2 = simpleIniApi.GetValue("Social", "SocialStagePart2", "_no_string_");

	loc_description_skillSlow = simpleIniApi.GetValue("Skills", "Slow", "_no_string_");
	loc_description_skillPoison = simpleIniApi.GetValue("Skills", "Poison", "_no_string_");
	loc_description_skillStun = simpleIniApi.GetValue("Skills", "Stun", "_no_string_");
	loc_description_skillConfusion = simpleIniApi.GetValue("Skills", "Confusion", "_no_string_");
	loc_description_skillRoot = simpleIniApi.GetValue("Skills", "Root", "_no_string_");
	loc_description_skillHeal = simpleIniApi.GetValue("Skills", "Heal", "_no_string_");
	loc_description_skillHaste = simpleIniApi.GetValue("Skills", "Haste", "_no_string_");
	loc_description_skillMight = simpleIniApi.GetValue("Skills", "Might", "_no_string_");
	
	return;
}