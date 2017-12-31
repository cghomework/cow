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
	//载入茶壶obj
	char localPath[MAX_PATH_NAME];
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "cow.obj");
	cow.init(localPath);

	//载入hint纹理
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
{//自动传入新窗口的大小（像素）
	glViewport(0, 0, w, h);//在窗口中显示的区域：显示区域左下角坐标显示区域的宽度与高度
	glMatrixMode(GL_PROJECTION);//申明将要进行投影设置操作
	glLoadIdentity();//重置变换矩阵
	
	//使用正交投影观察物体，观察方式是平行修剪空间
	if (w <= h) {//这里使无论宽高比如何，始终观察窗口那么大的一块区域
		glOrtho(-cow.size, cow.size, -cow.size*(GLfloat)h / (GLfloat)w, cow.size*(GLfloat)h / (GLfloat)w, 0, cow.size * 2);
		windowLeft = -cow.size;
		windowDown = -cow.size*(GLfloat)h / (GLfloat)w;
		//定义观察空间的：左、右、下、上、前、后
		size_change *= -(GLfloat)h / (GLfloat)w;
	}
	else {
		glOrtho(-cow.size*(GLfloat)w / (GLfloat)h, cow.size*(GLfloat)w / (GLfloat)h, -cow.size, cow.size, 0, cow.size * 2);
		windowLeft = -cow.size*(GLfloat)w / (GLfloat)h;
		windowDown = -cow.size;
		size_change *= -(GLfloat)w / (GLfloat)h;
	}
	//gluPerspective(90.0f, (GLdouble)w / (GLdouble)h, 1.0f, 1.0f);
	//也可以使用透视投影投影观察体
	//gluPerspective(90, w / h, 100, 600);//这里参数还需要调整
	//定义观察空间的垂直角度，宽高比，前截面，后界面

	glMatrixMode(GL_MODELVIEW);//申明将进行视景设置操作
	glLoadIdentity();//重置变换矩阵
	//ps.当参数是GL_TEXTURE时可以进行纹理操作
}

void setLight() {//添加光照
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


void showHint() {//显示提示
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空颜色、三维缓存？
	glMatrixMode(GL_MODELVIEW);//申明将进行视景设置操作
	glLoadIdentity();//重置变换矩阵
	gluLookAt(cow.size, 0, 0, 0, 0, 0, 0, 0, 1);//摄像机的xyz，观察点的xyz，相机上方向的xyz
	setLight();//设置光照
	glPushMatrix();
	for (int i = rotateLog.size() - 1; i >= 0; i--)
		glRotatef(rotateLog[i].second, axis[rotateLog[i].first][0], axis[rotateLog[i].first][1], axis[rotateLog[i].first][2]);
	//设置旋转
	glTranslatef(0, 0, -cow.size*0.5);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(bigger, bigger, bigger);//设置放大

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cow.tex[model]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	cow.draw();
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	showHint();//显示提示
	glutSwapBuffers();//置换缓存（刷新画面，因为使用了双缓存）
}

void logRotate(int axis, int count) {//记录旋转情况
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
			//设置旋转
			glTranslatef(0, 0, -cow.size*0.5);
			glRotatef(90, 1, 0, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(bigger, bigger, bigger);//设置放大
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
			/*glLoadIdentity();//重置变换矩阵
			glPushMatrix();
			for (int i = rotateLog.size() - 1; i >= 0; i--)
				glRotatef(rotateLog[i].second, axis[rotateLog[i].first][0], axis[rotateLog[i].first][1], axis[rotateLog[i].first][2]);
			//设置旋转
			glTranslatef(0, 0, -cow.size*0.5);
			glRotatef(90, 1, 0, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(bigger, bigger, bigger);//设置放大*/
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
	{//响应旋转事件
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

	glutPostRedisplay();//刷新视图
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);//初始化GLUT库；
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//双缓存模式、RGB模式、三维模式？
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 1000);//设置窗口大小
	glutCreateWindow("colorcube");//设置窗口名称
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//双面光栅化
	glEnable(GL_DEPTH_TEST);//开启三维模式？
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(3);//使用3维光栅化
	init();
	glutReshapeFunc(reshape);//设置当窗口大小发生改变时调用的函数
	glutDisplayFunc(display);//设置刷新函数
	printf("鼠标点击要变形的地方，按z向x轴正向移动，按x向x轴负向移动，按c向y轴正向移动，按v向y轴负向移动，按b向z轴正向移动，按n向z轴负向移动。\n");
	printf("按y牛角变形。\n");
	glutKeyboardFunc(keyboard);//设置键盘响应事件
	glutMouseFunc(mouse);
	glutMainLoop();//进入自动刷新循环
	return 0;
}
