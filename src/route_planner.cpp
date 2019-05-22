#include "route_planner.h"
#include <algorithm>
#include <iostream>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y)
                :  m_Model(model), m_startNode{nullptr},  m_endNode{nullptr}, m_distance{std::numeric_limits<float>::max()}
{
    const float SCALE_FACTOR = 0.01;
    start_x *= SCALE_FACTOR;
    start_y *= SCALE_FACTOR;
    end_x   *= SCALE_FACTOR;
    end_y   *= SCALE_FACTOR;
    m_startNode = &m_Model.FindClosestNode(start_x, start_y);
    m_endNode   = &m_Model.FindClosestNode(end_x, end_y);
}

vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node* current_node){
    RouteModel::Node* parent = nullptr;
    m_distance = 0.0f;
    vector<RouteModel::Node> foundPath{};
    if(current_node == nullptr){
        std::cout<<"ERROR: Nullpr passed, exiting"<<std::endl;
        return foundPath;
    }

    foundPath.emplace_back(*current_node);
    while((parent = current_node->parent)!=nullptr){
        foundPath.emplace_back(*parent);
        m_distance += current_node->distance(*parent);
        current_node = parent;
    }
    m_distance *= m_Model.MetricScale();
    return foundPath;
}

void RoutePlanner::AStarSearch(){
    m_startNode->visited = true;
    m_openList.push_back(m_startNode);
    RouteModel::Node* currentNode = nullptr;
    while(m_openList.size()>0){
        currentNode = NextNode();
        if(currentNode->distance(*m_endNode) == 0){      //TODO: What if this is not true, i.e., due to float arithimatics for example
            m_Model.path = ConstructFinalPath(currentNode);
            return;
        }
        else{
            AddNeighbors(currentNode);
        }
    }
}

float RoutePlanner::CalculateHValue(const RouteModel::Node* node){
    return node->distance(*m_endNode);
}

RouteModel::Node* RoutePlanner::NextNode(){
    RouteModel::Node* nextNode = nullptr;
    std::sort(m_openList.begin(), m_openList.end(), [](RouteModel::Node* n1, RouteModel::Node* n2) -> bool{
                                                        return (n1->g_value + n1->h_value) > (n2->g_value + n2->h_value);
                                                    }
    ); 
    if(m_openList.size()>0){
        nextNode = m_openList.back();
        m_openList.pop_back();
    }
    return nextNode;
}

void RoutePlanner::AddNeighbors(RouteModel::Node* currentNode){
    if(currentNode==nullptr){
        std::cout<<"RoutePlanner::AddNeighbors, received nullptr, exiting "<<std::endl;
    }
    currentNode->FindNeighbors();
    for(auto& neighborNode:currentNode->neighbors){
        neighborNode->parent = currentNode;
        neighborNode->g_value = currentNode->g_value + currentNode->distance(*neighborNode);
        neighborNode->h_value = CalculateHValue(neighborNode);
        m_openList.push_back(neighborNode);
        neighborNode->visited = true;
    }
}