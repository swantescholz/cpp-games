#include "glcTexture.h"

map<string, S_TextureIntern > glcTexture::c_mpTexture;
map<string, S_TextureIntern >::iterator glcTexture::c_mpi;
pair<string, S_TextureIntern > glcTexture::c_pair;
string glcTexture::c_sPathStart, glcTexture::c_sPathEnd = ".png";

glcTexture::glcTexture(const glcTexture &rhs) : m_sPath(rhs.m_sPath), m_sName(rhs.m_sName), m_mMatrix(rhs.m_mMatrix) {}
glcTexture::glcTexture(const tbMatrix &m)
{
	m_mMatrix = m;
}

glcTexture::glcTexture(const string &sPath, const tbMatrix &m) {
	m_mMatrix = m;
	
	Load(sPath);
}

glcTexture::~glcTexture()
{
}

void glcTexture::Bind(tbColor col, bool bWithTransformation) {
	glColor4fv(col.c);
	if (bWithTransformation) {
		glMatrixMode(GL_TEXTURE);
		g_pGL->LoadMatrix(c_mpTexture[m_sPath].mStdMatrix * m_mMatrix);
		glMatrixMode( GL_MODELVIEW );
	}
	glBindTexture(GL_TEXTURE_2D, c_mpTexture[m_sPath].iTexture);
}

void glcTexture::Load(string sPath) {
	m_sName = sPath;
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpTexture.find(sPath);
	if (c_mpi == c_mpTexture.end()) {
		throw glcException("requested texture not yet loaded:" + sPath);
	}
	
	m_sPath = sPath;
}

void glcTexture::LoadTexture(string sPath, const tbMatrix &mStdMatrix) {
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpTexture.find(sPath);
	if (c_mpi != c_mpTexture.end()) {
		g_pLogfile->Textout(BLUE,("warning: texture already loaded: " + sPath + "\t, loading canceled").data() );
		return;
	}
	
	S_TextureIntern texture;
	texture.iTexture = LoadTextureSimple(sPath.data());
	texture.mStdMatrix = mStdMatrix; 
	
	//map erweitern
	c_pair = make_pair (sPath, texture);
	c_mpTexture.insert(c_pair);
}

GLuint glcTexture::LoadTextureSimple(const char* file)
{
	SDL_Surface* surface = IMG_Load(file);
	
	//self-made
	if (surface == NULL) {
		string sTmp(file);
		throw glcException("texture does not exist: " + sTmp);
	}
	surface = FlipSurface(surface, FLIP_VERTICAL);
	   
	GLuint texture;
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	SDL_PixelFormat *format = surface->format;
	if (format->Amask)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
		surface->w, surface->h, GL_RGBA,GL_UNSIGNED_BYTE, surface->pixels);
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
			surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}
	SDL_FreeSurface(surface);
	return texture;
}

void glcTexture::DeleteTexture	(string sPath, bool bCompleteThePath) {
	if (bCompleteThePath)
		sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpTexture.find(sPath);
	if (c_mpi == c_mpTexture.end()) { // is not called when false sPath (TODO, but no idea)
		throw glcException("texture to be deleted does not exist: " + sPath);
	} else {
		glDeleteTextures(1, &c_mpi->second.iTexture);
		c_mpTexture.erase(c_mpi);
	}
}

bool glcTexture::IsTextureLoaded (string sPath, bool bCompleteThePath) {
	if (bCompleteThePath)
		sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpTexture.find(sPath);
	if (c_mpi == c_mpTexture.end()) {
		return false;
	}
	return true;
}

void glcTexture::Init(string sStart, string sEnd) {
	c_sPathStart 	= sStart;
	c_sPathEnd		= sEnd;
}
void glcTexture::Quit() {
	for(c_mpi=c_mpTexture.begin(); c_mpi!=c_mpTexture.end(); ++c_mpi) {
		glDeleteTextures(1, &c_mpi->second.iTexture);
	}
	c_mpTexture.clear();
}


Uint32 glcTexture::GetPixel32( SDL_Surface *surface, int x, int y )
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	
	//Get the requested pixel
	return pixels[ ( y * surface->w ) + x ];
}

void glcTexture::PutPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	
	//Set the pixel
	pixels[ ( y * surface->w ) + x ] = pixel;
}

SDL_Surface* glcTexture::FlipSurface( SDL_Surface *surface, int flags )
{
	//Pointer to the soon to be flipped surface
	SDL_Surface *flipped = NULL;
	
	//If the image is color keyed
	if( surface->flags & SDL_SRCCOLORKEY )
	{
		flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
	}
	//Otherwise
	else
	{
		flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
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
			Uint32 pixel = GetPixel32( surface, x, y );
			
			//Copy pixel
			if( ( flags & FLIP_VERTICAL ) && ( flags & FLIP_HORIZONTAL ) )
			{
				PutPixel32( flipped, rx, ry, pixel );
			}
			else if( flags & FLIP_HORIZONTAL )
			{
				PutPixel32( flipped, rx, y, pixel );
			}
			else if( flags & FLIP_VERTICAL )
			{
				PutPixel32( flipped, x, ry, pixel );
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


