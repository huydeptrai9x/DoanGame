
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "math.h"
//#define Directive (C/C++)


//thiet lap mat san
const float FLOOR_HEIGHT = -8.4;		// Độ cao của mặt sàn
const int CHECK_SIZE = 5;		// Kích thước của mỗi ô vuông
const int GROUND_SIZE = 25;		// Kích thước của mặt sàn

int moveDirection;		// Hướng di chuyển hiện tại
float moveX;		// Vị trí di chuyển đến theo trục x
float moveZ;		// Vị trí di chuyển đến theo trụ z
float theta;		// Góc xoay của camera (tính tạo độ x và z)
float y;		// Tọa độ y của camera
float dTheta;		// Mức tăng/giảm theta khi điều khiển 
float dy;		// Mức tăng/giảm y khi điều khiển


void DrawGround() {
	int x, z;
	int counter = 0;

	for (x = -GROUND_SIZE; x < GROUND_SIZE; x += CHECK_SIZE) {
		for (z = -GROUND_SIZE; z < GROUND_SIZE; z += CHECK_SIZE) {
			if (counter % 2 == 0) {
				glColor3f(1.0, 0.0, 0.0);		// Tô màu do
			}
			else {
				glColor3f(1.0, 1.0, 1.0);		// Tô màu trắng
			}
			glBegin(GL_QUADS);
			glNormal3d(0, 1, 0);
			glVertex3f(x, FLOOR_HEIGHT, z);
			glVertex3f(x, FLOOR_HEIGHT, z + CHECK_SIZE);
			glVertex3f(x + CHECK_SIZE, FLOOR_HEIGHT, z + CHECK_SIZE);
			glVertex3f(x + CHECK_SIZE, FLOOR_HEIGHT, z);
			glEnd();
			counter++;
		}
		counter++;
	}
}

void drawBall()
{
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.0);
	glEnd();
}

void SettingCamera(float theta, float y) {
	gluLookAt(50 * sin(theta), y, 50 * cos(theta), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void Keyboard(unsigned char key, int, int) {
	switch (key) {
	case 'a':
		theta -= dTheta;
		break;
	case 'd':
		theta += dTheta;
		break;
	case 'w':
		y += dy;
		break;
	/*case 's':
		if (y > dy) {
			y -= dy;
		}
		break;*/
	case 's':
		y -= dy;
		break;
	}
	glutPostRedisplay();
}


void render() {
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //mau den
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f); //màu nền xanh lam nhạt
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();		// Reset modelview matrix
	SettingCamera(theta, y);

	
	glPushMatrix();
	drawBall();		// Vẽ trai banh
	glColor3f(0, 2, 0);
	glutSolidSphere(3, 80, 100); 
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	DrawGround();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void Init() {
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Máu ánh sáng ambient
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	// Màu ánh sáng diffuse
	GLfloat	diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Vị trí nguồn sáng
	GLfloat lightPos[] = { 2.0f, 25.0f, 25.0f, 25.0f };
	/*GLfloat lightPos[] = { 0.0, 0.0, 1.0, 0.0 };*/  //vị trí chiếu sáng từ mắt nhìn chiếu vào
	// Hướng chiếu sáng
	GLfloat spotDir[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Gán giá trị mặc định ban đầu cho camera
	theta = 0.0f;
	y = 5.0f;
	dTheta = 0.04f;
	dy = 1.0f;
}

//void Mouse(int posX, int posY)
//{
//	theta = -0.01 * posX;
//	y = 0.01 * posY;
//	
//	//dy = 1.0 * posX;
//	//dTheta = -0.1 * posY;
//}


void Reshape(int Width, int Height) {
	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)Width / (GLfloat)Height, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*gluLookAt(5.0, 5.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);*/
	gluLookAt(0.0, 0.0, 0.0, 5.0, 5.0, 5.0, 0.0, 1.0, 0.0);
}

void Idle() {
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 650);
	glutInitWindowPosition(80, 30);
	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	//glutPassiveMotionFunc(Mouse);
	Init();
	glutMainLoop();
	return 0;
}

