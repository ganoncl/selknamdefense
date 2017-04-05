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

#include "checkbox.h"
#include "texturehelper.h"
#include "utils.h"

Checkbox::Checkbox()
{
	x = y = 0;
	boxRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("settings_checkbox"));
	w = TextureHelper::GetDimension(boxRD).x;
	h = TextureHelper::GetDimension(boxRD).y;
	okRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("settings_check_ok"));
	bIsChecked = true;
	bIsFocused = false;
	boxhRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("settings_checkbox_h"));
}

Checkbox::~Checkbox()
{

}

void Checkbox::Render()
{
	if(bIsFocused)
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(boxhRD.atlasName), CIwSVec2(x, y), boxhRD);
	else
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(boxRD.atlasName), CIwSVec2(x, y), boxRD);

	if(bIsChecked)
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(okRD.atlasName), CIwSVec2(x + 10, y + 7), okRD);
}

void Checkbox::SetPosition(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Checkbox::UpdateClickEvents(s3ePointerEvent* event)
{
	if(event->m_Pressed && Utils::MouseOn(event->m_x, event->m_y, x, y, w, h))
		bIsChecked = !bIsChecked;
}

bool Checkbox::GetIsChecked()
{
	return bIsChecked;
}

void Checkbox::SetIsChecked(bool b)
{
	bIsChecked = b;
}

void Checkbox::EnableFocus()
{
	bIsFocused = true;
}

void Checkbox::UnableFocus()
{
	bIsFocused = false;
}

void Checkbox::ExecuteFunction()
{
	bIsChecked = !bIsChecked;
}

bool Checkbox::HandleFocusMotion(s3ePointerMotionEvent* event)
{
	return Utils::MouseOn(event->m_x, event->m_y, x, y, w, h);
}