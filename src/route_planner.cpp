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
    while((parent = current_node->m_parent)!=nullptr){
        foundPath.emplace_back(*parent);
        m_distance += current_node->distance(*parent);
        current_node = parent;
    }
    m_distance *= m_Model.MetricScale();
    return foundPath;
}

