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

LIB_MINGW = $(LIBDIR_MINGW)/libSDLmain.a $(LIBDIR_MINGW)/libSDL.dll.a  $(LIBDIR_MINGW)/SDL_ttf.lib
LIB_G++ = $(LIBDIR_G++)/libSDL.so $(LIBDIR_G++)/libSDL_ttf.so

LIB = $(LIB_$(CUSE))

END_G++ =
END_MINGW = .exe

END = $(END_$(CUSE))

OBJ =object_simulator.o

all:object_simulator$(END) delete

object_simulator$(END): $(OBJ)
	$(CXX) $(ARG) -o $@ $(OBJ) $(LIB) 

object_simulator.o: object_simulator.cpp
	$(CXX) $(ARG) -c -o $@ object_simulator.cpp

delete:
	rm -f *.o
