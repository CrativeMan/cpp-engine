#include "header/camera.hpp"
#include "header/gfx.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "header/globals.h"
#include "header/shader.hpp"

namespace gfx {
void render(Shader *shader, Model *model, Skybox *skybox, Camera *camera,
            Window *window) {
  glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  glm::mat4 view = camera->GetViewMatrix();
  glm::mat4 projection = glm::perspective(
      glm::radians(camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 700.0f);
  shader->setMat4("model", modelMatrix);
  shader->setMat4("view", view);
  shader->setMat4("projection", projection);
  model->Draw(*shader);

  glDepthFunc(GL_LEQUAL);
  skybox->shader->use();
  view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
  skybox->shader->setMat4("view", view);
  skybox->shader->setMat4("projection", projection);
  // skybox cube
  glBindVertexArray(skybox->vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);

  snprintf(window->title, sizeof(window->title),
           "X:%.2f Y:%.2f Z:%.2f FOV:%.0f", camera->Position.x,
           camera->Position.y, camera->Position.z, camera->Zoom);
  glfwSetWindowTitle(window->id, window->title);
}
} // namespace gfx
