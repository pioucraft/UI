#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_DIRECTION 'r'
#define COLUMN_DIRECTION 'c'

#define CONTAINER_TYPE 'c'
#define TEXT_TYPE 't'

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
    printf("%s\n", *html);
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

int main() {
    // test the parseToHTML function with a simple container that contains one text element
    struct textElement text1 = {"Hello, World!"};
    void** elements = malloc(sizeof(void*) * 1);
    struct element elem1 = {TEXT_TYPE, &text1};
    elements[0] = &elem1;
    struct containerElement container = {
        elements,
        1,
        ROW_DIRECTION
    };
    char* html = malloc(1024);
    parseToHTML(&container, &html);
    printf("%s\n", html);
}


