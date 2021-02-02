#include "stage.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "entity.h"

#include <cmath>
#include <list>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

Stage::Stage() {

}
Stage::Stage(int window_width, int window_height, SDL_Window* window)
{
	printf("nothing\n");
}

//what to do when the image has to be draw
void Stage::render(Image* framebuffer)
{
}

int Stage::update(double seconds_elapsed)
{
	return 0;
}

//Keyboard event handler (sync input)
void Stage::onKeyDown(SDL_KeyboardEvent event)
{
}

void Stage::onKeyUp(SDL_KeyboardEvent event)
{
}

void Stage::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Stage::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Stage::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Stage::onMouseButtonDown(SDL_MouseButtonEvent event)
{
}

void Stage::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Stage::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Stage::onResize(int width, int height)
{
}

//sends the image to the framebuffer of the GPU
void Stage::showFramebuffer(Image* img)
{
	static Image finalframe;

	if (window_width < img->width * 4 || window_height < img->height * 4)
	{
		finalframe = *img;
		finalframe.scale(window_width, window_height);
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

void Stage::reset() {

}

//AUDIO STUFF ********************

void Stage::enableAudio()
{
}

void Stage::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
}
