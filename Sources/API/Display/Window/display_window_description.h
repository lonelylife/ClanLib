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
**    Kenneth Gangstoe
*/


#pragma once

#include "../api_display.h"
#include <memory>
#ifdef WIN32
#include <windows.h>
#endif

namespace clan
{
/// \addtogroup clanDisplay_Window clanDisplay Window
/// \{

class Size;
class Rect;
class DisplayWindow;
class DisplayWindowDescription_Impl;

/// \brief Display window description class.
///
/// This class allows you to setup a more advanced description when creating a display window.
class CL_API_DISPLAY DisplayWindowDescription
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a window description with default values.
	DisplayWindowDescription();

	/// \brief Constructs a window description with default values and a title.
	DisplayWindowDescription(const std::string &title);

	/// \brief Constructs a window description with default values, a title and a position.
	DisplayWindowDescription(const std::string &title, const Rect &position, bool client_area);

	/// \brief Constructs a window description with default values, a title and a size.
	DisplayWindowDescription(const std::string &title, const Size &size, bool client_area);

	/// \brief Constructs a window description with default values and a position.
	DisplayWindowDescription(const Rect &position, bool client_area);

	/// \brief Constructs a DisplayWindowDescription
	/// \param copy = Display Window Description
	DisplayWindowDescription(const DisplayWindowDescription &copy);

	virtual ~DisplayWindowDescription();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the window title stored in the description.
	const std::string &get_title() const;

	/// \brief Returns the size of the window stored in description.
	Size get_size() const;

	/// \brief Returns the initial position of the window.
	Rect get_position() const;

	/// \brief Returns if the client area is used for the initial position of the window
	bool get_position_client_area() const;

	/// \brief Returns if window should be initially fullscreen.
	bool is_fullscreen() const;

	/// \brief Returns the amount of flipping buffers to be created.
	int get_flipping_buffers() const;

	/// \brief Returns the swap interval to be used for the window
	int get_swap_interval() const;

	/// \brief Returns true if window should be resizeable.
	bool get_allow_resize() const;

	/// \brief Returns the numbers of bytes per pixel in the window.
	int get_bpp() const;

	/// \brief Returns the refresh rate of the displaymode.
	/** <p>Specifies the frequency, in hertz (cycles per second).
	    This value is also known as the vertical refresh rate.</p> */
	int get_refresh_rate() const;

	/// \brief Returns true if window is layered (black is transparent)
	bool is_layered() const;

#ifdef WIN32
	/// \brief Windows only. Returns the window handle to be used for the window.
	HWND get_handle() const;
#endif

	/// \brief Returns true if a title bar is shown.
	bool has_caption() const;

	/// \brief Returns true if the window has a window menu on its title bar.
	bool has_sysmenu() const;

	/// \brief Returns true if the window has a minimize button.
	bool has_minimize_button() const;

	/// \brief Returns true if the window has a maximize button.
	bool has_maximize_button() const;
	
	/// \brief Returns true if the window is a tool window.
	bool is_tool_window() const;

	/// \brief Returns true if the window is a dialog window.
	bool is_dialog() const;

	/// \brief Returns true if the window is initially visible.
	bool is_visible() const;

	/// \brief Returns true if the window should be placed above all non-topmost windows.
	bool is_topmost() const;

	/// \brief Returns true if window from the window cache should be used. (GUI Only)
	/** <p>This setting is ignored unless the GUI and GUIWindowManagerSystem are used.</p> */
	bool get_using_gui_window_cache() const;

	/// \brief Returns true if the window is drawn with a drop shadow effect.
	bool has_drop_shadow() const;

	/// \brief Returns the window owning this one.
	DisplayWindow get_owner() const;

	/// \brief Returns true if a tablet context is to be created for the window.
	bool get_tablet_context() const;

	/// \brief Returns the minimum required depth buffer.
	int get_depth_size() const;

	/// \brief Returns the minimum required stencil buffer.
	int get_stencil_size() const;

	/// \brief Returns the index of the monitor to go fullscreen on. See ScreenInfo.
	int get_fullscreen_monitor() const;

	/// \brief Returns true if the screensaver is allowed
	bool get_allow_screensaver() const;

	/// \brief Returns true if DisplayWindow::update should be supported
	bool is_update_supported() const;

	/// \brief Returns the number of samples per pixel.
	int get_multisampling() const;
/// \}
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	DisplayWindowDescription &operator =(const DisplayWindowDescription &copy);

	/// \brief Controls if a title bar (including frame) is shown or not.
	void show_caption(bool value = true);

	/// \brief Sets if the window has a window menu on its title bar.
	void show_sysmenu(bool value = true);

	/// \brief Sets if the window has a minimize button.
	void show_minimize_button(bool value = true);

	/// \brief Sets if the window has a maximize button.
	void show_maximize_button(bool value = true);
	
	/// \brief Toggles whether the window is created as initially visible.
	void set_visible(bool value = true);

	/// \brief Sets if the window should be placed above all non-topmost windows.
	void set_topmost(bool value = true);

	/// \brief Flags the window to be a tool window to the windowing system.
	void set_tool_window(bool value = true);

	/// \brief Flags the window to be a dialog window to the windowing system.
	void set_dialog_window(bool value = true);

	/// \brief Enables a drop shadow effect on the window.
	void set_drop_shadow(bool value = true);

	/// \brief Specifies another window which owns this one.
	void set_owner_window(const DisplayWindow &owner);

	/// \brief Sets the title of the window.
	void set_title(const std::string &title);

	/// \brief Sets the size of the window (including window frame).
	/// \param size = Window size
	/// \param client_area = false = include the window frame. true = exclude the window frame
	void set_size(const Size &size, bool client_area);

	/// \brief Sets the position of the window (including window frame).
	///
	/// The default position, is centred on the screen. If this is wanted, use set_size() instead
	///
	/// \param position = Window position
	/// \param client_area = false = include the window frame. true = exclude the window frame
	void set_position(const Rect &position, bool client_area);

	/// \brief Makes the window initially fullscreen.
	void set_fullscreen(bool fullscreen = true, int monitor = 0);

	/// \brief Sets the amount of flipping buffers to be used.
	void set_flipping_buffers(int num_buffers = 2);

	/// \brief Sets the swap interval to be used for the window
	void set_swap_interval(int interval = -1);

	/// \brief Sets if windows should be resizable.
	void set_allow_resize(bool allow_resize = true);

	/// \brief Sets the number of bytes per pixel
	void set_bpp(int bpp);

	/// \brief Set the refresh rate of the displaymode.
	/** <p>Specifies the frequency, in hertz (cycles per second).
	    This value is also known as the vertical refresh rate.</p> */
	void set_refresh_rate(int refresh_rate);

	/// \brief Sets to true if a tablet input context should be created for this window.
	void set_tablet_context(bool create);

	/// \brief Uses a window from the window cache (GUI Only). 
	/** <p>Creating a window in Windows XP is slow (about 100 ms). Use cached 
		windows for menus and dialogs to avoid sluggish operation in XP. 
		This setting is ignored unless the GUI and GUIWindowManagerSystem are used.</p>*/
	void set_using_gui_window_cache(bool value);

	/// \brief Creates a layered window (complex shaped window)
	void set_layered(bool layered);

	/// \brief Allow the screensaver to appear
	///
	/// \param allow_screensaver = Allow the allow_screensaver (default is true)
	void set_allow_screensaver(bool allow_screensaver = true);

#ifdef WIN32
	/// \brief Windows only.  Use an existing window handle for the window.
	void set_handle(HWND handle);
#endif

	/// \brief Sets the minimum required depth buffer.
	/** <p>If this value is zero, the smallest available depth buffer is preferred. Otherwise,
	    the largest available depth buffer of at least the minimum size is preferred.</p>*/
	void set_depth_size(int value);

	/// \brief Sets the minimum required stencil buffer.
	/** <p>If this value is zero, the smallest available stencil buffer is preferred. Otherwise,
	    the largest available stencil buffer of at least the minimum size is preferred.</p>*/
	void set_stencil_size(int value);

	/// \brief Controls if DisplayWindow::update is supported
	///
	/// The Direct3D target needs to know at window creation time if the application intends to update
	/// the screen by calling DisplayWindow::update.
	void set_update_supported(bool value) const;

	/// \brief Sets the number of samples per pixel.
	void set_multisampling(int value);

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<DisplayWindowDescription_Impl> impl;
/// \}
};

}

/// \}
