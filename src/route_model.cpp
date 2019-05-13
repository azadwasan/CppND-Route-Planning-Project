#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int index =0;
    for(const Model::Node& node:this->Nodes()){
        m_Nodes.emplace_back(index, this, node);
        index++;
    }
}

RouteModel::Node* RouteModel::Node::FindNeighbors(vector<int> node_indices) const{
    Node* closestNeighbor = nullptr;
    double currentDistance = std::numeric_limits<double>::max();
    for(int nodeIndex:node_indices){
        Node& node = parent_model->SNodes()[nodeIndex];
        if(!node.m_visited && (&node!=this)){
            double newDistnace = distance(node);
            if(newDistnace<currentDistance || closestNeighbor==nullptr){
                currentDistance = newDistnace;
                closestNeighbor = &node;
            }
        }
    }
    return closestNeighbor;
}
