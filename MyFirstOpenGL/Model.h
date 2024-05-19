#ifndef MODEL_H
#define MODEL_H
#include "Texture.h"
#include "ProgramManager.h"
#include "Camera.h"
#include "GameObject.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 880

enum ModelType { Troll = 0, Rock = 1, Cube=2 };
class Model : public GameObject {
public:
    Model(int IDProgram, const char* filePath,const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals, GLenum textureUnit, ModelType type);
    void Render(glm::mat4 view);
    


private:
    Texture* _texture;
    GLuint VAO, VBO, uvVBO, normalsVBO;
    ModelType _type;
    unsigned int numVertexs;
    int _programID;
    const char* _filePath;
};

#endif
