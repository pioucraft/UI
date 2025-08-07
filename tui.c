#include <stdio.h>
#include <unistd.h>

int main() {
  // Move cursor to row 5, col 10
  printf("\033[3J\033[2J\033[H");
  printf("\033[5;10H");
  printf("Hello, TUI world!");

  // Flush stdout
  fflush(stdout);

  // Wait 2 seconds
  sleep(2);

  // Now redo it all again, but write Hello, World! instead
  printf("\033[3J\033[2J\033[H");
  printf("\033[5;10H"); // Move cursor to row 5, col 10
  printf("Hello, World!");

  fflush(stdout); // Flush stdout

  // Wait 2 seconds
  sleep(2);

  // Move cursor to row 5, col 10
  printf("\033[3J\033[2J\033[H");
  printf("\033[5;10H");
  printf("Hello, TUI world!");

  // Flush stdout
  fflush(stdout);

  // Wait 2 seconds
  sleep(2);

  // Move cursor to bottom
  printf("\033[20;0H");

  return 0;
}
