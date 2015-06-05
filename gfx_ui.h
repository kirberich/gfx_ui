#pragma once
#include <Adafruit_GFX.h>
#include <TouchScreen.h>

const unsigned int MINPRESSURE = 1;
const unsigned int MAXPRESSURE = 1000;

class UIElement {
public:
	virtual void update(Adafruit_GFX &lcd);
	virtual void draw(Adafruit_GFX &lcd) = 0;
	virtual bool contains(uint16_t tx, uint16_t ty);
	virtual void touch_down();
	virtual void touch_up();
	virtual bool is_active();
	virtual bool needs_update();

protected:
	bool is_pressed = false;
	bool _needs_update = true;
};

class GFX_UI {
public:
	GFX_UI(Adafruit_GFX &lcd, TouchScreen &touch_screen, uint16_t ts_min_x, uint16_t ts_min_y, uint16_t ts_max_x, uint16_t ts_max_y);
	void update();
	void update(bool update_all);
	void info();
	void handle_touch();
	void handle_touch_down();
	void handle_touch_up();
	void add(UIElement &element);
private:
	uint16_t width;
	uint16_t height;
	UIElement *elements[100];
	byte num_elements = 0;

	uint16_t ts_min_x;
	uint16_t ts_min_y;
	uint16_t ts_max_x;
	uint16_t ts_max_y;

	bool is_touching = false;
	uint16_t touch_x = 0;
	uint16_t touch_y = 0;
	uint16_t touch_pressure = 0;
	unsigned long last_touch = 0;

	Adafruit_GFX *p_lcd;
	TouchScreen *p_touch_screen;
};

class Button : public UIElement {
public:
	Button(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, void (*callback)(), uint16_t bg_color, uint16_t fg_color);
	void draw(Adafruit_GFX &lcd);
	bool contains(uint16_t tx, uint16_t ty);
	void touch_down();
	void touch_up();
	void set_label(String label);

private:
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t bg_color;
	uint16_t fg_color;
	String label;
	void (*callback)();
};

class Label : public UIElement {
public:
	Label(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, byte size, uint16_t bg_color, uint16_t fg_color);
	void draw(Adafruit_GFX &lcd);
	void set_label(String label);

private:
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	String label;
	byte size;
	uint16_t bg_color;
	uint16_t fg_color;
};
