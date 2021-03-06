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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/Components/ribbon_page.h"
#include "API/GUI/Components/ribbon_section.h"
#include "API/GUI/Components/ribbon.h"
#include "API/GUI/Components/push_button.h"
#include "ribbon_page_impl.h"
#include "ribbon_impl.h"

namespace clan
{

//////////////////////////////////////////////////////////////////////////
// Construction

RibbonPage::RibbonPage(Ribbon *parent, const std::string &text)
: GUIComponent(parent->impl->page_area, "ribbon-page"), impl(new RibbonPage_Impl(parent->impl.get(), text))
{
	impl->component = this;
	parent->impl->add_page(this);
}

//////////////////////////////////////////////////////////////////////////
// Attributes
	
//////////////////////////////////////////////////////////////////////////
// Operations

void RibbonPage::show_page(bool enable)
{
	for (size_t i = 0; i < impl->ribbon_impl->pages.size(); i++)
	{
		if (impl->ribbon_impl->pages[i] == this)
		{
			impl->ribbon_impl->tab_buttons[i]->set_visible(enable, false);
			set_visible(enable, false);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Implementation

}
