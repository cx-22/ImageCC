#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "nodes.h"
#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE
void start(){
    buildNodeList();
    buildFuncs();
}

EMSCRIPTEN_KEEPALIVE
void end(){
    killList();
    killFuncs();
}