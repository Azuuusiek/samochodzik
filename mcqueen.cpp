#define _USE_MATH_DEFINES
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <GL/gl.h>
#include "Libs/noise/noise.h"
#include "Libs/noise/module/module.h"
#include "Libs/noise/module/perlin.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <queue>
const int WIDTH = 800;
const int HEIGHT = 800;
float x = HEIGHT / 2+50;
float y = 30;
float xstart = x;
float ystart = y;
float k = 4;
float vmax = 8;
int ex=x, ey=y;
int b = 0;
float a = 0;
float v = 0;
float v0 = 0;
float t = 0;
float dt = 0.01;
float t0 = 0;
bool case1 = false;
bool case2 = false;
bool case3 = false;
bool u = true;
float g = 9;
bool f = true;
int w = 0;
int nxx, nyy;
int kk = 0;
bool wall = false;
float alpha = M_PI/2;
bool isLeftKeyPressed = false;
bool isRightKeyPressed = false;
bool isSpacePressed = false;
bool isUpPressed = false;
bool road[1000][1000];
std::queue<std::pair<int, int> >q;
void handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT) {
        if (action == GLFW_PRESS)
            isLeftKeyPressed = true;
        else if (action == GLFW_RELEASE)
            isLeftKeyPressed = false;
    }
    else if (key == GLFW_KEY_RIGHT) {
        if (action == GLFW_PRESS)
            isRightKeyPressed = true;
        else if (action == GLFW_RELEASE)
            isRightKeyPressed = false;
    }
    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS)
            isSpacePressed = true;
        else if (action == GLFW_RELEASE)
            isSpacePressed = false;
    }
    if (key == GLFW_KEY_UP) {
        if (action == GLFW_PRESS)
            isUpPressed = true;
        else if (action == GLFW_RELEASE)
            isUpPressed = false;
    }
}

float map(float v, float x1, float y1, float x2, float y2) {
    return x2 + (y2 - x2) * ((v - x1) / (y1 - x1));
}

void rect(float x1, float y1, float x2, float y2) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x1, y1);
    glVertex2d(x1, y2);
    glVertex2d(x2, y2);
    glVertex2d(x2, y1);
    glVertex2d(x1, y1);
    glEnd();
}

std::vector<std::pair<std::pair<float,float>, std::pair<float, float> > > vertical,horizontal;
std::vector<std::pair<std::pair<float,float>, std::pair<float, float> > > walls;

int main() {

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "mcqueen", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WIDTH, HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1000, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    srand((unsigned)time(NULL));

    noise::module::Perlin perlin = noise::module::Perlin::Perlin();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE_LOOP);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // mapa
    float delf = 0.01;
    int c = 1;


    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (perlin.GetValue(i * delf+2138, j * delf, 0) < 0.6) {
                road[i][j] = true;
            }
        }
    }

    // 

    float r1 = 140 / 255.0f;
    float g1 = 139 / 255.0f;
    float b1 = 135 / 255.0f;

    float r2 = 104 / 255.0f;
    float g2 = 110 / 255.0f;
    float b2 = 48 / 255.0f;


    glfwSetKeyCallback(window, handleKey);

    while (!glfwWindowShouldClose(window)) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE_LOOP);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();

        // xD
        if (w < 2) {
            w++;
        }
        if (f) {
            for (int i = 0; i < HEIGHT / c; i++) {
                for (int j = 0; j < WIDTH / c; j++) {
                    if (road[i][j]) {
                        glColor4f(r1, g1, b1, 1);
                    }
                    else {
                        glColor4f(0, 0, 0, 1);
                    }
                    rect(i * c, j * c, i * c + c, j * c + c);
                }
            }
        }
        if (w > 1) {
            f = false;
        }
        //

        //samochdozik

      //xd v2

        if (b < 2) {
            b++;
        }
        if (u) {
            for (int i = floor(ex) - 10; i <= floor(ex) + 10; i++) {
                for (int j = floor(ey) - 10; j <= floor(ey) + 10; j++) {
                    if (road[i][j]) {
                        glColor4f(r1, g1, b1, 1);
                    }
                    else {
                        glColor4f(0, 0, 0, 1);
                    }
                    rect(i * c, j * c, i * c + c, j * c + c);
                }
            }
        }
        if (b > 1) {
            u = false;
        }

        for (int i = floor(x) - 10; i <= floor(x) + 10; i++) {
            for (int j = floor(y) - 10; j <= floor(y) + 10; j++) {
                if (road[i][j]) {
                    glColor4f(r1, g1, b1, 1);
                }
                else {
                    glColor4f(0, 0, 0, 1);
                }
                rect(i * c, j * c, i * c + c, j * c + c);
            }
        }

        //
        float ny = y + v * dt * sin(alpha);
        float nx = x + v * dt * cos(alpha);
        nyy = ny;
        nxx = nx;

        if (case1 || case2 || case3) {
            v = v0 + a * (t - t0) * (t - t0) / 2;
        }

        //sciana

        if (!road[nxx][nyy]) {
            ex = nxx;
            ey = nyy;
            glColor3f(1, 0, 0);
            wall = true;
            u = true;
            b = 0;
        }
        else {
            glColor3f(1, 1, 1);

            if (isSpacePressed) {
                glColor3f(0, 1, 0);
            }

            glTranslatef(x + 2, y + 4, 0);
            glRotatef(alpha * 180 / 3.14 - 90, 0, 0, 1);
            rect(-2, -4, 2, 4);
            glRotatef(-alpha * 180 / 3.14 + 90, 0, 0, 1);
            glTranslatef(-x - 2, -y - 4, 0);
        }

        if (wall) {
            a = 0;
            v0 = 0;
            v = 0;
            t0 = t;
            x = xstart;
            y = ystart;
            case1 = false;
            case2 = false;
            case3 = false;
            alpha = M_PI / 2;

        }
        glColor3f(0, 0, 0);
        rect(0, 0, 40, 40);
        wall = false;
        bool drift = false;

        if ((isLeftKeyPressed || isRightKeyPressed) && isSpacePressed) {
            float r = v * cos(0.003);
            if (v * v > 0.7 * r * g) {
                drift = true;
            }
        }

        if (drift) {
            if (isLeftKeyPressed) {
                alpha += 0.003;
            }
            if (isRightKeyPressed) {
                alpha -= 0.003;
            }
        }
        else {
            if (isLeftKeyPressed) {
                alpha += 0.001;
            }
            if (isRightKeyPressed) {
                alpha -= 0.001;
            }
        }
        glColor3f(1, 1, 1);
        if (drift) {
            rect(0, 0, 40, 40);
        }

        if (isUpPressed) {
            if (v > vmax) {
                a = 0;
                v = vmax;
                v0 = vmax;
            }
            else {
                if (!case1) {
                    a = 0.5;
                    case1 = true;
                    case2 = false;
                    case3 = false;
                    v0 = v;
                    t0 = t;
                }
            }
        }
        else {
            if (!isSpacePressed) {
                if (v <= 0) {
                    a = 0;
                    v0 = 0;
                }
                else {
                    if (!case2) {
                        a = -0.01;
                        case1 = false;
                        case2 = true;
                        case3 = false;
                        v0 = v;
                        t0 = t;
                    }
                }
            }
        }
        if (isSpacePressed) {
            if (v < 0) {
                a = 0;
                v0 = 0;
            }
            else {
                if (!case3) {
                    a = -1;
                    case1 = false;
                    case2 = false;
                    case3 = true;
                    v0 = v;
                    t0 = t;
                }
            }
        }
        if (drift) {
            v *= 1.4;
        }
        y += v * dt * sin(alpha);
        x += v * dt * cos(alpha);
        t += dt;

        glPopMatrix();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
