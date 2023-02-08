#ifndef __MODEL_H__
#define __MODEL_H__

#include "PolygonMesh.h"
#include "VertexAttribWithColor.h"
#include <vector>
using namespace std;

class Model
{
public:
    Model();
    ~Model();
    void updateCurve(float r);
    vector<util::PolygonMesh<VertexAttribWithColor>> getMeshes();
    float r;

private:
    vector<util::PolygonMesh<VertexAttribWithColor>> meshes;
};
#endif