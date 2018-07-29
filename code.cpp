#include <GL/glut.h>
#include <iostream>
#include <math.h>
//#include <stdlib.h>
//#include <time.h>
#include <string.h>

const int _width = 500;
const int _height = 500;


int n = 9;
int board[9][9];	// board for gameplay
int xclick, yclick, _region = 9, region = 9;
int turn;			// current move
int result;			// Result of the game
bool over;			// Is the game Over?
float xlinepos[4] = { 0, 166.66667, 333.3333, 500 };
float ylinepos[5] = { 50, 200, 350, 500, 500 };



//Sets the board for Tic Tac Toe

void Initialize() {
	turn = 1;
	for (int i = 0; i<n; i++) {
		for (int j = 0; j<n; j++)
			board[i][j] = 0;
	}
}

//Called when any key from keyboard is pressed

void OnKeyPress(unsigned char key, int x, int y) {
	switch (key) {
	case 'y':
		if (over == true) {
			over = false;
			_region = 9;
			region = 9;
			Initialize();
		}
		break;
	case 'n':
		if (over == true) {
			exit(0);
		}
		break;
	default:
		exit(0);
	}
}

//Called when Mouse is clicked

void OnMouseClick(int button, int state, int x, int y) {
	int x0 = _width / n, y0 = (_height - 50) / n;

	xclick = (y - 50) / y0;
	yclick = x / x0;

	region = yclick / 3 + 3 * (xclick / 3);
	if (_region == 9) _region = region;

	if (y < 50 || board[(y - 50) / y0][x / x0] != 0) return; //From the top, 50 px space is for messages hence, no mouse activity shall be recorded there

	if (over == false && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && region == _region) {
		if (turn == 1) {
			if (board[(y - 50) / y0][x / x0] == 0) {
				board[(y - 50) / y0][x / x0] = 1;
				turn = 2;
			}
		}
		else if (turn == 2) {
			if (board[(y - 50) / y0][x / x0] == 0) {
				board[(y - 50) / y0][x / x0] = 2;
				turn = 1;
			}
		}

		_region = (yclick % 3 + 3 * xclick) % 9;
	}
}

//Utility function to draw string

void DrawString(void *font, const char s[], float x, float y) {
	unsigned int i;
	glRasterPos2f(x, y);
	for (i = 0; i<strlen(s); i++) {
		glutBitmapCharacter(font, s[i]);
	}
}

//Function to draw up the horizontal and vertical lines

void DrawLines() {


	int x = _width / 3;
	int y = (_height - 50) / 3;


	for (int i = 0; i < 2; i++) {

		glLineWidth(4);
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);

		glVertex2f((i + 1)*x, 50);
		glVertex2f((i + 1)*x, _height);


		glVertex2f(0, (i + 1)*y + 50);
		glVertex2f(_width, (i + 1)*y + 50);

		glEnd();
	}


	x = _width / n;
	y = (_height - 50) / n;


	for (int i = 0; i < n - 1; i++) {

		glLineWidth(1);
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);

		glVertex2f((i + 1)*x, 50);
		glVertex2f((i + 1)*x, _height);


		glVertex2f(0, (i + 1)*y + 50);
		glVertex2f(_width, (i + 1)*y + 50);

		glEnd();
	}


}

//Utility function to draw the circle

void DrawCircle(float cx, float cy, float r, int num_segments) {

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num_segments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 
		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
}

//Function to draw the cross and circle of Tic Tac Toe

void DrawXO() {

	int x = _width / n, y = (_height - 50) / n;  //x gives the width of each box and y gives the height of each box

	int xh = x / 2, xf = x / 4, yf = y / 4, yh = y / 2;

	for (int i = 0; i<n; i++) {
		for (int j = 0; j<n; j++) {
			if (board[i][j] == 1) {
				glBegin(GL_LINES);
				glVertex2f(xh + j * x - xf, yh + i * y - yf + 50);
				glVertex2f(xh + j * x + xf, yh + i * y + yf + 50);
				glVertex2f(xh + j * x - xf, yh + i * y + yf + 50);
				glVertex2f(xh + j * x + xf, yh + i * y - yf + 50);
				glEnd();
			}
			else if (board[i][j] == 2) {

				DrawCircle(xh + j * x, yh + i * y + 50, xf, 15);
			}
		}
	}
}

//Function to check if there is any winner

bool CheckWinner()
{

	int flag1 = 0, flag2 = 0;
	int i, j;
	// horizontal check
	for (i = 0; i<n; i++) {
		for (j = 0; j<3; j++) {

			int jdash = j + (i / 3) * 3;

			if (board[jdash][(i*3)%9] != 0 && board[jdash][(i * 3) % 9] == board[jdash][(i * 3) % 9 + 1] && board[jdash][(i * 3) % 9] == board[jdash][(i * 3) % 9 + 2]) {
				return true;
			}
		}
	}
	// vertical check
	for (i = 0; i<n; i++) {
		for (j = 0; j<3; j++) {

			int jdash = j + (i * 3)%9;

			if (board[(i/3)*3][jdash] != 0 && board[(i / 3) * 3][jdash] == board[(i / 3) * 3 + 1][jdash] && board[(i / 3) * 3][jdash] == board[(i / 3) * 3 + 2][jdash]) {
					return true;
			}
		}
	}
	//Diagonal Check
	for (int j = 0; j < n; j++) {
		
			int xcord = (j / 3) * 3;
			int ycord = (j * 3) % 9;
			
			if (board[xcord][ycord] != 0 && board[xcord][ycord] == board[xcord + 1][ycord + 1] && board[xcord][ycord] == board[xcord + 2][ycord + 2])
				return true;
			else if (board[xcord][ycord + 2] != 0 && board[xcord][ycord + 2] == board[xcord + 1][ycord + 1] && board[xcord][ycord + 2] == board[xcord + 2][ycord])
				return true;
			else continue;
	}
	
	return false;
}


//function to check if there is draw

bool CheckIfDraw() {
	int i, j, k, count =0;
	bool draw;
	
	for (i = 0; i<n; i++) {
		count = 0;
		for (j = 0; j < 3; j++) {
			int idash = j + (i / 3) * 3;
			for (k = 0; k < 3; k++) {
				int jdash = k + (i * 3) % 9;
				if (board[idash][jdash] != 0)
					count++;
			}
		}
		if (count == 9) return true;
	}
	return false;
}

//Function to display up everything

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	glColor3f(0, 0, 0);

	//Gradient bg
	glBegin(GL_POLYGON);
	glColor3f(0.254901961, 0.41015625, 0.87890625);
	glVertex2f(0, 0);
	glColor3f(0.0, 0.74609375, 1.0);
	glVertex2f(0, _height);
	glColor3f(1, 1, 1);
	glVertex2f(_width, _height);
	glColor3f(0.515625, 0.4375, 1);
	glVertex2f(_width, 0);
	glEnd();

	//Selected Region
	float xlen = _width / 3, ylen = (_height - 50) / 3;
	glColor3f(0, 0.7, 0);
	glPointSize(4);
	glBegin(GL_POLYGON);
	glVertex2f(xlinepos[_region % 3], ylinepos[_region / 3]);
	glVertex2f(xlinepos[_region % 3 + 1], ylinepos[_region / 3]);
	glVertex2f(xlinepos[_region % 3 + 1], ylinepos[(_region + 3) / 3]);
	glVertex2f(xlinepos[_region % 3], ylinepos[(_region + 3) / 3]);
	glEnd();

	glColor3f(0, 0, 0);
	glPointSize(1);
	if (turn == 1)
		DrawString(GLUT_BITMAP_HELVETICA_18, "Player1's turn", 200, 30);
	else
		DrawString(GLUT_BITMAP_HELVETICA_18, "Player2's turn", 200, 30);

	DrawLines();
	DrawXO();

	if (CheckWinner() == true) {
		if (turn == 1) {
			over = true;
			result = 2;
		}
		else {
			over = true;
			result = 1;
		}
	}
	else if (CheckIfDraw() == true) {
		over = true;
		result = 0;
	}


	glColor3f(1, 0, 0);


	if (over == true) {
		DrawString(GLUT_BITMAP_HELVETICA_18, "Game Over", 200, 180);
		if (result == 0)
			DrawString(GLUT_BITMAP_HELVETICA_18, "It's a draw", 200, 220); //210
		if (result == 1)
			DrawString(GLUT_BITMAP_HELVETICA_18, "Player1 wins", 200, 220);
		if (result == 2)
			DrawString(GLUT_BITMAP_HELVETICA_18, "Player2 wins", 200, 220);
		DrawString(GLUT_BITMAP_HELVETICA_18, "Do you want to continue (y/n)", 150, 240);
	}
	glutSwapBuffers();
}

//Function to reshape

void Reshape(int x, int y) {
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x, y, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}

//Driver Function

int main(int argc, char **argv) {
	Initialize();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(550, 200);
	glutInitWindowSize(_width, _height);
	glutCreateWindow("Tic Tac Toe");
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(OnKeyPress);
	glutMouseFunc(OnMouseClick);
	glutIdleFunc(Display);
	
	glutMainLoop();
	return 0;
}
