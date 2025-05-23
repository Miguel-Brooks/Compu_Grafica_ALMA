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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;


Model Blackhawk_M;



//Importar Partes del auto
Model BaseAuto;
Model CofreAuto;
Model RuedaDerFroAuto;
Model RuedaIzqFroAuto;
Model RuedaDerTraAuto;
Model RuedaIzqTraAuto;

//Importar lampara
Model lampara;

//Importar BMO
Model BMO;

//Dado
Texture dadoTexture;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


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

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDadoD8()
{
	unsigned int indices[] = {
		0, 1, 2,   // Cara 5
		3, 4, 5,   // Cara 3
		6, 7, 8,   // Cara 1
		9, 10,11,  // Cara 7
		12,13,14,  // Cara 4
		15,16,17,  // Cara 2
		18,19,20,  // Cara 6
		21,22,23   // Cara 8
	};

	GLfloat vertices[] = {
		// Cara 5
		//x		y		z		S		T			NX		NY		NZ
		0.0f,  1.0f,  0.0f,   0.75f, 1.0f,    1.0f, -1.0f, -1.0f,
	   -1.0f,  0.0f,  0.0f,   0.5f,  0.75f,   1.0f, -1.0f, -1.0f,
		0.0f,  0.0f,  1.0f,   0.98f,   0.75f,   1.0f, -1.0f, -1.0f,

		// Cara 3
		0.0f,  1.0f,  0.0f,   0.0f, 0.75,   -1.0f, -1.0f, -1.0f,
		0.0f,  0.0f,  1.0f,   0.24f,  0.5f,    -1.0f, -1.0f, -1.0f,
		1.0f,  0.0f,  0.0f,   0.49f,   0.75f,    -1.0f, -1.0f, -1.0f,

		// Cara 1
		0.0f,  1.0f,  0.0f,   0.5f, 0.75f,   -1.0f, -1.0f, 1.0f,
		1.0f,  0.0f,  0.0f,   0.25f,  0.5f,    -1.0f, -1.0f, 1.0f,
		0.0f,  0.0f, -1.0f,   0.75f,   0.5f,    -1.0f, -1.0f, 1.0f,

		// Cara 7
		0.0f,  1.0f,  0.0f,   0.98f, 0.74f,   1.0f, -1.0f, 1.0f,      //punta
		0.0f,  0.0f, -1.0f,   0.51f,   0.74f,    1.0f, -1.0f, 1.0f, //Izquierda
	   -1.0f,  0.0f,  0.0f,   0.75f,  0.5f,    1.0f, -1.0f, 1.0f,    //derecha

	   // Cara 4
	   0.0f, -1.0f,  0.0f,   0.5f, 0.25f,    1.0f, 1.0f, -1.0f,
	  -1.0f,  0.0f,  0.0f,   0.25f,  0.5f,   1.0f, 1.0f, -1.0f,
	   0.0f,  0.0f,  1.0f,   0.75f,   0.5f,   1.0f, 1.0f, -1.0f,

	   // Cara 2
	   0.0f, -1.0f,  0.0f,   0.0f, 0.25f,   -1.0f, 1.0f, -1.0f,
	   0.0f,  0.0f,  1.0f,   0.25f,  0.49f,    -1.0f, 1.0f, -1.0f,
	   1.0f,  0.0f,  0.0f,   0.49f,   0.25f,    -1.0f, 1.0f, -1.0f,

	   // Cara 6
	   0.0f, -1.0f,  0.0f,   0.98f, 0.25f,   -1.0f, 1.0f, 1.0f,		//punta
	   1.0f,  0.0f,  0.0f,   0.5f,   0.25f,    -1.0f, 1.0f, 1.0f,	//Izquierda
	   0.0f,  0.0f, -1.0f,   0.75f,  0.49f,    -1.0f, 1.0f, 1.0f,	//derecha

	   // Cara 8
	   0.0f, -1.0f,  0.0f,   0.75f, 0.0f,    1.0f, 1.0f, 1.0f,
	   0.0f,  0.0f, -1.0f,   0.98f,  0.25f,   1.0f, 1.0f, 1.0f,
	  -1.0f,  0.0f,  0.0f,   0.5f,   0.25f,   1.0f, 1.0f, 1.0f
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(vertices, indices, sizeof(vertices) / sizeof(GLfloat), sizeof(indices) / sizeof(unsigned int));
	meshList.push_back(dado);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDadoD8();
	CreateShaders();

	camera = Camera(glm::vec3(-10.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	//Importar auto
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

	//Importar lampara
	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");

	//Importar BMO
	BMO = Model();
	BMO.LoadModel("Models/BMO.obj");

	//Dado
	dadoTexture = Texture("Textures/Dado8Caras.png");
	dadoTexture.LoadTextureA();

	//Skybox
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f); //Iluminaci�n del sol
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f); //No tocar
	pointLightCount++;


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija faro derecho
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, //Color
		1.0f, 2.5f,                             //ambient, diffuse intensity
		1.9f, 1.15f, 3.0f,					 //posici�n	inicial 														
		0.0f, 0.0f, 1.0f,                  //direcci�n
		1.0f, 0.0f, 0.0f,                   //atenuaci�n
		12.0f);                            //�ngulo de luz (borde del cono)
	spotLightCount++;

	//luz fija faro izquierdo
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f, //Color
		1.0f, 2.5f,                             //ambient, diffuse intensity
		-1.9f, 1.15f, 3.0f,					 //posici�n	inicial 														
		0.0f, 0.0f, 1.0f,                  //direcci�n
		1.0f, 0.0f, 0.0f,                   //atenuaci�n
		12.0f);                            //�ngulo de luz (borde del cono)
	spotLightCount++;

	//Luz del cofre
	spotLights[3] = SpotLight(0.5f, 0.5f, 0.0f, //Color
		1.0f, 2.5f,                             //ambient, diffuse intensity
		0.0f, 1.7f, 3.0f,					 //posici�n	inicial 														
		0.0f, -1.0f, 0.0f,                  //direcci�n
		1.0f, 0.0f, 0.0f,                   //atenuaci�n
		25.0f);                            //�ngulo de luz (borde del cono)
	spotLightCount++;



	//luz fija helicoptero
	spotLights[4] = SpotLight(1.0f, 1.0f, 0.0f, //Color
		1.0f, 2.5f,                             //ambient, diffuse intensity
		-1.3f, 5.0f, 6.0f,					 //posici�n	inicial 														
		0.0f, -1.0f, 0.0f,                  //direcci�n
		1.0f, 0.0f, 0.0f,                   //atenuaci�n
		12.0f);                            //�ngulo de luz (borde del cono)
	spotLightCount++;


	//luz fija lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, //Color
		1.0f, 1.0f,                             //ambient, diffuse intensity
		10.0f, 13.7f, 11.9,					 //posici�n	inicial 														
		0.3f, 0.2f, 0.1f);                   //atenuaci�n
	pointLightCount++;

	//luz fija faro derecho trasero
	spotLights[5] = SpotLight(1.0f, 0.0f, 0.0f, //Color
		1.0f, 2.5f,                             //ambient, diffuse intensity
		1.9f, 1.7f, -6.0f,					 //posici�n	inicial 														
		0.0f, 0.0f, -1.0f,                  //direcci�n
		1.0f, 0.0f, 0.0f,                   //atenuaci�n
		12.0f);                            //�ngulo de luz (borde del cono)
	spotLightCount++;

	//luz fija faro izquierdo trasero
	spotLights[6] = SpotLight(1.0f, 0.0f, 0.0f, //Color
		1.0f, 2.5f,                             //ambient, diffuse intensity
		-1.9f, 1.7f, -6.0f,					 //posici�n	inicial 														
		0.0f, 0.0f, -1.0f,                  //direcci�n
		1.0f, 0.0f, 0.0f,                   //atenuaci�n
		12.0f);                            //�ngulo de luz (borde del cono)
	spotLightCount++;

	//luz fija BMO
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f, //Color
		0.5f, 0.4f,                             //ambient, diffuse intensity
		-10.0f, 0.7f, 6.0,					 //posici�n	inicial 														
		0.6f, 0.2f, 0.1f);                   //atenuaci�n                           //�ngulo de luz (borde del cono)
	pointLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);





		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); //Luz se mueva respecto a la camara

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//Dado de Opengl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();


		model = glm::mat4(1.0f);
		float avanceZ = mainWindow.getavanzar();

		// movimiento global del coche
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, avanceZ));
		modelaux = model;

		// Obtener posici�n global del faro
		glm::vec3 posicionGlobalFaroDer = glm::vec3(modelaux * glm::vec4(1.9f, 1.15f, 3.0f, 1.0f));
		// Direcci�n global hacia adelante
		glm::vec3 direccionGlobalFaro = glm::vec3(modelaux * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		// Actualizar la luz en cada frame
		
		bool* keys = mainWindow.getsKeys();
		if (keys[GLFW_KEY_G]) {
			spotLights[1].SetFlash(posicionGlobalFaroDer, direccionGlobalFaro);//Encender luz
		}
		else {
			spotLights[1].SetFlash(glm::vec3(0.0f), glm::vec3(0.0f));// Apagar luz
		}

		//Luz de reversa derecho 
		// Obtener posici�n global del faro
		glm::vec3 posicionGlobalFaroDerTra = glm::vec3(modelaux * glm::vec4(1.9f, 1.7f, -6.0f, 1.0f));
		// Direcci�n global hacia adelante
		glm::vec3 direccionGlobalFaroTra = glm::vec3(modelaux * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
		// Actualizar la luz en cada frame

		if (keys[GLFW_KEY_T]) {
			spotLights[5].SetFlash(posicionGlobalFaroDerTra, direccionGlobalFaroTra);//Encender luz
		}
		else {
			spotLights[5].SetFlash(glm::vec3(0.0f), glm::vec3(0.0f));// Apagar luz
		}


		// Obtener posici�n global del faro
		glm::vec3 posicionGlobalFaroIzq = glm::vec3(modelaux * glm::vec4(-1.9f, 1.15f, 3.0f, 1.0f));
		// Actualizar la luz en cada frame
		if (keys[GLFW_KEY_G]) {
			spotLights[2].SetFlash(posicionGlobalFaroIzq, direccionGlobalFaro);//Encender luz
		}
		else {
			spotLights[2].SetFlash(glm::vec3(0.0f), glm::vec3(0.0f));// Apagar luz
		}

		//Luz de reversa izquierdo
		// Obtener posici�n global del faro
		glm::vec3 posicionGlobalFaroIzqTra = glm::vec3(modelaux * glm::vec4(-1.9f, 1.7f, -6.0f, 1.0f));
		// Actualizar la luz en cada frame
		if (keys[GLFW_KEY_T]) {
			spotLights[6].SetFlash(posicionGlobalFaroIzqTra, direccionGlobalFaroTra);//Encender luz
		}
		else {
			spotLights[6].SetFlash(glm::vec3(0.0f), glm::vec3(0.0f));// Apagar luz
		}




		// Base del coche
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, 0);
		BaseAuto.RenderModel();

		// Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.1f, 1.85f, 0.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		//Luz del cofre
		glm::vec3 offsetLuzCofre = glm::vec3(0.0f, 1.792f, 3.0f); //Punto inicial de la luz
		glm::vec3 posGlobalCofre = glm::vec3(modelaux * glm::vec4(offsetLuzCofre, 1.0f));
		glm::vec3 dirGlobalCofre = glm::vec3(model * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
		spotLights[3].SetFlash(posGlobalCofre, dirGlobalCofre);
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, 0);
		CofreAuto.RenderModel();




		// Rueda delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 2.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaDerFroAuto.RenderModel();

		// Rueda delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaIzqFroAuto.RenderModel();

		// Rueda trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -5.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaDerTraAuto.RenderModel();

		// Rueda trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, -5.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaIzqTraAuto.RenderModel();


		//Helicoptero
		model = glm::mat4(1.0);
		// movimiento global del Helicoptero
		float avanceX = mainWindow.getavanzarH();

		model = glm::translate(model, glm::vec3(avanceX, 0.0f, 0.0f));
		modelaux = model;

		// Obtener posici�n global del faro
		glm::vec3 offsetFaroH = glm::vec3(-1.3f, 5.0f, 6.0f);
		glm::vec3 posicionGlobalH = glm::vec3(modelaux * glm::vec4(offsetFaroH, 1.0f));
		// Direcci�n global hacia adelante
		glm::vec3 direccionGlobalH = glm::vec3(modelaux * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
		// Actualizar la luz en cada frame
		spotLights[4].SetFlash(posicionGlobalH, direccionGlobalH);


		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.9f, 6.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		if (mainWindow.getLuzLamp() == 1)
		{
			pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
				1.0f, 1.0f,                          
				10.0f, 13.7f, 11.9,								
				0.3f, 0.2f, 0.1f); //Encender lampara
		}
		else if (mainWindow.getLuzLamp() == 0)
		{
			pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, 
				0.0f, 0.0f,                            
				10.0f, 13.7f, 11.9,																		
				0.3f, 0.2f, 0.1f); //Apagar lampara
		}


		//BMO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.7f, 6.0));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BMO.RenderModel();

		if (mainWindow.getLuzLamp2() == 1)
		{
			pointLights[2] = PointLight(1.0f, 1.0f, 1.0f, 
				0.5f, 0.4f,                           
				-10.0f, 0.7f, 6.0,					
				0.6f, 0.2f, 0.1f);  //Encender lampara
		}
		else if (mainWindow.getLuzLamp2() == 0)
		{
			pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
				0.0f, 0.0f,                            
				-10.0f, 0.7f, 6.0,					 													
				0.6f, 0.2f, 0.1f); //Apagar lampara
		}


		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		//Desaparece
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));



		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
