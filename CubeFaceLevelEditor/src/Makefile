# G++ or MINGW
CUSE = MINGW
CMINGW = i586-mingw32msvc-g++
CG++ = g++

CXX = $(C$(CUSE))

ARG_MINGW = -I/usr/i586-mingw32msvc/usr/include -I/usr/i586-mingw32msvc/usr/include/SDL -u _WinMain@16 -L/usr/i586-mingw32msvc/usr/lib
ARG_G++ = -I/usr/include/SDL -I/usr/include/GL

ARG = $(ARG_$(CUSE))

LIB_MINGW = -lSDL_ttf -lSDL_mixer -lSDL_image -lSDLmain -lSDL -lopengl32 -lglu32
#LIB_MINGW = /usr/i586-mingw32msvc/usr/lib/libSDL_ttf.a /usr/i586-mingw32msvc/usr/lib/libSDL_mixer.a /usr/i586-mingw32msvc/usr/lib/libSDL_image.a /usr/i586-mingw32msvc/usr/lib/libSDLmain.a /usr/i586-mingw32msvc/usr/lib/libSDL.dll.a -lopengl32 -lglu32
LIB_G++ = -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer -lGL -lGLU

LIB = $(LIB_$(CUSE))

END_G++ =
END_MINGW = .exe

END = $(END_$(CUSE))

OBJ = main.o tbMatrix.o tbUtils.o glcTimer.o glcException.o glcLogfile.o glcSDL_GL.o glcSphere.o glcModel.o glcTexture.o glcSound.o glcMusic.o glcFont.o GLC.o CGame.o CCF_LevelEditor.o CLevel.o CBlock.o CGS.o

all: CF_LevelEditor$(END) delete

CF_LevelEditor$(END): $(OBJ)
	$(CXX) -o $@ $(ARG) $(OBJ) $(LIB) 

%.o: %.cpp
	$(CXX) -o $@ $(ARG) -c $<

delete:
	rm -f *.o
