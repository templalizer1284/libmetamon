#ifndef METAMON_H
#define METAMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

/* #define __inline inline __attribute__((always_inline)) extern */
#define windowTitle "Test"
#define FPS 60

#define print(x) printf("%s", x)
#define rc SDL_RenderClear(w.renderer);
#define rp SDL_RenderPresent(w.renderer);
#define render(x, y, z) SDL_RenderCopy(w.renderer, x, &y, &z)
#define renderex(x, y, z, a, b, c) SDL_RenderCopyEx(w.renderer, x, &y, &z, a, &b, c)
#define rcolor(x, y, z, a) SDL_SetRenderDrawColor(w.renderer, x, y, z, a)
#define wait(x) SDL_Delay(x)
#define NOPATH "no_path"
#define TextureConstruct(x, y) tfInit(x); tfLoad(x, y)
#define MusicConstruct(x, y) mfInit(x); mfLoad(x, y)
#define Render(x) tfRender(x)
#define Render2(x, y) tfRender(x); tfRender(y);
#define Render3(x, y, z) tfRender(x); tfRender(y); tfRender(z);
#define PlayMusic(x) mfPlay(x)
#define pe SDL_PollEvent(&game.ev)
#define Input(x) if(GetInput() == x)

/* Enums */
enum Rotation {
	CLOCKWISE,
	C_CLOCKWISE
};

enum Directions {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum ButtonStates {
	DEFAULT,
	HOVER,
	PRESSED
};

enum ErrorFlags {
	SDL,
	IMG,
	TTF,
	MIX
};

enum GameState {
	BOOT,
	MAINMENU,
	LOADGAME,
	NEWGAME,
	EXIT
};

typedef struct Game Game;
typedef struct Window Window;
typedef struct TextureAnimation TextureAnimation;
typedef struct TextureFont TextureFont;
typedef struct TextureColorKey TextureColorKey;
typedef struct TextureColorMod TextureColorMod;
typedef struct TextureBlending TextureBlending;
typedef struct TextureCollission TextureCollission;
typedef struct TextureVelocity TextureVelocity;
typedef struct TextureFloating TextureFloating;
typedef struct TextureGravity TextureGravity;
typedef struct TextureSpin TextureSpin;
typedef struct Texture Texture;
typedef struct Music Music;
typedef struct Sound Sound;
typedef struct Button Button;
typedef struct GameMap GameMap;
typedef struct Tile Tile;

struct Game {
	Uint8 state;
	bool running;
	SDL_Event ev;
	Uint32 frameStart, frameTime;
};

struct Window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Point mouse;
};

struct TextureAnimation {
	Uint8 currentFrame, numberOfFrames, speed, speed_count, delay, delay_count;
	bool enabled;
	SDL_Rect rect[16];
};

struct TextureFont {
	bool enabled;
	TTF_Font *font;
	const char *message;
	const char *path;
	SDL_Color color;
	Uint8 size;	
};

struct TextureColorMod {
	bool enabled;
	SDL_Color color;
};

struct TextureColorKey {
	bool enabled;
	SDL_Color color;
};

struct TextureBlending {
	bool enabled;
	Uint8 mode;
	Uint8 alpha;
};

struct TextureCollission {
	bool enabled;
	SDL_Rect rect[];
};

struct TextureVelocity {
	Uint8 max;
	Uint8 posx, posy;
	Uint8 velx, vely;
	Uint8 direction;
};

struct TextureFloating {
	bool enabled;
	Uint16 count, delay_count;
	Uint8 direction, delay;
};

struct TextureGravity {
	bool enabled, working;
	Uint8 direction, speed, delay, delay_count;
	Uint8 earth;
};

struct TextureSpin {
	bool enabled;
	Uint8 speed, delay, delay_count, direction;
};

struct Texture {
	SDL_Texture *texture;
	SDL_Rect srcrect;
	SDL_Rect dstrect;
	SDL_Color color;
	double angle;
	SDL_Point center;
	SDL_RendererFlip flip;
	bool shown;
	Uint8 state; // needs work
	const char *path;
	TextureFont font;
	TextureColorMod colormod;
	TextureColorKey colorkey;
	TextureBlending blending;
	TextureCollission collission;
	TextureVelocity velocity;
	TextureFloating floating;
	TextureGravity gravity;
	TextureSpin spin;
	TextureAnimation animation[16];
};

struct Music {
	Mix_Music *music;
	bool playable, paused;
};

struct Sound {
	Mix_Chunk *sound;
	bool playable;
};

/* Functions */
Uint8 Move(Texture *, Uint8);
bool isClicked(Texture *);
void TrackMouse(void);
void tfRotate(Texture *, Uint8);
void err(Uint8);
void sfInit(Sound *);
void sfLoad(Sound *, const char *);
Uint8 sfPlay(Sound *);
void sfDestroy(Sound *);
void mfInit(Music *);
void mfLoad(Music *, const char *);
Uint8 mfPlay(Music *);
void mfDestroy(Music *);
SDL_Rect FillRect(int, int, int, int);
SDL_Color FillColor(int, int, int, int);
void wfDestroy(Window *);
void wfInit(Window *, const char *, Uint16, Uint16, Uint32);
Uint16 GetInput(void);
void handleFPS(void);
void FillBackground(Uint8 , Uint8 , Uint8);
Uint8 tfFadeIn(Texture*);
Uint8 tfBlend(Texture*);
void tfDestroy(Texture*);
void tfInit(Texture *);
void tfLoad(Texture* , const char *);
Uint8 tfHandleAnimation(Texture *, Uint8);
Uint8 tfRender(Texture* );
Uint8 tfFloat(Texture *, Uint16 , Uint8 );
Uint8 tfGravity(Texture *);
Uint8 tfSpin(Texture *);

#endif
