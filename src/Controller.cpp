#include "Controller.h"
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Controller::Controller(Model &m, View &v)
{
    model = m;
    view = v;
}

Controller::~Controller()
{
}

void Controller::run()
{
    vector<util::PolygonMesh<VertexAttribWithColor>>
        meshes = model.getMeshes();
    view.init(this, meshes);
    int timeclock = 0;
    view.radiusIC = model.r;

    while (!view.shouldWindowClose())
    {
        timeclock = (timeclock + 1) % (INT_MAX - 1);
        view.display(timeclock);
    }
    view.closeWindow();
    exit(EXIT_SUCCESS);
}
void Controller::onkey(unsigned int codepoint)
{
    if ((codepoint == 'I'))
    {
        if (view.radiusIC <= view.radiusOC - 5.0f)
        {
            view.radiusIC += 5.0f;

            model.updateCurve(view.radiusIC);

            vector<util::PolygonMesh<VertexAttribWithColor>>
                meshes = model.getMeshes();

            util::ObjectInstance *obj = new util::ObjectInstance("triangles");
            obj->initPolygonMesh<VertexAttribWithColor>(
                view.program,                   // the shader program
                view.shaderLocations,           // the shader locations
                view.shaderVarsToVertexAttribs, // the shader variable -> attrib map
                meshes[3]);                     // the actual mesh object

            view.objects.pop_back();
            view.objects.push_back(obj);
        }
    }
    else if ((codepoint == 'i'))
    {
        if (view.radiusIC >= 10.0f)
        {
            view.radiusIC -= 5.0f;

            model.updateCurve(view.radiusIC);

            vector<util::PolygonMesh<VertexAttribWithColor>>
                meshes = model.getMeshes();

            util::ObjectInstance *obj = new util::ObjectInstance("triangles");
            obj->initPolygonMesh<VertexAttribWithColor>(
                view.program,                   // the shader program
                view.shaderLocations,           // the shader locations
                view.shaderVarsToVertexAttribs, // the shader variable -> attrib map
                meshes[3]);                     // the actual mesh object

            view.objects.pop_back();
            view.objects.push_back(obj);
        }
    }
    else if ((codepoint == 'c' || codepoint == 'C'))
    {
        view.toggleDraw = !view.toggleDraw;
    }
}

void Controller::reshape(int width, int height)
{
    view.projection = glm::ortho(-800.0f, 800.0f, -800.0f, 800.0f);
    glViewport(0, 0, width, height);
}

void Controller::dispose()
{
    view.closeWindow();
}

void Controller::error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}
