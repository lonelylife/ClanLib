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
**    Harry Storbacka
**    Magnus Norddahl
*/

#pragma once


#include "Core/precomp.h"
#include <vector>
#include "API/Core/Math/point.h"

namespace clan
{

class BezierCurve_Impl
{
public:
	BezierCurve_Impl();
	~BezierCurve_Impl();

	std::vector<Pointf> generate_curve_points(const Angle &split_angle);
	std::vector<Pointf> subdivide_bezier(float start_pos, float end_pos)  const;
	Pointf get_point_relative(float) const;

	std::vector<Pointf> control_points;
	mutable std::vector<Pointf> P;

	float split_angle_rad;
};

}
