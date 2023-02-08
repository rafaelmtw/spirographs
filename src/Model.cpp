#include "Model.h"
#include <GLFW/glfw3.h>

Model::Model()
{
    vector<glm::vec4> circlePositions;

    // create vertices here
    int SLICES = 100;
    float PI = 3.14159f;
    // the first vertex for the center

    for (int i = 0; i < SLICES + 1; i++)
    {
        float theta = i * 2 * PI / SLICES;

        circlePositions.push_back(glm::vec4(
            cos(theta),
            sin(theta),
            0,
            1));
    }

    // set up outer circle
    vector<VertexAttribWithColor> outerCircleVertex;
    for (unsigned int i = 0; i < circlePositions.size(); i++)
    {
        VertexAttribWithColor v;
        vector<float> data;

        data.push_back(circlePositions[i].x);
        data.push_back(circlePositions[i].y);
        data.push_back(circlePositions[i].z);
        data.push_back(circlePositions[i].w);
        v.setData("position", data);

        data.clear();
        data.push_back(1.0f); // red
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(1.0f);
        v.setData("color", data);

        outerCircleVertex.push_back(v);
    }

    // set up inner circle
    vector<VertexAttribWithColor> innerCircleVertex;
    for (unsigned int i = 0; i < circlePositions.size(); i++)
    {
        VertexAttribWithColor v;
        vector<float> data;

        data.push_back(circlePositions[i].x);
        data.push_back(circlePositions[i].y);
        data.push_back(circlePositions[i].z);
        data.push_back(circlePositions[i].w);
        v.setData("position", data);

        data.clear();
        data.push_back(1.0f); // orange
        data.push_back(0.65f);
        data.push_back(0.0f);
        data.push_back(1.0f);
        v.setData("color", data);

        innerCircleVertex.push_back(v);
    }

    // set up seed point
    vector<VertexAttribWithColor> seedPointVertex;
    for (unsigned int i = 0; i < circlePositions.size(); i++)
    {
        VertexAttribWithColor v;
        vector<float> data;

        data.push_back(circlePositions[i].x);
        data.push_back(circlePositions[i].y);
        data.push_back(circlePositions[i].z);
        data.push_back(circlePositions[i].w);
        v.setData("position", data);

        data.clear();
        data.push_back(0.0f); // green
        data.push_back(1.0f);
        data.push_back(0.0f);
        data.push_back(1.0f);
        v.setData("color", data);

        seedPointVertex.push_back(v);
    }

    vector<unsigned int> circleIndices;

    for (int i = 0; i < circlePositions.size(); i++)
    {
        circleIndices.push_back(i);
    }

    // now we create a polygon mesh object. Think of this as literally a mesh
    // or network of polygons. There are vertices and they form polygons
    util::PolygonMesh<VertexAttribWithColor> outerCircleMesh;
    util::PolygonMesh<VertexAttribWithColor> innerCircleMesh;
    util::PolygonMesh<VertexAttribWithColor> seedPointMesh;

    // give it the vertex data
    outerCircleMesh.setVertexData(outerCircleVertex);
    innerCircleMesh.setVertexData(innerCircleVertex);
    seedPointMesh.setVertexData(seedPointVertex);
    // give it the index data that forms the polygons
    outerCircleMesh.setPrimitives(circleIndices);
    innerCircleMesh.setPrimitives(circleIndices);
    seedPointMesh.setPrimitives(circleIndices);

    outerCircleMesh.setPrimitiveType(
        GL_LINE_STRIP);                  // when rendering specify this to OpenGL
    outerCircleMesh.setPrimitiveSize(2); // 2 vertices per polygon

    innerCircleMesh.setPrimitiveType(
        GL_LINE_STRIP);                  // when rendering specify this to OpenGL
    innerCircleMesh.setPrimitiveSize(2); // 2 vertices per polygon

    seedPointMesh.setPrimitiveType(
        GL_LINE_STRIP);                // when rendering specify this to OpenGL
    seedPointMesh.setPrimitiveSize(2); // 2 vertices per polygon

    meshes.push_back(outerCircleMesh);
    meshes.push_back(innerCircleMesh);
    meshes.push_back(seedPointMesh);

    vector<glm::vec4> curvePositions;

    // inspired by github of @dharmendranamdev

    float R = 400;
    r = 200;
    float p = r / 2;

    float k = r / R;
    float l = p / r;

    float a; // angle
    float x1 = R * ((1 - k) * cos(a) + l * k * cos(((1 - k) / k) * a));
    float y1 = R * ((1 - k) * sin(a) - l * k * sin(((1 - k) / k) * a));
    float x, y;
    a = 0.05;
    while (a < 300) // approximately 50 revolutions
    {
        //
        x = R * ((1 - k) * cos(a) + l * k * cos(((1 - k) / k) * a));
        y = R * ((1 - k) * sin(a) - l * k * sin(((1 - k) / k) * a));

        curvePositions.push_back(glm::vec4(
            x,
            y,
            0,
            1));
        curvePositions.push_back(glm::vec4(
            x1,
            y1,
            0,
            1));
        x1 = x;
        y1 = y;
        a += 0.05;
    };

    // set up the curve
    vector<VertexAttribWithColor> curveVertex;
    for (unsigned int i = 0; i < curvePositions.size(); i++)
    {
        VertexAttribWithColor v;
        vector<float> data;

        data.push_back(curvePositions[i].x);
        data.push_back(curvePositions[i].y);
        data.push_back(curvePositions[i].z);
        data.push_back(curvePositions[i].w);
        v.setData("position", data);

        data.clear();
        data.push_back(0.0f); // green
        data.push_back(1.0f);
        data.push_back(0.0f);
        data.push_back(1.0f);
        v.setData("color", data);

        curveVertex.push_back(v);
    }

    vector<unsigned int> curveIndices;

    for (int i = 0; i < curvePositions.size(); i++)
    {
        curveIndices.push_back(i);
    }

    util::PolygonMesh<VertexAttribWithColor> curveMesh;

    // give it the vertex data
    curveMesh.setVertexData(curveVertex);

    // give it the index data that forms the polygons
    curveMesh.setPrimitives(curveIndices);

    curveMesh.setPrimitiveType(
        GL_LINE_STRIP);            // when rendering specify this to OpenGL
    curveMesh.setPrimitiveSize(2); // 2 vertices per polygon

    meshes.push_back(curveMesh);
}

void Model::updateCurve(float r)
{
    vector<glm::vec4> curvePositions;

    float R = 400;
    float p = r / 2;

    float k = r / R;
    float l = p / r;

    float a; // angle
    float x1 = R * ((1 - k) * cos(a) + l * k * cos(((1 - k) / k) * a));
    float y1 = R * ((1 - k) * sin(a) - l * k * sin(((1 - k) / k) * a));
    float x, y;
    a = 0.05;
    while (a < 300) // approximately 50 revolutions
    {
        //
        x = R * ((1 - k) * cos(a) + l * k * cos(((1 - k) / k) * a));
        y = R * ((1 - k) * sin(a) - l * k * sin(((1 - k) / k) * a));

        curvePositions.push_back(glm::vec4(
            x,
            y,
            0,
            1));
        curvePositions.push_back(glm::vec4(
            x1,
            y1,
            0,
            1));
        x1 = x;
        y1 = y;
        a += 0.05;
    };

    // set up the curve
    vector<VertexAttribWithColor> curveVertex;
    for (unsigned int i = 0; i < curvePositions.size(); i++)
    {
        VertexAttribWithColor v;
        vector<float> data;

        data.push_back(curvePositions[i].x);
        data.push_back(curvePositions[i].y);
        data.push_back(curvePositions[i].z);
        data.push_back(curvePositions[i].w);
        v.setData("position", data);

        data.clear();
        data.push_back(0.0f); // green
        data.push_back(1.0f);
        data.push_back(0.0f);
        data.push_back(1.0f);
        v.setData("color", data);

        curveVertex.push_back(v);
    }

    vector<unsigned int> curveIndices;

    for (int i = 0; i < curvePositions.size(); i++)
    {
        curveIndices.push_back(i);
    }

    util::PolygonMesh<VertexAttribWithColor> curveMesh;

    // give it the vertex data
    curveMesh.setVertexData(curveVertex);

    // give it the index data that forms the polygons
    curveMesh.setPrimitives(curveIndices);

    curveMesh.setPrimitiveType(
        GL_LINE_STRIP);            // when rendering specify this to OpenGL
    curveMesh.setPrimitiveSize(2); // 2 vertices per polygon

    meshes.pop_back();           // remove the previous one
    meshes.push_back(curveMesh); // push the new one
}

vector<util::PolygonMesh<VertexAttribWithColor>> Model::getMeshes()
{
    return meshes;
}

Model::~Model()
{
}
