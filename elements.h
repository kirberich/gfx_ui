#pragma once
#include <ILI9341_t3.h>
#include "color.h"

class UIElement {
public:
	virtual void update(ILI9341_t3 &lcd, bool reset = false);
	void draw(ILI9341_t3 &lcd);
	virtual void do_draw(ILI9341_t3 &lcd) = 0;
	virtual bool contains(const int &tx, const int &ty);
	virtual void touch_down(const int &tx, const int &ty);
	virtual void touch_up(const int &tx, const int &ty);
	virtual void touch_move(const int &tx, const int &ty);
	virtual bool is_active();
	virtual bool needs_update();
	virtual bool receive_move();

	bool get_is_touching();
	String get_name();

	void set_label(String label);
	void set_bg_color(Color bg_color);
	void set_fg_color(Color fg_color);
	void set_width(uint16_t width);
	void set_height(uint16_t height);
	void set_x(uint16_t x);
	void set_y(uint16_t y);
	void set_is_touching(bool is_touching);

protected:
	bool is_touching = false;
	bool _needs_update = true;
	bool is_drawn = false;
	int x;
	int y;
	uint16_t width;
	uint16_t height;
	Color bg_color;
	Color fg_color;
	String label;
	String name;
};

class Button : public UIElement {
public:
	Button(String name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, void (*callback)(UIElement&), Color bg_color, Color fg_color);
	void do_draw(ILI9341_t3 &lcd);
	bool contains(const int &tx, const int &ty);
	void touch_down(const int &tx, const int &ty);
	void touch_up(const int &tx, const int &ty);
protected:
	void (*callback)(UIElement&);
	bool state = false;
};

class ToggleButton : public Button {
public:
	ToggleButton(String name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, void (*callback)(UIElement&),Color bg_color, Color fg_color);
	bool get_state();
	void set_state(bool new_state);
	void touch_down(const int &tx, const int &ty);
	void touch_up(const int &tx, const int &ty);
};

class Label : public UIElement {
public:
	Label(String name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, byte size, Color bg_color, Color fg_color);
	void do_draw(ILI9341_t3 &lcd);

private:
	byte size;
};

class Dial : public UIElement {
public:
	Dial(String name, uint16_t x, uint16_t y, uint16_t radius, double data_min, double data_max, double data_step, Color bg_color, Color fg_color);
	void do_draw(ILI9341_t3 &lcd);
	bool receive_move();
	bool contains(const int &tx, const int &ty);
	void touch_down(const int &tx, const int &ty);
	void touch_move(const int &tx, const int &ty);

	double get_value();
	double get_min();
	double get_max();
	void set_value(double new_value);
	void set_min(double new_min);
	void set_max(double new_max);

private:
	uint16_t radius;
	double data_min;
	double data_max;
	double data_step;
	double value;
	uint16_t old_arc_x;
	uint16_t old_arc_y;

	int drag_start_x;
	int drag_start_y;
	double drag_start_value;
	double smooth_delta_x;
	double smooth_delta_y;
	uint16_t label_width = 0;

	void get_arc_coordinates(int &arc_x, int &arc_y);
	double data_range();
	String value_string(double _value);
	String value_string();
};
