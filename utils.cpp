#include "utils.h"

uint16_t rgb(byte red, byte green, byte blue) {
  return (((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255);
}

void rgb_components(uint16_t color, byte &red, byte &green, byte &blue) {
	red = (color & 0xF800) >> 8;
	green = (color & 0x7E0) >> 3;
	blue = (color & 0x1F) << 2;
}
