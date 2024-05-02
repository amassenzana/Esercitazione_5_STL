#ifndef __UTILS_H
#define __UTILS_H

#include "PolygonalMesh.hpp"
#include <fstream>
using namespace std;

namespace PolygonalLibrary{

bool ImportMesh(const string& path, PolygonalMesh& mesh);
bool ImportCell0D(const string& path, PolygonalMesh& mesh);
bool ImportCell1D(const string& path, PolygonalMesh& mesh);
bool ImportCell2D(const string& path, PolygonalMesh& mesh);




} // namespace PolygonalLibrary


#endif
