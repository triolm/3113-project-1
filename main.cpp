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

constexpr char    BG_COLOUR[] = "#FFFFFF";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { (float) SIZE, (float) SIZE };

// Image owned by Gorillaz @see https://gorillaz.com/
constexpr char BG[] = "assets/bg.png";
constexpr char MOUNT1[] = "assets/m1.png";
constexpr char MOUNT2[] = "assets/m2.png";
constexpr char MOUNT3[] = "assets/m3.png";
constexpr char TRAIN[] = "assets/train2.png";
constexpr char TRACK[] = "assets/track.png";
constexpr char STEAM[] = "assets/steam.png";
constexpr char WHEEL[] = "assets/wheel.png";
constexpr char STICK[] = "assets/stick.png";

constexpr float MOUNT1_SPEED = 150;
constexpr float MOUNT2_SPEED = 70;
constexpr float MOUNT3_SPEED = 30;
constexpr float TRACK_SPEED = 400;

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

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();


Texture2D gBackground;
Texture2D gMount1;
Texture2D gMount2;
Texture2D gMount3;
Texture2D gTrain;
Texture2D gTrack;
Texture2D gSteam;
Texture2D gWheel;
Texture2D gStick;

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gBackground = LoadTexture(BG);
    gMount1 = LoadTexture(MOUNT1);
    gMount2 = LoadTexture(MOUNT2);
    gMount3 = LoadTexture(MOUNT3);
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
    

    if(gSteamPos > 37) {
        gSteamPos = 2;
    }

}

float scaleHeight(Texture2D t){
    return (static_cast<float>(t.height)/static_cast<float>(t.width)) * SCREEN_WIDTH;
}

void renderMountain(Texture2D mountain, float speed){
    Rectangle textureArea =  {
        // top-left corner
        gParallaxOffset * speed,0,
        // bottom-right corner (of texture)
        static_cast<float>(mountain.width), 
        static_cast<float>(mountain.height),
    };

    Rectangle destinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(mountain),
        SCREEN_WIDTH, scaleHeight(mountain)
    };
   
    DrawTexturePro(
        mountain, 
        textureArea,
        destinationArea,
        {0,0}, 0, WHITE
    );
}

void renderWheel(float trainOffset, float xOffset){
    Rectangle wheelTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gWheel.width), 
        static_cast<float>(gWheel.height),
    };
    
    float wheelSize = SCREEN_WIDTH * (236.0f/4839.0f);

    Rectangle wheelDestinationArea = { 
        trainOffset + xOffset, SCREEN_HEIGHT - 77,
        wheelSize, wheelSize
    };
    DrawTexturePro(
        gWheel, 
        wheelTextureArea,
        wheelDestinationArea,
        {wheelSize/2, wheelSize/2}, gWheelRot, WHITE
    );
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    Rectangle bgTextureArea =  {
        // top-left corner
         0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gBackground.width), 
        static_cast<float>(gBackground.height)
    };
    Rectangle bgDestinationArea = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

     DrawTexturePro(
        gBackground, 
        bgTextureArea,
        bgDestinationArea,
        {0,0}, 0, WHITE
    );


    renderMountain(gMount3, MOUNT3_SPEED);
    renderMountain(gMount2, MOUNT2_SPEED);
    renderMountain(gMount1, MOUNT1_SPEED);


    Rectangle trackTextureArea =  {
        // top-left corner
        gParallaxOffset * TRACK_SPEED,0,
        // bottom-right corner (of texture)
        static_cast<float>(gTrack.width), 
        static_cast<float>(gTrack.height),
    };

    Rectangle trackDestinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(gTrack) - 50,
        SCREEN_WIDTH, scaleHeight(gTrack)
    };
   
    DrawTexturePro(
        gTrack, 
        trackTextureArea,
        trackDestinationArea,
        {0,0}, 0, WHITE
    );

    float trainOffset = -50* (1-sin(gTrainOffsetTime));

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
    
        Color tint = WHITE;
        tint.a = 255 - 255 * (thisPos / 285);

        DrawTexturePro(
            gSteam, 
            steamTextureArea,
            steamDestinationArea,
            {0,0}, 0, tint
        );
    }

    Rectangle trainTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gTrain.width), 
        static_cast<float>(gTrain.height),
    };

    Rectangle trainDestinationArea = { 
        trainOffset , SCREEN_HEIGHT - 175,
        SCREEN_WIDTH * (4195.0f/4839.0f), SCREEN_HEIGHT * (721.0f/2721.0f)
    };
   
    DrawTexturePro(
        gTrain, 
        trainTextureArea,
        trainDestinationArea,
        {0,0}, 0, WHITE
    );

    renderWheel(trainOffset, 604);
    renderWheel(trainOffset, 554);
    renderWheel(trainOffset, 503);



    Rectangle stickTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gStick.width), 
        static_cast<float>(gStick.height),
    };
    
    float stickWidth = SCREEN_WIDTH * (645.0f/4839.0f);
    float stickHeight = SCREEN_WIDTH * (37.0f/4839.0f);

    Rectangle stickDestinationArea = { 
        trainOffset + 500 + (float)cos(gWheelRot*0.0174533) * 10.0f, 
        SCREEN_HEIGHT - 80 + (float)sin(gWheelRot*0.0174533) * 10.0f,
        stickWidth, stickHeight
    };
    DrawTexturePro(
        gStick, 
        stickTextureArea,
        stickDestinationArea,
        {0,0}, 0, WHITE
    );


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