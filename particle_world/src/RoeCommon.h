#ifndef ROECOMMON_H_
#define ROECOMMON_H_

#include <SDL/SDL.h>
#include <string>
#include <iostream>
using namespace std;

//RoeCommon.h
//this file contains useful and general defines, enums, typedefs and constants

/* for copy-and-past
std::cout <<  << std::endl;
std::cout << "1" << std::endl;
std::cout << "a" << std::endl;
*/

//useful defines

// *switches*
//#define ROE_COMPILE_PHYSICS
//#define ROE_COMPILE_AUDIO
//#define ROE_COMPILE_SYSTEM_SPECIFICS

//operating system
#ifdef __linux
	#define ROE_OS_LINUX
#else
#ifdef _WIN32
	#define ROE_OS_WINDOWS
#endif
#endif

#ifdef ROE_OS_LINUX
	#ifdef ROE_OS_WINDOWS
		#error "two operating systems cannot been defined!"
	#endif
#else
	#ifdef ROE_OS_WINDOWS
		
	#else
		#error "no operating system defined!"
	#endif
#endif

#ifdef ROE_OS_WINDOWS
	#include <windows.h>
#endif

#define ROE_REAL_TOLERANCE 0.000001                 //general tolerance for Real(float) numbers
#define ROE_PI 3.14159265359                        //pi
#define ROE_DEG_TO_RAD(x) ((x) * 0.0174532925199)   //transforms a degree value to radian
#define ROE_RAD_TO_DEG(x) ((x) * 57.295779513082)   //transforms a radian value to degree
#define ROE_SINGP(x)      ((x)::getSingletonPtr())  //returns singleton pointer of a singleton class
#define ROE_SAFE_DELETE(x)             {if((x)) {delete   (x); (x) = NULL;}}
#define ROE_SAFE_DELETE_ARRAY(x)       {if((x)) {delete[] (x); (x) = NULL;}}
#define nullptr NULL//TODO

//power of 2 macros for flags
#define ROE_FLAG_NONE 0x00000000 //->0
#define ROE_FLAG_ALL 0xffffffff //->4294967295
#define ROE_POW_2_0  0x00000001 //->1
#define ROE_POW_2_1  0x00000002 //->2
#define ROE_POW_2_2  0x00000004 //->4
#define ROE_POW_2_3  0x00000008 //->8
#define ROE_POW_2_4  0x00000010 //->16
#define ROE_POW_2_5  0x00000020 //->32
#define ROE_POW_2_6  0x00000040 //->64
#define ROE_POW_2_7  0x00000080 //->128
#define ROE_POW_2_8  0x00000100 //->256
#define ROE_POW_2_9  0x00000200 //->512
#define ROE_POW_2_10 0x00000400 //->1024
#define ROE_POW_2_11 0x00000800 //->2048
#define ROE_POW_2_12 0x00001000 //->4096
#define ROE_POW_2_13 0x00002000 //->8192
#define ROE_POW_2_14 0x00004000 //->16384
#define ROE_POW_2_15 0x00008000 //->32768
#define ROE_POW_2_16 0x00010000 //->65536
#define ROE_POW_2_17 0x00020000 //->131072
#define ROE_POW_2_18 0x00040000 //->262144
#define ROE_POW_2_19 0x00080000 //->524288
#define ROE_POW_2_20 0x00100000 //->1048576
#define ROE_POW_2_21 0x00200000 //->2097152
#define ROE_POW_2_22 0x00400000 //->4194304
#define ROE_POW_2_23 0x00800000 //->8388608
#define ROE_POW_2_24 0x01000000 //->16777216
#define ROE_POW_2_25 0x02000000 //->33554432
#define ROE_POW_2_26 0x04000000 //->67108864
#define ROE_POW_2_27 0x08000000 //->134217728
#define ROE_POW_2_28 0x10000000 //->268435456
#define ROE_POW_2_29 0x20000000 //->536870912
#define ROE_POW_2_30 0x40000000 //->1073741824
#define ROE_POW_2_31 0x80000000 //->2147483648


namespace Roe {
	
	typedef std::string     String;    // maybe self-made class later
	typedef float           Degree;  //typedef for pointing out that a value should be in degrees
	typedef float           Radian;  //typedef for pointing out that a value should be in radian
	
	//typedefs for bytes and flags
	typedef unsigned char  byte;
	typedef unsigned short byte2;
	typedef unsigned long  byte4;
	typedef unsigned char  flag8;
	typedef unsigned short flag16;
	typedef unsigned long  flag32;
	
	enum ECWCCW {
		 CW_CW  = 1  //clockwise direction
		,CW_CCW = 2 //counter-clockwise direction
	};
	
	// *useful enums*
	enum  EKeyCode{ // KEY CODES
		KC_ESCAPE       = SDLK_ESCAPE,
		KC_TAB          = SDLK_TAB,
		KC_RETURN       = SDLK_RETURN,
		KC_BACKSPACE    = SDLK_BACKSPACE,
		KC_SPACE        = SDLK_SPACE,
		KC_RSHIFT       = SDLK_RSHIFT,
		KC_LSHIFT       = SDLK_LSHIFT,
		KC_RCTRL        = SDLK_RCTRL,
		KC_LCTRL        = SDLK_LCTRL,
		KC_RALT         = SDLK_RALT,
		KC_LALT         = SDLK_LALT,
		KC_COMMA        = SDLK_COMMA,
		KC_PERIOD       = SDLK_PERIOD,
		
		KC_A            = SDLK_a,
		KC_B            = SDLK_b,
		KC_C            = SDLK_c,
		KC_D            = SDLK_d,
		KC_E            = SDLK_e,
		KC_F            = SDLK_f,
		KC_G            = SDLK_g,
		KC_H            = SDLK_h,
		KC_I            = SDLK_i,
		KC_J            = SDLK_j,
		KC_K            = SDLK_k,
		KC_L            = SDLK_l,
		KC_M            = SDLK_m,
		KC_N            = SDLK_n,
		KC_O            = SDLK_o,
		KC_P            = SDLK_p,
		KC_Q            = SDLK_q,
		KC_R            = SDLK_r,
		KC_S            = SDLK_s,
		KC_T            = SDLK_t,
		KC_U            = SDLK_u,
		KC_V            = SDLK_v,
		KC_W            = SDLK_w,
		KC_X            = SDLK_x,
		KC_Y            = SDLK_y,
		KC_Z            = SDLK_z,
		KC_0            = SDLK_0, // numbers in normal keyboard range
		KC_1            = SDLK_1,
		KC_2            = SDLK_2,
		KC_3            = SDLK_3,
		KC_4            = SDLK_4,
		KC_5            = SDLK_5,
		KC_6            = SDLK_6,
		KC_7            = SDLK_7,
		KC_8            = SDLK_8,
		KC_9            = SDLK_9,
		KC_F1           = SDLK_F1,
		KC_F2           = SDLK_F2,
		KC_F3           = SDLK_F3,
		KC_F4           = SDLK_F4,
		KC_F5           = SDLK_F5,
		KC_F6           = SDLK_F6,
		KC_F7           = SDLK_F7,
		KC_F8           = SDLK_F8,
		KC_F9           = SDLK_F9,
		KC_F10          = SDLK_F10,
		KC_F11          = SDLK_F11,
		KC_F12          = SDLK_F12,
		KC_F13          = SDLK_F13,
		KC_F14          = SDLK_F14,
		KC_F15          = SDLK_F15,
		
		KC_KP_0         = SDLK_KP0, // keypad / numpad
		KC_KP_1         = SDLK_KP1,
		KC_KP_2         = SDLK_KP2,
		KC_KP_3         = SDLK_KP3,
		KC_KP_4         = SDLK_KP4,
		KC_KP_5         = SDLK_KP5,
		KC_KP_6         = SDLK_KP6,
		KC_KP_7         = SDLK_KP7,
		KC_KP_8         = SDLK_KP8,
		KC_KP_9         = SDLK_KP9,
		KC_KP_DIVIDE    = SDLK_KP_DIVIDE,
		KC_KP_MULTIPLY  = SDLK_KP_MULTIPLY,
		KC_KP_PLUS      = SDLK_KP_PLUS,
		KC_KP_MINUS     = SDLK_KP_MINUS,
		KC_KP_ENTER     = SDLK_KP_ENTER,
		
		KC_ARR_INSERT   = SDLK_INSERT,
		KC_ARR_DELETE   = SDLK_DELETE,
		KC_ARR_HOME     = SDLK_HOME,
		KC_ARR_END      = SDLK_END,
		KC_ARR_PGUP     = SDLK_PAGEUP,
		KC_ARR_PGDOWN   = SDLK_PAGEDOWN,
		KC_ARR_UP       = SDLK_UP,
		KC_ARR_DOWN     = SDLK_DOWN,
		KC_ARR_LEFT     = SDLK_LEFT,
		KC_ARR_RIGHT    = SDLK_RIGHT
		
	}; //KEY CODES
	
	enum EMouseButton { // MOUSE BUTTONS
		MB_L        = SDL_BUTTON_LEFT,
		MB_R        = SDL_BUTTON_RIGHT,
		MB_M        = SDL_BUTTON_MIDDLE,
		MB_WHEELUP      = SDL_BUTTON_WHEELUP,
		MB_WHEELDOWN    = SDL_BUTTON_WHEELDOWN
	}; // MOUSE BUTTONS
	
	
	
} // namespace Roe

#endif /*ROECOMMON_H_*/
