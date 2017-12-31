#include <vector>
#include <Windows.h>
#include <GL\glut.h>
using std::vector;

class Point {
public:
	float x, y, z;
	Point(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
};

class Vertex {
public:
	int vi;
	Vertex(float _vi) :vi(_vi) {}
};

class Obj {
public:
	vector<Point> v;
	vector<vector<Vertex>> f;
	float size = 0;
	GLuint tex[3];
	Obj(){}
	void init(char* filename) {
		char line[100];
		char mark[5];
		char *pch;

		FILE* fp;
		fp = fopen(filename, "r");
		v.push_back(Point(0, 0, 0));

		if (!fp) {
			MessageBoxA(HWND_DESKTOP, "Í¼Æ¬µ¼ÈëÊ§°Ü!\n(TextureLoad Failed!)", "Error", MB_OK | MB_ICONEXCLAMATION);
			exit(1);
		}

		while (fgets(line, 100, fp)) {
			switch (line[0]) {
			case 'v':
				float x, y, z;
				sscanf(line, "%s %f %f %f", mark, &x, &y, &z);
				
				
					v.push_back(Point(x, y, z));
					if (abs(x) > size)
						size = abs(x);
					if (abs(y) > size)
						size = abs(y);
					if (abs(z) > size)
						size = abs(z);
					break;
				
				

			case 'f':
				pch = strtok(line, " ");
				pch = strtok(NULL, " ");
				vector<Vertex> now_f;
				while (pch != NULL && pch[0] != '\n')
				{
					int v;
					sscanf(pch, "%d", &v);
					now_f.push_back(Vertex(v));
					pch = strtok(NULL, " ");
				}
				f.push_back(now_f);
				break;
			}
		}
	}

	void draw() {
		glPushMatrix();
		for (int i = 0; i < f.size(); i++) {
			glBegin(GL_POLYGON);
			float vn[3];
			//三个顶点
			float sv1[3];
			float sv2[3];
			float sv3[3];
			sv1[0] = v[f[i][0].vi].x;
			sv1[1] = v[f[i][0].vi].y;
			sv1[2] = v[f[i][0].vi].z;
			sv2[0] = v[f[i][1].vi].x;
			sv2[1] = v[f[i][1].vi].y;
			sv2[2] = v[f[i][1].vi].z;
			sv3[0] = v[f[i][2].vi].x;
			sv3[1] = v[f[i][2].vi].y;
			sv3[2] = v[f[i][2].vi].z;
			float vec1[3], vec2[3], vec3[3];//计算法向量
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = sv1[0] - sv2[0];
			vec1[1] = sv1[1] - sv2[1];
			vec1[2] = sv1[2] - sv2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = sv1[0] - sv3[0];
			vec2[1] = sv1[1] - sv3[1];
			vec2[2] = sv1[2] - sv3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			vn[0] = vec3[0] / D;
			vn[1] = vec3[1] / D;
			vn[2] = vec3[2] / D;

			glNormal3f(vn[0], vn[1], vn[2]);//绘制法向量
			for (int j = 0; j < f[i].size(); j++) {
				//glTexCoord3f(vt[f[i][j].vti].x, vt[f[i][j].vti].y, vt[f[i][j].vti].z);
				//glNormal3f(vn[f[i][j].vni].x, vn[f[i][j].vni].y, vn[f[i][j].vni].z);
				glVertex3f(v[f[i][j].vi].x, v[f[i][j].vi].y, v[f[i][j].vi].z);
				glColor3f(0.8f, 0.3f, 0.1f);
			}
			glEnd();
		}
		glPopMatrix();
	}
};