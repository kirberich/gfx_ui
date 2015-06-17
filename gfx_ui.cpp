#include <gfx_ui.h>

GFX_UI::GFX_UI() : lcd(ILI9341_t3(10, 9)), touch_screen(UTouch(15, 16, 17, 18, 19)) {
}

void GFX_UI::init(Color bg_color, byte rotation /* = 0 */	) {
	// Rotation can be 0, 1, 2, 3, corresponding to 0, 90, 180 and 270 degrees
	// The touch screen library only accepts rotation to either portrait or landscape (default)
	// So everything else is done when interpreting the touch coordinates
	touch_screen.InitTouch(rotation == 0 || rotation == 2 ? PORTRAIT : LANDSCAPE);
	touch_screen.setPrecision(4);

	if (rotation == 0 || rotation == 3) {
		// for 0 and 270 degrees the touch values need to be flipped.
		flip_touch = true;
	}

	// We're conveniently using the same values for rotation as the adafruit library, so there's nothing special to do here
	lcd.begin();
	lcd.setRotation(rotation);
	update(true);
}

void GFX_UI::add(UIElement &element) {
	elements[num_elements] = &element;
	num_elements++;
	element.draw(lcd);
}

uint16_t GFX_UI::width() {
	return lcd.width();
}

uint16_t GFX_UI::height() {
	return lcd.height();
}

void GFX_UI::set_bg_color(Color new_bg_color) {
	bg_color = new_bg_color;
	update(true);
}

UIElement* GFX_UI::get_element_by_name(String name) {
	for(byte i = 0; i<num_elements; ++i) {
		if (elements[i]->get_name() == name) {
			return elements[i];
		}
	}
	return nullptr;
}

ILI9341_t3& GFX_UI::get_lcd() {
	return lcd;
}

void GFX_UI::update(bool reset) {
	if (reset) {
		lcd.fillScreen(bg_color);
	}
	for(byte i = 0; i<num_elements; ++i) {
		if (reset || elements[i]->needs_update()) {
			elements[i]->update(lcd, reset);
		}
	}
}

void GFX_UI::update() {
	update(false);
}

void GFX_UI::handle_touch() {
	unsigned long now = millis();
	touch_screen.read();
	int x = -1;
	int y = -1;

	if (touch_screen.dataAvailable()) {
		x = touch_screen.getX();
		y = touch_screen.getY();

		if (flip_touch) {
			x = width() - x;
			y = height() - y;
		}

		last_x = x;
		last_y = y;
	}

	if (x != -1 && y != -1) {
		last_touch = now;

		if (!is_touching) {
			is_touching = true;
			handle_touch_down(x, y);
		} else {
			handle_touch_move(x, y);
		}
	} else if (is_touching && now - last_touch > 100){
		handle_touch_up(last_x, last_y);
		is_touching = false;
	}
}

void GFX_UI::handle_touch_down(const int &tx, const int &ty) {
	for(byte i = 0; i<num_elements; ++i) {
		if(elements[i]->contains(tx, ty)) {
			elements[i]->set_is_touching(true);
			elements[i]->touch_down(tx, ty);
			elements[i]->update(lcd);
		}
	}
}

void GFX_UI::handle_touch_up(const int &tx, const int &ty) {
	for(byte i = 0; i<num_elements; ++i) {
		if (elements[i]->get_is_touching()) {
			elements[i]->set_is_touching(false);
			elements[i]->touch_up(tx, ty);
			elements[i]->update(lcd);
		}
	}
}

void GFX_UI::handle_touch_move(const int &tx, const int &ty) {
	for(byte i = 0; i<num_elements; ++i) {
		if (elements[i]->receive_move()) {
			elements[i]->touch_move(tx, ty);
			elements[i]->update(lcd);
		}
	}
}
