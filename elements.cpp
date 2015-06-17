#include "elements.h"

void noop(UIElement& caller) {}


/*
 *  UIElement Base Class
 */

void UIElement::update(ILI9341_t3 &lcd, bool reset) {
	if (reset) {
		is_drawn = false;
	}

	if (reset || _needs_update) {
		draw(lcd);
	}

	_needs_update = false;
}

void UIElement::draw(ILI9341_t3 &lcd) {
	do_draw(lcd);
	is_drawn = true;
}

bool UIElement::is_active() {
	return is_touching;
}

bool UIElement::needs_update() {
	return _needs_update;
}

void UIElement::touch_down(const int &tx, const int &ty) {}
void UIElement::touch_up(const int &tx, const int &ty) {}
void UIElement::touch_move(const int &tx, const int &ty) {}

bool UIElement::contains(const int &tx, const int &ty) {
	return false;
}

bool UIElement::get_is_touching() {
	return is_touching;
}

String UIElement::get_name() {
	return name;
}

bool UIElement::receive_move() {
	return false;
}

void UIElement::set_label(String label) {
	_needs_update = true;
	this->label = label;
}

void UIElement::set_bg_color(Color bg_color) {
	_needs_update = true;
	this->bg_color = bg_color;
}

void UIElement::set_fg_color(Color fg_color) {
	_needs_update = true;
	this->fg_color = fg_color;
}

void UIElement::set_width(uint16_t width) {
	_needs_update = true;
	this->width = width;
}
void UIElement::set_height(uint16_t height) {
	_needs_update = true;
	this->height = height;
}
void UIElement::set_x(uint16_t x) {
	_needs_update = true;
	this->x = x;
}
void UIElement::set_y(uint16_t y) {
	_needs_update = true;
	this->y = y;
}
void UIElement::set_is_touching(bool is_touching) {
	_needs_update = true;
	this->is_touching = is_touching;
}

/*
 *  Button
 */

Button::Button(String name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, void (*callback)(UIElement&), Color bg_color, Color fg_color) {
	this->name = name;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->label = label;
	this->callback = callback;
	this->bg_color = bg_color;
	this->fg_color = fg_color;
}

void Button::do_draw(ILI9341_t3 &lcd) {
	Color _fg_color = state || is_touching ? bg_color : fg_color;
	Color _bg_color;
	if (is_touching) {
		_bg_color = fg_color + Color({50, 50, 50});
	} else {
		_bg_color = state ? fg_color : bg_color;
	}

	uint16_t lum = _bg_color.luminosity();

	lcd.setCursor(x + width/2 - (label.length()*6)/2, (y+height/2-4));
	lcd.setTextSize(1);

	lcd.fillRect(x+2, y+2, width-4, height/2, _bg_color);
	lcd.setTextColor(_fg_color);

	// Draw highlights
	if (lum > 50) {
		Color hl_color1 = Color({_bg_color.red/4, _bg_color.green/4, _bg_color.blue/4});
		Color hl_color2 = Color({_bg_color.red/5, _bg_color.green/5, _bg_color.blue/5});
		lcd.drawLine(x+2, y+2, x+width-4, y+2, _bg_color - hl_color2);
		lcd.fillRect(x+2, y + height/2, width-4, height/2-2, _bg_color - hl_color2);
		lcd.drawLine(x+3, y + height/2, x+width-4, y + height/2, _bg_color + hl_color1);
	} else {
		lcd.fillRect(x+2, y+2, width-4, height/2-2, _bg_color + Color({15, 15, 15}));
		lcd.drawLine(x+3, y + height/2, x+width-4, y + height/2, Color({50, 50, 50}));
	}

	// Outline, note this always uses the actual bg_color, not _bg_color
	lcd.drawRect(x+1, y+1, width-2, height-2, bg_color);
	lcd.drawRect(x, y, width, height, fg_color);

	lcd.print(label);
}

bool Button::contains(const int &tx, const int &ty) {
	if (tx >= x && tx <= x+width && ty >= y && ty <= y+height) {
		return true;
	}
	return false;
}

void Button::touch_down(const int &tx, const int &ty) {
	state = true;
}

void Button::touch_up(const int &tx, const int &ty) {
	state = false;
	if (contains(tx, ty)) {
		callback(*this);
	}
}

/*
 *  ToggleButton
 */

ToggleButton::ToggleButton(String name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, void (*callback)(UIElement&), Color bg_color, Color fg_color)
	: Button::Button(name, x, y, width, height, label, callback, bg_color, fg_color) {}

void ToggleButton::touch_down(const int &tx, const int &ty) {
	state = !state;
}

void ToggleButton::touch_up(const int &tx, const int &ty) {
	if (contains(tx, ty)) {
		callback(*this);
	} else {
		state = false;
	}
}

/*
 *  Label
 */

Label::Label(String name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, byte size, Color bg_color, Color fg_color) {
	this->name = name;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->label = label;
	this->size = size;
	this->bg_color = bg_color;
	this->fg_color = fg_color;
}

void Label::do_draw(ILI9341_t3 &lcd) {
	lcd.fillRect(x, y, width, height, bg_color);

	lcd.setCursor(x + width/2 - (label.length()*6*size)/2, (y+height/2-4*size));
	lcd.setTextSize(size);
	lcd.setTextColor(fg_color);

	lcd.print(label);
}

/*
 * Dial
 */

Dial::Dial(String name, uint16_t x, uint16_t y, uint16_t radius, double data_min, double data_max, double data_step, Color bg_color, Color fg_color) {
	this->name = name;
	this->x = x;
	this->y = y;
	this->width = radius*2;
	this->height = radius*2 + 10; // Height of Dial plus label plus 2 pixels padding
	this->radius = radius;
	this->data_min = data_min;
	this->data_max = data_max;
	this->data_step = data_step;
	this->bg_color = bg_color;
	this->fg_color = fg_color;
}

bool Dial::contains(const int &tx, const int &ty) {
	if (abs(tx - x) <= radius && abs(ty - y) <= radius) {
		return true;
	}
	return false;
}

double Dial::get_value() {
	return value;
}

double Dial::get_min() {
	return data_min;
}

double Dial::get_max() {
	return data_max;
}

void Dial::get_arc_coordinates(int &arc_x, int &arc_y) {
	double val_rad = -1.5707 + (((value - data_min)/(data_max - data_min)) * 6.28);
	arc_x = round(x + (radius-3)*(cos(val_rad)));
	arc_y = round(y + (radius-3)*(sin(val_rad)));
}

double Dial::data_range() {
	return data_max - data_min;
}

void Dial::set_value(double new_value) {
	double old_value = value;

	int steps = round(new_value/data_step);
	new_value = steps*data_step;
	value = max(data_min, min(data_max, new_value));

	if (old_value != value) {
		_needs_update = true;
	}
}

void Dial::set_min(double new_min) {
	data_min = new_min;
	set_value(value);
}

void Dial::set_max(double new_max) {
	data_max = new_max;
	set_value(value);
}

String Dial::value_string(double _value) {
	return fmod(data_step, 1) == 0 ? String(int(round(_value))) : String(_value, 2);
}

String Dial::value_string() {
	return value_string(value);
}

void Dial::do_draw(ILI9341_t3 &lcd) {
	// Get amount of degrees the value should be displaced around the Dial
	int x2;
	int y2;
	get_arc_coordinates(x2, y2);

	Color highlight = bg_color + Color({50, 50, 50});

	if(is_drawn) {
		lcd.drawLine(x, y, old_arc_x, old_arc_y, bg_color);

		Color dial_highlight = Color({255, 128, 128});
		lcd.drawLine(x, y, x2, y2, is_touching ? dial_highlight : fg_color);

		old_arc_x = x2;
		old_arc_y = y2;
	} else {
		old_arc_x = x2;
		old_arc_y = y2;

		lcd.fillCircle(x, y, radius, bg_color);
		lcd.drawCircle(x, y, radius, fg_color);
		lcd.drawCircle(x, y, radius-1, highlight);

		lcd.drawLine(x, y, x2, y2, fg_color);

		label_width = max(label_width, value_string(data_max).length()*6 + 4);
		label_width = max(label_width, value_string(data_min).length()*6 + 4);

		lcd.fillRect(x-label_width/2, y+radius + 4, label_width, 10, bg_color);
		lcd.drawRect(x-label_width/2 - 1, y+radius + 3, label_width+2, 12, highlight);
		lcd.drawRect(x-label_width/2 - 2, y+radius + 2, label_width+4, 14, fg_color);
	}

	String label = value_string();
	label_width = max(label_width, label.length()*6 + 4);

	lcd.fillRect(x-label_width/2, y+radius + 4, label_width, 10, bg_color);

	lcd.setTextSize(1);
	lcd.setTextColor(fg_color);
	lcd.setCursor(x - label.length()*3, y + radius + 5);
	lcd.print(label);

}

bool Dial::receive_move() {
	// Receive moves if a touch started on the element
	return is_touching;
}

void Dial::touch_down(const int &tx, const int &ty) {
	drag_start_x = tx;
	drag_start_y = ty;
	drag_start_value = value;
	smooth_delta_x = 0;
	smooth_delta_y = 0;
}

void Dial::touch_move(const int &tx, const int &ty) {
	smooth_delta_x = smooth_delta_x*0.97 + (tx - drag_start_x)*0.03;
	smooth_delta_y = smooth_delta_y*0.97 + (drag_start_y - ty)*0.03;

	double delta_x = smooth_delta_x * 0.01 * (data_max - data_min); // 1000 pixels is 1 (full dial range)
	double delta_y = smooth_delta_y * 0.01 * (data_max - data_min);  // 100 pixels is 1

	set_value(drag_start_value + delta_x + delta_y);
}
