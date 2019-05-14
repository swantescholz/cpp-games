#include "RoeFont.h"
#include "RoeException.h"

namespace Roe {

	std::map <std::string, S_FontIntern>           Font::s_mpFont;
	std::map <std::string, S_FontIntern>::iterator Font::s_mpi;
	std::pair<std::string, S_FontIntern>           Font::s_pair;
	std::string Font::s_sPathStart, Font::s_sPathEnd = ".ttf";
	std::map <std::string, S_FontInternStringTextureMemory>           Font::s_mpStringTextureMemory;
	
	Font::Font() {
		m_iSize = -1;
	}
	Font::Font(const std::string &s, int iSize) {
		load(s, iSize);
	}
	Font::~Font() {
		
	}
	
	void Font::load(const std::string &s, int iSize) {
		m_sName = s;
		std::string sPath = s_sPathStart + s + s_sPathEnd;
		
		s_mpi = s_mpFont.find(sPath);
		if (s_mpi == s_mpFont.end()) {
			throw Exception("requested font (" + s + ") not yet loaded", "load", "RoeFont.cpp", ROE_LINE);
		}
		
		m_sPath = sPath;
		
		if (iSize < s_mpi->second.iSizeMin) {
			throw Exception("requested size(" + Util::toString(iSize) + ") of font (" + s + ") does not exist", "load", "RoeFont.cpp", ROE_LINE);
		}
		m_iSize = iSize;
	}
	void Font::setSize(int iSize) {
		s_mpi = s_mpFont.find(m_sPath);
		if (iSize < s_mpi->second.iSizeMin || iSize > s_mpi->second.iSizeMax) {
			throw Exception("required fontsize(" + Util::toString(iSize) + ") does not exist", "setSize", "RoeFont.cpp", ROE_LINE);
		}
		m_iSize = iSize;
	}
	
	Vector2 Font::calcTextSize(std::string sText, int textSize) {
		if (textSize < 0) textSize = m_iSize;
		const int ciOldSize = m_iSize;
		m_iSize = textSize;
		int w, h;
		TTF_SizeText(getFont(), sText.c_str(), &w, &h);
		m_iSize = ciOldSize;
		return Vector2(w,h);
	}
	
	void Font::renderText2(const double& X, const double& Y, const double& Z,
	                       const std::string& Text, Vector2* pvOut, bool rememberString)
	{
		//std::cout << SDL_GetTicks() << std::endl;
		//static int ticks = 0;
		//static int tmpI;
		//tmpI = SDL_GetTicks();
		
		Vector2 size;
		unsigned int Texture = createTexture(Text, &size, rememberString);
		
		// Draw this texture on a quad with the given xyz coordinates.
		glBegin(GL_QUADS);
			glTexCoord2i(0, 1); glVertex3d(X       , Y       , Z);
			glTexCoord2i(1, 1); glVertex3d(X+size.x, Y       , Z);
			glTexCoord2i(1, 0); glVertex3d(X+size.x, Y+size.y, Z);
			glTexCoord2i(0, 0); glVertex3d(X       , Y+size.y, Z);
		glEnd();
		
		if (pvOut) {*pvOut = size;}
		
		//Clean up
		if (!rememberString)
			glDeleteTextures(1, &Texture);
		
		//ticks += SDL_GetTicks() - tmpI;
		//if (ticks >= 30) std::cout << ticks << std::endl;
	}
	
	void Font::renderText3(const std::string& text, const Color& col, const Vector3 &v1, const Vector3 &v2,
	                      const Vector3 &v3, const Vector3 &v4, bool bDisableDepthTest, float fOffset, bool rememberString) {
		
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glColor4fv(col.rg);
		
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_LIGHTING);
		if (bDisableDepthTest)
			glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //self made
		
		unsigned int Texture = createTexture(text, NULL, rememberString);
		
		//calculate offset vector
		Vector3 vOffset = fOffset * Vector3::normalize(Vector3::crossProduct(Vector3::normalize(v2 - v1), Vector3::normalize(v3 - v1)) );
		
		//Draw this texture on a quad with the given xyz coordinates
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3fv((v1 + vOffset).rg);
			glTexCoord2d(1, 1); glVertex3fv((v2 + vOffset).rg);
			glTexCoord2d(1, 0); glVertex3fv((v3 + vOffset).rg);
			glTexCoord2d(0, 0); glVertex3fv((v4 + vOffset).rg);
		glEnd();
		
		// Clean up
		if (!rememberString)
			glDeleteTextures(1, &Texture);
		
		glPopAttrib();
	}
	
	void Font::renderText3(const std::string& text, const Color& amb, const Color& diff, const Color& spec,
	                       const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4,
	                       bool bDisableDepthTest, float fOffset, bool rememberString) {
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT ,  amb.rg);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , diff.rg);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec.rg);
		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
		
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glEnable(GL_LIGHTING);
		if (bDisableDepthTest)
			glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //self made
		
		
		unsigned int Texture = createTexture(text, NULL, rememberString);
		
		//calculate offset vector
		Vector3 vOffset = fOffset * Vector3::normalize(Vector3::crossProduct(Vector3::normalize(v2 - v1), Vector3::normalize(v3 - v1)) );
		
		// Draw this texture on a quad with the given xyz coordinates
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex3fv((v1 + vOffset).rg);
			glTexCoord2d(1, 1); glVertex3fv((v2 + vOffset).rg);
			glTexCoord2d(1, 0); glVertex3fv((v3 + vOffset).rg);
			glTexCoord2d(0, 0); glVertex3fv((v4 + vOffset).rg);
		glEnd();
		
		// Clean up
		if (!rememberString)
			glDeleteTextures(1, &Texture);
		
		glPopAttrib();
	}
	
	
	unsigned int Font::createTexture(const std::string& s, Vector2 *pvSize, bool rememberString) {
		
		//was the texture already loaded?
		for(std::map<std::string, S_FontInternStringTextureMemory>::iterator mpi = s_mpStringTextureMemory.begin();
		    mpi!=s_mpStringTextureMemory.end(); ++mpi) {
			if (mpi->first == Util::toString(static_cast<int>(m_sPath.length())) + "###" +
				              Util::toString(m_iSize) + "###" + m_sPath + s) {
				glBindTexture(GL_TEXTURE_2D, mpi->second.iTextureID);
				if (pvSize) *pvSize = mpi->second.vSize;
				return mpi->second.iTextureID; //return texture id
			}
		}
		//not found therefore new texture:
		
		//Create some variables
		unsigned Texture = 0;
		SDL_Color Color = {255, 255, 255};
		SDL_Surface *Message = TTF_RenderText_Blended(getFont() , s.c_str(), Color);
		
		// Generate an OpenGL 2D texture from the SDL_Surface
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Message->pixels); //previously: 2.GL_BGRA!
		
		if (pvSize) {
			pvSize->x = Message->w;
			pvSize->y = Message->h;
		}
		
		if (rememberString) {
			std::string tmp;
			tmp += Util::toString(static_cast<int>(m_sPath.length()));
			tmp += "###" + Util::toString(m_iSize) + "###";
			tmp += m_sPath + s;
			
			S_FontInternStringTextureMemory tmpMem;
			tmpMem.iTextureID = Texture;
			tmpMem.vSize = Vector2(Message->w, Message->h);
			
			std::pair<std::string, S_FontInternStringTextureMemory> tmpPair = std::make_pair(tmp, tmpMem);
			s_mpStringTextureMemory.insert(tmpPair);
		}
		
		SDL_FreeSurface(Message); //surface no longer needed
		
		return Texture;
		//should be deleted somwhere sometime if rememberString==false;
		//otherwise the texture will be deleted with quit or clearStringTextureMemory()
	}
	
	
	// ---static---
	void Font::begin2D()
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
	void Font::end2D()
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		
		glPopAttrib();
	}
	
	void Font::quit() {
		clearStringTextureMemory(); //delete remembered string textures
		for(s_mpi = s_mpFont.begin(); s_mpi!=s_mpFont.end(); ++s_mpi) {
			for (int i = 0; i < static_cast<int> (s_mpi->second.vFont.size()); i++) 
				TTF_CloseFont(s_mpi->second.vFont[i]);
		}
		s_mpFont.clear();
	}
	void Font::deleteTTF (std::string sPath, bool bCompleteThePath)	{
		if (bCompleteThePath)
			sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpFont.find(sPath);
		if (s_mpi == s_mpFont.end()) {
			throw Exception("TTF font(" + sPath + ") to be deleted does not exist", "deleteTTF", "RoeFont.cpp", ROE_LINE);
		} else {
			for (int i = 0; i < static_cast<int>(s_mpi->second.vFont.size()); i++) {
				if(s_mpi->second.vFont[i]) {
					TTF_CloseFont(s_mpi->second.vFont[i]);
					s_mpi->second.vFont[i] = NULL;
				}
			}
			s_mpi->second.vFont.clear();
			s_mpFont.erase(s_mpi);
		}
	}
	
	void Font::loadTTF(std::string sPath, int ptsizeMin, int ptsizeMax) {
		if (ptsizeMin > ptsizeMax) {
			throw Exception("ptsizeMin > ptsizeMax", "loadTTF", "RoeFont.cpp", ROE_LINE);
		}
		
		sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpFont.find(sPath);
		if (s_mpi == s_mpFont.end()) {
			S_FontIntern sfont;
			sfont.iSizeMin = ptsizeMin;
			sfont.iSizeMax = ptsizeMax;
			for (int i = ptsizeMin; i <= ptsizeMax; i++) {
				sfont.vFont.push_back(TTF_OpenFont(sPath.c_str(), i));
			}
			
			//map erweitern
			s_pair = std::make_pair (sPath.c_str(), sfont);
			s_mpFont.insert(s_pair);
		} else {
			throw Exception("font already loaded", "loadTTF", "RoeFont.cpp", ROE_LINE);
		}
	}
	
	void Font::clearStringTextureMemory() {
		for(std::map<std::string,  S_FontInternStringTextureMemory>::iterator mpi = s_mpStringTextureMemory.begin();
		    mpi!=s_mpStringTextureMemory.end(); ++mpi) {
			glDeleteTextures(1, &mpi->second.iTextureID);
		}
		s_mpStringTextureMemory.clear();
	}
	
} // namespace Roe





