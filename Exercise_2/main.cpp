#include "Utils.hpp"
#include <iostream>

int main(){

    string filepath = "PolygonalMesh";
    PolygonalLibrary::PolygonalMesh mesh;

    if(ImportMesh(filepath, mesh))
        cout << "OK";

    return 0;
}
