#include "stage_lvl1.h"
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

Image mujer;
Image casa;
Image fondo;
Image ropa;
Image mancha;
Image child;
Image chancla;
Image fly;
Color bgcolor(130, 80, 100);

std::vector<entity> entities;
std::vector<entity> chanclas;
std::vector<entity> enemies;
float ch_y = CH_Y;
float v0 = 0;
float acc = 0;
float dt = 0;
float bg_x = 0;
bool stop = 0;
float camera = 0;
int anim = 3;
int no_jump = 1;
float ch_x = 0;
float last_unit = 0;
float child_x = 90;
#define CHANCLA_SPEED 100

Stage_lvl1::Stage_lvl1(int window_width, int window_height, SDL_Window* window)
{
	
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	casa.loadTGA("data/casa.tga"); //example to load an sprite
	mujer.loadTGA("data/charact.tga");
	fondo.loadTGA("data/fondo.tga");
	mancha.loadTGA("data/mancha.tga");
	ropa.loadTGA("data/ropa.tga");
	child.loadTGA("data/child.tga");
	chancla.loadTGA("data/chancla.tga");
	fly.loadTGA("data/fly.tga");
	//enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/coin.wav",1,true);
	//synth.osc1.amplitude = 0.5;

}
Stage_lvl1::Stage_lvl1() {

}
//what to do when the image has to be draw
void Stage_lvl1::render(Image* framebuffer)
{
	
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	//Image framebuffer(128, 128); //do not change framebuffer size
	
	//Consecutive backgrounds
	framebuffer->drawImage(fondo, int(camera)%666, 0);
	//framebuffer->drawImage(fondo, floor(bg_x + 660) + camera, 0);

	//Print all entities
	if (entities.size() > 0) {
		for (std::vector<entity>::iterator it = entities.begin(); it != entities.end(); ++it) {
			(it)->print(framebuffer, camera, 0, 0);
		}
	}
	if (chanclas.size() > 0) {
		for (std::vector<entity>::iterator it = chanclas.begin(); it != chanclas.end(); ++it) {
			(it)->print(framebuffer, 0, 0, 4 * floor((clock() / 100) % 8)*(!stop));
		}
	}
	if(enemies.size()>0){
		for (std::vector<entity>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
			(it)->print(framebuffer, camera,0, ENEMY_SPRITE_H * floor((clock()/60) % 2));
		}
	}

	if (stop) child_x += 2; //cuando pierdes el niño se va corriendo

	//framebuffer->drawRectangle(ch_x + HITBOX_OFFSET, ch_y, CH_W - HITBOX_OFFSET*2, CH_H, Color::RED); //HITBOX
	framebuffer->drawImage(mujer, ch_x, ch_y, 20*anim, 20 * floor((clock() / 100) % 2)*no_jump, 20, 20);
	framebuffer->drawImage(child, child_x, CH_Y, 0, 20 * floor((clock() / 60) % 2), 20, 20);
	
	
}

int Stage_lvl1::update(double seconds_elapsed)
{
	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode

	// Si toquen 
	if (ch_x + CH_W + 2 * HITBOX_OFFSET >= CHILD_X) {
		return MODE_WIN;
	}


	// MOVE ALL THE ENTITIES
	//--------------------- JUMP MOVEMENT
	if (Input::wasKeyPressed(SDL_SCANCODE_UP) && dt == 0) //if key up
	{
		v0 = 38; //inicial impulse
		acc = 40; //gravity
		dt = seconds_elapsed; //time variable for position formula
		no_jump = 0;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key up
	{
		anim = 0;
	} else anim = 3;
	if (!stop) ch_y = CH_Y - v0 * dt * 3 + pow(dt * 3, 2) / 2 * acc; //position formula y = y0 +v*y + 1/2*a*t^2.
	if (ch_y >= CH_Y) {  //Stop the movement after reaching the floor.
		dt = 0;
		ch_y = CH_Y;
		no_jump = 1;
	}
	else { //otherwise, continue jumping
		dt += seconds_elapsed;
	}

	

	

	//-------------------------- Actions on every entity -------------------------------
	//Iterate over all entities. OBSTACLES
	std::vector<entity>::iterator it = entities.begin();
	std::vector< std::vector<entity>::iterator > to_delete;
	while (it != entities.end() && stop == false) { //

		//HIT DETECTION
		if ((it)->hit_detect(ch_x + HITBOX_OFFSET, floor(ch_y), CH_W - 2 * HITBOX_OFFSET, camera) &&
			!( (*it).name == "mancha" && Input::isKeyPressed(SDL_SCANCODE_DOWN)) ) {
			no_jump = 0;
			stop = true;
			return MODE_LOSS;
		}
		else if ((it)->hit_detect(ch_x + HITBOX_OFFSET - PASS_TH/2, floor(ch_y), CH_W-2*HITBOX_OFFSET,camera) && (*it).name == "mancha" && Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
			to_delete.emplace_back(it);
			ch_x += 5;
		}
		
		//entity Skip detection
		if (it->its_over(ch_x + HITBOX_OFFSET, floor(ch_y), CH_W - 2 * HITBOX_OFFSET, camera) && !it->skipped) {
			it->skipped = true;
			ch_x += 5;
		}

		//INSIDE SCREEN OR NOT (+delete)
		if ((it)->outside(camera)) { //We move the entity and test if is inside the screen
			to_delete.emplace_back(it);
		}

		it++;		
	} //end while

	//DELETE ENTITES OUT OF THE SCREEN
	std::vector< std::vector<entity>::iterator >::iterator it2;
	for (it2 = to_delete.begin(); it2 != to_delete.end(); it2++) {
		entities.erase((*it2));
	}

	//------------- ENEMIES / BULLETS DETECTIONS
	//-------------------------- Actions on every entity -------------------------------
	//Iterate over all entities.

	if (enemies.size() > 0) {
		it = enemies.begin();
		std::vector< std::vector<entity>::iterator > enemy_to_delete;
		std::vector< std::vector<entity>::iterator > bullet_to_delete;
		while (it != enemies.end() && stop == false) { //

			//HIT DETECTION WITH THE CHARACTER
			if ((it)->hit_detect(ch_x + HITBOX_OFFSET, floor(ch_y), CH_W - 2 * HITBOX_OFFSET, camera)) {
				stop = true;
				return MODE_LOSS;
			}

			if (chanclas.size() > 0) {
				for (std::vector<entity>::iterator bullet = chanclas.begin(); bullet != chanclas.end(); bullet++) {
					if (it->hit_detect(bullet->x, bullet->y, bullet->hit_width, camera)) {
						enemy_to_delete.emplace_back(it);
						bullet_to_delete.emplace_back(bullet);
						ch_x += 5;
					}
				}
			}
		
			//INSIDE SCREEN OR NOT (+delete)
			if ((it)->outside(camera)) { //We move the entity and test if is inside the screen
				to_delete.emplace_back(it);
			}

			it++;
		} //end while

		for (it2 = enemy_to_delete.begin(); it2 != enemy_to_delete.end(); it2++) {
			enemies.erase((*it2));
		}
		for (it2 = bullet_to_delete.begin(); it2 != bullet_to_delete.end(); it2++) {
			chanclas.erase((*it2));
		}
	}
	//------------------------------ ENTITIES CREATION --------TO IMPROVE!!!--------------
	//Create random entities each 2000 miliseconds
	//printf("clock: %f, entity: %f", clock(), entities.back().t_crated);
	

	// ---------- MOVE BACKGROUND----------------------
	if (!stop)	camera -= entity::v * seconds_elapsed;

	// Mover chancla. Es independiente del fondo
	if (!stop && chanclas.size() > 0) {
		for (std::vector<entity>::iterator it = chanclas.begin(); it != chanclas.end(); ++it) {
			(it)->move(it->x + seconds_elapsed * CHANCLA_SPEED, it->y);
			//printf("moving ch\n");
		}
	}

	//Crear nuevas entidades y enemigos
	if ((clock() - last_unit) > 800) {
		float random = rand() % 100;
		if (random < 25) {
			entities.push_back(*new entity(150-camera, ENT_Y1, MANCHA_HIT_W, MANCHA_HIT_H, mancha, MANCHA_SPRITE_W, MANCHA_SPRITE_H, "mancha"));
		}
		else if (random < 50) {
			entities.push_back(*new entity(150-camera, ENT_Y1, ROPA_HIT_W, ROPA_HIT_H, ropa, ROPA_SPRITE_W, ROPA_SPRITE_H, "ropa"));
		}
		else if (random < 75) {
			enemies.push_back(*new entity(ENEMY_X-camera, ENEMY_Y, ENEMY_HIT_W, ENEMY_HIT_H, fly, ENEMY_SPRITE_W, ENEMY_SPRITE_H, "fly"));
		}
		last_unit = clock();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		chanclas.push_back(*new entity(ch_x+ CH_W, ch_y+10, 4, 4, chancla, 4, 4, "chancla") );
	}
	return MODE_LVL1;
}

void Stage_lvl1::reset() {
	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	entities.clear();
	enemies.clear();
	chanclas.clear();
	ch_y = CH_Y;
	v0 = 0;
	acc = 0;
	dt = 0;
	bg_x = 0;
	stop = 0;
	camera = 0;
	anim = 3;
	ch_x = 0;
	no_jump = 1;
	child_x = 90;
}

void Stage_lvl1::select_player(int num) {
	if (num == 0) {
		mujer.loadTGA("data/charact.tga");
	}
	else if (num == 1) {
		mujer.loadTGA("data/mujer.tga");
	}
}
//Keyboard event handler (sync input)
void Stage_lvl1::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Stage_lvl1::onKeyUp(SDL_KeyboardEvent event)
{
}

void Stage_lvl1::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Stage_lvl1::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Stage_lvl1::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Stage_lvl1::onMouseButtonDown(SDL_MouseButtonEvent event)
{
}

void Stage_lvl1::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Stage_lvl1::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Stage_lvl1::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}
