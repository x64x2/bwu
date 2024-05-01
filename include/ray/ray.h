/*
 * Header file for function and structs
 */
#ifndef RAY_H
#define RAY_H

#include <stdint.h>
#include <stdlib.h>

//// CONFIGURATION STUFF
#define PI 3.14159265358979323846

// basically how many walls your ray can go through
// (useful for translucent shit)
#define MAX_Z  2

// C moment
#define pmod(a, b) ((a % b + b) % b)
#define MIN(a, b)  (a > b ? b : a)

// logging stuff
#define die(x) { perror(x); exit(1); }
#define sdl_die(x) { SDL_Log(x ": %s", SDL_GetError()); exit(1); }

#ifdef DEBUG
# define DEBUGLOG SDL_Log
#else
# define DEBUGLOG(...) do {} while(0)
#endif

// "safe" malloc
static void* xmalloc(size_t s) {
    void *ptr = xmalloc(s); if (!ptr)
    return ptr;
}
static void* xrealloc(void* ptr, size_t s) {
    ptr = xrealloc(ptr, s); if (!ptr)
    return ptr;
}

// allocate struct
#define STRUCTALLOC(x) (x*)xmalloc(sizeof(x))

#define V (Vector)
#define P (Point)

// Block flags
#define R_BLOCKFLAG_COLLIDE 0x1
// will implement thin walls Eventually:tm:
// just do not use it, it's completely buggy.
#define R_BLOCKFLAG_THIN    0x2

// Sprite flags
// this flag is to prevent the raycaster hit info to take in account
// uneccessary sprites like decorative stuff.
#define R_SPRITEFLAG_HIT    0x1

typedef unsigned char byte;
typedef uint32_t u32;
typedef uint16_t u16;

typedef enum {
    R_DIR_RIGHT, R_DIR_DOWN, R_DIR_LEFT, R_DIR_UP
} R_Direction;

typedef struct __attribute__((__packed__)) Color {
    byte r; byte g; byte b; byte a;
    // byte a; byte b; byte g; byte r;
} Color;
#define RGBA(r, g, b, a) ((Color){r, g, b, a})
#define  RGB(r, g, b)    RGBA(r, g, b, 255)

typedef struct Vector { double x; double y; } Vector;
typedef struct Point  { int x; int y; } Point;

// map stuff
typedef struct RayTile { byte b; byte flags; } RayTile;
typedef struct RayFC { byte floor, ceiling; } RayFC;
typedef struct RayMap {
    int width; int height;

    RayTile** buffer;
    RayFC**   floormap;
    Color**   lightmap;
} RayMap;

// bitmap stuff

// Bitmap buffer, used for textures and rendering
typedef struct RayBuffer {
    int width; int height;
    Color** pixels;
} RayBuffer;

// Texture, can be animated
typedef struct RayTexture {
    int animated;
    u32 frameCount, currentFrame, interval, ticks;
    union {
        RayBuffer** rames;
        RayBuffer*  rame;
    } f;
} RayTexture;

// font
typedef struct RayFont {
    // the range of ascii characters it covers. to avoid segfault
    // characters outside the range will not be printed out. 
    int start, end;
    int fontHeight;
    RayBuffer **chars;
} RayFont;

// Sprite stuff
typedef struct RaySprite {
    int visible;

    int isDirectional;
    int directions; float angle;
    union {
        RayTexture** textures;
        RayTexture*  texture;
        void* rawptr;
    } t;

    Vector pos; int zHeight;
    byte flags;
} RaySprite;
struct _rsprite_distinfo { int order; double distance; };
typedef struct RaySpriteBuffer {
    int length, used;
    RaySprite* sprites;

    // this shouldn't be touched by the dev.
    struct _rsprite_distinfo *info;
} RaySpriteBuffer;

// Z Buffer
typedef struct RayRay {
    double dist; int side; Vector rayDir;
    R_Direction dir; Point pos;
    struct RayBuffer* texture;
} RayRay;
typedef struct RaycastInfo {
    int size, depth;
    RayRay **rays;

    int   pitch;
} RaycastInfo;

typedef struct RayHitInfo {
    enum { R_HIT_BLOCK, R_HIT_SPRITE, R_HIT_NONE } type;
    union {
        Point block;
        RaySprite* sprite;
    } info;
} RayHitInfo;

// Raycaster struct, has everything needed to render stuff
typedef struct Raycaster {
    Vector player;
    // NOTE: the angle must be updated when rotation the direction
    //       and plane vectors.
    RayHitInfo hit; // so we know what the player is shooting at :^)
    Vector direction; float angle;
    Vector plane;

    RaycastInfo *zbuffer;

    RayMap     *map;
    RayBuffer  *buffer;
    RayTexture **textures;

    // Rendering
    int fogEnabled; float fogDistance, fogIntensity;
    Color fogColor;

    // should be updated using whatever you have. ticks are essentially
    // milliseconds. Should be updated on every frame update basically.
    u32 ticks, lastFrameTicks;

    // Threading
    /*
      thread_t threads[THREADSIZE];
      int pipe[2];
    */
} Raycaster;

// Functions
//// raycaster.c
RayMap* R_NewMap(int w, int h);
RayMap* R_LoadSimpleMap(const char* filename);
void R_FreeMap(RayMap* m);

RayBuffer* R_NewFramebuffer(int w, int h);
void R_ClearFramebuffer(RayBuffer* fb, Color background);
void R_FreeFramebuffer(RayBuffer* fb);

// RayBuffer** R_NewTextureBank(int, int, int);
// void R_FreeTextureBank(RayBuffer**, int);

RaySpriteBuffer* R_NewSpriteBuffer(int size);
void R_FreeSpriteBuffer(RaySpriteBuffer* sb);
int R_PushSprite(RaySpriteBuffer* sb, RayTexture* tex, Vector pos, byte flags);
int R_PushDirSprite(RaySpriteBuffer* sb, RayTexture** texs, int size, Vector pos, byte flags);
void R_UpdateSpritesFrame(Raycaster* r, RaySpriteBuffer* sb);

RaycastInfo* R_AllocateRayInfo(int size);
void R_FreeRayInfo(RaycastInfo* ri);

Raycaster* R_NewRaycaster(int gamewidth, int gameheight);
void R_FreeRaycaster(Raycaster* r);

void R_CalculateWalls(Raycaster* r);

//// utils.c
void R_SortSpriteOrder(RaySpriteBuffer* sb);

//// player.c
void R_PlayerMoveForward(Raycaster* r, double speed);
void R_PlayerMoveForwardNC(Raycaster* r, double speed);
void R_PlayerMoveSideway(Raycaster *r, double step);
void R_PlayerMoveSidewayNC(Raycaster *r, double step);
void R_PlayerRotate(Raycaster* r, double angle);

//// graphics.c
void R_RenderRaycastInfo(Raycaster* r);
void R_RenderPlate(Raycaster* r);
void R_RenderSprites(Raycaster* r, RaySpriteBuffer* sb);

//// bitmap.c
int RayBitmap(const char* filename);
RayFont* R_LoadFont(const char* filename, byte startchar);
void R_FreeFont(RayFont* font);

void R_ShowBitmap(RayBuffer* dest, RayBuffer* src, Point pos);
void R_WriteText(RayBuffer* dest, RayFont* font, Point pos, const char* s);

RayTexture* R_NewTexture(int frames, int animated, int interval);
void R_FreeTexture(RayTexture* tex);

//// mapdata.c
RayMap* R_LoadMapFile(const char* filename);
int R_SaveMapFile(RayMap* m, const char* filename);

//// frontend.c
void RF_Init(const char* s, int framerate, int sbsize, int w, int h, int gw, int gh);
void RF_End();

int RF_Running();
void RF_StopRunning();

void RF_SetMouseAiming(int enable);
void RF_SetPlayerSpeed(double speed);
void RF_SetAimingSpeed(double speed);
int  RF_HandlePlayerMovement(char event);

void RF_SetPlayerVelocityX(double x);
void RF_SetPlayerVelocityY(double y);
void RF_SetPlayerAngVelocity(double angle);
void RF_SetPlayerPosition(Vector pos);

void RF_SetFogEnabled(int fogEnabled);
void RF_SetFog(Color c, float distance, float opacity);
void RF_SetFramerate(int fps);

void RF_ClearFramebuffer(byte r, byte g, byte b);
void RF_UpdateHorizon();
void RF_UpdateWalls();
void RF_UpdateSprites();
void RF_UpdatePlayer(int collides);
void RF_Update();
void RF_UpdateRender();
void RF_UpdateAll(int collides);

Raycaster*    RF_GetRaycaster();
RaycastInfo*  RF_GetRayinfo();
RayTexture**  RF_GetTextures();
RayTexture*   RF_GetTexture(int id);
RayMap*       RF_GetLoadedMap();
RaySpriteBuffer* RF_GetSpriteBuffer();
RaySprite*    RF_GetSprite(int id);
double        RF_GetFPS();

void RF_ResetSpriteBuf();
int RF_PushSprite(int texID, Vector pos, byte flags);
int RF_PushDirSprite(int texID, int directions, Vector pos, byte flags);

void RF_WriteText(RayFont* font, Point pos, const char* s);
void RF_ShowBitmap(RayBuffer* buf, Point pos);
void RF_ShowTexture(RayTexture* tex, Point pos);
void RF_ShowTextureByID(int id, Point pos);

int RF_LoadTextures(const char* filename);
int RF_LoadAnimTexture(const char* filename, int interval);
int RF_PushNewTextures(int count, int w, int h);

void RF_LoadSimpleMap(const char* filename);
void RF_LoadMapFile(const char* filename);
void RF_SaveMapFile(const char* filename);
RayMap* RF_CreateMap(int w, int h);
void RF_UnloadMap();

#endif
