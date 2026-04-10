#include <stdlib.h>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_ttf.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "main.h"
#include "utils.h"
#include "storage.h"
#include "unicode.h"
#include "messages.h"
#include "sound.h"


Screen screen;
Random rndGen;


static void initScreen()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throw Exception(std::wstring(L"Error initializing SDL: ") + 
                fromMbcs(SDL_GetError()));
    atexit(SDL_Quit);
    if (TTF_Init())
        throw Exception(L"Error initializing font engine");
    screen.setMode(VideoMode(800, 600, 24,
#ifdef __EMSCRIPTEN__
                false
#else
                getStorage()->get(L"fullscreen", 1) != 0
#endif
                ));
    screen.initCursors();
    
    SDL_Surface *mouse = loadImage(L"cursor.bmp");
    SDL_SetColorKey(mouse, SDL_SRCCOLORKEY, SDL_MapRGB(mouse->format, 0, 0, 0));
    screen.setMouseImage(mouse);
    SDL_FreeSurface(mouse);
    SDL_WM_SetCaption("Einstein", NULL);

#ifdef __EMSCRIPTEN__
    SDL_StartTextInput();
#endif

#ifdef __APPLE__
    screen.setCursor(false);
#else
    screen.setCursor(getStorage()->get(L"niceCursor", 1));
#endif
}

static void initAudio()
{
    sound = new Sound();
    sound->setVolume((float)getStorage()->get(L"volume", 20) / 100.0f);
}


#ifdef __APPLE__
static std::wstring getResourcesPath(const std::wstring& path)
{
    int idx = path.find_last_of(L'/');
    return path.substr(0, idx) + L"/../../"; 
}
#endif

static void loadResources(const std::wstring &selfPath)
{
    StringList dirs;
#ifdef __EMSCRIPTEN__
    dirs.push_back(L".");
#elif defined(WIN32)
    dirs.push_back(getStorage()->get(L"path", L"") + L"\\res");
#elif defined(__APPLE__)
    dirs.push_back(getResourcesPath(selfPath));
#else
    dirs.push_back("/usr" L"/share/einstein/res");
    dirs.push_back(fromMbcs(getenv("HOME")) + L"/.einstein/res");
#endif
#ifndef __EMSCRIPTEN__
    dirs.push_back(L"res");
    dirs.push_back(L".");
#endif
    resources = new ResourcesCollection(dirs);
    msg.load();
}


/*static void checkBetaExpire()
{
    if (1124832535L + 60L*60L*24L*40L < time(NULL)) {
        Font font(L"laudcn2.ttf", 16);
        Area area;
        showMessageWindow(&area, L"darkpattern.bmp", 
                700, 100, &font, 255,255,255, 
                msg(L"expired"));
    }
}*/



int main(int argc, char *argv[])
{
#ifdef __EMSCRIPTEN__
    // Mount IDBFS and synchronously wait for IndexedDB to load
    EM_ASM(
        FS.mkdir('/einstein_data');
        FS.mount(IDBFS, {}, '/einstein_data');
        window._idbfs_ready = false;
        FS.syncfs(true, function(err) {
            if (err) console.error('Error loading from IDBFS:', err);
            window._idbfs_ready = true;
        });
    );
    while (!emscripten_run_script_int("window._idbfs_ready ? 1 : 0"))
        emscripten_sleep(50);
    ensureDirExists(L"/einstein_data/save");
#elif !defined(WIN32)
    ensureDirExists(fromMbcs(getenv("HOME")) + std::wstring(L"/.einstein"));
#endif
    
    try {
        loadResources(fromUtf8(argv[0]));
        initScreen();
        initAudio();
//        checkBetaExpire();
        menu();
        getStorage()->flush();
    } catch (Exception &e) {
        std::cerr << L"ERROR: " << e.getMessage() << std::endl;
    } catch (...) {
        std::cerr << L"ERROR: Unknown exception" << std::endl;
    }
    screen.doneCursors();
    
    return 0;
}

