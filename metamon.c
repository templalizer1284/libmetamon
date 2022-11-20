#include "metamon.h"

Game game = { BOOT, true };
Window w;

void err(Uint8 e){
	switch(e){
	case SDL:
		printf("SDL error occured: %s\n", SDL_GetError());
		break;
	case IMG:
		printf("IMG error occured: %s\n", IMG_GetError());
		break;
	case TTF:
		printf("TTF error occured: %s\n", TTF_GetError());
		break;
	case MIX:
		printf("MIX error occured: %s\n", Mix_GetError());
		break;
	}
}

// MUSIC
void sfInit(Sound *s){
	s->sound = NULL;
	s->playable = false;
}

void sfLoad(Sound *s, const char *path){
	if( (s->sound = Mix_LoadWAV(path)) == NULL ){
		print("Mix_LoadWAV - ");
		err(MIX);
	}
}

Uint8 sfPlay(Sound *s){
	if(s->playable == false){
		return 1;
	}

	if( (Mix_PlayChannel(-1, s->sound, 0)) == -1 ){
		print("Mix_PlayChannel - ");
		err(MIX);
	}

	return 0;
}

void sfDestroy(Sound *s){
	Mix_FreeChunk(s->sound);
	s->sound = NULL;
}

void mfInit(Music *m){
	m->music = NULL;
	m->playable = 1;
	m->paused = 0;
}

void mfLoad(Music *m, const char *path){
	if( (m->music = Mix_LoadMUS(path)) == NULL ){
		print("Mix_LoadMUS - ");
		err(MIX);
	}
}

Uint8 mfPlay(Music *m){
	if( m->playable == false ){
		return 1;
	}

	if( Mix_PlayingMusic() == 0){
		if( Mix_PlayMusic(m->music, 1) != 0 ){
			print("Mix_PlayMusic - ");
			err(MIX);
		}
	}

	return 0;
}

void mfDestroy(Music *m){
	Mix_FreeMusic(m->music);
	m->music = NULL;
}

SDL_Rect FillRect(int x, int y, int w, int h){
	SDL_Rect r = { x, y, w, h };
	return r;
}

SDL_Color FillColor(int r, int g, int b, int a){
	SDL_Color c = { r, g, b, a };
	return c;
}

void wfDestroy(Window *w){
	SDL_DestroyRenderer(w->renderer);
	w->renderer = NULL;
	SDL_DestroyWindow(w->window);
	w->window = NULL;
	IMG_Quit();
	SDL_Quit();
}

void wfInit(Window *w, const char *title, Uint16 WINW, Uint16 WINH, Uint32 flag){
	if( (SDL_Init(SDL_INIT_VIDEO)) != 0 ){
		print("SDL_Init - ");
		err(SDL);
	}

	if( (TTF_Init()) == -1 ){
		printf("TTF_Init - ");
		err(TTF);
	}

	int img = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	if( !img ){
		print("IMG_Init - ");
		err(IMG);
	}

	if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1 ){
		print("Mix_OpenAudio - ");
		err(MIX);
	}

	w->window = SDL_CreateWindow(title, 0, 0, WINW, WINH, flag);

	if(w->window == NULL){
		print("SDL_CreateWindow - ");
		err(SDL);
	}

	w->renderer = SDL_CreateRenderer(w->window, -1, SDL_RENDERER_ACCELERATED |
					 SDL_RENDERER_PRESENTVSYNC);

	if( w->renderer == NULL ){
		print("SDL_CreateRenderer - ");
		err(SDL);
	}

	if( (SDL_RenderSetLogicalSize(w->renderer, WINW, WINH)) != 0 ){
		print("SDL_RenderSetLogicalSize - ");
		err(SDL);
	}

	if( (SDL_RenderSetIntegerScale(w->renderer, SDL_TRUE)) != 0 ){
		print("SDL_RenderSetIntegerScale - ");
		err(SDL);
	}
}

Uint16 GetInput(){
		const Uint8 *state = SDL_GetKeyboardState( NULL );
		if(state[SDL_SCANCODE_LSHIFT] && state[SDL_SCANCODE_Q]){
			return SDL_QUIT;
		}
		if(state[SDL_SCANCODE_W]){
			return SDL_SCANCODE_W;
		}
		if(state[SDL_SCANCODE_D]){
			return SDL_SCANCODE_D;
		}
		if(state[SDL_SCANCODE_A]){
			return SDL_SCANCODE_A;
		}
		if(state[SDL_SCANCODE_UP]){
			return SDL_SCANCODE_UP;
		}
		if(state[SDL_SCANCODE_DOWN]){
			return SDL_SCANCODE_DOWN;
		}
		if(state[SDL_SCANCODE_LEFT]){
			return SDL_SCANCODE_LEFT;
		}
		if(state[SDL_SCANCODE_RIGHT]){
			return SDL_SCANCODE_RIGHT;
		}
		if(state[SDL_SCANCODE_SPACE]){
			return SDL_SCANCODE_SPACE;
		}

		if(game.ev.type == SDL_MOUSEBUTTONDOWN || game.ev.type == SDL_MOUSEBUTTONUP){
			if(game.ev.button.button == SDL_BUTTON_LEFT){
				return SDL_BUTTON_LEFT;
			}
			if(game.ev.button.button == SDL_BUTTON_RIGHT){
				return SDL_BUTTON_RIGHT;
			}
			if(game.ev.button.button == SDL_BUTTON_MIDDLE){
				return SDL_BUTTON_MIDDLE;
			}
		}

		return 0;
	}

void handleFPS(){
	if( game.frameTime < 1000/FPS ){
		SDL_Delay(1000/FPS - game.frameTime);
	}
}

Uint8 tfFadeIn(Texture *t){
	if( t->blending.alpha < 60 ){
		t->blending.alpha++;
		if( (SDL_SetTextureAlphaMod(t->texture, t->blending.alpha)) != 0 ){
			print("SDL_SetTextureAlphaMod - ");
			err(SDL);
		}
		return 0;
	}else{
		return 1;
	}
}

Uint8 tfFadeOut(Texture *t){
	if( t->blending.alpha != 0 ){
		t->blending.alpha--;
		if( (SDL_SetTextureAlphaMod(t->texture, t->blending.alpha)) != 0 ){
			print("SDL_SetTextureAlphaMod - ");
			err(SDL);
		}
		return 0;
	}else{
		return 1;
	}
}


Uint8 tfBlend(Texture *t){
	if( t->blending.enabled == false ){
		if( (SDL_SetTextureBlendMode(t->texture, SDL_BLENDMODE_NONE)) != 0 ){
			print("SDL_SetTextureBlendMode - ");
			err(SDL);
		}
		return 1;
	}
	
	if( (SDL_SetTextureBlendMode(t->texture, SDL_BLENDMODE_BLEND)) != 0){
		print("SDL_SetTextureBlendMode - ");
		err(SDL);
	}

	return 0;
}

void tfDestroy(Texture *t){
	SDL_DestroyTexture(t->texture);
	TTF_CloseFont(t->font.font);
	t->texture = NULL;
	t->font.font = NULL;
}

void tfLoad(Texture *t, const char *path){
	t->path = path;

	if( t->font.enabled == true ){
		if( (t->font.font = TTF_OpenFont( t->font.path, t->font.size )) == NULL){
			print("TTF_OpenFont - ");
			err(TTF);
		}

		if( (w.surface = TTF_RenderText_Solid(t->font.font, t->font.message,
						      t->font.color)) == NULL){
			print("TTF_RenderText_Solid - ");
			err(TTF);
		}
		
		goto WITH_TTF;
	}

	if( (w.surface = IMG_Load(path)) == NULL ){
		print("IMG_Load - ");
		err(IMG);
	}

WITH_TTF:
	if( t->colorkey.enabled == true ){
		t->colorkey.enabled = false;
		if( (SDL_SetColorKey(w.surface, SDL_TRUE, SDL_MapRGB(w.surface->format,
								     t->colorkey.color.r,
								     t->colorkey.color.g,
								     t->colorkey.color.b ))) != 0 ){
			print("SDL_SetColorKey");
			err(SDL);
		}
	}

	if( (t->texture = SDL_CreateTextureFromSurface(w.renderer, w.surface)) == NULL ){
		print("SDL_CreateTextureFromSurface - ");
		err(SDL);
	}

	SDL_QueryTexture(t->texture, NULL, NULL, &t->srcrect.w, &t->srcrect.h);
	SDL_QueryTexture(t->texture, NULL, NULL, &t->dstrect.w, &t->dstrect.h);

	SDL_FreeSurface(w.surface);
}

Uint8 tfHandleAnimation(Texture *t, Uint8 anim_num){
	if(t->animation[anim_num].enabled == true){
		t->srcrect = t->animation[anim_num].rect[t->animation[anim_num].currentFrame/t->animation[anim_num].numberOfFrames];
		t->dstrect.w = t->animation[anim_num].rect[t->animation[anim_num].currentFrame/t->animation[anim_num].numberOfFrames].w;
		t->dstrect.h = t->animation[anim_num].rect[t->animation[anim_num].currentFrame/t->animation[anim_num].numberOfFrames].h;
	}

	else if(t->animation[anim_num].enabled == false){
		return 1;
	}

	if(t->animation[anim_num].delay > 0){
		if(t->animation[anim_num].delay_count == t->animation[anim_num].delay){
			t->animation[anim_num].delay_count = 0;
			goto MOVE;
		}
		t->animation[anim_num].delay_count++;
		return 1;
	}
	
MOVE:
	if(t->animation[anim_num].currentFrame == t->animation[anim_num].numberOfFrames * t->animation[anim_num].numberOfFrames){
		t->animation[anim_num].currentFrame = 0;
	}

	t->srcrect = t->animation[anim_num].rect[t->animation[anim_num].currentFrame/t->animation[anim_num].numberOfFrames];
	t->dstrect.w = t->animation[anim_num].rect[t->animation[anim_num].currentFrame/t->animation[anim_num].numberOfFrames].w;
	t->dstrect.h = t->animation[anim_num].rect[t->animation[anim_num].currentFrame/t->animation[anim_num].numberOfFrames].h;

	if((t->animation[anim_num].speed_count++) == t->animation[anim_num].speed){
		t->animation[anim_num].speed_count = 0;
		t->animation[anim_num].currentFrame++;
	}
	
	return 0;
}

Uint8 tfRender(Texture *t){
	if( t->shown == false ){
		return 1;
	}

	if( t->colormod.enabled == true ){
		if( (SDL_SetTextureColorMod(t->texture, t->colormod.color.r, t->colormod.color.g, t->colormod.color.b)) != 0 ){
			print("SDL_SetTextureColorMod - ");
			err(SDL);
		}
	}

	if( (renderex(t->texture, t->srcrect, t->dstrect, t->angle, t->center, t->flip)) != 0 ){
		print("SDL_RenderCopyEx - ");
		err(SDL);
	}

	return 0;
}

void tfRotate(Texture *t, Uint8 d){
	switch(d){
	case CLOCKWISE:
		t->angle++;
		break;
	case C_CLOCKWISE:
		t->angle--;
		break;
	}
}

void tfInit(Texture *t){
	t->texture = NULL;
	t->srcrect = FillRect(0, 0, 0, 0);
	t->dstrect = FillRect(0, 0, 0, 0);
	t->color = FillColor(0, 0, 0, 0);
	t->angle = 0;
	t->center.x = 0;
	t->center.y = 0;
	t->flip = SDL_FLIP_NONE;
	t->shown = true;
	t->state = 0;
	t->path = "no_path";
	for(Uint8 x = 0; x < 16; x++){
		t->animation[x].numberOfFrames = 0;
		t->animation[x].currentFrame = 0;
		t->animation[x].speed = 0;
		t->animation[x].speed_count = 0;
		t->animation[x].delay = 0;
		t->animation[x].delay_count = 0;
		t->animation[x].enabled = false;
		x++;
	}
	t->font.enabled = false;
	t->font.font = NULL;
	t->font.message = "no_message";
	t->font.path = "no_path";
	t->font.color = FillColor(0, 0, 0, 0);
	t->font.size = 0;
	t->colormod.enabled = false;
	t->colormod.color = FillColor(0, 0, 0, 0);
	t->colorkey.enabled = false;
	t->colorkey.color = FillColor(0, 0, 0, 0);
	t->blending.enabled = false;
	t->blending.mode = SDL_BLENDMODE_NONE;
	t->blending.alpha = 0;
	t->collission.enabled = false;
	t->velocity.max = 0;
	t->velocity.posx = 0;
	t->velocity.posy = 0;
	t->velocity.velx = 0;
	t->velocity.vely = 0;
	t->velocity.direction = 5;
	t->floating.enabled = false;
	t->floating.count = 0;
	t->floating.delay_count = 0;
	t->floating.direction = DOWN;
	t->gravity.enabled = false;
	t->gravity.speed = 0;
	t->gravity.direction = DOWN;
	t->gravity.delay = 0;
	t->gravity.delay_count = 0;
	t->gravity.earth = 0;
	t->gravity.working = false;
	t->spin.enabled = false;
	t->spin.speed = 0;
	t->spin.delay = 0;
	t->spin.delay_count = 0;
	t->spin.direction = 5;
}

void TrackMouse(){
	SDL_GetMouseState(&w.mouse.x, &w.mouse.y);
}

bool isClicked(Texture *t){
	TrackMouse();
	if(w.mouse.x > t->dstrect.x && w.mouse.x < (t->dstrect.x + t->dstrect.w)){
		if(w.mouse.y > t->dstrect.y && w.mouse.y < (t->dstrect.y + t->dstrect.h)){
			return true;
		}
	}

	return false;
}

Uint8 tfFloat(Texture *t, Uint16 float_size, Uint8 speed){
	if(t->floating.enabled == false){
		return 1;
	}

	if(t->floating.delay_count == t->floating.delay){
		t->floating.delay_count = 0;
		goto MOVE;
	}
	t->floating.delay_count++;
	return 1;

MOVE:
	switch(t->floating.direction){
	case DOWN:
		if(t->floating.count == float_size){
			t->floating.direction = UP;
			t->floating.count = 0;
		}
		t->dstrect.y += speed;
		break;
	case UP:
		if(t->floating.count == float_size){
			t->floating.direction = DOWN;
			t->floating.count = 0;
		}
		t->dstrect.y -= speed;
		break;
	}

	t->floating.count++;

	return 0;
}

Uint8 tfGravity(Texture *t){
	if(t->gravity.enabled == false){
		return 1;
	}

	t->gravity.working = true;

	if(t->gravity.delay_count == t->gravity.delay){
		t->gravity.delay_count = 0;
		goto MOVE;
	}
	t->gravity.delay_count++;
	return 1;

MOVE:
	if( (t->dstrect.y + t->dstrect.h) == t->gravity.earth ){
		t->gravity.working = false;
		return 1;
	}

	switch(t->gravity.direction){
	case DOWN:
		t->dstrect.y += t->gravity.speed;
		break;
	case UP:
		t->dstrect.y -= t->gravity.speed;
		break;
	}

	return 0;
}

Uint8 tfSpin(Texture *t){
	if(t->spin.enabled == false){
		return 1;
	}
	
	if(t->spin.delay_count == t->spin.delay){
		t->spin.delay_count = 0;
		goto MOVE;
	}
	t->spin.delay_count++;
	return 1;

MOVE:
	switch(t->spin.direction){
	case LEFT:
		t->angle -= t->spin.speed;
		break;
	case RIGHT:
		t->angle += t->spin.speed;
		break;
	}

	return 0;
}
