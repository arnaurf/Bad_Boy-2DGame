#include "main_screen.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "entity.h"


#include <time.h>
#include <stdio.h>
#include <stdlib.h>

Image fontt;
Image minifontt;
Image boy;
Image girl;
Image flecha;
Image space;
Image flecha_joystick;
Image intro;
Image background;
Image text;
int phase = 0;
int selection = 0;
Stage_mainscreen::Stage_mainscreen(int window_width, int window_height, SDL_Window* window)
{

	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	fontt.loadTGA("data/bitmap-font-white.tga");
	minifontt.loadTGA("data/mini-font-white-4x6.tga");
	boy.loadTGA("data/charact.tga");
	girl.loadTGA("data/mujer.tga");
	flecha.loadTGA("data/flecha.tga");
	flecha_joystick.loadTGA("data/flechas_joystic.tga");
	space.loadTGA("data/space.tga");
	intro.loadTGA("data/intro.tga");
	background.loadTGA("data/background.tga");
	text.resize(100, 30);
}
Stage_mainscreen::Stage_mainscreen() {

}
//what to do when the image has to be draw
void Stage_mainscreen::render(Image* framebuffer)
{

	//framebuffer->fill(Color::BLACK);
	framebuffer->drawImage(background, 0, 0);


	text.fill(Color(0, 0, 0, 0));
	text.drawText("PRESS SPACE TO START", 0, 0, minifontt, 4, 6);
	text.changeAlpha(floor((clock() / 7) % 255));
	framebuffer->drawImage(text, 25, 105);
	switch (phase) {
	case 0:

		framebuffer->drawImage(intro, 20, 10);

		//framebuffer->drawRectangle(10, 100, 100, 29, Color(0, 0, 0, floor((clock() / 7) % 255)));
		break;

	case 1:

		framebuffer->drawText("SELECT PLAYER",21, 20,fontt);
		framebuffer->drawImage(boy, 35, 50, 0, 40, 20, 20);
		framebuffer->drawImage(girl, 75, 50, 0, 40, 20, 20);
		framebuffer->drawImage(flecha, 41 + 40 * selection, 40);
		framebuffer->drawText("JUMP    SWEEP    SHOOT", 14, 80, minifontt, 4, 6);
		framebuffer->drawImage(flecha_joystick, 31, 78, 0, 0, 7, 9);
		framebuffer->drawImage(flecha_joystick, 67, 78, 0, 9, 7, 9);
		framebuffer->drawImage(space, 103, 81);

		
		break;
	}
	//framebuffer->drawImage(intro, 20, 2);
	//framebuffer->drawText("SELECT PLAYER",21, 30,fontt);
}

int Stage_mainscreen::update(double dt) {
	
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) { //if key up
		if (!phase) phase = 1;
		else return (5 + selection);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT))
		selection = 1;
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT))
		selection = 0;
	return 4;
}


void Stage_mainscreen::reset() {
}
//Keyboard event handler (sync input)
void Stage_mainscreen::onKeyDown(SDL_KeyboardEvent event)
{
}

void Stage_mainscreen::onKeyUp(SDL_KeyboardEvent event)
{
}

void Stage_mainscreen::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Stage_mainscreen::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Stage_mainscreen::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Stage_mainscreen::onMouseButtonDown(SDL_MouseButtonEvent event)
{
}

void Stage_mainscreen::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Stage_mainscreen::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Stage_mainscreen::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}
