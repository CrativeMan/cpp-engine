#ifndef MODEL_HPP
#define MODEL_HPP

#include "fileHandler.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <cstring>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>

class Model {
public:
  std::vector<unsigned int> textureIds; // texutre ids for Debug utils
  std::vector<Mesh> meshes;
  std::string path;
  std::string file;
  unsigned int numVertices;
  unsigned int numIndices;

  Model(const char *path) {
    Logger::debug("Starting model load");
    numVertices = 0;
    numIndices = 0;
    this->path = path;
    this->file = file::getFileName(path);
    loadModel(path);
    Logger::info("Model", "Created model (mc: %d, tc: %d, dir:%s)",
                 meshes.size(), textures_loaded.size(), directory.c_str());
  }

  ~Model() {
    Logger::debug("Destroying model");
    for (Texture &texture : textures_loaded) {
      glDeleteTextures(1, &texture.id);
    }

    Logger::info("Model", "Destroyed model");
  }

  // Draws all meshes associated to model
  void Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(shader);
  }

private:
  std::vector<Texture> textures_loaded;
  std::string directory;

  void loadModel(std::string path) {
    Logger::info("Model", "Starting model loading");
    Assimp::Importer importer; // starts importer
    // reads obj model into aiScene (assimp high level scene)
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // check if model loaded correctly
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      Logger::error("ASSIMP", "%s", importer.GetErrorString());
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    // process nodes recursivly
    processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode *node, const aiScene *scene) {
    unsigned int i;
    // getting each mesh from node
    for (i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }

    for (i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  // turn mesh into our data type
  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int i;
    for (i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      // process vertex positions, normals and texture coordinates
      glm::vec3 vector;
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;

      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;

      if (mesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.TexCoords = vec;
      } else
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      vertices.push_back(vertex);
    }

    // process indices
    for (i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      std::vector<Texture> diffuseMaps = loadMaterialTextures(
          material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      std::vector<Texture> specularMaps = loadMaterialTextures(
          material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    numVertices += vertices.size();
    numIndices += indices.size();
    Logger::debug("Processed mesh %s (vc: %zu, ic:%zu)", mesh->mName.C_Str(),
                  vertices.size(), indices.size());
    return Mesh(vertices, indices, textures);
  }

  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName) {
    std::vector<Texture> textures;
    // loop over all the textures in the material
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str); // getting the texutre path
      bool skip = false;
      // check if texture has already been loaded
      for (unsigned int j = 0; j < textures_loaded.size(); j++) {
        if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0) {
          textures.push_back(textures_loaded[j]);
          skip = true;
          break;
        }
      }
      if (!skip) { // skip when already added
        Texture texture;
        texture.id = file::generateImage(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = str;
        textures.push_back(texture);
        textures_loaded.push_back(texture);
        textureIds.push_back(texture.id);
      }
    }
    return textures;
  }
};

#endif // MODEL_HPP