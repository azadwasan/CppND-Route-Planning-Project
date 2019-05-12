#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int index =0;
    for(const Model::Node& node:this->Nodes()){
        m_Nodes.emplace_back(index, this, node);
        index++;
    }
}
