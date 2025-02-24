#version 330
layout (location =0) in vec3 pos;
//layout (location = 1) in vec3 vertexColor; //Nuevo
out vec4 vColor;
uniform mat4 model;
uniform mat4 projection;
uniform int colorIndex;
void main()
{
	gl_Position=projection*model*vec4(pos,1.0f);
	//vColor=vec4(color,1.0f);
	//vColor=vec4(clamp(pos,0.0f,1.0f),1.0f);
	
	if(colorIndex == 0)
        vColor = vec4(1.0, 0.0, 0.0, 1.0);           // Rojo
    else if(colorIndex == 1)
        vColor = vec4(0.0, 1.0, 0.0, 1.0);           // Verde claro (ventanas y puerta)
    else if(colorIndex == 2)
        vColor = vec4(0.0, 0.5, 0.0, 1.0);           // Verde oscuro (copa del árbol)
    else if(colorIndex == 3)
        vColor = vec4(0.0, 0.0, 1.0, 1.0);           // Azul
    else if(colorIndex == 4)
        vColor = vec4(0.478, 0.255, 0.067, 1.0);     // Café
    else
        vColor = vec4(1.0, 1.0, 1.0, 1.0);           // Blanco 
 
}

