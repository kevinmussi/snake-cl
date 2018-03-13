# Snake
This repository contains a C implementation of the classic Snake game, which exploits the `ncurses` library to build a graphical interface in the Terminal.

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
./snake <nickname>
```
where `<nickname>` it's a name of your choice, just used to keep track of the scores obtained when playing the game. Note that if you're using **Windows**, the name of the executable is `snake.exe` and you must have the `libncursesw6.dll` file in the same folder as the executable (the file is included in the "windows" folder).

## How to play
When playing, you can move the snake in the four cardinal directions using the directional arrows or using the *WASD* buttons. You can quit at any time pressing the *q* button, and you can pause/unpause the game pressing the *spacebar* button. At the end of each game (either after losing or quitting) your score is saved in a log file called `snake_log.txt` (you can freely delete the file).
