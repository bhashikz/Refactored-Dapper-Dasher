#include "raylib.h"

struct AnimData{ // AnimData is a custom datatype 

    // Member variables 
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight){ // Refactoring
    return data.pos.y >= (windowHeight - data.rec.height); // Logic to check if scarfy is on ground
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame){ // Refactoring

        data.runningTime = data.runningTime + deltaTime; // Update running time
        if(data.runningTime >= data.updateTime){
            data.runningTime = 0.0;

            // Update scarfy's animation frame
            data.rec.x = data.frame* data.rec.width;
            data.frame++;

            if (data.frame > maxFrame){
                data.frame = 0;
            }
        }
    return data;
}

int main(){

    int windowDimensions[2]; // Array of window dimension
    windowDimensions[0] = 512; // Window Width
    windowDimensions[1] = 380; // Window Height

    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher Game"); // Creates a window

    // Acceleration due to gravity (pixel/ sec) per sec
    const int gravity = 1000;

    // Nebula texture from spritesheet
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int numOfNebule = 20; // Number of nebulas that spawn on the window

    // AnimData called "nebulae" contains an array of animation data for all nebulas with properties 
    // defined in the custom datatype "AnimData" i.e. rec, pos, frame, updateTime, runningTime. 
    AnimData nebulae[numOfNebule]{}; 
    
    for (int i = 0; i < numOfNebule; i++){ // Nebulae properties
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;

        nebulae[i].pos.x = windowDimensions[0] + 300*i;
        nebulae[i].pos.y = windowDimensions[1] -  nebula.height/8;

        nebulae[i].frame = 0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].runningTime = 0;
    }

    float finishLine{ nebulae[numOfNebule - 1].pos.x };

    int nebVel = -200; // Nebula x velocity (pixels/ sec). Move Nebula to the left.

    // Scarfy texture from spritesheet
    Texture2D scarfy = LoadTexture("textures/scarfy.png"); // Scarfy sprite sheet with different frames

    // Creates an AnimData called "scarfyData" with all the properties of AnimData. 
    AnimData scarfyData; // AnimData is a structure.

    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;

    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2; 
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;

    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;
 
    bool isInAir = false; // Is the rectange in the air?

    const int jumpVal = -600;// Jump velocity in (pixel/ second) of scarfy.

    int velocity = 0;

    // Creating background texture 
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX = 0; // x position of background

    // Creating midground texture
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX = 0;

    // Creating foreground texture
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX = 0;

    SetTargetFPS(60); // Updating the animation frame 60 times per second with custom Raylib function.
    bool collision = false;

    while(!WindowShouldClose()){ // If the window is open, WindowShouldClose() returns false.

        // Delta time (time since last frame) or time duration between two consequtive frames
        const float dT = GetFrameTime(); // 

        // Start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Update background x position every frame
        bgX = bgX - 20*dT; // Scroll the background at 20 pixels per second to the left
        mgX = mgX - 40*dT; // Scroll midground at 40 pixels per second to the left
        fgX = fgX - 80*dT; // scroll the foreground at 80 pixels per second to the left

        if (bgX <= -background.width*2){
            bgX = 0.0;
        }

        if (mgX <= -midground.width*2){
            mgX = 0.0;
        }

        if (fgX <= -foreground.width*2){
            fgX = 0.0;
        }

        // Draw background
        Vector2 bg1Pos{bgX, 0.0}; // Background position
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE); // DrawTextureEx takes extra parameters

        Vector2 bg2Pos{bgX + background.width*2, 0.0}; // since we're drawing the texture scaled by factor 2, we've to multiply the background.width by 2.
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);

        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);  

        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);

        Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // Game Logic
        if (isOnGround(scarfyData, windowDimensions[1])){ // If scarfy is on the ground
            // block is on the ground
            velocity = 0;  
            isInAir = false;   
        } 
        else{
            velocity += gravity*dT; // block is in the air, apply gravity
            isInAir = true;
        } 

        // Jump only when the space key is pressed and the block is on the ground.
        if( IsKeyPressed(KEY_SPACE) && isInAir == false ){
            velocity += jumpVal;
        }

        // Updating Nebule positions
        for(int i = 0; i < numOfNebule; i++){    
            nebulae[i].pos.x += nebVel*dT; // Make the movement frame rate independent
        }

        // Update finish line    
         finishLine = finishLine + nebVel*dT;

        // Updating Scarfy positions
        scarfyData.pos.y += velocity*dT; // Ensures frame rate independent movement

        if( isInAir == false){ // If scarfy is on ground.
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for (int i = 0; i < numOfNebule; i++){
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        for (AnimData nebula: nebulae){

            float pad = 50;
            Rectangle nebRec {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec)){
                collision = true;
            }
        }

        if (collision == true){
            // lose the game
            DrawText("Game Over!", windowDimensions[0]/4, windowDimensions[1]/2, 33,  WHITE );
        }

        else if (scarfyData.pos.x > finishLine)
        {
            DrawText("You Win!", windowDimensions[0]/4, windowDimensions[1]/4, 33, WHITE );
        }
        
        else{
                // Draw each Nebula
                for (int i = 0; i < numOfNebule; i++){    
                    DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                }
                
                // Draw scarfy
                DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
        // End drawing and Game Logic Ends
        EndDrawing();
        
        
    }
        UnloadTexture(scarfy);
        UnloadTexture(nebula);
        UnloadTexture(background);
        UnloadTexture(midground);
        UnloadTexture(foreground);
        CloseWindow();
}