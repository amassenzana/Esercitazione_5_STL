#ifndef __UTILS_H
#define __UTILS_H

#include <iostream>
#include "TriangularMesh.hpp"

using namespace std;

namespace MeshLibrary{

bool ImportMesh(const string& path, TriangularMesh& mesh);
bool ImportCell0D(const string& path, TriangularMesh& mesh);
bool ImportCell1D(const string& path, TriangularMesh& mesh);
bool ImportCell2D(const string& path, TriangularMesh& mesh);

}

#endif
