#include "ProgramManager.h"
#include "Texture.h"
#include "MyInputManager.h"
#include "Source.h"
#include "Camera.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


std::vector<Model*> models;
Texture* _texture;
void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {


	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
	glUniform2f(glGetUniformLocation(ProgramManager::getInstance().compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
}

//Funcion que leera un .obj y devolvera un modelo para poder ser renderizado
Model* LoadOBJModel(int IDProgram,const std::string& filePath, const char* texturefilePath, GLenum textureUnit, ModelType type) {

	//Verifico archivo y si no puedo abrirlo cierro aplicativo
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Variables lectura fichero
	std::string line;
	std::stringstream ss;
	std::string prefix;
	glm::vec3 tmpVec3;
	glm::vec2 tmpVec2;

	//Variables elemento modelo
	std::vector<float> vertexs;
	std::vector<float> vertexNormal;
	std::vector<float> textureCoordinates;

	//Variables temporales para algoritmos de sort
	std::vector<float> tmpVertexs;
	std::vector<float> tmpNormals;
	std::vector<float> tmpTextureCoordinates;

	//Recorremos archivo linea por linea
	while (std::getline(file, line)) {

		//Por cada linea reviso el prefijo del archivo que me indica que estoy analizando
		ss.clear();
		ss.str(line);
		ss >> prefix;

		//Estoy leyendo un vertice
		if (prefix == "v") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector de vertices los valores
			tmpVertexs.push_back(tmpVec3.x);
			tmpVertexs.push_back(tmpVec3.y);
			tmpVertexs.push_back(tmpVec3.z);
		}

		//Estoy leyendo una UV (texture coordinate)
		else if (prefix == "vt") {

			//Las UVs son siempre imagenes 2D asi que uso el tmpvec2 para almacenarlas
			ss >> tmpVec2.x >> tmpVec2.y;

			//Almaceno en mi vector temporal las UVs
			tmpTextureCoordinates.push_back(tmpVec2.x);
			tmpTextureCoordinates.push_back(tmpVec2.y);

		}

		//Estoy leyendo una normal
		else if (prefix == "vn") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector temporal de normales las normales
			tmpNormals.push_back(tmpVec3.x);
			tmpNormals.push_back(tmpVec3.y);
			tmpNormals.push_back(tmpVec3.z);

		}

		//Estoy leyendo una cara
		else if (prefix == "f") {

			int vertexData;
			short counter = 0;

			//Obtengo todos los valores hasta un espacio
			while (ss >> vertexData) {

				//En orden cada numero sigue el patron de vertice/uv/normal
				switch (counter) {
				case 0:
					//Si es un vertice lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec3, salto 1 / y aumento el contador en 1
					vertexs.push_back(tmpVertexs[(vertexData - 1) * 3]);
					vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 1]);
					vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 2]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 1:
					//Si es un uv lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec2, salto 1 / y aumento el contador en 1
					textureCoordinates.push_back(tmpTextureCoordinates[(vertexData - 1) * 2]);
					textureCoordinates.push_back(tmpTextureCoordinates[((vertexData - 1) * 2) + 1]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 2:
					//Si es una normal la almaceno - 1 por el offset y almaceno tres seguidos al ser un vec3, salto 1 / y reinicio
					vertexNormal.push_back(tmpNormals[(vertexData - 1) * 3]);
					vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
					vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
					counter = 0;
					break;
				}
			}
		}
	}

	return new Model(IDProgram,texturefilePath, vertexs, textureCoordinates, vertexNormal, textureUnit, type);
}

void main() {
	
	
	//Definir semillas del rand según el tiempo
	srand(static_cast<unsigned int>(time(NULL)));

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos función de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);

	//Indicamos lado del culling
	glCullFace(GL_BACK);

	//Indicamos lado del culling
	glEnable(GL_DEPTH_TEST);

	
	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {
		
		//Cargo Modelos
		GenerateCube();
		GenerateTroll();
		GenerateRocks();
		GenerateClouds();

		//Definimos color para limpiar el buffer de color
		glClearColor(0.251f, 0.878f, 0.816f, 1.0f);

		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();
			
			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			MyInputManager::getInstance().InputTransforms(window);
			if(MyInputManager::getInstance().getKey1Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::Orbit);
			}
			if (MyInputManager::getInstance().getKey1Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::FocusTroll1);
			}
			if (MyInputManager::getInstance().getKey2Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::FocusTroll2);
			}
			if (MyInputManager::getInstance().getKey3Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::FocusTroll3);
			}
			for (Model *model : models)
			{
				model->Render(Camera::getInstance().getViewMatrix());
				Camera::getInstance().Update();

			}
			
			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}
		glUseProgram(0);
		
		for (int i = 0; i < 4; i++) {
			glDeleteProgram(ProgramManager::getInstance().compiledPrograms[i]);
		}

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();

}

void GenerateCube()
{
	Model* cube1;

	cube1 = LoadOBJModel(0, "Assets/Models/cube.obj", "Assets/Textures/cube.png", GL_TEXTURE2, ModelType::Cube);
	cube1->_position = glm::vec3{ 0.f,-.1f,5.5f };
	cube1->_rotation = glm::vec3{ 90.f,0.f,0.f };
	cube1->_scale = glm::vec3{ 5.f,5.f,.1f };
	models.push_back(cube1);
}



void GenerateRocks()
{
	Model* rock1; Model* rock2; Model* rock3; Model* rock4; Model* rock5;

	rock1 = LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock1->_position = glm::vec3{0.7f,0.5f,4.5f};
	rock1->_rotation = glm::vec3{ 90.f,0.f,35.f };
	rock1->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock1); 

	rock2 = LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock2->_position = glm::vec3{-.7f,0.5f,4.5f};
	rock2->_rotation = glm::vec3{90.f,0.f,-35.f };
	rock2->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock2); 

	rock3 = LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock3->_position = glm::vec3{.7f,0.5f,3.f};
	rock3->_rotation = glm::vec3{ 90.f,0.f,-35.f };
	rock3->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock3);

	rock4 = LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock4->_position = glm::vec3{-.7f,0.5f,3.f};
	rock4->_rotation = glm::vec3{ 90.f,0.f,35.f };
	rock4->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock4);

	

	
}

void GenerateTroll()
{
	Model* troll1; Model* troll3; Model* troll2; Model* troll4;
	//Blue Troll
	troll1 = LoadOBJModel(1, "Assets/Models/troll.obj", "Assets/Textures/troll.png", GL_TEXTURE0, ModelType::Troll);
	troll1->_position = glm::vec3{ -3.f,0.f,3.f };
	troll1->_rotation = glm::vec3{ 0.f,90.f,0.f };
	troll1->_scale = glm::vec3{ 1.f,1.f,1.f };
	models.push_back(troll1);

	//Green Troll
	troll2 = LoadOBJModel(2, "Assets/Models/troll.obj", "Assets/Textures/troll.png", GL_TEXTURE0, ModelType::Troll);
	troll2->_position = glm::vec3{ 3.f,0.f,3.f };
	troll2->_rotation = glm::vec3{ 0.f,-90.f,0.f };
	troll2->_scale = glm::vec3{ 1.f,1.f,1.f };
	models.push_back(troll2);

	//Normal Troll
	troll3 = LoadOBJModel(0, "Assets/Models/troll.obj", "Assets/Textures/troll.png", GL_TEXTURE0, ModelType::Troll);
	troll3->_position = glm::vec3{ 0.f,0.f,6.f };
	troll3->_rotation = glm::vec3{ 0.f,180.f,0.f };
	troll3->_scale = glm::vec3{ 1.f,1.f,1.f };
	models.push_back(troll3);


	
}



void GenerateClouds()
{
	Model* cloud1; Model* cloud2;
	cloud1 = LoadOBJModel(3, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	cloud1->_position = glm::vec3{ 2.f,3.f,3.f };
	cloud1->_rotation = glm::vec3{ 90.f,0.f,90.f };
	cloud1->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(cloud1);

	cloud2 = LoadOBJModel(3, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	cloud2->_position = glm::vec3{ -2.f,3.f,7.f };
	cloud2->_rotation = glm::vec3{ 90.f,0.f,90.f };
	cloud2->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(cloud2);
}



