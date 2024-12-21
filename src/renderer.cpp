#include "header/gfx.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "header/globals.h"

namespace gfx {
void render(Global *g, Shader *ourShader, unsigned int VAO) {
  glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                    (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

  if (g->renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
  trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

  ourShader->use();
  ourShader->setMat4("trans", trans);
  glBindTexture(GL_TEXTURE_2D, g->texture[0]);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  ui::render(true, g->show_demo_window, g->texture, g->sysMon);
}
} // namespace gfx
