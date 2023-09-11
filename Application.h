/*
 * App_proj2.h
 *
 *  Created on: March 3, 2023
 *      Author: Sanjiv Rao
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  3000  // 3 seconds
#define PREGAME_SCREEN_WAIT  2000 // 2 seconds
#define GAME_SCREEN_WAIT  4000 // 1 seconds
#define TEMPORARY_WAIT 1000

#define NUMBER_OF_LIVES 3;
#define STRLEN 10;
#define SCORE_INCR 1000;

enum _ScreenState
{
    TITLE_SCREEN, MENU_SCREEN, INSTRUCTIONS_SCREEN, PREGAME_SCREEN, GAME_SCREEN, RESULT_SCREEN, HIGHSCORE_SCREEN
};
typedef enum _ScreenState ScreenState;


enum _GameState
{
    RESET, PREGAME, METER_FILLER, DIRECTION_OBEYER, COLOR_CONFIRMER
};
typedef enum _GameState GameState;

/**
 * The top-level Application object, initialized in the main() entry point and
 * passed around to basically every other function. Holds state variables
 * which YOU define as part of your application's state.
 */
struct _App_proj2
{
    ScreenState state;  // Determines which screen is currently shown
    SWTimer timer;    // General-purpose timer for when screens must disappear

};
typedef struct _App_proj2 App_proj2;

struct _Game
{
    GameState state;  // Determines which game is currently shown
    SWTimer pregame_timer;    // General-purpose timer for when screens must disappear
    SWTimer game_timer;
    SWTimer one_timer;
    int lives;
    int score;
    int counter; // Simple counter that assists the program in determining which game to present
    int hs[3];

};
typedef struct _Game Game;




// Boilerplate constructor function for the Application
App_proj2 App_proj2_construct(HAL* hal_p);
Game Game_construct(HAL* hal_p);

void App_proj2_loop(App_proj2* app_p, Game* game_p, HAL* hal_p);

// Handle callback functions used for each state of the App_proj2_loop()
// function. State transitions are embedded inside of each of these functions.
void App_proj2_handleTitleScreen(App_proj2* app, HAL* hal);
void App_proj2_handleMenuScreen(App_proj2* app_p, Game* game_p, HAL* hal_p);
void App_proj2_handleInstructionsScreen(App_proj2* app, HAL* hal_p);
void App_proj2_handleHSScreen(App_proj2* app_p, HAL* hal_p);
void App_proj2_handleGameScreen(App_proj2* app_p, Game* game_p, HAL* hal_p);
void App_proj2_handleResultScreen(App_proj2* app, HAL* hal_p, Game* game_p);

// Helper functions which clear the screen and draw the text for each state
void App_proj2_showTitleScreen(GFX* gfxContext_p);
void App_proj2_showMenuScreen(App_proj2* app_p, GFX* gfxContext_p);
void App_proj2_showInstructionsScreen(App_proj2* app_p, GFX* gfxContext_p);
void App_proj2_showHSScreen(App_proj2* app_p, Game* game_p, GFX* gfx_p);
void App_proj2_showGameScreen(App_proj2* app_p, GFX* gfxContext_p, int lives, int score);
void App_proj2_showResultScreen(App_proj2* app_p, HAL* hal_p, Game* game_p);

// Helper function which helps work with the cursor
void clear_Option(HAL* hal_p, int row);

// Transition function which helps the FSM decide which state to go to based on game order
// Also restarts the game timer whenever a new game is opened
void Game_transition(Game* game_p);

// Helper function for the meter filler game
void meter_filler(Game* game_p, HAL* hal_p, int sq[]);


// Helper functions for the color confirmer game
void color_check(Game* game_p, HAL* hal_p, int color, int lie);
void color_confirmer(App_proj2* app_p, Game* game_p, HAL* hal_p, int color);
void print_color(HAL* hal_p, int val);

// Helper function for the direction obeyer game
void direction_selector(char direction[], int num_direction, int* num);
void direction_obeyer(Game* game_p, HAL* hal_p,int* firstCall,char direction[], int* num);

// Function to check if game time has expired
void time_expired(Game* game_p, HAL* hal_p, int* firstCall);

// Functions to update the high scores after an ended game and reset the game
void reset_game(Game* game_p);
void update_HS(Game* game_p);

// Simple functions that condense success and failure operations
void success(Game* game_p, HAL* hal_p);
void failure(Game* game_p, HAL* hal_p);

// Simple wait function to wait after displaying the result of a game
void wait();

#endif /* APPLICATION_H_ */
