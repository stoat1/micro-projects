#include <cstdlib>
#include <cmath>
#include <ctime>
#include <SDL.h>
#include "../include/Graph.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <SDL.h>
#include "SDL_gfxPrimitives.h"

using namespace std;

// color constants
const Uint32 POINT_COLOR = 0x00ff00ff;
const Uint32 ARROW_COLOR = 0x777777ff;
const Uint32 TEXT_COLOR = 0xccccccff;

// metric constants
const size_t ARROW_HEAD_SIZE = 2;
const size_t POINT_SIZE = 4;
const size_t MARGIN = 32;
const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

struct Rectangle
{
public:
    Rectangle()
        : x(0), y(0), width(0), height(0)
    {}

    Rectangle(double x, double y, double w, double h)
        : x(x), y(y), width(w), height(h)
    {}

    pair<Rectangle, Rectangle> hsplit(double xOffset)
    {
        Rectangle left, right;

        left.x = x;
        right.x = x + xOffset;
        left.width = xOffset;
        right.width = width - xOffset;

        left.y = right.y = y;
        left.height = right.height = height;

        return make_pair(left, right);
    }

    pair<Rectangle, Rectangle> vsplit(double yOffset)
    {
        Rectangle top, bottom;

        top.x = bottom.x = x;
        top.width = bottom.width = width;

        top.y = y;
        bottom.y = y + yOffset;
        top.height = yOffset;
        bottom.height = height - yOffset;

        return make_pair(top, bottom);
    }

    int draw(SDL_Surface * surface)
    {
        return rectangleColor(surface, x, y, width, height, 0xff0000ff);
    }

    double x, y, width, height;
};

void saveScreenshot()
{
    SDL_Surface * screen = SDL_GetVideoSurface();

    time_t rawTime = time(NULL);
    struct tm * ptm = gmtime( & rawTime );

    ostringstream osstr;
    osstr << ".\\Graph_"
        << setw(4) << setfill('0') << ptm->tm_year + 1900
        << "" << setw(2) << ptm->tm_mon + 1
        << "" << setw(2) << ptm->tm_mday + 1
        << "_" << setw(2) << ptm->tm_hour
        << "" << setw(2) << ptm->tm_min
        << "" << setw(2) << ptm->tm_sec
        << "_" << clock() << ".bmp";

    if( -1 == SDL_SaveBMP( screen, osstr.str().c_str() ) )
    {
        fprintf( "Cannot save bmp: %s\n", osstr.str().c_str() );
    }
}

int drawVArrow( SDL_Surface * surface, Sint16 x, Sint16 y1, Sint16 y2 );
int drawHArrow( SDL_Surface * surface, Sint16 x1, Sint16 x2, Sint16 y );
int drawPoint( SDL_Surface * surface, Sint16 x, Sint16 y );

int drawGraph(
    const Graph * graph,
    SDL_Surface * surface,
    Sint16 x0,
    Sint16 y0,
    Uint16 width,
    Uint16 height,
    Sint16 x = -1,
    Sint16 y = -1
)
{
    double minX, maxX;
    double minY, maxY;

    minX = maxX = graph->front().x;
    minY = maxX = graph->front().y;

    for( vector<Point>::const_iterator iter = graph->begin();
            iter != graph->end(); ++iter )
    {
        maxX = max( maxX, iter->x );
        minX = min( minX, iter->x );
        maxY = max( maxY, iter->y );
        minY = min( minY, iter->y );
    }

    if( maxX - minX == 0 )
    {
        maxX = minX + 1;
    }
    if( maxY - minY == 0 )
    {
        maxY = minY + 1;
    }

    drawHArrow( surface, x0, x0 + width, y0 + height );
    drawVArrow( surface, x0, y0, y0 + height );

    for( vector<Point>::const_iterator iter = graph->begin();
            iter != graph->end(); ++iter )
    {
        double pointX = iter->x;
        double pointY = iter->y;

        Sint16 x = x0 + ( pointX - minX ) * width / ( maxX - minX );
        Sint16 y = ( y0 + height ) - ( pointY - minY ) * height / ( maxY - minY );

        drawPoint( surface, x, y );
    }

    if(x == -1)
        return 0;

    double pointX = minX + (x - x0) * (maxX - minX) / width;
    double pointY = minY - (y - y0 - height) * (maxY - minY) / height;

    ostringstream osstr;
    osstr.precision(5);
    osstr << fixed << "(" << pointX << ", " << pointY << ")";

    x -= osstr.str().length() * 4;
    y -= 8;

    if(x + 8 * osstr.str().length() > x0 + width)
    {
        x = x0 + width - 8 * osstr.str().length();
    }
    if(x < x0)
    {
        x = x0;
    }

    stringColor(surface, x, y, osstr.str().c_str(), 0x777777ff);

    return 0;
}

int drawHArrow( SDL_Surface * surface, Sint16 x1, Sint16 x2, Sint16 y )
{
    int result = 0;

    result += hlineColor( surface, x1, x2, y, ARROW_COLOR );
    result += lineColor( surface, x2, y, x2 - ARROW_HEAD_SIZE, y + ARROW_HEAD_SIZE, ARROW_COLOR );
    result += lineColor( surface, x2, y, x2 - ARROW_HEAD_SIZE, y - ARROW_HEAD_SIZE, ARROW_COLOR );

    if( result < 0 )
        return -1;
    else
        return 0;
}

int drawVArrow( SDL_Surface * surface, Sint16 x, Sint16 y1, Sint16 y2 )
{
    int result = 0;

    result += vlineColor( surface, x, y1, y2, ARROW_COLOR );
    result += lineColor( surface, x, y1, x + ARROW_HEAD_SIZE, y1 + ARROW_HEAD_SIZE, ARROW_COLOR );
    result += lineColor( surface, x, y1, x - ARROW_HEAD_SIZE, y1 + ARROW_HEAD_SIZE, ARROW_COLOR );

    if( result < 0 )
        return -1;
    else
        return 0;
}

int drawPoint( SDL_Surface * surface, Sint16 x, Sint16 y )
{
    int result = 0;

    result += vlineColor( surface, x, y - POINT_SIZE, y + POINT_SIZE, POINT_COLOR );
    result += hlineColor( surface, x - POINT_SIZE, x + POINT_SIZE, y, POINT_COLOR );

    if( result < 0 )
        return -1;
    else
        return 0;
}

int main( int argc, char** argv )
{
    vector<Graph *> graphs;
    Sint16 mouseX = -1, mouseY = -1;

    for( int i = 1; i < argc; ++i )
    {
        Graph * graph = new Graph( argv[ i ] );

        ifstream ifst( argv[ i ], ios::in );
        if(ifst.good())
        {
            ifst >> * graph;
            graphs.push_back( graph );
        }
        else
        {
            graphs.push_back( NULL );
            delete graph;
        }

    }

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    vector<Graph *>::iterator iGraph = graphs.begin();

    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;

                if( event.key.keysym.sym == SDLK_RIGHT )
                {
                    if( iGraph != graphs.end() )
                    {
                        ++iGraph;
                    }
                }

                if( event.key.keysym.sym == SDLK_LEFT )
                {
                    if( iGraph != graphs.begin() )
                    {
                        --iGraph;
                    }
                }

                if( event.key.keysym.sym == SDLK_F12 )
                {
                    saveScreenshot();
                }

                if( event.key.keysym.sym == SDLK_F11 )
                {
                    if( 0 == SDL_WM_ToggleFullScreen( screen ) )
                    {
                        fprintf( stderr, "Cannot toggle full screen\n" );
                    }
                }

                break;

            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                break;

            case SDL_VIDEORESIZE:

                // create a new window
                screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16,
                                SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE);
                break;
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw info
        ostringstream osstr;
        if( graphs.empty() )
        {
            osstr << "No arguments";
        }
        else
        {
            osstr << "Graph " << setw( 2 ) << iGraph - graphs.begin()
            << " of " << setw( 2 ) << graphs.size();
        }
        stringColor(screen, 0, 0, osstr.str().c_str(), TEXT_COLOR);

        // draw graph
        if(iGraph != graphs.end())
        {
            Graph * g = * iGraph;
            if( g != NULL )
            {
                stringColor(screen, 0, 8, (* iGraph)->getName().c_str(), TEXT_COLOR);

                if(mouseX < MARGIN || screen->w - MARGIN < mouseX
                    || mouseY < MARGIN || screen->h - MARGIN < mouseY)
                {
                    mouseX = mouseY = -1;
                }

                    drawGraph(
                        * iGraph,
                        screen,
                        MARGIN,
                        MARGIN,
                        screen->w - MARGIN * 2,
                        screen->h - MARGIN * 2,
                        mouseX,
                        mouseY
                    );
            }
            else
            {
                stringColor(screen, 0, 16, "graph not loaded", TEXT_COLOR);
            }
        }

        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(screen);

        SDL_Delay(15);
    } // end main loop

    for( vector<Graph *>::iterator iGraph = graphs.begin();
            iGraph != graphs.end(); ++iGraph )
    {
        if( * iGraph != NULL )
        {
            delete * iGraph;
        }
    }

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
