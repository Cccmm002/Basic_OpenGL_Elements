#include "stdio.h"
/*#include <gl/GL.h>
#include <gl/GLU.h>*/
#include <gl/glew.h>
#include <gl/glut.h>
#include <math.h>

#pragma comment(lib, "glew32.lib")

#define GL_PI 3.1415f

GLuint myProgObj;
const char *vertexShaderSource = "uniform vec3 lightposition;\n"\
"uniform vec4 lightcolor;\n"\
"uniform vec4 ambient;\n"\
"uniform float Ns;\n"\
"uniform float attenuation; \n"\
"varying vec4 color;\n"\
"void main()\n"\
"{\n"\
"	vec4 eyepos = gl_ModelViewMatrix * gl_Vertex;\n"\
"	vec3 eyeposition = eyepos.xyz;\n"\
"	vec3 ECPosition = vec3(gl_ModelViewMatrix * gl_Vertex);\n"\
"	vec3 N = normalize(0 - gl_NormalMatrix * gl_Normal);\n"\
"	vec3 L = normalize(lightposition - ECPosition);\n"\
"	vec3 V = normalize(eyeposition - ECPosition);\n"\
"	vec3 H = normalize(V + L);\n"\
"\n"\
"	vec4 diffuse4 = lightcolor * max(dot(N, L), 0);\n"\
"	vec3 diffuse = diffuse4.xyz;\n"\
"	vec4 specular4 = lightcolor * pow(max(dot(N, H), 0), Ns) * attenuation;\n"\
"	vec3 specular = specular4.xyz;\n"\
"\n"\
"	color = vec4(clamp((diffuse + specular), 0.0, 1.0), 1.0);\n"\
"	color = color + ambient;\n"\
"\n"\
"	gl_Position = ftransform();\n"\
"}";

const char *fragmentShaderSource = "varying vec4 color;\n"\
"void main()\n"\
"{\n"\
"	gl_FragColor = color;\n"\
"}";

GLint SCREEN_WIDTH = 0;
GLint SCREEN_HEIGHT = 0;

GLint windowWidth = 1366;
GLint windowHeight = 768;

GLfloat xRotAngle = 0.0f;
GLfloat yRotAngle = 0.0f;
GLfloat sizes[2];
GLfloat step;
GLfloat scale = 1.0f;

enum MODELS
{
	SPHERE = 0,
	CONE,
	CUBE,
	TORUS,
	DODECAHEDRON,
	TEAPOT,
	OCTAHEDRON,
	TETRAHEDRON,
	ICOSAHEDRON
};
GLint iModel = TEAPOT;
GLboolean bWire = GL_TRUE;

bool flagLight = false;
void TurnOnLight()
{
	if (!flagLight)
	{
		//Parameters of Light
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 50.0 };
		GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
		GLfloat surface_light[] = { 0.4, 0.4, 1.0, 1.0 };
		GLfloat Light_Model_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
		GLfloat ambient_Light[] = { 1.0, 1.0, 1.0, 1.0 };

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, surface_light);
		glLightfv(GL_LIGHT0, GL_SPECULAR, surface_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_Light);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);

		flagLight = true;
	}
}
void TurnOffLight()
{
	if (flagLight)
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);

		flagLight = false;
	}
}

bool flagShader = false;
void TurnOnShader()
{
	if (flagShader) return;

	myProgObj = glCreateProgram();
	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glAttachShader(myProgObj, vShader);
	glAttachShader(myProgObj, fShader);
	glShaderSource(vShader, 1, (const GLchar**)&vertexShaderSource, NULL);
	glShaderSource(fShader, 1, (const GLchar**)&fragmentShaderSource, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	glLinkProgram(myProgObj);
	
	flagShader = true;
}

void TurnOffShader()
{
	if (!flagShader) return;
	flagShader = false;
}

void processMenu(int value)
{
	switch (value)
	{
	case 1:
		iModel = SPHERE;
		break;
	case 2:
		iModel = CONE;
		break;
	case 3:
		iModel = CUBE;
		break;
	case 4:
		iModel = TORUS;
		break;
	case 5:
		iModel = DODECAHEDRON;
		break;
	case 6:
		iModel = TEAPOT;
		break;
	case 7:
		iModel = OCTAHEDRON;
		break;
	case 8:
		iModel = TETRAHEDRON;
		break;
	case 9:
		iModel = ICOSAHEDRON;
		break;
	case 10:
		bWire = GL_TRUE;
		break;
	case 11:
		bWire = GL_FALSE;
		break;
	case 12:
		TurnOffShader();
		TurnOnLight();
		break;
	case 13:
		TurnOffLight();
		break;
	case 14:
		TurnOffLight();
		TurnOnShader();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void renderScreen(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(1);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glPushMatrix();
	glRotatef(xRotAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotAngle, 0.0f, 1.0f, 0.0f);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);

	if (flagShader)
	{
		GLfloat shader_LightPos[] = { -10.0, -10.0, -10.0, 0.0 };
		GLfloat shader_LightColor[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat shader_Ambient[] = { 0.1, 0.1, 0.1, 1.0 };
		GLfloat Ns = 8.0;
		GLfloat attenuation = 0.1;

		glUseProgram(myProgObj);
		glUniform3f(glGetUniformLocation(myProgObj, "lightposition"), shader_LightPos[0], shader_LightPos[1], shader_LightPos[2]);
		glUniform4f(glGetUniformLocation(myProgObj, "lightcolor"), shader_LightColor[0], shader_LightColor[1], shader_LightColor[2], shader_LightColor[3]);
		glUniform4f(glGetUniformLocation(myProgObj, "ambient"), shader_Ambient[0], shader_Ambient[1], shader_Ambient[2], shader_Ambient[3]);
		glUniform1f(glGetUniformLocation(myProgObj, "Ns"), Ns);
		glUniform1f(glGetUniformLocation(myProgObj, "attenuation"), attenuation);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(-9.0f, 0.0f, 0.0f);
	glVertex3f(9.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -9.0f, 0.0f);
	glVertex3f(0.0f, 9.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -9.0f);
	glVertex3f(0.0f, 0.0f, 9.0f);
	glEnd();

	glPushMatrix();
	glTranslatef(9.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glutSolidCone(0.3, 0.6, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 9.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.3, 0.6, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 9.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glutSolidCone(0.3, 0.6, 10, 10);
	glPopMatrix();

	glScalef(scale, scale, scale);

	glColor3f(0.0f, 1.0f, 0.0f);
	switch (iModel)
	{
	case SPHERE:
		if (bWire)
			glutWireSphere(8.0f, 20, 20);
		else
			glutSolidSphere(8.0f, 20, 20);
		break;
	case CONE:
		if (bWire)
			glutWireCone(4.0f, 8.0f, 20, 20);
		else
			glutSolidCone(4.0f, 8.0f, 20, 20);
		break;
	case CUBE:
		if (bWire)
			glutWireCube(8.0f);
		else
			glutSolidCube(8.0f);
		break;
	case TORUS:
		if (bWire)
			glutWireTorus(3.0f, 6.0f, 20, 20);
		else
			glutSolidTorus(3.0f, 6.0f, 20, 20);
		break;
	case DODECAHEDRON:
		glScalef(6.0f, 6.0f, 6.0f);
		if (bWire)
			glutWireDodecahedron();
		else
			glutSolidDodecahedron();
		break;
	case TEAPOT:
		if (bWire)
			glutWireTeapot(8.0f);
		else
			glutSolidTeapot(8.0f);
		break;
	case OCTAHEDRON:
		glScalef(6.0f, 6.0f, 6.0f);
		if (bWire)
			glutWireOctahedron();
		else
			glutSolidOctahedron();
		break;
	case TETRAHEDRON:
		glScalef(6.0f, 6.0f, 6.0f);
		if (bWire)
			glutWireTetrahedron();
		else
			glutSolidTetrahedron();
		break;
	case ICOSAHEDRON:
		glScalef(6.0f, 6.0f, 6.0f);
		if (bWire)
			glutWireIcosahedron();
		else
			glutSolidIcosahedron();
		break;
	default:
		break;
	}

	glPopMatrix();
	glutSwapBuffers();
}

void setupRederingState(void)
{
	glClearColor(0.0f, 0.0, 0.0, 1.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
}
void changSize(GLint w, GLint h)
{
	GLfloat ratio;
	GLfloat coordinatesize = 10.0f;
	if ((w == 0) || (h == 0))
		return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ratio = (GLfloat)w / (GLfloat)h;
	//Ortho Project
	if (w<h)
		glOrtho(-coordinatesize, coordinatesize, -coordinatesize / ratio, coordinatesize / ratio, -coordinatesize, coordinatesize);
	else
		glOrtho(-coordinatesize*ratio, coordinatesize*ratio, -coordinatesize, coordinatesize, -coordinatesize, coordinatesize);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void specialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRotAngle -= 5.0f;
	else if (key == GLUT_KEY_DOWN)
		xRotAngle += 5.0f;
	else if (key == GLUT_KEY_LEFT)
		yRotAngle -= 5.0f;
	else if (key == GLUT_KEY_RIGHT)
		yRotAngle += 5.0f;
	else if (key == GLUT_KEY_PAGE_UP)
	{
		if (scale >= 1) scale += 0.5;
		else scale = scale * 2;
	}
	else if (key == GLUT_KEY_PAGE_DOWN)
	{
		if (scale >= 1) scale -= 0.5;
		else scale = scale / 2;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	int nModelMenu;
	int nWireMenu;
	int nMainMenu;
	int nLightMenu;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	glutCreateWindow("OpenGL");
	glutReshapeWindow(windowWidth, windowHeight);
	glutPositionWindow((SCREEN_WIDTH - windowWidth) / 2, (SCREEN_HEIGHT - windowHeight) / 2);
	nModelMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Sphere", 1);
	glutAddMenuEntry("Cone", 2);
	glutAddMenuEntry("Cube", 3);
	glutAddMenuEntry("Torus", 4);
	glutAddMenuEntry("Dodecahedron", 5);
	glutAddMenuEntry("Teapot", 6);
	glutAddMenuEntry("Octahedron", 7);
	glutAddMenuEntry("Tetrahedron", 8);
	glutAddMenuEntry("Icosahedron", 9);
	nWireMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Wire", 10);
	glutAddMenuEntry("Solid", 11);
	nLightMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Light on", 12);
	glutAddMenuEntry("Light off", 13);
	glutAddMenuEntry("Use shaders", 14);
	nMainMenu = glutCreateMenu(processMenu);
	glutAddSubMenu("Model", nModelMenu);
	glutAddSubMenu("WireOrSolid", nWireMenu);
	glutAddSubMenu("Light", nLightMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutReshapeFunc(changSize);
	glutDisplayFunc(renderScreen);
	glutSpecialFunc(specialKey);
	setupRederingState();
	GLenum err = glewInit();
	glutMainLoop();
	return 0;
}