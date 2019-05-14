
//main von CF_LevelEditor

#include "CCF_LevelEditor.h"



int main (int argc, char* argv[]) {
	try {
		g_pGS->m_sArg0 = argv[0];
		g_pGS->m_sArg1 = argv[argc-1];
		if (g_pGS->m_sArg1[0] == '-')
			g_pGS->m_sArg1.erase(0,1);
		int iTmp = 0;
		for (;;) {
			iTmp = g_pGS->m_sArg0.find("\\");
			if (iTmp != string::npos)
				g_pGS->m_sArg0.replace(iTmp,1,"/");
			else
				break;
		}
		for (;;) {
			iTmp = g_pGS->m_sArg1.find("\\");
			if (iTmp != string::npos)
				g_pGS->m_sArg1.replace(iTmp,1,"/");
			else
				break;
		}
			
		CCF_LevelEditor::Get();
		
		if ( argc != 2)
			throw glcException("zu wenige oder zu viele argumente");
		
		g_pCF_LE->Load(g_pGS->m_sArg1);
		g_pCF_LE->Run();
		CCF_LevelEditor::Del();
	}
	catch (const glcException &ex) {
		g_Logfile << ex;
		ex.Out();
	}
	
	return 0;
}
