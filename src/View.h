#ifndef __VIEW_H__
#define __VIEW_H__

#include <glad/glad.h>
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
#include <cstdio>
#include <ShaderProgram.h>
#include <ObjectInstance.h>
#include "PolygonMesh.h"
#include "VertexAttribWithColor.h"
#include "Callbacks.h"

class View
{
public:
    View();
    ~View();
    void init(Callbacks *callbacks, vector<util::PolygonMesh<VertexAttribWithColor>> &meshes);
    void display(int timeclock);
    bool shouldWindowClose();
    void closeWindow();
    util::ShaderProgram program;
    util::ShaderLocationsVault shaderLocations;
    vector<util::ObjectInstance *> objects;
    map<string, string> shaderVarsToVertexAttribs;
    glm::mat4 projection, modelview;
    bool toggleDraw;
    float radiusOC, radiusIC, speed;

private:
    GLFWwindow *window;
    int frames;
    double time;
    glm::vec2 window_dimensions;
    int timeclock;
};

#endif