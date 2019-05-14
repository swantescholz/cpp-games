# G++ or MINGW
CUSE = MINGW

CMINGW = i586-mingw32msvc-g++
CG++ = g++

CXX = $(C$(CUSE))

ARG_MINGW = -I/usr/i586-mingw32msvc/usr/include/SDL -u _WinMain@16
ARG_G++ = -I/usr/include/SDL

ARG = $(ARG_$(CUSE))

LIBDIR_MINGW = /usr/i586-mingw32msvc/usr/lib
LIBDIR_G++ = /usr/lib

LIB_MINGW = $(LIBDIR_MINGW)/libSDLmain.a $(LIBDIR_MINGW)/libSDL.dll.a $(LIBDIR_MINGW)/SDL_image.lib $(LIBDIR_MINGW)/SDL_ttf.lib $(LIBDIR_MINGW)/SDL_mixer.lib 
LIB_G++ = $(LIBDIR_G++)/libSDL.so $(LIBDIR_G++)/libSDL_ttf.so $(LIBDIR_G++)/libSDL_image.so $(LIBDIR_G++)/libSDL_mixer.so

LIB = $(LIB_$(CUSE))

END_G++ =
END_MINGW = .exe

END = $(END_$(CUSE))

OBJ = JaR.o

all: JaR$(END) delete

JaR$(END): $(OBJ)
	$(CXX) $(ARG) -o $@ $(OBJ) $(LIB) 

JaR.o: JaR.cpp
	$(CXX) $(ARG) -c -o $@ JaR.cpp

delete:
	rm -f *.o
