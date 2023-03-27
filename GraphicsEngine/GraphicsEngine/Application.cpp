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


    scene = new Scene(glm::vec2(windowWidth, windowHeight), window);
    // red light on the left
    scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
    // green light on the right
    scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));

    if (!scene->Start()) {
        return false;
    }

    // Load shaders from file 
    shader.CompileShader("./shaders/simple.vert", "./shaders/simple.frag");
    phongShader.CompileShader("./shaders/phong.vert", "./shaders/phong.frag");
    normalMapShader.CompileShader("./shaders/normalmap.vert", "./shaders/normalmap.frag");
    simpleKernalShader.CompileShader("./shaders/normalmap.vert", "./shaders/simplekernal.frag");

    distortShader.CompileShader("./shaders/post.vert", "./shaders/distort.frag");
    sobelEdgeShader.CompileShader("./shaders/post.vert", "./shaders/sobeledge.frag");
    boxBlurShader.CompileShader("./shaders/post.vert", "./shaders/boxblur.frag");
    postShader.CompileShader("./shaders/post.vert", "./shaders/post.frag");

    
    filters.push_back( Filter("No Filter", &postShader, nullptr));
    filters.push_back( Filter("Distortion", &distortShader, nullptr));
    filters.push_back( Filter("Sobel Edge", &sobelEdgeShader, nullptr));
    filters.push_back( Filter("Box Blur", &boxBlurShader, nullptr));

    for (int i = 0; i < 10; i++) {
        Mesh* mesh = new Mesh();
        mesh->InitialiseFromFile("models/soulspear.obj");
        mesh->LoadMaterial("models/soulspear.mtl");
        Instance* meshInst = new Instance(mesh, &normalMapShader);
        meshInst->SetTransform(vec3(-10 + (5*i), 0, 0), vec3(0), vec3(1));
        meshInst->BindToRenderTarget(&scene->renderTarget, 0);
        scene->AddInstance(meshInst);

    }


   /* Mesh* grid = new Mesh();
    grid->InitialiseQuad();
    grid->LoadDiffuseTexture("textures/numbered_grid.tga");
    Instance* gridInst = new Instance(grid, &normalMapShader);
    gridInst->SetTransform(vec3(0), vec3(0), vec3(1));
    scene->AddInstance(gridInst);*/

    for (int i = 0; i < filters.size(); i++) {
        Mesh* quad = new Mesh();
        quad->InitialiseFullscreenQuad();
        //quad->LoadDiffuseTexture("textures/numbered_grid.tga");
        Instance* filter = new Instance(quad, filters[i].shader);
        filter->SetTransform(vec3(0), vec3(0), vec3(1));
        filter->BindToRenderTarget(&scene->renderTarget, i, true);
        scene->AddInstance(filter);
        filter->isActive = false;
        filters[i].filter = filter;
    }


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
   

    scene->Draw();

    aie::ImGui_NewFrame();

    ImGui::Begin("FPS");
    ImGui::Text(("FPS: " + std::to_string(fps)).c_str());
    ImGui::End();

    ImGui::Begin("Point Lights");
    for (int i = 0; i < scene->GetNumLights(); i++) {
        std::string id = std::to_string(i + 1);
        ImGui::Text(("Light " + id).c_str());
        ImGui::DragFloat3(("Colour"+ id).c_str(), &scene->GetPointLights()[i].colour[0], 0.1f, 0.0f, 1);
        ImGui::DragFloat(("Intensity" + id).c_str(), &scene->GetPointLights()[i].intensity, 0.1f, 0.0f, 1000);
    }
    ImGui::Checkbox("Show Debug", &scene->showPointLightsDebug);
    ImGui::End();

    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &scene->sunLight.direction[0], 0.1f, -1.0f,
        1.0f);
    ImGui::Checkbox("Spin Sunlight", &scene->spinLight);
    ImGui::DragFloat3("Sunlight Colour", &scene->sunLight.colour[0], 0.1f, 0.0f,
        2.0f);
    ImGui::End();


    ImGui::Begin("Post Processing");
    for (int i = 0; i < filters.size(); i++) {
        ImGui::RadioButton(filters[i].name, &selectedPostProcess, i);
        filters[i].filter->isActive = false;
    }
    ImGui::End();
    filters[selectedPostProcess].filter->isActive = true;

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
