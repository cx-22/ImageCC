#pragma once

#include "base-node.h"
#include "types.h"
#include <Qslider>
#include <QComboBox>

class InputNode : public BaseNode{
public:
    InputNode(){
        type = INPUT;
        title = "Input";
        initialize();
    }
};

class BMathNode : public BaseNode{
public:
    uchar mode;
    uchar val;
    float factor;
    QComboBox* combo;
    QSlider* slider;
    BMathNode(){
        type = 1;
        title = "Arithmetic";

        val = 100;
		factor = 2.0f;

        combo = new QComboBox();
        combo->addItem("Addition");
        combo->addItem("Subtraction");
        combo->addItem("Factor");
		combo->setCurrentIndex(0);

        slider = new QSlider(Qt::Horizontal);
        slider->setRange(0, 255);
        slider->setValue(100);
        slider->connect(slider, &QSlider::valueChanged, [this](int value) {
            if (mode == 2) {
                factor = static_cast<float>(value) / 100.0f;
            }
            else {
                val = static_cast<uchar>(value);
			}

            params[1] = (mode == 2) ? static_cast<void*>(&factor) : static_cast<void*>(&val);
            update();
            });

        combo->connect(combo, &QComboBox::currentIndexChanged, [this](int index) {
            mode = static_cast<uchar>(index);
            params[0] = static_cast<void*>(&mode);

            if (mode == 2) {
                slider->setRange(1, 400);
                slider->setValue(factor * 100);
                params[1] = static_cast<void*>(&factor);
            }
            else {
                slider->setRange(0, 255);
                slider->setValue(val);
                params[1] = static_cast<void*>(&val);
            }

            update();
            });
        main_layout->addWidget(combo);
        main_layout->addWidget(slider);
        initialize();
    }
    ~BMathNode(){
        delete slider;
        delete combo;
	}
};

class BlendNode : public BaseNode {
public:
    QSlider* slider;
    float val;
    BlendNode() {
        type = 2;
        title = "Blend";
        slider = new QSlider(Qt::Horizontal);
        slider->setRange(0, 100);
        slider->setValue(50);
        slider->connect(slider, &QSlider::valueChanged, [this](int value) {
            val = static_cast<float>(value) / 100.0f;
			params[0] = static_cast<void*>(&val);
            update();
            });
        main_layout->addWidget(slider);
		initialize();
    }
    ~BlendNode() {
        delete slider;
    }
};


class RGBSplitNode : public BaseNode {
public:
    RGBSplitNode() {
        type = 3;
        title = "RGB Split";
        initialize();
    }
};


class QuantizeNode : public BaseNode {
public:
    QSlider* slider;
    uchar val;
    QuantizeNode() {
        type = 4;
        title = "Quantize RGB";
        slider = new QSlider(Qt::Horizontal);
        slider->setRange(1, 256);
        slider->setValue(100);
        slider->connect(slider, &QSlider::valueChanged, [this](int value) {
            val = static_cast<uchar>(value);
            params[0] = static_cast<void*>(&val);
            update();
            });
        main_layout->addWidget(slider);
        initialize();
    }
    ~QuantizeNode() {
        delete slider;
    }
};


class GrayNode : public BaseNode {
public:
    QComboBox* combo;
    uchar mode;
    GrayNode() {
        type = 5;
        title = "Grayscale";
        combo = new QComboBox();
        combo->addItem("NTSC");
        combo->addItem("From R");
        combo->addItem("From G");
        combo->addItem("From B");
        combo->connect(combo, &QComboBox::currentIndexChanged, [this](int index) {
            mode = static_cast<uchar>(index);
            params[0] = static_cast<void*>(&mode);
            update();
            });
        main_layout->addWidget(combo);
        initialize();
    }
    ~GrayNode() {
        delete combo;
    }
};

class HSVSplitNode : public BaseNode {
public:
    HSVSplitNode() {
        type = 6;
        title = "HSV Split";
        initialize();
    }
};

class BinaryThresNode : public BaseNode {
public:
    QSlider* slider;
    uchar val;
    BinaryThresNode() {
        type = 7;
        title = "Binary Threshold";
        val = 100;
        slider = new QSlider(Qt::Horizontal);
        slider->setRange(0, 255);
        slider->setValue(100);
        slider->connect(slider, &QSlider::valueChanged, [this](int value) {
            val = static_cast<uchar>(value);
            params[0] = static_cast<void*>(&val);
            update();
            });
        main_layout->addWidget(slider);
        initialize();
    }
    ~BinaryThresNode() {
        delete slider;
    }
};

class RGBMaskNode : public BaseNode {
public:
    RGBMaskNode() {
        type = 8;
        title = "RGB Mask";
        initialize();
    }
};