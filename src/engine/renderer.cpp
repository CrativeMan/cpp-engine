#include "header/camera.hpp"
#include "header/gfx.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <memory>

#include "header/globals.h"
#include "header/model.hpp"
#include "header/shader.hpp"

namespace gfx {
void render(Shader *shader, std::vector<std::unique_ptr<Model>> &models,
            Camera *camera, Window *window) {
  glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  glm::mat4 view = camera->GetViewMatrix();
  glm::mat4 projection = glm::perspective(
      glm::radians(camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  shader->setMat4("view", view);
  shader->setMat4("projection", projection);
  for (long unsigned int i = 0; i < models.size(); i++) {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, i * 6.0f));
    shader->setMat4("model", modelMatrix);
    models[i]->Draw(*shader);
  }

  snprintf(window->title, sizeof(window->title),
           "X:%.2f Y:%.2f Z:%.2f FOV:%.0f", camera->Position.x,
           camera->Position.y, camera->Position.z, camera->Zoom);
  glfwSetWindowTitle(window->id, window->title);
}
} // namespace gfx
