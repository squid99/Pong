#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include <SDL2/SDL_ttf.h> 

int ball_hit=0;
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
SDL_Color text_color = {255, 255, 255, 155}; 

int last_frame_time = 0;

struct player1{
    float x;
    float y;
    float width;
    float height;
    int speed;
    int move_up;
    int move_down;
    int score;
} player1;

struct player2{
    float x;
    float y;
    float width;
    float height;
    int speed;
    int move_up;
    int move_down;
    int score;
} player2;

struct ball{
    float x;
    float y;
    float width;
    float height;
    int speedx;
    int speedy;
} ball;

int initialize_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) !=0){
        fprintf(stderr,"Error Initializing SDL.\n");
        return FALSE;   
    }
    window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if(!window){
        fprintf(stderr,"Error creating SDL window.\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer){
        fprintf(stderr,"Error creasinf SDL renderer.\n");
        return FALSE;
    }

//didn't understand shit about font and text falan filan
    if (TTF_Init() == -1) {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
        return FALSE; // Stop the game if the engine fails
    }

    font = TTF_OpenFont("vga857.ttf", 100);
    
    if (font == NULL) {
        // This will tell you exactly why the font isn't loading
        fprintf(stderr, "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return FALSE; // Stop the game before the render loop crashes it
    }

    return TRUE;
}

void setup(){
    ball.x=WINDOW_WIDTH/2;
    ball.y=WINDOW_HEIGHT/2;
    ball.width=14;
    ball.height=14;
    ball.speedx=BALL_SPEED_X;
    ball.speedy=BALL_SPEED_Y;

    player1.width= 10;
    player1.height= 90;
    player1.x= WINDOW_WIDTH-10-player1.width;
    player1.y= WINDOW_HEIGHT/2 - player1.height/2;
    player1.speed= PLAYER_SPEED;
    player1.move_up=0;
    player1.move_down=0;
    player1.score=0;

    player2.width= 10;
    player2.height= 90;
    player2.x= 10;
    player2.y= WINDOW_HEIGHT/2 - player2.height/2;
    player2.speed= PLAYER_SPEED;
    player2.move_up=0;
    player2.move_down=0;
    player2.score=0;
}

void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type){
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){ game_is_running = FALSE; }
             if(event.key.keysym.sym == SDLK_UP){ player1.move_up=1;}
            else if(event.key.keysym.sym == SDLK_DOWN){player1.move_down=1;}
            if(event.key.keysym.sym == SDLK_w){player2.move_up=1;}
            else if(event.key.keysym.sym == SDLK_s){player2.move_down=1;}
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_UP){player1.move_up=0;}
            else if(event.key.keysym.sym == SDLK_DOWN){player1.move_down=0;}
            if(event.key.keysym.sym == SDLK_w){player2.move_up=0;}
            else if(event.key.keysym.sym == SDLK_s){player2.move_down=0;}
            break;
    }
}

void update(){

    //target time in millisecond
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    //delay if there is more time
    if(time_to_wait>0 && time_to_wait<= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }
//ball hitting players
    if(ball.x+ball.width>=WINDOW_WIDTH-player1.width-10 && (ball.y+ball.height/2 <=player1.y+player1.height && ball.y+ball.height/2>=player1.y)){ ball.x=WINDOW_WIDTH-player1.width-ball.width-10;
         ball_hit++; ball.speedx +=4*ball_hit; ball.speedx *= -1;ball.speedy=BALL_SPEED_Y * ((player1.y+player1.height/2)-(ball.y+ball.height/2))*-1;}
    if(ball.x<=player2.width+player2.x && (ball.y+ball.height/2 <=player2.y+player2.height && ball.y+ball.height/2 >=player2.y)){ ball.x=player2.width+player2.x; ball_hit++; ball.speedx *= -1; 
        ball.speedx +=4*ball_hit;ball.speedy=BALL_SPEED_Y * ((player2.y+player2.height/2)-(ball.y+ball.height/2)) *-1;}
//ball hitting sides
    if(ball.y<=2){ball.y=2;ball.speedy*=-1;}
    else if(ball.y>=WINDOW_HEIGHT-ball.height-2){ball.y=WINDOW_HEIGHT-ball.height-2;ball.speedy*=-1;}
//score
    if(ball.x>WINDOW_WIDTH+5){ball.x=WINDOW_WIDTH/2; ball.y=WINDOW_HEIGHT/2; ball_hit=0; ball.speedx=BALL_SPEED_X; ball.speedx*=-1; player2.score++;}
    if(ball.x<0){ball.x=WINDOW_WIDTH/2; ball.y=WINDOW_HEIGHT/2; ball_hit=0; ball.speedx=BALL_SPEED_X; ball.speedx*=-1; player1.score++;}

//time gap for speed
    float delta_time = (SDL_GetTicks() - last_frame_time)/1000.0f;
    last_frame_time = SDL_GetTicks();
//movement for p2
    if(player2.move_up){player2.y -= player2.speed * delta_time;}
    else if(player2.move_down){player2.y += player2.speed * delta_time;}
//movement for p1
    if(player1.move_up){player1.y -= player1.speed * delta_time;}
    else if(player1.move_down){player1.y += player1.speed * delta_time;}
//border for p2
    if(player2.y<0){player2.y=2;}
    else if(player2.y>WINDOW_HEIGHT-2-player2.height){player2.y=WINDOW_HEIGHT-2-player2.height;}
//border for p1
    if(player1.y<0){player1.y=2;}
    else if(player1.y>WINDOW_HEIGHT-2-player1.height){player1.y=WINDOW_HEIGHT-2-player1.height;}
    
    ball.x += ball.speedx * delta_time;
    ball.y += ball.speedy * delta_time;
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect ball_rect ={(int)ball.x, (int)ball.y, (int)ball.width, (int)ball.height};
    SDL_Rect player1_rect ={(int)player1.x, (int)player1.y, (int)player1.width, (int)player1.height};
    SDL_Rect player2_rect = { (int)player2.x, (int)player2.y, (int)player2.width, (int)player2.height};
//players
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player2_rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player1_rect);
//ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//net in the middle
    for (int i = 0; i < WINDOW_HEIGHT; i += 40) {SDL_Rect net_piece = {WINDOW_WIDTH / 2 - 2, i + 10, 4, 20};SDL_RenderFillRect(renderer, &net_piece);}
//frame
    SDL_RenderDrawLineF(renderer,2,2,2,WINDOW_HEIGHT-2);
    SDL_RenderDrawLineF(renderer,2,2,WINDOW_WIDTH-2,2);
    SDL_RenderDrawLineF(renderer,WINDOW_WIDTH-2,2,WINDOW_WIDTH-2,WINDOW_HEIGHT-2);
    SDL_RenderDrawLineF(renderer,2,WINDOW_HEIGHT-2,WINDOW_WIDTH-2,WINDOW_HEIGHT-2);
//don't know this part but something about score from gemini
    char p1_str[10];
    char p2_str[10];
    snprintf(p1_str, sizeof(p1_str), "%d", player2.score);
    snprintf(p2_str, sizeof(p2_str), "%d", player1.score);

    SDL_Surface* p1_surf = TTF_RenderText_Solid(font, p1_str, text_color);
    SDL_Surface* p2_surf = TTF_RenderText_Solid(font, p2_str, text_color);

    SDL_Texture* p1_tex = SDL_CreateTextureFromSurface(renderer, p1_surf);
    SDL_Texture* p2_tex = SDL_CreateTextureFromSurface(renderer, p2_surf);

    int scale_factor = 2; // Change this to 3 or 4 for massive text

    SDL_Rect p1_rect;
    p1_rect.w = p1_surf->w * scale_factor;
    p1_rect.h = p1_surf->h * scale_factor;
    p1_rect.x = (WINDOW_WIDTH / 4) - (p1_rect.w / 2); // Keeps it centered
    p1_rect.y = 20;

    SDL_Rect p2_rect;
    p2_rect.w = p2_surf->w * scale_factor;
    p2_rect.h = p2_surf->h * scale_factor;
    p2_rect.x = ((WINDOW_WIDTH / 4) * 3) - (p2_rect.w / 2); // Keeps it centered
    p2_rect.y = 20;

    SDL_FreeSurface(p1_surf);
    SDL_FreeSurface(p2_surf);

    SDL_RenderCopy(renderer, p1_tex, NULL, &p1_rect);
    SDL_RenderCopy(renderer, p2_tex, NULL, &p2_rect);

    SDL_DestroyTexture(p1_tex);
    SDL_DestroyTexture(p2_tex);
    SDL_RenderPresent(renderer);
}

void destroy_window(){
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    game_is_running = initialize_window();

    setup();

    while(game_is_running){
        process_input();
        update();
        render();
    }
    
    destroy_window();

    return 0;
}
