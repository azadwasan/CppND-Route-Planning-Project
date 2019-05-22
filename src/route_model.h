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
       /*****************************************
        **********    Public methods   **********
        *****************************************/ 

    Node(){}
    Node(float x, float y);
    Node(int idx, RouteModel * search_model, Model::Node node);
    void FindNeighbors();

    double distance(const Model::Node& otherNode) const {
      return std::sqrt(std::pow((this->x - otherNode.x), 2.0f) + std::pow((this->y - otherNode.y), 2.0f)) ;
    }
       /*****************************************
        ******** Public member variables ********
        *****************************************/ 
    public:
    Node* parent;
    float h_value;
    float g_value;
    bool visited;
    vector<Node*> neighbors;

       /*****************************************
        ********    Private Functions    ********
        *****************************************/ 
    private:
      Node* FindNeighbor(vector<int> node_indices) const;
       /*****************************************
        ******* Private member variables ********
        *****************************************/ 
    private:
    int index;
    RouteModel * parent_model = nullptr;
  };

  /*****************************************
  **********    Public methods   **********
  *****************************************/ 
  RouteModel(const std::vector<std::byte> &xml);  

  inline std::unordered_map<int, vector<const Model::Road*>>& GetNodeToRoadMap(){
    return node_to_road;
  }

  RouteModel::Node& FindClosestNode(float x, float y);
  /*****************************************
  **********    Public Variables  **********
  *****************************************/ 
  std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.
  inline vector<Node>& SNodes() {return m_Nodes;}

  private:
  /*****************************************
  ********    Private Methods    ********
  *****************************************/ 
  void CreateNodeToRoadHashmap();

  /*****************************************
  ******* Private member variables ********
  *****************************************/ 
  private:
  vector<Node> m_Nodes;

  std::unordered_map<int, vector<const Model::Road*>> node_to_road;
};