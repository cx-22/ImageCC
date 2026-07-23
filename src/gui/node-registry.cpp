#include "node-registry.h"

std::shared_ptr<NodeDelegateModelRegistry> build_reg(){
    auto reg = std::make_shared<NodeDelegateModelRegistry>();

    reg->registerModel<InputNode>();
    reg->registerModel<OutputNode>();
    reg->registerModel<BMathNode>();
    reg->registerModel<BlendNode>();
    reg->registerModel<RGBSplitNode>();
    reg->registerModel<QuantizeNode>();
    reg->registerModel<GrayNode>();

    return reg;
}

std::map<std::string, int> g_name_func_map = {
    {"Input", -2},
    {"Output", -1},
    {"Arithmetic", 0},
    {"Blend", 1},
    {"RGB Split", 2},
    {"Quantize RGB", 3},
    {"Grayscale", 4},
};
