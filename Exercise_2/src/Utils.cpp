#include "Utils.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include "Eigen/Eigen"

namespace PolygonalLibrary{

double computeArea(unsigned int IDpoly, PolygonalMesh &mesh){
    int n = mesh.Cell2DVertices[IDpoly].size();
    double area = 0.;
    for(int i = 0; i < n; i++){
        int j = (i+1) % n;
        double ax = mesh.Cell0DCoordinates[mesh.Cell2DVertices[IDpoly][i]](0);
        double ay = mesh.Cell0DCoordinates[mesh.Cell2DVertices[IDpoly][i]](1);
        double bx = mesh.Cell0DCoordinates[mesh.Cell2DVertices[IDpoly][j]](0);
        double by = mesh.Cell0DCoordinates[mesh.Cell2DVertices[IDpoly][j]](1);
        area += ax*by-bx*ay;
    }
    area = fabs(area)/2.0;

    return area;
}

bool ImportMesh(const string &path, PolygonalMesh &mesh){
    if(!ImportCell0D(path,mesh))
        return false;
    else{
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }



    if(!ImportCell1D(path,mesh))
        return false;
    else{
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }


    if(!ImportCell2D(path,mesh))
        return false;
    else{
        /* TEST 1) All marker have been stored correctly
         * Controllo che i punti con marker 5 abbiano y<=TUTTI GLI ALTRI etc...
         * Controllo che i lati con marker 5 abbiano la stessa y?
         */


        /* TEST 2) the edges of polygons have non-zero length
         * Per fare questo test devo entrare in ogni poligono,
         * selezionare i suoi lati e controllarne la lunghezza. */

        for(unsigned int i = 0; i < mesh.NumberCell2D; i++){
            // lati del poligono i: mesh.Cell2DEdges[i]
            for(const unsigned int& edgeId : mesh.Cell2DEdges[i]){
                int aID = mesh.Cell1DVertices[edgeId](0);
                int bID = mesh.Cell1DVertices[edgeId](1);
                double ax = mesh.Cell0DCoordinates[aID](0);
                double ay = mesh.Cell0DCoordinates[aID](1);
                double bx = mesh.Cell0DCoordinates[bID](0);
                double by = mesh.Cell0DCoordinates[bID](1);

                double len = fabs(ax-bx)+fabs(ay-by);
                if (len < 1e-8){
                    cerr << "ERRORE: " << aID << " " << bID << endl;
                    return false;
                }
            }
        }
        cout << "TEST 2: Lunghezza dei lati > 0. SUPERATO CORRETTAMENTE"<< endl;

        /* TEST 3) the area of the polygons is non-zero
         * Devo entrare in ogni poligono e calcolarne l'area
         * Poi controllo che sia più grande di una certa tolleranza. */

        for(unsigned int i = 0; i < mesh.NumberCell2D; i++){
            double area = computeArea(i, mesh);
            if(area <= 1e-10){
                cerr << "Errrore: poligono " << i << " troppo piccolo!";
                return false;
            }
        }
        cout << "TEST 3: Area dei poligoni > 0. SUPERATO CORRETTAMENTE" << endl;
    }


    return true;
}

bool ImportCell0D(const string &path, PolygonalMesh &mesh){

    ifstream f(path+"/Cell0Ds.csv");
    if(f.fail()) return false;

    list<string> lines;
    string line;
    while(getline(f,line))
        lines.push_back(line);

    f.close();
    lines.pop_front();

    mesh.NumberCell0D = lines.size();
    if(mesh.NumberCell0D==0){
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for(string line : lines){

        istringstream iss(line);
        string token;
        getline(iss, token, ';');
        unsigned int id = stoi(token);
        // cout << token;
        getline(iss, token, ';');
        unsigned int marker = stoi(token);
        // cout << " " << token;
        getline(iss, token, ';');
        Eigen::Vector2d coordinates;
        istringstream iss2(token);
        iss2 >> coordinates(0);
        // cout << " " << token;
        getline(iss, token, ';');
        istringstream iss3(token);
        iss3 >> coordinates(1);
        // cout << " " << token << endl;



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

bool ImportCell1D(const string &path, PolygonalMesh &mesh){

    ifstream f(path+"/Cell1Ds.csv");
    if(f.fail())
        return false;

    list<string> listLines;
    string line;

    while(getline(f,line))
        listLines.push_back(line);
    f.close();

    listLines.pop_front();
    mesh.NumberCell1D = listLines.size();

    if(mesh.NumberCell1D == 0){
        cerr << "There is no cell 1D" << endl;
        return false;
    }


    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for(const string& line : listLines){
        string token;
        istringstream iss(line);
        getline(iss,token,';');
        unsigned int id = stoi(token);
        getline(iss,token,';');
        unsigned int marker = stoi(token);
        getline(iss,token,';');
        Eigen::Vector2i vertices;
        vertices(0) = stoi(token);
        getline(iss,token,';');
        vertices(1) = stoi(token);

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

bool ImportCell2D(const string &path, PolygonalMesh &mesh){
    ifstream f(path+"/Cell2Ds.csv");
    if(f.fail())
        return false;

    list<string> listLines;
    string line;

    while(getline(f, line))
        listLines.push_back(line);
    f.close();

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
        string token;
        istringstream iss(line);
        getline(iss, token, ';');
        unsigned int id = stoi(token);
        getline(iss, token, ';');

        mesh.Cell2DId.push_back(id);
        // cout << "riga: " << line << endl;

        getline(iss, token, ';');
        unsigned int numVert = stoi(token);

        vector<unsigned int> vert(numVert);
        for(unsigned int i = 0; i < numVert; i++){
            getline(iss, token, ';');
            vert[i] = stoi(token);
        }
        mesh.Cell2DVertices.push_back(vert);




        getline(iss, token, ';');
        unsigned int numEdg = stoi(token);

        vector<unsigned int> edg(numEdg);
        for(unsigned int i = 0; i < numEdg; i++){
            getline(iss, token, ';');
            edg[i] = stoi(token);
        }
        mesh.Cell2DEdges.push_back(edg);

    }


    return true;
}


} // namespace PolygonalMesh
