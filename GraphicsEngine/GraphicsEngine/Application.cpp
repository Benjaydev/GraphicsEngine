#pragma once
#include "Application.h"
#include <iostream>

Application* Application::instance;

bool Application::Startup()
{
    if (glfwInit() == false)
        return -1;

    window = glfwCreateWindow(windowWidth, windowHeight,
        "Computer Graphics",
        nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -3;
    }

    Gizmos::create(10000, 10000, 0, 0);

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST); // enables the depth buffer 

    applicationIsActive = true;
    instance = this;

    glfwSetCursorPosCallback(window, &Application::SetMousePosition);


    // Load vertex shader from file 
    shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
    // Load fragment shader from file 
    shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
    if (shader.link() == false) {
        printf("Shader Error: %s\n", shader.getLastError());
    }

    quadMesh.InitialiseFromFile("stanford/Bunny.obj");
    //quadMesh.InitialiseBox();
    // make the quad 10 units wide 
    quadTransform = {
     1,0,0,0,
     0,1,0,0,
     0,0,1,0,
     0,0,0,1 };

    lastTime = std::chrono::high_resolution_clock::now();
    return true;

}

void Application::Update()
{
    float deltaTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - lastTime).count()/1000;
    lastTime = std::chrono::high_resolution_clock::now();
    fps = (int)(1 / deltaTime);

    std::cout << fps << std::endl;

    glfwPollEvents();

    if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        applicationIsActive = false;
    }

    camera.Update(deltaTime, window);

    lastMousePosition = mousePosition;

}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Gizmos::clear();

    Gizmos::addTransform(glm::mat4(1));
    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i < 21; ++i) {
        Gizmos::addLine(vec3(-10 + i, 0, 10),
            vec3(-10 + i, 0, -10),
            i == 10 ? white : black);

        Gizmos::addLine(vec3(10, 0, -10 + i),
            vec3(-10, 0, -10 + i),
            i == 10 ? white : black);
    }

    glm::mat4 pv = camera.GetProjectionMatrix(windowWidth, windowHeight) * camera.GetViewMatrix();

    Gizmos::draw(pv);

    // bind shader 
    shader.bind();

    // bind transform 
    auto pvm = pv * quadTransform;
    shader.bindUniform("ProjectionViewModel", pvm);

    // draw quad 
    quadMesh.Draw();

    glfwSwapBuffers(window);

}

void Application::Shutdown()
{
    Gizmos::destroy();

    glfwDestroyWindow(window);
    glfwTerminate();
    applicationIsActive = false;
}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    instance->mousePosition.x = (float)x;
    instance->mousePosition.y = (float)y;
}
