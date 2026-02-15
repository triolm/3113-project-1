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

// backgrounds
constexpr char BG_DAY[] = "assets/bg_day.png";
// i like hte word gloaming
constexpr char BG_GLOAMING[] = "assets/bg_eve.png";
constexpr char BG_NIGHT[] = "assets/bg_night.png";

// daytime mountain textures
constexpr char MOUNT1_DAY[] = "assets/mountains/m1_day.png";
constexpr char MOUNT2_DAY[] = "assets/mountains/m2_day.png";
constexpr char MOUNT3_DAY[] = "assets/mountains/m3_day.png";

// morning/gloaming montain textures
constexpr char MOUNT1_GLOAMING[] = "assets/mountains/m1_eve.png";
constexpr char MOUNT2_GLOAMING[] = "assets/mountains/m2_eve.png";
constexpr char MOUNT3_GLOAMING[] = "assets/mountains/m3_eve.png";

// night time mountain textures
constexpr char MOUNT1_NIGHT[] = "assets/mountains/m1_night.png";
constexpr char MOUNT2_NIGHT[] = "assets/mountains/m2_night.png";
constexpr char MOUNT3_NIGHT[] = "assets/mountains/m3_night.png";

// u can change this to assets/train.png to get rid of the professors
constexpr char TRAIN[] = "assets/train2.png";
constexpr char SUN[] = "assets/sun.png";
constexpr char MOON[] = "assets/moon.png";
constexpr char TRACK[] = "assets/track.png";
constexpr char STEAM[] = "assets/steam.png";
constexpr char WHEEL[] = "assets/wheel.png";
constexpr char STICK[] = "assets/stick.png";

// speeds of parallax items
constexpr float MOUNT1_SPEED = 150;
constexpr float MOUNT2_SPEED = 70;
constexpr float MOUNT3_SPEED = 30;
constexpr float TRACK_SPEED = 400;

enum TIME {DAY,GLOAMING, NIGHT, DAWN};

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gScaleFactor   = SIZE,
          gAngle         = 0.0f,
          gPulseTime     = 0.0f;
Vector2   gPosition      = ORIGIN;
Vector2   gScale         = BASE_SIZE;
float     gPreviousTicks = 0.0f;
float     gParallaxOffset = 0.0f;
// steam starts at 2 because it's passed into ln and ln(0) is undefined
float     gSteamPos      = 2.0f; 
float     gWheelRot      = 0.0f; 
float     gTrainOffsetTime   = 0.0f; 
TIME      gTimeOfDay = GLOAMING;
float     gDayOffset = 100;
// % of fade between two times of day
float     gTransitionPercent = 0;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();


Texture2D gSun,gMoon;

Texture2D gBg_day, gBg_gloaming, gBg_night;

Texture2D gMount1_day, gMount2_day, gMount3_day;
Texture2D gMount1_gloaming, gMount2_gloaming, gMount3_gloaming;
Texture2D gMount1_night, gMount2_night, gMount3_night;

Texture2D gTrain;
Texture2D gTrack;
Texture2D gSteam;
Texture2D gWheel;
Texture2D gStick;

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    // sorry there are a lot oops
    gSun = LoadTexture(SUN);
    gMoon = LoadTexture(MOON);
    gBg_day = LoadTexture(BG_DAY);
    gBg_gloaming = LoadTexture(BG_GLOAMING);
    gBg_night = LoadTexture(BG_NIGHT);
    gMount1_day = LoadTexture(MOUNT1_DAY);
    gMount2_day = LoadTexture(MOUNT2_DAY);
    gMount3_day = LoadTexture(MOUNT3_DAY);
    gMount1_gloaming = LoadTexture(MOUNT1_GLOAMING);
    gMount2_gloaming = LoadTexture(MOUNT2_GLOAMING);
    gMount3_gloaming = LoadTexture(MOUNT3_GLOAMING);
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

    // calculate delta time
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    // update the counters
    gParallaxOffset += 1.5f * deltaTime;
    gSteamPos += 40.0f * deltaTime;
    gWheelRot += 300.0f * deltaTime;
    gTrainOffsetTime += 0.25f * deltaTime;
    gDayOffset += 30 * deltaTime;
    

    // figure out what time of day it is
    float dayLength = 200;
    if(gDayOffset > 4 * dayLength) gDayOffset = 0;
    else if(gDayOffset > 3 * dayLength) {
        if(gTimeOfDay != DAY) gTransitionPercent = 1; 
        gTimeOfDay = DAY;
    }
    else if(gDayOffset > 2 * dayLength) {
        if(gTimeOfDay != DAWN) gTransitionPercent = 1; 
        gTimeOfDay = DAWN;
    }
    else if(gDayOffset > 1 * dayLength) {
        if(gTimeOfDay != NIGHT) gTransitionPercent = 1; 
        gTimeOfDay = NIGHT;
    }
    else if(gDayOffset > 0 * dayLength) {
        if(gTimeOfDay != GLOAMING) gTransitionPercent = 1; 
        gTimeOfDay = GLOAMING;
    }

    // i'm aware this goes into the negatives
    // hopefully it doesn't like wrap around to the highest value
    // shhhhhhh
    gTransitionPercent -= 0.5f * deltaTime;

    // when the first ball of steam gets to where the second ball of steam
    // started, reset the steam
    // they start at 2 because ln(0) is undefined and that was annoying
    if(gSteamPos > 37) {
        gSteamPos = 2;
    }

}


// i drew the textures in inkscape so i'm just making sure they retain the 
// scale that i drew them at
float scaleHeight(Texture2D& t){
    return (static_cast<float>(t.height)/static_cast<float>(t.width)) * SCREEN_WIDTH;
}

// train & friends get a bit darker at night
// tint is a parameter of DrawTexturePro
Color tintColor(){
    Color tint = WHITE;
    if(gTimeOfDay == NIGHT) tint = GRAY;
    if(gTimeOfDay == DAWN && gTransitionPercent > 0){
        tint = Color {
            // please don't kill me
            (unsigned char)(255 - gTransitionPercent*125.0f),
            (unsigned char)(255 - gTransitionPercent*125.0f),
            (unsigned char)(255 - gTransitionPercent*125.0f),
            255
        };
    }
    if(gTimeOfDay == NIGHT && gTransitionPercent > 0){
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

void renderMountain(Texture2D& day, Texture2D& gloaming, Texture2D& night,  float speed){
    Texture2D* mountain = &day;
    if(gTimeOfDay == GLOAMING) mountain = &gloaming;
    if(gTimeOfDay == DAWN) mountain = &gloaming;
    if(gTimeOfDay == NIGHT) mountain = &night;

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
        if(gTimeOfDay == NIGHT) prevMountain = &gloaming;
        if(gTimeOfDay == DAY) prevMountain = &gloaming;
        if(gTimeOfDay == DAWN) prevMountain = &night;

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
    if(gTimeOfDay == NIGHT) tint = GRAY;

    DrawTexturePro(
        gStick, 
        stickTextureArea,
        stickDestinationArea,
        {0,0}, 0, tintColor()
    );
}

void renderBg(){
    Texture2D* bg = &gBg_day;
    if(gTimeOfDay == GLOAMING) bg = &gBg_gloaming;
    if(gTimeOfDay == DAWN) bg = &gBg_gloaming;
    if(gTimeOfDay == NIGHT) bg = &gBg_night;


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
        if(gTimeOfDay == NIGHT) prevBg = &gBg_gloaming;
        if(gTimeOfDay == DAY) prevBg = &gBg_gloaming;
        if(gTimeOfDay == DAWN) prevBg = &gBg_night;

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
    if(gTimeOfDay == NIGHT) tint = GRAY;
    if(gTimeOfDay == DAWN && gTransitionPercent > 0){
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
    float sunRangeUp = SCREEN_HEIGHT / 3.5;
    float sunRangeDown = SCREEN_HEIGHT/2;
    float scale = 1;
    Color tint = WHITE;

    if(gTimeOfDay == NIGHT && gTransitionPercent > 0){
        yPos += sunRangeDown * (1-gTransitionPercent);
    } else if(gTimeOfDay == GLOAMING && gTransitionPercent > 0){
        yPos -= sunRangeUp * (gTransitionPercent);
        scale -= .5 * (gTransitionPercent);
        tint.a -= 40 * (gTransitionPercent);
    } else if(gTimeOfDay == DAY && gTransitionPercent > 0){
        yPos -= sunRangeUp * (1-gTransitionPercent);
        scale -= .5 * (1-gTransitionPercent);
        tint.a -= 40 * (1-gTransitionPercent);
    } else if(gTimeOfDay == DAWN && gTransitionPercent > 0){
        yPos += sunRangeDown * (gTransitionPercent);
    } else if (gTimeOfDay == DAY){
        yPos -= sunRangeUp;
        scale = .5;
        tint.a -= 40;
    } else if(gTimeOfDay == NIGHT) return;

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
        sunSize*scale, sunSize*scale
    };

    DrawTexturePro(
        gSun, 
        trainTextureArea,
        trainDestinationArea,
        {trainDestinationArea.width/2,trainDestinationArea.height/2}, 0, tint
    );
}


void renderMoon(float distFromBottom, float distFromLeft){

    float yPos = distFromBottom;
    float moonRange = SCREEN_HEIGHT/3 + SCREEN_HEIGHT * (1008/2721.0f);

    if(gTimeOfDay == DAWN && gTransitionPercent > 0){
        yPos += moonRange * (1-gTransitionPercent);
    } else if(gTimeOfDay == NIGHT && gTransitionPercent > 0){
        yPos -= moonRange * (gTransitionPercent);
    } else if(gTimeOfDay == DAWN && gTransitionPercent > 0){
        yPos -= moonRange * (1-gTransitionPercent);
    } else if(gTimeOfDay == NIGHT && gTransitionPercent > 0){
        yPos += moonRange * (gTransitionPercent);
    } else if(gTimeOfDay != NIGHT) return;

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

    renderSun(SCREEN_HEIGHT/2.0f,200);
    renderMoon(SCREEN_HEIGHT/2.0f-100,SCREEN_WIDTH- 200);

    renderMountain(gMount3_day, gMount3_gloaming, gMount3_night, MOUNT3_SPEED);
    renderMountain(gMount2_day, gMount2_gloaming, gMount2_night, MOUNT2_SPEED);
    renderMountain(gMount1_day, gMount1_gloaming, gMount1_night, MOUNT1_SPEED);

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