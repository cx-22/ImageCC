#include "node-registry.h"

std::shared_ptr<NodeDelegateModelRegistry> build_reg(){
    auto reg = std::make_shared<NodeDelegateModelRegistry>();

    reg->registerModel<InputNode>();
    reg->registerModel<BMathNode>();
    reg->registerModel<BlendNode>();
    reg->registerModel<RGBSplitNode>();
    reg->registerModel<QuantizeNode>();
    reg->registerModel<GrayNode>();
    reg->registerModel<HSVSplitNode>();
    reg->registerModel<BinaryThresNode>();
    reg->registerModel<RGBMaskNode>();
    reg->registerModel<HistEqNode>();

    return reg;
}

std::map<std::string, int> g_name_func_map = {
    {"Input", 0},
    {"Arithmetic", 1},
    {"Blend", 2},
    {"RGB Split", 3},
    {"Quantize RGB", 4},
    {"Grayscale", 5},
    {"HSV Split", 6},
    {"Binary Threshold", 7},
    {"RGB Mask", 8},
    {"Hist Eq", 9},
};
