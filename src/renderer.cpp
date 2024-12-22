#include "header/gfx.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "header/globals.h"
#include "header/shader.hpp"

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

namespace gfx {
void render(Global *g, Shader *lightingShader, Shader *lightCubeShader,
            Camera *camera, unsigned int cubeVAO, unsigned int lightCubeVAO) {
  if (g->renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  lightingShader->use();
  lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  glm::mat4 projection = glm::perspective(
      glm::radians(camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  lightingShader->setMat4("projection", projection);
  lightingShader->setMat4("view", view);

  glm::mat4 model = glm::mat4(1.0f);
  lightingShader->setMat4("model", model);

  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  lightCubeShader->use();
  lightCubeShader->setMat4("projection", projection);
  lightCubeShader->setMat4("view", view);
  model = glm::mat4(1.0f);
  model = glm::translate(model, lightPos);
  model = glm::scale(model, glm::vec3(0.2f));
  lightCubeShader->setMat4("model", model);

  glBindVertexArray(lightCubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);
  ui::render(true, g->show_demo_window, g->texture, g->sysMon);
}
} // namespace gfx
