#include <iostream>
#include <fstream>
#include "your_code/BinaryHeap.h"
#include "your_code/MeshGraph.h"
#include "your_code/ObjLoader.h"
#include "your_code/Structs.h"
#include "your_code/ObjLoader.cpp"
#include "your_code/MeshGraph.cpp"
#include "your_code/BinaryHeap.cpp"

using namespace std;

int main(int argc, char **argv){
    // get all obj files in the folder if temp1 doesn't exist then create it
    string command = "dir /b /s objects > temp1.txt";
    system(command.c_str());
    ifstream file("temp1.txt");
    string line;
    vector<string> objs;
    while(getline(file, line)) objs.push_back(line);
    cout << line;
    file.close();
    // remove temp.txt
    command = "del temp1.txt";
    system(command.c_str());
    ObjLoader loader;
    // load the obj file
    int i = 0; 
    cin >> i;
    string path = objs[i];
    std::vector<Double3> vertexPositions;
    std::vector<IdPair> edges;
    loader.LoadObjFile(path);
    vertexPositions = loader.GetVertexPos();
    edges = loader.GetEdges();

    // testing begins
    MeshGraph graph(vertexPositions, edges);

    cout << "Testing obj:" << i << endl;
    cout << "Average distance between vertices: " << graph.AverageDistanceBetweenVertices() << endl;
    cout << "Average edge per vertex: " << graph.AverageEdgePerVertex() << endl;
    cout << "Total vertex count: " << graph.TotalVertexCount() << endl;
    cout << "Total edge count: " << graph.TotalEdgeCount() << endl;
    cout << "Vertex edge count: " << graph.VertexEdgeCount(0) << endl;
    cout << "Immediate neighbours: ";
    std::vector<int> neighbours;
    graph.ImmediateNeighbours(neighbours, 0);
    for(std::vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;
    graph.ImmediateNeighbours(neighbours, graph.TotalVertexCount()/4);
    for(std::vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;
    graph.ImmediateNeighbours(neighbours, graph.TotalVertexCount()/2);
    for(std::vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;
    graph.ImmediateNeighbours(neighbours, graph.TotalVertexCount());
    for(std::vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;
    
    // PAINT IN RANGE
    std::vector<Color> paintInBetween;
    graph.PaintInBetweenVertex(paintInBetween, graph.TotalVertexCount()/4, graph.TotalVertexCount()/2, Color{255, 0, 0});

    cout << "PaintInBetweenVertex(" << graph.TotalVertexCount()/4 << ", " << graph.TotalVertexCount()/2 << "):" << endl;
    for(int i = 0; i < paintInBetween.size(); i++){
            cout << "Vertex " << i << ": " << (int)paintInBetween[i].r << " " << (int)paintInBetween[i].g << " " << (int)paintInBetween[i].b << endl;
    }
    cout << endl;
    graph.PaintInBetweenVertex(paintInBetween, graph.TotalVertexCount()/2, graph.TotalVertexCount(), Color{255, 0, 0});
    cout << "PaintInBetweenVertex(" << graph.TotalVertexCount()/2 << ", " << graph.TotalVertexCount() << "):" << endl;
    for(int i = 0; i < paintInBetween.size(); i++){
            cout << "Vertex " << i << ": " << (int)paintInBetween[i].r << " " << (int)paintInBetween[i].g << " " << (int)paintInBetween[i].b << endl;
    }
    cout << endl;
    //GEODESIC BOX FILTER
    std::vector<Color> geodesic_box;
    graph.PaintInRangeGeodesic(geodesic_box, graph.TotalVertexCount()/4, Color{255, 0, 0}, 2, FILTER_BOX, 0.5);
    cout << "PaintInRangeGeodesic(" << graph.TotalVertexCount()/4 << ", 2, FILTER_BOX, 0.5):" << endl;
    for(int i = 0; i < geodesic_box.size(); i++){
            cout << "Vertex " << i << ": " << (int)geodesic_box[i].r << " " << (int)geodesic_box[i].g << " " << (int)geodesic_box[i].b << endl;
    }
    graph.PaintInRangeGeodesic(geodesic_box, graph.TotalVertexCount()/4, Color{255, 0, 0}, 4, FILTER_BOX, 0.5);
    cout << "PaintInRangeGeodesic(" << graph.TotalVertexCount()/4 << ", 4, FILTER_BOX, 0.5):" << endl;
    for(int i = 0; i < geodesic_box.size(); i++){
            cout << "Vertex " << i << ": " << (int)geodesic_box[i].r << " " << (int)geodesic_box[i].g << " " << (int)geodesic_box[i].b << endl;
    }
    //GEODESIC GAUSSIAN FILTER
    std::vector<Color> geodesic_gaussian;
    graph.PaintInRangeGeodesic(geodesic_gaussian, graph.TotalVertexCount()/4, Color{255, 0, 0}, 2, FILTER_GAUSSIAN, 0.5);
    cout << "PaintInRangeGeodesic(" << graph.TotalVertexCount()/4 << ", 2, FILTER_GAUSSIAN, 0.5):" << endl;
    for(int i = 0; i < geodesic_gaussian.size(); i++){
            cout << "Vertex " << i << ": " << (int)geodesic_gaussian[i].r << " " << (int)geodesic_gaussian[i].g << " " << (int)geodesic_gaussian[i].b << endl;
    }
    graph.PaintInRangeGeodesic(geodesic_gaussian, graph.TotalVertexCount()/4, Color{255, 0, 0}, 4, FILTER_GAUSSIAN, 0.5);
    cout << "PaintInRangeGeodesic(" << graph.TotalVertexCount()/4 << ", 4, FILTER_GAUSSIAN, 0.5):" << endl;
    for(int i = 0; i < geodesic_gaussian.size(); i++){
            cout << "Vertex " << i << ": " << (int)geodesic_gaussian[i].r << " " << (int)geodesic_gaussian[i].g << " " << (int)geodesic_gaussian[i].b << endl;
    }
    //EUCLIDIAN BOX FILTER
    std::vector<Color> euclidian_box;
    graph.PaintInRangeEuclidian(euclidian_box, graph.TotalVertexCount()/4, Color{255, 0, 0}, 2, FILTER_BOX, 0.5);
    cout << "PaintInRangeEuclidian(" << graph.TotalVertexCount()/4 << ", 2, FILTER_BOX, 0.5):" << endl;
    for(int i = 0; i < euclidian_box.size(); i++){
            cout << "Vertex " << i << ": " << (int)euclidian_box[i].r << " " << (int)euclidian_box[i].g << " " << (int)euclidian_box[i].b << endl;
    }
    graph.PaintInRangeEuclidian(euclidian_box, graph.TotalVertexCount()/4, Color{255, 0, 0}, 4, FILTER_BOX, 0.5);
    cout << "PaintInRangeEuclidian(" << graph.TotalVertexCount()/4 << ", 4, FILTER_BOX, 0.5):" << endl;
    for(int i = 0; i < euclidian_box.size(); i++){
            cout << "Vertex " << i << ": " << (int)euclidian_box[i].r << " " << (int)euclidian_box[i].g << " " << (int)euclidian_box[i].b << endl;
    }
    //EUCLIDIAN GAUSSIAN FILTER
    std::vector<Color> euclidian_gaussian;
    graph.PaintInRangeEuclidian(euclidian_gaussian, graph.TotalVertexCount()/4, Color{255, 0, 0}, 2, FILTER_GAUSSIAN, 0.5);
    cout << "PaintInRangeEuclidian(" << graph.TotalVertexCount()/4 << ", 2, FILTER_GAUSSIAN, 0.5):" << endl;
    for(int i = 0; i < euclidian_gaussian.size(); i++){
            cout << "Vertex " << i << ": " << (int)euclidian_gaussian[i].r << " " << (int)euclidian_gaussian[i].g << " " << (int)euclidian_gaussian[i].b << endl;
    }
    graph.PaintInRangeEuclidian(euclidian_gaussian, graph.TotalVertexCount()/4, Color{255, 0, 0}, 4, FILTER_GAUSSIAN, 0.5);
    cout << "PaintInRangeEuclidian(" << graph.TotalVertexCount()/4 << ", 4, FILTER_GAUSSIAN, 0.5):" << endl;
    for(int i = 0; i < euclidian_gaussian.size(); i++){
            cout << "Vertex " << i << ": " << (int)euclidian_gaussian[i].r << " " << (int)euclidian_gaussian[i].g << " " << (int)euclidian_gaussian[i].b << endl;
    }
    cout << endl;
    return 0;
}
