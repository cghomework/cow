#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����  

/*bitmap����
GLUT_BITMAP_8_BY_13
GLUT_BITMAP_9_BY_15
GLUT_BITMAP_TIMES_ROMAN_10 ���壺TIMES_ROMAN ��С��10
GLUT_BITMAP_TIMES_ROMAN_24 ���壺TIMES_ROMAN ��С��24
GLUT_BITMAP_HELVETICA_10
GLUT_BITMAP_HELVETICA_12
GLUT_BITMAP_HELVETICA_18
*/

class button{
private:
	// ����power_of_two�����ж�һ�������ǲ���2����������    
	int power_of_two(int n){
		if (n <= 0)
			return 0;
		return (n & (n - 1)) == 0;
	}
	void showTex(float x, float y){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _tex);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x, y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x + w, y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x + w, y + h);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x, y + h);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glPopAttrib();
	}
	void showText(){
		setColor(colorText);
		glRasterPos2d(x + 5, y + h - 18);
		for (char *c = _text; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
		}
	}
	void showBorder(){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		setColor(colorBorder);
		glBegin(GL_LINE_STRIP & GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
		glEnd();
	}
	void showInside(){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		setColor(colorInside);
		glBegin(GL_POLYGON);
		glVertex3f(x, y, 0);
		glVertex3f(x + w, y, 0);
		glVertex3f(x + w, y + h, 0);
		glVertex3f(x, y + h, 0);
		glEnd();
	}
	void setColor(double *color){
		glColor4dv(color);
	}
public:
	float x, y;
	float w, h;
	double colorInside[4];
	double colorBorder[4];
	double colorHoverInside[4];
	double colorHoverBorder[4];
	double colorText[4];
	int solid;
	int border;
	int mouseIn;
	GLuint _tex;
	int tex;
	char *_text;
	button(float _x = 0, float _y = 0, float _w = 125, float _h = 25) :
		x(_x), y(_y), w(_w), h(_h), solid(1), border(0), mouseIn(0), tex(0), _text(NULL){
		for (int i = 0; i<3; i++){
			colorBorder[i] = 0.5;
			colorInside[i] = 1.0;
			colorHoverBorder[i] = 0.5;
			colorHoverInside[i] = 1.0;
			colorText[i] = 0.0;
		}
		colorBorder[3] = 1.0;
		colorInside[3] = 1.0;
		colorHoverBorder[3] = 1.0;
		colorHoverInside[3] = 1.0;
		colorText[3] = 1.0;
	}
	void setPosition(int _x, int _y){
		x = _x;
		y = _y;
	}
	void setSize(int _w, int _h){
		w = _w;
		h = _h;
	}
	void setColorInside(double c1, double c2, double c3, double c4 = 1.0){
		colorInside[0] = c1;
		colorInside[1] = c2;
		colorInside[2] = c3;
		colorInside[3] = c4;
	}
	void setColorBorder(double c1, double c2, double c3, double c4 = 1.0){
		colorBorder[0] = c1;
		colorBorder[1] = c2;
		colorBorder[2] = c3;
		colorBorder[3] = c4;
	}
	void setText(const char *text){
		if (text == NULL){
			_text = NULL;
			return;
		}
		_text = new char[strlen(text)];
		strcpy(_text, text);
	}
	void setBorder(int x = 1){
		border = x;
	}
	void setSolid(int x = 1){
		solid = x;
	}
	void setTex(int x = 0){
		tex = x;
	}
	void show(float x, float y){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		if (solid){
			if (tex) showTex(x, y);
			else showInside();
		}
		if (border) showBorder();
		if (_text != NULL)	showText();
		glPopMatrix();
		glPopAttrib();
	}
	void mouseHover(){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		/*
		setColor(colorHoverInside);
		glBegin(GL_POLYGON);
		glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
		glEnd();
		*/

		setColor(colorHoverBorder);
		glBegin(GL_LINE_STRIP & GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
		glEnd();
		//		glFlush();
		glPopMatrix();
		glPopAttrib();
	}
	void mouseUnhover(){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		setColor(colorInside);
		glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
		glEnd();
		setColor(colorBorder);
		glBegin(GL_LINE_STRIP & GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
		glEnd();
		//		glFlush();
		glPopMatrix();
		glPopAttrib();
	}
	int setTexImage(const char* file_name){
		GLint width, height, total_bytes;
		GLubyte* pixels = 0;
		GLuint last_texture_ID = 0, texture_ID = 0;

		// ���ļ������ʧ�ܣ�����    
		FILE* pFile = fopen(file_name, "rb");
		if (pFile == 0)
			return 2;

		// ��ȡ�ļ���ͼ��Ŀ��Ⱥ͸߶�    
		fseek(pFile, 0x0012, SEEK_SET);
		fread(&width, 4, 1, pFile);
		fread(&height, 4, 1, pFile);
		fseek(pFile, BMP_Header_Length, SEEK_SET);

		// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���    
		{
			GLint line_bytes = width * 3;
			while (line_bytes % 4 != 0)
				++line_bytes;
			total_bytes = line_bytes * height;
		}

		// �����������ֽ��������ڴ�    
		pixels = (GLubyte*)malloc(total_bytes);
		if (pixels == 0)
		{
			fclose(pFile);
			return 3;
		}

		// ��ȡ��������    
		if (fread(pixels, total_bytes, 1, pFile) <= 0)
		{
			free(pixels);
			fclose(pFile);
			return 4;
		}

		// �Ծ;ɰ汾�ļ��ݣ����ͼ��Ŀ��Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������    
		// ��ͼ����߳�����OpenGL�涨�����ֵ��Ҳ����    
		/*{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if( !power_of_two(width)
		|| !power_of_two(height)
		|| width > max
		|| height > max )
		{
		const GLint new_width = 256;
		const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
		GLint new_line_bytes, new_total_bytes;
		GLubyte* new_pixels = 0;

		// ����ÿ����Ҫ���ֽ��������ֽ���
		new_line_bytes = new_width * 3;
		while( new_line_bytes % 4 != 0 )
		++new_line_bytes;
		new_total_bytes = new_line_bytes * new_height;

		// �����ڴ�
		new_pixels = (GLubyte*)malloc(new_total_bytes);
		if( new_pixels == 0 )
		{
		free(pixels);
		fclose(pFile);
		return 5;
		}

		// ������������
		gluScaleImage(GL_RGB,
		width, height, GL_UNSIGNED_BYTE, pixels,
		new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

		// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
		free(pixels);
		pixels = new_pixels;
		width = new_width;
		height = new_height;
		}
		}*/

		// ����һ���µ��������    
		glGenTextures(1, &texture_ID);
		if (texture_ID == 0)
		{
			free(pixels);
			fclose(pFile);
			return 6;
		}

		// ���µ�����������������������������    
		// �ڰ�ǰ���Ȼ��ԭ���󶨵�������ţ��Ա��������лָ�    
		GLint lastTextureID = last_texture_ID;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
		glBindTexture(GL_TEXTURE_2D, texture_ID);
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		/* glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels); */

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, 0x80E0, GL_UNSIGNED_BYTE, pixels);

		glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ��������    
		free(pixels);
		_tex = texture_ID;
		return 1;
	}
};