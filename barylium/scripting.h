/*

  MANG Scripting Test code
  © Adam Wright 2000

  A highly simple scripting language.

  Has basic variable support, logical compares. Token delimitation performed by newline
  and whitespace

  Event/Trigger model. Psuedo functions are called when a target event occurs

*/



typedef struct Handler {
	int startLine;
	int endLine;
	int handler;
} HANDLER;

//Some engine types

#define SYSTEM 1
#define LEXICAL 2
#define PARSER 3

//Our biggest script (dynamic loading is sloooooooow, so we eat memory instead)

#define MAX_RAW_SCRIPT_SIZE 8096
#define MAX_SCRIPT_LINES 150
#define MAX_SCRIPT_LINE_LENGTH 512

//Handler element defines

#define PLAYER_CONNECT 1
#define MAP_BEGIN 2

int scriptingInit(char *file);


