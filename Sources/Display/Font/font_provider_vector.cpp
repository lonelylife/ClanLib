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
**    Mark Page
*/

#include "Display/precomp.h"
#include "font_provider_vector.h"

#ifdef __APPLE__
#include "FontEngine/font_engine_cocoa.h"
#elif defined(WIN32)
#include "FontEngine/font_engine_win32.h"
#else
#include "X11/font_engine_freetype.h"
#include "../Display/X11/font_config.h"
#endif

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_vector.h"
#include "API/Display/2D/shape2d.h"
#include "../2D/render_batch_triangle.h"
#include "../2D/canvas_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Vector Construction:

FontProvider_Vector::FontProvider_Vector() : font_engine(NULL), is_filled(true)
{
}

void FontProvider_Vector::load_font(const FontDescription &desc, const std::string &filename)
{
#ifdef WIN32
	font_engine = new FontEngine_Win32(desc, filename);
#elif defined(__APPLE__)
	font_engine = new FontEngine_Cocoa(desc, filename);
#else

	std::string font_file_path = filename;
	if (font_file_path.empty())
	{
	    // Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		font_file_path = fc.match_font(desc);
	}

	std::string path = PathHelp::get_fullpath(font_file_path, PathHelp::path_type_file);
	std::string new_filename = PathHelp::get_filename(font_file_path, PathHelp::path_type_file);
	FileSystem vfs(path);
	IODevice io_dev = vfs.open_file(new_filename);

	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Ensure width and height are positive
	if (average_width < 0) average_width =-average_width;
	if (height < 0) height =-height;

	font_engine = new FontEngine_Freetype(io_dev, average_width, height);
#endif

	metrics = font_engine->get_metrics();
}

FontProvider_Vector::~FontProvider_Vector()
{
	if (font_engine)
	{
		delete font_engine;
	}

}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Vector Attributes:


FontMetrics FontProvider_Vector::get_font_metrics()
{
	return metrics;
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Vector Operations:

int FontProvider_Vector::get_character_index(GraphicContext &gc, const std::string &text, const Point &point)
{
	throw Exception("FIXME");
}

void FontProvider_Vector::draw_text(Canvas &canvas, float x, float y, const std::string &text, const Colorf &color)
{
	if (text.length() == 0)
		return;

	unsigned int text_length = text.length();
	DataBuffer buf_out_glyphs(sizeof(int) * (text_length+1));
	DataBuffer buf_interspacing_x(sizeof(float) * (text_length+1));
	int *out_glyphs = (int *) buf_out_glyphs.get_data();
	float *interspacing_x = (float *) buf_interspacing_x.get_data();
 
	get_glyphs(text, out_glyphs, interspacing_x, 0);

	draw_glyphs(canvas, x, y, out_glyphs, text_length, is_filled, interspacing_x, 0, color);

}

Size FontProvider_Vector::get_text_size(GraphicContext &gc, const std::string &text)
{
	if (text.length() == 0)
		return Size(0, 0);

	unsigned int text_length = text.length();
	DataBuffer buf_out_glyphs(sizeof(int) * (text_length+1));
	DataBuffer buf_interspacing_x(sizeof(float) * (text_length+1));
	int *out_glyphs = (int *) buf_out_glyphs.get_data();
	float *interspacing_x = (float *) buf_interspacing_x.get_data();
 
	get_glyphs(text, out_glyphs, interspacing_x, 0);

	float offset_x=0.0f;
	float offset_y=metrics.get_height();

	float max_x = 0.0f;

	for( int i=0; i<text_length; i++ )
	{
		offset_x += interspacing_x[i];

		if (offset_x > max_x)
			max_x = offset_x;

		if( out_glyphs[i] == '\n' )
		{
			offset_x = 0;
			offset_y += metrics.get_height();
		}
	}
	return Size(max_x, offset_y);
}

void FontProvider_Vector::get_glyphs(
	const std::string &text,
	int *out_glyphs,
	float *out_interspacing_x,
	float *out_interspacing_y)
{
	for( unsigned int i=0; i<text.length(); i++ )
	{
		store_in_char_cache(text[i]);
		
		out_glyphs[i] = text[i];
		out_interspacing_x[i] = char_cache[text[i]].advance_x;
	}
}

void FontProvider_Vector::store_in_char_cache(unsigned int glyph)
{
	if( char_cache.find(glyph) == char_cache.end() )
	{
		int out_advance_x;
		Shape2D outline = font_engine->load_glyph_outline(glyph, out_advance_x);
		vector_glyph &vg = char_cache[glyph];
		vg.advance_x = out_advance_x;
		outline.get_triangles(vg.primitives_array);
		outline.get_outline(vg.primitives_array_outline);
	}
}

void FontProvider_Vector::draw_glyphs(
	Canvas &canvas,
	float x,
	float y,
	int *glyphs,
	int length,
	bool filled,
	float *interspacing_x,
	float *interspacing_y,
	const Colorf &color)
{
	float offset_x=0;
	float offset_y=0;

	for( int i=0; i<length; i++ )
	{
		canvas.push_translate(x+offset_x, y+offset_y);

		if( filled )
		{
			draw_prim_array(canvas, char_cache[glyphs[i]], color);
		}
		else
		{
			std::vector< std::vector<Vec2f> > &prim_array_outline = char_cache[glyphs[i]].primitives_array_outline;

			std::vector< std::vector<Vec2f> >::iterator it;

			for (it = prim_array_outline.begin(); it != prim_array_outline.end(); ++it)
			{
				canvas.draw_line_strip(&((*it)[0]), it->size(), color);
			}
		}
		
		canvas.pop_modelview();
		
		offset_x += interspacing_x[i];
		if( glyphs[i] == '\n' )
		{
			offset_x = 0;
			y += metrics.get_height();
		}
	}
}

void FontProvider_Vector::set_filled(bool enable)
{
	is_filled = enable;
}

Rectf FontProvider_Vector::get_bounding_box(const std::string &reference_string)
{
	Rectf bounding_rect(100000.0f, 100000.0f, -100000.0f, -100000.0f);
	for( unsigned int i=0; i<reference_string.length(); i++ )
	{
		store_in_char_cache(reference_string[i]);
		std::vector<Vec2f> &primitives_array = char_cache[reference_string[i]].primitives_array;

		for (unsigned int cnt=0; cnt< primitives_array.size(); cnt++)
		{
			clan::Vec2f pos = primitives_array[cnt];
			bounding_rect.left = clan::min(bounding_rect.left, pos.x);
			bounding_rect.top = clan::min(bounding_rect.top, pos.y);
			bounding_rect.right = clan::max(bounding_rect.right, pos.x);
			bounding_rect.bottom = clan::max(bounding_rect.bottom, pos.y);
		}

	}

	if (bounding_rect.left > bounding_rect.right)
		return Rectf();

	return bounding_rect;
}


void FontProvider_Vector::set_texture(const Texture2D &src_texture, const Rectf &bounding_rect, const Rectf &texture_rect)
{
	current_texture = src_texture;
	current_bounding_rect = bounding_rect;
	current_texture_rect = texture_rect;

}

const std::vector<Vec2f> &FontProvider_Vector::get_glyph_filled(unsigned int glyph)
{
	store_in_char_cache(glyph);
	return char_cache[glyph].primitives_array;
}

const std::vector< std::vector<Vec2f> > &FontProvider_Vector::get_glyph_outline(unsigned int glyph)
{
	store_in_char_cache(glyph);
	return char_cache[glyph].primitives_array_outline;
}

/////////////////////////////////////////////////////////////////////////////
// FontProvider_Vector Implementation:

void FontProvider_Vector::draw_prim_array(Canvas &canvas, vector_glyph &vg, const Colorf &color)
{
	if (vg.primitives_array.empty())
		return;

	if (current_texture.is_null())
	{
		canvas.fill_triangles(&vg.primitives_array[0], vg.primitives_array.size(), color);
		return;
	}

	if ( (current_bounding_rect != vg.calculated_bounding_rect) || (current_texture_rect != vg.calculated_texture_rect) || vg.texture_positions.empty() )
	{
		vg.calculated_bounding_rect = current_bounding_rect;
		vg.calculated_texture_rect = current_texture_rect;
		vg.texture_positions.resize(vg.primitives_array.size());

		float scale_x = current_texture_rect.get_width() / current_bounding_rect.get_width();
		float scale_y = current_texture_rect.get_height() / current_bounding_rect.get_height();


		// Recalculate the texture positions
		for (unsigned int cnt=0; cnt< vg.primitives_array.size(); cnt++)
		{
			clan::Vec2f pos = vg.primitives_array[cnt];
			pos.x -= current_bounding_rect.left;
			pos.y -= current_bounding_rect.top;
			pos.x *= scale_x;
			pos.y *= scale_y;
			pos.x += current_texture_rect.left;
			pos.y += current_texture_rect.top;

			vg.texture_positions[cnt] = pos;
		}
	}
	canvas.fill_triangles(&vg.primitives_array[0], &vg.texture_positions[0], vg.primitives_array.size(), current_texture, color);

}

}
