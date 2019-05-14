#include "RoeMaterial.h"

namespace Roe {
	
	const Material Material::BLACK      (Color(0,0,0), Color(0,0,0), Color(0,0,0), 0);
	const Material Material::WHITE      (Color(0,0,0), Color(1,1,1), Color(1,1,1), 64);
	const Material Material::WHITE_AMB  (Color(1,1,1), Color(1,1,1), Color(1,1,1), 64);
	const Material Material::RED        (Color(0,0,0), Color(1,0,0), Color(1,0,0), 64);
	const Material Material::RED_AMB    (Color(1,0,0), Color(1,0,0), Color(1,0,0), 64);
	const Material Material::GREEN      (Color(0,0,0), Color(0,1,0), Color(0,1,0), 64);
	const Material Material::GREEN_AMB  (Color(0,1,0), Color(0,1,0), Color(0,1,0), 64);
	const Material Material::BLUE       (Color(0,0,0), Color(0,0,1), Color(0,0,1), 64);
	const Material Material::BLUE_AMB   (Color(0,0,1), Color(0,0,1), Color(0,0,1), 64);
	const Material Material::PURPLE     (Color(0,0,0), Color(1,0,1), Color(1,0,1), 64);
	const Material Material::PURPLE_AMB (Color(1,0,1), Color(1,0,1), Color(1,0,1), 64);
	const Material Material::YELLOW     (Color(0,0,0), Color(1,1,0), Color(1,1,0), 64);
	const Material Material::YELLOW_AMB (Color(1,1,0), Color(1,1,0), Color(1,1,0), 64);
	const Material Material::CYAN       (Color(0,0,0), Color(0,1,1), Color(0,1,1), 64);
	const Material Material::CYAN_AMB   (Color(0,1,1), Color(0,1,1), Color(0,1,1), 64);
	const Material Material::ORANGE     (Color(0,0,0)  , Color(1,0.5,0), Color(1,0.5,0), 64);
	const Material Material::ORANGE_AMB (Color(1,0.5,0), Color(1,0.5,0), Color(1,0.5,0), 64);
	const Material Material::PINK       (Color(0,0,0)          , Color(0,0.2667,0.7333), Color(0,0.2667,0.7333), 64);
	const Material Material::PINK_AMB   (Color(1,0.2667,0.7333), Color(0,0.2667,0.7333), Color(0,0.2667,0.7333), 64);
	const Material Material::SKY        (Color(0,0,0)      , Color(0.5,0.7,0.9), Color(0.5,0.7,0.9), 64);
	const Material Material::SKY_AMB    (Color(0.5,0.7,0.9), Color(0.5,0.7,0.9), Color(0.5,0.7,0.9), 64);
	const Material Material::MELLOW     (Color(0,0,0)      , Color(0.9,0.7,0.5), Color(0.9,0.7,0.5), 64);
	const Material Material::MELLOW_AMB (Color(0.9,0.7,0.5), Color(0.9,0.7,0.5), Color(0.9,0.7,0.5), 64);
	const Material Material::FOREST     (Color(0,0,0)            , Color(0.247,0.498,0.373), Color(0.247,0.498,0.373), 64);
	const Material Material::FOREST_AMB (Color(0.247,0.498,0.373), Color(0.247,0.498,0.373), Color(0.247,0.498,0.373), 64);
	const Material Material::SILVER   (Color(0.2,0.2,0.2), Color(0.784314,0.784314,0.784314), Color(0.784314,0.784314,0.784314), 96);
	const Material Material::GOLD     (Color(0.2,0.2,0.2), Color(0.862745,0.745098,0.0     ), Color(0.862745,0.745098,     0.0), 96);
	static const Material PURPLE;
		static const Material PURPLE_AMB;
		static const Material YELLOW;
		static const Material YELLOW_AMB;
		static const Material ORANGE;
		static const Material ORANGE_AMB;
		static const Material PINK;
		static const Material PINK_AMB;
		static const Material SKY;
		static const Material SKY_AMB;
		static const Material MELLOW;
		static const Material MELLOW_AMB;
		static const Material FOREST;
		static const Material FOREST_AMB;
	
	std::string Material::toString() const {
		char ch[280]; sprintf(ch,"Material( (%f,%f,%f), (%f,%f,%f), (%f,%f,%f), %f)",
		                      a.r,a.g,a.b, b.r,b.g,b.b, c.r,c.g,c.b, s);
		return std::string(ch);
	}
	
} // namespace Roe
