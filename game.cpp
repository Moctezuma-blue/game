#include <raylib.h>
#include <string>
#include <vector>
#include <raymath.h>
#include <math.h>

//Structs

typedef struct province{
    int id;
    std::string name;
    std::vector<Vector2> vertices;
    Vector2 center;
    int NationId;
}province;

typedef struct nation{
    int id;
    std::string name;
    std::vector<province> provinces;
    int r;
    int g;
    int b;
}nation;

//Variables

const float MAP_WIDTH = 8000.0f;
const float MAP_HEIGHT = 3000.0f;
const float SCREEN_WIDTH = 1080.0f;
const float SCREEN_HEIGHT = 720.0f;
const float EDGE_MARGIN = 5.0f;
const float EDGE_SCROLL_SPEED = 500.0f;
const float MIN_ZOOM = SCREEN_WIDTH/MAP_WIDTH; // Minimum zoom to fit the entire map on screen
const float MAX_ZOOM = 10.0f;
const float ZOOM_SPEED = 0.1f;

//Functions

void displayMSG(){
    if (IsKeyDown(KEY_P)){
        DrawText("Federation Lives!", 290, 200, 20, LIGHTGRAY);
    }
}

void ToggleMonitorFullscreen(){ //fuction to toggle fulscreen with button and not with OS bar buttons
    if (!IsWindowFullscreen()){
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else {
        ToggleFullscreen(); 
        SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        ToggleFullscreen(); 
        int monitor = GetCurrentMonitor();
        Vector2 monitorPos = GetMonitorPosition(monitor);
        int x =(int)monitorPos.x + (GetMonitorWidth(monitor) - SCREEN_WIDTH)/2;
        int y =(int)monitorPos.y + (GetMonitorHeight(monitor) - SCREEN_HEIGHT)/2;
        SetWindowPosition(x, y);
    }
}

//Main

int main(){
SetTraceLogLevel(LOG_WARNING); // Set log level to INFO
SetConfigFlags(FLAG_WINDOW_RESIZABLE);//Fullscreen
InitWindow(1080, 720, "Federation: The End of The Earth Editor");
SetTargetFPS(60);

//Texture Loading !!!! Don't forget to Unload them later to avoid memory leaks

Texture2D FederationFlag = LoadTexture("assets/flags/FederationFlag.png");
Texture2D USFlag = LoadTexture("assets/flags/USFlag.png");
Texture2D AztecEmpireFlag = LoadTexture("assets/flags/AztecEmpireFlag.png");

//Camera

Camera2D camera = { 0 };
camera.target = (Vector2){ 0, 0 };
camera.offset = (Vector2){ 0, 0 };
camera.rotation = 0.0f;
camera.zoom = 1.0f;

while(!WindowShouldClose()){

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        camera.target.x -= GetMouseDelta().x/camera.zoom;
        camera.target.y -= GetMouseDelta().y/camera.zoom;
    }

    camera.target.x = Clamp(camera.target.x, 0.0f, MAP_WIDTH);
    camera.target.y = Clamp(camera.target.y, 0.0f, MAP_HEIGHT);

    Vector2 mousePosition = GetMousePosition();
    float dt = GetFrameTime();

    if(IsWindowFocused()){ //edge scrolling
    // Left edge scroll
    if (mousePosition.x >= 0 && mousePosition.x < EDGE_MARGIN) {
     camera.target.x -= (EDGE_SCROLL_SPEED * dt) / camera.zoom;
    }
    // Right edge scroll
    if (mousePosition.x > GetScreenWidth() - EDGE_MARGIN && mousePosition.x <= GetScreenWidth()) {
        camera.target.x += (EDGE_SCROLL_SPEED * dt) / camera.zoom;
    }
    // Top edge scroll
    if (mousePosition.y >= 0 && mousePosition.y < EDGE_MARGIN) {
        camera.target.y -= (EDGE_SCROLL_SPEED * dt) / camera.zoom;
    }
    // Bottom edge scroll
    if (mousePosition.y > GetScreenHeight() - EDGE_MARGIN && mousePosition.y <= GetScreenHeight()) {
        camera.target.y += (EDGE_SCROLL_SPEED * dt) / camera.zoom;
    }
    }

    float minZoomX = (float)GetScreenWidth() / MAP_WIDTH;
    float minZoomY = (float)GetScreenHeight() / MAP_HEIGHT;
    float dynamicMinZoom = fmaxf(minZoomX, minZoomY);   
    float whell = GetMouseWheelMove();
    if (whell != 0.0f) {

        Vector2 mouseWorldPosBeforeZoom = GetScreenToWorld2D(mousePosition, camera);
        camera.zoom += whell * ZOOM_SPEED;
        camera.zoom = Clamp(camera.zoom, dynamicMinZoom, MAX_ZOOM);
        Vector2 mouseWorldPosAfterZoom = GetScreenToWorld2D(mousePosition, camera);
        camera.target.x += (mouseWorldPosBeforeZoom.x - mouseWorldPosAfterZoom.x);
        camera.target.y += (mouseWorldPosBeforeZoom.y - mouseWorldPosAfterZoom.y);
        camera.target.x = fmodf(camera.target.x, MAP_WIDTH);
        if (camera.target.x < 0.0f) camera.target.x += MAP_WIDTH;
    }

    if (camera.zoom < dynamicMinZoom) {
    camera.zoom = dynamicMinZoom;
    }

    float visibleWidth = GetScreenWidth() / camera.zoom;
    float visibleHeight = GetScreenHeight() / camera.zoom;
    camera.target.y = Clamp(camera.target.y, 0.0f, MAP_HEIGHT - visibleHeight);

    camera.target.x = fmodf(camera.target.x, MAP_WIDTH);
    if(camera.target.x < 0.0f) camera.target.x += MAP_WIDTH;




    BeginDrawing();



    ClearBackground(SKYBLUE);
    BeginMode2D(camera);
    DrawText("Hello, World!", 200, 200, 20, LIGHTGRAY);
    displayMSG();
    DrawTexture(FederationFlag, 1000, 1000, WHITE);
    DrawTexture(USFlag, 7000, 2000, WHITE);
    DrawTexture(AztecEmpireFlag, 4000, 2000, WHITE);

    EndMode2D();



    EndDrawing();

}

//Texture Unload !!!! Important to avoid memory leaks

UnloadTexture(FederationFlag);
UnloadTexture(USFlag);
UnloadTexture(AztecEmpireFlag);


CloseWindow();
    return 0;
}