#include "Instance.h"


Instance::Instance(glm::mat4 _transform, Mesh* _mesh, aie::ShaderProgram* _shader)
{
    transform = _transform;
    mesh = _mesh;
    shader = _shader;
}

Instance::Instance(Mesh* _mesh, aie::ShaderProgram* _shader)
{
    mesh = _mesh;
    shader = _shader;
}

void Instance::Draw(Scene* scene, bool rebind)
{
    // Only bind to shader if needed. Used with shader batchingn to avoid repetitive rebinding
    if (rebind) {
        // Set the shader pipeline 
        shader->bind();
        shader->bindUniform("AmbientColour", scene->ambientLight);
        shader->bindUniform("LightColour", scene->sunLight.colour);
        shader->bindUniform("LightDirection", scene->sunLight.direction);
        shader->bindUniform("cameraPosition", scene->camera->GetPosition());
    }

    int numLights = scene->GetNumLights();
    if(numLights <= 4){
        shader->bindUniform("numLights", numLights);
        shader->bindUniform("PointLightPosition", numLights, scene->GetPointLightPositions());
        shader->bindUniform("PointLightColour", numLights, scene->GetPointLightColours());
    }
    else {
        //{distance, index}
        glm::vec2 closestLights[4];

        // Skip the first four
        for (int i = 0; i < numLights; i++) {
            float dist = SqrMagnitude(scene->GetPointLightPositions()[i], GetPosition());
            // First four are closest
            if (i <= 3) {
                closestLights[i] = glm::vec2(dist, i);
                continue;
            }


            // Search for highest distance in closest lights
            glm::vec2 highestIndex = glm::vec2(0,0);
            for (int j = 0; j < 4; j++) {
                if (closestLights[j][0] > highestIndex[0]) {
                    highestIndex = closestLights[j];
                }
            }
            // If the current checked light is lower than the highest
            if (dist < highestIndex[0]) {
                closestLights[(int)highestIndex[1]] = glm::vec2(dist, i);
            }
        }

        glm::vec2 closestPositions[4] = {
            scene->GetPointLightPositions()[(int)closestLights[0][1]],
            scene->GetPointLightPositions()[(int)closestLights[1][1]],
            scene->GetPointLightPositions()[(int)closestLights[2][1]],
            scene->GetPointLightPositions()[(int)closestLights[3][1]]
        };
        glm::vec2 closestColours[4] = {
           scene->GetPointLightColours()[(int)closestLights[0][1]],
           scene->GetPointLightColours()[(int)closestLights[1][1]],
           scene->GetPointLightColours()[(int)closestLights[2][1]],
           scene->GetPointLightColours()[(int)closestLights[3][1]]
        };
        shader->bindUniform("numLights", 4);
        shader->bindUniform("PointLightPosition", 4, closestPositions);
        shader->bindUniform("PointLightColour", 4, closestColours);
    }



    // Bind transform and other uniforms 
    auto pvm = scene->camera->GetProjectionMatrix(scene->windowSize.x, scene->windowSize.y)
        * scene->camera->GetViewMatrix() * transform;
    shader->bindUniform("ProjectionViewModel", pvm);
    shader->bindUniform("ModelMatrix", transform);





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

void Instance::SetTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
    transform = MakeTransform(position, eulerAngles, scale);
}

float Instance::SqrMagnitude(glm::vec3 pos1, glm::vec3 pos2)
{
    glm::vec3 newDiff = pos1 - pos2;
    return glm::dot(newDiff, newDiff);
}
