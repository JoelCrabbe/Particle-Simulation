#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

typedef struct{
    Vector3 position; 
    Vector3 velocity;
    Vector3 acceleration;
    float radius;
    Color color;

}Particle;

typedef struct{
    Vector3 position;
    float radius;
    Color color;
}Container;


int check_container_collision(Particle* particle, Container* container){
    //if particles coordinates ever satisfy (x-a)^2 + (y-b)^2 + (z-c)^2 = r^2, then do something. a,b,c is the centre of the sphere
    float radius_squared = container->radius * container->radius;
    float equation = (float) pow(particle->position.x - container->position.x, 2) + (float) pow(particle->position.y - container->position.y, 2) + (float) pow(particle->position.z - container->position.z, 2);
    if(equation > radius_squared){ //then it has hit the sphere container
        Vector3 line_through_centers = Vector3Subtract(particle->position, container->position);
        Vector3 unit_vector = Vector3Normalize(line_through_centers);
        float magnitude = Vector3Length(particle->velocity);

        particle->velocity = Vector3Scale(unit_vector, -1 * magnitude);

    }





}



//TODO improve collisions with walls because particles can kind of get stuck against walls=
//I think the top and bottom coordinates of the wall in 2d are y = +- 22.50 because fovy is centered at (0,0,0) and we have half the fov in each direction
int check_position(Particle* particle, Camera3D* camera){
    float edge_position = camera->target.y + camera->fovy / 2;
    //this handles collisions of the particles with the top and bottom edge of the window
    if(particle->position.y - particle->radius < -1 * edge_position || particle->position.y + particle->radius > edge_position){
        particle->velocity.y *= -1;
    }
    //collision with left and right edge of window
    else if(particle->position.x + particle->radius > edge_position || particle->position.x - particle->radius < -1 * edge_position){
        particle->velocity.x *= -1;
    }
     else if(particle->position.z + particle->radius > 5.0f || particle->position.x - particle->radius < -1 * 5.0f){
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

//TODO check collisions this somewhat works now
int check_collisions(Particle* p1, Particle* p2){
    Vector3 line_through_centers = Vector3Subtract(p1->position, p2->position);
    float distance = Vector3Length(line_through_centers);
    Vector3 unit_vector = Vector3Normalize(line_through_centers);

    if(distance < p1->radius + p2->radius){ //then the particles have collided
    float overlap = distance - (p1->radius + p2->radius);

    p1->position = Vector3Subtract(p1->position, Vector3Scale(unit_vector, overlap / 2.0f));
    p2->position = Vector3Add(p2->position, Vector3Scale(unit_vector, overlap / 2.0f));

    float p1_magnitude = Vector3Length(p1->velocity); //calculate size of velocities
    float p2_magnitude = Vector3Length(p2->velocity);


    p1->velocity = Vector3Scale(unit_vector, p2_magnitude); //new velocity is along the line_through_centers, move in opposite directions, velocities swap
    p2->velocity = Vector3Scale(unit_vector, -1 * p1_magnitude);


    }
}

//TODO particles have no sense of energy they will always keep moving (elastic collisions) not very realistic
//TODO collisions with walls are not detected if they hit a wall or particle between frames
int main(void){
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulation");

    Color color = {125,125,125,50};
    Container container = {{0.0f,3.20f,0.0f}, 3.20f, color};
    
    Particle p1 = {{0.0f,4.20f,0.0f}, {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, 0.7f};
    /*
    Particle p2 = {{0.0f,4.0f,0.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f}, 1.2f};
    Particle p3 = {{16.0f,5.0f,0.0f}, {8.0f,2.0f,0.0f}, {0.0f,0.0f,0.0f}, 3.2f};
    Particle p4 = {{3.0f,2.0f,0.0f}, {4.0f,10.0f,0.0f}, {0.0f,0.0f,0.0f}, 3.2f};
    */
    
    

    int size = 100;
    Particle* particles = malloc(sizeof(Particle) * size); //allocating space on the heap for size particles
    for(int i = 0; i < size; i++){

        float px = ((float) rand() / (float) RAND_MAX) * 22.50;
        float py = ((float) rand() / (float) RAND_MAX) * 22.50;
        float pz = ((float) rand() / (float) RAND_MAX) * 22.50;
        particles[i].position = (Vector3){px,py,pz};


        float vx = ((float) rand() / (float) RAND_MAX) * 10;
        float vy = ((float) rand() / (float) RAND_MAX) * 10;
        float vz = ((float) rand() / (float) RAND_MAX) * 10;
        particles[i].velocity = (Vector3){vx,vy,vz};


        float ax = ((float) rand() / (float) RAND_MAX) * 10;
        float ay = ((float) rand() / (float) RAND_MAX) * 10;
        float az = ((float) rand() / (float) RAND_MAX) * 10;
        particles[i].acceleration = (Vector3){0.0f,-4.905f,0.0f};

        float radius = ((float) rand() / (float) RAND_MAX) * 3;
        particles[i].radius = 0.4f;


        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        Color color = {r,g,b,255};
        particles[i].color = BLUE;

    }

    Camera3D camera = {0};
    camera.position = (Vector3){ 0.0f, 0.0f, 10.0f }; // Camera position in 3D space
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };    // Camera looks at this point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };        // Up vector for the camera
    camera.fovy = 45.0f;                              // Field of view 22.5 degrees up and 22.5 degrees down
    camera.projection = CAMERA_PERSPECTIVE;        

    //edges of screens calculateed by  camera.target.y + fovy / 2
    
    //Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    //Vector2 cubeScreenPosition = { 0.0f, 0.0f };
    SetTargetFPS(60);


    while(!WindowShouldClose()){
        
        BeginDrawing(); //we want to output graphics so start drawing
        DrawFPS(10,10);
        ClearBackground(WHITE);
        BeginMode3D(camera);
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        //cubeScreenPosition = GetWorldToScreen((Vector3){cubePosition.x, cubePosition.y + 2.5f, cubePosition.z}, camera);
       

        DrawGrid(10, 1.0f); // the 10 controls the dimensions of the grid (10x10), the 1.0f controls how big each square is
        DrawSphere(p1.position, p1.radius, RED);
        DrawSphere(container.position, 3.20f, color);


        update_velocity(&p1);
        update_position(&p1);
        check_container_collision(&p1, &container);



        /*
        DrawSphere(p3.position, p3.radius, BLUE);
        DrawSphere(p4.position, p4.radius, YELLOW);

        update_velocity(&p2);
        update_velocity(&p3);
        update_velocity(&p4);
       

        update_position(&p2);
        update_position(&p3);
        update_position(&p4);
       

        check_position(&p2, &camera);
        check_position(&p3, &camera);
        check_position(&p4, &camera);
       

        check_collisions(&p1, &p2);
        check_collisions(&p1, &p3);
        check_collisions(&p1, &p4);
        check_collisions(&p2, &p3);
        check_collisions(&p2, &p4);
        check_collisions(&p3, &p4);
        */
        

    
        /*
        for(int i = 0; i < size; i++){
            DrawSphere(particles[i].position, particles[i].radius, particles[i].color);
        }


        for(int i = 0; i < size; i++){
            update_velocity(&particles[i]);
        }

        for(int i = 0; i < size; i++){
            update_position(&particles[i]);
        }

        for(int i = 0; i < size; i++){
            check_position(&particles[i], &camera);
        }

        
        for(int i = 0; i < size; i++){
            for(int j = i + 1; j < size; j++){
                check_collisions(&particles[i], &particles[j]);
            }
        }

        */

        


        
        

        

        
        


        EndMode3D();
        EndDrawing(); //ends the drawing for this particular frame then loops again
        

        
    }

    CloseWindow();
    free(particles);
    return 0;
    }

