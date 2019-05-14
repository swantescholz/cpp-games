#include "glcFont.h"

map<string, S_FontIntern> glcFont::c_mpFont;
map<string, S_FontIntern>::iterator glcFont::c_mpi;
pair<string, S_FontIntern> glcFont::c_pair;
string glcFont::c_sPathStart, glcFont::c_sPathEnd = ".ttf";


glcFont::glcFont() {
	m_iSize = -1;
}
glcFont::glcFont(const string &s, int iSize) {
	Load(s, iSize);
}
glcFont::~glcFont() {
	
}

void glcFont::Load(const string &s, int iSize) {
	string sPath = c_sPathStart + s + c_sPathEnd;
	
	c_mpi = c_mpFont.find(sPath);
	if (c_mpi == c_mpFont.end()) {
		throw glcException("angefragtes font noch nicht geladen");
	}
	
	m_sPath = sPath;
	m_iSize = iSize;
}
void glcFont::SetSize(int iSize) {
	c_mpi = c_mpFont.find(m_sPath);
	if (iSize < c_mpi->second.iSizeMin || iSize > c_mpi->second.iSizeMax) {
		throw glcException("fontsize gibt es nicht");
	}
	m_iSize = iSize;
}

void glcFont::RenderText(const std::string& text, const tbColor& col, const tbVector3 &v1, const tbVector3 &v2,
						 const tbVector3 &v3, const tbVector3 &v4, bool bDisableDepthTest, float fOffset) {
	
	glColor4fv(col.c);
	
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	if (bDisableDepthTest)
		glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //self made
	
	
	/*Create some variables.*/
	SDL_Color Color = {255, 255, 255};
	SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(GetFont()), text.c_str(), Color);
	unsigned Texture = 0;
 
	/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, Message->pixels);
	
	//berechne offset matrix
	tbVector3 vOffset = fOffset * tbVector3Normalize(tbVector3Cross(tbVector3Normalize(v2 - v1), tbVector3Normalize(v3 - v1)) );
	
	/*Draw this texture on a quad with the given xyz coordinates.*/
	glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex3fv((v1 + vOffset).c);
		glTexCoord2d(1, 1); glVertex3fv((v2 + vOffset).c);
		glTexCoord2d(1, 0); glVertex3fv((v3 + vOffset).c);
		glTexCoord2d(0, 0); glVertex3fv((v4 + vOffset).c);
	glEnd();
	
	/*Clean up.*/
	glDeleteTextures(1, &Texture);
	SDL_FreeSurface(Message);
	
	glPopAttrib();
}
				


void glcFont::Begin()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //self made
	
	int vPort[4];
  
	glGetIntegerv(GL_VIEWPORT, vPort);
  
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}
void glcFont::End()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glPopAttrib();
}

void glcFont::Quit() {
	for(c_mpi = c_mpFont.begin(); c_mpi!=c_mpFont.end(); ++c_mpi) {
		for (int i = 0; i < static_cast<int> (c_mpi->second.vFont.size()); i++) 
			TTF_CloseFont(c_mpi->second.vFont[i]);
	}
	c_mpFont.clear();
}

void glcFont::LoadTTF(string sPath, int ptsizeMin, int ptsizeMax) {
	if (ptsizeMin > ptsizeMax) {
		throw glcException("ptsizeMin > ptsizeMax");
	}
	
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpFont.find(sPath);
	if (c_mpi == c_mpFont.end()) {
		S_FontIntern sfont;
		sfont.iSizeMin = ptsizeMin;
		sfont.iSizeMax = ptsizeMax;
		for (int i = ptsizeMin; i <= ptsizeMax; i++) {
			sfont.vFont.push_back(TTF_OpenFont(sPath.data(), i));
		}
		
		//map erweitern
		c_pair = make_pair (sPath.data(), sfont);
		c_mpFont.insert(c_pair);
	} else {
		throw glcException("font schon geladen");
	}
}


void glcFont::RenderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B, const double& X, 
const double& Y, const double& Z,  const std::string& Text, tbVector2* pvOut)
{
	/*Create some variables.*/
	SDL_Color Color = {R, G, B};
	SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
	unsigned Texture = 0;
 
	/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, Message->pixels);
 
	/*Draw this texture on a quad with the given xyz coordinates.*/
	glBegin(GL_QUADS);
		/*glTexCoord2d(0, 0); glVertex3d(X, Y, Z);
		glTexCoord2d(1, 0); glVertex3d(X+Message->w, Y, Z);
		glTexCoord2d(1, 1); glVertex3d(X+Message->w, Y+Message->h, Z);
		glTexCoord2d(0, 1); glVertex3d(X, Y+Message->h, Z);*/
		//self-made
		glTexCoord2d(0, 1); glVertex3d(X, Y, Z);
		glTexCoord2d(1, 1); glVertex3d(X+Message->w, Y, Z);
		glTexCoord2d(1, 0); glVertex3d(X+Message->w, Y+Message->h, Z);
		glTexCoord2d(0, 0); glVertex3d(X, Y+Message->h, Z);
	glEnd();
 	
 	//self-made
 	if (pvOut) {
 		pvOut->x = Message->w;
 		pvOut->y = Message->h;
 	}
 	
	/*Clean up.*/
	glDeleteTextures(1, &Texture);
	SDL_FreeSurface(Message);
}





