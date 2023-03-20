#include "Instance.h"


Instance::Instance(glm::mat4 _transform, Mesh* _mesh, aie::ShaderProgram* _shader)
{
    transform = _transform;
    mesh = _mesh;
    shader = _shader;
}

void Instance::Draw(Scene* scene)
{
    // set the shader pipeline 
    shader->bind();

    // bind transform and other uniforms 
    auto pvm = scene->camera->GetProjectionMatrix(scene->windowSize.x, scene->windowSize.y)
        * scene->camera->GetViewMatrix() * transform;
    shader->bindUniform("ProjectionViewModel", pvm);

    shader->bindUniform("ModelMatrix", transform);
    shader->bindUniform("AmbientColour", scene->ambientLight);
    shader->bindUniform("LightColour", scene->light.colour);
    shader->bindUniform("LightDirection", scene->light.direction);

    shader->bindUniform("cameraPosition", scene->camera->GetPosition());
    mesh->ApplyMaterial(shader);
    // draw mesh 
    mesh->Draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
    return glm::translate(glm::mat4(1), position)
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
        * glm::scale(glm::mat4(1), scale);
}
