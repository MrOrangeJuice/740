
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
using namespace std;

float canvasSize[] = { 10.0f, 10.0f };
int rasterSize[] = { 800, 600 };

// structure for storing 3 2D vertices of a triangle
int numOfVertices = 0;
float v[2 * 3];
float color[3];
float pointSize;
float lineWidth;
int mode;
vector<vector<float>> points;
float pointPos[2];

float mousePos[2];

void init(void)
{
    for (int i = 0; i < 6; i++)
        v[i] = 0.0f;
    mousePos[0] = mousePos[1] = 0.0f;
    color[0] = 1.0f;
    color[1] = color[2] = 0.0f;
    pointSize = 10.0f;
    mode = 1;
}

void drawCursor()
{
    glColor3f(1.0f, 0.0f, 1.0f);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glVertex2fv(mousePos);
    glEnd();
    glPointSize(1.0f);
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3fv(color);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (numOfVertices > 0 && numOfVertices < 3) {

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numOfVertices; i++)
            glVertex2fv(v + i * 2);
        glVertex2fv(mousePos);
        glEnd();
    }
    else if (numOfVertices == 3) {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < numOfVertices; i++)
            glVertex2fv(v + i * 2);
        glEnd();
    }

    for (int i = 0; i < points.size(); i += 2)
    {
        glColor3fv(color);
        glPointSize(points[i][1]);
        glBegin(GL_POINTS);
        pointPos[0] = points[i][0];
        pointPos[1] = points[i + 1][0];
        glVertex2fv(pointPos);
        glEnd();
    }

    drawCursor();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    rasterSize[0] = w;
    rasterSize[1] = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvasSize[0], 0.0, canvasSize[1]);
    glViewport(0, 0, rasterSize[0], rasterSize[1]);

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (mode)
    {
        case 0:
            points.push_back({ mousePos[0],pointSize });
            points.push_back({ mousePos[1],pointSize });
            break;
        case 1:
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                if (numOfVertices >= 3)
                    numOfVertices = 0;

                mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
                mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];
                v[numOfVertices * 2 + 0] = mousePos[0];
                v[numOfVertices * 2 + 1] = mousePos[1];

                numOfVertices++;
                glutPostRedisplay();
            }
            break;
    }
}

void motion(int x, int y)
{
    // mouse events are handled by OS, eventually. When using mouse in the raster window, it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
    mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
    mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    }
}

void menu(int value)
{
    switch (value) {
    case 0: // clear
        numOfVertices = 0;
        points.clear();
        glutPostRedisplay();
        break;
    case 1: //exit
        exit(0);
    case 2: // red
        color[0] = 1.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        glutPostRedisplay();
        break;
    case 3: // green
        color[0] = 0.0f;
        color[1] = 1.0f;
        color[2] = 0.0f;
        glutPostRedisplay();
        break;
    case 4: // blue
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 1.0f;
        glutPostRedisplay();
        break;
    case 5:
        mode = 0;
        break;
    case 6:
        mode = 1;
        break;
    case 7:
        mode = 2;
        break;
    case 8:
        mode = 3;
        break;
    case 9:
        mode = 4;
        break;
    case 10:
        pointSize = 5.0f;
        break;
    case 11:
        pointSize = 10.0f;
        break;
    case 12:
        pointSize = 20.0f;
        break;
    default:
        break;
    }
}
void createMenu()
{

    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Blue", 4);

    int shapeMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Points", 5);
    glutAddMenuEntry("Triangles", 6);
    glutAddMenuEntry("Quads", 7);
    glutAddMenuEntry("Polygons", 8);
    glutAddMenuEntry("Lines", 9);

    int pointMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 10);
    glutAddMenuEntry("Medium", 11);
    glutAddMenuEntry("Large", 12);

    int lineMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Thin", 13);
    glutAddMenuEntry("Medium", 14);
    glutAddMenuEntry("Thick", 15);

    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddSubMenu("Colors", colorMenu);
    glutAddSubMenu("Objects", shapeMenu);
    glutAddSubMenu("Point Size", pointMenu);
    glutAddSubMenu("Line Width", lineMenu);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(rasterSize[0], rasterSize[1]);
    glutCreateWindow("Mouse Event - draw a triangle");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    createMenu();
    glutMainLoop();
    return 0;


}