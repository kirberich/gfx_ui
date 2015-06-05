#include <gfx_ui.h>

void UIElement::update(Adafruit_GFX &lcd) {
	_needs_update = false;
	draw(lcd);
}

bool UIElement::is_active() {
	return is_pressed;
}

bool UIElement::needs_update() {
	return _needs_update;
}

void UIElement::touch_down() {
}

void UIElement::touch_up() {
}

bool UIElement::contains(uint16_t tx, uint16_t ty) {
	return false;
}

GFX_UI::GFX_UI(Adafruit_GFX &lcd, TouchScreen &touch_screen, uint16_t ts_min_x, uint16_t ts_min_y, uint16_t ts_max_x, uint16_t ts_max_y) {
	this->p_lcd = &lcd;
	this->p_touch_screen = &touch_screen;

	// Initialize lcd. Set rotation to 180 degrees, because that that's the only angle the display looks good for me.
	lcd.setRotation(2);
	lcd.fillScreen(0);

	this->width = lcd.width();
	this->height = lcd.height();
	pinMode(13, OUTPUT);

	this->ts_min_x = ts_min_x;
	this->ts_min_y = ts_min_y;
	this->ts_max_x = ts_max_x;
	this->ts_max_y = ts_max_y;
}

void GFX_UI::add(UIElement &element) {
	elements[num_elements] = &element;
	num_elements++;
	element.draw(*p_lcd);
}

void GFX_UI::update(bool update_all) {
	for(byte i = 0; i<num_elements; ++i) {
		if (elements[i]->needs_update()) {
			elements[i]->update(*p_lcd);
		}
	}
}

void GFX_UI::update() {
	update(false);
}

void GFX_UI::handle_touch() {
	TSPoint p = p_touch_screen->getPoint();

	pinMode(A2, OUTPUT);
	pinMode(A1, OUTPUT);

	if (p.z > MINPRESSURE) {
		// scale from 0->1023 to tft.width
		p.x = map(p.x, ts_min_x, ts_max_x, width, 0);
		p.y = map(p.y, ts_min_y, ts_max_y, height, 0);

		// Reverse coordinates, because we'll use the display rotated 180 degrees
		touch_x = width - p.x;
		touch_y = height - p.y;
		touch_pressure = p.z;

		if (!is_touching) {
			handle_touch_down();
		}

		is_touching = true;
		last_touch = millis();
	} else if (is_touching && millis() - last_touch > 100){
		handle_touch_up();
		is_touching = false;
	}
}

void GFX_UI::handle_touch_down() {
	for(byte i = 0; i<num_elements; ++i) {
		if(elements[i]->contains(touch_x, touch_y)) {
			elements[i]->touch_down();
			elements[i]->draw(*p_lcd);
		}
	}
}

void GFX_UI::handle_touch_up() {
	for(byte i = 0; i<num_elements; ++i) {
		if (elements[i]->is_active()) {
			elements[i]->touch_up();
			elements[i]->draw(*p_lcd);
		}
	}
}

Button::Button(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, void (*callback)(), uint16_t bg_color, uint16_t fg_color) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->label = label;
	this->callback = callback;
	this->bg_color = bg_color;
	this->fg_color = fg_color;
}

void Button::draw(Adafruit_GFX &lcd) {
	lcd.drawRect(x, y, width, height, fg_color);
	lcd.drawRect(x+1, y+1, width-2, height-2, bg_color);

	lcd.setCursor(x + width/2 - (label.length()*6)/2, (y+height/2-4));
	lcd.setTextSize(1);

	if (is_pressed) {
		lcd.fillRect(x+2, y+2, width-4, height-4, fg_color);
		lcd.setTextColor(bg_color);
	} else {
		lcd.fillRect(x+2, y+2, width-4, height-4, bg_color);
		lcd.setTextColor(fg_color);
	}

	lcd.print(label);
}

bool Button::contains(uint16_t tx, uint16_t ty) {
	if (tx >= x && tx <= x+width && ty >= y && ty <= y+height) {
		return true;
	}
	return false;
}

void Button::touch_down() {
	is_pressed = true;
}

void Button::touch_up() {
	is_pressed = false;
	callback();
}

void Button::set_label(String label) {
	_needs_update = true;
	this->label = label;
}

Label::Label(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, byte size, uint16_t bg_color, uint16_t fg_color) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->label = label;
	this->size = size;
	this->bg_color = bg_color;
	this->fg_color = fg_color;
}

void Label::draw(Adafruit_GFX &lcd) {
	lcd.fillRect(x, y, width, height, bg_color);

	lcd.setCursor(x + width/2 - (label.length()*6*size)/2, (y+height/2-4*size));
	lcd.setTextSize(size);
	lcd.setTextColor(fg_color);

	lcd.print(label);
}

void Label::set_label(String label) {
	this->label = label;
}
