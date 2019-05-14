#ifndef ROEFONT_H_
#define ROEFONT_H_

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include <string>
#include <map>
#include "RoeVector2.h"
#include "RoeVector3.h"
#include "RoeColor.h"
#include "RoeUtil.h"

namespace Roe {

	struct S_FontIntern { //used to save fonts
		std::vector<TTF_Font*> vFont;
		int iSizeMin, iSizeMax;
	};
	struct S_FontInternStringTextureMemory { //used to save remembere string textures
		unsigned int iTextureID;
		Vector2      vSize;
	};
	
	//for 2D font, call the fitting methods between begin2D() and end2D()
	class Font
	{
	public:
		Font();
		Font(const std::string &s, int iSize);         //calls load()
		~Font();
		
		void load(const std::string &s, int iSize);    //does load from the font-map, not from any file!
		std::string getName() const {return m_sName;}
		int         getSize() const {return m_iSize;}
		void        setSize(int iSize);
		Vector2     calcTextSize(std::string sText, int textSize = -1); //returns w and h of the surface that would be rendered with sText with textSize
		  //with textSize = -1, the std. size of this font will be used
		
		//with 2D between begin2D
		//without extra color
		//v saves position in pixels (bottom left = (0,0))
		void renderText2(const std::string& text, SDL_Rect *rct) {
			Vector2 vTmp;
			renderText2(rct->x, rct->y, 0, text, &vTmp);
			rct->w = vTmp.x;
			rct->h = vTmp.y;
		}
		void renderText2(const std::string& text, const Vector2 &v, Vector2 *pvOut = NULL, bool rememberString = true) {
			renderText2(v.x, v.y, 0, text, pvOut, rememberString);
		}
		void renderText2(const std::string& text, const Vector3 &v, Vector2 *pvOut = NULL, bool rememberString = true) {
			renderText2(v.x, v.y, v.z, text, pvOut, rememberString);
		}
		
		//with extra color
		void renderText2(const std::string& text, const Color& col, SDL_Rect *rct, bool rememberString = true) {
			glColor4fv(col.rg);
			Vector2 vTmp;
			renderText2(rct->x, rct->y, 0, text, &vTmp, rememberString);
			rct->w = vTmp.x;
			rct->h = vTmp.y;
		}
		void renderText2(const std::string& text, const Color& col, const Vector2 &v, Vector2 *pvOut = NULL, bool rememberString = true) {
			glColor4fv(col.rg);
			renderText2(v.x, v.y, 0, text, pvOut, rememberString);
		}
		void renderText2(const std::string& text, const Color& col, const Vector3 &v, Vector2 *pvOut = NULL, bool rememberString = true) {
			glColor4fv(col.rg);
			renderText2(v.x, v.y, v.z, text, pvOut, rememberString);
		}
		
		//original basis method for 2D (pvOut save sidth and height)
		void renderText2(const double& X, const double& Y, const double& Z,
		                 const std::string& Text, Vector2* pvOut = NULL, bool rememberString = true);
		
		//with 3D (not between begin2D() and end2D())
		//v1: bottom left, v2: bottom right, v3: top right, v4: top left
		void renderText3(const std::string& text, const Color& col, const Vector3 &v1, const Vector3 &v2,
		                const Vector3 &v3, const Vector3 &v4, bool bDisableDepthTest = false, float fOffset = 0.0f, bool rememberString = true);
		//with ligth
		void renderText3(const std::string& text, const Color& amb, const Color& diff, const Color& spec,
		                const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4,
		                bool bDisableDepthTest = false, float fOffset = 0.0f, bool rememberString = true);
		void renderText3(const std::string& text, Color *acol,
		                const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4,
		                bool bDisableDepthTest = false, float fOffset = 0.0f, bool rememberString = true) {
			renderText3(text,acol[0],acol[1],acol[2],v1,v2,v3,v4,bDisableDepthTest,fOffset,rememberString);
		}
		
		TTF_Font* getFont () {
			s_mpi = s_mpFont.find(m_sPath);
			return s_mpi->second.vFont[m_iSize - s_mpi->second.iSizeMin];
		}
		unsigned int createTexture(const std::string& s, Vector2 *pvSize, bool rememberString = true);
		
		static void begin2D(); // Go in HUD-drawing mode
		static void end2D();   // Come out of HUD mode
		
		static void loadTTF   (std::string sPath, int ptsizeMin, int ptsizeMax); //loads all sizes between min and max
		static void deleteTTF (std::string sPath, bool bCompleteThePath = true); //deletes the complete font
		static void clearStringTextureMemory(); //deletes the remebered textures
		//for the static variables
		static void init(std::string sStart, std::string sEnd)  {s_sPathStart = sStart; s_sPathEnd = sEnd;}
		static void setPathStart(std::string s)                 {s_sPathStart = s;}
		static void setPathEnd  (std::string s)                 {s_sPathEnd   = s;}
		static void quit();
		
	private:
		static int nextpoweroftwo(int x) {
			double logbase2 = log(x) / log(2);
			return Util::round(pow(2,ceil(logbase2)));
		}
		
		
		static std::map <std::string, S_FontIntern>            s_mpFont; //map with all display lists
		static std::map <std::string, S_FontIntern>::iterator  s_mpi;    //map iterator
		static std::pair<std::string, S_FontIntern>            s_pair;   //pair-object for the map
		static std::string s_sPathStart, s_sPathEnd; //standard path
		static std::map <std::string, S_FontInternStringTextureMemory> s_mpStringTextureMemory; //remembers the textures of strings already rendered
		  //this std::string has following order: PATH_LENGTH###SIZE###PATH_NAME|STRING  without the pipe '|'
		
		std::string m_sPath; //complete path
		std::string m_sName; //name
		int         m_iSize; //actual size
	};
	
} // namespace Roe

#endif /*ROEFONT_H_*/
