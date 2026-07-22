#include "node-registry.h"

std::shared_ptr<NodeDelegateModelRegistry> build_reg(){
    auto reg = std::make_shared<NodeDelegateModelRegistry>();

    reg->registerModel<InputNode>();
    reg->registerModel<OutputNode>();
    reg->registerModel<AddNode>();

    return reg;
}

std::map<std::string, int> g_name_func_map = {
    {"Input", -2},
    {"Output", -1},
    {"Add", 0},
};
