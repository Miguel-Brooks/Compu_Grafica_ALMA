/*Pr�ctica 4: Modelado Jer�rquico.
Se implementa el uso de matrices adicionales para almacenar informaci�n de transformaciones geom�tricas que se quiere 
heredar entre diversas instancias para que est�n unidas
Teclas de la F a la K para rotaciones de articulaciones, patas y cola 
Teclas de la U a la P para rotaciones de ruedas
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los v�rtices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(500, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CreateShaders();
	
	

	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		model = glm::mat4(1.0);



		/////////////////////////Base y ruedas/////////////////////////
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.1f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[4]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;

		///ruedas///

		modelaux = model;

		model = glm::translate(model, glm::vec3(0.8f, -1.2f, 0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;
		////separaci�n rueda
		model = glm::translate(model, glm::vec3(-0.8f, -1.2f, -0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));


		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;

		//Separacion rueda
		model = glm::translate(model, glm::vec3(0.8f, -1.2f, -0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));


		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;


		//Separacion rueda

		model = glm::translate(model, glm::vec3(-0.8f, -1.2f, 0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));


		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;



		//Cabina/////////////////////////



		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;


		//AQU� SE DIBUJA LA CABINA, LA BASE, LAS 4 LLANTAS

		// SE EMPIEZA EL DIBUJO DEL BRAZO
		//articulaci�n 1

		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		//rotaci�n alrededor de la articulaci�n que une con la cabina
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		sp.render();

		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		//para descartar la escala que no quiero heredar se carga la informaci�n de la matrix auxiliar
		model = modelaux;

		//articulaci�n 2
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//dibujar una peque�a esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux;

		//segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, -2.8f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

		model = modelaux;

		//articulaci�n 3 extremo derecho del segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		//dibujar una peque�a esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux;

		//Tercer brazo
		model = glm::translate(model, glm::vec3(-2.5f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

		model = modelaux;

		//articulaci�n 4 extremo derecho del segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;


		//dibujar una peque�a esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux;

		//Dibujo de pala de la grua 
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

		model = modelaux;





		///DIBUJADO DE NUEVA FIGURA
		//Aqu� debes utilizar de la articulazi�n 5 a 6 que seran letras j, k y l

		model = glm::mat4(1.0);

		modelaux = model;

		//Cuerpo perro
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));

		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.545f, 0.271f, 0.075f); //Cafe
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;

		//Cuello perro
		modelaux = model;

		model = glm::translate(model, glm::vec3(-0.5f, 1.0f, -4.0f));

		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.545f, 0.271f, 0.075f); //Cafe
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		model = modelaux;

		//Hocico perro

		model = glm::translate(model, glm::vec3(-1.0f, 2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.545f, 0.271f, 0.075f); //Cafe
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;

		//Cara perro
		model = glm::translate(model, glm::vec3(-0.5f, 2.5f, -4.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.545f, 0.271f, 0.075f); //Cafe
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;


		//Nariz perro

		model = glm::translate(model, glm::vec3(-2.0f, 2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;

		//Lengua metalica del perro robot

		model = glm::translate(model, glm::vec3(-1.8f, 1.6f, -4.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;

		//Ojo robotico
		model = glm::translate(model, glm::vec3(-1.0f, 2.5f, -4.0f));

		model = glm::scale(model, glm::vec3(0.4f, 0.3f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;

		//Oreja derecha
		model = glm::translate(model, glm::vec3(-0.5f, 3.0f, -4.3f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.3f, 0.8f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.545f, 0.271f, 0.075f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;

		//Oreja izquierda
		model = glm::translate(model, glm::vec3(-0.5f, 3.0f, -3.7f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.3f, 0.8f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.545f, 0.271f, 0.075f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;


		//Pata delantera izquierda
		//base

		glm::mat4 modelLeft = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.3f, -3.5f));

		modelLeft = glm::rotate(modelLeft, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 modelaux = modelLeft;

		modelLeft = glm::scale(modelLeft, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLeft));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//Tuerca

		modelLeft = glm::translate(modelLeft, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = modelLeft;
		modelLeft = glm::scale(modelLeft, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLeft));
		color = glm::vec3(0.f, 0.f, 0.f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		modelLeft = modelaux;



		//Pierna

		modelLeft = glm::translate(modelLeft, glm::vec3(0.0f, -1.0f, -1.0f));
		modelaux = modelLeft;

		modelLeft = glm::scale(modelLeft, glm::vec3(0.8f, 1.5f, 0.8f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLeft));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		modelLeft = modelaux;

		//Pie

		modelLeft = glm::rotate(modelLeft, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelLeft = glm::translate(modelLeft, glm::vec3(-1.0f, 0.5f, 0.0f));
		modelLeft = glm::scale(modelLeft, glm::vec3(1.5f, 2.5f, 1.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLeft));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[4]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		// Restaurar modelo antes de pasar a la siguiente pata
		modelLeft = modelaux;


		//Pata delantera derecha
		//base

		glm::mat4 modelRight = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.3f, -4.5f));

		modelRight = glm::rotate(modelRight, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = modelRight;

		modelRight = glm::scale(modelRight, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRight));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		modelRight = modelaux;

		//Tuerca

		modelRight = glm::translate(modelRight, glm::vec3(0.0f, 0.0f, -0.3f));
		modelaux = modelRight;
		modelRight = glm::scale(modelRight, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRight));
		color = glm::vec3(0.f, 0.f, 0.f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		modelRight = modelaux;



		//Pierna

		modelRight = glm::translate(modelRight, glm::vec3(0.0f, -0.3f, 0.4f));
		modelaux = modelRight;

		modelRight = glm::scale(modelRight, glm::vec3(0.25f, 0.5f, 0.25f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRight));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		modelRight = modelaux;

		//Pie

		modelRight = glm::rotate(modelRight, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelRight = glm::translate(modelRight, glm::vec3(-0.4f, 0.2f, 0.0f));
		modelRight = glm::scale(modelRight, glm::vec3(0.5f, 0.7f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRight));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[4]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		// Restaurar modelo antes de pasar a la siguiente pata
		modelRight = modelaux;

		
		// Pata trasera izquierda
		// base
		glm::mat4 modelBackLeft = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.3f, -3.5f));

		modelBackLeft = glm::rotate(modelBackLeft, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 modelAux = modelBackLeft;

		modelBackLeft = glm::scale(modelBackLeft, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackLeft));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// Tuerca
		modelBackLeft = glm::translate(modelBackLeft, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = modelBackLeft;
		modelBackLeft = glm::scale(modelBackLeft, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackLeft));
		color = glm::vec3(0.f, 0.f, 0.f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		modelBackLeft = modelaux;

		// Pierna
		modelBackLeft = glm::translate(modelBackLeft, glm::vec3(0.0f, -1.0f, -1.0f));
		modelaux = modelBackLeft;

		modelBackLeft = glm::scale(modelBackLeft, glm::vec3(0.8f, 1.5f, 0.8f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackLeft));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objeto
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		modelBackLeft = modelaux;

		// Pie
		modelBackLeft = glm::rotate(modelBackLeft, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelBackLeft = glm::translate(modelBackLeft, glm::vec3(-1.0f, 0.5f, 0.0f));
		modelBackLeft = glm::scale(modelBackLeft, glm::vec3(1.5f, 2.5f, 1.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackLeft));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objeto
		meshList[4]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		// Restaurar modelo antes de pasar a la siguiente pata
		modelBackLeft = modelaux;


		// Pata trasera derecha
		// base
		glm::mat4 modelBackRight = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.3f, -4.5f));

		modelBackRight = glm::rotate(modelBackRight, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = modelBackRight;

		modelBackRight = glm::scale(modelBackRight, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackRight));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		modelBackRight = modelaux;

		// Tuerca
		modelBackRight = glm::translate(modelBackRight, glm::vec3(0.0f, 0.0f, -0.3f));
		modelaux = modelBackRight;
		modelBackRight = glm::scale(modelBackRight, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackRight));
		color = glm::vec3(0.f, 0.f, 0.f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		modelBackRight = modelaux;

		// Pierna
		modelBackRight = glm::translate(modelBackRight, glm::vec3(0.0f, -0.3f, 0.4f));
		modelaux = modelBackRight;

		modelBackRight = glm::scale(modelBackRight, glm::vec3(0.25f, 0.5f, 0.25f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackRight));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objeto
		meshList[2]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		modelBackRight = modelaux;

		// Pie
		modelBackRight = glm::rotate(modelBackRight, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelBackRight = glm::translate(modelBackRight, glm::vec3(-0.4f, 0.2f, 0.0f));
		modelBackRight = glm::scale(modelBackRight, glm::vec3(0.5f, 0.7f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBackRight));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objeto
		meshList[4]->RenderMeshGeometry(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular

		// Restaurar modelo antes de pasar a la siguiente pata
		modelBackRight = modelaux;


		// Base de la cola 
		glm::mat4 modelTail = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -4.0f)); // Posicionamos la base de la cola
		modelTail = glm::rotate(modelTail, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotaci�n de la base
		glm::mat4 modelAux1 = modelTail; 

		
		modelTail = glm::scale(modelTail, glm::vec3(0.2f, 1.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTail));
		color = glm::vec3(0.545f, 0.271f, 0.075f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		modelTail = modelAux1;

		//Articulaci�n cola
		modelTail = glm::translate(modelTail, glm::vec3(0.0f, -1.0f, -0.0f));
		modelTail = glm::rotate(modelTail, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f)); 

		modelTail = glm::scale(modelTail, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTail));
		color = glm::vec3(0.75f, 0.75f, 0.75f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); 
        glm::mat4 modelAux2 = modelTail;

		//Punta cola
		
		modelTail = glm::translate(modelTail, glm::vec3(1.4f, -0.9f, 0.0f));
		modelTail = glm::rotate(modelTail, glm::radians(-120.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelTail = glm::scale(modelTail, glm::vec3(0.5f, 3.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTail));
		color = glm::vec3(0.545f, 0.271f, 0.075f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();
		modelTail = modelaux;



		
		
		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		