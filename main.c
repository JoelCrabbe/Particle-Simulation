#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include <time.h>


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

 

typedef struct{
    Vector3 position; 
    Vector3 velocity;
    Vector3 acceleration;
    float radius;
    Color color;


}Particle;

//TODO Need to find out how to get the coordinates of each of the walls of the window
//I think the top and bottom coordinates of the wall in 2d are y = +- 22.50 because fovy is centered at (0,0,0) and we have half the fov in each direction
int check_position(Particle* particle){
    //this handles collisions of the particles with the top and bottom edge of the window
    if(particle->position.y - particle->radius <= -22.50f || particle->position.y + particle->radius >= 22.50f){
        particle->velocity.y *= -1;
    }
    else if(particle->position.x + particle->radius >= 22.50f || particle->position.x - particle->radius <= -22.50f){
        particle->velocity.x *= -1;

    }


}

int update_position(Particle* particle){
    //s = ut + 1/2 at^2
    particle->position.x += (particle->velocity.x * GetFrameTime()) + 0.5 * (particle->acceleration.x * GetFrameTime() * GetFrameTime());
    particle->position.y += (particle->velocity.y * GetFrameTime()) + 0.5 * (particle->acceleration.y * GetFrameTime() * GetFrameTime());
    particle->position.z += (particle->velocity.z * GetFrameTime()) + 0.5 * (particle->acceleration.z * GetFrameTime() * GetFrameTime());

}

int update_velocity(Particle* particle){
    //at each new frame we calculate the particles x,y and z velocity using v = u + at
    particle->velocity.x += particle->acceleration.x * GetFrameTime();
    particle->velocity.y += particle->acceleration.y * GetFrameTime(); 
    particle->velocity.z += particle->acceleration.z * GetFrameTime(); 
  
}

int update_acceleration(Particle* particle){
     // I think acceleration might only change when we start adding collisions, as at the moment we are assuming no air resistance

}
//TODO check collisions
int check_collisions(Particle* p1, Particle* p2){

    Vector3 line_through_centers = Vector3Subtract(p1->position, p2->position);
    float distance = Vector3Length(line_through_centers);

    if(distance <= p1->radius + p2->radius){ //if they have collided
        Vector3 unit_vector = Vector3Normalize(line_through_centers);
        float v1 = Vector3DotProduct(p1->velocity, unit_vector);
        float v2 = Vector3DotProduct(p2->velocity, unit_vector); //these give us how much the velocity aligns with the line_through_centers
        
        p1->velocity = Vector3Add(p1->velocity, Vector3Scale(unit_vector, v2 - v1));
        p2->velocity = Vector3Add(p2->velocity, Vector3Scale(unit_vector, v1 - v2));
    }
}

//TODO particles have no sense of energy they will always keep moving (elastic collisions)
//TODO collisions with walls are not detected if they hit a wall or particle not in a frame
int main(void){
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulation");


    Particle* particles = malloc(sizeof(Particle) * 100); //allocating space on the heap for 100 particles
    for(int i = 0; i < 100; i++){
        particles[i].position = (Vector3){0.0f,20.0f,0.0f};

        float vx = ((float) rand() / (float) RAND_MAX) * 10;
        float vy = ((float) rand() / (float) RAND_MAX) * 10;
        float vz = ((float) rand() / (float) RAND_MAX) * 10;
        particles[i].velocity = (Vector3){vx,vy,0.0f};


        float ax = ((float) rand() / (float) RAND_MAX) * 10;
        float ay = ((float) rand() / (float) RAND_MAX) * 10;
        float az = ((float) rand() / (float) RAND_MAX) * 10;
        particles[i].acceleration = (Vector3){0.0f,-4.905f,0.0f};

        float radius = ((float) rand() / (float) RAND_MAX) * 3;
        particles[i].radius = radius;


        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        Color color = {r,g,b,255};
        particles[i].color = color;


    }



    //shader settings
        
    //camera settings
    /*
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_ORTHOGRAPHIC;             // Camera projection type
    */
    
    Camera3D camera = {0};
    camera.position = (Vector3){ 0.0f, 0.0f, 10.0f }; // Camera position in 3D space
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };    // Camera looks at this point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };        // Up vector for the camera
    camera.fovy = 45.0f;                              // Field of view 22.5 degrees up and 22.5 degrees down
    camera.projection = CAMERA_ORTHOGRAPHIC;          
    

    

    
    //Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    //Vector2 cubeScreenPosition = { 0.0f, 0.0f };
    SetTargetFPS(60);


    while(!WindowShouldClose()){
        
        BeginDrawing(); //we want to output graphics so start drawing
        DrawFPS(10,10);
        ClearBackground(WHITE);
        BeginMode3D(camera);
        //UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        //cubeScreenPosition = GetWorldToScreen((Vector3){cubePosition.x, cubePosition.y + 2.5f, cubePosition.z}, camera);
       

        //DrawGrid(10, 1.0f); // the 10 controls the dimensions of the grid (10x10), the 1.0f controls how big each square is


        


        for(int i = 0; i < 100; i++){
            DrawSphere(particles[i].position, particles[i].radius, particles[i].color);
        }


        for(int i = 0; i < 100; i++){
            update_velocity(&particles[i]);
        }

        for(int i = 0; i < 100; i++){
            update_position(&particles[i]);
        }

        for(int i = 0; i < 100; i++){
            check_position(&particles[i]);
        }

        
        for(int i = 0; i < 100; i++){
            for(int j = i + 1; j < 100; j++){
                check_collisions(&particles[i], &particles[j]);
            }
        }

        
    
        
   

     
       

        
        //printf("(%f,%f,%f)\n", p1.position.x, p1.position.y, p1.position.z);







        
 
      
        

        EndMode3D();
        EndDrawing(); //ends the drawing for this particular frame then loops again





        

     

        
    }

    CloseWindow();
    return 0;
}