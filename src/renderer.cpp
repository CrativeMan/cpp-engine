#include "header/gfx.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "header/globals.h"
#include "header/shader.hpp"

namespace gfx {
void render(Shader *shader, Model *model, Camera *camera, Window *window) {
  glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();

  // view/projection transformations
  glm::mat4 projection = glm::perspective(
      glm::radians(camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);

  // render the loaded model
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      glm::mat4 modelMatrix = glm::mat4(1.0f);
      modelMatrix = glm::translate(
          modelMatrix, glm::vec3(1.0f * (i * 4), 0.0f,
                                 1.0f * (j * 4))); // translate it down so it's
                                                   // at the center of the scene
      modelMatrix = glm::scale(
          modelMatrix,
          glm::vec3(
              1.0f, 1.0f,
              1.0f)); // it's a bit too big for our scene, so scale it down
      shader->setMat4("model", modelMatrix);
      model->Draw(*shader);
    }
  }

  snprintf(window->title, sizeof(window->title),
           "X:%.2f Y:%.2f Z:%.2f FOV:%.0f", camera->Position.x,
           camera->Position.y, camera->Position.z, camera->Zoom);
  glfwSetWindowTitle(window->id, window->title);
}
} // namespace gfx
