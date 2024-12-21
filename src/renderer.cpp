#include "header/gfx.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "header/globals.h"
#include "header/shader.hpp"

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

namespace gfx {
void render(Global *g, Shader *shader, unsigned int VAO) {
  if (g->renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
  trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

  shader->use();
  shader->setMat4("view", view);
  shader->setMat4("projection", projection);

  glBindTexture(GL_TEXTURE_2D, g->texture[0]);
  glBindVertexArray(VAO);

  for (int i = 0; i < 10; i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    shader->setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  glBindVertexArray(0);
  ui::render(true, g->show_demo_window, g->texture, g->sysMon);
}
} // namespace gfx
