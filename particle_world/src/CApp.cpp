#include "CApp.h"

#define ROOM_SIZE 10000

CApp::CApp()
{
	m_iNumParticles = 1000;
	m_fCamSpeed    = 12.0f;
	m_fCamRotSpeed = 60.0f;
	m_fFPS = 0.0f;
	m_fCoordSystemArrowSize = 1.0f;
	m_bShow2DText = true;
	m_vGravity = Vector3(0,-9.81,0);
	m_pParticles = nullptr;
}


CApp::~CApp()
{
	if (m_pParticles != nullptr) {
		for (int i = 0; i < m_iNumParticles; i++) {
			delete m_pParticles[i];
		}
		delete []m_pParticles;
	}
	m_ffg.clear();
}



void CApp::run() {
	
	loadInputFile("input.txt");
	SDL_GL *pgl = SDL_GL::getSingletonPtr();
	pgl->init(800,600,"particle_world","particle_world", "icon.tga", SDL_GL::F_LIGHTING | SDL_GL::F_ALPHA_BLENDING |
	                                               SDL_GL::F_TEXTURE2D);// | SDL_GL::F_FULLSCREEN); // | SDL_GL::F_CULL_SHOW_CCW
	
	glEnable(GL_NORMALIZE);
	
	pgl->setClearColor(Color(0.5,0.7,0.9));
	Font::init("res/fonts/", ".ttf");
	Font::loadTTF("Ubuntu-R", 8,66);
	Font fUb("Ubuntu-R", 20);
	Texture::init("res/textures/", ".png");
	Texture::loadTexture("three");
	Texture::loadTexture("ground");
	Texture::loadTexture("axiom");
	Texture::loadTexture("player");
	Texture::loadTexture("env_map_up");
	Texture::loadTexture("env_map_dn");
	Texture::loadTexture("env_map_fr");
	Texture::loadTexture("env_map_bk");
	Texture::loadTexture("env_map_lf");
	Texture::loadTexture("env_map_rt");
	Texture::loadTexture("particles/fire");
	Texture::loadTexture("particles/sqfire");
	Texture::loadTexture("particles/snow");
	Model::init("res/models/", ".off");
	Model::loadOFF("arrow" ,true, CW_CW, 25.0f);
	Model::loadOFF("arrow2",true, CW_CW, 25.0f);
	Model::loadOFF("sphere"     , true, CW_CW ,15.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("capsule"    , true, CW_CW ,15.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("icosphere"  , true, CW_CW ,15.0f, Matrix::scaling(Vector3(1)));
	Model::loadOFF("cuboid"     , true, CW_CW , 0.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("cylinder"   , true, CW_CW ,15.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("torus"      , true, CW_CW ,15.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("tetrahedron", true, CW_CCW, 0.0f, Matrix::scaling(Vector3(1)));
	Model::loadOFF("ufo"        , true, CW_CW ,15.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("tank"       , true, CW_CW , 0.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("monkey"     , true, CW_CW , 0.0f, Matrix::scaling(Vector3(1,1,1)));
	Model::loadOFF("enviroment" , true, CW_CCW, 0.0f, Matrix::scaling(Vector3(12,12,12)));
	
	Roe::Camera camera(2,Roe::Vector3(10),Roe::Vector3(-1));
	Timer myTimer(2.0);
	
	Light light1(Vector3(0,100000,0), true, true);
	light1.setMaterial(Color(0.2,0.2,0.2,1),Color(1,1,1,1),Color(1,1,1,1));
	Light light2(Vector3(0,-100000,0), true, true);
	light2.setMaterial(Color(0.2,0.2,0.2,1),Color(1,1,1,1),Color(1,1,1,1));
	Light::lightModelAmb(Color(0.2,0.2,0.2,1).rg);
	Timer::updateAll(); //->start elapsed low
	
	while (pgl->mouseQuit() == false && pgl->keyWasDown(KC_ESCAPE) == false && pgl->keyWasDown(KC_F4) == false) {
		
		if (pgl->mouseWasDown(MB_R)) {pgl->toggleFullscreen();}
		
		pgl->clearBuffers();
		pgl->resetModelViewMatrix();
		pgl->updateEvents();
		Roe::Timer::updateAll();
		if (1.0f/Timer::getElapsed() < m_fMinFPS && pgl->getCurrentTime() > 5.5f) break; //breaking program because of FPS
		
		if (myTimer.expired()) {myTimer.restart(); m_fFPS = 1.0f/Timer::getElapsed(); Font::clearStringTextureMemory();}
		if      (pgl->keyIsDown(KC_ARR_LEFT )) {camera.yaw  (Timer::getElapsed() * -ROE_DEG_TO_RAD(m_fCamRotSpeed));}
		else if (pgl->keyIsDown(KC_ARR_RIGHT)) {camera.yaw  (Timer::getElapsed() *  ROE_DEG_TO_RAD(m_fCamRotSpeed));}
		if      (pgl->keyIsDown(KC_ARR_UP   )) {camera.pitch(Timer::getElapsed() *  ROE_DEG_TO_RAD(m_fCamRotSpeed));}
		else if (pgl->keyIsDown(KC_ARR_DOWN )) {camera.pitch(Timer::getElapsed() * -ROE_DEG_TO_RAD(m_fCamRotSpeed));}
		if      (pgl->keyIsDown(KC_C        )) {camera.pos+= Vector3::normalize(camera.dir)*(Timer::getElapsed() *  m_fCamSpeed);}
		else if (pgl->keyIsDown(KC_X        )) {camera.pos+= Vector3::normalize(camera.dir)*(Timer::getElapsed() * -m_fCamSpeed);}
		if (pgl->keyIsDown(KC_X) || pgl->keyIsDown(KC_C)) {camera.up = Vector3(0,1,0);}
		if (pgl->keyWasDown(KC_R)) loadInputFile("input.txt");           //reload
		if (pgl->keyWasDown(KC_P)) pgl->saveScreenshot("res/screenshots/screenshot"); //take a screenshot
		//pgl->saveScreenshot("res/screenmovie/screenshot" + Util::addZeros(Util::toString(Util::getNextNumber()), 4) + ".bmp"); //take a screenmovie

		
		pgl->applyCameraToScene(camera);
		light1.light();
		light2.light();

		Material::WHITE.render();
		
		pgl->renderSkyBox(Vector3(165536.0), camera.pos, "env_map");
		
		glDisable(GL_LIGHTING);
		
		drawArrowVector(Vector3::ZERO, Vector3(1,0,0), Color(1,0,0), Vector3(m_fCoordSystemArrowSize));
		drawArrowVector(Vector3::ZERO, Vector3(0,1,0), Color(0,1,0), Vector3(m_fCoordSystemArrowSize));
		drawArrowVector(Vector3::ZERO, Vector3(0,0,1), Color(0,0,1), Vector3(m_fCoordSystemArrowSize));
		
		//PARTICLES in force fields
		if (roe_rand_unit < 0.02) {
			//cout << Util::distancePointToRay(Vector3(-2,-6,1), Vector3(5,9,1), Vector3(3,2,2)) << endl;
		}
		glEnable(GL_TEXTURE_2D);
		
		
		static long long numFrames = 0;
		static long double ldTime = 0.0;
		static double average = -1.0;
		static Timer frameTimer(1.0);
		Particle::setElapsed(Timer::getElapsed());
		Util::timeDiff();
		for (int i = 0; i < m_iNumParticles; i++) {
			Vector3 acc = m_ffg.influence(m_pParticles[i]->getPos(), m_pParticles[i]->getVel());
			m_pParticles[i]->setAcc(acc);
			m_pParticles[i]->update();
		}
		ldTime += Util::timeDiff();
		++numFrames;
		if (frameTimer.expired()) {
			average = (ldTime/numFrames)*1000;
			frameTimer.restart();
		}
		
		
		Particle::sortZ(m_pParticles, m_iNumParticles, camera.pos);
		Texture("particles/sqfire").bind();
		for (int i = m_iNumParticles-1; i >= 0; --i) {
			m_pParticles[i]->render();
		}
		glDisable(GL_TEXTURE_2D);
		//PARTICESL END*/
		glEnable(GL_LIGHTING);
		glColor4fv(Color(.9,.7,.5,.3).rg);
		Material(Color(.9,.7,.5,.3),Color(.9,.7,.5,.3),Color(.9,.7,.5,.3)).render();
		
		for (auto it = m_ffg.begin(); it != m_ffg.end(); ++it) {
			drawForceField(*(*it));
		}
		
		if (m_bShow2DText) {
			Font::begin2D();
				Vector2 wh(0,0), pos(0,pgl->getWindowH() - 20);
				Color col(.9,.7,.5,.8);
				fUb.renderText2("FPS: " + Util::toString(m_fFPS), col, pos, &wh, false); pos.y -= wh.y;
				fUb.renderText2(           camera.pos.toString(), col, pos, &wh, false); pos.y -= wh.y;
				fUb.renderText2("Update(msec):" + Util::toString(average), col, pos, &wh, false); pos.y -= wh.y;
			Font::end2D();
		}
		if(pgl->keyIsDown(KC_W))
			pgl->applyBlackWhiteFilter();
		pgl->flip();
	}

	Font::quit();
	Texture::quit();
	Model::quit();
	
	string tmp = "Ticks: " + Util::toString((int)(Timer::getTimeSinceInit()*1000));
	pgl->quit();
	cout << endl << tmp << endl;
	return;
	
}

void CApp::loadInputFile(string sFile) {
	if (m_pParticles != nullptr) { //cleaning up first
		for (int i = 0; i < m_iNumParticles; i++) {
			delete m_pParticles[i];
		}
		delete[] m_pParticles;
	}
	//textfile rloading
	string sLine;
	Textfile tf(sFile);
	tf.readFile();
	tf.setText(Util::deleteComments(tf.getText(), "//", "/*", "*/", true)); //deleting comments
	sLine = tf.getLine(0);
	loadWorldFile(sLine); //particle force field world loading...
	tf.popFront();
	Tokenizer tok(tf.getText());
	m_iNumParticles         = tok.readInt(); tok.skipLine();
	Vector3 pos, dir;
	float rand1, rand2;
	pos.x            = tok.readFloat();
	pos.y            = tok.readFloat();
	pos.z            = tok.readFloat();
	rand1            = tok.readFloat(); tok.skipLine();
	dir.x            = tok.readFloat();
	dir.y            = tok.readFloat();
	dir.z            = tok.readFloat();
	rand2            = tok.readFloat(); tok.skipLine();
	const float w           = tok.readFloat();
	const float h           = tok.readFloat(); tok.skipLine();
	m_fCamSpeed             = tok.readFloat(); tok.skipLine();
	m_fCamRotSpeed          = tok.readFloat(); tok.skipLine();
	m_fCoordSystemArrowSize = tok.readFloat(); tok.skipLine();
	m_vGravity.x            = tok.readFloat();
	m_vGravity.y            = tok.readFloat();
	m_vGravity.z            = tok.readFloat(); tok.skipLine();
	m_bShow2DText           = tok.readBool(); tok.skipLine();
	m_fMinFPS               = tok.readFloat(); tok.skipLine();
	
	m_pParticles = new Particle*[m_iNumParticles];
	for(int i = 0; i < m_iNumParticles; i++) {
		m_pParticles[i] = new Particle();
		m_pParticles[i]->setLighted(false);
		m_pParticles[i]->setFade(0.0);
		m_pParticles[i]->setLife(0.5);
		m_pParticles[i]->addKeyframeColor(Color::WHITE, 0.0);
		m_pParticles[i]->addKeyframeColor(Color::WHITE, 1.0);
	}
	for(int i = 0; i < m_iNumParticles; i++) {
		m_pParticles[i]->setWH(Vector2(w,h));
		m_pParticles[i]->start(pos+Vector3::createRandomUnit()*roe_random.uniformReal(0.0f,rand1),
		                       dir+Vector3::createRandomUnit()*roe_random.uniformReal(0.0f,rand2));
	}
	Timer::updateAll();
	
}

void CApp::loadWorldFile(string sFile) {
	//textfile rloading
	string sLine;
	Textfile tf(sFile);
	tf.readFile();
	tf.setText(Util::deleteComments(tf.getText(), "//", "/*", "*/", true)); //deleting comments
	
	m_ffg.clear();
	ForceField *ff = nullptr;
	vector<string> vs;
	
	while(tf.length() > 0) {
		sLine = tf.getLine(0);
		tf.popFront();
		if (Util::stringBeginsWith(sLine, "sphere") ||
			Util::stringBeginsWith(sLine, "box") ||
			Util::stringBeginsWith(sLine, "cylinder") ||
			Util::stringBeginsWith(sLine, "torus")) {
			vs.clear();
			vs.push_back(sLine);
			while(tf.getLine(0)[0] == '\t') {
				vs.push_back( tf.getLine(0).substr(1) );
				tf.popFront();
				if (tf.length() <= 0) break;
			}
			ff = loadForceField(vs);
			m_ffg.addForceField(*ff);
		}
	}
}

ForceField* CApp::loadForceField(vector<string> vsText) {
	
	ForceField *ff = new ForceField();
	Matrix mTransform;
	Tokenizer tok;
	string type = vsText[0];
	if (Util::stringBeginsWith(type, "sphere"))   ff->setBehaviourCollision(ffbcSphere());
	if (Util::stringBeginsWith(type, "box"))      ff->setBehaviourCollision(ffbcBox());
	if (Util::stringBeginsWith(type, "cylinder")) ff->setBehaviourCollision(ffbcCylinder());
	if (Util::stringBeginsWith(type, "torus")) {
		tok.setText(type.substr(6));
		double rSmall = tok.readDouble();
		ff->setBehaviourCollision(ffbcTorus(rSmall));
	}
	vector<string> vs;
	for (int i = 1; i < (int)vsText.size(); i++) {
		if (Util::stringBeginsWith(vsText[i],"matrix") || Util::stringBeginsWith(vsText[i],"direction") ||
			Util::stringBeginsWith(vsText[i],"strength")) {
			vs.clear();
			for (int j = i+1; j<(int)vsText.size() && vsText[j][0] == '\t'; j++) {
				vs.push_back(vsText[j].substr(1));
			}
		}
		if (Util::stringBeginsWith(vsText[i],"matrix")) {
			
			mTransform = loadMatrix(vs);
			vs.clear();
			ff->setTransform(mTransform);
		}
		if (Util::stringBeginsWith(vsText[i],"direction")) {
			loadDirection(vs, ff);
		}
		if (Util::stringBeginsWith(vsText[i],"strength")) {
			loadStrength(vs, ff);
		}
	}
	
	return ff;
}

Matrix CApp::loadMatrix(vector<string> vsText) {
	Matrix m = Matrix::IDENTITY;
	double d1, d2, d3, d4, d5, d6;
	for (int i = 0; i < (int)vsText.size(); i++) {
		if (Util::stringBeginsWith(vsText[i], "translation")) {
			Tokenizer tok(vsText[i].substr(string("translation").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			m *= Matrix::translation(Vector3(d1,d2,d3));
		}
		if (Util::stringBeginsWith(vsText[i], "scaling")) {
			Tokenizer tok(vsText[i].substr(string("scaling").length()));
			d1 = tok.readDouble(); tok.readDouble(d2=d1); tok.readDouble(d3=d1);
			m *= Matrix::scaling(Vector3(d1,d2,d3));
		}
		if (Util::stringBeginsWith(vsText[i], "rotationAxis")) {
			Tokenizer tok(vsText[i].substr(string("rotationAxis").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble(); d4 = tok.readDouble();
			m *= Matrix::rotationAxis(Vector3(d1,d2,d3), d4);
		}
		else if (Util::stringBeginsWith(vsText[i], "rotationFromTo")) {
			Tokenizer tok(vsText[i].substr(string("rotationFromTo").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			d4 = tok.readDouble(); d5 = tok.readDouble(); d6 = tok.readDouble();
			m *= Matrix::rotationFromTo(Vector3(d1,d2,d3), Vector3(d4,d5,d6));
		}
		else if (Util::stringBeginsWith(vsText[i], "rotation")) {
			Tokenizer tok(vsText[i].substr(string("rotation").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			m *= Matrix::rotation(Vector3(d1,d2,d3));
		}
	}
	return m;
}

void CApp::loadDirection(vector<string> vsText, ForceField *ff) {
	double d1, d2, d3, d4, d5, d6, d7;
	std::string sText;
	for (int i = 0; i < (int)vsText.size(); ++i) {
		sText = vsText[i];
		if (Util::stringBeginsWith(sText, "constant")) {
			Tokenizer tok(sText.substr(string("constant").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			ff->setBehaviourDir(ffbdConstant(Vector3(d1,d2,d3)));
		}
		else if (Util::stringBeginsWith(sText, "pointCentered")) {
			Tokenizer tok(sText.substr(string("pointCentered").length()));
			tok.readDouble(d1 = 0.0); tok.readDouble(d2 = 0.0); tok.readDouble(d3 = 0.0);
			ff->setBehaviourDir(ffbdPointCentered(Vector3(d1,d2,d3)));
		}
		else if (Util::stringBeginsWith(sText, "lineCentered")) {
			Tokenizer tok(sText.substr(string("lineCentered").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			d4 = tok.readDouble(); d5 = tok.readDouble(); d6 = tok.readDouble();
			ff->setBehaviourDir(ffbdLineCentered(Vector3(d1,d2,d3), Vector3(d4,d5,d6)));
		}
		else if (Util::stringBeginsWith(sText, "planeCentered")) {
			Tokenizer tok(sText.substr(string("planeCentered").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			d4 = tok.readDouble(); d5 = tok.readDouble(); d6 = tok.readDouble();
			ff->setBehaviourDir(ffbdPlaneCentered(Plane(Vector3(d1,d2,d3), Vector3(d4,d5,d6))));
		}
		else if (Util::stringBeginsWith(sText, "torusCentered")) {
			Tokenizer tok(sText.substr(string("torusCentered").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			tok.readDouble(d4 = 0.0); tok.readDouble(d5 = 1.0); tok.readDouble(d6 = 0.0); tok.readDouble(d7 = 1.0);
			ff->setBehaviourDir(ffbdTorusCentered(Vector3(d1,d2,d3), Vector3(d4,d5,d6), d7));
		}
		else if (Util::stringBeginsWith(sText, "resistance")) {
			Tokenizer tok(sText.substr(string("resistance").length()));
			ff->setBehaviourDir(ffbdResistance());
		}
		else if (Util::stringBeginsWith(sText, "cross")) {
			Tokenizer tok(sText.substr(string("cross").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			ff->setBehaviourDir(ffbdCross(Vector3(d1,d2,d3)));
		}
		else if (Util::stringBeginsWith(sText, "randomOrtho")) {
			Tokenizer tok(sText.substr(string("randomOrtho").length()));
			ff->setBehaviourDir(ffbdRandomOrtho());
		}
		else continue;
		return;
	}
}

void CApp::loadStrength(vector<string> vsText, ForceField *ff) {
	double d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
	std::string sText;
	for (int i = 0; i < (int)vsText.size(); ++i) {
		sText = vsText[i];
		if (Util::stringBeginsWith(sText, "constant")) {
			Tokenizer tok(sText.substr(string("constant").length()));
			d1 = tok.readDouble();
			ff->setBehaviourStrength(ffbsConstant(d1));
		}
		else if (Util::stringBeginsWith(sText, "distPoint")) {
			Tokenizer tok(sText.substr(string("distPoint").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); tok.readDouble(d3 = 1.0);
			tok.readDouble(d4 = 0.0); tok.readDouble(d5 = 0.0); tok.readDouble(d6 = 0.0);
			ff->setBehaviourStrength(ffbsDistPoint(d1,d2,d3, Vector3(d4,d5,d6)));
		}
		else if (Util::stringBeginsWith(sText, "distLine")) {
			Tokenizer tok(sText.substr(string("distLine").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			d4 = tok.readDouble(); d5 = tok.readDouble(); d6 = tok.readDouble();
			d7 = tok.readDouble(); d8 = tok.readDouble(); tok.readDouble(d9 = 1.0);
			ff->setBehaviourStrength(ffbsDistLine(d1,d2, Vector3(d3,d4,d5), Vector3(d6,d7,d8), d9));
		}
		else if (Util::stringBeginsWith(sText, "distPlane")) {
			Tokenizer tok(sText.substr(string("distPlane").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble();
			d4 = tok.readDouble(); d5 = tok.readDouble(); d6 = tok.readDouble();
			d7 = tok.readDouble(); d8 = tok.readDouble(); tok.readDouble(d9 = 1.0);
			ff->setBehaviourStrength(ffbsDistPlane(d1,d2, Plane(Vector3(d3,d4,d5), Vector3(d6,d7,d8)), d9));
		}
		else if (Util::stringBeginsWith(sText, "distTorus")) {
			Tokenizer tok(sText.substr(string("distTorus").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble(); d3 = tok.readDouble(); tok.readDouble(d4 = 1.0);
			tok.readDouble(d5 = 0.0); tok.readDouble(d6 = 0.0); tok.readDouble(d7 = 0.0);
			tok.readDouble(d8 = 0.0); tok.readDouble(d9 = 1.0); tok.readDouble(d10 = 0.0);
			ff->setBehaviourStrength(ffbsDistTorus(d1,d2, d3));
		}
		else if (Util::stringBeginsWith(sText, "dragTurbulence")) {
			Tokenizer tok(sText.substr(string("dragTurbulence").length()));
			d1 = tok.readDouble(); tok.readDouble(d2 = 1.0); tok.readDouble(d3 = 2.0);
			ff->setBehaviourStrength(ffbsDragTurbulence(d1,d2,d3));
		}
		else if (Util::stringBeginsWith(sText, "dragViscosity")) {
			Tokenizer tok(sText.substr(string("dragViscosity").length()));
			d1 = tok.readDouble(); tok.readDouble(d2 = 0.053051647697295);
			ff->setBehaviourStrength(ffbsDragViscosity(d1,d2));
		}
		else if (Util::stringBeginsWith(sText, "randomUniform")) {
			Tokenizer tok(sText.substr(string("randomUniform").length()));
			tok.readDouble(d1 = 0.0); tok.readDouble(d2 = 1.0);
			ff->setBehaviourStrength(ffbsRandomUniform(d1,d2));
		}
		else if (Util::stringBeginsWith(sText, "randomNormal")) {
			Tokenizer tok(sText.substr(string("randomNormal").length()));
			d1 = tok.readDouble(); d2 = tok.readDouble();
			ff->setBehaviourStrength(ffbsRandomNormal(d1,d2));
		}
		else continue;
		return; //std
	}
}

void CApp::drawArrowVector(Vector3 pos, Vector3 dir, Color col, Vector3 scale) {
	bool wasEnabled = glIsEnabled(GL_TEXTURE_2D);
	if (wasEnabled) glDisable(GL_TEXTURE_2D);
	Model arrow("cylinder");
	//dir.normalize();
	arrow.setScale(Vector3(0.028,0.028,dir.length()*0.5f));
	arrow.multScale(scale);
	const float length = (arrow.getBoxMax()-arrow.getBoxMin()).z;
	if (glIsEnabled(GL_LIGHTING))
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col.rg);
	else
		glColor4fv(col.rg);
	arrow.transform(Matrix::rotationX(0.5*ROE_PI));
	arrow.transform(Matrix::rotationFromTo(Vector3::Y, Vector3::normalize(dir)));
	arrow.transform(Matrix::translation(pos+Vector3::normalize(dir)*0.5f*length));
	arrow.render();//z
	if (wasEnabled) glEnable(GL_TEXTURE_2D);
}
void CApp::drawSphere(Vector3 pos, float r) {
	Model mod("sphere",true);
	mod.setScale(Vector3(r));
	mod.transform(Matrix::translation(pos));
	mod.render();
}
void CApp::drawSphere(const Matrix& m) {
	Model mod("sphere",true);
	mod.transform(m);
	mod.render();
}
void CApp::drawBox(Vector3 pos, float size) {
	Model mod("cuboid",true);
	mod.setScale(Vector3(size));
	mod.transform(Matrix::translation(pos));
	mod.render();
}
void CApp::drawForceField(const ForceField& s) {
	string name = "";
	switch(s.getCollisionType()) {
		case 1:  name = "sphere";   break;
		case 2:  name = "cuboid";   break;
		case 3:  name = "cylinder"; break;
		case 4:  name = "torus";    break;
		default: name = "sphere";   break;
	}
	drawModel(name, s.getTransform());
}
void CApp::drawModel(string name, const Matrix& m) {
	Model mod(name,true);
	mod.transform(m);
	mod.render();
}

