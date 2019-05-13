#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int index =0;
    for(const Model::Node& node:this->Nodes()){
        m_Nodes.emplace_back(index, this, node);
        index++;
    }
}

RouteModel::Node(int idx, RouteModel * search_model, Model::Node node) :
    Model::Node(node), parent_model(search_model)
      , m_parent{nullptr}, m_hValue{std::numeric_limits<float>::max()}, m_gValue{0.0}
    , m_visited{false}, m_neighbors{}, index(idx) {

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

void RouteModel::Node::FindNeighbors(){
    for(const auto& road:parent_model->Ways()[road]){

    }
}
