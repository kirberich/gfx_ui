#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <gfx_ui.h>

TouchScreen ts = TouchScreen(6, A1, A2, 7, 300);
Adafruit_TFTLCD tft(A3, A2, A1, A0, A4);
GFX_UI ui(tft, ts, 179, 145, 856, 920);

uint16_t text_color = rgb(100, 100, 200);
uint16_t button_color = rgb(0, 0, 100);

uint16_t rgb(byte red, byte green, byte blue) {
  return (((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255);
}

void callback1() {
}
Button button1 = Button(40, 40, tft.width()-80, 40, "Button 1", callback1, button_color, text_color);

Label header = Label(0, 0, tft.width(), 20, "Testing", 2, 0, text_color);

void setup(void) {
  Serial.begin(9600);

  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);

  ui.add(button1);
  ui.add(header);
}

void loop() {
	ui.handle_touch();
	ui.update();
}
