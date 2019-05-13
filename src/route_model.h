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

    double distance(const Node& otherNode) const {
      return std::sqrt(std::pow((x - otherNode.x), 2) + std::pow((y - otherNode.y), 2)) ;
    }
       /*****************************************
        ******** Public member variables ********
        *****************************************/ 
    public:
    Node* m_parent;
    float m_hValue;
    float m_gValue;
    bool m_visited;
    vector<Node*> m_neighbors;

       /*****************************************
        ********    Private Functions    ********
        *****************************************/ 
    private:
      Node* FindNeighbors(vector<int> node_indices) const;
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