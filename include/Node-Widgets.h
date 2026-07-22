#pragma once

#include "base-node.h"
#include "types-c.h"
#include <Qslider>

class InputNode : public BaseNode{
public:
    InputNode(){
        type = INPUT;
        title = "Input";
    }
};

class OutputNode : public BaseNode{
public:
    OutputNode(){
        type = OUTPUT;
        title = "Output";
    }
};


class AddNode : public BaseNode{
public:
    QSlider* slider;
    uchar addend;
    AddNode(){
        type = 0;
        title = "Add";
        addend = 100;
        //node->params[0] = static_cast<void*>(&addend);
    }
};
