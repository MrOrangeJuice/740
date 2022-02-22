//Usage:
//Hold down the number keys , 1 or 2, to select one or two quads.
//While quad(s) are selected, use the left mouse button to translate and use the right mouse button to rotate.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

int win_width = 600, win_height = 600;
float canvas_width = 20.0f, canvas_height = 20.0f;

bool keyStates[256];
int buttonState;
float colors[3 * 2];
float translations[2 * 2];
float rotations[2];

float curMouse[2];
float preMouse[2];

class Quad
{
    private:
        float left;
        float right;
        float top;
        float bottom;
        float color[3];
        float lineWidth;
        float rotation;
        int level;
    public:
        Quad(float left, float right, float top, float bottom, float colorr, float colorg, float colorb, float lineWidth, int id)
        {
            this->left = left;
            this->right = right;
            this->top = top;
            this->bottom = bottom;
            this->color[0] = colorr;
            this->color[1] = colorg;
            this->color[2] = colorb;
            this->lineWidth = lineWidth;
            this->level = id;
            this->rotation = 0.0f;
        }
        void Draw()
        {
            glRotatef(rotation, 0.0f, 0.0f, 1.0f);
            glColor3fv(color);
            glLineWidth(3.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(left, bottom);
            glVertex2f(left, top);
            glVertex2f(right, top);
            glVertex2f(right, bottom);
            glEnd();
            glLineWidth(1.0f);
        }
        void Rotate(float newRotation)
        {
            this->rotation += newRotation;
        }
        int GetLevel()
        {
            return level;
        }
};

vector<Quad> quadVector;

void init(void)
{
    for (int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }

    Quad torso = Quad(-2, 2, 2, 0, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad chest = Quad(-2.5f, 2.5f, 4.5f, 2, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad neck = Quad(-0.5f, 0.5f, 6, 4.5f, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad head = Quad(-1.5f, 1.5f, 9, 6, 0.0f, 0.0f, 0.0f, 3.0f, 0);

    // Left Arm
    Quad leftArm = Quad(-4.5f, -2.5f, 4, 2.5f, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad leftForeArm = Quad(-6.5f, -4.5f, 4, 2.5f, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad leftHand = Quad(-8, -6.5f, 4.25f, 2.25f, 0.0f, 0.0f, 0.0f, 3.0f, 0);

    // Right Arm
    Quad rightArm = Quad(4.5f, 2.5f, 4, 2.5f, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad rightForeArm = Quad(6.5f, 4.5f, 4, 2.5f, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad rightHand = Quad(8, 6.5f, 4.25f, 2.25f, 0.0f, 0.0f, 0.0f, 3.0f, 0);

    // Left Leg
    Quad leftThigh = Quad(-2, -1, 0, -3, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad leftLeg = Quad(-2, -1, -3, -6, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad leftFoot = Quad(-4, -1, -6, -7.5f, 0.0f, 0.0f, 0.0f, 3.0f, 0);

    // Right Leg
    Quad rightThigh = Quad(2, 1, 0, -3, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad rightLeg = Quad(2, 1, -3, -6, 0.0f, 0.0f, 0.0f, 3.0f, 0);
    Quad rightFoot = Quad(4, 1, -6, -7.5f, 0.0f, 0.0f, 0.0f, 3.0f, 0);

    quadVector.push_back(torso);
    quadVector.push_back(chest);
    quadVector.push_back(neck);
    quadVector.push_back(head);

    quadVector.push_back(leftArm);
    quadVector.push_back(leftForeArm);
    quadVector.push_back(leftHand);

    quadVector.push_back(rightArm);
    quadVector.push_back(rightForeArm);
    quadVector.push_back(rightHand);

    quadVector.push_back(leftThigh);
    quadVector.push_back(leftLeg);
    quadVector.push_back(leftFoot);
    
    quadVector.push_back(rightThigh);
    quadVector.push_back(rightLeg);
    quadVector.push_back(rightFoot);

}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int i = 0; i < quadVector.size(); i++)
    {
        quadVector[i].Draw();
    }

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    win_width = w;
    win_height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-canvas_width / 2, canvas_width / 2, -canvas_height / 2, canvas_height / 2);
    glViewport(0, 0, (GLsizei)win_width, (GLsizei)win_height);

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // 'esc' key
        exit(0);

    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Project 2 Robot");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMainLoop();
    return 0;

}