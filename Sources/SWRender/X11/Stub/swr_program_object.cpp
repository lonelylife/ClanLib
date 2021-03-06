/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include "SWRender/precomp.h"
#include "API/SWRender/swr_graphic_context.h"
#include "API/SWRender/swr_program_object.h"

// NON-SSE2 stub

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject_SWRender Construction:

CL_ProgramObject_SWRender::CL_ProgramObject_SWRender(CL_SoftwareProgram *program, bool is_sprite_program)
{
}

CL_ProgramObject_SWRender::CL_ProgramObject_SWRender(const CL_ProgramObject &program_object) : CL_ProgramObject(program_object)
{
}

CL_ProgramObject_SWRender::~CL_ProgramObject_SWRender()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject_SWRender Attributes:

CL_SoftwareProgram *CL_ProgramObject_SWRender::get_program() const
{
	return NULL;
}

bool CL_ProgramObject_SWRender::is_sprite_program() const
{
	return false;
}

CL_SWRenderProgramObjectProvider *CL_ProgramObject_SWRender::get_provider() const
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject_SWRender Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject_SWRender Implementation:

