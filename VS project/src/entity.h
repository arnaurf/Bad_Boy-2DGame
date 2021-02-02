#pragma once
#include "image.h"
#include <string>
#define CH_X 10
#define CH_Y 90
#define CH_W 20
#define CH_H 20
#define CHILD_X 90

#define ENT_Y1 104
#define HITBOX_OFFSET 2 //CHARACTER HITBOX WIDTH
//ROPA
#define ROPA_HIT_W 17
#define ROPA_SPRITE_W 20
#define ROPA_SPRITE_H 7
#define ROPA_HIT_H 7
//MANCHA
#define MANCHA_HIT_W 16
#define MANCHA_HIT_H 6
#define MANCHA_SPRITE_W 17
#define MANCHA_SPRITE_H 8

//ENEMIGOS
#define ENEMY_X 130
#define ENEMY_Y 88
#define ENEMY_HIT_W 9
#define ENEMY_HIT_H 7
#define ENEMY_SPRITE_W 10
#define ENEMY_SPRITE_H 7


#define PASS_TH 30
#define OUTSCREEN -30

#define MODE_LVL1 0
#define MODE_LOSS 1
#define MODE_WIN 2
class entity
{
public:
	float x, y;
	int img_width, img_high, hit_width, hit_high;
	static int v;
	float t_crated;
	bool skipped;
	Image texture;
	std::string name;

	entity(int x, int y, int hit_width, int hit_high, Image texture, int img_w, int img_h,std::string name);
	~entity();
	void entity::move(int new_x, int new_y);
	bool hit_detect(int charact_x, int charact_y, int w, float camera);
	bool its_over(int charact_x, int charact_y, int w, float camera);
	void print(Image* framebuffer, float camera, int x_img, int y_img);
	bool outside(float camera);

};

