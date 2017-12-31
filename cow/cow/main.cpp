#include <cstdio>
#include <cstdlib>
#include <vector>
#include <windows.h>
#include <GL\glut.h>
#include <cstring>
#include <cmath>
#include "button.h"
#include "Texture.h"
#include "obj.h"
		 
using std::vector;
using std::pair;

#define MAX_PATH_NAME 128
Obj cow;
button* hint;

void init() {
	//������obj
	char localPath[MAX_PATH_NAME];
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "cow.obj");
	cow.init(localPath);

	//����hint����
	hint = new button(-cow.size, -cow.size, cow.size, cow.size * 35 / 113);
	hint->setTex(1);
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "set.bmp");
	hint->setTexImage(localPath);
}

GLfloat windowLeft, windowDown;
GLfloat size_change = 1.0;
void reshape(int w, int h)
{//�Զ������´��ڵĴ�С�����أ�
	glViewport(0, 0, w, h);//�ڴ�������ʾ��������ʾ�������½�������ʾ����Ŀ����߶�
	glMatrixMode(GL_PROJECTION);//������Ҫ����ͶӰ���ò���
	glLoadIdentity();//���ñ任����
	
	//ʹ������ͶӰ�۲����壬�۲췽ʽ��ƽ���޼��ռ�
	if (w <= h) {//����ʹ���ۿ�߱���Σ�ʼ�չ۲촰����ô���һ������
		glOrtho(-cow.size, cow.size, -cow.size*(GLfloat)h / (GLfloat)w, cow.size*(GLfloat)h / (GLfloat)w, 0, cow.size * 2);
		windowLeft = -cow.size;
		windowDown = -cow.size*(GLfloat)h / (GLfloat)w;
		//����۲�ռ�ģ����ҡ��¡��ϡ�ǰ����
		size_change *= -(GLfloat)h / (GLfloat)w;
	}
	else {
		glOrtho(-cow.size*(GLfloat)w / (GLfloat)h, cow.size*(GLfloat)w / (GLfloat)h, -cow.size, cow.size, 0, cow.size * 2);
		windowLeft = -cow.size*(GLfloat)w / (GLfloat)h;
		windowDown = -cow.size;
		size_change *= -(GLfloat)w / (GLfloat)h;
	}
	//gluPerspective(90.0f, (GLdouble)w / (GLdouble)h, 1.0f, 1.0f);
	//Ҳ����ʹ��͸��ͶӰͶӰ�۲���
	//gluPerspective(90, w / h, 100, 600);//�����������Ҫ����
	//����۲�ռ�Ĵ�ֱ�Ƕȣ���߱ȣ�ǰ���棬�����

	glMatrixMode(GL_MODELVIEW);//�����������Ӿ����ò���
	glLoadIdentity();//���ñ任����
	//ps.��������GL_TEXTUREʱ���Խ����������
}

void setLight() {//��ӹ���
	GLfloat lmodel_ambient[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	GLfloat light_position[] = { 1, 1, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat light[] = { 1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
}


void showHint() {//��ʾ��ʾ
	glPushMatrix();
	//glTranslatef(cow.size, 0, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	hint->show(windowLeft, windowDown);
	glPopMatrix();
}

enum Model { cow_TEX1, cow_TEX2, cowTEX3, cow_SLOID, cow_LINE, cow_POINT }model;
vector<pair<int, GLfloat>> rotateLog;
const GLfloat axis[3][3] = { { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };
GLfloat bigger = 1.0;

void display()
{
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����ά���棿
	glMatrixMode(GL_MODELVIEW);//�����������Ӿ����ò���
	glLoadIdentity();//���ñ任����
	gluLookAt(cow.size, 0, 0, 0, 0, 0, 0, 0, 1);//�������xyz���۲���xyz������Ϸ����xyz
	setLight();//���ù���
	glPushMatrix();
	for (int i = rotateLog.size() - 1; i >= 0; i--)
		glRotatef(rotateLog[i].second, axis[rotateLog[i].first][0], axis[rotateLog[i].first][1], axis[rotateLog[i].first][2]);
	//������ת
	glTranslatef(0, 0, -cow.size*0.5);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(bigger, bigger, bigger);//���÷Ŵ�

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cow.tex[model]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	cow.draw();
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	showHint();//��ʾ��ʾ
	glutSwapBuffers();//�û����棨ˢ�»��棬��Ϊʹ����˫���棩
}

void logRotate(int axis, int count) {//��¼��ת���
	if (rotateLog.size() == 0)
		rotateLog.push_back(pair<int, GLfloat>(axis, count));
	else if (rotateLog.back().first == axis) {
		rotateLog.back().second += count;
		if ((int)rotateLog.back().second % 360 == 0)
			rotateLog.pop_back();
	}
	else rotateLog.push_back(pair<int, GLfloat>(axis, count));
}

GLdouble wx, wy, wz;  /*  returned world x, y, z coords  */
void mouse(int button, int state, int x, int y)
{
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLint realy;  /*  OpenGL y coordinate position  */
	GLfloat winX, winY, winZ;
	
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			glPushMatrix();
			for (int i = rotateLog.size() - 1; i >= 0; i--)
				glRotatef(rotateLog[i].second, axis[rotateLog[i].first][0], axis[rotateLog[i].first][1], axis[rotateLog[i].first][2]);
			//������ת
			glTranslatef(0, 0, -cow.size*0.5);
			glRotatef(90, 1, 0, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(bigger, bigger, bigger);//���÷Ŵ�
			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

			//realy = viewport[3] - (GLint)y - 1;
			//printf("Coordinates at cursor are (%4d, %4d)\n", x, realy);
			/*gluUnProject((GLdouble)x, (GLdouble)(1000-y), 0.0,
				mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
			printf("World coords at z=0.0 are (%f, %f, %f)\n", wx, wy, wz);
			gluUnProject((GLdouble)x, (GLdouble)(1000-y), 1.0,
				mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
			printf("World coords at z=1.0 are (%f, %f, %f)\n", wx, wy, wz);*/
			/*glLoadIdentity();//���ñ任����
			glPushMatrix();
			for (int i = rotateLog.size() - 1; i >= 0; i--)
				glRotatef(rotateLog[i].second, axis[rotateLog[i].first][0], axis[rotateLog[i].first][1], axis[rotateLog[i].first][2]);
			//������ת
			glTranslatef(0, 0, -cow.size*0.5);
			glRotatef(90, 1, 0, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(bigger, bigger, bigger);//���÷Ŵ�*/
			winX = x;
			winY = viewport[3] - (float)y;
			glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
			gluUnProject(winX, winY, winZ, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
			//printf("(%f, %f, %f)\n", wx, wy, wz);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			exit(0);
		break;
	default:
		break;
	}
}



void change_2() {

	float x1 = 164.441745;
	float y1 = 138.956303;

	//float dist = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	for (int i = 0; i < cow.v.size(); i++) {

		if (cow.v[i].x > x1 &&cow.v[i].y >y1  ){
			float change_y = cow.v[i].x - x1;
			float change_x = cow.v[i].y - y1;
			cow.v[i].x -= change_x;
			cow.v[i].y += change_y;
		}
	}
}

void big_x() {
	for (int i = 0; i < cow.v.size(); i++) {

		if (cow.v[i].x > wx - 10.0 && cow.v[i].x < wx + 10.0 &&cow.v[i].y >wy - 10.0&& cow.v[i].y <wy + 10.0&&cow.v[i].z>wz-10.0&&cow.v[i].z<wz+10.0){
			cow.v[i].x += 5.0;
		}
	}
}

void small_x() {
	for (int i = 0; i < cow.v.size(); i++) {
		if (cow.v[i].x > wx - 10.0 && cow.v[i].x < wx + 10.0 &&cow.v[i].y >wy - 10.0&& cow.v[i].y <wy + 10.0&&cow.v[i].z>wz - 10.0&&cow.v[i].z<wz + 10.0){
			cow.v[i].x -= 5.0;
		}
	}
}

void big_y() {
	for (int i = 0; i < cow.v.size(); i++) {
		if (cow.v[i].x > wx - 10.0 && cow.v[i].x < wx + 10.0 &&cow.v[i].y >wy - 10.0&& cow.v[i].y <wy + 10.0&&cow.v[i].z>wz - 10.0&&cow.v[i].z<wz + 10.0){
			cow.v[i].y += 5.0;
		}
	}
}

void small_y() {
	for (int i = 0; i < cow.v.size(); i++) {
		if (cow.v[i].x > wx - 10.0 && cow.v[i].x < wx + 10.0 &&cow.v[i].y >wy - 10.0&& cow.v[i].y <wy + 10.0&&cow.v[i].z>wz - 10.0&&cow.v[i].z<wz + 10.0){
			cow.v[i].y -= 5.0;
		}
	}
}

void big_z() {
	for (int i = 0; i < cow.v.size(); i++) {
		if (cow.v[i].x > wx - 10.0 && cow.v[i].x < wx + 10.0 &&cow.v[i].y >wy - 10.0&& cow.v[i].y <wy + 10.0&&cow.v[i].z>wz - 10.0&&cow.v[i].z<wz + 10.0){
			cow.v[i].z += 5.0;
		}
	}
}

void small_z() {
	for (int i = 0; i < cow.v.size(); i++) {
		if (cow.v[i].x > wx - 10.0 && cow.v[i].x < wx + 10.0 &&cow.v[i].y >wy - 10.0&& cow.v[i].y <wy + 10.0&&cow.v[i].z>wz - 10.0&&cow.v[i].z<wz + 10.0){
			cow.v[i].z -= 5.0;
		}
	}
}

void keyboard(unsigned char key, int _x, int _y) {
	switch (key)
	{//��Ӧ��ת�¼�
	case 'q':
		logRotate(0, 1);
		break;
	case 'e':
		logRotate(0, -1);
		break;
	case 'w':
		logRotate(1, -1);
		break;
	case 's':
		logRotate(1, 1);
		break;
	case 'a':
		logRotate(2, -1);
		break;
	case 'd':
		logRotate(2, 1);
		break;
	}
	
	switch (key)
	{
	case 'r':
		bigger *= 1.01;
		break;
	case 'f':
		bigger /= 1.01;
		if (bigger == 0)
			bigger = 1;
		break;
	}

	switch (key)
	{
	case 'z':
		big_x();
		break;
	case 'x':
		small_x();
		break;
	case 'c':
		big_y();
		break;
	case 'v':
		small_y();
		break;
	case 'b':
		big_z();
		break;
	case 'n':
		small_z();
		break;
	}
	
	switch (key)
	{
	case 'y':
		change_2();
		break;
	}

	glutPostRedisplay();//ˢ����ͼ
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);//��ʼ��GLUT�⣻
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//˫����ģʽ��RGBģʽ����άģʽ��
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 1000);//���ô��ڴ�С
	glutCreateWindow("colorcube");//���ô�������
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//˫���դ��
	glEnable(GL_DEPTH_TEST);//������άģʽ��
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(3);//ʹ��3ά��դ��
	init();
	glutReshapeFunc(reshape);//���õ����ڴ�С�����ı�ʱ���õĺ���
	glutDisplayFunc(display);//����ˢ�º���
	printf("�����Ҫ���εĵط�����z��x�������ƶ�����x��x�Ḻ���ƶ�����c��y�������ƶ�����v��y�Ḻ���ƶ�����b��z�������ƶ�����n��z�Ḻ���ƶ���\n");
	printf("��yţ�Ǳ��Ρ�\n");
	glutKeyboardFunc(keyboard);//���ü�����Ӧ�¼�
	glutMouseFunc(mouse);
	glutMainLoop();//�����Զ�ˢ��ѭ��
	return 0;
}
