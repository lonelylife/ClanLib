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

#include <memory>

// This is the Application class (That is instantiated by the Program Class)
class Joints
{
public:
	int start(const std::vector<std::string> &args);

private:
	Body create_ground_body(PhysicWorld &phys_world);
	Body create_box_body(PhysicWorld &phys_world);
	std::shared_ptr<Joint> create_joint(PhysicWorld &phys_world, Body &bodyA, Body &bodyB, int type);

	void on_input_up(const InputEvent &key);
	void on_window_close();

private:
	bool quit;
};