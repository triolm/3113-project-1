#include "CS3113/cs3113.h"
#include <math.h>
#include <iostream>

// are we allowed to define our own functions
//unload texture

// Global Constants
constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SIZE          = 200,
                FRAME_LIMIT   = 100;
constexpr float MAX_AMP       = 10.0f;
constexpr float TEXTURE_SCALE_WIDTH = 4838.0f;

constexpr char    BG_COLOUR[] = "#FFFFFF";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { (float) SIZE, (float) SIZE };

// Image owned by Gorillaz @see https://gorillaz.com/
constexpr char BG_DAY[] = "assets/bg_day.png";
constexpr char BG_EVE[] = "assets/bg_eve.png";
constexpr char BG_NIGHT[] = "assets/bg_night.png";
constexpr char MOUNT1_DAY[] = "assets/mountains/m1_day.png";
constexpr char MOUNT2_DAY[] = "assets/mountains/m2_day.png";
constexpr char MOUNT3_DAY[] = "assets/mountains/m3_day.png";
constexpr char MOUNT1_EVE[] = "assets/mountains/m1_eve.png";
constexpr char MOUNT2_EVE[] = "assets/mountains/m2_eve.png";
constexpr char MOUNT3_EVE[] = "assets/mountains/m3_eve.png";
constexpr char MOUNT1_NIGHT[] = "assets/mountains/m1_night.png";
constexpr char MOUNT2_NIGHT[] = "assets/mountains/m2_night.png";
constexpr char MOUNT3_NIGHT[] = "assets/mountains/m3_night.png";
constexpr char TRAIN[] = "assets/train2.png";
constexpr char SUN[] = "assets/sun.png";
constexpr char MOON[] = "assets/moon.png";
constexpr char TRACK[] = "assets/track.png";
constexpr char STEAM[] = "assets/steam.png";
constexpr char WHEEL[] = "assets/wheel.png";
constexpr char STICK[] = "assets/stick.png";

constexpr float MOUNT1_SPEED = 150;
constexpr float MOUNT2_SPEED = 70;
constexpr float MOUNT3_SPEED = 30;
constexpr float TRACK_SPEED = 400;

enum TIME {DAY,EVE, NIGHT, MORN};

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gScaleFactor   = SIZE,
          gAngle         = 0.0f,
          gPulseTime     = 0.0f;
Vector2   gPosition      = ORIGIN;
Vector2   gScale         = BASE_SIZE;
float     gPreviousTicks = 0.0f;
float     gParallaxOffset = 0.0f;
float     gSteamPos      = 2.0f; 
float     gWheelRot      = 0.0f; 
float     gTrainOffsetTime   = 0.0f; 
TIME      gCurrentTime = EVE;
float     gDayOffset = 0;
float     gTransitionPercent = 0;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();


Texture2D gSun;
Texture2D gMoon;
Texture2D gBg_day;
Texture2D gBg_eve;
Texture2D gBg_night;
Texture2D gMount1_day;
Texture2D gMount2_day;
Texture2D gMount3_day;
Texture2D gMount1_eve;
Texture2D gMount2_eve;
Texture2D gMount3_eve;
Texture2D gMount1_night;
Texture2D gMount2_night;
Texture2D gMount3_night;
Texture2D gTrain;
Texture2D gTrack;
Texture2D gSteam;
Texture2D gWheel;
Texture2D gStick;

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gSun = LoadTexture(SUN);
    gMoon = LoadTexture(MOON);
    gBg_day = LoadTexture(BG_DAY);
    gBg_eve = LoadTexture(BG_EVE);
    gBg_night = LoadTexture(BG_NIGHT);
    gMount1_day = LoadTexture(MOUNT1_DAY);
    gMount2_day = LoadTexture(MOUNT2_DAY);
    gMount3_day = LoadTexture(MOUNT3_DAY);
    gMount1_eve = LoadTexture(MOUNT1_EVE);
    gMount2_eve = LoadTexture(MOUNT2_EVE);
    gMount3_eve = LoadTexture(MOUNT3_EVE);
    gMount1_night = LoadTexture(MOUNT1_NIGHT);
    gMount2_night = LoadTexture(MOUNT2_NIGHT);
    gMount3_night = LoadTexture(MOUNT3_NIGHT);
    gTrain = LoadTexture(TRAIN);
    gTrack = LoadTexture(TRACK);
    gSteam = LoadTexture(STEAM);
    gWheel = LoadTexture(WHEEL);
    gStick = LoadTexture(STICK);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{

    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gParallaxOffset += 1.5f * deltaTime;
    gSteamPos += 40.0f * deltaTime;
    gWheelRot += 300.0f * deltaTime;
    gTrainOffsetTime += 0.25f * deltaTime;
    gDayOffset += 30 * deltaTime;
    
    float dayLength = 200;
    if(gDayOffset > 4 * dayLength) gDayOffset = 0;
    else if(gDayOffset > 3 * dayLength) {
        if(gCurrentTime != DAY) gTransitionPercent = 1; 
        gCurrentTime = DAY;
    }
    else if(gDayOffset > 2 * dayLength) {
        if(gCurrentTime != MORN) gTransitionPercent = 1; 
        gCurrentTime = MORN;
    }
    else if(gDayOffset > 1 * dayLength) {
        if(gCurrentTime != NIGHT) gTransitionPercent = 1; 
        gCurrentTime = NIGHT;
    }
    else if(gDayOffset > 0 * dayLength) {
        if(gCurrentTime != EVE) gTransitionPercent = 1; 
        gCurrentTime = EVE;
    }

    gTransitionPercent -= 0.5f * deltaTime;

    if(gSteamPos > 37) {
        gSteamPos = 2;
    }

}

float scaleHeight(Texture2D& t){
    return (static_cast<float>(t.height)/static_cast<float>(t.width)) * SCREEN_WIDTH;
}


Color tintColor(){
    Color tint = WHITE;
    if(gCurrentTime == NIGHT) tint = GRAY;
    if(gCurrentTime == MORN && gTransitionPercent > 0){
        tint = Color {
            // please don't kill me
            (unsigned char)(255 - gTransitionPercent*125.0f),
            (unsigned char)(255 - gTransitionPercent*125.0f),
            (unsigned char)(255 - gTransitionPercent*125.0f),
            255
        };
    }
    if(gCurrentTime == NIGHT && gTransitionPercent > 0){
        tint = Color {
            // please don't kill me
            (unsigned char)(130 + gTransitionPercent*125.0f),
            (unsigned char)(130 + gTransitionPercent*125.0f),
            (unsigned char)(130 + gTransitionPercent*125.0f),
            255
        };
    }
    return tint;
}

void renderMountain(Texture2D& day, Texture2D& eve, Texture2D& night,  float speed){
    Texture2D* mountain = &day;
    if(gCurrentTime == EVE) mountain = &eve;
    if(gCurrentTime == MORN) mountain = &eve;
    if(gCurrentTime == NIGHT) mountain = &night;

    Rectangle textureArea =  {
        // top-left corner
        gParallaxOffset * speed,0,
        // bottom-right corner (of texture)
        static_cast<float>(mountain->width), 
        static_cast<float>(mountain->height),
    };

    Rectangle destinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(*mountain),
        SCREEN_WIDTH, scaleHeight(*mountain)
    };
   
    DrawTexturePro(
        *mountain, 
        textureArea,
        destinationArea,
        {0,0}, 0, WHITE
    );

    if(gTransitionPercent > 0){
        Texture2D* prevMountain = &day;
        if(gCurrentTime == NIGHT) prevMountain = &eve;
        if(gCurrentTime == DAY) prevMountain = &eve;
        if(gCurrentTime == MORN) prevMountain = &night;

        Color tint = WHITE;
        tint.a = (gTransitionPercent * 255.0f);
        // printf("%i\n", tint.a);
        // printf("%i\n", gTransitionPercent);
        DrawTexturePro(
            *prevMountain, 
            textureArea,
            destinationArea,
            {0,0}, 0, tint
        );
    }
}

void renderWheel(float trainOffset, float xOffset, float distFromBottom){
    Rectangle wheelTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gWheel.width), 
        static_cast<float>(gWheel.height),
    };
    
    float wheelSize = SCREEN_WIDTH * (236.0f/TEXTURE_SCALE_WIDTH);

    Rectangle wheelDestinationArea = { 
        trainOffset + xOffset, SCREEN_HEIGHT - distFromBottom,
        wheelSize, wheelSize
    };


    DrawTexturePro(
        gWheel, 
        wheelTextureArea,
        wheelDestinationArea,
        {wheelSize/2, wheelSize/2}, gWheelRot, tintColor()
    );

}

void renderSteam(float trainOffset){
    for (size_t i = 0; i < 8; i ++){

        float thisPos = gSteamPos + (35*i);

        Rectangle steamTextureArea =  {
            // top-left corner
            0,0,
            // bottom-right corner (of texture)
            static_cast<float>(gSteam.width), 
            static_cast<float>(gSteam.height),
        };

        // yeah i know the position is slightly offset. i'm trying, ok.

        float steamX = SCREEN_WIDTH - 160 - thisPos;
        float steamY = -15*std::log(thisPos/30) + SCREEN_HEIGHT - 220;

        float thisSize = 20 + (thisPos / 100) * 20;

        Rectangle steamDestinationArea = { 
            trainOffset + steamX - thisSize/2, steamY - thisSize/2,
            thisSize, thisSize
        };
    
        Color tint = tintColor();
        tint.a = 255 - 255 * (thisPos / 285);

        DrawTexturePro(
            gSteam, 
            steamTextureArea,
            steamDestinationArea,
            {0,0}, 0, tint
        );
    }
}

void renderStick(float trainOffset, float distFromBottom, float distFromLeft){

    Rectangle stickTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gStick.width), 
        static_cast<float>(gStick.height),
    };
    
    // ratio of stick texture width in pixels to bg texture width in pixels
    float stickWidth = SCREEN_WIDTH * (645.0f/TEXTURE_SCALE_WIDTH);
    float stickHeight = SCREEN_WIDTH * (37.0f/TEXTURE_SCALE_WIDTH);

    Rectangle stickDestinationArea = { 
        // (0.0174533 is the conversion to radians)
        trainOffset + distFromLeft + (float)cos(gWheelRot*0.0174533) * 10.0f, 
        SCREEN_HEIGHT - distFromBottom + (float)sin(gWheelRot*0.0174533) * 10.0f,
        stickWidth, stickHeight
    };

    Color tint = WHITE;
    if(gCurrentTime == NIGHT) tint = GRAY;

    DrawTexturePro(
        gStick, 
        stickTextureArea,
        stickDestinationArea,
        {0,0}, 0, tintColor()
    );
}

void renderBg(){
    Texture2D* bg = &gBg_day;
    if(gCurrentTime == EVE) bg = &gBg_eve;
    if(gCurrentTime == MORN) bg = &gBg_eve;
    if(gCurrentTime == NIGHT) bg = &gBg_night;


      Rectangle bgTextureArea =  {
        // top-left corner
         0,0,
        // bottom-right corner (of texture)
        static_cast<float>(bg->width), 
        static_cast<float>(bg->height)
    };
    Rectangle bgDestinationArea = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

     DrawTexturePro(
        *bg, 
        bgTextureArea,
        bgDestinationArea,
        {0,0}, 0, WHITE
    );

    if(gTransitionPercent > 0){
        Texture2D* prevBg = &gBg_day;
        if(gCurrentTime == NIGHT) prevBg = &gBg_eve;
        if(gCurrentTime == DAY) prevBg = &gBg_eve;
        if(gCurrentTime == MORN) prevBg = &gBg_night;

        Color tint = WHITE;
        tint.a = (gTransitionPercent * 255.0f);
        // printf("%i\n", tint.a);
        // printf("%i\n", gTransitionPercent);
        DrawTexturePro(
            *prevBg, 
            bgTextureArea,
            bgDestinationArea,
            {0,0}, 0, tint
        );
    }
}

void renderTrack(float distFromBottom){
    Rectangle trackTextureArea =  {
        // top-left corner
        gParallaxOffset * TRACK_SPEED,0,
        // bottom-right corner (of texture)
        static_cast<float>(gTrack.width), 
        static_cast<float>(gTrack.height),
    };

    Rectangle trackDestinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(gTrack) - distFromBottom,
        SCREEN_WIDTH, scaleHeight(gTrack)
    };

    Color tint = WHITE;
    if(gCurrentTime == NIGHT) tint = GRAY;
    if(gCurrentTime == MORN && gTransitionPercent > 0){
        tint = Color {
            // please don't kill me
            (unsigned char)(255 - gTransitionPercent*125.0f),
            (unsigned char)(255 - gTransitionPercent*125.0f),
            (unsigned char)(255 - gTransitionPercent*125.0f),
            255
        };
    }
   
    DrawTexturePro(
        gTrack, 
        trackTextureArea,
        trackDestinationArea,
        {0,0}, 0, tintColor()
    );


}

void renderTrain(float trainOffset, float distFromBottom){

    Rectangle trainTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gTrain.width), 
        static_cast<float>(gTrain.height),
    };

    Rectangle trainDestinationArea = { 
        trainOffset , SCREEN_HEIGHT - distFromBottom,
        // aspect ratio compared to max with of textures
        SCREEN_WIDTH * (4195.0f/TEXTURE_SCALE_WIDTH), SCREEN_HEIGHT * (721.0f/2721.0f)
    };
   
    

    DrawTexturePro(
        gTrain, 
        trainTextureArea,
        trainDestinationArea,
        {0,0}, 0, tintColor()
    );
}

void renderSun(float distFromBottom, float distFromLeft){

    float yPos = distFromBottom;
    float sunSize = SCREEN_HEIGHT * (1008/2721.0f);
    float sunRange = SCREEN_HEIGHT/2 + sunSize;

    if(gCurrentTime == NIGHT && gTransitionPercent > 0){
        yPos += sunRange * (1-gTransitionPercent);
    } else if(gCurrentTime == EVE && gTransitionPercent > 0){
        yPos -= sunRange * (gTransitionPercent);
    } else if(gCurrentTime == DAY && gTransitionPercent > 0){
        yPos -= sunRange * (1-gTransitionPercent);
    } else if(gCurrentTime == MORN && gTransitionPercent > 0){
        yPos += sunRange * (gTransitionPercent);
    } else if(gCurrentTime == DAY || gCurrentTime == NIGHT) return;

    Rectangle trainTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gSun.width), 
        static_cast<float>(gSun.height),
    };


    Rectangle trainDestinationArea = { 
        distFromLeft, yPos,
        // aspect ratio compared to max with of textures
        sunSize, sunSize
    };

    DrawTexturePro(
        gSun, 
        trainTextureArea,
        trainDestinationArea,
        {trainDestinationArea.width/2,trainDestinationArea.height/2}, 0, WHITE
    );
}


void renderMoon(float distFromBottom, float distFromLeft){

    float yPos = distFromBottom;
    float moonRange = SCREEN_HEIGHT/3 + SCREEN_HEIGHT * (1008/2721.0f);

    if(gCurrentTime == MORN && gTransitionPercent > 0){
        yPos += moonRange * (1-gTransitionPercent);
    } else if(gCurrentTime == NIGHT && gTransitionPercent > 0){
        yPos -= moonRange * (gTransitionPercent);
    } else if(gCurrentTime == MORN && gTransitionPercent > 0){
        yPos -= moonRange * (1-gTransitionPercent);
    } else if(gCurrentTime == NIGHT && gTransitionPercent > 0){
        yPos += moonRange * (gTransitionPercent);
    } else if(gCurrentTime != NIGHT) return;

    Rectangle trainTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gMoon.width), 
        static_cast<float>(gMoon.height),
    };


    Rectangle trainDestinationArea = { 
        distFromLeft, yPos,
        // aspect ratio compared to max with of textures
        SCREEN_WIDTH * (426/TEXTURE_SCALE_WIDTH), SCREEN_HEIGHT * (489/2721.0f)
    };

    DrawTexturePro(
        gMoon, 
        trainTextureArea,
        trainDestinationArea,
        {trainDestinationArea.width/2,trainDestinationArea.height/2}, 0, WHITE
    );
}

void render()
{   
    float trainOffset = -50* (1-sin(gTrainOffsetTime));

    
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    renderBg();

    renderSun(SCREEN_HEIGHT/2.0f,250);
    renderMoon(SCREEN_HEIGHT/2.0f-100,SCREEN_WIDTH- 250);

    renderMountain(gMount3_day, gMount3_eve, gMount3_night, MOUNT3_SPEED);
    renderMountain(gMount2_day, gMount2_eve, gMount2_night, MOUNT2_SPEED);
    renderMountain(gMount1_day, gMount1_eve, gMount1_night, MOUNT1_SPEED);

    renderTrack(50);
    
    renderTrain(trainOffset , 175);


    renderWheel(trainOffset, 604, 77);
    renderWheel(trainOffset, 554, 77);
    renderWheel(trainOffset, 503, 77);

    renderSteam(trainOffset);
    renderStick(trainOffset, 80, 500);

    EndDrawing();
}


void shutdown() { CloseWindow(); }

int main(void)
{

    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}