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

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "common.h"
#include "isoterrain.h"
#include "isopawn.h"
#include "utils.h"

//Motion modes
typedef enum {
	Mode_Parabolic, //Go directly to objective
	Mode_Linear,   //Parabolic motion
} Projectile_Mode;

typedef enum
{
	ObjectiveMode_None,
	ObjectiveMode_IsoPawn,
	ObjectiveMode_IsoTerrain
} ObjectiveMode;

class IsoProjectile : public IsoObject
{
public:
	IsoProjectile();
	~IsoProjectile();
	virtual void Update();
	virtual void CleanIso();

	//Add data from the inflictor to the arrow
	void InitInflictor(IsoPawn* target, PawnAttributes pawn_att, int damage, Skill_Type skill);
	void InitInflictor(IsoTerrain* terrain, int range, PawnAttributes pawn_att, int damage, Skill_Type skill);

	//Arro will hit this target
	void InitTarget(IsoPawn* target);
	void InitTarget(IsoTerrain* target);
	//Init texture projectiles
	void InitProjectile(IsoProjectileType type);
	//Reference to the main game class
	void SetGame(CGame* game) { this->game = game; };
	
	//Getters
	IsoProjectileType GetType() { return type; };

	float correction_x;
	float correction_y;
	float correction_target_x;
	float correction_target_y;

	//Draw the projectile on the screen.
	void RenderImpact();

	bool bRotate;

	void RenderQuad(CIwFVec3* quad, CIwFVec2* uv_quad, int* num); 
	void SetCorrection(float cx, float cy);
	void SetTargetCorrection(float cx, float cy);
	void SetAttackRange(int range = 0);
	void RangeAttackAffectsToEnemies(bool value);

private:
	bool bToEnemyDamage;
	virtual void InitIso();

	ObjectiveMode objMode;

	int splashAttackRange;

	CGame* game;
	IsoPawn* target;
	IsoTerrain* terrainTarget;
	IsoPawn* inflictor;
	IsoProjectileType type;

	//target position
	float target_x;
	float target_y;
	float target_z;

	GameTimer impact_time;
	bool bImpact;

	//textures
	CIwTexture* impact_tex;
	CIwTexture* impact_bullet_tex;
	CIwTexture* impact_rock_tex;

	CIwSVec2 impact_position;

	bool NotTooClose();

	//projectile rotation
	CIwFVec3 rotate_coord(float x, float y, double angle);
	double toRand(double angle);
	double angle;

	float xi;
	float yi;
	float init_angle;

	//Inflictor data
	PawnAttributes inflictor_att;
	int damage;
	Skill_Type skill;

	//Variable used by jau's algorithm, it's the time instant t e [0,1], 0:Init - 1:End
	float t; 
	float speed;
	bool bFailedToInit;
	//Parabolic motion
	//Castel Jau's Algorithm (To drawing the curve)
	CIwFVec2 points[100]; //Pre-defined points in order to interpolate a curve.
	int length_points; //Numbers of points used in the algorithm.
	CIwFVec2 getCastelJausPoint(int r, int i, float t); //Castel Jau's Recursive implementation.
	void InitCastelJauAlgorithm(Projectile_Mode mode); //Init points array.
	//If target is a mobile target, then the endpoint of the curve is the next tile where
	//that target is going to be.
	void AdjustCurveIfTargetIsMoving();

	//Getters
	float GetInitialAngle();
	
	float EuclideanDistance(float x1, float y1, float x2, float y2);
	float GetYLength(float _length);

	RenderData impact1RenderData;
	RenderData impact2RenderData;
	RenderData impactRenderData;

	RenderData arrowRenderData;
	RenderData rockRenderData;
	RenderData cannonRenderData;
};

#endif