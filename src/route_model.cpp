#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int index =0;
    for(const Model::Node& node:this->Nodes()){
        m_Nodes.emplace_back(index, this, node);
        index++;
    }
    CreateNodeToRoadHashmap();
}

void RouteModel::CreateNodeToRoadHashmap(){
    for(const Road& road:Roads()){
        if(road.type!=Model::Road::Type::Footway){
            for(int nodeIndex:Ways()[road.way].nodes){
                if(node_to_road.find(nodeIndex)==node_to_road.end()){
                    node_to_road[nodeIndex] = vector<const Model::Road*>{};
                }
                node_to_road[nodeIndex].push_back(&road);
            }
        }
    }
}

RouteModel::Node& RouteModel::FindClosestNode(float x, float y){
    RouteModel::Node node{x, y};
    float min_dist = std::numeric_limits<float>::max();
    int closest_idx;
    for(const auto& road:Roads()){
        if(road.type!=Model::Road::Type::Footway){
            for(int otherNodeIdx:Ways()[road.way].nodes){
                float dist = node.distance(SNodes()[otherNodeIdx]);
                if( dist < min_dist){
                    min_dist    = dist;
                    closest_idx = otherNodeIdx;
                }
            }
        }
    }
    return SNodes()[closest_idx];
}


/************************* Node - Internal Class definition ******************************/ 
RouteModel::Node::Node(float x, float y):RouteModel::Node::Node(0, nullptr, Model::Node{x,y}){  //User construcotr delegation
}

RouteModel::Node::Node(int idx, RouteModel * search_model, Model::Node node) :
    Model::Node(node), parent_model{search_model}
      , parent{nullptr}, h_value{std::numeric_limits<float>::max()}, g_value{0.0}
      , visited{false}, neighbors{}, index(idx) {
}

RouteModel::Node* RouteModel::Node::FindNeighbor(vector<int> node_indices) const{
    Node* closestNeighbor = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for(int nodeIndex:node_indices){
        Node& node = parent_model->m_Nodes[nodeIndex];
        if(!node.visited && distance(node)!=0){
            double newDistnace = distance(node);
            if(newDistnace<minDistance || closestNeighbor==nullptr){
                minDistance = newDistnace;
                closestNeighbor = &node;
            }
        }
    }
    return closestNeighbor;
}

void RouteModel::Node::FindNeighbors(){
   for(const Road* road:parent_model->node_to_road[this->index]){
        RouteModel::Node* neighboringNode = nullptr;
        if(parent_model){
            neighboringNode = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
        }
        if(neighboringNode){
            this->neighbors.push_back(neighboringNode);
        }
    }
}
