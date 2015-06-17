#pragma once
struct Color {
	uint16_t red;
	uint16_t green;
	uint16_t blue;

	Color() {
		red = green = blue = 0;
	}

	Color(int red, int green, int blue) {
		this->red = max(0, min(255, red));
		this->green = max(0, min(255, green));
		this->blue = max(0, min(255, blue));
	}

	Color(uint16_t encoded) {
		decode(encoded);
	}

	uint16_t encode() {
		return (((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255);
	}

	uint16_t luminosity() {
		return (red + green + blue) / 3;
	}

	void decode(uint16_t color) {
		red = (color & 0xF800) >> 8;
		green = (color & 0x7E0) >> 3;
		blue = (color & 0x1F) << 3;
	}

	operator uint16_t() { return encode(); }
	Color operator+(Color rhs) {
		return {red + rhs.red, green + rhs.green, blue + rhs.blue};
	}
	Color operator-(Color rhs) {
		return {red - rhs.red, green - rhs.green, blue - rhs.blue};
	}
};
