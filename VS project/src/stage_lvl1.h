#pragma once
/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef STAGE_LVL1
#define STAGE_LVL1

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "stage.h"

class Stage_lvl1: public Stage
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
	Stage_lvl1();
	Stage_lvl1(int window_width, int window_height, SDL_Window* window);

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
	void select_player(int num);
//called constantly to fill the audio buffer
};


#endif 