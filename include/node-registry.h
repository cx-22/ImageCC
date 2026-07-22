#pragma once

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>

#include "node-widgets.h"
#include <map>

using namespace QtNodes;

std::shared_ptr<NodeDelegateModelRegistry> build_reg();
extern std::map<std::string, int> g_name_func_map;
