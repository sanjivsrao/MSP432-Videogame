# MSP432-Videogame
The purpose of this project was to develop a set of minigames to be played on the Texas Instrument’s MSP432P401R Launchpad microcontroller. This was done in order to garner knowledge about interfacing with the microcontroller board through GPIO, timers, ADC with the joystick, and working with the LCD screen.

Project Description
    The application I have generated establishes an interface by which one can play a set of
replay able minigames that are dependent on time, user input, and visual cues. There is a title
screen built in, alongside a menu that allows users to navigate through the game. The primary
functionality is that of 3 minigames, called Meter Filler, Direction Obeyer, and Color Confirmer.
Before each game, instructions on how to play the game are displayed in a 2 second pre-game
screen. Success in any of these games awards score and allows one to progress through the game.
The games all have a common lives counter, which starts at 3 and decrements every time the user
fails a mini game. After all lives are lost, the game is over, and the user is displayed the menu
options again. These options include an option to play the game, view the instructions, and view
the recent past 3 high scores.
    The first game consists of a rectangular meter, filling in three stages over the course of 4
seconds. After the bar has been filled, users are expected to input a button press through the
launchpad peripherals to succeed in the game. Not pressing the button or failing to do so in the
allocated time will result in a failure. After this minigame, the application automatically
transitions to the Direction Obeyer minigame. In this feature, a string is displayed which instructs
the user to press the joystick in the direction they are instructed to. If done, the player succeeds.
If they fail to input the correct direction in time or simply input the wrong direction, the player
fails and loses a life. Following Direction Obeyer, the Color Confirmer game is displayed. In this
game, a square of a specific color and a statement about the color of the square is displayed. If
the statement about the square is true, then the user is instructed to press button 1 on the booster
pack. If the statement about the square is false, the then the user will be instructed to press button
2. The color and statement will be purely random, so the user will have to decide the correct
input. Succeeding or failing in this game follow similar conditions as the previous games. Upon
completing this game, the application will transition to the first game and the cycle continues
until the player runs out of lives. This game fulfills all expectations of the project description
aside from displaying the lives and score during the nano games themselves.

Example Progression of Game:
Instructions Screen
Menu Screen
High Score Screen (no
games played)
Title Screen
Pre-Game (Meter Filler)
Meter Filler (Bar not
pressed in time)
Meter Filler (Bar is
growing)
Meter Filler (Bar filled
but timer not expired)
Pre-Game (Direction
Obeyer)
Direction Obeyer
(Success state)
Pre-Game (Color
Confirmer)
Color Confirmer
(Success State)


Code Quality
In accordance with the requirements defined for this project, the code was designed to be
modular and easily read. All function names are based on their functionality and variables were
declared with understandable names. Comments were added in “application.h” to explain the
purpose and functionality for each of the helper functions. Similar operations that occurred often
were condensed into functions that could be called when needed. Any variables that were
necessary locally were created with static declarations. Variables that were used by many parts
of the application, such as lives and score, were condensed within another struct. No global
variables were utilized aside from some macros to define commonly used numbers. Hardcoding
was avoided as much as possible, yet some functions such as snprintf would not accept macro-
based parameters. Alongside this, all functions were shortened so they were not 60 lines or
longer. All code was designed to be non-blocking
