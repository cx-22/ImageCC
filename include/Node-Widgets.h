#pragma once

#include "base-node.h"
#include "types-c.h"
#include <Qslider>

class InputNode : public BaseNode{
public:
    int type = INPUT;
    std::string title = "Input";

    QLabel* label;

    InputNode(){
        label = new QLabel();
        label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        QFont f = label->font();
        f.setBold(true);
        f.setItalic(true);
        label->setFont(f);
        label->setMinimumSize(200, 200);
        label->installEventFilter(this);
    }
};

class OutputNode : public BaseNode{
public:
    int type = OUTPUT;
    std::string title = "Output";

    QLabel* label;

    OutputNode(){
        label = new QLabel();
        label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        QFont f = label->font();
        f.setBold(true);
        f.setItalic(true);
        label->setFont(f);
        label->setMinimumSize(200, 200);
        label->installEventFilter(this);
    }
};


class AddNode : public BaseNode{
public:
    int type = 0;
    std::string title = "Add";

    QLabel* label;
    QSlider* slider;
    uchar addend;
    AddNode(){

    }

};
