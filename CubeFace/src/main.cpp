//main
//CubeFace 3-D 2d


#include "CCubeFace.h"

using namespace std;

int main (int argc, char **argv) {
	try {
		g_pCubeFace->Run();
		CCubeFace::Del();
	}
	catch (const glcException &ex) {
		g_Logfile << ex;
		ex.Out();
	}
	
	return 0;
}