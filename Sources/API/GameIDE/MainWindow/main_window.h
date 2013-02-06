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
**    Magnus Norddahl
*/

/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

#pragma once

#include "../SolutionModel/solution_model.h"
#include "../FileItemType/file_item_type_factory.h"
#include "../TextEditor/text_editor_file_item_type.h"
#include "../UIController/ui_controller.h"
#include "../BuildSystem/build_system.h"
namespace clan
{
class clan::Ribbon;
class Workspace;
class SolutionExplorer;
class PropertyManager;

class EditorMainWindow : public clan::GUIComponent
{
public:
	EditorMainWindow(clan::GUIManager *manager, const FileItemTypeFactory &file_item_type_factory);
	SolutionModel &get_model() { return model; }

private:
	void on_resized();
	bool on_close();
	void on_item_new();
	void on_item_open();
	void on_item_save();
	void on_item_save_as();
	void on_item_about();
	void on_item_exit();

	static clan::GUITopLevelDescription get_top_level_description();

	clan::Ribbon *ribbon;
	Workspace *workspace;
	std::unique_ptr<UIController> ui_controller;
	SolutionExplorer *explorer;
	PropertyManager *property_manager;
	SolutionModel model;
	BuildSystem build_system;
	FileItemTypeFactory file_item_type_factory;
	TextEditorFileItemType text_editor_file_item_type;
};
}

/// \}
