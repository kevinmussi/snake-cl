# Snake
This repository contains a C implementation of the classic Snake game, which exploits the `ncurses` library to build a nice text interface in the Terminal.

## How to compile
If you're using a **Unix** system (MacOS or Linux), just use the Terminal to go in the folder containing the `main.c` source file and the `Makefile` and type the command 
```
make
```
This should compile the C program into an executable called `snake`. If there's compilation problems, it's probably due to the file `ncurses.h` missing: if that's the case, just download it and include it in your build path.
If you're using **Windows**, I already compiled the program and included the executable in the appropriate folder.

## How to lanuch
To launch the program, just go in the folder containing the `snake` executable and type
```
./snake
```
to play the game with the default settings. You can change the game mode (hard mode, screen wrap mode and "no generated obstacles" mode) by writing the appropriate flags after the executable name. To know these flags, type
```
./snake --help
```
to print the help message. Note that you can write more flags when launching the game, hence you can have more game modes at the same time.
Also note that if you're using **Windows**, the name of the executable is `snake.exe` and you must have the `libncursesw6.dll` file in the same folder as the executable (the file is included in the "windows" folder).

## How to play
When playing, you can move the snake in the four cardinal directions using the directional arrows or using the *WASD* buttons. You can quit at any time pressing the *q* button, and you can pause/unpause the game pressing the *spacebar* button. At the end of each game (either after losing or quitting) your score is saved in a log file called `snake_log.txt` (you can freely delete the file).
