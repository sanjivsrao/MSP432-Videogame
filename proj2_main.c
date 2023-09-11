/**
 * Starter code for Project 2. Good luck!
 *
 * We recommending copy/pasting your HAL folder from Project 1
 * into this project.
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Include for randomizer */
#include <time.h>

/* HAL and Application includes */
#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <Application.h>
#include <HAL/Joystick.h>


int main(void)
{
    // Stop Watchdog Timer - THIS SHOULD ALWAYS BE THE FIRST LINE OF YOUR MAIN
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer, used to enable
    // software timers to time their measurements properly.
    InitSystemTiming();

    // Initialize the main Application object and the HAL.
    HAL hal = HAL_construct();

    // Main constructors for the application as a whole and the game screen
    App_proj2 app = App_proj2_construct(&hal);
    Game game = Game_construct(&hal);

    // Shows the title screen off of startup
    App_proj2_showTitleScreen(&hal.gfxContext);

    // Main super-loop! In a polling architecture, this function should call
    // your main FSM function over and over.
    while (true)
    {
        App_proj2_loop(&app, &game, &hal);  //update my program, application state, output
        HAL_refresh(&hal); // check the inputs
    }
}

App_proj2 App_proj2_construct(HAL* hal_p)
{
    // The App_proj2 object to initialize
    App_proj2 app;

//    // Initialization of FSM variables
    app.state = TITLE_SCREEN;
    app.timer = SWTimer_construct(TITLE_SCREEN_WAIT);
    SWTimer_start(&app.timer);

    // Return the completed Application struct to the user
    return app;
}

Game Game_construct(HAL* hal_p)
{
    // The GAME object to initialize
    Game game;

    // Initialization of FSM variables
    game.state = RESET;

    // Initialization of timer variables
    game.pregame_timer = SWTimer_construct(PREGAME_SCREEN_WAIT);
    game.game_timer = SWTimer_construct(GAME_SCREEN_WAIT);
    game.one_timer = SWTimer_construct(TEMPORARY_WAIT);

    //Constant values used to keep track of game progress
    game.lives = NUMBER_OF_LIVES;
    game.score = 000000;

    game.counter = 0;  //Simple counter that identifies what game to play in defined order

    // Initializing the top 3 high scores
    game.hs[0] = 0;
    game.hs[1] = 0;
    game.hs[2] = 0;

    // Return the completed Application struct to the user
    return game;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time.
 */
void App_proj2_loop(App_proj2* app_p, Game* game_p, HAL* hal_p)
{
    if (Button_isPressed(&hal_p->launchpadS1))
        LED_turnOn(&hal_p->launchpadLED1);
    else
        LED_turnOff(&hal_p->launchpadLED1);
    // Main switch statement for LED screen states
    switch (app_p->state)
    {
        case TITLE_SCREEN:
            App_proj2_handleTitleScreen(app_p, hal_p);
            break;
        case MENU_SCREEN:
            App_proj2_handleMenuScreen(app_p, game_p, hal_p);
            break;
        case INSTRUCTIONS_SCREEN:
            App_proj2_handleInstructionsScreen(app_p, hal_p);
            break;
        case HIGHSCORE_SCREEN:
            App_proj2_handleHSScreen(app_p, hal_p);
            break;
        case GAME_SCREEN:
            App_proj2_handleGameScreen(app_p, game_p, hal_p);
            break;
        case RESULT_SCREEN:
            App_proj2_handleResultScreen(app_p, hal_p, game_p);
            break;

        default:
            break;
    }
}

void App_proj2_showTitleScreen(GFX* gfx_p)
{
    // Clears the screen
    GFX_clear(gfx_p);

    // Draws on title screen
    GFX_print(gfx_p, "WARIO WARE 9000  ", 0, 0);
    GFX_print(gfx_p, "---------------------", 1, 0);
    GFX_print(gfx_p, "By: Sanjiv Rao", 2, 0);
}

void App_proj2_handleTitleScreen(App_proj2* app_p, HAL* hal_p)
{
    // Transitions from Title screen to Menu screen
    if (SWTimer_expired(&app_p->timer))
    {
        app_p->state = MENU_SCREEN;
        App_proj2_showMenuScreen(app_p, &hal_p->gfxContext);

    }
}

void App_proj2_showMenuScreen(App_proj2* app_p, GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);

    // Display the text
    GFX_print(gfx_p, "Menu         ", 0, 0);
    GFX_print(gfx_p, "---------------------", 1, 0);
    GFX_print(gfx_p, "Play Game", 5, 3);
    GFX_print(gfx_p, "High Scores", 7, 3);
    GFX_print(gfx_p, "How to Play", 9, 3);
}

void App_proj2_handleMenuScreen(App_proj2* app_p, Game* game_p, HAL* hal_p)
{
    // Enum for menu options
    enum _menu_Option {PLAY,HIGH_SCORES,HOWTO};
    typedef enum _menu_Option menu_Option;
    static menu_Option option = PLAY;
    switch (option){
    case PLAY:
        GFX_print(&hal_p->gfxContext, ">", 5, 2);
        if (Joystick_isTappedDown(&hal_p->joystick)){
            clear_Option(hal_p,5);
            option = HIGH_SCORES;
            break;
        }
        if (Button_isTapped(&hal_p->boosterpackJS)){
            app_p->state = GAME_SCREEN;
            GFX_clear(&hal_p->gfxContext);
            break;
        }

        break;
    case HIGH_SCORES:
        GFX_print(&hal_p->gfxContext, ">", 7, 2);
        if (Joystick_isTappedUp(&hal_p->joystick)){
            clear_Option(hal_p,7);
            option = PLAY;
            break;
        }
        else if (Joystick_isTappedDown(&hal_p->joystick)){
            clear_Option(hal_p,7);
            option = HOWTO;
            break;
        }
        if (Button_isTapped(&hal_p->boosterpackJS)){
            app_p->state = HIGHSCORE_SCREEN;
            App_proj2_showHSScreen(app_p, game_p, &hal_p->gfxContext);
            break;
        }
        break;
    case HOWTO:
        GFX_print(&hal_p->gfxContext, ">", 9, 2);
        if (Joystick_isTappedUp(&hal_p->joystick)){
            clear_Option(hal_p,9);
            option = HIGH_SCORES;
            break;
        }
        if (Button_isTapped(&hal_p->boosterpackJS)){
            app_p->state = INSTRUCTIONS_SCREEN;
            App_proj2_showInstructionsScreen(app_p, &hal_p->gfxContext);
            break;
        }
        break;
    }
}

void clear_Option(HAL* hal_p, int row){
    GFX_setForeground(&hal_p->gfxContext, BG_COLOR);
    GFX_print(&hal_p->gfxContext, ">", row, 2);
    GFX_setForeground(&hal_p->gfxContext, FG_COLOR);

}

void App_proj2_showInstructionsScreen(App_proj2* app_p, GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);

    // Display the text
    GFX_print(gfx_p, "Instructions         ", 0, 0);
    GFX_print(gfx_p, "---------------------", 1, 0);

    GFX_print(gfx_p, "Enjoy 3 minigames!", 2, 0);
    GFX_print(gfx_p, "Each game starts with", 3, 0);
    GFX_print(gfx_p, "a short preview of", 4, 0);
    GFX_print(gfx_p, "what to do. Do what", 5, 0);
    GFX_print(gfx_p, "needs to be done and", 6, 0);
    GFX_print(gfx_p, "get score! Fail to do", 7, 0);
    GFX_print(gfx_p, "so and lose a life!", 8, 0);
    GFX_print(gfx_p, "Lose 3 and its gg!", 9, 0);
    GFX_print(gfx_p, "Try and get as much", 10, 0);
    GFX_print(gfx_p, "score as you can!", 11, 0);
    GFX_print(gfx_p, "(Press JS button", 14, 0);
    GFX_print(gfx_p, "to return)", 15, 0);
}


void App_proj2_handleInstructionsScreen(App_proj2* app_p, HAL* hal_p)
{
    if (Button_isTapped(&hal_p->boosterpackJS)){
        app_p->state = MENU_SCREEN;
        App_proj2_showMenuScreen(app_p, &hal_p->gfxContext);
    }
}

void App_proj2_showHSScreen(App_proj2* app_p, Game* game_p, GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);

    char hs1_char[10] = "";
    char hs2_char[10] = "";
    char hs3_char[10] = "";

    snprintf(hs1_char, 6, "%d",game_p->hs[0]); //Could not create macros for snprintf function, it resulted in errors
    snprintf(hs2_char, 6, "%d",game_p->hs[1]);
    snprintf(hs3_char, 6, "%d",game_p->hs[2]);


    // Display the text
    GFX_print(gfx_p, "High Score         ", 0, 0);
    GFX_print(gfx_p, "---------------------", 1, 0);

    GFX_print(gfx_p, "#1: ", 2, 0);
    GFX_print(gfx_p, hs1_char, 2, 4);
    GFX_print(gfx_p, "#2: ", 4, 0);
    GFX_print(gfx_p, hs2_char, 4, 4);
    GFX_print(gfx_p, "#3: ", 6, 0);
    GFX_print(gfx_p, hs3_char, 6, 4);
    GFX_print(gfx_p, "(Press JS button", 14, 0);
    GFX_print(gfx_p, "to return)", 15, 0);
}


void App_proj2_handleHSScreen(App_proj2* app_p, HAL* hal_p)
{
    if (Button_isTapped(&hal_p->boosterpackJS)){
        app_p->state = MENU_SCREEN;
        App_proj2_showMenuScreen(app_p, &hal_p->gfxContext);
    }
}

void App_proj2_showPreGameScreen(Game* game_p, GFX* gfx_p)
{
    char lives_char[] = "";
    char score_char[] = "";
    snprintf(lives_char, 2, "%d",game_p->lives); //Cannot use macros for snprintf
    snprintf(score_char, 7, "%d",game_p->score);
    // Display the text conditionally
    if (game_p->counter==0){
        GFX_print(gfx_p, "Press BB1 when the", 5, 2);
        GFX_print(gfx_p, "meter is full!", 6, 4);
    }
    else if (game_p->counter==1){
        GFX_print(gfx_p, "Move the JS in the", 5, 2);
        GFX_print(gfx_p, "direction indicated!", 6, 1);
    }
    else if (game_p->counter==2){
        GFX_print(gfx_p, "Press BB1 if the", 4, 2);
        GFX_print(gfx_p, "statement is true -", 5, 1);
        GFX_print(gfx_p, "Press BB1 if false!!", 7, 1);
    }

    //Always display lives and score
    GFX_print(gfx_p, "Lives:", 9, 6);
    GFX_print(gfx_p, lives_char, 9, 12);
    GFX_print(gfx_p, "Score:", 10, 6);
    GFX_print(gfx_p, score_char, 10, 12);

}


void App_proj2_handleGameScreen(App_proj2* app_p, Game* game_p, HAL* hal_p)
{
    switch (game_p->state){
    case RESET:
        if (game_p->lives==0){
            app_p->state=RESULT_SCREEN;
            App_proj2_showResultScreen(app_p, hal_p, game_p);
            break;
        }
        SWTimer_start(&game_p->pregame_timer);
        game_p->state = PREGAME;
        GFX_clear(&hal_p->gfxContext);
        break;
    case PREGAME:
        App_proj2_showPreGameScreen(game_p, &hal_p->gfxContext);
        if (SWTimer_expired(&game_p->pregame_timer))
        {
            //Switches state based on predefined order (MF->DO->CC->MF)
            GFX_clear(&hal_p->gfxContext);
            Game_transition(game_p);
            break;
        }
        break;
    case METER_FILLER:{
        static int sq[3] = {0,0,0};
        meter_filler(game_p, hal_p, sq);
        break;}
    case DIRECTION_OBEYER:{
        static int firstCall = 1; //Integer substitute for T/F
        static char direction[10] = "";
        static int num;
        direction_obeyer(game_p, hal_p, &firstCall, direction, &num);
        if (firstCall==1){break;}
        time_expired(game_p, hal_p, &firstCall);
        break;}
    case COLOR_CONFIRMER:{
        static int firstCall = 1;
        static int color;
        static int lie;
        if(firstCall){
            srand(Timer32_getValue(TIMER32_0_BASE)); //Seeds the random number generator with the current time (pure random)
            color = rand()%3; //Randomizer from 0-2
            srand(Timer32_getValue(TIMER32_0_BASE));
            lie = rand()%3; //Randomizer from 0-2
            color_confirmer(app_p, game_p, hal_p, color);
            print_color(hal_p, lie);
            firstCall = 0;
        }
        if (Button_isTapped(&hal_p->boosterpackS1)||Button_isTapped(&hal_p->boosterpackS2)){
            color_check(game_p, hal_p, color, lie);
            firstCall = 1;
            wait();
            break;
        }
        time_expired(game_p, hal_p, &firstCall);
        break;}
    }
}

void Game_transition(Game* game_p){
    if (game_p->counter==0){
        game_p->counter +=1;
        game_p->state = METER_FILLER;
        SWTimer_start(&game_p->one_timer);
    }
    else if (game_p->counter==1){
        game_p->counter +=1;
        game_p->state = DIRECTION_OBEYER;
        SWTimer_start(&game_p->game_timer);
    }
    else if (game_p->counter==2){
        game_p->counter = 0;
        game_p->state = COLOR_CONFIRMER;
        SWTimer_start(&game_p->game_timer);
    }
}

void direction_obeyer(Game* game_p, HAL* hal_p,int* firstCall,char direction[], int* num){
    if(*firstCall){
        srand(Timer32_getValue(TIMER32_0_BASE));
        int dir = rand()%4; //Randomizes from 0-3
        direction_selector(direction, dir, num);
        *firstCall = 0;
    }
    char str[50];
    snprintf(str,30,"Hold %s on JS!",direction);

    GFX_print(&hal_p->gfxContext, str, 3, 3);
    if (Joystick_isTappedUp(&hal_p->joystick)||Joystick_isTappedDown(&hal_p->joystick)||
            Joystick_isTappedToLeft(&hal_p->joystick)||Joystick_isTappedToRight(&hal_p->joystick)){
        if (Joystick_isTappedUp(&hal_p->joystick)&&(*num==1)){
            success(game_p, hal_p);
        }
        else if (Joystick_isTappedDown(&hal_p->joystick)&&(*num==0)){
            success(game_p, hal_p);
        }
        else if (Joystick_isTappedToRight(&hal_p->joystick)&&(*num==2)){
            success(game_p, hal_p);
        }
        else if (Joystick_isTappedToLeft(&hal_p->joystick)&&(*num==3)){
            success(game_p, hal_p);
        }
        else{
            failure(game_p, hal_p);
        }
        *firstCall = 1;
        wait();
    }
}

void color_confirmer(App_proj2* app_p, Game* game_p, HAL* hal_p, int color){

    GFX_print(&hal_p->gfxContext, "BB1:True - BB2:False", 3, 1);
    Graphics_Rectangle myRect = {56, 70, 72, 86};
    if (color==0){
        Graphics_setForegroundColor(&hal_p->gfxContext.context, GRAPHICS_COLOR_RED);
        Graphics_fillRectangle(&hal_p->gfxContext.context, &myRect);
    }
    else if (color==1){
        Graphics_setForegroundColor(&hal_p->gfxContext.context, GRAPHICS_COLOR_BLUE);
        Graphics_fillRectangle(&hal_p->gfxContext.context, &myRect);
    }
    else if (color==2){
        Graphics_setForegroundColor(&hal_p->gfxContext.context, GRAPHICS_COLOR_GREEN);
        Graphics_fillRectangle(&hal_p->gfxContext.context, &myRect);
    }
    Graphics_setForegroundColor(&hal_p->gfxContext.context, GRAPHICS_COLOR_WHITE);
}

void print_color(HAL* hal_p, int val){
    if (val==0){
        GFX_print(&hal_p->gfxContext, "RED Square", 7, 6);
    }
    else if (val==1){
        GFX_print(&hal_p->gfxContext, "BLUE Square", 7, 6);
    }
    else if (val==2){
        GFX_print(&hal_p->gfxContext, "GREEN Square", 7, 5);
    }
}

void wait(){
    int b = 0;
    SWTimer timer = SWTimer_construct(TEMPORARY_WAIT);
    SWTimer_start(&timer);
    while (b!=1){
        if(SWTimer_expired(&timer)){b=1;}
    }
}

void direction_selector(char direction[], int num_direction, int* num){
    char a[] = "Down\0";
    char b[] = "Up\0";
    char c[] = "Right\0";
    char d[] = "Left\0";
    if (num_direction == 0){
        snprintf(direction,5, "%s",a);
    }
    else if (num_direction == 1){
        snprintf(direction,3, "%s",b);
    }
    else if (num_direction == 2){
        snprintf(direction,6,"%s",c);
    }
    else if (num_direction == 3){
        snprintf(direction,5,"%s",d);
    }
    *num = num_direction;
}
void meter_filler(Game* game_p, HAL* hal_p, int sq[]){
    GFX_print(&hal_p->gfxContext, "Press BB1 at the top!", 0, 0);
    Graphics_drawLineH(&hal_p->gfxContext.context,54,74,40);
    Graphics_drawLineH(&hal_p->gfxContext.context,54,74,96);
    Graphics_drawLineV(&hal_p->gfxContext.context,54,40,96);
    Graphics_drawLineV(&hal_p->gfxContext.context,74,40,96);

    //Add foundation for drawing in a function
    if (SWTimer_expired(&game_p->one_timer)&& !sq[0]){
        //Draw 1 block 16x16 x1=
        Graphics_Rectangle myRect1 = {56, 78, 72, 94};
        Graphics_fillRectangle(&hal_p->gfxContext.context, &myRect1);
        SWTimer_start(&game_p->one_timer);
        sq[0] = 1;
    }
    else if (SWTimer_expired(&game_p->one_timer)&& !sq[1]){
        Graphics_Rectangle myRect2 = {56, 60, 72, 76};
        Graphics_fillRectangle(&hal_p->gfxContext.context, &myRect2);
        SWTimer_start(&game_p->one_timer);
        sq[1] = 1;
    }
    else if (SWTimer_expired(&game_p->one_timer)&& !sq[2]){
        //Draw 1 block 16x16 x1=
        Graphics_Rectangle myRect3 = {56, 42, 72, 58};
        Graphics_fillRectangle(&hal_p->gfxContext.context, &myRect3);
        SWTimer_start(&game_p->one_timer);
        sq[2] = 1;
    }

    if (Button_isTapped(&hal_p->boosterpackS1)&& !sq[2]){
        failure(game_p, hal_p);
        sq[0]=0;
        sq[1]=0;
        sq[2]=0;
        wait();
    }
    else if (Button_isTapped(&hal_p->boosterpackS1)&& !SWTimer_expired(&game_p->one_timer) && sq[2]){
        success(game_p, hal_p);
        sq[0]=0;
        sq[1]=0;
        sq[2]=0;
        wait();
    }
    else if (SWTimer_expired(&game_p->one_timer) && sq[2]){
        failure(game_p, hal_p);
        sq[0]=0;
        sq[1]=0;
        sq[2]=0;
        wait();
    }
}


void color_check(Game* game_p, HAL* hal_p, int color, int lie){
    if (Button_isTapped(&hal_p->boosterpackS1)){
        if (color == lie){
            success(game_p, hal_p);
        }
        else{
            failure(game_p, hal_p);
        }
    }
    else if (Button_isTapped(&hal_p->boosterpackS2)){
        if (color == lie){
            failure(game_p, hal_p);
        }
        else{
            success(game_p, hal_p);
        }
    }
}

void time_expired(Game* game_p, HAL* hal_p, int* firstCall){
    if (SWTimer_expired(&game_p->game_timer)){
        GFX_print(&hal_p->gfxContext, "TOO SLOW", 13, 7);
        game_p->lives -= 1;
        game_p->state = RESET;
        *firstCall = 1;
        wait();
    }
}

void App_proj2_handleResultScreen(App_proj2* app_p, HAL* hal_p, Game* game_p){
    update_HS(game_p);

    if (Button_isTapped(&hal_p->boosterpackJS)){
        reset_game(game_p);
        app_p->state = MENU_SCREEN;
        App_proj2_showMenuScreen(app_p, &hal_p->gfxContext);
    }
}

void App_proj2_showResultScreen(App_proj2* app_p, HAL* hal_p, Game* game_p){
    GFX_clear(&hal_p->gfxContext);

    GFX_print(&hal_p->gfxContext, "GAME OVER", 6, 6);

    char score_char[] = "";
    snprintf(score_char, 7, "%d",game_p->score);
    GFX_print(&hal_p->gfxContext, "Score: ", 9, 5);
    GFX_print(&hal_p->gfxContext, score_char, 9, 12);

    GFX_print(&hal_p->gfxContext, "(Press JS button", 14, 0);
    GFX_print(&hal_p->gfxContext, "to return)", 15, 0);
}

void update_HS(Game* game_p){
    int hs1 = game_p->hs[0];
    int hs2 = game_p->hs[1];
    int hs3 = game_p->hs[2];
    if (game_p->score > hs1){
        game_p->hs[2] = game_p->hs[1];
        game_p->hs[1] = game_p->hs[0];
        game_p->hs[0] = game_p->score;
    }
    else if (game_p->score > hs2 && game_p->score < hs1){
        game_p->hs[2] = game_p->hs[1];
        game_p->hs[1] = game_p->score;
    }
    else if (game_p->score > hs3 && game_p->score < hs2){
        game_p->hs[2] = game_p->score;
    }
}

void reset_game(Game* game_p){
    game_p->counter = 0;
    game_p->lives = NUMBER_OF_LIVES;
    game_p->score = 000000;
}

void success(Game* game_p, HAL* hal_p){
    GFX_print(&hal_p->gfxContext, "SUCCESS", 13, 7);
    game_p->score += SCORE_INCR;
    game_p->state = RESET;
}

void failure(Game* game_p, HAL* hal_p){
    GFX_print(&hal_p->gfxContext, "FAILURE", 13, 7);
    game_p->lives -= 1;
    game_p->state = RESET;
}
