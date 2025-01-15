#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

 

typedef struct{
    Vector3 position; 
    Vector3 velocity;
    Vector3 acceleration;
    float radius;


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
    //calculate the distance between the two particles centers
    Vector3 vector = Vector3Subtract(p1->position, p2->position);
    float distance = (float) sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
    if(distance <= p1->radius + p2->radius){
        //we need to move the particles away from each other along a line which passes through both their centres
        //Vector3 a = p1->position;
        //float alpha;
        //Vector3 r = Vector3Add(a, Vector3Scale(vector, alpha)); //this is the line passing through the centers of both particles
        //so r is the vector equation of the line passing through the centers of p1 and p2

        Vector3 normal = Vector3Scale(vector, 1/distance); //normal unit vector
        float p1_velocity_along_normal =  Vector3DotProduct(p1->velocity, normal);
        p1->velocity = Vector3Scale(p1->velocity, p1_velocity_along_normal);

        float p2_velocity_along_normal = Vector3DotProduct(p2->velocity, normal);
        p2->velocity = Vector3Scale(p2->velocity, p2_velocity_along_normal);





        




        /*
        p1->velocity.y = p1->velocity.y * -1;
        p2->velocity.y = p2->velocity.y * -1;

        p1->velocity.x = p1->velocity.x * -1;
        p2->velocity.x = p2->velocity.x * -1;

        p1->velocity.z = p1->velocity.z * -1;
        p2->velocity.z = p2->velocity.z * -1;

        */

        



        




    }

}

//TODO particles have no sense of energy they will always keep moving (elastic collisions)
//TODO collisions with walls are not detected if they hit a wall or particle not in a frame
int main(void){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulation");

    //                   position         velocity         acceleration      radius
    Particle p1 = {{10.0f,15.0f,0.0f}, {-2.0f,-4.0f,0.0f}, {0.0f,-4.905f,0.0f}, 3.0f}; 
    Particle p2 = {{4.0f,15.0f,0.0f}, {2.0f,6.0f,0.0f}, {0.0f,-4.905f,0.0f}, 3.0f};
    Particle p3 = {{4.0f,0.0f,0.0f}, {-3.0f,-4.0f,0.0f}, {0.0f,-4.905f,0.0f}, 3.0f};
    Particle p4 = {{6.0f,-9.0f,0.0f}, {0.0f,-2.0f,0.0f}, {0.0f,-4.905f,0.0f}, 3.0f};
    Particle p5 = {{8.0f,-12.0f,0.0f}, {10.0f,2.9f,0.0f}, {0.0f,-4.905f,0.0f}, 3.0f};
    Particle p6 = {{7.0f,-15.0f,0.0f}, {15.0f,1.5f,0.0f}, {0.0f,-4.905f,0.0f}, 3.0f};

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
    SetTargetFPS(144);


    while(!WindowShouldClose()){
        
        BeginDrawing(); //we want to output graphics so start drawing
        ClearBackground(WHITE);
        BeginMode3D(camera);
        //UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        //cubeScreenPosition = GetWorldToScreen((Vector3){cubePosition.x, cubePosition.y + 2.5f, cubePosition.z}, camera);
       

        //DrawGrid(10, 1.0f); // the 10 controls the dimensions of the grid (10x10), the 1.0f controls how big each square is
        DrawSphere(p1.position, p1.radius, BLUE);
        DrawSphere(p2.position, p2.radius, RED);
        DrawSphere(p3.position, p2.radius, GREEN);
        DrawSphere(p4.position, p2.radius, GREEN);
        DrawSphere(p5.position, p2.radius, GREEN);
        DrawSphere(p6.position, p2.radius, GREEN);

    
        update_velocity(&p1);
        update_velocity(&p2);
        update_velocity(&p3);
        update_velocity(&p4);
        update_velocity(&p5);
        update_velocity(&p6);

        update_position(&p1);
        update_position(&p2);
        update_position(&p3);
        update_position(&p4);
        update_position(&p5);
        update_position(&p6);

        check_position(&p1);
        check_position(&p2);
        check_position(&p3);
        check_position(&p4);
        check_position(&p5);
        check_position(&p6);

       check_collisions(&p1, &p2);
       check_collisions(&p1, &p3);
       check_collisions(&p1, &p4);
       check_collisions(&p1, &p5);
       check_collisions(&p1, &p6);
       check_collisions(&p2, &p3);
       check_collisions(&p2, &p4);
       check_collisions(&p2, &p5);
       check_collisions(&p2, &p6);
       check_collisions(&p3, &p4);
       check_collisions(&p3, &p5);
       check_collisions(&p3, &p6);
       check_collisions(&p4, &p5);
       check_collisions(&p4, &p6);
       check_collisions(&p5, &p6);
        //printf("%f\n", p1.velocity.y);

       

        
        //printf("(%f,%f,%f)\n", p1.position.x, p1.position.y, p1.position.z);







        
 
      
        

        EndMode3D();
        EndDrawing(); //ends the drawing for this particular frame then loops again





        

     

        
    }

    CloseWindow();
    return 0;
}