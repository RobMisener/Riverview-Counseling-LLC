#ifndef FONTTEXTURE_H
#define FONTTEXTURE_H

// MemoryPalace.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

//Using SDL, SDL_image, SDL_ttf, standard IO, math, and strings
#include <sstream> // for
#include <ostream> // for
#include <windows.h>
#include <stdio.h>
#include <sqlite3.h>
#include <iostream> // for
using namespace std;

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h> //for sound

//Texture wrapper class
class FontTexture
{
public:
	//Initializes variables
	FontTexture();

	//Deallocates memory
	~FontTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture * mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

#endif