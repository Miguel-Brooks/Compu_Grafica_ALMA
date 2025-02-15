#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h> // Biblioteca para rand()
#include <time.h>   // Biblioteca para srand()

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO[2], VBO[2], shader; //[2] por ser 2 figuras las cuales estaran conformadas por vertices, si quiero más tendre que modificar esto

int numVertices = 0; // Variable global para almacenar la cantidad de vértices

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,0.0f,1.0f);	//Si quiero editar el color de forma aleatoria colocar un switch en esta parte para las figuras 			\n\
}";



void CrearFiguras()
{
	GLfloat letra_A[] = {
		// Triángulo lateral izquierdo inferior izquierdo
		-1.0f, -0.5f, 0.0f,
		-0.9f,  0.0f, 0.0f,
		-0.76f, -0.0f, 0.0f,

		// Triángulo lateral izquierdo inferior izquierdo derecho
		-1.0f, -0.5f, 0.0f,
		-0.8f,  -0.5f, 0.0f,
		-0.76f, -0.0f, 0.0f,


		// Triángulo lateral derecho inferior derecho
		-0.7f, -0.5f, 0.0f,
		-0.74f,  0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,

		// Triángulo lateral derecho inferior derecho izquierdo
		-0.6f, -0.0f, 0.0f,
		-0.74f,  0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,

		//Partes superiores del triangulo de la sección 2
		-0.9f, -0.0f, 0.0f,
		-0.85f,  0.25f, 0.0f,
		-0.75f, 0.25f, 0.0f,
		
		//texto para separar las secciones
		-0.9f, -0.0f, 0.0f,
		-0.75f,  0.0f, 0.0f,
		-0.75f, 0.25f, 0.0f,

		//Partes superiores del triangulo de la sección 2
		-0.6f, -0.0f, 0.0f,
		-0.65f,  0.25f, 0.0f,
		-0.75f, 0.25f, 0.0f,

		//texto para separar las secciones
		-0.6f, -0.0f, 0.0f,
		-0.75f,  0.0f, 0.0f,
		-0.75f, 0.25f, 0.0f,


		//Partes superiores del triangulo de la sección 3
		-0.85f, 0.25f, 0.0f,
		-0.76f,  0.25f, 0.0f,
		-0.75f, 0.3f, 0.0f,

		//texto para separar las secciones
		-0.65f, 0.25f, 0.0f,
		-0.74f,  0.25f, 0.0f,
		-0.75f, 0.3f, 0.0f,


		//Partes superiores del triangulo de la sección 4
		-0.85f, 0.25f, 0.0f,
		-0.8f,  0.5f, 0.0f,
		-0.75f, 0.3f, 0.0f,

		//texto para separar las secciones
		-0.65f, 0.25f, 0.0f,
		-0.7f,  0.5f, 0.0f,
		-0.75f, 0.3f, 0.0f,

		//Partes superiores del triangulo de la sección 5
		-0.7f,  0.5f, 0.0f,
		-0.8f,  0.5f, 0.0f,
		-0.75f, 0.3f, 0.0f,
	};

	// Vértices de la letra L
	GLfloat letra_L[] = {
		//Partes  del triangulo de la sección 1
		-0.25f, -0.5f, 0.0f,
		-0.0f,  0.5f, 0.0f,
		-0.25f, 0.5f, 0.0f,

		//texto para separar las secciones
		-0.25f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
		-0.0f, -0.5f, 0.0f,

		//Partes del triangulo de la sección 2
		-0.0f, -0.25f, 0.0f,
		0.25f,  -0.5f, 0.0f,
		-0.0f, -0.5f, 0.0f,

		//texto para separar las secciones
		-0.0f, -0.25f, 0.0f,
		0.25f,  -0.5f, 0.0f,
		0.25f, -0.25f, 0.0f,
		
	};


	// Vértices de la letra M
	GLfloat letra_M[] = {
		//Partes del triangulo de la sección 1 izquierda
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		0.625f, -0.5f, 0.0f,

		//texto para separar las secciones
		0.625f, 0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		0.625f, -0.5f, 0.0f,

		//Partes del triangulo de la sección 1 derecha
		0.875f, 0.5f, 0.0f,
		0.875f,  -0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,

		//texto para separar las secciones
		1.0, -0.5f, 0.0f,
		0.875f,  -0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,


		//Partes del triangulo de la sección central inferior 
		0.625f, 0.125f, 0.0f,
		0.7f,  0.0f, 0.0f,
		0.7f, 0.125f, 0.0f,

		//texto para separar las secciones
		0.75f, 0.0625f, 0.0f,
		0.7f,  -0.0f, 0.0f,
		0.7f, 0.125f, 0.0f,

		//texto para separar las secciones
		0.75f, 0.0625f, 0.0f,
		0.7f,  0.0f, 0.0f,
		0.8f, 0.0f, 0.0f,

		//texto para separar las secciones
		0.75f, 0.0625f, 0.0f,
		0.8f,  0.125f, 0.0f,
		0.7f, 0.125f, 0.0f,

		//texto para separar las secciones
		0.75f, 0.0625f, 0.0f,
		0.8f,  0.125f, 0.0f,
		0.8f, 0.0f, 0.0f,

		//texto para separar las secciones
		0.875f, 0.125f, 0.0f,
		0.8f,  0.125f, 0.0f,
		0.8f, 0.0f, 0.0f,

		//Partes del triangulo de la sección central superior
		0.625f, 0.125f, 0.0f,
		0.875f,  0.125f, 0.0f,
		0.75f, 0.25f, 0.0f,

		//texto para separar las secciones
		0.625f, 0.125f, 0.0f,
		0.625f,  0.5f, 0.0f,
		0.75f, 0.25f, 0.0f,

		//texto para separar las secciones
		0.875f, 0.5f, 0.0f,
		0.875f,  0.125f, 0.0f,
		0.75f, 0.25f, 0.0f,

	};




	numVertices = (sizeof(letra_A) + sizeof(letra_L) + sizeof(letra_M)) / (3 * sizeof(GLfloat)); // Calcular la cantidad correcta de vértices



	glGenVertexArrays(1, VAO); // Generar 1 VAO
	glGenBuffers(1, VBO); // Generar 1 VBO

	glBindVertexArray(VAO[0]); // Asignar VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(letra_A) + sizeof(letra_L) + sizeof(letra_M), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(letra_A), letra_A);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(letra_A), sizeof(letra_L), letra_L);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(letra_A) + sizeof(letra_L), sizeof(letra_M), letra_M);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ejercicio practica 1", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}



	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Crear tríangulo
	CrearFiguras();
	CompileShaders();

	double lastTime = glfwGetTime();
	int colorIndex = 0; //Para declarar el color 


	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		//glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		double currentTime = glfwGetTime();
		if (currentTime - lastTime >= 2.0) { // Cambiar color cada 2 segundos
			int colorIndex = rand() % 3; // Elegir un número aleatorio entre 0 y 2

			if (colorIndex == 0) {
				glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Rojo
			}
			else if (colorIndex == 1) {
				glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // Verde
			}
			else {
				glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Azul
			}

			lastTime = currentTime;
		}

		
		glClear(GL_COLOR_BUFFER_BIT);



		glUseProgram(shader);

		glBindVertexArray(VAO[0]);  // Dibujar la A correctamente
		glDrawArrays(GL_TRIANGLES, 0, numVertices); // Dibuja todos los triángulos usando la cantidad correcta de vértices
		
		
		
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}