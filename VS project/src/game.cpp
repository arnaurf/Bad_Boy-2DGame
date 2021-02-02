#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "entity.h"
#include "stage.h"
#include "stage_lvl1.h"
#include "main_screen.h"

#include <cmath>
#include <list>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


Game* Game::instance = NULL;

Image font;
Image minifont;
Image sprite;

Stage* stage = NULL;
Stage_lvl1* stage_1;
Stage_mainscreen* main_screen;
int mode;
//what to do when the image has to be draw
Image framebuffer(128, 128);

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image

	stage_1 = new Stage_lvl1(window_width, window_height, window);
	main_screen = new Stage_mainscreen(window_width, window_height, window);
	stage = main_screen;
	mode = 4;
	//enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/coin.wav",1,true);
	//synth.osc1.amplitude = 0.5;
}

void Game::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	
	//Image framebuffer(128, 128); //do not change framebuffer size
	
	
	switch (mode) {

	case MODE_LVL1:
		stage->render(&framebuffer);
		break;
	case MODE_LOSS:
		stage->render(&framebuffer);
		framebuffer.drawText("Game over", 30, 64, font);
		break;
	case MODE_WIN:
		framebuffer.drawText("you win", 30, 64, font);
		break;
	case 4:
		stage->render(&framebuffer);
		//printf("hye\n");
		break;		 
	}


	//send image to screen
	showFramebuffer(&framebuffer);
}

void Game::update(double seconds_elapsed)
{
	if (mode != MODE_LOSS) {
		mode = stage->update(seconds_elapsed);
	}
	
	switch (mode) {
	case 4:
		stage = main_screen;
		break;
	case 5:
	case 6:
		stage_1->select_player(mode - 5);
		stage = stage_1;
		break;
	default:
		stage = stage_1;
		break;
	}
	//printf("Mode %i\n", mode);
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) {
		(*stage_1).reset();
		mode = 4;
	}
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{

	static Image finalframe;

	if (window_width < img->width * 4 || window_height < img->height * 4)
	{
		finalframe = *img;
		finalframe.scale( window_width, window_height );
	}
	else
	{
		if (finalframe.width != window_width || finalframe.height != window_height)
		{
			finalframe.resize(window_width, window_height);
			finalframe.fill(Color::BLACK);
		}
		finalframe.drawImage(*img, (window_width - img->width * 4) * 0.5, (window_height - img->height * 4) * 0.5, img->width * 4, img->height * 4);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
		glRasterPos2f(-1, 1);
		glPixelZoom(1, -1);
	}

	glDrawPixels(finalframe.width, finalframe.height, GL_RGBA, GL_UNSIGNED_BYTE, finalframe.pixels);
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
