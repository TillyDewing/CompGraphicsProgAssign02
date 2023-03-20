//Tilly Dewing - Computer Graphics Assignment 2 - 3/20/2023
//Sam Houston State University - COSC 4332 - Dr. Islam

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/*  Set initial display-window size. */
GLsizei winWidth = 400, winHeight = 400;

/*  Set range for world coordinates.  */
GLfloat xwcMin = -250, xwcMax = 250;
GLfloat ywcMin = -250, ywcMax = 250;
//number of Verts to transform
CONST GLint nVerts = 4;

struct wcPt2D
{
   public:
      GLfloat x, y;
};

//Copy v1 to v2
void CopyVerts(wcPt2D* v1, wcPt2D* v2)
{
    for(int i = 0; i < nVerts; i++)
    {
        v2[i] = v1[i];
    }
}

typedef GLfloat Matrix3x3 [3][3];

Matrix3x3 matComposite;
const GLdouble pi = 3.14159;

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
}

/* Construct the 3 by 3 identity matrix. */
void matrix3x3SetIdentity (Matrix3x3 matIdent3x3)
{
   GLint row, col;

   for (row = 0; row < 3; row++)
      for (col = 0; col < 3; col++)
         matIdent3x3 [row][col] = (row == col);
}

/* Premultiply matrix m1 times matrix m2, store result in m2. */
void matrix3x3PreMultiply (Matrix3x3 m1, Matrix3x3 m2)
{
   GLint row, col;
   Matrix3x3 matTemp;

   for (row = 0; row < 3; row++)
   {
      for (col = 0; col < 3 ; col++)
      {
         matTemp [row][col] = m1 [row][0] * m2 [0][col] + m1 [row][1] * m2 [1][col] + m1 [row][2] * m2 [2][col];
      }
   }
   for (row = 0; row < 3; row++)
   {
      for (col = 0; col < 3; col++)
      {
         m2 [row][col] = matTemp [row][col];
      }
   }
}

void translate2D (GLfloat tx, GLfloat ty)
{
    Matrix3x3 matTransl;

    /*  Initialize translation matrix to identity.  */
    matrix3x3SetIdentity (matTransl);

    matTransl [0][2] = tx;
    matTransl [1][2] = ty;

    /*  Concatenate matTransl with the composite matrix.  */
    matrix3x3PreMultiply (matTransl, matComposite);
}

void rotate2D (wcPt2D pivotPt, GLfloat theta)
{
    Matrix3x3 matRot;

    /*  Initialize rotation matrix to identity.  */
    matrix3x3SetIdentity (matRot);

    matRot [0][0] = cos (theta);
    matRot [0][1] = -sin (theta);
    matRot [0][2] = pivotPt.x * (1 - cos (theta)) + pivotPt.y * sin (theta);
    matRot [1][0] = sin (theta);
    matRot [1][1] = cos (theta);
    matRot [1][2] = pivotPt.y * (1 - cos (theta)) - pivotPt.x * sin (theta);

    /*  Concatenate matRot with the composite matrix.  */
    matrix3x3PreMultiply (matRot, matComposite);
}

void scale2D (GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
    Matrix3x3 matScale;

    /*  Initialize scaling matrix to identity.  */
    matrix3x3SetIdentity (matScale);

    matScale [0][0] = sx;
    matScale [0][2] = (1 - sx) * fixedPt.x;
    matScale [1][1] = sy;
    matScale [1][2] = (1 - sy) * fixedPt.y;

    /*  Concatenate matScale with the composite matrix.  */
    matrix3x3PreMultiply (matScale, matComposite);
}

void shear2D (GLfloat shX, GLfloat yRef)
{
    Matrix3x3 matSheer;
    matrix3x3SetIdentity (matSheer);

    matSheer[0][1] = shX;
    matSheer[0][2] = -shX * yRef;

    /*  Concatenate matSheer with the composite matrix.  */
    matrix3x3PreMultiply (matSheer, matComposite);
}

/* Using the composite matrix, calculate transformed coordinates. */
void transformVerts2D (GLint nVerts, wcPt2D * verts)
{
    GLint k;
    GLfloat temp;

    for (k = 0; k < nVerts; k++)
    {
        temp = matComposite [0][0] * verts [k].x + matComposite [0][1] * verts [k].y + matComposite [0][2];
        verts [k].y = matComposite [1][0] * verts [k].x + matComposite [1][1] * verts [k].y + matComposite [1][2];
        verts [k].x = temp;
   }
}

void Quad (wcPt2D *verts) //Draw a filled quad
{
    GLint k;

    glBegin (GL_QUADS);
    for (k = 0; k < 4; k++)
    {
        glVertex2f (verts[k].x, verts[k].y);
    }
    glEnd ( );
}

wcPt2D preTranVerts [nVerts] = {{10.0,50.0}, {10.0,10.0}, {50.0,10.0}, {50.0,50.0}}; //Points of the quad prior to transformation

void GetUserInput() //Prompt user for points of quad.
{
    printf("Enter %i points for a quad (-250 to 250) : x,y \n", nVerts);
    for(int i = 0; i < nVerts; i++)
    {
        float x,y = 0;
        scanf("%f,%f", &x, &y);
        preTranVerts[i].x = x;
        preTranVerts[i].y = y;
    }
}

void DrawAxis() //Draw lines for x and y axis (to better illustrate transformations)
{
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    //X axis
    glVertex2f(xwcMin,0);
    glVertex2f(xwcMax,0);
    //Y axis
    glVertex2f(0,ywcMin);
    glVertex2f(0,ywcMax);
    glEnd();
}

void display1(void) //rotate 45deg
{
    //init
    glClear (GL_COLOR_BUFFER_BIT);
    DrawAxis();
    //Draw Axis
    glColor3f (0.8, 0.8, 1.0); //light blue for pre trans.
    Quad(preTranVerts); //Draw shape pre transform

    //Rotate
    wcPt2D postTranVerts[4]; //temp transform storage.
    CopyVerts(preTranVerts, postTranVerts);

    GLdouble theta = 45.0 * (M_PI / 180.0); //45 deg
    matrix3x3SetIdentity (matComposite); // set trans mat to identity
    wcPt2D pivPt = {0,0}; //rotate about origin
    rotate2D (pivPt, theta); //rotate
    transformVerts2D (nVerts, postTranVerts); //apply transform mat.

    //Draw transformed object
    glColor3f (1.0, 0.3, 0.3); //red for post trans.
    Quad(postTranVerts);

    glFlush();
}

void display2(void) //scale by 3,2
{
    glClear (GL_COLOR_BUFFER_BIT);
    DrawAxis();
    glColor3f (0.8, 0.8, 1.0);
    Quad(preTranVerts); //Draw shape pre transform

    wcPt2D postTranVerts[4]; //temp transform storage.
    CopyVerts(preTranVerts, postTranVerts);

    //get center for scaling
    wcPt2D centroidPt;
    GLint k, xSum = 0, ySum = 0;
    for (k = 0; k < nVerts;  k++)
    {
        xSum += preTranVerts [k].x;
        ySum += preTranVerts [k].y;
    }
    centroidPt.x = GLfloat (xSum) / GLfloat (nVerts);
    centroidPt.y = GLfloat (ySum) / GLfloat (nVerts);

    matrix3x3SetIdentity (matComposite); // set trans mat to identity
    scale2D(3.0, 2.0, centroidPt);
    transformVerts2D (nVerts, postTranVerts); //apply transform mat.

    //Draw transformed object
    glColor3f (1.0, 0.3, 0.3); //red for post trans.
    Quad(postTranVerts);

    glFlush();
}

void display3(void) //scale a point by 2,3
{
    glClear (GL_COLOR_BUFFER_BIT);
    DrawAxis();
    glColor3f (0.8, 0.8, 1.0);
    Quad(preTranVerts); //Draw shape pre transform

    wcPt2D postTranVerts[4]; //temp transform storage.
    CopyVerts(preTranVerts, postTranVerts);

    postTranVerts[3] = {(postTranVerts[3].x * 1.5), (postTranVerts[3].y * 2.0)};

    //Draw transformed object
    glColor3f (1.0, 0.3, 0.3); //red for post trans.
    Quad(postTranVerts);

    glFlush();
}

void display4(void) //reflect across origin
{
    glClear (GL_COLOR_BUFFER_BIT);
    DrawAxis();
    glColor3f (0.8, 0.8, 1.0);
    Quad(preTranVerts); //Draw shape pre transform

    wcPt2D postTranVerts[4]; //temp transform storage.
    CopyVerts(preTranVerts, postTranVerts);

    for(int i = 0; i < nVerts; i++)
    {
        postTranVerts[i] = {(postTranVerts[i].x * -1.0), (postTranVerts[i].y * -1.0)};
    }

    //Draw transformed object
    glColor3f (1.0, 0.3, 0.3); //red for post trans.
    Quad(postTranVerts);

    glFlush();
}
void display5(void) //sheer Shx = 2, relative to a reference line y = -1
{
    glClear (GL_COLOR_BUFFER_BIT);
    DrawAxis();
    glColor3f (0.8, 0.8, 1.0);
    Quad(preTranVerts); //Draw shape pre transform

    wcPt2D postTranVerts[4]; //temp transform storage.
    CopyVerts(preTranVerts, postTranVerts);

    matrix3x3SetIdentity (matComposite); // set trans mat to identity
    shear2D(2.0,-1.0);
    transformVerts2D (nVerts, postTranVerts); //apply transform mat.

    //Draw transformed object
    glColor3f (1.0, 0.3, 0.3); //red for post trans.
    Quad(postTranVerts);

    glFlush();
}
void displayFcn (void) //NOT USED
{
   /*  Define initial position for triangle.  */
   //wcPt2D verts [3] = { {50.0, 25.0}, {150.0, 25.0}, {100.0, 100.0} };

   /*  Calculate position of triangle centroid.  */
   wcPt2D centroidPt;

   GLint k, xSum = 0, ySum = 0;
   for (k = 0; k < nVerts;  k++) {
      xSum += preTranVerts [k].x;
      ySum += preTranVerts [k].y;
   }
   centroidPt.x = GLfloat (xSum) / GLfloat (nVerts);
   centroidPt.y = GLfloat (ySum) / GLfloat (nVerts);

   /*  Set geometric transformation parameters.  */
   wcPt2D pivPt, fixedPt;
   pivPt = centroidPt;
   fixedPt = centroidPt;

   GLfloat tx = 0.0, ty = 100.0;
   GLfloat sx = 0.5, sy = 0.5;
   GLdouble theta = pi/2.0;

   glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

   glColor3f (0.0, 0.0, 1.0);       //  Set initial fill color to blue.
   Quad(preTranVerts);                //  Display blue triangle.

   /*  Initialize composite matrix to identity.  */
   matrix3x3SetIdentity (matComposite);

   /*  Construct composite matrix for transformation sequence.  */
    translate2D (tx, ty);
   rotate2D (pivPt, theta);
   scale2D (sx, sy, fixedPt);   //  First transformation: Scale.
        //  Second transformation: Rotate
         //  Final transformation: Translate.

   /*  Apply composite matrix to triangle vertices.  */
   transformVerts2D (nVerts, preTranVerts);

   glColor3f (1.0, 0.0, 0.0);  // Set color for transformed triangle.
   Quad(preTranVerts);           // Display red transformed triangle.

   glFlush ( );
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);

    glClear (GL_COLOR_BUFFER_BIT);
}

int main (int argc, char ** argv)
{
    //GetUserInput();
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (winWidth, winHeight);

    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Transform 1");
    init ( );
    glutDisplayFunc (display1);
    glutReshapeFunc (winReshapeFcn);

    glutInitWindowPosition (450, 50);
    glutCreateWindow ("Transform 2");
    init ( );
    glutDisplayFunc (display2);
    glutReshapeFunc (winReshapeFcn);

    glutInitWindowPosition (850, 50);
    glutCreateWindow ("Transform 3");
    init ( );
    glutDisplayFunc (display3);
    glutReshapeFunc (winReshapeFcn);

    glutInitWindowPosition (50, 480);
    glutCreateWindow ("Transform 4");
    init ( );
    glutDisplayFunc (display4);
    glutReshapeFunc (winReshapeFcn);

    glutInitWindowPosition (450, 480);
    glutCreateWindow ("Transform 5");
    init ( );
    glutDisplayFunc (display5);
    glutReshapeFunc (winReshapeFcn);


    glutMainLoop ( );
}
