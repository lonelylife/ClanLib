/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Arkadiusz Kalinowski
*/

#pragma once

#include "../api_physics.h"
#include "../World/physic_world.h"
#include "../../Core/Math/vec2.h"
#include <memory>

/// \addtogroup clanPhysics_Dynamics clanPhysics Dynamics
/// \{
namespace clan
{

class Body;
class FixtureDescription;
class Fixture_Impl;

/// \brief Fixture class
///
/// \xmlonly !group=Physics/Dynamics! !header=fixture.h! \endxmlonly
class CL_API_PHYSICS Fixture
{
public:
	/// \brief Constructs a null instance.
	Fixture();

	/// \brief Constructs a World
	///
	/// \param body = Body
	/// \param description = Fixture Description
	Fixture(Body &body, const FixtureDescription &description);

	~Fixture();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Check if Fixture is a sensor.
	bool is_sensor() const;


/// \}
/// \name Operations
/// \{
public:

	/// \brief Set the fixture as a sensor.
	void set_as_sensor (const bool value = true);

	/// \brief Checks given point for collision with the fixture.
	bool test_point (const Vec2f &p) const;

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<Fixture_Impl> impl;
/// \}

};

}
/// \}