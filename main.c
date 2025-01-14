#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800
 

typedef struct{
    Vector3 position; 
    Vector3 velocity;
    Vector3 acceleration;

}Particle;

//TODO Need to find out how to get the coordinates of each of the walls of the window
int check_position(Particle* particle){
  
}

int update_position(Particle* particle){
    //TODO need to update position for z direction
    //s = ut + 1/2 at^2
    particle->position.x += (particle->velocity.x * GetFrameTime()) + 0.5 * (particle->acceleration.x * GetFrameTime() * GetFrameTime());
    particle->position.y += (particle->velocity.y * GetFrameTime()) + 0.5 * (particle->acceleration.y * GetFrameTime() * GetFrameTime());

}

int update_velocity(Particle* particle){
    //TODO need to update velocity for z direction
    //TODO I think the calculation is fine but if the particle has a constant acceleration then it should be speeding up on the screen
    //at each new frame we calculate the particles x,y and z velocity using v = u + at
    particle->velocity.x += particle->acceleration.x * GetFrameTime();
    particle->velocity.y += particle->acceleration.y * GetFrameTime(); 
  
}

int update_acceleration(Particle* particle){
     // I think acceleration might only change when we start adding collisions, as at the moment we are assuming no air resistance


}

int main(void){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulation");
    
    //                   position         velocity         acceleration
    Particle p1 = {{0.0f,15.0f,0.0f}, {1.0f,-1.0f,0.0f}, {1.0f,-4.905f,0.0f}}; 
    Particle p2 = {{2.0f,15.0f,0.0f}, {2.0f,-2.0f,0.0f}, {2.0f,-4.905f,0.0f}};
    Particle p3 = {{4.0f,15.0f,0.0f}, {3.0f,-3.0f,0.0f}, {3.0f,-4.905f,0.0f}};
        
    //camera settings
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f }; // Camera position in 3D space
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };    // Camera looks at this point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };        // Up vector for the camera
    camera.fovy = 45.0f;                              // Field of view
    camera.projection = CAMERA_ORTHOGRAPHIC;          // this camera projection makes it so particles always appear the same size

        
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        
        BeginDrawing(); //we want to output graphics so start drawing
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

        DrawSphere(p1.position, 0.2f, BLUE);
        DrawSphere(p2.position, 0.2f, RED);
        DrawSphere(p3.position, 0.2f, GREEN);

        update_velocity(&p1);
        update_position(&p1);

        update_velocity(&p2);
        update_position(&p2);

        update_velocity(&p3);
        update_position(&p3);







        
 
      
        


        EndMode3D();
        EndDrawing(); //ends the drawing for this particular frame then loops again





        

     

        
    }

    CloseWindow();
    return 0;
}