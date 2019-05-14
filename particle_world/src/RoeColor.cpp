#include "RoeColor.h"

namespace Roe {
	
	const Color Color::ZERO (0,0,0,0);
	const Color Color::BLACK(0,0,0,1);
	const Color Color::WHITE(1,1,1,1);
	const Color Color::RED  (1,0,0,1);
	const Color Color::GREEN(0,1,0,1);
	const Color Color::BLUE (0,0,1,1);
	const Color Color::YELLOW (1,1,0,1);
	const Color Color::PURPLE (1,0,1,1);
	const Color Color::CYAN   (0,1,1,1);
	const Color Color::ORANGE (1,0.5,0,1);
	const Color Color::PINK   (0,0.2667,0.7333);
	const Color Color::SKY    (0.5,0.7,0.9);
	const Color Color::MELLOW (0.9,0.7,0.5);
	const Color Color::FOREST (0.247,0.498,0.373);
	const Color Color::SILVER (0.784314,0.784314,0.784314);
	const Color Color::GOLD   (0.862745,0.745098,0.0     );
	
	void Color::fromColorArrayToByteArray(const Color *colors, unsigned char *bytes, unsigned long num, bool getsAlpha) {
		const int bpp = getsAlpha ? 4:3;
		for (unsigned long int i = 0; i < num; i++) {
			bytes[i*bpp+0] = (unsigned char)(colors[i].r*255);
			bytes[i*bpp+1] = (unsigned char)(colors[i].g*255);
			bytes[i*bpp+2] = (unsigned char)(colors[i].b*255);
			if(getsAlpha) bytes[i*bpp+3] = (unsigned char)(colors[i].a*255);
		}
	}
	void Color::fromByteArrayToColorArray(const unsigned char *bytes, Color *colors, unsigned long num, bool hasAlpha) {
		const int bpp = hasAlpha ? 4:3;
		for (unsigned long int i = 0; i < num; i++) {
			colors[i].r = bytes[i*bpp+0]/255.0f;
			colors[i].g = bytes[i*bpp+1]/255.0f;
			colors[i].b = bytes[i*bpp+2]/255.0f;
			colors[i].a = hasAlpha ? bytes[i*bpp+3] : 1.0f;
		}
	}
	
} // namespace Roe
