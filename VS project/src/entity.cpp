#include "entity.h"
#include <ctime>
#include <math.h>

entity::entity(int x, int y, int hit_width, int hit_high, Image texture, int img_w, int img_h, std::string name) {
	this->x = x;
	this->y = y;
	this->img_width = img_w;
	this->img_high = img_h;
	this->hit_width = hit_width;
	this->hit_high = hit_high;
	this->t_crated = clock();
	this->texture = texture;
	this->skipped = false;
	this->name = name;
}
int entity::v = 100;

entity::~entity() {

}
bool entity::hit_detect(int charact_x, int charact_y, int w, float camera) {

	if ( (charact_x > x + camera && charact_x < x +camera + hit_width && charact_y + CH_H > y && charact_y  < y + this->hit_high)
		|| (charact_x + w> x +camera && charact_x + w < x + camera + hit_width && charact_y + CH_H > y && charact_y  < y + this->hit_high) ) {
		return true;
	}
	return false;
}

void entity::move(int new_x, int new_y) {
	this->x = new_x;
	this->y = new_y;
}
bool entity::its_over(int charact_x, int charact_y, int w, float camera) {
	//if ((charact_x > x + camera && charact_x < x + camera + width && charact_y - CH_H > y)
	//	|| (charact_x + w > x + camera && charact_x + w < x + camera + width && charact_y - CH_H > y)) {
	if(charact_x > x + camera + PASS_TH){
		return true;
	}
	return false;
}


void entity::print(Image* framebuffer, float camera, int x_img, int y_img) {
	//framebuffer->drawRectangle(floor(x) + camera, floor(y), this->width, this->high, Color::RED);
	framebuffer->drawImage(this->texture,floor(x)+camera,floor(y), x_img, y_img, this->img_width, this->img_high);
	
}
bool entity::outside(float camera) {
	//Return false if the entity is outside the screen
	if (x + img_width + camera < OUTSCREEN)
		return true;
	return false;
}
