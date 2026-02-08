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
constexpr char TRAIN[] = "assets/train.png";
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
    

    if(gSteamPos > 37) {
        gSteamPos = 2;
    }

}

float scaleHeight(Texture2D t){
    return (static_cast<float>(t.height)/static_cast<float>(t.width)) * SCREEN_WIDTH;
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


    Rectangle mount3TextureArea =  {
        // top-left corner
        gParallaxOffset * MOUNT3_SPEED,0,
        // bottom-right corner (of texture)
        static_cast<float>(gMount3.width), 
        static_cast<float>(gMount3.height),
    };

    Rectangle mount3DestinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(gMount3),
        SCREEN_WIDTH, scaleHeight(gMount3)
    };
   
    DrawTexturePro(
        gMount3, 
        mount3TextureArea,
        mount3DestinationArea,
        {0,0}, 0, WHITE
    );

    Rectangle mount2TextureArea =  {
        // top-left corner
        gParallaxOffset * MOUNT2_SPEED,0,
        // bottom-right corner (of texture)
        static_cast<float>(gMount2.width), 
        static_cast<float>(gMount2.height),
    };

    Rectangle mount2DestinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(gMount2),
        SCREEN_WIDTH, scaleHeight(gMount2)
    };
   
    DrawTexturePro(
        gMount2, 
        mount2TextureArea,
        mount2DestinationArea,
        {0,0}, 0, WHITE
    );

    Rectangle mount1TextureArea =  {
        // top-left corner
        gParallaxOffset * MOUNT1_SPEED,0,
        // bottom-right corner (of texture)
        static_cast<float>(gMount1.width), 
        static_cast<float>(gMount1.height),
    };

    Rectangle mount1DestinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(gMount1),
        SCREEN_WIDTH, scaleHeight(gMount1)
    };
   
    DrawTexturePro(
        gMount1, 
        mount1TextureArea,
        mount1DestinationArea,
        {0,0}, 0, WHITE
    );
    Rectangle trackTextureArea =  {
        // top-left corner
        gParallaxOffset * TRACK_SPEED,0,
        // bottom-right corner (of texture)
        static_cast<float>(gTrack.width), 
        static_cast<float>(gTrack.height),
    };

    Rectangle trackDestinationArea = { 
        0, SCREEN_HEIGHT - scaleHeight(gTrack) - 49,
        SCREEN_WIDTH, scaleHeight(gTrack)
    };
   
    DrawTexturePro(
        gTrack, 
        trackTextureArea,
        trackDestinationArea,
        {0,0}, 0, WHITE
    );

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
        float steamY = -20*std::log(thisPos/30) + SCREEN_HEIGHT - 240;

        float thisSize = 20 + (thisPos / 100) * 20;

        Rectangle steamDestinationArea = { 
            steamX - thisSize/2, steamY - thisSize/2,
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
        0, SCREEN_HEIGHT - 175,
        SCREEN_WIDTH * (4195.0f/4839.0f), SCREEN_HEIGHT * (721.0f/2721.0f)
    };
   
    DrawTexturePro(
        gTrain, 
        trainTextureArea,
        trainDestinationArea,
        {0,0}, 0, WHITE
    );

    Rectangle wheelTextureArea =  {
        // top-left corner
        0,0,
        // bottom-right corner (of texture)
        static_cast<float>(gWheel.width), 
        static_cast<float>(gWheel.height),
    };
    
    float wheelSize = SCREEN_WIDTH * (236.0f/4839.0f);

    Rectangle wheelDestinationArea = { 
        604, SCREEN_HEIGHT - 77,
        wheelSize, wheelSize
    };
    DrawTexturePro(
        gWheel, 
        wheelTextureArea,
        wheelDestinationArea,
        {wheelSize/2, wheelSize/2}, gWheelRot, WHITE
    );

    Rectangle wheelDestinationArea2 = { 
        554, SCREEN_HEIGHT - 77,
        wheelSize, wheelSize
    };
    DrawTexturePro(
        gWheel, 
        wheelTextureArea,
        wheelDestinationArea2,
        {wheelSize/2, wheelSize/2}, gWheelRot, WHITE
    );

    Rectangle wheelDestinationArea3 = { 
        503, SCREEN_HEIGHT - 77,
        wheelSize, wheelSize
    };
    DrawTexturePro(
        gWheel, 
        wheelTextureArea,
        wheelDestinationArea3,
        {wheelSize/2, wheelSize/2}, gWheelRot, WHITE
    );


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
        500 + (float)cos(gWheelRot*0.0174533) * 10.0f, 
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