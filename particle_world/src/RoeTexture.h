#ifndef ROETEXTURE_H_
#define ROETEXTURE_H_

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <map>
#include "RoeCommon.h"
#include "RoeVector3.h"
#include "RoeMatrix.h"
#include "RoeColor.h"

namespace Roe {
	
	struct S_TextureIntern {
		GLuint iTexture;   //textur-index
		Matrix mStdMatrix; //standard-transformation-matrix
		int iWidth;
		int iHeight;
	};
	
	//alpha textures from back to front rendering!
	//not POT textures are allowerd now, but for
	//mipmapped textures POT w and h values are advised(but not strictly needed)
	class Texture
	{
	public:
		enum EFlipType {
			FT_HORIZONTAL = 1,
			FT_VERTICAL   = 2
		};
		Texture(const Matrix& m = Matrix::IDENTITY);
		Texture(const std::string& sPath, const Matrix& m = Matrix::IDENTITY);
		~Texture();
		
		void        load      (std::string sPath);
		std::string getName   ()               const {return m_sName;}
		Matrix      getMatrix ()               const {return m_mMatrix;}
		void        setMatrix (const Matrix &m)      {m_mMatrix = m;}
		
		//bind just after load()
		//does change the texture matrix, but returns to modelview (if bool true)
		//bottom left, bottom right, top right then top left is the right order
		//works now with lighting (dont forget the normal vector!)
		void bind(Color col = Color(1.0,1.0,1.0,1.0), bool bWithTransformation = true);
		int  getWidth () {return s_mpTexture[m_sPath].iWidth;}
		int  getHeight() {return s_mpTexture[m_sPath].iHeight;}
		
		// *static*
		//loading and deleting
		static void     loadTexture         (std::string sPath, int numMipmaps = -1, const Matrix& mStdMatrix = Matrix::IDENTITY);
		static GLuint   loadTextureIntoGL   (std::string sPath, int numMipmaps = -1); //loads the texture+numMipmaps mipmaps(if -1, then self calculated)
		static void     scaleImage          (const byte *oldData, int w1, int h1, byte *newData, int w2, int h2, int bpp = 4); //scales an image 2D
		static void     scaleImage1D        (const byte *oldData, int w1, byte *newData, int w2, int bpp = 4); //scales an image 2D
		static void     deleteTexture       (std::string sPath, bool bCompleteThePath = true);
		//static GLuint   loadTextureSimpleOld(std::string sPath); //original old version
		static void     createHeightmapOFF  (std::string sImage, std::string sOFF); //converts an loaded image to an heightmap OFF model-file
		
		// for the static variables
		static void init(std::string sStart, std::string sEnd);
		static void setPathStart(std::string s) {s_sPathStart = s;}
		static void setPathEnd  (std::string s) {s_sPathEnd   = s;}
		static void quit();
		
		//other functions needed
		static Uint32 getPixel32( SDL_Surface *surface, int x, int y );
		static void   putPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
		static SDL_Surface* flipSurface( SDL_Surface *surface, int flags );
		
	private:
		
		static std::map<std::string, S_TextureIntern >               s_mpTexture;
		static std::map<std::string, S_TextureIntern >::iterator     s_mpi;
		static std::pair<std::string,S_TextureIntern >               s_pair; //pair-object for the map
		static std::string s_sPathStart, s_sPathEnd; //standard path
		
		std::string m_sPath;
		std::string m_sName;
		Matrix      m_mMatrix;
	};
	
} // namespace Roe

#endif /*ROETEXTURE_H_*/
