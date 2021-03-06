
#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/Display/Window/keys.h>

#include <map>

#include <ClanLib/csslayout.h>

#include "calendar.h"


class CL_CalendarComponent_Impl
{
public:
	CL_CalendarComponent_Impl() : component(0), has_mouse_over(false), selection_valid(false), hot_xy_index(-1,-1)
	{
		day_names = CL_StringHelp::split_text("Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday", ",");
		day_short_names = CL_StringHelp::split_text("Su,Mo,Tu,We,Th,Fr,Sa", ",");
		month_names = CL_StringHelp::split_text("January,February,March,April,May,June,July,August,September,October,November,December", ",");
		month_short_names = CL_StringHelp::split_text("Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec", ",");
		
		date_view = CL_DateTime::get_current_local_time();
	}

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_process_message(CL_GUIMessage &msg);
	void on_style_changed();
	void on_enablemode_changed();
	void on_resize();

	void on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_left_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_left_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_leave();

	void create_parts();
	int get_days_in_month(int month);
	CL_Rect get_day_name_box(int day);
	int get_day_at_pos(const CL_Point &pos);
	CL_Vec2i get_xy_index_at_pos(const CL_Point &pos);
	CL_Callback_v0 func_selection_changed;

	CL_CalendarComponent *component;
	CL_Callback_v0 func_clicked;
	CL_CSSLayout layout;
	CL_DateTime current_date;

	CL_GUIThemePart part_day;
	CL_GUIThemePart part_week;
	CL_GUIThemePart part_next_month;
	CL_GUIThemePart part_prev_month;
	CL_GUIThemePart part_background;
	CL_GUIThemePart part_day_name;
	CL_GUIThemePart part_month_year;

	std::vector<CL_String> day_names;
	std::vector<CL_String> day_short_names;
	std::vector<CL_String> month_names;
	std::vector<CL_String> month_short_names;

	bool has_mouse_over;
	CL_Point last_mouse_pos;
	CL_DateTime date_view;
	CL_DateTime date_selected;

	CL_Rect content_box;
	CL_Rect next_month_box;
	CL_Rect prev_month_box;
	CL_Rect month_year_box;

	CL_Vec2i hot_xy_index;
	int day_table[6][7];
	bool selection_valid; // initially false, set when a date is picked.
};

//////////////////////////////////////////////////////////////////////////
// Construction

CL_CalendarComponent::CL_CalendarComponent(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_CalendarComponent_Impl)
{
	impl->component = this;

	set_type_name("calendar");
	set_blocks_default_action(false);
	set_focus_policy(focus_local);

	func_process_message().set(impl.get(), &CL_CalendarComponent_Impl::on_process_message);
	func_render().set(impl.get(), &CL_CalendarComponent_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_CalendarComponent_Impl::on_style_changed);
	func_enablemode_changed().set(impl.get(), &CL_CalendarComponent_Impl::on_enablemode_changed);
	func_resized().set(impl.get(), &CL_CalendarComponent_Impl::on_resize);

	impl->create_parts();
}

//////////////////////////////////////////////////////////////////////////
// Attributes

CL_Size CL_CalendarComponent::get_preferred_size() const
{
	CL_Size size;
	size.height = 
		impl->part_next_month.get_preferred_height() +
		impl->part_day_name.get_preferred_height() + 
		6 * impl->part_day.get_preferred_height();

	size.width = cl_max(
		impl->part_prev_month.get_preferred_width() + 
		impl->part_next_month.get_preferred_width() + 
		impl->part_month_year.get_preferred_width(), 
		// OR
		7 * impl->part_day.get_preferred_width());

	CL_Rect shrink = impl->part_background.get_content_shrink_box();	
	size.width += abs(shrink.left) + abs(shrink.right);
	size.height += abs(shrink.top) + abs(shrink.bottom);

	return size;
}

CL_DateTime CL_CalendarComponent::get_selected_date() const
{
	return impl->date_selected;
}

//////////////////////////////////////////////////////////////////////////
// Operations

void CL_CalendarComponent::set_current_date(const CL_DateTime &date)
{
	impl->current_date = date;

	request_repaint();
}

//////////////////////////////////////////////////////////////////////////
// Operations

void CL_CalendarComponent::set_day_names( CL_String comma_separated_monday_to_sunday )
{
	impl->day_names = CL_StringHelp::split_text(comma_separated_monday_to_sunday, ",");
}

void CL_CalendarComponent::set_day_short_names( CL_String comma_separated_monday_to_sunday )
{
	impl->day_short_names = CL_StringHelp::split_text(comma_separated_monday_to_sunday, ",");
}

void CL_CalendarComponent::set_month_names( CL_String comma_separated_monday_to_sunday )
{
	impl->day_names = CL_StringHelp::split_text(comma_separated_monday_to_sunday, ",");
}

void CL_CalendarComponent::set_month_short_names( CL_String comma_separated_monday_to_sunday )
{

}

void CL_CalendarComponent::clear_selection()
{
	impl->selection_valid = false;
}

//////////////////////////////////////////////////////////////////////////
// Signals

CL_Callback_v0 &CL_CalendarComponent::func_selection_changed()
{
	return impl->func_selection_changed;
}

//////////////////////////////////////////////////////////////////////////
// Implementation

void CL_CalendarComponent_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	part_background.render_box(gc, component->get_size(), update_rect);
	
	//////////////////////////////////////////////////////////////////////////
	// Header: month navigation, current month & year text

	// Draw prev & next month graphics
	part_prev_month.render_box(gc, prev_month_box, update_rect);
	part_next_month.render_box(gc, next_month_box, update_rect);

	// Month name & year
	part_month_year.render_box(gc, month_year_box, update_rect);
	part_month_year.render_text(gc,
		month_names[date_view.get_month()-1] + " " + CL_StringHelp::int_to_text(date_view.get_year()),
		part_month_year.get_content_box(month_year_box),
		update_rect);

	//////////////////////////////////////////////////////////////////////////
	// Day names row (mo, tu, we...)
	for (int i=0; i<7; i++)
	{
		CL_Rect day_name_box = get_day_name_box(i);
		CL_Rect day_name_content_box = part_day_name.get_content_box(day_name_box);
		part_day_name.render_box(gc, day_name_box, update_rect);
		part_day_name.render_text(gc, day_short_names[(i+1)%7], day_name_content_box, update_rect);
		day_name_box.translate(day_name_box.get_width(), 0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Day numbers

	CL_DateTime today = CL_DateTime::get_current_local_time();

	CL_Rect tmp_day_name_box = get_day_name_box(0);
	CL_Rect day_rect = part_day.get_preferred_size();
	day_rect.translate(content_box.left, tmp_day_name_box.bottom);

	int prev_month = date_view.get_month()-1;
	if (prev_month < 1)
		prev_month = 12;

	int days_in_month = get_days_in_month(date_view.get_month());
	int days_in_prev_month = get_days_in_month(prev_month);

	CL_DateTime first_of_month(date_view.get_year(), date_view.get_month(), 1);
	int num_days_of_prev_month = first_of_month.get_day_of_week()-1;
	if (num_days_of_prev_month < 1)
		num_days_of_prev_month += 7;

	int day_number = days_in_prev_month - num_days_of_prev_month + 1;

	part_day.set_state("filler", true);
	bool drawing_prev_month_days = true;
	bool drawing_next_month_days = false;

	for (int w=0; w<6; w++)
	{
		CL_Rect day_rect_iterator = day_rect;

		for (int d=0;d<7;d++)
		{
			if (CL_Vec2i(d,w) == hot_xy_index)
			{
				part_day.set_state("hot", true);
			}

			if (!drawing_prev_month_days && !drawing_next_month_days)
			{
				if (today.get_year() == date_view.get_year() &&
					today.get_month() == date_view.get_month() && 
					today.get_day() == day_number)
				{
					part_day.set_state("today", true);
				}
				if (selection_valid && 
					date_selected.get_day() == day_number && 
					date_selected.get_month() == date_view.get_month() && 
					date_selected.get_year() == date_view.get_year())
				{
					part_day.set_state("selected", true);
				}
			}

			part_day.render_box(gc, day_rect_iterator, update_rect);
			CL_Rect day_content_box = part_day.get_content_box(day_rect_iterator);
			part_day.render_text(gc, CL_StringHelp::int_to_text(day_number), day_content_box, update_rect);
			day_rect_iterator.translate(day_rect.get_width(), 0);

			day_table[w][d] = day_number; // update table for clicking.

			if (CL_Vec2i(d,w) == hot_xy_index)
			{
				part_day.set_state("hot", false);
			}
			if (!drawing_prev_month_days && !drawing_next_month_days)
			{
				if (today.get_year() == date_view.get_year() &&
					today.get_month() == date_view.get_month() && 
					today.get_day() == day_number)
				{
					part_day.set_state("today", false);
				}
				if (selection_valid && 
					date_selected.get_day() == day_number && 
					date_selected.get_month() == date_view.get_month() && 
					date_selected.get_year() == date_view.get_year())
				{
					part_day.set_state("selected", false);
				}
			}

			day_number++;

			if (drawing_prev_month_days)
			{
				if (day_number > days_in_prev_month )
				{
					drawing_prev_month_days = false;
					day_number = 1;
					part_day.set_state("filler", false);
				}
			}
			else
			{
				if (day_number > days_in_month)
				{
					day_number = 1;
					part_day.set_state("filler", true);
					drawing_next_month_days = true;
				}
			}
		}

		day_rect.translate(0, day_rect.get_height());
	}
}

void CL_CalendarComponent_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!component->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input = msg;
		CL_InputEvent input_event = input.get_event();
		if (input_event.type == CL_InputEvent::pointer_moved)
			on_mouse_move(input, input_event);
		else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_LEFT)
			on_mouse_left_down(input, input_event);
		else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_LEFT)
			on_mouse_left_up(input, input_event);
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
	}
}

void CL_CalendarComponent_Impl::on_style_changed()
{

}

void CL_CalendarComponent_Impl::on_enablemode_changed()
{

}

void CL_CalendarComponent_Impl::create_parts()
{
	part_background = CL_GUIThemePart(component);
	part_next_month = CL_GUIThemePart(component, "next_month");
	part_prev_month = CL_GUIThemePart(component, "prev_month");
	part_day_name = CL_GUIThemePart(component, "day_name");
	part_day = CL_GUIThemePart(component, "day");
	part_week = CL_GUIThemePart(component, "week");
	part_month_year = CL_GUIThemePart(component, "month_year");
}

int CL_CalendarComponent_Impl::get_days_in_month( int month )
{
	if (month < 1 || month > 12)
		throw CL_Exception("CL_CalendarComponent: Month parameter not in 1-12 interval.");

	int days_per_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	int num_days = days_per_month[month-1];

	if (month == 2)
	{
		int year = date_view.get_year();
		bool leap_year = (year%4) == 0; // Leap years are every 4th year
		if ((year%100) == 0 && (year%400) != 0) // Except for those divisible by 100 unless they are also divisible by 400
			leap_year = false;
		if (leap_year)
		{
			num_days = 29;
		}
	}

	return num_days;
}

void CL_CalendarComponent_Impl::on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	bool changed = false;

	CL_Point mouse_pos = input_event.mouse_pos;

	CL_Vec2i xy_index = get_xy_index_at_pos(mouse_pos);
	if (xy_index != hot_xy_index)
	{
		hot_xy_index = xy_index;
		changed = true;
	}

	changed |= part_prev_month.set_state("hot", prev_month_box.contains(mouse_pos));
	changed |= part_next_month.set_state("hot", next_month_box.contains(mouse_pos));
	changed |= part_month_year.set_state("hot", month_year_box.contains(mouse_pos));

	if (changed)
		component->request_repaint();
}

void CL_CalendarComponent_Impl::on_mouse_left_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	if (next_month_box.contains(input_event.mouse_pos))
	{
		int month = date_view.get_month();
		int year = date_view.get_year();

		month++;
		if (month > 12)
		{
			month = 1;
			year++;
			date_view.set_year(year);
		}
		date_view.set_month(month);
		component->request_repaint();
	}
	else if (prev_month_box.contains(input_event.mouse_pos))
	{
		int month = date_view.get_month();
		int year = date_view.get_year();

		month--;
		if (month < 1)
		{
			month = 12;
			year--;
			date_view.set_year(year);
		}
		date_view.set_month(month);
		component->request_repaint();
	}
	else
	{
		int tmp_day = get_day_at_pos(input_event.mouse_pos);
		if (tmp_day != -1)
		{
			if (selection_valid)
			{
				if (tmp_day != date_selected.get_day())
				{
					date_selected = date_view;
					date_selected.set_day(tmp_day);
					if (!func_selection_changed.is_null())
						func_selection_changed.invoke();
					component->request_repaint();
				}
			}
			else
			{
				date_selected = date_view;
				date_selected.set_day(tmp_day);
				selection_valid = true;
				if (!func_selection_changed.is_null())
					func_selection_changed.invoke();
				component->request_repaint();
			}
		}
	}
}

void CL_CalendarComponent_Impl::on_mouse_left_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{

}

void CL_CalendarComponent_Impl::on_resize()
{
	content_box = part_background.get_content_box(component->get_size());

	prev_month_box = part_next_month.get_preferred_size();
	prev_month_box.translate(content_box.left, content_box.top);

	next_month_box = part_next_month.get_preferred_size();
	next_month_box.translate(content_box.right - next_month_box.get_width(), content_box.top);

	month_year_box = CL_Rect(prev_month_box.right, content_box.top, next_month_box.left, next_month_box.bottom);

	component->request_repaint();
}

CL_Rect CL_CalendarComponent_Impl::get_day_name_box(int day)
{
	CL_Rect box = part_day_name.get_preferred_size();
	box.translate(content_box.left, prev_month_box.bottom);
	box.translate(day * box.get_width(), 0);
	return box;
}

void CL_CalendarComponent_Impl::on_mouse_leave()
{
	hot_xy_index = CL_Vec2i(-1,-1);
	part_month_year.set_state("hot", false);
	part_prev_month.set_state("hot", false);
	part_next_month.set_state("hot", false);
	component->request_repaint();
}

CL_Vec2i CL_CalendarComponent_Impl::get_xy_index_at_pos(const CL_Point &pos)
{
	CL_Rect day_name_box = get_day_name_box(0);
	CL_Rect day_box = part_day.get_preferred_size();
	CL_Rect days_area(CL_Point(day_name_box.left, day_name_box.bottom), CL_Size(day_box.right*7, day_box.bottom*6));

	CL_Vec2i xy_index(-1,-1);
	if (days_area.contains(pos))
	{
		xy_index = (pos - days_area.get_top_left()) / day_box.get_bottom_right();
	}

	return xy_index;
}

int CL_CalendarComponent_Impl::get_day_at_pos(const CL_Point &pos)
{
	CL_Vec2i xy_index = get_xy_index_at_pos(pos);

	int day = day_table[xy_index.y][xy_index.x];

	if (xy_index.y <= 1 && day > 15)
		day = -1;
	else if (xy_index.y > 3 && day < 15)
		day = -1;

	return day;
}

