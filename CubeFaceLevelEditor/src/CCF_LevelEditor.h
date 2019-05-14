#ifndef CCF_LEVELEDITOR_H_
#define CCF_LEVELEDITOR_H_

#include "GLC.h"
#include "CGame.h"

#define g_pCF_LE CCF_LevelEditor::Get()

class CCF_LevelEditor : public TSingleton<CCF_LevelEditor>
{
public:
	CCF_LevelEditor();
	~CCF_LevelEditor();
	
	void Run();
	void Load(string sPath);
	
private:
	void Update();
	void Render();
	
	void Quit();
	
	bool m_bQuit;
};

#endif /*CCF_LEVELEDITOR_H_*/
