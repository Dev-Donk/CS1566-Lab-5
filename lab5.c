/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include <stdio.h>
#include <stdbool.h>
#include "initShader.h"
#include "myLib.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

vec4 vertices[33] =
{{ 0.0,  0.5,  0.0, 1.0},	
 {-0.5, -0.5,  0.0, 1.0},	
 { 0.5, -0.5,  0.0, 1.0}};

vec4 colors[3] = 
{{1.0, 1.0, 1.0, 1.0},
 {1.0, 1.0, 1.0, 1.0},
 {1.0, 1.0, 1.0, 1.0}};

int num_vertices = 3;

mat4 my_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
GLuint ctm_location; // Almost like an index

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    ctm_location = glGetUniformLocation(program, "ctm");
    printf("ctm_location: %d\n", ctm_location); // Good to check what the location is. If uniform variable is wrong, then it would return -1

    //my_ctm = identity();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &my_ctm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

bool is_leftClicking = false;
float s_factor = 1.0;
float glx = 0.0;
float gly = 0.0;

void motion(int x, int y) {

    if(is_leftClicking) {
        glx = (x / 256.0) - 1;
        gly = 1 - (y / 256.0);

        my_ctm = mul_m4_to_m4(translate_m4(glx, gly, 0.0), scale_m4(s_factor, s_factor, s_factor));

        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    //printf("%d %d %d %d\n", button, state, x, y);
        
    glx = glx;
    gly = gly;

    if(button == 0 && state == 0) {
        is_leftClicking = true;
        glx = (x / 256.0) - 1;
        gly = 1 - (y / 256.0);
    } else if (button == 0 && state == 1)
        is_leftClicking = false;

    if(button == 2 && state == 0)
        s_factor = 0.5;
    else
        s_factor = 1.0;

    my_ctm = mul_m4_to_m4(translate_m4(glx, gly, 0.0), scale_m4(s_factor, s_factor, s_factor));

    glutPostRedisplay();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lab 5");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
