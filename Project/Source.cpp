#define _USE_MATH_DEFINES
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"

static int shoulder = 0, elbow = 0, hand = 0;
//angle of rotation for the camera direction
float angle = 0.0f;
int _angle = 0;

// actual vector representing the camera's direction
float lx = 0.0f, lz = 1.0f;

//XZ position of the camera
float x = 0.0f, z = 4.0f;

//the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

void changeSize(int w, int h)
{
    //Pervent a divide by zero when window is too small
    //(you cant make a window of zero width)
    if (h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

    //use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    //reset matrix
    glLoadIdentity();

    //set the viewport to be the entire window
    glViewport(0, 0, w, h);

    //Set the correct perspective
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // get back to the modelview
    glMatrixMode(GL_MODELVIEW);
}


void drawCylinder(float radius, float height, int numSegments, float r, float g, float b) {
    float angle = 0.0f;
    float segmentAngle = 2.0 * M_PI / numSegments;

    // Draw top circle
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex3f(0.0f, height / 2, 0.0f); // Center of the circle
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * segmentAngle;
        float x = radius * sin(angle);
        float z = radius * cos(angle);
        glVertex3f(x, height / 2, z);
    }
    glEnd();

    // Draw bottom circle
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex3f(0.0f, -height / 2, 0.0f); // Center of the circle
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * segmentAngle;
        float x = radius * sin(angle);
        float z = radius * cos(angle);
        glVertex3f(x, -height / 2, z);
    }
    glEnd();

    // Draw side surface
    glBegin(GL_QUAD_STRIP);
    glColor3f(r, g, b);
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * segmentAngle;
        float x = radius * sin(angle);
        float z = radius * cos(angle);

        glVertex3f(x, height / 2, z);
        glVertex3f(x, -height / 2, z);
    }
    glEnd();
}

void arm() {
    glPushMatrix();
    //pivot sholder
    glTranslatef(0.0f, 0.25f, 0.0f);
    glRotatef((GLfloat)shoulder, 1.0, 0.0, 0.0);
    glTranslatef(0.0f, -0.25f, 0.0f);
    //sholder cloth
    glColor3f(1.0f, 1.0f, 1.0f);
    glRotatef(270, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.2f, 0.5f, 50, 50);
    //sholder
    glRotatef(-270, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -0.3f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    drawCylinder(0.1f, 0.6f, 32, 1.0f, 0.8f, 0.0f);

    //pivot elbow
    glTranslatef(0.0f, -0.25f, 0.0f);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    glRotatef((GLfloat)elbow, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, -0.45f, 0.0f);
    //elbow
    drawCylinder(0.1f, 0.8f, 32, 1.0f, 0.8f, 0.0f);

    //pivot hand
    glTranslatef(0.0f, -0.45f, 0.0f);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    glRotatef((GLfloat)hand, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, -0.05f, 0.0f);
    //hand
    glutSolidSphere(0.1f, 50, 50);

    //fingers
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.1f);
    glRotatef(45, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.02f, 0.3f, 32, 1.0f, 0.8f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.2f, -0.05f);
    drawCylinder(0.02f, 0.2f, 32, 1.0f, 0.8f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.2f, 0.0f);
    drawCylinder(0.02f, 0.2f, 32, 1.0f, 0.8f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.2f, 0.05f);
    drawCylinder(0.02f, 0.2f, 32, 1.0f, 0.8f, 0.0f);
    glPopMatrix();
    glPopMatrix();
}

void leg() {
    //first leg
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    //upper leg
    glTranslatef(0.0f, 4.0f, -0.5f);
    glScalef(2.0f, 1.0f, 2.0f);
    drawCylinder(0.1f, 0.5f, 32, 0.4f, 0.2f, 0.05f);

    //lower leg
    glTranslatef(0.0f, -0.75f, 0.0f);
    glScalef(0.5f, 1.0f, 0.5f);
    drawCylinder(0.1f, 1.0f, 32, 1.0f, 1.0f, 0.0f); //yellow
    glTranslatef(0.0f, -0.6f, 0.0f);
    drawCylinder(0.1f, 0.2f, 32, 1.0f, 1.0f, 1.0f); //white
    glTranslatef(0.0f, -0.125f, 0.0f);
    drawCylinder(0.1f, 0.05f, 32, 0.0f, 0.0f, 1.0f); //blue
    glTranslatef(0.0f, -0.05f, 0.0f);
    drawCylinder(0.1f, 0.05f, 32, 1.0f, 1.0f, 1.0f); //white
    glTranslatef(0.0f, -0.05f, 0.0f);
    drawCylinder(0.1f, 0.05f, 32, 1.0f, 0.0f, 0.0f); //red
    glTranslatef(0.0f, -0.175f, 0.0f);
    drawCylinder(0.1f, 0.3f, 32, 1.0f, 1.0f, 1.0f); //white\

    //shoe
    glTranslatef(-0.2f, -0.3f, 0.0f);
    glScalef(2.0, 1.0, 1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidCube(0.3f);
    glPopMatrix();

    //second leg
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    //upper leg
    glTranslatef(0.0f, 4.0f, 0.5f);
    glScalef(2.0f, 1.0f, 2.0f);
    drawCylinder(0.1f, 0.5f, 32, 0.4f, 0.2f, 0.05f);

    //lower leg
    glTranslatef(0.0f, -0.75f, 0.0f);
    glScalef(0.5f, 1.0f, 0.5f);
    drawCylinder(0.1f, 1.0f, 32, 1.0f, 1.0f, 0.0f); //yellow
    glTranslatef(0.0f, -0.6f, 0.0f);
    drawCylinder(0.1f, 0.2f, 32, 1.0f, 1.0f, 1.0f); //white
    glTranslatef(0.0f, -0.125f, 0.0f);
    drawCylinder(0.1f, 0.05f, 32, 0.0f, 0.0f, 1.0f); //blue
    glTranslatef(0.0f, -0.05f, 0.0f);
    drawCylinder(0.1f, 0.05f, 32, 1.0f, 1.0f, 1.0f); //white
    glTranslatef(0.0f, -0.05f, 0.0f);
    drawCylinder(0.1f, 0.05f, 32, 1.0f, 0.0f, 0.0f); //red
    glTranslatef(0.0f, -0.175f, 0.0f);
    drawCylinder(0.1f, 0.3f, 32, 1.0f, 1.0f, 1.0f); //white\

    //shoe
    glTranslatef(-0.2f, -0.3f, 0.0f);
    glScalef(2.0, 1.0, 1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidCube(0.3f);
    glPopMatrix();
}

void body()
{
    glPushMatrix();
    //main body
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.2f, 0.5f);
    glutSolidCube(2.0f);
    glColor3f(1.0f, 0.9f, 0.4f);
    glScalef(1.0f, 0.8f, 1.5f);

    //left eyes
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-0.5f, 0.5f, 0.7f);
    glScalef(1.0f, 1.0f, 0.1f);
    glutSolidSphere(0.4f, 100, 100);
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.5f);
    glutSolidSphere(0.2f, 100, 100);
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.2f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glTranslatef(-0.1f, 0.4f, 0.3f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.5f, 0.0f);
    for (int i = 0; i < 3; i++) {
        glutSolidCube(0.5f);
        if (i == 3) {
            break;
        }
        glTranslatef(0.7f, 0.0f, 0.0f);
    }
    glPopMatrix();

    //right eyes
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.5f, 0.5f, 0.7f);
    glScalef(1.0f, 1.0f, 0.1f);
    glutSolidSphere(0.4f, 100, 100);
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.5f);
    glutSolidSphere(0.2f, 100, 100);
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.2f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glTranslatef(-0.1f, 0.4f, 0.3f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.5f, 0.0f);
    for (int i = 0; i < 3; i++) {
        glutSolidCube(0.5f);
        if (i == 3) {
            break;
        }
        glTranslatef(0.7f, 0.0f, 0.0f);
    }
    glPopMatrix();

    //nose
    glPushMatrix();
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 1.0f, 0.0f);
    drawCylinder(0.1f, 1.0f, 32, 1.0f, 0.7f, 0.0f);
    glPopMatrix();

    //holes
    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(0.8f, 1.0f, 0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.15f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(-0.8f, 1.0f, 0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(0.7f, -0.6f, 0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.15f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(0.7f, -0.9f, 0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(-0.7f, -0.75f, 0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(-0.7f, -1.0f, 0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.15f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(0.8f, 1.0f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.15f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(-0.8f, 1.0f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(0.7f, -0.6f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.15f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(0.7f, -0.9f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(-0.7f, -0.75f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(-0.7f, -1.0f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.15f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(0.2f, 0.8f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.3f, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.6f, 0.1f);
    glTranslatef(-0.1f, -0.5f, -0.7f);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.15f, 100, 100);
    glPopMatrix();

    //mouth
    glPushMatrix();
    glColor3f(0.4f, 0.1f, 0.1f);
    glTranslatef(0.0f, -0.4f, 0.7f);
    glScalef(1.2f, 0.8f, 0.1f);
    glutSolidCube(0.5f);

    glPushMatrix();
    glColor3f(1.0f, 0.5f, 0.5f);
    glTranslatef(0.0f, -0.3f, 0.0f);
    glScalef(1.0f, 0.4f, 0.0f);
    glutSolidCube(0.5f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.8f, 1.2f, 1.0f);
    glScalef(0.5f, 1.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-0.2f, 0.15f, 0.2f);
    glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.8f, 1.2f, 1.0f);
    glScalef(0.5f, 1.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.2f, 0.15f, 0.2f);
    glutSolidCube(0.2f);
    glPopMatrix();
    glPopMatrix();

    //clothes
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, -1.4f, 0.0f);
    glPushMatrix();
    glScalef(1.0f, 0.1f, 0.5f);
    glutSolidCube(2.0f);
    glPopMatrix();
    glColor3f(0.4f, 0.2f, 0.05f);
    glTranslatef(0.0f, -0.4f, 0.0f);
    glPushMatrix();
    glScalef(1.0f, 0.2f, 0.5f);
    glutSolidCube(2.0f);
    glPopMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.4f, 0.7f);
    glPushMatrix();
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 0.5f, 0.0f);
    glutSolidCube(0.5f);
    glPopMatrix();
    glPopMatrix();
}

void drawCharacter() {
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, -3.0f);
    glRotatef(_angle, 0.0f, 1.0f, 0.0f);
    body();
    glPushMatrix();
    glTranslatef(-1.0f, -1.5f, 0.0f);
    arm();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.0f, -1.5f, 0.0f);
    arm();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -6.0f, 0.0f);
    leg();
    glPopMatrix();
    glPopMatrix();
}

void computePos(float deltaMove)
{
    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

//Makes the image into a texture and returns the id of the texture
GLuint loadTexture(Image* image) 
{
    GLuint textureId;
    glGenTextures(1, &textureId);//make room for textures
    glBindTexture(GL_TEXTURE_2D, textureId); //tell oengl which texture to edit
    //map the image to the texture
    glTexImage2D(GL_TEXTURE_2D, // always gl texture 2d
        0, //0 for now
        GL_RGB, //format opengl uses for image
        image->width, image->height, // width and height
        0, //the border of the image
        GL_RGB, // gl rgb because pixels are stored in rgb format
        GL_UNSIGNED_BYTE, //gl_unsigned_byte becase pixels are stored as unsigned numbers
        image->pixels); //the actual pixel data
    return textureId;
}

GLuint _textureId1;
GLuint _textureId2;
GLuint _textureId3;
GLuint _textureId4;
GLuint _textureId5;
GLuint _textureId6;

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    Image* image1 = loadBMP("C:\\Users\\jxhua\\Desktop\\codes\\Project\\Project\\images\\bg.bmp");
    _textureId1 = loadTexture(image1);
    delete image1;

    Image* image2 = loadBMP("C:\\Users\\jxhua\\Desktop\\codes\\Project\\Project\\images\\bg.bmp");
    _textureId2 = loadTexture(image2);
    delete image2;

    Image* image3 = loadBMP("C:\\Users\\jxhua\\Desktop\\codes\\Project\\Project\\images\\bg.bmp");
    _textureId3 = loadTexture(image3);
    delete image3;

    Image* image4 = loadBMP("C:\\Users\\jxhua\\Desktop\\codes\\Project\\Project\\images\\bg.bmp");
    _textureId4 = loadTexture(image4);
    delete image4;

    Image* image5 = loadBMP("C:\\Users\\jxhua\\Desktop\\codes\\Project\\Project\\images\\sky.bmp");
    _textureId5 = loadTexture(image5);
    delete image5;

    Image* image6 = loadBMP("C:\\Users\\jxhua\\Desktop\\codes\\Project\\Project\\images\\ground.bmp");
    _textureId6 = loadTexture(image6);
    delete image6;
}

void bg() 
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    //front
    glColor3f(1.0, 1.0, 1.0);
    glScaled(1.5, 0.6, 1.5);
    glTranslated(1, -2, 0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, -1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-10.0f, -10.0f, -10.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-10.0f, 10.0f, -10.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(10.0f, 10.0f, -10.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(10.0f, -10.0f, -10.0f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //back
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-10.0f, -10.0f, 10.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-10.0f, 10.0f, 10.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(10.0f, 10.0f, 10.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(10.0f, -10.0f, 10.0f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //right
    glBegin(GL_QUADS);
    glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(10.0f, -10.0f, -10.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(10.0f, 10.0f, -10.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(10.0f, 10.0f, 10.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(10.0f, -10.0f, 10.0f);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //left
    glBegin(GL_QUADS);
    glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-10.0f, -10.0f, -10.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-10.0f, 10.0f, -10.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(-10.0f, 10.0f, 10.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(-10.0f, -10.0f, 10.0f);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId5);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //top
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-10.0f, 10.0f, -10.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-10.0f, 10.0f, 10.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(10.0f, 10.0f, 10.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(10.0f, 10.0f, -10.0f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId6);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //bottom
    glBegin(GL_QUADS);
    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-10.0f, -10.0f, -10.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-10.0f, -10.0f, 10.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(10.0f, -10.0f, 10.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(10.0f, -10.0f, -10.0f);
    glEnd();

}


void renderScene(void)
{
    if (deltaMove)
        computePos(deltaMove);

    //clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //reset transformations
    glLoadIdentity();

    //set the camera
    gluLookAt(x, 0.0f, z,
        x + lx, 0.0f, z + lz,
        0.0f, 1.0f, 0.0f);

    glTranslatef(-1.0, 1.5, -4.0);

    GLfloat ambientLight[] = { 1.0,1.0,1.0,1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    bg();

    glScalef(1.0f, 2.0f, 2.0f);
    glTranslatef(0.0f, -1.5f, 1.0f);
    drawCharacter();

    glutSwapBuffers();
}

void elbowAdd(void)
{
    elbow = (elbow + 5) % 360;
}
void elbowSubtract(void)
{
    elbow = (elbow - 5) % 360;
}
void shoulderAdd(void)
{
    shoulder = (shoulder + 5) % 360;
}
void shoulderSubtract(void)
{
    shoulder = (shoulder - 5) % 360;
}
void handAdd(void)
{
    if (hand < 90)
        hand = (hand + 5) % 360;
}
void handSubtract(void)
{
    if (hand > 0)
        hand = (hand - 5) % 360;
}

void processNormalKeys(unsigned char key, int xx, int yy)
{
    switch (key)
    {
    case'a':
        shoulderSubtract();
        glutPostRedisplay();
        break;
    case'A':
        shoulderAdd();
        glutPostRedisplay();
        break;
    case's':
        elbowSubtract();
        glutPostRedisplay();
        break;
    case'S':
        elbowAdd();
        glutPostRedisplay();
        break;
    case'd':
        handSubtract();
        glutPostRedisplay();
        break;
    case'D':
        handAdd();
        glutPostRedisplay();
        break;
    case'z':
        _angle = (_angle + 5) % 360;
        glutPostRedisplay();
        break;
    case'x':
        PlaySound(TEXT("C:\\Users\\jxhua\\Desktop\\codes\\Project\\Project\\music\\theme.wav"), NULL, SND_LOOP | SND_ASYNC);
        break;
    case'X':
        PlaySound(NULL, 0, 0);
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void pressKey(int key, int xx, int yy)
{
    switch (key)
    {
    case GLUT_KEY_UP: deltaMove = 0.5f; break;
    case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
    }
}

void releaseKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
        deltaMove = 0;
        break;
    }
}

void mouseMove(int x, int y)
{
    //this will only be true when the left mouse button is down
    if (xOrigin >= 0)
    {
        //update deltaAngle
        deltaAngle = (x - xOrigin) * 0.001f;

        //update camera's direction
        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
    }
}


void mouseButton(int button, int state, int x, int y)
{

    //only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON)
    {
        //when the button is released
        if (state == GLUT_UP)
        {
            angle += deltaAngle;
            xOrigin = -1;
        }
        else // state = GLUT_DOWN
        {
            xOrigin = x;
        }
    }
}

int main(int argc, char** argv)
{
    //initialize GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Project");
    initRendering();

    //register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);

    //here are the two new functions
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    //OpenGL init
    glEnable(GL_DEPTH_TEST);

    //enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}