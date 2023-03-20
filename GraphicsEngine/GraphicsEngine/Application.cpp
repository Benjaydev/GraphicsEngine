#pragma once
#include "Application.h"
#include <iostream>
#include "imgui_glfw3.h"
#include "Input.h"
#include <string>

Application* Application::instance;

bool Application::Startup()
{
    if (glfwInit() == false)
        return -1;

    window = glfwCreateWindow(windowWidth, windowHeight,
        "Graphics Engine",
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

    // Create input
    aie::Input::create();
    // Create gizmos
    Gizmos::create(10000, 10000, 0, 0);
    // Create GUI
    aie::ImGui_Init(window, true);

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST); // enables the depth buffer 

    applicationIsActive = true;
    instance = this;

    glfwSetCursorPosCallback(window, &Application::SetMousePosition);



    // Load shaders from file 
    shader.CompileShader("./shaders/simple.vert", "./shaders/simple.frag");
    phongShader.CompileShader("./shaders/phong.vert", "./shaders/phong.frag");
    normalMapShader.CompileShader("./shaders/normalmap.vert", "./shaders/phong.frag");

    Mesh* mesh = new Mesh();
    mesh->InitialiseFromFile("models/soulspear.obj");
    mesh->LoadMaterial("models/soulspear.mtl");
    glm::mat4 meshTransform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1 };

    scene = new Scene(glm::vec2(windowWidth, windowHeight), window);
    scene->AddInstance(new Instance(meshTransform, mesh, &normalMapShader));

    lastTime = std::chrono::high_resolution_clock::now();
    return true;

}

void Application::Update()
{

    float deltaTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - lastTime).count()/1000;
    lastTime = std::chrono::high_resolution_clock::now();
    fps = (int)(1 / deltaTime);

    glfwPollEvents();

    if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        applicationIsActive = false;
    }

    scene->Update(deltaTime);

    lastMousePosition = mousePosition;


}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Gizmos::clear();

    Gizmos::addTransform(glm::mat4(1));

    glm::mat4 pv = scene->camera->GetProjectionMatrix(windowWidth, windowHeight) * scene->camera->GetViewMatrix();


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

    Gizmos::draw(pv);

    scene->Draw();

    aie::ImGui_NewFrame();

    ImGui::Begin("FPS");
    ImGui::Text(("FPS: " + std::to_string(fps)).c_str());
    ImGui::End();

    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &scene->light.direction[0], 0.1f, -1.0f,
        1.0f);
    ImGui::Checkbox("Spin Sunlight", &scene->spinLight);
    ImGui::DragFloat3("Sunlight Colour", &scene->light.colour[0], 0.1f, 0.0f,
        2.0f);
    ImGui::End();

    ImGui::Render();

    glfwSwapBuffers(window);

}

void Application::Shutdown()
{
    aie::ImGui_Shutdown();

    Gizmos::destroy();
    delete scene;

    glfwDestroyWindow(window);
    glfwTerminate();
    applicationIsActive = false;
}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    instance->mousePosition.x = (float)x;
    instance->mousePosition.y = (float)y;
}
