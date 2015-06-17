#include <Adafruit_GFX.h>    // Core graphics library
#include <ILI9341_t3.h>
#include <UTouch.h>
#include <gfx_ui.h>
#include <SPI.h>

GFX_UI ui;

Color text_color = {200, 200, 200};
Color button_color = {80, 80, 80};

void callback1(UIElement& caller) {
	// If caller is needed, cast it like so:
	// Button& call_button = static_cast<Button&>(caller);

	Color new_color = {random(255), random(255), random(255)};
	ui.set_bg_color(new_color);
	Dial* pdial = static_cast<Dial*>(ui.get_element_by_name("dial"));
	pdial->set_value(pdial->get_value()+5);
}

Button button1 = Button("button1", 40, 40, 0, 40, "Button 1", callback1, button_color, text_color);
ToggleButton button2 = ToggleButton("button2", 40, 79, 0, 40, "Button 1", callback1, button_color.encode(), text_color.encode());
Dial dial = Dial("dial", 0, 150, 20, -10, 100, 1, button_color, text_color);

Label header = Label("label", 0, 0, 0, 20, "Testing", 2, 0, text_color.encode());
unsigned long last_frame = millis();

void setup(void) {
	Serial.begin(9600);
	ui.init(0, 1);

	// Set values that might depend on the ui rotation after initialising the ui.
	button1.set_width(ui.width()-80);
	button2.set_width(ui.width()-80);
	header.set_width(ui.width());
	dial.set_x(ui.width()/2);

	// Draw pretty background, before adding (and thereby drawing) elements
	for (int y=0; y<ui.get_lcd().height(); y++) {
		ui.get_lcd().drawLine(0, y, ui.get_lcd().width(), y, Color({y-150, y-150, y-150}));
	}

	ui.add(button1);
	ui.add(button2);
	ui.add(header);
	ui.add(dial);
}

void loop() {
	ui.handle_touch();
	ui.update();
}
