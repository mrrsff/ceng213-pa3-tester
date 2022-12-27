#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>

MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    // TODO:
    for(int i=0;i<vertexPositions.size();i++){
        std::list<Vertex*> temp;
        adjList.push_back(temp);
    }
    for(int i = 0; i < vertexPositions.size(); i++)
    {
        Vertex temp;
        temp.id = i;
        temp.position3D = vertexPositions[i];
        vertices.push_back(temp);
    }
    for(int i=0;i<edges.size();i++){
        adjList[edges[i].vertexId0].push_back(&vertices[edges[i].vertexId1]);
        adjList[edges[i].vertexId1].push_back(&vertices[edges[i].vertexId0]);
    }
}

double MeshGraph::AverageDistanceBetweenVertices() const
{
    // TODO:
    if(adjList.size() == 0) return 0;
    double sum = 0, divider = 0;
    for(int i=0;i<adjList.size();i++){
        for(std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++){
            sum += Double3::Distance(vertices[i].position3D, (*it)->position3D);
            divider++;
        }
    }
    return sum/divider;
}

double MeshGraph::AverageEdgePerVertex() const
{
    // TODO:
    if(adjList.size() == 0) return 0;
    double sum = 0;
    for(int i=0;i<adjList.size();i++){
        sum += adjList[i].size();
    }
    return sum/(2 * adjList.size());
}

int MeshGraph::TotalVertexCount() const
{
    // TODO:
    return adjList.size();
}

int MeshGraph::TotalEdgeCount() const
{
    // TODO:
    int sum = 0;
    for(int i=0;i<adjList.size();i++){
        sum += adjList[i].size();
    }
    return sum/2;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    // TODO:
    if(vertexId >= adjList.size()) return -1;
    return adjList[vertexId].size();
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,
                                    int vertexId) const
{
    // TODO:
    if(vertexId >= adjList.size()) return;
    for(std::list<Vertex*>::const_iterator it = adjList[vertexId].begin(); it != adjList[vertexId].end(); it++){
        outVertexIds.push_back((*it)->id);
    }
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    // TODO:
    if(vertexIdFrom >= adjList.size() || vertexIdTo >= adjList.size()) return;
    outputColorAllVertex.resize(vertices.size());
    Color black;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    for(int i=0;i<outputColorAllVertex.size();i++){
        outputColorAllVertex[i] = black;
    }
    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> previous(vertices.size(), -1);
    std::vector<double> distances(vertices.size(), INFINITY);
    BinaryHeap heap;
    heap.Add(vertexIdFrom, 0);
    distances[vertexIdFrom] = 0;
    while(heap.HeapSize() > 0){ 
        int i;
        double weight;
        heap.PopHeap(i, weight);
        if(i == vertexIdTo) continue;
        visited[i] = true;
        for(std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++){
            if(visited[(*it)->id]) continue;
            double dist = Double3::Distance(vertices[i].position3D, (*it)->position3D);
            if(distances[(*it)->id] > distances[i] + dist){
                distances[(*it)->id] = distances[i] + dist;
                previous[(*it)->id] = i;
                heap.Add((*it)->id, distances[(*it)->id]);
            }
        }
    }
    // if no path found
    if(previous[vertexIdTo] == INFINITY) return;
    std::vector<int> path;
    int i = vertexIdTo;
    while(i != -1){
        path.push_back(i);
        i = previous[i];
    }
    // paint it
    for(int i=0;i<path.size();i++){
        outputColorAllVertex[path[i]] = color;
    }
}


void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    // TODO:
    if(vertexId >= adjList.size()) return;
    outputColorAllVertex.resize(vertices.size());
    Color black;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    for(int i=0;i<outputColorAllVertex.size();i++){
        outputColorAllVertex[i] = black;
    }
    // implement BFS to track depth and distance at the same time
    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> previous(vertices.size(), -1);
    std::vector<double> distances(vertices.size(), INFINITY);
    std::vector<std::vector<int> > levels(maxDepth + 2, std::vector<int>());
    std::vector<HeapElement> queue;
    HeapElement temp;
    temp.uniqueId = vertexId;
    temp.weight = 0;
    queue.push_back(temp);
    distances[vertexId] = 0;
    visited[vertexId] = true;
    levels[0].push_back(vertexId);
    double currentLevel = 1;
    while(queue.size() > 0){

        int id = queue[0].uniqueId;
        double weight = queue[0].weight;
        queue.erase(queue.begin());
        
        // sort the adjacent vertices according to their id using binary heap as a priority queue.
        BinaryHeap sortedAdj;
        for(std::list<Vertex*>::const_iterator it = adjList[id].begin(); it != adjList[id].end(); it++) sortedAdj.Add((*it)->id,(*it)->id);
        // if the current level is not the same as the previous one, update the current level
        if(weight/1000 > currentLevel){
            currentLevel = weight/1000;
            if(currentLevel > maxDepth+1) break;
        }
        // for each adjacent vertex, update previous and add it to the queue
        while(sortedAdj.HeapSize() > 0){
            int adj_id;
            double _b;
            sortedAdj.PopHeap(adj_id, _b);
            if(visited[adj_id]) continue;
            previous[adj_id] = id;
            if(distances[adj_id]==INFINITY) distances[adj_id] = distances[id] + Double3::Distance(vertices[id].position3D, vertices[adj_id].position3D);
            levels[currentLevel].push_back(adj_id);
            visited[adj_id] = true;
            HeapElement temp;
            temp.uniqueId = adj_id;
            temp.weight = (currentLevel+1)*1000 + adj_id;
            queue.push_back(temp);
        }
    }
    /*for(int i=0;i<maxDepth+1;i++){
        std::cout << "level " << i << ": " << std::endl;
        for(int j=0;j<levels[i].size();j++){
            int node = levels[i][j];
            std::cout << "(id " << node <<" previous:"<< previous[node] 
            <<" dist:" << distances[node] << " x:" << vertices[node].position3D.x << " y:"
            << vertices[node].position3D.y << " z:" << vertices[node].position3D.z << ") adj of " << node << ": ";
            for(std::list<Vertex*>::const_iterator it = adjList[node].begin(); it != adjList[node].end(); it++){
                std::cout << (*it)->id << " ";
            }
            std::cout << std::endl;
        }
    }*/

    for(int i=0;i<maxDepth+1;i++){
        // std::cout << "level " << i << ": " << std::endl;
        for(int j=0;j<levels[i].size();j++){
            int node = levels[i][j];
            //std::cout << "(id " << node <<" previous:"<< previous[node] <<" dist:" << distances[node] << " x:" << vertices[node].position3D.x << " y:" << vertices[node].position3D.y << " z:" << vertices[node].position3D.z << ")" << std::endl; 
            if(type == FILTER_GAUSSIAN){
                double filterValue = std::exp(-(distances[node] * distances[node]) / (alpha * alpha));
                outputColorAllVertex[node].r = color.r*filterValue;
                outputColorAllVertex[node].g = color.g*filterValue;
                outputColorAllVertex[node].b = color.b*filterValue;
            }
            else if(type == FILTER_BOX){
                double filterValue = ((-alpha <= distances[node] && distances[node] <= alpha) ? 1 : 0);
                outputColorAllVertex[node].r = color.r*filterValue;
                outputColorAllVertex[node].g = color.g*filterValue;
                outputColorAllVertex[node].b = color.b*filterValue;
            }
        }
    }
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    // TODO:
    if(vertexId >= adjList.size()) return;
    outputColorAllVertex.resize(vertices.size());
    Color black;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    for(int i=0;i<outputColorAllVertex.size();i++){
        outputColorAllVertex[i] = black;
    }
    // declare variables
    std::vector<bool> visited(vertices.size(), false);
    std::vector<std::vector<int> > levels(maxDepth + 2, std::vector<int>());
    std::vector<HeapElement> queue;
    HeapElement temp;
    temp.uniqueId = vertexId;
    temp.weight = 0;
    queue.push_back(temp);
    levels[0].push_back(vertexId);
    double currentLevel = 1;
    while(queue.size() > 0){
        int id = queue[0].uniqueId;
        double weight = queue[0].weight;
        queue.erase(queue.begin());
        if(weight/1000 > currentLevel){
            currentLevel = weight/1000;
            if(currentLevel > maxDepth+1) break;
        }
        for(std::list<Vertex*>::const_iterator it = adjList[id].begin(); it != adjList[id].end(); it++){
            if(visited[(*it)->id]) continue;
            levels[currentLevel].push_back((*it)->id);
            visited[(*it)->id] = true;
            HeapElement temp;
            temp.uniqueId = (*it)->id;
            temp.weight = (currentLevel+1)*1000 + (*it)->id;
            queue.push_back(temp);
        }
    }
    /*for(int i=0;i<maxDepth+1;i++){
        std::cout << "level " << i << ": " << std::endl;
        for(int j=0;j<levels[i].size();j++){
            int node = levels[i][j];
            std::cout << "(id " << node <<" dist:" << Double3::Distance(vertices[node].position3D, vertices[vertexId].position3D) << " x:" << vertices[node].position3D.x << " y:"
            << vertices[node].position3D.y << " z:" << vertices[node].position3D.z << ") adj of " << node << ": ";
            for(std::list<Vertex*>::const_iterator it = adjList[node].begin(); it != adjList[node].end(); it++){
                std::cout << (*it)->id << " ";
            }
            std::cout << std::endl;
        }
    }*/

    for(int i=0;i<maxDepth+1;i++){
        // std::cout << "level " << i << ": " << std::endl;
        for(int j=0;j<levels[i].size();j++){
            int node = levels[i][j];
            //std::cout << "(id " << node <<" previous:"<< previous[node] <<" dist:" << distances[node] << " x:" << vertices[node].position3D.x << " y:" << vertices[node].position3D.y << " z:" << vertices[node].position3D.z << ")" << std::endl; 
            if(type == FILTER_GAUSSIAN){
                double filterValue = std::exp(-(Double3::Distance(vertices[node].position3D, vertices[vertexId].position3D)*Double3::Distance(vertices[node].position3D, vertices[vertexId].position3D)) / (alpha * alpha));
                outputColorAllVertex[node].r = color.r*filterValue;
                outputColorAllVertex[node].g = color.g*filterValue;
                outputColorAllVertex[node].b = color.b*filterValue;
            }
            else if(type == FILTER_BOX){
                double filterValue = (-alpha <= Double3::Distance(vertices[node].position3D, vertices[vertexId].position3D) && Double3::Distance(vertices[node].position3D, vertices[vertexId].position3D) <= alpha) ? 1 : 0;
                outputColorAllVertex[node].r = color.r*filterValue;
                outputColorAllVertex[node].g = color.g*filterValue;
                outputColorAllVertex[node].b = color.b*filterValue;
            }
        }
    }
}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}