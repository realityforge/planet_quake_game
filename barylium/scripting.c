/*

  MANG Scripting Test code
  © Adam Wright 2000

  A highly simple scripting language. 
  
  Has basic variable support, logical compares. Token delimitation performed by newline
  and whitespace

  Event/Trigger model. Psedo functions are called when a target event occurs

  This is NOWHERE NEAR finished, I'm working on it when I have time (experiementing with 
  about 100 diffrent approaches)

*/


#include "g_local.h"


char rawscript[MAX_RAW_SCRIPT_SIZE];
char script[MAX_SCRIPT_LINES][MAX_SCRIPT_LINE_LENGTH];
int scriptlines;

int handlerentrypoints[50];
char *varstack[100];
int stackfree;

//stuff for the tokeniser to share

char token[MAX_SCRIPT_LINE_LENGTH];
int lastoffset;
int tokenisingline;

int varStackPush(char *token) {
	if(stackfree>100)
		return 0;
	varstack[stackfree] = token;
	stackfree++;
	return 1;
}

char *varStackPop() {
	stackfree--;
	return varstack[stackfree+1];
}

void scriptMsg(int type, char *error, ...) {
	va_list	argptr;
	static char	string[2][32000];
	static int	index = 0;
	char *buf;

	buf = string[index & 1];
	index++;

	va_start(argptr, error);
	vsprintf(buf, error, argptr);
	va_end(argptr);

	switch(type) {
	case SYSTEM:
		Com_Printf("Be_MANG (System) : %s\n", buf);
		break;
	case LEXICAL:
		Com_Printf("Be_MANG (Lexical) : %s\n", buf);
		break;
	case PARSER:
		Com_Printf("Be_MANG (Parser) : %s\n", buf);
		break;
	};
}

//Returns true if the token resolves to just numbers
int isTokenInt(char *token) {
	int total, counter = 0;

	total = strlen(token);
	for(counter; counter<total; counter++) {
		if(token[counter] < '0' || token[counter] > '9')
			return 0;
	}
	return 1;
}


//Provides continuity
int isTokenMixed(char *token) {
  return !isTokenInt(token);
}

int getCleanLines() {
	int bytesfilled = 0;
	int counter = 0;
	char lastchar = '\0';
	int offset = 0;
	int line = -1;
	int charwritten = 0;
	int length;

	length = strlen(rawscript);
	scriptMsg(LEXICAL, "Attempting to parse script (%i bytes)", length);
	while(offset<length) {

		scriptMsg(LEXICAL, "Line %i found at offset %i", line+1, offset);
		charwritten = 0;
		lastchar = '\0';
		counter = 0;
		line++;

		if(line>MAX_SCRIPT_LINES)
			return 0;

		while(1) {

			if(offset>=length) {
				line--;
				break;
			}

			if(counter>MAX_SCRIPT_LINE_LENGTH)
				return 0;

			if(rawscript[offset] == '\r' || rawscript[offset] == '\n') {
				if(charwritten==0)
					line--;
				offset++;
				if(rawscript[offset] == '\n' || rawscript[offset] == '\r')
					offset++;
				break;
			}

			//Too much whitespace?
			if(rawscript[offset]==' ') {
				if(lastchar==' ' || charwritten==0) {
					offset++;
					continue;
				}
			}

			//Comment?
			if(rawscript[offset] == '/' && rawscript[offset+1] == '/') {
				scriptMsg(LEXICAL, "Comment line found, active line negated");
				while(rawscript[offset]!='\r' && rawscript[offset]!='\n')
					offset++;
				offset++;
				if(rawscript[offset]=='\r' || rawscript[offset]=='\n')
					offset++;
				line--;
				break;
			}

			//Byte!
			script[line][counter] = rawscript[offset];
			charwritten = 1;
			lastchar = rawscript[offset];
			counter++;
			offset++;
			bytesfilled++;
		}		
	}
	
	//line+1 because our arrays are 0 based, but it doesnt make sense to say line 0
	scriptMsg(LEXICAL, "Parsed %i lines with %i bytes of token data", line+1, bytesfilled);
	scriptlines = line;
	return bytesfilled;
}

int constructData(char *filename) {
	int len;
	fileHandle_t file;
	scriptMsg(SYSTEM, "Attempting to open %s", filename);

	len = trap_FS_FOpenFile( filename, &file, FS_READ );

	if (!file) {
		scriptMsg(SYSTEM, "Failed to read file %s", filename);
		return 0;
	}

	if(len > MAX_RAW_SCRIPT_SIZE) {
		scriptMsg(SYSTEM, "Script to large %s", filename);
		trap_FS_FCloseFile(file);
		return 0;
	}

	trap_FS_Read(rawscript, len, file);
	rawscript[len] = 0;
	trap_FS_FCloseFile(file);

	return getCleanLines();
}

//uses these globals to tokenise a line. 
//char token[512];
//int lastoffset;
//int tokenisingline;

int tokenise(int line) {
	char linecopy[MAX_SCRIPT_LINE_LENGTH];
	//char temptoken[MAX_SCRIPT_LINE_LENGTH];

	if(line!=tokenisingline) {
		tokenisingline = line;
		lastoffset = 0;
	}

	//strcpy(linecopy, scriptlines[line]);

	while(1) {

		if(linecopy[lastoffset]==' ') {
			lastoffset++;
			continue;
		}
		
		if(linecopy[lastoffset]=='/0')
			break;

	}

	return 1;
}

int parse(int entrypoint) {

	return 0;
}

int scriptingInit(char *file) {
	int counter = 0;

	scriptMsg(SYSTEM, "Init of MANG Engine underway");
	if(!constructData(file)) {
		scriptMsg(SYSTEM, "Parsing Failed!");
		return 0;
	}
	scriptMsg(SYSTEM, "Parsing Succeeded!");
	//Lets loop the script and see what we got
	
	for(counter = 0; counter < scriptlines+1; counter++)
		scriptMsg(PARSER, "Line %i : %s", counter+1, script[counter]);
	return 1;
}
