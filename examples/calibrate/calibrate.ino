#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

TouchScreen ts = TouchScreen(6, A1, A2, 7, 300);
Adafruit_TFTLCD lcd(A3, A2, A1, A0, A4);

uint16_t min_x = 10000;
uint16_t min_y = 10000;
uint16_t max_x = 0;
uint16_t max_y = 0;

void setup(void) {
	lcd.setRotation(2);
	lcd.fillScreen(0);

	lcd.setCursor(0, 0);
	lcd.setTextSize(1);
	lcd.setTextColor(0xFFFF, 0);
	lcd.print("Touch every corner of the screen");
	lcd.setCursor(0, 10);
	lcd.print("to get some rough calibration values.");
	lcd.setCursor(0, 20);
	lcd.print("Use these values to instantiate");
	lcd.setCursor(0, 30);
	lcd.print("the UI in your sketch.");
}

void loop() {
	TSPoint p = ts.getPoint();

	if (p.z < 5) {
		return;
	}

	pinMode(A2, OUTPUT);
	pinMode(A1, OUTPUT);

	min_x = min(min_x, p.x);
	min_y = min(min_y, p.y);
	max_x = max(max_x, p.x);
	max_y = max(max_y, p.y);
	lcd.setTextSize(1);
	lcd.setTextColor(0xFFFF, 0);

	lcd.setCursor(0, 100);
	lcd.print("min x ");
	lcd.print(min_x);
	lcd.print(", ");
	lcd.print("min y ");
	lcd.print(min_y);

	lcd.setCursor(0, 110);
	lcd.print("max x ");
	lcd.print(max_x);
	lcd.print(", ");
	lcd.print("max y ");
	lcd.print(max_y);
}
