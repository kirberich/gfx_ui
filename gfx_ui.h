#pragma once
#include <UTouch.h>
#include <ILI9341_t3.h>
#include <SPI.h>
#include "utils.h"
#include "color.h"
#include "elements.h"
#include <Adafruit_GFX.h>

class GFX_UI {
public:
	GFX_UI();
	void init(Color bg_color = Color({0, 0, 0}), byte rotation = 0);
	void update();
	void update(bool update_all);
	void info();
	void handle_touch();
	void handle_touch_down(const int &tx, const int &ty);
	void handle_touch_up(const int &tx, const int &ty);
	void handle_touch_move(const int &tx, const int &ty);
	void add(UIElement &element);
	uint16_t width();
	uint16_t height();

	void set_bg_color(Color new_bg_color);
	UIElement* get_element_by_name(String name);
	ILI9341_t3& get_lcd();

private:
	UIElement *elements[100];
	byte num_elements = 0;

	bool is_touching = false;
	int last_x = 0;
	int last_y = 0;
	unsigned long last_touch = 0;
	ILI9341_t3 lcd;
	UTouch touch_screen;

	bool flip_touch = false;

	Color bg_color;
};
