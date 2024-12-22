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
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

namespace gfx {
void render(Global *g, Shader *lightingShader, Shader *lightCubeShader,
            Camera *camera, unsigned int cubeVAO, unsigned int lightCubeVAO) {
  // pre stuff
  if (g->renderFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // rendering
  lightingShader->use();
  lightingShader->setVec3("viewPos", camera->Position);
  lightingShader->setFloat("material.shininess", 32.0f);

  // directional light
  lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
  // set point lights
  int i;
  for (i = 0; i < 4; i++) {
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "pointLights[%d].position", i);
    lightingShader->setVec3(buffer, pointLightPositions[i]);
    snprintf(buffer, sizeof(buffer), "pointLights[%d].ambient", i);
    lightingShader->setVec3(buffer, 0.2f, 0.2f, 0.2f);
    snprintf(buffer, sizeof(buffer), "pointLights[%d].diffuse", i);
    lightingShader->setVec3(buffer, 0.5f, 0.5f, 0.5f);
    snprintf(buffer, sizeof(buffer), "pointLights[%d].specular", i);
    lightingShader->setVec3(buffer, 1.0f, 1.0f, 1.0f);
    snprintf(buffer, sizeof(buffer), "pointLights[%d].constant", i);
    lightingShader->setFloat(buffer, 1.0f);
    snprintf(buffer, sizeof(buffer), "pointLights[%d].linear", i);
    lightingShader->setFloat(buffer, 0.09f);
    snprintf(buffer, sizeof(buffer), "pointLights[%d].quadratic", i);
    lightingShader->setFloat(buffer, 0.032f);
  }
  // spotLight
  lightingShader->setVec3("spotLight.position", camera->Position);
  lightingShader->setVec3("spotLight.direction", camera->Front);
  lightingShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  lightingShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  lightingShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  lightingShader->setFloat("spotLight.constant", 1.0f);
  lightingShader->setFloat("spotLight.linear", 0.09f);
  lightingShader->setFloat("spotLight.quadratic", 0.032f);
  lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
  lightingShader->setFloat("spotLight.outerCutOff",
                           glm::cos(glm::radians(15.0f)));

  // view/projection transformations
  glm::mat4 projection = glm::perspective(
      glm::radians(camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  lightingShader->setMat4("projection", projection);
  lightingShader->setMat4("view", view);

  // world transformation
  glm::mat4 model = glm::mat4(1.0f);
  lightingShader->setMat4("model", model);

  // bind diffuse map
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, g->texture[0]);
  // bind specular map
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, g->texture[1]);

  // render containers
  glBindVertexArray(cubeVAO);
  for (unsigned int i = 0; i < 10; i++) {
    // calculate the model matrix for each object and pass it to shader before
    // drawing
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    lightingShader->setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  // also draw the lamp object(s)
  lightCubeShader->use();
  lightCubeShader->setMat4("projection", projection);
  lightCubeShader->setMat4("view", view);

  // we now draw as many light bulbs as we have point lights.
  glBindVertexArray(lightCubeVAO);
  for (unsigned int i = 0; i < 4; i++) {
    model = glm::mat4(1.0f);
    model = glm::translate(model, pointLightPositions[i]);
    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
    lightCubeShader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  // ui
  ui::render(true, g->show_demo_window, g->texture, g->sysMon);
}
} // namespace gfx
