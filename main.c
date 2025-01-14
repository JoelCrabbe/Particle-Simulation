#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800
#define TOP_OF_SCREEN_POS (Vector3){0.0f, 22.50f, 0.0f}
 



//should probably declare which directions are taken as positive
/*

y direction: downwards = positive, upwards = negative
x direction: right = positive, left = negative
z direction: doesn't matter at the moment


*/

typedef struct{
    Vector3 position; 
    Vector3 velocity;
    Vector3 acceleration;

}Particle;



//acceleration in the y direction at any given moment is gravity + particle->acceleration.y (particle->acceleration.y is due to an initial acceleration)


//check if particle is outside window
//TODO Need to find out how to get the coordinates of each of the walls of the window
int check_position(Particle* particle){

    if(particle->position.y < -32.0f){
        particle->position = TOP_OF_SCREEN_POS; //reset particle to origin
    }
}


//we know distance = speed x time
int update_position(Particle* particle){
    particle->position.y = -100 * (particle->velocity.y * GetFrameTime()) + 0.5 * (particle->acceleration.y * GetFrameTime() * GetFrameTime());
    // s = ut + 1/2 at^2
    //the -100 somehow makes the particles move downwards and also makes them have a better speed, so they move faster
    //instead of incredibly slow

}

int update_velocity(Particle* particle){
    //TODO im not sure y velocity is calculated correctly. Do i have to account for gravity and initial acceleration?
    //this method works but the numbers become insanely large very fast
    //so at each frame we need to calculate the particles new velocity in each direction (x,y,z) based off its current conditions, essentially SUVAT I think   
    particle->velocity.y = (particle->velocity.y + (particle->acceleration.y * GetFrameTime())); 
    particle->velocity.x = (particle->velocity.x + (particle->acceleration.x * GetFrameTime()));
    particle->velocity.z = (particle->velocity.z + (particle->acceleration.z * GetFrameTime()));
    //new velocity = inital velocity + acceleration x dt (essentially)  v = u + at

}

int update_acceleration(Particle* particle){
     // I think acceleration might only change when we start adding collisions, as at the moment we are assuming no air resistance
     // and acceleration is constant in the y direction


}

//TODO I need to always initialise particles with an acceleration in the y direction of 9.81 and then add ontop of that the initial acceleration (I think this has been done)
int main(void){
    //                   position         velocity         acceleration
    Particle p1 = {{0.0f,22.5f,0.0f}, {2.0f,1.0f,5.0f}, {2.0f,4.905 + 1.0f,2.0f}}; //the 4.905 + is accounting for gravity. Even if a particle has 0 initial acceleration, it still falls due to gravity
    Particle p2 = {{2.0f,22.5f,0.0f}, {2.0f,1.0f,5.0f}, {2.0f,4.905 + 1.0f,2.0f}};
    //Acceleration in the y direction is given by 4.905 + (initial acceleration)
    //when the particles have 0 acceleration they should still move due to gravity

    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulation");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f }; // Camera position in 3D space
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };    // Camera looks at this point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };        // Up vector for the camera
    camera.fovy = 45.0f;                              // Field of view
    camera.projection = CAMERA_ORTHOGRAPHIC;  
        
    SetTargetFPS(60);

    //I think this while loop loops 60 times each frame as 60FPS
    while(!WindowShouldClose()){
        

        BeginDrawing(); //we want to output graphics so start drawing
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);


        DrawSphere(p1.position, 0.2f, BLUE);
        DrawSphere(p2.position, 0.2f, RED);

        update_velocity(&p1);
        update_velocity(&p2);

        update_position(&p1);
        update_position(&p2);

        printf("p1: %f\n", p1.acceleration.y);
        printf("p2: %f\n", p2.acceleration.y);

        //printf("p1: %f\n", p1.velocity.y);
        //printf("p2: %f\n", p2.velocity.y);




        //update position due to gravity
        //gravity(&p1);
        //gravity(&p2);
        //update_velocity(&p1);
        //update_velocity(&p2);
        //printf("(%f,%f,%f)\n", p1.position.x, p1.position.y, p1.position.z);
        check_position(&p1);
        check_position(&p2);
        //printf("x = %f\n", p1.velocity.x);
        //printf("y = %f\n", p1.velocity.y);
        //printf("z = %f\n", p1.velocity.z);
        
 
      
        


        EndMode3D();
        EndDrawing(); //ends the drawing for this particular frame then loops again





        

     

        
    }

    CloseWindow();


    return 0;
}