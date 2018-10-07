#version 330                                                                
layout (location = 0) in vec3 vPosition;                                                                       																	  
																  
out vec4 color;																 
uniform mat4 transform;

void main()                                                                    
{                                                                                
    gl_Position = transform * vec4(vPosition, 1.0); 
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}