#include "RoeTexture.h"
#include "RoeException.h"
#include "RoeUtil.h"
#include "RoeTextfile.h"
#include <GL/gl.h>

namespace Roe {

	std::map <std::string, S_TextureIntern >           Texture::s_mpTexture;
	std::map <std::string, S_TextureIntern >::iterator Texture::s_mpi;
	std::pair<std::string, S_TextureIntern >           Texture::s_pair;
	std::string Texture::s_sPathStart, Texture::s_sPathEnd = ".png";
	
	Texture::Texture(const Matrix &m)
	{
		m_mMatrix = m;
	}
	
	Texture::Texture(const std::string &sPath, const Matrix &m) {
		m_mMatrix = m;
		load(sPath);
	}
	
	Texture::~Texture()
	{
	}
	
	void Texture::bind(Color col, bool bWithTransformation) {
		glColor4fv(col.rg);
		if (bWithTransformation) {
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf((s_mpTexture[m_sPath].mStdMatrix * m_mMatrix).n);
			glMatrixMode(GL_MODELVIEW);
		}
		glBindTexture(GL_TEXTURE_2D, s_mpTexture[m_sPath].iTexture);
	}
	
	void Texture::load(std::string sPath) {
		m_sName = sPath;
		sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpTexture.find(sPath);
		if (s_mpi == s_mpTexture.end()) {
			throw Exception("angefragte Textur noch nicht geladen: " + sPath, "load", "RoeTexture.cpp", ROE_LINE);
		}
		
		m_sPath = sPath;
	}
	
	void Texture::loadTexture(std::string sPath, int numMipmaps, const Matrix& mStdMatrix) {
		sPath = s_sPathStart + sPath + s_sPathEnd;
		
		S_TextureIntern texture;
		texture.iTexture   = loadTextureIntoGL(sPath, numMipmaps);
		texture.mStdMatrix = mStdMatrix;
		glBindTexture(GL_TEXTURE_2D, texture.iTexture);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &texture.iWidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texture.iHeight);
		
		//expand map
		s_pair = make_pair (sPath, texture);
		s_mpTexture.insert(s_pair);
	}
	GLuint Texture::loadTextureIntoGL(std::string sPath, int numMipmaps)
	{
		
		SDL_Surface* surface = IMG_Load(sPath.c_str());
		
		if (surface == NULL) {
			throw Exception("texture does not exist: " + sPath, "loadTextureSimple", "RoeTexture.cpp", ROE_LINE);
		}
		
		int width  = surface->w;
		int height = surface->h;
		const int bpp = surface->format->BytesPerPixel;
		int imageSize = width * height * bpp;
		GLubyte *data = new GLubyte[imageSize];
		int pos = 0;
		bool hasAlpha = (bpp == 4);
		
		for (int y = height - 1; y >= 0; y--)
		{
			for (int x = 0; x < width; x++)
			{
				byte r, g, b, a;
				
				//new (very complicated), controls also non-aplha textures
				byte4 color = *((byte4*)&(((byte*)surface->pixels)[(y*width + x)*bpp]));
				
				if (hasAlpha)
					SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);
				else
					SDL_GetRGB(color, surface->format, &r, &g, &b);
				
				data[pos++] = r;
				data[pos++] = g;
				data[pos++] = b;
				
				if (hasAlpha)
					data[pos++] = a;
			}
		}
		
		SDL_FreeSurface(surface); //not needed anymore
		
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, numMipmaps==0 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
		
		int type = hasAlpha ? GL_RGBA : GL_RGB;
		bool oldWasPOT = Util::isPowerOfTwo(width) && Util::isPowerOfTwo(height); //was image before power of two?
		if (oldWasPOT || numMipmaps==0) {
			glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		}
		
		//Util::saveImageTGA(std::string("res/mip") + Util::getNextNumber() + "_" + Util::toString(width)+"_"+Util::toString(height)+".tga",
		//                   data, width, height, bpp==4);
		
		//MIPMAPPING
		if (numMipmaps < 0) { //auto calculate num mipmaps
			for (numMipmaps = 0; 1<<numMipmaps <= width || 1<<numMipmaps <= height; numMipmaps++) {}
			if (oldWasPOT) numMipmaps--;
		}
		
		byte *newData = NULL;
		
		int newWidth, newHeight;
		for (int i = 1; i <= numMipmaps; i++) { //scaling for each mipmap
			//oldWasPOT = Util::isPowerOfTwo(width) && Util::isPowerOfTwo(height);
			newWidth  = Util::previousPowerOfTwo(width);
			newHeight = Util::previousPowerOfTwo(height);
			newData = new byte[newWidth*newHeight*bpp];
			scaleImage(data, width, height, newData, newWidth, newHeight, bpp); //scaling image to new size
			glTexImage2D(GL_TEXTURE_2D, i-(oldWasPOT ? 0:1),
			             type, newWidth, newHeight, 0, type, GL_UNSIGNED_BYTE, newData); //create mipmap in GL
			
			width = newWidth;
			height= newHeight;
			delete[] data;
			data = new byte[newWidth*newHeight*bpp];
			for (int j = 0; j < newWidth*newHeight*bpp; j++) {
				data[j] = newData[j];
			}
			delete[] newData;
		}
		
		
		
		delete[] data;
		
		return id;
	}
	
	void Texture::scaleImage(const byte *oldData, int w1, int h1, byte *newData, int w2, int h2, int bpp) {
		
		byte *tmp = new byte[w2*h1*bpp];
		byte *columnOld = new byte[h1*bpp];
		byte *columnNew = new byte[h2*bpp];
		
		//scale first W
		for (int i = 0; i < h1; i++) {
			scaleImage1D(oldData + (i*w1*bpp), w1, tmp + (i*w2*bpp), w2, bpp); //resize row
		}
		
		//then scale H
		for (int i = 0; i < w2; i++) {
			for (int j = 0; j < h1; j++) { //copy data from image into column array
				for (int h = 0; h < bpp; h++) columnOld[j*bpp+h] = tmp[(j*w2+i)*bpp+h];
			}
			scaleImage1D(columnOld, h1, columnNew, h2, bpp); //resize column
			for (int j = 0; j < h2; j++) { //copy data from new column array into new image
				for (int h = 0; h < bpp; h++) {
					newData[(j*w2+i)*bpp+h] = columnNew[j*bpp+h];
				}
			}
		}
		
		//Util::saveImageTGA(std::string("res/mip") + Util::getNextNumber() + "_" + Util::toString(w2)+"_"+Util::toString(h2)+".tga",
		//                   newData, w2, h2, bpp==4?true:false);
		
		delete[] columnNew;
		delete[] columnOld;
		delete[] tmp;
	}
	void Texture::scaleImage1D(const byte *oldData, int w1, byte *newData, int w2, int bpp) {
		
		if (w2==w1) { //catch equality
			for (int i = 0; i < w1*bpp; i++) {
				newData[i] = oldData[i];
			}
			return;
		}
		if (w2>=w1) { //catch upscaling
			byte *tmp = new byte[w1*bpp*2]; //upscaled oldData by 2
			for (int i = 0; i < w1; i++) {
				tmp[(2*i+0)*bpp+0] = oldData[i*bpp+0];
				tmp[(2*i+0)*bpp+1] = oldData[i*bpp+1];
				tmp[(2*i+0)*bpp+2] = oldData[i*bpp+2];
				tmp[(2*i+1)*bpp+0] = oldData[i*bpp+0];
				tmp[(2*i+1)*bpp+1] = oldData[i*bpp+1];
				tmp[(2*i+1)*bpp+2] = oldData[i*bpp+2];
			}
			scaleImage1D(tmp, w1*2, newData, w2, bpp);
			delete[] tmp;
			return;
		}
		
		//normal DOWNSCALING
		const float factorW = static_cast<float>(w2-1) / (w1-1);
		
		//TEST for more sharp look
		int index = 0;
		for (int i = 0; i < w2; i++) {
			for (int j = 0; j < bpp; j++) {
				index = static_cast<int>(i*(w1*1.0f/w2))*bpp+j;
				//if (index >= bpp && index <= (w1-1)*bpp)
				//	index += roe_random.uniformInt(-1,1)*bpp;
				newData[i*bpp+j] = oldData[index];
			}
		}
		return;
		//TEST END
		
		float *leftBorders   = new float[w1];
		float *intensityLeft = new float[w1];
		int   *leftPixel     = new int  [w1];
		float *newIntensity  = new float[w2*bpp];
		float *intensitySum  = new float[w2];
		float *opacity       = new float[w1];
		for (int i = 0; i < w2    ; i++) intensitySum[i] = 0.0f;
		for (int i = 0; i < w2*bpp; i++) newIntensity[i] = 0.0f;
		for (int i = 0; i < w1    ; i++) {
			if (bpp!=4) opacity[i] = 1.0f;
			else {
				opacity[i] = 1.0f;
				opacity[i] = oldData[i*bpp+3] / 256.0f;
			}
		}
		
		for (int i = 0; i < w1; i++) { //calculating all important informations
			leftBorders   [i] = factorW * i;
			leftPixel     [i] = static_cast<int>(leftBorders[i]);
			intensityLeft [i] = ((leftPixel[i]+1.0f) - leftBorders[i]) * opacity[i];
			intensitySum[leftPixel[i] ] += intensityLeft[i];
			if (leftPixel[i]+1 < w2) {
				intensitySum[leftPixel[i]+1] += opacity[i] - intensityLeft[i];
			}
		}
		
		for (int i = 0; i < w1; i++) {
			for (int j = 0; j < bpp; j++) {
				newIntensity[bpp*leftPixel[i]+j] += oldData[bpp*i+j] * intensityLeft[i]; 
				if (leftPixel[i]+1 < w2) {
					newIntensity[bpp*leftPixel[i]+bpp+j] += oldData[bpp*i+j] * (opacity[i] - intensityLeft[i]);
				}
			}
		}
		for (int i = 0; i < w2; i++) {
			for (int j = 0; j < bpp; j++) {
				newData[i*bpp+j] = (int)Util::round(newIntensity[i*bpp+j] / intensitySum[i]); //adjust intensity
			}
		}
		
		delete[] leftBorders;
		delete[] intensityLeft;
		delete[] leftPixel;
		delete[] newIntensity;
		delete[] intensitySum;
		delete[] opacity;
	}
	/*
	GLuint Texture::loadTextureSimpleOld(std::string sPath)
	{
		SDL_Surface* surface = IMG_Load(sPath.c_str());
		
		if (surface == NULL) {
			throw Exception("texture does not exist: " + sPath, "loadTextureSimple", "RoeTexture.cpp");
		}
		
		surface = flipSurface(surface, FT_VERTICAL);
		
		GLuint texture;
		glPixelStorei(GL_UNPACK_ALIGNMENT,4);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		SDL_PixelFormat *format = surface->format;
		
		//new
		if (format->Amask) {
			glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		} else{
			glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		}
		
		//too old maybe
		//if (format->Amask) {
		//	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, surface->w, surface->h, GL_RGBA,GL_UNSIGNED_BYTE, surface->pixels);
		//} else{
		//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		//}
		
		SDL_FreeSurface(surface);
		
		return texture;
	}*/
	
	void Texture::deleteTexture (std::string sPath, bool bCompleteThePath) {
		if (bCompleteThePath)
			sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpTexture.find(sPath);
		if (s_mpi == s_mpTexture.end()) {
			throw Exception("texture to be deleted does not exist: " + sPath, "deleteTexture", "RoeTexture.cpp", ROE_LINE);
		} else {
			glDeleteTextures(1, &s_mpi->second.iTexture);
			s_mpTexture.erase(s_mpi);
		}
	}
	
	void Texture::createHeightmapOFF  (std::string sImage, std::string sOFF) { //converts an loaded image to an heightmap
		Textfile tf(sOFF);
		Texture texture(sImage);
		const int w = texture.getWidth();
		const int h = texture.getHeight();
		tf.pushBack("OFF");
		tf.pushBack(Util::toString(w*h) + " " + Util::toString(2*(w-1)*(h-1)) + " 0");
		tf.saveFile(false); //erase original file
		tf.clear();
		tf.pushBack("\n");
		Vector3 pos;
		std::string sTmp;
		const int bpp = 4;
		byte* pixels = new byte[w*h*bpp];
		texture.bind();
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		
		for (int i = h-1; i >= 0; i--) {
			for (int j = 0; j < w; j++) {
				pos.y = ((int)pixels[(i*w+j)*bpp+0]+pixels[(i*w+j)*bpp+1]+pixels[(i*w+j)*bpp+2]) * pixels[(i*w+j)*bpp+3] / 196608.0f;
				pos.x = (j-0.5*w)/w;
				pos.z = (i-0.5*h)/h;
				sTmp = Util::toString(pos.x) + " " + Util::toString(pos.y) + " " + Util::toString(pos.z);
				tf.pushBack(sTmp);
			}
		}
		for (int i = 0; i < h-1; i++) {
			for (int j = 0; j < w-1; j++) {
				tf.pushBack("3 " + Util::toString(i*w+j)   + " " +  Util::toString(i*w+j+1) + " " +  Util::toString(i*w+j+w));
				tf.pushBack("3 " + Util::toString(i*w+j+1) + " " +  Util::toString(i*w+j+w+1) + " " +  Util::toString(i*w+j+w));
			}
		}
		tf.saveFile(true);
		tf.clear();
		delete[] pixels;
	}
	
	void Texture::init(std::string sStart, std::string sEnd) {
		s_sPathStart  = sStart;
		s_sPathEnd    = sEnd;
	}
	void Texture::quit() {
		for(s_mpi=s_mpTexture.begin(); s_mpi!=s_mpTexture.end(); ++s_mpi) {
			glDeleteTextures(1, &s_mpi->second.iTexture);
		}
		s_mpTexture.clear();
	}
	
	
	Uint32 Texture::getPixel32( SDL_Surface *surface, int x, int y )
	{
		//Convert the pixels to 32 bit
		Uint32 *pixels = (Uint32 *)surface->pixels;
		
		//Get the requested pixel
		return pixels[ ( y * surface->w ) + x ];
	}
	
	void Texture::putPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
	{
		//Convert the pixels to 32 bit
		Uint32 *pixels = (Uint32 *)surface->pixels;
		
		//Set the pixel
		pixels[ ( y * surface->w ) + x ] = pixel;
	}
	
	SDL_Surface* Texture::flipSurface( SDL_Surface *surface, int flags )
	{
		//Pointer to the soon to be flipped surface
		SDL_Surface *flipped = NULL;
		
		//If the image is color keyed
		if( surface->flags & SDL_SRCCOLORKEY )
		{
			flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel,
			                                surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
		}
		//Otherwise
		else
		{
			flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel,
			                                surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
		}
		
		//If the surface must be locked
		if( SDL_MUSTLOCK( surface ) )
		{
			//Lock the surface
			SDL_LockSurface( surface );
		}
		
		//Go through columns
		for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
		{
			//Go through rows
			for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
			{
				//Get pixel
				Uint32 pixel = getPixel32( surface, x, y );
				
				//Copy pixel
				if( ( flags & FT_VERTICAL ) && ( flags & FT_HORIZONTAL ) )
				{
					putPixel32( flipped, rx, ry, pixel );
				}
				else if( flags & FT_HORIZONTAL )
				{
					putPixel32( flipped, rx, y, pixel );
				}
				else if( flags & FT_VERTICAL )
				{
					putPixel32( flipped, x, ry, pixel );
				}
			}
		}
		
		//Unlock surface
		if( SDL_MUSTLOCK( surface ) )
		{
			SDL_UnlockSurface( surface );
		}
		
		//Copy color key
		if( surface->flags & SDL_SRCCOLORKEY )
		{
			SDL_SetColorKey( flipped, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
		}
		
		//Return flipped surface
		return flipped;
	}
	
} // namespace Roe


