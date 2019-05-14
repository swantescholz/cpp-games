#include "CSurface.h"


CSurface::CSurface() {
	avVertices[0] = Roe::Vector3(-0.5,0.0, 0.5);
	avVertices[1] = Roe::Vector3( 0.5,0.0, 0.5);
	avVertices[2] = Roe::Vector3( 0.5,0.0,-0.5);
	avVertices[3] = Roe::Vector3(-0.5,0.0,-0.5);
	size = Roe::Vector2(1,1);
	dir  = Roe::Vector3(0,0,1);
	normal  = Roe::Vector3(0,1,0);
	pos     = Roe::Vector3(0,0,0);
}

CSurface::~CSurface() {
	
}

void CSurface::render() {
	glBegin(GL_QUADS);
		glNormal3fv(normal.rg);
		glTexCoord2i(0,0); glVertex3fv(avVertices[0].rg);
		glTexCoord2i(1,0); glVertex3fv(avVertices[1].rg);
		glTexCoord2i(1,1); glVertex3fv(avVertices[2].rg);
		glTexCoord2i(0,1); glVertex3fv(avVertices[3].rg);
	glEnd();
}

void CSurface::update() {
	static Roe::Vector3 av[4] = {
		Roe::Vector3( 0.5,0.0,-0.5),
		Roe::Vector3(-0.5,0.0,-0.5),
		Roe::Vector3(-0.5,0.0, 0.5),
		Roe::Vector3( 0.5,0.0, 0.5)};
	Roe::Matrix mTransform = Roe::Matrix::IDENTITY;
	
	mTransform =  Roe::Matrix::rotationFromTo(Roe::Vector3::Z, dir);
	normal = Roe::Vector3::makePerpendicularTo(normal, dir);
	const float angle = Roe::Vector3::angleBetween(normal, Roe::Vector3::makePerpendicularTo(Roe::Vector3::Y, dir));
	
	Roe::Plane plane = Roe::Plane::createByPoints(Roe::Vector3::ZERO, dir, Roe::Vector3::Y);
	mTransform *= Roe::Matrix::rotationAxis(dir, Roe::Util::algebraicSign(plane.dotCoords(normal)) *  angle);
	mTransform *= Roe::Matrix::translation(pos);
	
	for(int i = 0; i < 4; i++) {
		avVertices[i] = Roe::Matrix::transformCoords(av[i]*Roe::Vector3(size.x,1.0,size.y), mTransform);
	}
}
