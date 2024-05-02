#include "Utils.hpp"
#include <iostream>





int main(){

    string filepath = "PolygonalMesh";
    PolygonalLibrary::PolygonalMesh mesh;

    if(ImportMesh(filepath, mesh))
        cout << "Tutta la costruzione e tutti i test sono stati"
                "eseguiti e superati correttamente. " << endl;


    return 0;
}
