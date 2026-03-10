#pragma once
#include <string>

#include <SDL3_ttf/SDL_ttf.h>
#include "UIComponent.hpp"

//Drawable
class Text {

	std::string str_value;
	TTF_Font* font;
	float offset_x;
	float offset_y;
public:

	float x;
	float y;

	Text(): str_value(""),
		font(nullptr),
		x(0), y(0),
		offset_x(0), offset_y(0) {


	}

	explicit Text(std::string _str,TTF_Font* font,float x,float y) : 
		str_value(str_value),
		font(font),
		x(x),y(y),
		offset_x(0),offset_y(0) {


	}

	std::string& str() {
		return str_value;
	}

	ui_layer_t layer() {
	
	}

	void offset(float x, float y) {
		offset_x = x;
		offset_y = y;
	}

	uint32_t render(SDL_Renderer* renderer) {

	}

};