#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_DIRECTION 'r'
#define COLUMN_DIRECTION 'c'

#define CONTAINER_TYPE 'c'
#define TEXT_TYPE 't'

int terminalWidth;
int terminalHeight;

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

void parseToTUI(struct containerElement* container) {
}

void getTerminalSize(int* width, int* height) {
    // get the terminal size using system calls or libraries 

}

int main() {
    getTerminalSize(&terminalWidth, &terminalHeight);

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
    parseToTUI(&container);
}


