#include "ProgramManager.h"
#include "Texture.h"
#include "Engine.h"
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
		Engine::getInstance().Init();
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

			//We call engine update (inputs,deltatime...)
			Engine::getInstance().Update(window);

			//Inputs Logic
			if(Engine::getInstance().getKey1Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::Orbit);
			}
			if (Engine::getInstance().getKey1Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::FocusTroll1);
			}
			if (Engine::getInstance().getKey2Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::FocusTroll2);
			}
			if (Engine::getInstance().getKey3Pressed())
			{
				Camera::getInstance().setOrbit(CameraState::FocusTroll3);
			}
			//Render models
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

		//We delete programs to avoid overloaded info
		for (int i = 0; i < 4; i++) {
			glDeleteProgram(ProgramManager::getInstance().compiledPrograms[i]);
		}

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Terminate GLFW
	glfwTerminate();

}


//Generating models logic
void GenerateCube()
{
	//Floor with orange texture using porgram 0
	Model* cube1;
	cube1 = Engine::getInstance().LoadOBJModel(0, "Assets/Models/cube.obj", "Assets/Textures/cube.png", GL_TEXTURE2, ModelType::Cube);
	cube1->_position = glm::vec3{ 0.f,-.1f,5.5f };
	cube1->_rotation = glm::vec3{ 90.f,0.f,0.f };
	cube1->_scale = glm::vec3{ 5.f,5.f,.1f };
	models.push_back(cube1);
}



void GenerateRocks()
{

	//Rocks with its texture using porgram 0
	Model* rock1; Model* rock2; Model* rock3; Model* rock4; Model* rock5;
	rock1 = Engine::getInstance().LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock1->_position = glm::vec3{0.7f,0.5f,4.5f};
	rock1->_rotation = glm::vec3{ 90.f,0.f,35.f };
	rock1->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock1); 

	rock2 = Engine::getInstance().LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock2->_position = glm::vec3{-.7f,0.5f,4.5f};
	rock2->_rotation = glm::vec3{90.f,0.f,-35.f };
	rock2->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock2); 

	rock3 = Engine::getInstance().LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock3->_position = glm::vec3{.7f,0.5f,3.f};
	rock3->_rotation = glm::vec3{ 90.f,0.f,-35.f };
	rock3->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock3);

	rock4 = Engine::getInstance().LoadOBJModel(0, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	rock4->_position = glm::vec3{-.7f,0.5f,3.f};
	rock4->_rotation = glm::vec3{ 90.f,0.f,35.f };
	rock4->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(rock4);

	

	
}

void GenerateTroll()
{
	//Troll with its texture using porgram 1. Using a blue colour fragment shader modifcator
	Model* troll1; Model* troll3; Model* troll2; Model* troll4;

	troll1 = Engine::getInstance().LoadOBJModel(1, "Assets/Models/troll.obj", "Assets/Textures/troll.png", GL_TEXTURE0, ModelType::Troll);
	troll1->_position = glm::vec3{ -3.f,0.f,3.f };
	troll1->_rotation = glm::vec3{ 0.f,90.f,0.f };
	troll1->_scale = glm::vec3{ 1.f,1.f,1.f };
	models.push_back(troll1);

	//Troll with its texture using porgram 2. Using a green colour fragment shader modifcator
	troll2 = Engine::getInstance().LoadOBJModel(2, "Assets/Models/troll.obj", "Assets/Textures/troll.png", GL_TEXTURE0, ModelType::Troll);
	troll2->_position = glm::vec3{ 3.f,0.f,3.f };
	troll2->_rotation = glm::vec3{ 0.f,-90.f,0.f };
	troll2->_scale = glm::vec3{ 1.f,1.f,1.f };
	models.push_back(troll2);

	
	//Troll with its texture using porgram 0 
	troll3 = Engine::getInstance().LoadOBJModel(0, "Assets/Models/troll.obj", "Assets/Textures/troll.png", GL_TEXTURE0, ModelType::Troll);
	troll3->_position = glm::vec3{ 0.f,0.f,6.f };
	troll3->_rotation = glm::vec3{ 0.f,180.f,0.f };
	troll3->_scale = glm::vec3{ 1.f,1.f,1.f };
	models.push_back(troll3);

}



void GenerateClouds()
{
	//Clouds using rock geometry and its texture. Using porgram 3 to 
	Model* cloud1; Model* cloud2;
	cloud1 = Engine::getInstance().LoadOBJModel(3, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	cloud1->_position = glm::vec3{ 2.f,3.f,3.f };
	cloud1->_rotation = glm::vec3{ 90.f,0.f,90.f };
	cloud1->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(cloud1);

	cloud2 = Engine::getInstance().LoadOBJModel(3, "Assets/Models/rock.obj", "Assets/Textures/rock.png", GL_TEXTURE1, ModelType::Rock);
	cloud2->_position = glm::vec3{ -2.f,3.f,7.f };
	cloud2->_rotation = glm::vec3{ 90.f,0.f,90.f };
	cloud2->_scale = glm::vec3{ .6f,1.f,.7f };
	models.push_back(cloud2);
}



