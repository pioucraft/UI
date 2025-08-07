#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define ROW_DIRECTION 'r'
#define COLUMN_DIRECTION 'c'

#define CONTAINER_TYPE 'c'
#define TEXT_TYPE 't'

struct winsize terminalSize;
// idea : instad of a double pointer, use a triple pointer (row, column, symbol (that can take one or multiple characters... hehe))
char** TUIString;

struct element {
    char type; // "c" for container, "t" for text
    void* data;
};

struct containerElement {
    void** elements; // pointer to an array of elements (its children)
    int numElements; // number of elements in the array
    char direction; // "r" for row, "c" for column
};

struct textElement {
    char* text;
};

void parseToHTML(struct containerElement* container, char** html) {
    if (container->direction == ROW_DIRECTION) {
        strcat(*html, "<div style='display: flex; flex-direction: row;'>");
    } else if (container->direction == COLUMN_DIRECTION) {
        strcat(*html, "<div style='display: flex; flex-direction: column;'>");
    }
    for (int i = 0; i < container->numElements; i++) {
        struct element* elem = (struct element*)container->elements[i];
        if(elem->type == CONTAINER_TYPE) {
            struct containerElement* childContainer = (struct containerElement*)elem->data;
            parseToHTML(childContainer, html);
        } else if (elem->type == TEXT_TYPE) {
            struct textElement* textElem = (struct textElement*)elem->data;
            strcat(*html, "<div>");
            strcat(*html, textElem->text);
            strcat(*html, "</div>");
        }

    }
    strcat(*html, "</div>");
}

void parseToTUI(struct containerElement* container, int currentAvailableWidth, int startRow, int startColumn) {
    int currentColumn = startColumn;

    if (container->direction == COLUMN_DIRECTION) {
        for (int i = 0; i < container->numElements; i++) {
            struct element* elem = (struct element*)container->elements[i];
            if(elem->type == CONTAINER_TYPE) {
                struct containerElement* childContainer = (struct containerElement*)elem->data;
                parseToTUI(childContainer, currentAvailableWidth, startRow, currentColumn);
            } else if (elem->type == TEXT_TYPE) {
                struct textElement* textElem = (struct textElement*)elem->data;
                for (int j = 0; j < strlen(textElem->text); j++) {

                }
            }
        }
    }
}

void getTerminalSize() {
    if (ioctl(0, TIOCGWINSZ, &terminalSize) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }
}

void resetTUIString() {
    TUIString = malloc(sizeof(char*) * terminalSize.ws_row);
    for (int i = 0; i < terminalSize.ws_row; i++) {
        TUIString[i] = malloc(terminalSize.ws_col + 1);
        memset(TUIString[i], ' ', terminalSize.ws_col);
        TUIString[i][terminalSize.ws_col] = '\0'; // Null-terminate
    }
}

void printTUIString() {
    printf("\033[3J");
    printf("\033[0;0H");
    for (int i = 0; i < terminalSize.ws_row; i++) {
        printf("%s", TUIString[i]);
    }
    printf("\e[?25l");
    fflush(stdout);
}

int main() {
    getTerminalSize();
    resetTUIString();
    printf("\e[?25h");

    struct textElement text1 = {"Hello, World!"};
    struct element elem1 = {TEXT_TYPE, &text1};

    struct textElement text2 = {"This is a test."};
    struct element elem2 = {TEXT_TYPE, &text2};

    void** elements = malloc(sizeof(void*) * 2);

    elements[0] = &elem1;
    elements[1] = &elem2;

    struct containerElement container = {
        elements,
        2,
        COLUMN_DIRECTION 
    };

    char* html = malloc(1024);
    int currentAvailableWidth = terminalSize.ws_col;
    parseToTUI(&container, currentAvailableWidth, 0, 0);
    sleep(2);
}


