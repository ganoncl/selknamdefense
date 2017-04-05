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

#include "isoprojectile.h"


IsoProjectile::IsoProjectile()
{	
	CleanIso();

	impact1RenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fx_impact1"));
	impact2RenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fx_impact2"));

	impact_bullet_tex = TextureHelper::GetCIwTexture(impact1RenderData.atlasName);
	impact_rock_tex   = TextureHelper::GetCIwTexture(impact2RenderData.atlasName);

	arrowRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fx_proyectile_arrow"));
	rockRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fx_proyectile_stone"));
	
	impact_time.SetTimeLimit(200);

	this->target_x = 0;
	this->target_y = 0;
	this->target_z = 0;
	xi = yi = 0;

	correction_x = correction_y = 0;
}

IsoProjectile::~IsoProjectile()
{
	impact_bullet_tex = NULL;
	impact_rock_tex   = NULL;
	impact_tex        = NULL;
}

void IsoProjectile::InitInflictor(IsoPawn* target, PawnAttributes pawn_att, int damage, Skill_Type skill)
{
	//Null? For some reason the game crush if this line is commented. But I don't know why.
	if(target == NULL || !target->getInitialized())
	{
		bFailedToInit = true;
		return;
	}
	//Inital position of the projectile, needs to readjust with an offset
	xi = target->getPosX();
	yi = target->getPosY();
	setPosX(xi);
	setPosY(yi);
	setPosZ(target->getPosZ());

	//Store inflictor data to call TakeDamage later.
	this->inflictor = target;
	this->inflictor_att = pawn_att;
	this->damage = damage;
	this->skill = skill;
}

void IsoProjectile::InitTarget(IsoPawn* target)
{
	//Sanity Check
	if(target == NULL || !target->getInitialized())
	{
		bFailedToInit = true;
		return;
	}
	this->target = target;
	this->target_x = target->getPosX();
	this->target_y = target->getPosY();
	this->target_z = target->getPosZ();
}

void IsoProjectile::InitProjectile(IsoProjectileType type)
{
	this->type = type;
	InitIso();
}

void IsoProjectile::InitIso()
{
	if(type == Projectile_Arrow)
	{
		impact_tex = NULL;
		setWidth(26);
		setHeight(5);
		angle = GetInitialAngle();
		InitCastelJauAlgorithm(Mode_Parabolic);

	}
	else if(type == Projectile_Bullet)
	{
		impact_tex = impact_bullet_tex;
		impactRenderData = impact1RenderData;
		InitCastelJauAlgorithm(Mode_Linear);
	}
	else if(type == Projectile_Rock)
	{
		impact_tex = impact_rock_tex;
		impactRenderData = impact2RenderData;
		setWidth(12);
		setHeight(12);
		angle = 0;
		InitCastelJauAlgorithm(Mode_Parabolic);
	}

	setUvMinX(0.0f);
	setUvMaxX(1.0f);
	setUvMinY(0.0f);
	setUvMaxY(1.0f);
	bImpact = false;
	impact_time.Reset();

	if(bFailedToInit)
		CleanIso();
}

void IsoProjectile::Update()
{
	if(bFailedToInit)
		return;

	impact_time.Update();

	if(impact_time.GetIsEnd())
	{
		CleanIso();
		return;
	}

	if(bImpact) return;

	if(t <= 1 && !NotTooClose())
	{
		CIwFVec2 tmp = getCastelJausPoint(length_points - 1, 0, t);
		CIwFVec2 next = getCastelJausPoint(length_points - 1, 0, t + speed);
		setPosX(tmp.x);
		setPosY(tmp.y);

		t += speed;

		IsoDirection dir = inflictor->GetOrientation();
		if(dir == DIR_NorthEast)
		{
			angle = atan2(next.y - getPosY(), next.x - getPosX()) + PI;
		}
		else if(dir == DIR_SouthWest || dir == DIR_West || dir == DIR_East)
		{
			angle = atan2(getPosY() - next.y, getPosX() - next.x);
		}
		else
		{
			angle = atan2(next.y - getPosY(), next.x - getPosX()) + PI;
		}
	}
	else
	{
		if(target != NULL)
		{
			if(target->getInitialized() && !target->GetIsDead())
			{
				target->TakeDamage(damage, inflictor, inflictor_att, skill);
			}
		}
		bImpact = true;
		impact_time.Reset();
		impact_time.Start();

		impact_position.x = int((rand() % 32) - 16 + target->getPosX()); 
		impact_position.y = int(-28 + target->getPosY()); 
	}
}

void IsoProjectile::CleanIso()
{
	type = Projectile_None;
	bFailedToInit = false;
}

void IsoProjectile::RenderQuad(CIwFVec3* quad, CIwFVec2* uv_quad, int* num)
{	
	int index = (*num) * 4;

	quad[index + 0] = rotate_coord(getPosX(), getPosY(), angle);
	quad[index + 1] = rotate_coord(getPosX(), getPosY() + getHeight(), angle);
	quad[index + 2] = rotate_coord(getPosX() + getWidth(), getPosY() + getHeight(), angle);
	quad[index + 3] = rotate_coord(getPosX() + getWidth(), getPosY(), angle);

	float uvMinX = 0;
	float uvMinY = 0;
	float uvMaxX = 1;
	float uvMaxY = 1;

	if(type == Projectile_Arrow)
	{
		uvMinX = arrowRenderData.uvInfo.minX;
		uvMinY = arrowRenderData.uvInfo.minY;
		uvMaxX = arrowRenderData.uvInfo.maxX;
		uvMinX = arrowRenderData.uvInfo.minX;
	}
	else if(type == Projectile_Rock)
	{
		uvMinX = rockRenderData.uvInfo.minX;
		uvMinY = rockRenderData.uvInfo.minY;
		uvMaxX = rockRenderData.uvInfo.maxX;
		uvMaxY = rockRenderData.uvInfo.maxY;
	}

	uv_quad[index + 0] = CIwFVec2(uvMinX, uvMinY);
	uv_quad[index + 1] = CIwFVec2(uvMinX, uvMaxY);
	uv_quad[index + 2] = CIwFVec2(uvMaxX, uvMaxY);
	uv_quad[index + 3] = CIwFVec2(uvMaxX, uvMinY);

	(*num)++;
}

void IsoProjectile::Render()
{
	if(bImpact)
	{
		if(impact_tex != NULL) {
			Utils::InitQuadDrawing(impact_tex);
			Utils::DrawQuadBatch((float)impactRenderData.width, (float)impactRenderData.height, 
				impact_position.x, impact_position.y, 1.0f,
				impactRenderData.uvInfo.minX, impactRenderData.uvInfo.maxX,
				impactRenderData.uvInfo.minY, impactRenderData.uvInfo.maxY, 
				0.0f, 0.0f);
			Utils::RenderQuadBatch();
		}
	}
}



CIwFVec3 IsoProjectile::rotate_coord(float x, float y, double angle)
{
	CIwFVec3 ret;
	
	float center_x = getPosX() + getWidth() * 0.5f;
	float center_y = getPosY() + getHeight() * 0.5f;

	x -= center_x;
	y -= center_y;

	ret.x = (x * (float)cos(angle) - y * (float)sin(angle));
	ret.y = (x * (float)sin(angle) + y * (float)cos(angle));

	ret.x += center_x;
	ret.y += center_y;

	ret.z = getPosZ();

	return ret;
}

double IsoProjectile::toRand(double angle)
{
	return (angle * PI / 180.0f);  
}

CIwFVec2 IsoProjectile::getCastelJausPoint(int r, int i, float t)
{	
	if(r == 0) return points[i];
	CIwFVec2 p1 = getCastelJausPoint(r - 1, i, t);
	CIwFVec2 p2 = getCastelJausPoint(r - 1, i + 1, t);

	return CIwFVec2(((1 - t) * p1.x + t * p2.x), ((1 - t) * p1.y + t * p2.y));	
}

void IsoProjectile::AdjustCurveIfTargetIsMoving()
{
	float offset = 32.0f;
	if(target->bMove)
	{
		IsoDirection dir = target->GetOrientation();
		if(dir == DIR_East)
		{
			target_x += offset;
			target_y += offset;
		}
		else if(dir == DIR_West)
		{
			target_x -= offset;
			target_y -= offset;
		}
		else if(dir == DIR_North)
		{
			target_x += offset;
			target_y -= offset;
		}
		else if(dir == DIR_South)
		{
			target_x -= offset;
			target_y += offset;
		}
	}
}

void IsoProjectile::SetCorrection(float cx, float cy)
{
	correction_x = cx;
	correction_y = cy;
	IwTrace(APPLOG, ("CORRECTION: %f %f", cx, cy));
}

void IsoProjectile::SetTargetCorrection(float cx, float cy)
{
	correction_target_x = cx;
	correction_target_y = cy;
}

void IsoProjectile::InitCastelJauAlgorithm(Projectile_Mode mode)
{
	if(target == NULL || target->GetTile() == NULL)
	{
		bFailedToInit = true;
		return;
	}
	IsoDirection dir = inflictor->GetOrientation();

	bool straight_line = false;
	int target_diff    = abs(target->GetTile()->getXNumTile() - inflictor->GetTile()->getXNumTile());
	int inflictor_diff = abs(target->GetTile()->getYNumTile() - inflictor->GetTile()->getYNumTile());
	if(target_diff == inflictor_diff)
		straight_line = true;

	if((dir == DIR_NorthWest && straight_line) ||
	   (dir == DIR_SouthEast))
	{
		length_points = 2;
		xi -= correction_x;
		yi -= correction_y;
		//Initial point is where the inflictor is, end point is where the target is.
		points[0].x = xi;
		points[0].y = yi;
		points[length_points - 1].x = target_x;
		points[length_points - 1].y = target_y;

		if(dir == DIR_NorthWest)
		{
			setPosZ(getPosZ() + 0.001f);
		}
		else if(dir == DIR_SouthEast)
		{
			setPosZ(target->getPosZ() + 0.001f);
		}
	}
	else
	{	
		float angle_modifier = (float)atan2(target_y - yi, target_x - xi);

		length_points = 4;
		xi -= correction_x;
		yi -= correction_y;
		target_x -= correction_target_x;
		target_y -= correction_target_y;
		points[0].x = xi;
		points[0].y = yi;
		points[length_points - 1].x = target_x;
		points[length_points - 1].y = target_y;

		float length_vector = (float)fabs(EuclideanDistance(xi, yi, target_x, target_y) / 3);
		float y_length = GetYLength(length_vector);

		CIwFVec3 vector_a   = CIwFVec3(target_x - xi, target_y - yi, 0);
		CIwFVec3 aux_vector = CIwFVec3(0, 0, 1);


		if(dir == DIR_West || dir == DIR_SouthWest || dir == DIR_South)
			aux_vector = Utils::VectorMultiplyScalar(aux_vector, -1);

		if(dir == DIR_NorthWest || dir == DIR_West || dir == DIR_South || dir == DIR_North)
		{
			setPosZ(getPosZ() + 0.001f);
		}

		CIwFVec3 intermediate_vector = Utils::CrossProduct(vector_a, aux_vector) / Utils::Norm(vector_a); 
		intermediate_vector = Utils::VectorMultiplyScalar(intermediate_vector, length_vector);
		intermediate_vector = CIwFVec3(intermediate_vector.x + xi, intermediate_vector.y + yi, 0);

		float lx = (target_x - xi) / 3;
		float ly = (target_y - yi) / 3;
		CIwFVec3 p1 = CIwFVec3(intermediate_vector.x + lx, intermediate_vector.y + ly, 0);
		CIwFVec3 p2 = CIwFVec3(intermediate_vector.x + lx * 2, intermediate_vector.y + ly * 2, 0);

		points[1].x = p1.x; points[1].y = p1.y;
		points[2].x = p2.x; points[2].y = p2.y;
	}
	
	speed = (0.085f * 180.0f) / float(fabs(points[0].x - points[length_points - 1].x) + fabs(points[0].y - points[length_points - 1].y));
	//Castel Jau counter for interpolation position.
	t = 0;
}

float IsoProjectile::GetYLength(float _length)
{
	return 0.3f*_length;
}

float IsoProjectile::EuclideanDistance(float x1, float y1, float x2, float y2)
{
	return (float)(sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}

float IsoProjectile::GetInitialAngle()
{
	IsoDirection dir = inflictor->GetOrientation();
	float ret = 0.0f;

	if(dir == DIR_North)          ret = 45.0f;
	else if(dir == DIR_South)     ret = 225.0f;
	else if(dir == DIR_West)      ret = 135.0f;
	else if(dir == DIR_East)      ret = -45.0f;
	else if(dir == DIR_NorthEast) ret = 0.0f;
	else if(dir == DIR_NorthWest) ret = 90.0f;
	else if(dir == DIR_SouthEast) ret = -90.0f;
	else if(dir == DIR_SouthWest) ret = 180.0f;

	return float(toRand(ret));
}

bool IsoProjectile::NotTooClose() 
{
	if(fabs(target->getPosX() - getPosX()) <= 16 && fabs(target->getPosY() - getPosY()) <= 8)
	   return true;
	else
	   return false;
}