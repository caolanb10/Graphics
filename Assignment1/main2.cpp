glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
glutInitWindowSize(800, 600);
glutCreateWindow("Hello Triangle");
// Tell glut where the display function is
glutDisplayFunc(display);
GLenum res = glewInit();

if (res != GLEW_OK) {
	fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	return 1;
}
// Set up your objects and shaders
	// Create 3 vertices that make up a triangle that fits on the viewport 
GLfloat vertices1[] = { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
						 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						 -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat vertices2[] = { 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
						1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

GLuint numVertices = 3;

// Set up the shaders
GLuint shaderProgramID = CompileShaders(pVS, pFS);
GLuint VAO2 = 0, VAO1 = 0, VBO1 = 0, VBO2 = 0;

glGenVertexArrays(1, &VAO1);
glGenBuffers(1, &VBO1);
glBindVertexArray(VAO1);
//glGenBuffers(1, &VBO);
// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
// Buffer will contain an array of vertices 
glBindBuffer(GL_ARRAY_BUFFER, VBO1);
// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
glBufferData(GL_ARRAY_BUFFER, numVertices * 6 * sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

linkCurrentBuffertoShader(shaderProgramID);
glBindVertexArray(VAO1);

glClear(GL_COLOR_BUFFER_BIT);
glDrawArrays(GL_TRIANGLES, 0, 3);


glGenVertexArrays(1, &VAO2);
glGenBuffers(1, &VBO2);
glBindVertexArray(VAO2);
//glGenBuffers(1, &VBO);
// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
// Buffer will contain an array of vertices 
glBindBuffer(GL_ARRAY_BUFFER, VBO2);
// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
glBufferData(GL_ARRAY_BUFFER, numVertices * 6 * sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
GLuint shaderProgramID2 = CompileShaders(pVS, pFS1);
linkCurrentBuffertoShader(shaderProgramID2);
glBindVertexArray(VAO2);

// Link the current buffer to the shader
// Begin infinite event loop
// NB: Make the call to draw the geometry in the currently activated vertex buffer. This is where the GPU starts to work!

glDrawArrays(GL_TRIANGLES, 0, 3);

glutSwapBuffers();
glutMainLoop();
return 0;