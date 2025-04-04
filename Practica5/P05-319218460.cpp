/*
Pr�ctica 5: Optimizaci�n y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

//Importar Partes del auto
Model BaseAuto;
Model CofreAuto;
Model RuedaDerFroAuto;
Model RuedaIzqFroAuto;
Model RuedaDerTraAuto;
Model RuedaIzqTraAuto;



Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	BaseAuto = Model();
	BaseAuto.LoadModel("Models/BaseAuto.obj");
	CofreAuto = Model();
	CofreAuto.LoadModel("Models/CofreAuto.obj");
	RuedaDerFroAuto = Model();
	RuedaDerFroAuto.LoadModel("Models/RuedaDerFroAuto.obj");
	RuedaIzqFroAuto = Model();
	RuedaIzqFroAuto.LoadModel("Models/RuedaIzqFroAuto.obj");
	RuedaDerTraAuto = Model();
	RuedaDerTraAuto.LoadModel("Models/RuedaDerTraAuto.obj");
	RuedaIzqTraAuto = Model();
	RuedaIzqTraAuto.LoadModel("Models/RuedaIzqTraAuto.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.886f, 0.357f, 0.114f);//piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();


		//------------*INICIA DIBUJO DE NUESTROS DEM�S OBJETOS-------------------*
		//Base de auto
		color = glm::vec3(0.1f, 0.2f, 0.6f); //Cofre de audi azul y modelo de audi azul

		model = glm::mat4(1.0);
		modelaux = model;

		//Para avanzar
		float avanceZ = mainWindow.getavanzar();

		glm::mat4 modelBase = modelaux;
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, 0.0f, -1.5f));
		modelBase = glm::scale(modelBase, glm::vec3(0.1f, 0.1f, 0.1f));
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, 0.0f, avanceZ));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
		BaseAuto.RenderModel();

		model = modelaux;

		//Cofre de auto
		modelaux = model;


		glm::mat4 modelCofre = modelaux;
		modelCofre = glm::translate(modelCofre, glm::vec3(0.1f, 1.95f, 0.8f)); // Ajusta seg�n el modelo
		modelCofre = glm::scale(modelCofre, glm::vec3(0.1f, 0.1f, 0.1f));
		modelCofre = glm::translate(modelCofre, glm::vec3(0.0f, 0.0f, avanceZ));

		modelCofre = glm::rotate(modelCofre, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCofre));
		CofreAuto.RenderModel();



		model = modelaux;




		//Rueda delantera derecha
		modelaux = model;
		color = glm::vec3(0.05f, 0.05f, 0.05f); //Color de las llantas 

		glm::mat4 modelDerFro = modelaux;
		modelDerFro = glm::translate(modelDerFro, glm::vec3(-2.0f, 0.0f, 2.1f));
		modelDerFro = glm::scale(modelDerFro, glm::vec3(0.1f, 0.1f, 0.1f));
		modelDerFro = glm::translate(modelDerFro, glm::vec3(0.0f, 0.0f, avanceZ));

		modelDerFro = glm::rotate(modelDerFro, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDerFro));
		RuedaDerFroAuto.RenderModel();

		model = modelaux;

		//Rueda delantera izquierda
		modelaux = model;

		glm::mat4 modelIzqFro = model; // parte del cuerpo
		modelIzqFro = glm::translate(modelIzqFro, glm::vec3(2.0f, 0.0f, 2.1f));
		modelIzqFro = glm::scale(modelIzqFro, glm::vec3(0.1f, 0.1f, 0.1f));
		modelIzqFro = glm::translate(modelIzqFro, glm::vec3(0.0f, 0.0f, avanceZ));

		modelIzqFro = glm::rotate(modelIzqFro, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelIzqFro));
		RuedaIzqFroAuto.RenderModel();

		model = modelaux;

		//Rueda trasera derecha
		modelaux = model;

		glm::mat4 modelDerTra = model;
		modelDerTra = glm::translate(modelDerTra, glm::vec3(-2.0f, 0.0f, -5.6f));
		modelDerTra = glm::scale(modelDerTra, glm::vec3(0.1f, 0.1f, 0.1f));
		modelDerTra = glm::translate(modelDerTra, glm::vec3(0.0f, 0.0f, avanceZ));

		modelDerTra = glm::rotate(modelDerTra, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDerTra));
		RuedaDerTraAuto.RenderModel();

		model = modelaux;

		//Rueda trasera izquierda
		modelaux = model;
		glm::mat4 modelIzqTra = model;
		modelIzqTra = glm::translate(modelIzqTra, glm::vec3(2.0f, 0.0f, -5.6f));
		modelIzqTra = glm::scale(modelIzqTra, glm::vec3(0.1f, 0.1f, 0.1f));
		modelIzqTra = glm::translate(modelIzqTra, glm::vec3(0.0f, 0.0f, avanceZ));

		modelIzqTra = glm::rotate(modelIzqTra, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelIzqTra));
		RuedaIzqTraAuto.RenderModel();
		model = modelaux;




		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
