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

#ifndef _DRAWUTILS_H_
#define _DRAWUTILS_H_

//image pool
const int	MaxImages=1024;

CIwFVec3	s_Quad[4];
CIwColour	s_Colors[4];
CIwFVec2    s_UVs[4];

//batching method
int numImagesBatched = 0;
CIwFVec3	s_QuadBatch[4 * MaxImages];
CIwColour	s_ColorsBatch[4 * MaxImages];
CIwFVec2    s_UVsBatch[4 * MaxImages];

// Texture object
CIwTexture* s_Texture = NULL;
CIwResGroup* pGroup = NULL;

#endif	// _DRAWUTILS_H_