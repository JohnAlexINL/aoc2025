/*
    The dial starts by pointing at 50.
    The actual password is the number of times the dial
    is left pointing at 0 after any rotation in the sequence.
*/

#include <stdlib.h>
#include <stdio.h>
#include "dat.h"

#ifdef DEBUG
#define log(...) printf(__VA_ARGS__)
#else
#define log(...) do {} while (0)
#endif


int dialPosition = 50;
int dialZeros = 0;

char * streamAdvance(char *input) {
    while (*input && *input !='\n') { input++; } input++;
    return input;
}

int dialTurn(int distance) {
    int step; if (distance < 0) { step = -1; } else { step = 1; }
    while( distance != 0 ) {
        dialPosition += step; 
            if ( dialPosition == 0 )    { dialZeros++; }
            if ( dialPosition == 100 )  { dialZeros++; dialPosition -= 100; }
            if ( dialPosition < 0 )     { dialPosition += 100; }
        distance -= step;
    }
    log(" { %2d } ", dialPosition);
    return dialPosition;
}

int dialParse(char *input) {
    int distance;
    switch(*input) {
        case 'L':   distance = -1; break;
        case 'R':   distance =  1; break;
        default:    log("ERR dialParse { char %d unexpected } %c%c%c%c ...\n", input[0], input[0], input[1], input[2], input[3]); exit(0); break;
    }
    distance *= atoi(input+1);
    log("M%-4d  ", distance);
    return distance;
}

int main () {
    printf("Processing data stream [%d bytes]\n", sizeof(data));
    int steps = 0; char *stream = data; 
    int cursor = 0;
    while (1) {
        cursor = (stream - data); if ( cursor >= sizeof(data)-1 ) { break; }
        log("%-4d    ", cursor);
        int distance = dialParse(stream);
        int position = dialTurn(distance);
        log("\n");
        steps++; stream = streamAdvance(stream);
    }
    printf("Result %d calculated in %d turns\n", dialZeros, steps);
}