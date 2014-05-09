//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//wielkoœæ okna
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//iloœæ i wielkoœæ buttonów
const int BUTTON_WIDTH = 30;
const int BUTTON_HEIGHT = 30;
const int TOTAL_BUTTONS = 210;

enum LButtonSprite
{
	Woda = 3,
	Trafiony_statek = 1,
	Trafiona_woda = 0,
	Statek = 2,
	BUTTON_SPRITE_TOTAL = 4
};

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

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
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e, int i);

	//Shows button sprite
	void render();

public:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};


int flaga_statek;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Mouse button sprites
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[TOTAL_BUTTONS];
int ButtonZ[TOTAL_BUTTONS];
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void komp();

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = Woda;
}

int wyb_statku(int i) //wybieranie statku do ustawiania
{
	if (i >= 200)
	{
		if ((i >= 200) && (i<204))
		{
			flaga_statek = 4;
		}
		if ((i >= 204) && (i<207))
		{
			flaga_statek = 3;
		}
		if ((i >= 207) && (i<209))
		{
			flaga_statek = 2;
		}
		if (i == 209)
		{
			flaga_statek = 1;
		}
	}
	return flaga_statek;
}


void strzelanie(int i, bool flaga) //strzelanie gracza i kompa
{
	if ((i>99) && (i<200))
	{
		if (ButtonZ[i] == 0)
		{
			gButtons[i].mCurrentSprite = Trafiona_woda;
			ButtonZ[i] = 1;
			flaga = true;
		}
		if (ButtonZ[i] == 2)
		{
			gButtons[i].mCurrentSprite = Trafiony_statek;
			ButtonZ[i] = 1;
			flaga = true;
		}
		//na razie nie mam leprzego pomys³u na strza³y kompa
		if (flaga == true)
		{
			komp();
			flaga = false;
		}
		for (int i = 0; i<100; i++)
		{
			if (ButtonZ[i] == 10)
			{
				gButtons[i].mCurrentSprite = Trafiona_woda;
				ButtonZ[i] = 1;
			}
			if (ButtonZ[i] == 20)
			{
				gButtons[i].mCurrentSprite = Trafiony_statek;
				ButtonZ[i] = 1;
			}
		}
	}
}

void ustaw_statek(int i, int y)
{
	if (i<100){
		if (y>0){
			switch (y)
			{
			case 1:
				gButtons[i].mCurrentSprite = Statek;
				ButtonZ[i] = 2;
				break;

			case 2:
				gButtons[i].mCurrentSprite = Statek;
				gButtons[i + 1].mCurrentSprite = Statek;
				ButtonZ[i] = 2;
				ButtonZ[i + 1] = 2;
				break;

			case 3:
				gButtons[i].mCurrentSprite = Statek;
				gButtons[i + 1].mCurrentSprite = Statek;
				gButtons[i + 2].mCurrentSprite = Statek;
				ButtonZ[i] = 2;
				ButtonZ[i + 1] = 2;
				ButtonZ[i + 2] = 2;
				break;

			case 4:
				gButtons[i].mCurrentSprite = Statek;
				gButtons[i + 1].mCurrentSprite = Statek;
				gButtons[i + 2].mCurrentSprite = Statek;
				gButtons[i + 3].mCurrentSprite = Statek;
				ButtonZ[i] = 2;
				ButtonZ[i + 1] = 2;
				ButtonZ[i + 2] = 2;
				ButtonZ[i + 3] = 2;
				break;
			}
		}
	}
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e, int i)
{
	bool flaga = false;

	//If mouse event happened
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{

				//if(x==0)
				//{
				//strzelanie(i,flaga);

				//}
				//if(x>0)
				//{
				int x;
				int licznik = 0;
				while (licznik<10)
				{
					x = wyb_statku(i);
					ustaw_statek(i, x);
					licznik++;
				}
				strzelanie(i, flaga);

				//}


			}
		}


	}
}

void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Statki", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!gButtonSpriteSheetTexture.loadFromFile("images/kkk.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites

		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * BUTTON_WIDTH;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		for (int i = 0; i<10; i++)
		{
			for (int j = 0; j<10; j++)
			{
				gButtons[(j * 10) + i].setPosition(0 + (i*BUTTON_WIDTH), 0 + (j*BUTTON_HEIGHT));
			}
		}
		for (int i = 0; i<10; i++)
		{
			for (int j = 0; j<10; j++)
			{
				gButtons[100 + ((j * 10) + i)].setPosition(SCREEN_WIDTH - (i*BUTTON_WIDTH) - BUTTON_WIDTH, 0 + (j*BUTTON_HEIGHT));
			}
		}
		for (int i = 0; i<4; i++)
		{
			gButtons[200 + i].setPosition(2 * BUTTON_WIDTH + i*BUTTON_WIDTH, 12 * BUTTON_HEIGHT);
			gButtons[200 + i].mCurrentSprite = Statek;
		}
		for (int i = 0; i<3; i++)
		{
			gButtons[204 + i].setPosition(3 * BUTTON_WIDTH + i*BUTTON_WIDTH, 13 * BUTTON_HEIGHT);
			gButtons[204 + i].mCurrentSprite = Statek;
		}
		for (int i = 0; i<2; i++)
		{
			gButtons[207 + i].setPosition(4 * BUTTON_WIDTH + i*BUTTON_WIDTH, 14 * BUTTON_HEIGHT);
			gButtons[207 + i].mCurrentSprite = Statek;
		}

		gButtons[209].setPosition(5 * BUTTON_WIDTH, 15 * BUTTON_HEIGHT);
		gButtons[209].mCurrentSprite = Statek;



	}

	return success;
}

void close()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

//strzelanie kompa

void komp(){
	int z;
	do
	{
		z = (rand() % 100);
	} while (ButtonZ[z] == 1);

	if (ButtonZ[z] == 0){
		ButtonZ[z] = 10;
	}
	if (ButtonZ[z] == 2)
	{
		ButtonZ[z] = 20;
	}
}


int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;
			for (int i = 0; i<TOTAL_BUTTONS; i++)
			{
				ButtonZ[i] = 0;

			}
			ButtonZ[10] = 2;
			gButtons[10].mCurrentSprite = Statek;
			ButtonZ[103] = 2;
			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle button events
					for (int i = 0; i < TOTAL_BUTTONS; ++i)
					{

						gButtons[i].handleEvent(&e, i);

					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render buttons
				for (int i = 0; i < TOTAL_BUTTONS; ++i)
				{
					gButtons[i].render();
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}