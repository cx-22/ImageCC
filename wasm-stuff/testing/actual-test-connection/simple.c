#include <emscripten.h>

void buildNodeList(){
    EM_ASM({
        console.log('built list');
    });
}
void killList(){
    EM_ASM({
        console.log('killed list');
    });
}

EMSCRIPTEN_KEEPALIVE
void* my_alloc(int size) {
    return malloc(size);
}

void start(){
    EM_ASM({
        console.log('starting');
    });
    buildNodeList();
}

EMSCRIPTEN_KEEPALIVE
void end(){
    EM_ASM({
        console.log('ending');
    });
    killList();
}

EMSCRIPTEN_KEEPALIVE
void createNode(int nid, int type){
    EM_ASM({
        console.log('made node $0 of typ $1');
    }nid, type);
}


EMSCRIPTEN_KEEPALIVE
void connectNodes(int input_nid, int output_nid, int index_start, int index_end){
    EM_ASM({
        console.log('connected nodes $0 and $1');
    }input_nid, output_nid);
}

/*
EMSCRIPTEN_KEEPALIVE
void disconnectNodes(int input_nid, int output_nid, int index_start, int index_end){

}


EMSCRIPTEN_KEEPALIVE
void addParam(int nid, int arg){

}


EMSCRIPTEN_KEEPALIVE
void setImageInput(int nid){

}


EMSCRIPTEN_KEEPALIVE
void killNode(int nid){

}


EMSCRIPTEN_KEEPALIVE
void clearNodes(){

}


EMSCRIPTEN_KEEPALIVE
void killList(){

}
*/
