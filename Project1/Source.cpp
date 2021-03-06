#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <soil.h>
#include <string>
#include <fstream>
#include <vector>

#define M_PI       3.14159265358979323846

// angle of rotation for the camera direction
float angle = -91.5; //�������� ����
float VerticalAngle = 1.0f;
GLfloat xRotated, yRotated, zRotated;
GLdouble radius = 1;
// actual vector representing the camera's direction
float lx = 0.4f, lz = 1.0f; //������������ ������
// XZ position of the camera
float x = -0.4f, z = -1.0f, y = 2.0f;
float sizeofcube = 0.8f;
float YAngle = 0.0f;
float deltaAngle = 0.0f;
int xOrigin = -1;
int yOrigin = -1;
float ly = 0.0f;
int Turns = 0;

void selectColor(int id) {
	switch (id) { //����
		case 1: glColor3f(0.0, 0.0, 0.0); break; //������
		case 2: glColor3f(0.0, 0.0, 1.0); break; //�����
		case 3: glColor3f(1.0, 0.0, 0.0); break; //��������
		case 4: glColor3f(0.0, 1.0, 0.0); break; //�������
		case 5: glColor3f(1.0, 1.0, 1.0); break; //�����
		case 6: glColor3f(1.0, 1.0, 0.0); break; //������
	}
}

class �ube {
	public:
	float cubeX, cubeY, cubeZ;
	int color1 = 1; int color2 = 2;
	int color3 = 3; int color4 = 4;
	int color5 = 5; int color6 = 6;

	void MakeACube(float x, float y, float z) {
		cubeX = x; cubeY = y; cubeZ = z;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//glTranslatef(0, dimY / 2, 0);

		//glScalef(dimX / 2, dimY / 2, dimZ / 2);

		glBegin(GL_QUADS);

		selectColor(color1);
		glVertex3f(x, y, z);
		glVertex3f(x + sizeofcube, y, z);
		glVertex3f(x + sizeofcube, y, z + sizeofcube);
		glVertex3f(x, y, z + sizeofcube);

		selectColor(color2);
		glVertex3f(x, y + sizeofcube, z);
		glVertex3f(x + sizeofcube, y + sizeofcube, z);
		glVertex3f(x + sizeofcube, y + sizeofcube, z + sizeofcube);
		glVertex3f(x, y + sizeofcube, z + sizeofcube);

		selectColor(color3);
		glVertex3f(x, y, z + sizeofcube);
		glVertex3f(x + sizeofcube, y, z + sizeofcube);
		glVertex3f(x + sizeofcube, y + sizeofcube, z + sizeofcube);
		glVertex3f(x, y + sizeofcube, z + sizeofcube);

		selectColor(color4);
		glVertex3f(x, y, z);
		glVertex3f(x, y, z + sizeofcube);
		glVertex3f(x, y + sizeofcube, z + sizeofcube);
		glVertex3f(x, y + sizeofcube, z);

		selectColor(color5);
		glVertex3f(x + sizeofcube, y, z);
		glVertex3f(x + sizeofcube, y, z + sizeofcube);
		glVertex3f(x + sizeofcube, y + sizeofcube, z + sizeofcube);
		glVertex3f(x + sizeofcube, y + sizeofcube, z);

		selectColor(color6);
		glVertex3f(x + sizeofcube, y + sizeofcube, z);
		glVertex3f(x, y + sizeofcube, z);
		glVertex3f(x, y, z);
		glVertex3f(x + sizeofcube, y, z);

		glEnd();

		glPopMatrix();
	}
};

class ShaderUtil {
	private:
	unsigned int mProgramId;
	GLuint r_mod;
	unsigned int vs, fs;

	unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source) {
		unsigned int shader_id = glCreateShader(shader_type);

		const char* c_source = shader_source.c_str();
		glShaderSource(shader_id, 1, &c_source, nullptr);
		glCompileShader(shader_id);

		GLint result;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE) {
			std::cout << "Compile shader error" << std::endl;
			int length;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

			GLchar* strInfoLog = new GLchar[length + 1];
			glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

			fprintf(stderr, "Compilation error in shader: %s\n", strInfoLog);
			delete[] strInfoLog;
		}

		return shader_id;
	}

	public:
	ShaderUtil() {}
	~ShaderUtil() {}

	// Load a vertex and a fragment shader from file
	bool load() {

		const char* f_fs =
			"varying float x, y, z;"//This
			"uniform float r_mod;"
			"float rand(float s, float r) { return mod(mod(s, r + r_mod) * 112341, 1); }"
			//"float rand(float s, float r) { return mod(mod(s, r + r_mod) * 2341, 1); }"
			"void main() {"
			//"	gl_FragColor = vec4(rand(gl_FragCoord.x, x), rand(gl_FragCoord.y, y), rand(gl_FragCoord.z, z), 1);"
			"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
			//"	gl_FragColor = vec4(1.0, 1.0, 2.0, 3.0);"
			"}";

			//"out vec4 FragColor;\n" 
			//"in vec3 ourColor;\n"
			//"void main()\n"
			//"{\n"
			//"   FragColor = vec4(ourColor, 1.0f);\n"
			//"}\n\0";

			//"out vec4 FragColor;\n"
			//"in vec4 vertexColor;\n"
			//"void main()\n"
			//"{\n"
			//"   FragColor = vertexColor;\n"
			//"}\n\0";

			//"varying vec4 cords"
			//"uniform sampler2D gcolor"
			//"void main() {"
			//" vec3 color = texture2D(gcolor, cords.st).rgb"
			//"	gl_FragColor = vec4(color.rgb, 1.0f);"
			//"}";

		const char* f_vs =
			"varying float x, y, z;" //This
			"void main() {"
			"	gl_Position = ftransform();"
			"	x = gl_Position.x; y = gl_Position.y; z = gl_Position.z;"
			//"	x += y; y -= x; z += x - y;"
			//" gl_Position = gl_Vertex;"
			"}";

			//"layout (location = 0) in vec3 aPos;\n" 
			//"layout (location = 1) in vec3 aColor;\n"
			//"out vec3 ourColor;\n"
			//"void main()\n"
			//"{\n"
			//"   gl_Position = vec4(aPos, 1.0);\n"
			//"   ourColor = aColor;\n"
			//"}\0";

			//"layout(location = 0) in vec3 aPos;"
			//"out vec4 vertexColor; "
			//"void main()\n"
			//"{\n"
			//"   gl_Position = vec4(aPos, 1.0);\n"
			//"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0); \n"
			//"}\0";

			//"varying vec4 cords"
			//"void main() {"
			//"	gl_Position = ftransform();"
			//"	cords = gl_MultiTexCoord0;"
			//"}";

		vs = GetCompiledShader(GL_VERTEX_SHADER, f_vs);
		fs = GetCompiledShader(GL_FRAGMENT_SHADER, f_fs);

		if (vs && fs) {
			mProgramId = glCreateProgram();

			glAttachShader(mProgramId, vs);
			glAttachShader(mProgramId, fs);

			glLinkProgram(mProgramId);

			int linked = 0;
			glGetProgramiv(mProgramId, GL_LINK_STATUS, &linked);

			if (linked == GL_FALSE) {
				std::cout << "Shader error" << std::endl;
				int length = 0;
				glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &length);

				std::vector<char> info(length);
				glGetProgramInfoLog(mProgramId, length, &length, info.data());

				std::cout << std::string(info.begin(), info.end()) << std::endl;

				glDeleteShader(vs);
				glDeleteShader(fs);

				return false;
			}
			//r_mod = glGetUniformLocation(mProgramId, "r_mod");

			glDetachShader(mProgramId, vs);
			glDetachShader(mProgramId, fs);

			return true;
		}
		std::cout << "Shader error" << std::endl;
		return false;
	}

	bool load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
		std::ifstream is_vs(vertexShaderFile);
		const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

		std::ifstream is_fs(fragmentShaderFile);
		const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

		unsigned int vs = GetCompiledShader(GL_VERTEX_SHADER, f_vs);
		unsigned int fs = GetCompiledShader(GL_FRAGMENT_SHADER, f_fs);

		if (vs && fs) {
			mProgramId = glCreateProgram();

			glAttachShader(mProgramId, vs);
			glAttachShader(mProgramId, fs);

			glLinkProgram(mProgramId);

			int linked = 0;
			glGetProgramiv(mProgramId, GL_LINK_STATUS, &linked);

			if (linked == GL_FALSE) {
				std::cout << "Shader error" << std::endl;
				int length = 0;
				glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &length);

				std::vector<char> info(length);
				glGetProgramInfoLog(mProgramId, length, &length, info.data());

				std::cout << std::string(info.begin(), info.end()) << std::endl;

				glDeleteShader(vs);
				glDeleteShader(fs);

				return false;
			}
			r_mod = glGetUniformLocation(mProgramId, "r_mod");

			glDetachShader(mProgramId, vs);
			glDetachShader(mProgramId, fs);

			return true;
		}

		std::cout << "Shader error" << std::endl;
		return false;
	}

	// Use the program
	void init() {
		glUseProgram(mProgramId);
		r_mod = glGetUniformLocation(mProgramId, "r_mod");
	}

	// Delete the program
	void destroy() {
		//glDetachShader(mProgramId, vs);
		//glDetachShader(mProgramId, fs);
		glDeleteProgram(mProgramId);
	}
};

//�ube TestCube[3][3][3];
�ube TestCube;
ShaderUtil shaderUtil;

GLuint texture;
GLUquadric* MySphereSkyBox;

void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		} else {
			xOrigin = x;
		}
	}
}

void mouseMove(int x, int y) {
	// this will only be true when the left button is down
	if (xOrigin >= 0) {
		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;
		YAngle = (y - yOrigin) * 0.001f;
		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void SphereTextureMaking() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;
	unsigned char* image = SOIL_load_image("skybg2.png", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SOIL_free_image_data(image);
}

void SpaceBox() { //��������� ��������� -> ������� � shader.txt
	glColor3f(1, 1, 1);
	SphereTextureMaking();
	glBindTexture(GL_TEXTURE_2D, texture);
	gluQuadricDrawStyle(MySphereSkyBox, GLU_FILL);
	gluQuadricNormals(MySphereSkyBox, GLU_SMOOTH);
	gluSphere(MySphereSkyBox, 20, 20, 20);
	gluQuadricTexture(MySphereSkyBox, GLU_TRUE);
	glDeleteTextures(1, &texture);
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides) {
	const int numberOfVertices = 10 + 2;

	GLfloat twicePi = 2.0f * M_PI;

	GLfloat circleVerticesX[numberOfVertices];
	GLfloat circleVerticesY[numberOfVertices];
	GLfloat circleVerticesZ[numberOfVertices];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	GLfloat allCircleVertices[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawCircle() {
	glTranslatef(1.5f, 2.0f, 0.0f);

	glColor3f(0.9, 0.0, 0.9); // Red color used to draw.
	// changing in transformation matrix.
	// rotation about X axis
	glRotatef(xRotated, 1.0, 0.0, 0.0);

	glRotatef(yRotated, 0.0, 1.0, 0.0);// rotation about Y axis

	glRotatef(zRotated, 0.0, 0.0, 1.0);// rotation about Z axis

	glScalef(1.0, 1.0, 1.0);// scaling transfomation 
	// NOTE: built-in (glut library) function , draw you a sphere.
	glutSolidSphere(radius, 20, 20);
}


void display(void) {
	int i = 0; int j = 0; int k = 0;
	float axisX = 0.0f; float axisY = 0.8f; float axisZ = 0.8f;
	float XaxisX = 0.0f; float YaxisY = 0.4f; float ZaxisZ = 0.4f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SpaceBox(); //�������� �������� 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

	TestCube.MakeACube(1.0, 1.0, 1.0);

	//glTranslatef(-1.5f, 2.0f, 4.0f);
	//drawCircle(750 / 2, 750 / 2, 0, 120, 10);

	glTranslatef(-1.5f, 2.0f, 4.0f);
	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
			// Front
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Back
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, -1.0f, -1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left
	glColor3f(1.0f, 0.0f, 0.0f);       // Red
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);       // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);       // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();   // Done drawing the pyramid

	glTranslatef(1.5f, 2.0f, 4.0f);
	drawCircle();

	glFlush();

	glutSwapBuffers();
}

void init(void) {
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 1000);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void handleKeys(int key, int xx, int yy) {
	int color1T, color2T, color3T, color4T, color5T, color6T, color7T, color8T, color9T, color10T, color11T, color12T;
	float fraction = 0.3f;
	int i = 0; int j = 0;

	switch (key) {
		case GLUT_KEY_LEFT: { //������� ������ ������ 
			angle -= 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		}
		case GLUT_KEY_RIGHT: { //������� ������ �������� 
			angle += 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		}
		case GLUT_KEY_UP: { //��� ������ ������
			x += lx * fraction;
			z += lz * fraction;
			break;
		}
		case GLUT_KEY_DOWN: { //��� ������ �����
			x -= lx * fraction;
			z -= lz * fraction;
			break;
		}
		case GLUT_KEY_PAGE_UP: { //��� ������ �����
			y = y + 0.1;
			break;
		}
		case GLUT_KEY_PAGE_DOWN: { //��� ������ ������
			y = y - 0.1;
			break;
		}
		case GLUT_KEY_F1: { //������ ��� <-
			Turns += 1;
			color1T = TestCube.color6; //������ 

			color2T = TestCube.color5; //����� 

			color3T = TestCube.color3; //�������� 

			color4T = TestCube.color4; //������� 

			TestCube.color5 = color1T; //������ 

			TestCube.color3 = color2T; //����� 

			TestCube.color4 = color3T; //��������

			TestCube.color6 = color4T; //������� 
			break;
		}

		case GLUT_KEY_F2: { //������ �������� ^
			Turns += 1;
			color1T = TestCube.color3; //�������� 

			color2T = TestCube.color1; //������ 

			color3T = TestCube.color6; //������ 

			color4T = TestCube.color2; //�����  

			TestCube.color3 = color4T; //�����

			TestCube.color1 = color1T;	 //�������� 

			TestCube.color6 = color2T;	 //������ 

			TestCube.color2 = color3T;	 //������  
			break;
		}
	}
}

void idle(void) {
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(180, 40);
	glutCreateWindow("Cube");

	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0")) {
		fprintf(stderr, "GL 2.0 unsupported\n");
		return 1;
	}

	init();

	MySphereSkyBox = gluNewQuadric();

	//shaderUtil.load("vertex.shader", "fragment.shader");
	shaderUtil.load();

	glutDisplayFunc(display);
	glutSpecialFunc(handleKeys);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);

	std::cout << "Init" << std::endl;
	//shaderUtil.init();	//�� ������ ������� ��, ���� ���������� �������� ���� = ��� ��� [display()->SpaceBox()]

	glutMainLoop();

	std::cout << "Exit" << std::endl;
	shaderUtil.destroy();
	return 0;
}