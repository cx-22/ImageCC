#pragma once

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>

#include "node-widgets.h"

using namespace QtNodes;

std::shared_ptr<NodeDelegateModelRegistry> build_reg(){
    auto reg = std::make_shared<NodeDelegateModelRegistry>();

    reg->registerModel<InputNode>();
    reg->registerModel<OutputNode>();

    return reg;
}
