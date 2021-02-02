#pragma once
#pragma once
/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef STAGE
#define STAGE

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"

class Stage
{
public:
	static Stage* instance;

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
	Stage(int window_width, int window_height, SDL_Window* window);
	Stage();
	//main functions
	virtual void render(Image* framebuffer);
	virtual int update(double dt);

	void showFramebuffer(Image* img);

	//events
	virtual void onKeyDown(SDL_KeyboardEvent event);
	virtual void onKeyUp(SDL_KeyboardEvent event);
	virtual void onMouseButtonDown(SDL_MouseButtonEvent event);
	virtual void onMouseButtonUp(SDL_MouseButtonEvent event);
	virtual void onMouseMove(SDL_MouseMotionEvent event);
	virtual void onMouseWheel(SDL_MouseWheelEvent event);
	virtual void onGamepadButtonDown(SDL_JoyButtonEvent event);
	virtual void onGamepadButtonUp(SDL_JoyButtonEvent event);
	virtual void onResize(int width, int height);
	virtual void reset();

	//audio stuff
	void enableAudio(); //opens audio channel to play sound
	void onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec &audio_spec); //called constantly to fill the audio buffer
};


#endif 