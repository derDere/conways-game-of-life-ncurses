# Conway's Game of Life 
A ncursesw version of Conway's Game of Life for your terminal.
![Conway's Game of Life Screenshot](https://raw.githubusercontent.com/derDere/conways-game-of-life-ncurses/main/screenshot.png)

## Instalation
Enter the folowing command into your terminal:
```sh
git clone https://github.com/derDere/conways-game-of-life-ncurses.git
cd conways-game-of-life-ncurses
sudo make install
cd ..
rm -rf conways-game-of-life-ncurses
```

## Usage
To start the game simply type the following command:
```sh
cgol
```

If your terminal has problems displaying the block characters you can start the game in ASCII-mode:
```sh
cgol -a
```

If your want to start with a clear screen (no "living" cells) your can use this command:
```sh
cgol -c
```

You can also combine them:
```sh
cgol -a -c
```

## Key bindings
While the game is running you can use the following keybindings:
 - p or SPACE to pause or unpause the game.
 - capital C (C+Shift) to clear the screen.
 - q to quit out of the application.
 - Click anyware on the screen using the left Mouse button to create a seed and keep the game alive.

## Uninstall
Run the following command:
```sh
sudo rm /usr/local/bin/cgol
```

Or if you still have the repository, you can use the following command in its folder:
```sh
sudo make uninstall
```
