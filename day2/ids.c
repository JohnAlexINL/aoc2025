/*
    Iterate a file for a list of ranges of integers to check;
    For each stringwise representation of any integer,
    signal if the string matches a "%s%s" pattern (ex., "22", "1010", "123123")
    None have leading zeroes

    The input stream contains lists in the format "%d-%d," without newlines or extra char,
    but the examples file does have newlines.
    To be clean, we'll just skip newlines, carriage returns, and spaces.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "dat.h"

#ifdef DEBUG
#define log(...) printf(__VA_ARGS__)
#else
#define log(...) do {} while (0)
#endif

#define uint64_t unsigned long long

typedef struct { uint64_t start; uint64_t end; char *term; } range_t;

range_t getRange (char *stream) {
    char srepr[32]; char erepr[32];
    while(*stream == ',') { stream++; }
    char * head = stream;
    char * toe  = head; while ( *toe != '-' ) { toe++; } toe++; // *toe = '\0';
    char * term = toe; while  ( *term != 0 && *term != ',' ) { term++; } *term = '\0';
    uint64_t start = atoll(head);
    uint64_t end   = atoll(toe);
    log ("{ %-10llu to %-10llu [%-8d wide] }  ", start, end, end-start);
    if ( start < end ) { return (range_t) { start, end, term }; }
    return (range_t) { end, start, term };
}

bool flag ( uint64_t id ) {
    char repr[32]; sprintf(repr, "%d", id); int len = strlen(repr);
    if (len % 2 != 0 ) { return false; } int half = len/2;
    char * head = &repr[0]; char * toe = &repr[half];
    // int result = strncmp(head, toe, half);
    // return result == 0;
    int i; for(i=0;i<half;i++) { if (head[i] != toe[i]) { return false; } }
    return true;
}

int main () {
    printf("** DAY 2 ** Processing data stream [%-8d bytes]; ", sizeof(data));

    uint64_t invalidsum = 0; int invalidcount = 0;
    int steps=0; int iter=0; int cursor = 0; char *stream = data; 
    while(1) { steps++; cursor = ( stream - data );
        uint64_t i;
        if ( cursor >= sizeof(data) ) { break; }
        if ( steps > sizeof(data)/4 ) { printf("ERR steps exceeded maximum expected\n"); exit(-1); }
            log ("\n    %-4d    ", cursor);
        
        for(i=0;i<16;i++) { if (stream[i] && stream[i] != '\n') { log("%c", stream[i]); } else { log(" "); } } log("   --   ");
        range_t this = getRange(stream); // log("[RAW \"%.*s\"] start=%lu end=%lu  ", (int)(this.term-stream), stream, this.start, this.end);
        uint64_t thiscount = 0; uint64_t thissum = 0;
        for(i=this.start;i<=this.end;i++) {
            iter++; bool fail = flag(i); 
            if (fail == true) { thiscount++; thissum += i; }
        }   stream = this.term +1;
        
        log("  =>  %-2d fails, sum %d ", thiscount, thissum); 
        invalidsum += thissum; invalidcount += thiscount;
    }   log("\n");
    printf("Result %lu calculated in %d turns from %d ranges\n", invalidsum, iter, steps);
}