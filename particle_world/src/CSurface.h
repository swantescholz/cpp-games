#ifndef CSURFACE_H_
#define CSURFACE_H_

#include "Roe.h"

class CSurface
{
public:
	CSurface();
	~CSurface();
	
	void render();
	
	Roe::Vector3 getPos   () {return pos;}
	Roe::Vector3 getNormal() {return normal;}
	Roe::Vector3 getDir   () {return dir;}
	Roe::Vector2 getSize  () {return size;}
	void setPos   (Roe::Vector3 v) {pos = v; update();}
	void setNormal(Roe::Vector3 v) {normal = v; /*normal.normalize();*/ update();}
	void setDir   (Roe::Vector3 v) {dir    = v; dir.normalize()   ; update();}
	void setSize  (Roe::Vector2 v) {size = v; update();}
	
private:
	void update();
	
	Roe::Vector3 pos;
	Roe::Vector3 normal;
	Roe::Vector3 dir;
	Roe::Vector3 avVertices[4];
	Roe::Vector2 size; //edge length
};

#endif /* CSURFACE_H_ */
