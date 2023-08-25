#include <iostream>
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define APP "Game-of-Life"

using namespace std;

void quit();

/**
 * Project: Game-of-Life
 * Creator: deremer
 * Creation Date: Thu Sep  8 16:25:15 CEST 2022
 */
int main(int argc, char* argv[]) {
  bool asciiMode = false;
  bool clearStart = false;
  bool screenSaverMode = false;

  for(int i = 0; i < argc; i++) {
    string arg(argv[i]);
    if (arg == "-a") {
      asciiMode = true;
    }
    else if (arg == "-c") {
      clearStart = true;
    }
    else if (arg == "-s") {
      screenSaverMode = true;
    }
    else if (
      (arg == "-?") ||
      (arg == "/?") ||
      (arg == "help") ||
      (arg == "--help")
    ) {
      cout << "Conway's Game of Life" << endl
           << "---------------------" << endl
           << "Startup commands:" << endl
           << "  -c   Clear start" << endl
           << "  -a   Ascii Mode" << endl
           << "  -s   Screensaver Mode" << endl
           << endl
           << "Key bindings:" << endl
           << "  p          Pause / Unpause" << endl
           << "  SPACE      Pause / Unpause" << endl
           << "  C          Clear screen" << endl
           << "  q          Quit" << endl
           << "  MOUSE_1    Place seed" << endl;
      return 0;
    }
  }

  // Init Curses ----------
  setlocale(LC_ALL, "");
  WINDOW* win = initscr();
  atexit(quit);
  curs_set(0);
  start_color();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, true);
  mousemask(BUTTON1_CLICKED, NULL); //ALL_MOUSE_EVENTS, NULL);

  // Draw Start Screen
  if (!screenSaverMode) {
    attron(A_BOLD | A_UNDERLINE);
    mvaddstr( 3, 5, "Conway's Game of Life");
    attroff(A_UNDERLINE);
    mvaddstr( 5, 7, "Key bindings:");
    attroff(A_BOLD);
    mvaddstr( 6, 9, "p         Pause / Unpause");
    mvaddstr( 7, 9, "SPACE     Pause / Unpause");
    mvaddstr( 8, 9, "C         Clear screen");
    mvaddstr( 9, 9, "q         Quit");
    mvaddstr(10, 9, "MOUSE_1   Place seed");
    attron(A_BOLD);
    mvaddstr(12, 7, "Press any key to start...");
    attroff(A_BOLD);
    getch();
  }
  nodelay(win, true);

  // Creating Grid
  int w, h;
  getmaxyx(stdscr, h, w);
  w = w - 1;
  h = (h - 1) * 2;

  bool m1[w][h];
  bool m2[w][h];

  for (int y = 0; y < h; y++) {
    for(int x = 0; x < w; x++) {
      bool val = clearStart ? 0 : (rand() % 2);
      m1[x][y] = val;
      m2[x][y] = val;
    }
  }

  int key = '\0';
  bool paused = false;
  int neighbors[8][2] = {
    {-1, -1},
    { 0, -1},
    { 1, -1},
    {-1,  0},
    { 1,  0},
    {-1,  1},
    { 0,  1},
    { 1,  1}
  };

  while (key != 'q') {
    // Drawing Grid
    move(0, 0);
    for (int y = 0; y < h; y+=2) {
      for (int x = 0; x < w; x++) {
        bool top = m1[x][y];
        bool bot = m1[x][y+1];
        if (bot && top) {
          printw(asciiMode?":":"█");
        }
        else if (bot) {
           printw(asciiMode?".":"▄");
        }
        else if (top) {
          printw(asciiMode?"'":"▀");
        }
        else {
          printw(" ");
        }
      }
      printw("\n");
    }
    refresh();
    usleep(75000);

    if (!paused) {
      // Calculation evolution
      copy(&m1[0][0], &m1[0][0] + (w * h), &m2[0][0]);
      for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
          int lifeCount = 0;
          for (int n = 0; n < 8; n++) {
            int nx = x + neighbors[n][0];
            int ny = y + neighbors[n][1];
            if (nx < 0) nx += w;
            if (ny < 0) ny += h;
            if (nx >= w) nx %= w;
            if (ny >= h) ny %= h;
            if (m1[nx][ny]) lifeCount++;
          }
          if (m1[x][y] && (lifeCount < 2 || lifeCount > 3)) {
            m2[x][y] = false;
          } else if (!m1[x][y] && lifeCount == 3) {
            m2[x][y] = true;
          }
        }
      }
      copy(&m2[0][0], &m2[0][0] + (w * h), &m1[0][0]);
    }

    // getting user input
    key = getch();

    if (screenSaverMode && key != ERR) {
    	key = 'q';
    }

    // Check user input
    if (key == KEY_MOUSE) {
      MEVENT event;
      if (getmouse(&event) == OK) {
        if (
	        (event.bstate & BUTTON1_CLICKED) &&
	        (event.x >= 0) &&
	        (event.x < w) &&
	        (event.y >= 0) &&
	        (event.y < (h / 2))
	      ) {
	        int x = event.x;
	        int y = event.y;
          y *= 2;
          int seed[5][2] = {
            { 1, -1},
            { 0, -1},
            { 0,  0},
            {-1,  0},
            { 0,  1}
          };
          for (int n = 0; n < 5; n++) {
            int nx = x + seed[n][0];
            int ny = y + seed[n][1];
            m1[nx][ny] = true;
          }
	      }
      }
    } else if (key == 'p' || key == ' ') {
      paused = !paused;
    } else if (key == 'C') {
      for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
          m1[x][y] = false;
        }
      }
    }
  }

  return 0;
}

void quit() {
  endwin();
}
