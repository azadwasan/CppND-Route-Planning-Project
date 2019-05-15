#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    // Add public variables or methods declarations here.
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    float GetDistance(){return m_distance;}
    void AStarSearch();
  private:
    // Add private variables or methods declarations here.
    vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node* current_node);
    float CalculateHValue(const RouteModel::Node* node);
    RouteModel::Node* NextNode();

    RouteModel &m_Model;
    RouteModel::Node* m_startNode;
    RouteModel::Node* m_endNode;
    float m_distance;
    vector<RouteModel::Node*> m_openList;
};
