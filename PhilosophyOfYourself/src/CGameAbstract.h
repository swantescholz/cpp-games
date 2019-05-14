#ifndef CGAMEABSTRACT_H_
#define CGAMEABSTRACT_H_

//an abstract game class just for updating and rendering
class CGameAbstract
{
public:
	CGameAbstract();
	virtual ~CGameAbstract() = 0;
	
	virtual void Update() = 0;
	virtual void Render() = 0;
};

#endif /*CGAMEABSTRACT_H_*/

