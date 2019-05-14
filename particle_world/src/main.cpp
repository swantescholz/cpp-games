
#include "CApp.h"
#include <iostream>
using namespace std;
using namespace Roe;


int main(int argc, char **argv) {
	CApp *app = NULL;
	
	try {
		
			
		// Create application object
		app = new CApp();
		
		app->run();
	
	}
	catch(const Roe::Exception& e){
		roe_log << e;
		cout << e.toString() << endl;
	}
	
	if (app != NULL) delete app;
	
	Util::println("right quitting!!!");
	
	return 0;
	
}
//*/

