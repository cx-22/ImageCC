#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "nodes.h"

void start(){
    buildNodeList();
    buildFuncs();
}

void end(){
    killList();
    killFuncs();
}
