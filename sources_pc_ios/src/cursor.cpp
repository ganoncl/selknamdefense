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

#include "cursor.h"

Cursor* Cursor_Reference;

void HandleMotion_Cursor(s3ePointerMotionEvent* event)
{
	if(Cursor_Reference != NULL)
	{
		Cursor_Reference->HandleMotion(event);
	}
}

Cursor::Cursor()
{
	Cursor_Reference = this;

	cursorData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("cursor_mouse"));

	_x = 0;
	_y = 0;

	bRender = false;
}

Cursor::~Cursor()
{
}

void Cursor::HandleMotion(s3ePointerMotionEvent* event)
{
	int32 iNewMouseX = event->m_x;
	int32 iNewMouseY = event->m_y;
	
	if(iLastMouseX != iNewMouseX || iLastMouseY != iNewMouseY)
	{
		iLastMouseX = iNewMouseX;
		iLastMouseY = iNewMouseY;

		bRender = true;
		bMouseActive = true;
		_x = (int16)event->m_x;
		_y = (int16)event->m_y;
	}
}

void Cursor::InitCursor()
{
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Cursor, NULL);
}

void Cursor::EndCursor()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Cursor);
}

void Cursor::Render()
{
	//bRender and bMouseActive are the same, but anyway.
	if(bRender && bMouseActive)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(cursorData.atlasName), CIwSVec2(_x, _y), cursorData);
	}
}
