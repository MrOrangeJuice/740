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

void init(void)
{
    for (int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }

    // color of the first quad
    colors[0] = 0.0f; // red
    colors[1] = 0.0f; // green
    colors[2] = 0.0f; // blue

    // color of the second quad
    colors[3] = 0.0f; // red
    colors[4] = 0.0f; // green
    colors[5] = 1.0f; // blue

    // translation of the first quad
    translations[0] = 0.0f; // x
    translations[1] = 0.0f; // y

    // translation of the second quad
    translations[2] = 5.0f; // x
    translations[3] = 0.0f; // y

    // rotaion of the first quad
    rotations[0] = 0.0f;

    // rotation of the second quad
    rotations[1] = 0.0f;

    buttonState = -1; // no mouse button
}

void drawQuad(float left, float right, float top, float bottom, float* color)
{
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

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // transform the first quad // (-3, -3) and (3, 3)
    glTranslatef(translations[0], translations[1], 0.0f);
    glRotatef(rotations[0], 0.0f, 0.0f, 1.0f);
    drawQuad(-3, 3, 3, -3, colors);

    // transform the second quad // (4, -1) and (5, 1)
    glTranslatef(translations[2], translations[3], 0.0f);
    glRotatef(rotations[1], 0.0f, 0.0f, 1.0f);
    drawQuad(4, 5, 1, -1, colors + 3);


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

    if (key == '1') {
        keyStates['1'] = true;
        colors[0] = 1.0f;
        colors[1] = 0.0f;
        colors[2] = 0.0f;
    }
    if (key == '2') {
        keyStates['2'] = true;
        colors[3] = 1.0f;
        colors[4] = 0.0f;
        colors[5] = 0.0f;
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    if (key == '1') {
        keyStates['1'] = false;
        colors[0] = 0.0f;
        colors[1] = 0.0f;
        colors[2] = 0.0f;
    }
    if (key == '2') {
        keyStates['2'] = false;
        colors[3] = 0.0f;
        colors[4] = 0.0f;
        colors[5] = 0.0f;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        buttonState = button;
        curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
        curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
        preMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
        preMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;
    }
    else if (state == GLUT_UP)
        button = -1;
}

void motion(int x, int y)
{

    curMouse[0] = ((float)x / win_width - 0.5f) * canvas_width;
    curMouse[1] = ((float)(win_height - y) - 0.5f) / win_height * canvas_height;

    if (buttonState == GLUT_LEFT_BUTTON) {
        if (keyStates['1']) {
            translations[0] += curMouse[0] - preMouse[0];
            translations[1] += curMouse[1] - preMouse[1];
        }

        if (keyStates['2']) {
            translations[2] += curMouse[0] - preMouse[0];
            translations[3] += curMouse[1] - preMouse[1];
        }
    }

    else if (buttonState == GLUT_RIGHT_BUTTON) {
        if (keyStates['1'])
            rotations[0] += curMouse[0] - preMouse[0];
        if (keyStates['2'])
            rotations[1] += curMouse[0] - preMouse[0];
    }

    preMouse[0] = curMouse[0];
    preMouse[1] = curMouse[1];
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
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;

}