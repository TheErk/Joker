#include <QtGlobal>

#include <SDL2/SDL.h>
#if defined(Q_OS_MAC)
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif


#include <string>
#include <cstring>
#include "string.h"
#include "iostream"
//#include "SDLMain.h"

#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QApplication>


#include "PhTools/PhDebug.h"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *image = NULL;
SDL_Surface *foo = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *message = NULL;

//The event
SDL_Event event;

using namespace std;

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	PHDEBUG << "SDL_Init()";
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

	PHDEBUG << "SDL_SetVideoMode";
    //Set up the screen

	SDL_Window * window = SDL_CreateWindow("SDLTest", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_SWSURFACE);

	screen = SDL_GetWindowSurface(window);

	PHDEBUG << "SDL_WM_SetCaption";

	PHDEBUG << "current path : " << QApplication::applicationDirPath();
	string ressourcesPath = QApplication::applicationDirPath().toStdString();
	PHDEBUG << "load_image";
    // Create a surface from picture:
	string lookPath = ressourcesPath + "/look.png";
	image = IMG_Load( lookPath.c_str());

    if( image == NULL )
    {
        return 1;
    }

    // Display the picture:
	SDL_Rect imageRect = {0, 0, image->w, image->h};

	SDL_BlitSurface( image, NULL, screen, &imageRect );

    // Initialize TTF :
    if( TTF_Init() == -1 ) {
        PHDEBUG << "TTF error.";
        return 2;
    }

    //Create a font
	string fontPath = ressourcesPath + "/Bedizen.ttf";
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), 100 );
    if (font == NULL)
        return 3;

    //Font's color
    SDL_Color textColor = { 0, 0, 255, 1 };

    // Create a text surface:
    SDL_Surface *surface = TTF_RenderUTF8_Blended( font, "SDL test OK", textColor );

    PHDEBUG << "surface :" << surface->w << "x" << surface->h << "/" << surface->format->BytesPerPixel << "/" << surface->pitch;

    // Dump the text surface:
    //MemoryDump(surface->pixels, surface->pitch, surface->h, surface->format->BytesPerPixel);

    // Display the text surface:
	SDL_Rect textRect = {0, 0, surface->w, surface->h};
	SDL_BlitSurface( surface, NULL, screen, &textRect );

	//Update the screen
	if( SDL_UpdateWindowSurface(window))
        return -1;

    int ret = a.exec();

    //Free the surface and quit SDL
    SDL_FreeSurface( image );
    SDL_FreeSurface(message);
    SDL_FreeSurface(screen);

    //Quit SDL
    TTF_CloseFont( font );
    TTF_Quit();
    SDL_Quit();

    return ret;
}


