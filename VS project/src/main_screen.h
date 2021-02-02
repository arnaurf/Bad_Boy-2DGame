#pragma once
#pragma once
/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef STAGE_MAINSCREEN
#define STAGE_MAINSCREEN

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "stage.h"
#include "game.h"

class Stage_mainscreen : public Stage
{
public:

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
	float time;
	float elapsed_time;
	int fps;
	bool must_exit;
	//audio
	Synth synth;

	//ctor
	Stage_mainscreen();
	Stage_mainscreen(int window_width, int window_height, SDL_Window* window);

	//main functions
	void render(Image* framebuffer);
	int update(double dt);
	void reset();


	//events
	void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);
	//called constantly to fill the audio buffer
};


#endif 