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
*/

#include "CSSLayout/precomp.h"
#include "css_parser_clip.h"
#include "API/CSSLayout/PropertyValues/css_value_clip.h"

namespace clan
{

std::vector<std::string> CSSParserClip::get_names()
{
	std::vector<std::string> names;
	names.push_back("clip");
	return names;
}

void CSSParserClip::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueClip> clip(new CSSValueClip());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "auto"))
		{
			clip->type = CSSValueClip::type_auto;
		}
		else if (equals(token.value, "inherit"))
		{
			clip->type = CSSValueClip::type_inherit;
		}
		else
		{
			return;
		}
	}
	else if (token.type == CSSToken::type_function && equals(token.value, "rect"))
	{
		CSSLength rect[4];
		bool rect_auto[4] = { false, false, false, false };
		bool comma_separated = false;
		for (int dir = 0; dir < 4; dir++)
		{
			token = next_token(pos, tokens);

			if (dir > 0 && token.type == CSSToken::type_delim && token.value == ",")
			{
				if (dir == 1)
					comma_separated = true;
				if (comma_separated)
					token = next_token(pos, tokens);
			}

			if (is_length(token))
			{
				if (!parse_length(token, rect[dir]))
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == CSSToken::type_ident && equals(token.value, "auto"))
			{
				rect_auto[dir] = true;
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		token = next_token(pos, tokens);
		if (token.type == CSSToken::type_bracket_end && pos == tokens.size())
		{
			clip->type = CSSValueClip::type_rect;
			clip->top = rect[0];
			clip->right = rect[1];
			clip->bottom = rect[2];
			clip->left = rect[3];
			clip->top_auto = rect_auto[0];
			clip->right_auto = rect_auto[1];
			clip->bottom_auto = rect_auto[2];
			clip->left_auto = rect_auto[3];
		}
		else
		{
			return;
		}
	}

	inout_values.push_back(std::move(clip));
}

}
