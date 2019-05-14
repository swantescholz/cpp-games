#standart makefile for me (S. Scholz), using SDL, SDL_ttf, SDL_image, SDL_mixer, OpenGL, GLU
CUSE = MINGW#		G++ or MINGW (linux/windows)
NAME = particle_world#	name of the programm

CMINGW = i586-mingw32msvc-g++ -std=gnu++0x -U__STRICT_ANSI__
CG++ = g++ -std=c++0x

CXX = $(C$(CUSE))

ARG_MINGW = -I/usr/i586-mingw32msvc/include -I/usr/i586-mingw32msvc/include/SDL -u _WinMain@16 -L/usr/i586-mingw32msvc/lib
ARG_G++ = -I/usr/include/SDL -I/usr/include/GL

ARG = $(ARG_$(CUSE))

#LIB_MINGW = -lSDL_ttf -lSDL_mixer -lSDL_image -lSDLmain -lSDL -lopengl32
LIB_MINGW = /usr/i586-mingw32msvc/lib/SDL_ttf.lib /usr/i586-mingw32msvc/lib/SDL_mixer.lib /usr/i586-mingw32msvc/lib/SDL_image.lib /usr/i586-mingw32msvc/lib/libSDLmain.a /usr/i586-mingw32msvc/lib/libSDL.dll.a -lopengl32
LIB_G++ = -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer -lGL

LIB = $(LIB_$(CUSE))

END_G++ =
END_MINGW = .exe

END = $(END_$(CUSE))

#enter classes here
OBJ = main.o CApp.o RoeParticle.o RoeForceField.o RoeTokenizer.o RoeApplication.o RoeAudioManager.o RoeBinFile.o RoeCamera.o RoeColor.o RoeFont.o RoeLight.o RoeLogfile.o RoeMaterial.o RoeMatrix.o RoeModel.o RoeMusic.o RoePlane.o RoeSDL_GL.o RoeSound.o RoeTextfile.o RoeTexture.o RoeTimer.o RoeUtil.o RoeVector2.o RoeVector3.o RoeXML.o

all: $(NAME)$(END) delete

$(NAME)$(END): $(OBJ)
	$(CXX) -o $@ $(ARG) $(OBJ) $(LIB) 

%.o: %.cpp
	$(CXX) -o $@ $(ARG) -c $<

delete:
	rm -f *.o
