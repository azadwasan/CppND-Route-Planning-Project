#pragma once


#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>
#include <vector>

using std::vector;

class RouteModel : public Model {

  public:
  class Node : public Model::Node {
    public:
    // Add public Node variables and methods here.

    Node(){}
    Node(int idx, RouteModel * search_model, Model::Node node) :
    Model::Node(node), parent_model(search_model)
      , m_parent{nullptr}, m_hValue{std::numeric_limits<float>::max()}, m_gValue{0.0}
    , m_visited{false}, m_neighbors{}, index(idx) {}

    double distance(const Node& otherNode) const {
      return std::sqrt(std::pow((x - otherNode.x), 2) + std::pow((y - otherNode.y), 2)) ;
    }

    /* Private Functions */ 
    private:
      Node* FindNeighbors(vector<int> node_indices) const;

    /* Public member variables */ 
    public:
    Node* m_parent;
    float m_hValue;
    float m_gValue;
    bool m_visited;
    vector<Node*> m_neighbors;

    /* Private member variables */ 
    private:
    // Add private Node variables and methods here.
    int index;
    RouteModel * parent_model = nullptr;
  };

  // Add public RouteModel variables and methods here.
  RouteModel(const std::vector<std::byte> &xml);  
  std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.

  inline vector<Node>& SNodes() {return m_Nodes;}
  private:
  vector<Node> m_Nodes;
};