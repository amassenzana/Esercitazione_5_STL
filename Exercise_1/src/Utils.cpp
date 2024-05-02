#include "Utils.hpp"
#include <fstream>

namespace MeshLibrary{

bool ImportMesh(const string &filepath, TriangularMesh &mesh){

    if(!ImportCell0D(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell1D(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell2D(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        // Test:
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            array<unsigned int, 3> edges = mesh.Cell2DEdges[c];

            for(unsigned int e = 0; e < 3; e++)
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }

    return true;
}

bool ImportCell0D(const string& path, TriangularMesh &mesh){

    ifstream file(path);
    if(file.fail()) return false;

    list<string> lines;
    string line;

    while(getline(file,line))
        lines.push_back(line);

    file.close();
    lines.pop_front();

    mesh.NumberCell0D = lines.size();

    if(mesh.NumberCell0D==0){
        cerr << "There is no cell 0D" << endl;
        return false;
    }


    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);


    for(const string& line : lines){
        istringstream convert(line);
        unsigned int id, marker;
        Eigen::Vector2d coordinates;

        convert >> id >> marker >> coordinates(0) >> coordinates(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coordinates);

        if(marker!=0){
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }


    return true;
}

bool ImportCell1D(const string &fileName, TriangularMesh &mesh){
    ifstream file(fileName);
    if(file.fail()) return false;

    list<string> listLines;
    string line;

    while (getline(file,line))
        listLines.push_back(line);
    file.close();

    listLines.pop_front();
    mesh.NumberCell1D = listLines.size();

    if(mesh.NumberCell1D == 0){
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for(const string& line : listLines){
        istringstream converter(line);
        unsigned int id, marker;
        Eigen::Vector2i vertices;

        converter >> id >> marker >> vertices(0) >> vertices(1);
        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if(marker!=0){
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }


    return true;
}

bool ImportCell2D(const string &fileName, TriangularMesh &mesh){

    ifstream file(fileName);
    if(file.fail()) return false;

    list<string> listLines;
    string line;
    while(getline(file,line))
        listLines.push_back(line);
    file.close();

    listLines.pop_front();
    mesh.NumberCell2D = listLines.size();

    if(mesh.NumberCell2D == 0){
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for(const string& line : listLines){
        istringstream converter(line);
        unsigned int id;
        array<unsigned int, 3> vertices;
        array<unsigned int, 3> edges;

        converter >> id;
        for(unsigned int i = 0; i < 3; i++)
            converter >> vertices[i];
        for(unsigned int i = 0; i < 3; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }


    return true;
}



}
