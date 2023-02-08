#include "View.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexAttribWithColor.h"

View::View()
{
}

View::~View()
{
}

void View::init(Callbacks *callbacks, vector<util::PolygonMesh<VertexAttribWithColor>> &meshes)
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window_dimensions = glm::vec2(800, 800);

    window = glfwCreateWindow(window_dimensions.x, window_dimensions.y, "Spirographs", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowUserPointer(window, (void *)callbacks);

    glfwSetCharCallback(window,
                        [](GLFWwindow *window, unsigned int codepoint)
                        {
                            reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window))->onkey(codepoint);
                        });

    glfwSetWindowSizeCallback(window,
                              [](GLFWwindow *window, int width, int height)
                              {
                                  reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window))->reshape(width, height);
                              });

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // create the shader program
    program.createProgram(string("shaders/default.vert"),
                          string("shaders/default.frag"));
    // assuming it got created, get all the shader variables that it uses
    // so we can initialize them at some point
    // enable the shader program
    program.enable();
    shaderLocations = program.getAllShaderVariables();

    // currently there are only two per-vertex attribute: position and color
    shaderVarsToVertexAttribs["vPosition"] = "position";
    shaderVarsToVertexAttribs["vColor"] = "color";

    for (int i = 0; i < meshes.size(); i++)
    {
        util::ObjectInstance *obj = new util::ObjectInstance("triangles");
        obj->initPolygonMesh<VertexAttribWithColor>(
            program,                   // the shader program
            shaderLocations,           // the shader locations
            shaderVarsToVertexAttribs, // the shader variable -> attrib map
            meshes[i]);                // the actual mesh object

        objects.push_back(obj);
    }

    // prepare the projection matrix for orthographic projection
    projection = glm::ortho(-800.0f, 800.0f, -800.0f, 800.0f);
    glViewport(0, 0, window_dimensions[0], window_dimensions[1]);

    frames = 0;
    time = glfwGetTime();

    radiusOC = 400.0f;
    speed = 0.5f;
    toggleDraw = false;
}

void View::display(int timeclock)
{
    glm::vec4 color;

    program.enable();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw outer circle
    {
        modelview = glm::mat4(1.0);
        modelview = modelview *
                    glm::scale(glm::mat4(1.0), glm::vec3(radiusOC, radiusOC, radiusOC)); // send modelview matrix to GPU
        glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
        // send projection matrix to GPU
        glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

        objects[0]->draw();
    }

    // draw inner circle
    {
        modelview = glm::mat4(1.0);

        modelview = modelview *
                    glm::rotate(glm::mat4(1.0), glm::radians((float)speed * timeclock), glm::vec3(0, 0, 1));

        modelview = modelview *
                    glm::translate(glm::mat4(1.0), glm::vec3((radiusOC - radiusIC), 0.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0), glm::radians((float)-speed * (radiusOC) / radiusIC * timeclock), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::scale(glm::mat4(1.0), glm::vec3(radiusIC, radiusIC, radiusIC)); // send modelview matrix to GPU
        glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
        // send projection matrix to GPU
        glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

        objects[1]->draw();
    }

    // draw seed point
    {
        modelview = glm::mat4(1.0);

        modelview = modelview *
                    glm::rotate(glm::mat4(1.0), glm::radians((float)speed * timeclock), glm::vec3(0, 0, 1));

        modelview = modelview *
                    glm::translate(glm::mat4(1.0), glm::vec3(radiusOC - (radiusIC), 0.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0), glm::radians((float)-speed * (radiusOC) / radiusIC * timeclock), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::translate(glm::mat4(1.0), glm::vec3((radiusIC / 2), 0.0f, 0.0f)) *
                    glm::scale(glm::mat4(1.0), glm::vec3(5.0f, 5.0f, 5.0f)); // send modelview matrix to GPU
        glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
        // send projection matrix to GPU
        glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

        objects[2]->draw();
    }

    // draw the curve
    if (toggleDraw)
    {
        modelview = glm::mat4(1.0);

        glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
        // send projection matrix to GPU
        glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

        objects[3]->draw();
    }

    glFlush();
    program.disable();

    glfwSwapBuffers(window);
    glfwPollEvents();
    frames++;
    double currenttime = glfwGetTime();
    if ((currenttime - time) > 1.0)
    {
        printf("Framerate: %2.0f\r", frames / (currenttime - time));
        frames = 0;
        time = currenttime;
    }
}

bool View::shouldWindowClose()
{
    return glfwWindowShouldClose(window);
}

void View::closeWindow()
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->cleanup();
        delete objects[i];
    }
    objects.clear();
    glfwDestroyWindow(window);

    glfwTerminate();
}
