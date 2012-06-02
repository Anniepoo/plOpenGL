/*  $Id$

    plOpenGL - SWI-Prolog OpenGL Bindings

    Author:        Jan Tatham
    E-Mail:        jan@sebity.com
    WWW:           http://www.sebity.com
    Copyright (C): 2012, Jan Tatham.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <SWI-Prolog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define SLEEP(x) Sleep(x)   /* Windows version */
#else
#ifdef __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <unistd.h>
#define SLEEP(x) usleep(x*1000)   /* Linux version */
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <unistd.h>
#define SLEEP(x) usleep(x*1000)   /* Linux version */
#endif
#endif
/* storage for one texture  */
unsigned int texture[3];

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

// extended functions
void c_display(void);
void c_idle(void);
void c_keyboard(unsigned char Key, int X, int Y);
void c_motion(int X, int Y);
void c_mouse(int Button, int State, int X, int Y);
void c_reshape(int W, int H);

int c_imageLoad(char *filename, Image *image);

// standard functions
foreign_t c_sleep(term_t S);
foreign_t c_loadGLTexture(term_t Filename);
foreign_t c_loadGLTextures(term_t Filename, term_t Texture, term_t Width, term_t Height, term_t Data);

// gl functions
foreign_t c_glAccum(term_t Operation, term_t Value);
foreign_t c_glActiveTextureARB(term_t Texture);
foreign_t c_glAlphaFunc(term_t Func, term_t Ref);
foreign_t c_glArrayElement(term_t Index);
foreign_t c_glBegin(term_t Mode);
foreign_t c_glBindTexture(term_t Target, term_t Texture);
foreign_t c_glBitmap(term_t Width, term_t Height, term_t X1, term_t Y1, term_t X2, term_t Y2,
                     term_t Bitmap, term_t Num);
foreign_t c_glBlendFunc(term_t sFactor, term_t dFactor);
foreign_t c_glCallList(term_t List);
foreign_t c_glClear(term_t Mask);
foreign_t c_glClearColor(term_t Red, term_t Green, term_t Blue, term_t Alpha);
foreign_t c_glClearDepth(term_t Depth);
foreign_t c_glClearIndex(term_t Index);
foreign_t c_glClearStencil(term_t S);
foreign_t c_glClipPlane(term_t Plane, term_t Equation, term_t Num);
foreign_t c_glColor3f(term_t Red, term_t Green, term_t Blue);
foreign_t c_glColor4f(term_t Red, term_t Green, term_t Blue, term_t Alpha);
foreign_t c_glCopyPixels(term_t X, term_t Y, term_t Width, term_t Height, term_t Type);
foreign_t c_glCopyTexImage1D(term_t Target, term_t Level, term_t Internal, term_t X, term_t Y,
                             term_t Width, term_t Border);
foreign_t c_glCopyTexImage2D(term_t Target, term_t Level, term_t Internal, term_t X, term_t Y,
                             term_t Width, term_t Height, term_t Border);
foreign_t c_glCullFace(term_t Mode);
foreign_t c_glDepthFunc(term_t Mode);
foreign_t c_glDisable(term_t Mode);
foreign_t c_glEnable(term_t Mode);
foreign_t c_glEnd(void);
foreign_t c_glEndList(void);
foreign_t c_glFinish(void);
foreign_t c_glFlush(void);
foreign_t c_glFogf(term_t PName, term_t Param);
foreign_t c_glFogi(term_t PName, term_t Param);
foreign_t c_glFrustum(term_t Left, term_t Right, term_t Bottom, term_t Top, term_t Near, term_t Far);
foreign_t c_glGenTextures(term_t N, term_t TextureNames, term_t Num);
foreign_t c_glHint(term_t Target, term_t Hint);
foreign_t c_glIndexi(term_t Index);
foreign_t c_glLightfv(term_t Face, term_t PName, term_t Params, term_t Num);
foreign_t c_glLightModelfv(term_t PName, term_t Params, term_t Num);
foreign_t c_glLineStipple(term_t Factor, term_t Pattern);
foreign_t c_glLineWidth(term_t Width);
foreign_t c_glLoadIdentity(void);
foreign_t c_glMaterialfv(term_t Face, term_t PName, term_t Params, term_t Num);
foreign_t c_glMatrixMode(term_t Mode);
foreign_t c_glNewList(term_t List, term_t Mode);
foreign_t c_glOrtho(term_t Left, term_t Right, term_t Bottom, term_t Top, term_t Near, term_t Far);
foreign_t c_glPixelStorei(term_t Mode, term_t Param);
foreign_t c_glPointSize(term_t Size);
foreign_t c_glPopAttrib(void);
foreign_t c_glPopClientAttrib(void);
foreign_t c_glPopMatrix(void);
foreign_t c_glPopName(void);
foreign_t c_glPushAttrib(term_t Mask);
foreign_t c_glPushClientAttrib(term_t Mask);
foreign_t c_glPushMatrix(void);
foreign_t c_glPushName(term_t Name);
foreign_t c_glRasterPos2i(term_t X, term_t Y);
foreign_t c_glRectf(term_t X1, term_t Y1, term_t X2, term_t Y2);
foreign_t c_glRotatef(term_t Angle, term_t X, term_t Y, term_t Z);
foreign_t c_glScalef(term_t X, term_t Y, term_t Z);
foreign_t c_glShadeModel(term_t Mode);
foreign_t c_glStencilFunc(term_t Func, term_t Ref, term_t Mask);
foreign_t c_glStencilMask(term_t Mask);
foreign_t c_glStencilOp(term_t Fail, term_t zFail, term_t zPass);
foreign_t c_glTexCoord2f(term_t S, term_t T);
foreign_t c_glTexImage2D(term_t Target, term_t Level, term_t Internal, term_t Width, term_t Height,
                         term_t Border, term_t Format, term_t Type, term_t Texels);
foreign_t c_glTexParameteri(term_t Target, term_t PName, term_t Param);
foreign_t c_glTranslatef(term_t X, term_t Y, term_t Z);
foreign_t c_glVertex2f(term_t X, term_t Y);
foreign_t c_glVertex2i(term_t X, term_t Y);
foreign_t c_glVertex3d(term_t X, term_t Y, term_t Z);
foreign_t c_glVertex3f(term_t X, term_t Y, term_t Z);
foreign_t c_glVertex3i(term_t X, term_t Y, term_t Z);
foreign_t c_glViewport(term_t X, term_t Y, term_t Width, term_t Height);

// glu functions
foreign_t c_gluLookAt(term_t EyeX, term_t EyeY, term_t EyeZ,
                      term_t CenterX, term_t CenterY, term_t CenterZ,
                      term_t UpX, term_t UpY, term_t UpZ);
foreign_t c_gluOrtho2D(term_t Left, term_t Right, term_t Bottom, term_t Top);
foreign_t c_gluPerspective(term_t Fovy, term_t Aspect, term_t Near, term_t Far);


// glut functions
foreign_t c_glutCreateWindow(term_t String);
foreign_t c_glutDestroyWindow(void);
foreign_t c_glutDisplayFunc(void);
foreign_t c_glutIdleFunc(term_t String);
foreign_t c_glutInit(void);
foreign_t c_glutInitDisplayMode(term_t DisplayMode);
foreign_t c_glutInitWindowPosition(term_t X, term_t Y);
foreign_t c_glutInitWindowSize(term_t Width, term_t Height);
foreign_t c_glutKeyboardFunc(void);
foreign_t c_glutMainLoop(void);
foreign_t c_glutMotionFunc(void);
foreign_t c_glutMouseFunc(void);
foreign_t c_glutPostRedisplay(void);
foreign_t c_glutReshapeFunc(void);
foreign_t c_glutSetColor(term_t Index, term_t Red, term_t Green, term_t Blue);
foreign_t c_glutSolidCone(term_t Radius, term_t Height, term_t Slices, term_t Stacks);
foreign_t c_glutSolidCube(term_t Size);
foreign_t c_glutSolidSphere(term_t Radius, term_t Slices, term_t Stacks);
foreign_t c_glutSolidTeapot(term_t Size);
foreign_t c_glutSolidTorus(term_t InnerRadius, term_t OuterRadius, term_t NSides, term_t Rings);
foreign_t c_glutSwapBuffers(void);
foreign_t c_glutWireCone(term_t Radius, term_t Height, term_t Slices, term_t Stacks);
foreign_t c_glutWireCube(term_t Size);
foreign_t c_glutWireSphere(term_t Radius, term_t Slices, term_t Stacks);
foreign_t c_glutWireTeapot(term_t Size);
foreign_t c_glutWireTorus(term_t InnerRadius, term_t OuterRadius, term_t NSides, term_t Rings);


install_t install() {
  PL_register_foreign("c_sleep",1,c_sleep,PL_FA_NOTRACE);
  PL_register_foreign("c_loadGLTexture",1,c_loadGLTexture,PL_FA_NOTRACE);
  PL_register_foreign("c_loadGLTextures",5,c_loadGLTextures,PL_FA_NOTRACE);

  PL_register_foreign("c_glAccum",2,c_glAccum,PL_FA_NOTRACE);
  PL_register_foreign("c_glActiveTextureARB",1,c_glActiveTextureARB,PL_FA_NOTRACE);
  PL_register_foreign("c_glAlphaFunc",2,c_glAlphaFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glArrayElement",1,c_glArrayElement,PL_FA_NOTRACE);
  PL_register_foreign("c_glBegin",1,c_glBegin,PL_FA_NOTRACE);
  PL_register_foreign("c_glBindTexture",2,c_glBindTexture,PL_FA_NOTRACE);
  PL_register_foreign("c_glBitmap",8,c_glBitmap,PL_FA_NOTRACE);
  PL_register_foreign("c_glBlendFunc",2,c_glBlendFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glCallList",1,c_glCallList,PL_FA_NOTRACE);
  PL_register_foreign("c_glClear",1,c_glClear,PL_FA_NOTRACE);
  PL_register_foreign("c_glClearColor",4,c_glClearColor,PL_FA_NOTRACE);
  PL_register_foreign("c_glClearDepth",1,c_glClearDepth,PL_FA_NOTRACE);
  PL_register_foreign("c_glClearIndex",1,c_glClearIndex,PL_FA_NOTRACE);
  PL_register_foreign("c_glClearStencil",1,c_glClearStencil,PL_FA_NOTRACE);
  PL_register_foreign("c_glClipPlane",3,c_glClipPlane,PL_FA_NOTRACE);
  PL_register_foreign("c_glColor3f",3,c_glColor3f,PL_FA_NOTRACE);
  PL_register_foreign("c_glColor4f",4,c_glColor4f,PL_FA_NOTRACE);
  PL_register_foreign("c_glCopyPixels",5,c_glCopyPixels,PL_FA_NOTRACE);
  PL_register_foreign("c_glCopyTexImage1D",7,c_glCopyTexImage1D,PL_FA_NOTRACE);
  PL_register_foreign("c_glCopyTexImage2D",8,c_glCopyTexImage2D,PL_FA_NOTRACE);
  PL_register_foreign("c_glCullFace",1,c_glCullFace,PL_FA_NOTRACE);
  PL_register_foreign("c_glDepthFunc",1,c_glDepthFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glDisable",1,c_glDisable,PL_FA_NOTRACE);
  PL_register_foreign("c_glEnable",1,c_glEnable,PL_FA_NOTRACE);
  PL_register_foreign("c_glEnd",0,c_glEnd,PL_FA_NOTRACE);
  PL_register_foreign("c_glEndList",0,c_glEndList,PL_FA_NOTRACE);
  PL_register_foreign("c_glFinish",0,c_glFinish,PL_FA_NOTRACE);
  PL_register_foreign("c_glFlush",0,c_glFlush,PL_FA_NOTRACE);
  PL_register_foreign("c_glFogf",2,c_glFogf,PL_FA_NOTRACE);
  PL_register_foreign("c_glFogi",2,c_glFogi,PL_FA_NOTRACE);
  PL_register_foreign("c_glFrustum",6,c_glFrustum,PL_FA_NOTRACE);
  PL_register_foreign("c_glGenTextures",3,c_glGenTextures,PL_FA_NOTRACE);
  PL_register_foreign("c_glHint",2,c_glHint,PL_FA_NOTRACE);
  PL_register_foreign("c_glIndexi",1,c_glIndexi,PL_FA_NOTRACE);
  PL_register_foreign("c_glLightfv",4,c_glLightfv,PL_FA_NOTRACE);
  PL_register_foreign("c_glLightModelfv",3,c_glLightModelfv,PL_FA_NOTRACE);
  PL_register_foreign("c_glLineStipple",2,c_glLineStipple,PL_FA_NOTRACE);
  PL_register_foreign("c_glLineWidth",1,c_glLineWidth,PL_FA_NOTRACE);
  PL_register_foreign("c_glLoadIdentity",0,c_glLoadIdentity,PL_FA_NOTRACE);
  PL_register_foreign("c_glMaterialfv",4,c_glMaterialfv,PL_FA_NOTRACE);
  PL_register_foreign("c_glMatrixMode",1,c_glMatrixMode,PL_FA_NOTRACE);
  PL_register_foreign("c_glNewList",2,c_glNewList,PL_FA_NOTRACE);
  PL_register_foreign("c_glOrtho",6,c_glOrtho,PL_FA_NOTRACE);
  PL_register_foreign("c_glPixelStorei",2,c_glPixelStorei,PL_FA_NOTRACE);
  PL_register_foreign("c_glPointSize",1,c_glPointSize,PL_FA_NOTRACE);
  PL_register_foreign("c_glPopAttrib",0,c_glPushAttrib,PL_FA_NOTRACE);
  PL_register_foreign("c_glPopClientAttrib",0,c_glPushClientAttrib,PL_FA_NOTRACE);
  PL_register_foreign("c_glPopMatrix",0,c_glPopMatrix,PL_FA_NOTRACE);
  PL_register_foreign("c_glPopName",0,c_glPushName,PL_FA_NOTRACE);
  PL_register_foreign("c_glPushAttrib",1,c_glPushAttrib,PL_FA_NOTRACE);
  PL_register_foreign("c_glPushClientAttrib",1,c_glPushClientAttrib,PL_FA_NOTRACE);
  PL_register_foreign("c_glPushMatrix",0,c_glPushMatrix,PL_FA_NOTRACE);
  PL_register_foreign("c_glPushName",1,c_glPushName,PL_FA_NOTRACE);
  PL_register_foreign("c_glRasterPos2i",2,c_glRasterPos2i,PL_FA_NOTRACE);
  PL_register_foreign("c_glRectf",4,c_glRectf,PL_FA_NOTRACE);
  PL_register_foreign("c_glRotatef",4,c_glRotatef,PL_FA_NOTRACE);
  PL_register_foreign("c_glScalef",3,c_glScalef,PL_FA_NOTRACE);
  PL_register_foreign("c_glShadeModel",1,c_glShadeModel,PL_FA_NOTRACE);
  PL_register_foreign("c_glStencilMask",1,c_glStencilMask,PL_FA_NOTRACE);
  PL_register_foreign("c_glStencilFunc",3,c_glStencilFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glStencilOp",3,c_glStencilOp,PL_FA_NOTRACE);
  PL_register_foreign("c_glTexCoord2f",2,c_glTexCoord2f,PL_FA_NOTRACE);
  PL_register_foreign("c_glTexImage2D",9,c_glTexImage2D,PL_FA_NOTRACE);
  PL_register_foreign("c_glTexParameteri",3,c_glTexParameteri,PL_FA_NOTRACE);
  PL_register_foreign("c_glTranslatef",3,c_glTranslatef,PL_FA_NOTRACE);
  PL_register_foreign("c_glVertex2f",2,c_glVertex2f,PL_FA_NOTRACE);
  PL_register_foreign("c_glVertex2i",2,c_glVertex2i,PL_FA_NOTRACE);
  PL_register_foreign("c_glVertex3d",3,c_glVertex3d,PL_FA_NOTRACE);
  PL_register_foreign("c_glVertex3f",3,c_glVertex3f,PL_FA_NOTRACE);
  PL_register_foreign("c_glVertex3i",3,c_glVertex3i,PL_FA_NOTRACE);
  PL_register_foreign("c_glViewport",4,c_glViewport,PL_FA_NOTRACE);

  PL_register_foreign("c_gluLookAt",9,c_gluLookAt,PL_FA_NOTRACE);
  PL_register_foreign("c_gluOrtho2D",4,c_gluOrtho2D,PL_FA_NOTRACE);
  PL_register_foreign("c_gluPerspective",4,c_gluPerspective,PL_FA_NOTRACE);

  PL_register_foreign("c_glutCreateWindow",1,c_glutCreateWindow,PL_FA_NOTRACE);
  PL_register_foreign("c_glutDestroyWindow",0,c_glutDestroyWindow,PL_FA_NOTRACE);
  PL_register_foreign("c_glutDisplayFunc",0,c_glutDisplayFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glutIdleFunc",1,c_glutIdleFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glutInit",0,c_glutInit,PL_FA_NOTRACE);
  PL_register_foreign("c_glutInitDisplayMode",1,c_glutInitDisplayMode,PL_FA_NOTRACE);
  PL_register_foreign("c_glutInitWindowPosition",2,c_glutInitWindowPosition,PL_FA_NOTRACE);
  PL_register_foreign("c_glutInitWindowSize",2,c_glutInitWindowSize,PL_FA_NOTRACE);
  PL_register_foreign("c_glutKeyboardFunc",0,c_glutKeyboardFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glutMainLoop",0,c_glutMainLoop,PL_FA_NOTRACE);
  PL_register_foreign("c_glutMotionFunc",0,c_glutMotionFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glutMouseFunc",0,c_glutMouseFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glutPostRedisplay",0,c_glutPostRedisplay,PL_FA_NOTRACE);
  PL_register_foreign("c_glutReshapeFunc",0,c_glutReshapeFunc,PL_FA_NOTRACE);
  PL_register_foreign("c_glutSetColor",4,c_glutSetColor,PL_FA_NOTRACE);
  PL_register_foreign("c_glutSolidCone",4,c_glutSolidCone,PL_FA_NOTRACE);
  PL_register_foreign("c_glutSolidCube",1,c_glutWireCube,PL_FA_NOTRACE);
  PL_register_foreign("c_glutSolidSphere",3,c_glutSolidSphere,PL_FA_NOTRACE);
  PL_register_foreign("c_glutSolidTeapot",1,c_glutSolidTeapot,PL_FA_NOTRACE);
  PL_register_foreign("c_glutSolidTorus",4,c_glutSolidTorus,PL_FA_NOTRACE);
  PL_register_foreign("c_glutSwapBuffers",0,c_glutSwapBuffers,PL_FA_NOTRACE);
  PL_register_foreign("c_glutWireCone",4,c_glutWireCone,PL_FA_NOTRACE);
  PL_register_foreign("c_glutWireCube",1,c_glutWireCube,PL_FA_NOTRACE);
  PL_register_foreign("c_glutWireSphere",3,c_glutWireSphere,PL_FA_NOTRACE);
  PL_register_foreign("c_glutWireTeapot",1,c_glutWireTeapot,PL_FA_NOTRACE);
  PL_register_foreign("c_glutWireTorus",4,c_glutWireTorus,PL_FA_NOTRACE);
}


/* ====================== Other Functions ==================== */

/* Name: c_display
 * Params:
 * Returns:
 */
void c_display(void) {
  //fid_t fid = PL_open_foreign_frame();
  qid_t query_handle;
  static predicate_t p;
  static term_t display_predicate;
  int rc;

  if(!display_predicate)
    display_predicate= PL_new_term_refs(1);

  if(!p)
    p = PL_predicate("display",0,"user");

  query_handle = PL_open_query(NULL,PL_Q_NORMAL,p,display_predicate);
  rc = PL_next_solution(query_handle);

  PL_cut_query(query_handle);
}

/* Name: c_idle
 * Params:
 * Returns:
 */
void c_idle(void) {
  qid_t query_handle;
  static predicate_t p;
  static term_t idle_predicate;
  int rc;
  //char *string;

  //if(!PL_get_atom_chars(PL_String,&temp_string))
  //  return FALSE;
  //string = glut_idle;

  //printf("idle_2:%s\n",string);

  if(!p)
    p = PL_predicate("idle",0,"user");

  if(!idle_predicate)
    idle_predicate = PL_new_term_refs(1);

  query_handle = PL_open_query(NULL,PL_Q_NORMAL,p,idle_predicate);
  rc = PL_next_solution(query_handle);
  PL_cut_query(query_handle);
}

/* Name: c_keyboard
 * Params:
 * Returns:
 */
void c_keyboard(unsigned char PL_Key, int PL_X, int PL_Y) {
  //fid_t fid = PL_open_foreign_frame();
  qid_t query_handle;
  static predicate_t p;
  static term_t keyboard_predicate;
  int a,b,c;
  int rc;

  /*
  printf("K=%d\n",PL_Key);
  printf("X=%d\n",PL_X);
  printf("Y=%d\n",PL_Y);
  */

  if(!p)
    p = PL_predicate("keyboard",3,"user");

  if(!keyboard_predicate)
    keyboard_predicate= PL_new_term_refs(3);

  a = PL_put_integer(keyboard_predicate,PL_Key);
  b = PL_put_integer(keyboard_predicate+1,PL_X);
  c = PL_put_integer(keyboard_predicate+2,PL_Y);

  query_handle = PL_open_query(NULL,PL_Q_NORMAL,p,keyboard_predicate);
  rc = PL_next_solution(query_handle);
  //if (!rc) {
    PL_cut_query(query_handle);
    //PL_close_foreign_frame(fid);
    //}
    //PL_close_query(query_handle);
    //PL_discard_foreign_frame(fid);
}

/* Name: c_motion
 * Params:
 * Returns:
 */
void c_motion(int PL_X, int PL_Y) {
  //fid_t fid = PL_open_foreign_frame();
  qid_t query_handle;
  static predicate_t p;
  static term_t motion_predicate;
  int a,b;
  int rc;

  /*
  printf("X=%d\n",PL_X);
  printf("Y=%d\n",PL_Y);
  */

  if(!p)
    p = PL_predicate("motion",2,"user");

  if(!motion_predicate)
    motion_predicate= PL_new_term_refs(2);

  a = PL_put_integer(motion_predicate,PL_X);
  b = PL_put_integer(motion_predicate+1,PL_Y);

  query_handle = PL_open_query(NULL,PL_Q_NORMAL,p,motion_predicate);
  rc = PL_next_solution(query_handle);
  PL_cut_query(query_handle);
}

/* Name: c_mouse
 * Params:
 * Returns:
 */
void c_mouse(int PL_Button, int PL_State, int PL_X, int PL_Y) {
  //fid_t fid = PL_open_foreign_frame();
  qid_t query_handle;
  static predicate_t p;
  static term_t mouse_predicate;
  int a,b,c,d;
  int rc;

  /*
  printf("B=%d\n",PL_Button);
  printf("S=%d\n",PL_State);
  printf("X=%d\n",PL_X);
  printf("Y=%d\n",PL_Y);
  */

  if(!p)
    p = PL_predicate("mouse",4,"user");

  if(!mouse_predicate)
    mouse_predicate= PL_new_term_refs(4);

  a = PL_put_integer(mouse_predicate,PL_Button);
  b = PL_put_integer(mouse_predicate+1,PL_State);
  c = PL_put_integer(mouse_predicate+2,PL_X);
  d = PL_put_integer(mouse_predicate+3,PL_Y);

  query_handle = PL_open_query(NULL,PL_Q_NORMAL,p,mouse_predicate);
  rc = PL_next_solution(query_handle);
  //if (!rc) {
    PL_cut_query(query_handle);
    //PL_close_foreign_frame(fid);
    //}
    //PL_close_query(query_handle);
    //PL_discard_foreign_frame(fid);
}

/* Name: c_reshape
 * Params:
 * Returns:
 */
void c_reshape(int PL_W, int PL_H) {
  //fid_t fid = PL_open_foreign_frame();
  qid_t query_handle;
  static predicate_t p;
  static term_t reshape_predicate;
  int rc;

  //printf("w=%d\n",PL_W);
  //printf("h=%d\n",PL_H);

  if(!reshape_predicate)
    reshape_predicate= PL_new_term_refs(1);

  if(!p)
    p = PL_predicate("reshape",0,"user");

  query_handle = PL_open_query(NULL,PL_Q_NORMAL,p,reshape_predicate);
  rc = PL_next_solution(query_handle);
  //if (!rc) {
    PL_cut_query(query_handle);
    //PL_close_foreign_frame(fid);
    //}
    //PL_close_query(query_handle);
    //PL_discard_foreign_frame(fid);
}


/* Name: c_sleep
 * Params:
 * Returns:
 */
foreign_t c_sleep(term_t PL_S) {
  int time = 100;

  if(!PL_get_integer(PL_S,&time))
    return FALSE;

  SLEEP(time);

  PL_succeed;
}


int c_imageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1)
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL)
    {
	printf("File Not Found : %s\n",filename);
	return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
	printf("Error reading width from %s.\n", filename);
	return 0;
    }
    printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
	printf("Error reading height from %s.\n", filename);
	return 0;
    }
    printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
	printf("Error reading planes from %s.\n", filename);
	return 0;
    }
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
	printf("Error reading bpp from %s.\n", filename);
	return 0;
    }
    if (bpp != 24) {
	printf("Bpp from %s is not 24: %u\n", filename, bpp);
	return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
	temp = image->data[i];
	image->data[i] = image->data[i+2];
	image->data[i+2] = temp;
    }

    // we're done.
    return 1;
}



/* ====================== Image Functions ==================== */

/* Name: c_loadGLTexture
 * Params:
 * Returns:
 */
foreign_t c_loadGLTexture(term_t PL_Filename) {
  char *filename;
  Image *image1;

  if(!PL_get_atom_chars(PL_Filename,&filename))
    return FALSE;

  // allocate space for texture
  image1 = (Image *) malloc(sizeof(Image));
  if (image1 == NULL) {
    printf("Error allocating space for image");
    exit(0);
  }

  if (!c_imageLoad(filename, image1)) {
    exit(1);
  }

  // Create Texture
  glGenTextures(1, &texture[0]);
  glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

  // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
  // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.

  printf("texture[0]:%u\n",texture[0]);
  printf("&texture[0]:%p\n",&texture[0]);

  printf("image1->sizeX:%u\n",(unsigned int)image1->sizeX);
  printf("image1->sizeY:%u\n",(unsigned int)image1->sizeY);

  printf("image1->data:%p\n",image1->data);

  glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

  PL_succeed;
}

/* Name: c_loadGLTextures
 * Params:
 * Returns:
 */
foreign_t c_loadGLTextures(term_t PL_Filename, term_t PL_Texture, term_t PL_Width, term_t PL_Height, term_t PL_Data) {
  char *filename;
  Image *image1;
  int rval;
  void *ptr_texture;
  void *ptr_data;

  if(!PL_get_atom_chars(PL_Filename,&filename))
    return FALSE;

  // allocate space for texture
  image1 = (Image *) malloc(sizeof(Image));
  if (image1 == NULL) {
    printf("Error allocating space for image");
    exit(0);
  }

  if (!c_imageLoad(filename, image1)) {
    exit(1);
  }

  ptr_texture = &texture[0];
  ptr_data = image1->data;

  //printf("ptr:%s\n",itoa(ptr_texture));

  rval = PL_unify_pointer(PL_Texture, ptr_texture);
  rval = PL_unify_integer(PL_Width, (unsigned int)image1->sizeX);
  rval = PL_unify_integer(PL_Height, (unsigned int)image1->sizeY);
  rval = PL_unify_pointer(PL_Data, ptr_data);

  printf("texture:%d\n",texture[0]);
  printf("&texture:%p\n",&texture[0]);

  //printf("width:%u\n",(unsigned int)image1->sizeX);
  //printf("height:%u\n",(unsigned int)image1->sizeY);

  //printf("data:%p\n\n",image1->data);


  // Create Texture
  glGenTextures(1, &texture[0]);
  glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)

  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

  // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
  // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
  glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

  return rval;

  //PL_succeed;
}




/* ====================== gl Functions ==================== */

/* Name: c_glAccum
 * Params:
 * Returns:
 */
foreign_t c_glAccum(term_t PL_Operation, term_t PL_Value) {
  int operation;
  double value;

  if(!PL_get_integer(PL_Operation,&operation) ||
     !PL_get_float(PL_Value,&value))
    return FALSE;

  glAccum((GLenum)operation, (GLfloat)value);

  PL_succeed;
}

/* Name: c_glActiveTextureARB
 * Params:
 * Returns:
 */
foreign_t c_glActiveTextureARB(term_t PL_Texture) {
  int texture;

  if(!PL_get_integer(PL_Texture,&texture))
    return FALSE;

  //glActiveTextureARB((GLenum)texture);

  PL_succeed;
}

/* Name: c_glAlphaFunc
 * Params:
 * Returns:
 */
foreign_t c_glAlphaFunc(term_t PL_Func, term_t PL_Ref) {
  int func;
  double ref;

  if(!PL_get_integer(PL_Func,&func) ||
     !PL_get_float(PL_Ref,&ref))
    return FALSE;

  glAlphaFunc((GLenum)func, (GLclampf)ref);

  PL_succeed;
}

/* Name: c_glArrayElement
 * Params:
 * Returns:
 */
foreign_t c_glArrayElement(term_t PL_Index) {
  int index;

  if(!PL_get_integer(PL_Index,&index))
    return FALSE;

  glArrayElement((GLint)index);

  PL_succeed;
}

/* Name: c_glBegin
 * Params:
 * Returns:
 */
foreign_t c_glBegin(term_t PL_Mode) {
  GLenum mode;
  int temp;

  if(!PL_get_integer(PL_Mode,&temp))
    return FALSE;
  mode = (GLenum)temp;

  glBegin(mode);

  PL_succeed;
}

/* Name: c_glBindTexture
 * Params:
 * Returns:
 */
foreign_t c_glBindTexture(term_t PL_Target, term_t PL_Texture) {
  GLenum target;
  int texture;
  int temp_target, temp_texture;

  if(!PL_get_integer(PL_Target,&temp_target) ||
     !PL_get_integer(PL_Texture,&temp_texture))
    return FALSE;

  target = (GLenum)temp_target;
  texture = (GLuint)temp_texture;

  glBindTexture(target, texture);

  PL_succeed;
}

/* Name: c_glBitmap
 * Params:
 * Returns:
 */
foreign_t c_glBitmap(term_t PL_Width, term_t PL_Height, term_t PL_X1, term_t PL_Y1, term_t PL_X2, term_t PL_Y2, term_t PL_Bitmap, term_t PL_Num) {
  term_t head = PL_new_term_ref();      /* variable for the elements */
  term_t list = PL_copy_term_ref(PL_Bitmap);    /* copy as we need to write */

  double x1,y1,x2,y2;
  int w,h,num,count;
  GLubyte *bitmap;

  if(!PL_get_integer(PL_Num,&num))
    return FALSE;

  bitmap = malloc(num * sizeof(GLubyte));

  if(!PL_get_integer(PL_Width,&w) ||
     !PL_get_integer(PL_Height,&h))
    return FALSE;

  if(!PL_get_float(PL_X1,&x1) ||
     !PL_get_float(PL_Y1,&y1) ||
     !PL_get_float(PL_X2,&x2) ||
     !PL_get_float(PL_Y2,&y2))
    return FALSE;

  count = 0;
  while( PL_get_list(list, head, list) ) {
    char *s;

    if ( PL_get_chars(head, &s, CVT_INTEGER) ){
      bitmap[count] = (atoi(s));
      printf("bitmap[%d]=%x\n",count,atoi(s));
    }
    else
      PL_fail;

    count++;
  }

  glBitmap((GLsizei)w,(GLsizei)h,(GLfloat)x1,(GLfloat)y1,(GLfloat)x2,(GLfloat)y2,bitmap);

  free(bitmap);
  return PL_get_nil(list);

  PL_succeed;
}

/* Name: c_glBlendFunc
 * Params:
 * Returns:
 */
foreign_t c_glBlendFunc(term_t PL_sFactor, term_t PL_dFactor) {
  GLenum sfactor, dfactor;
  int temp_s, temp_d;

  if(!PL_get_integer(PL_sFactor,&temp_s) ||
     !PL_get_integer(PL_dFactor,&temp_d))
    return FALSE;
  sfactor = (GLenum)temp_s;
  dfactor = (GLenum)temp_d;

  glBlendFunc(sfactor, dfactor);

  PL_succeed;
}

/* Name: c_glCallList
 * Params:
 * Returns:
 */
foreign_t c_glCallList(term_t PL_List) {
  int list;

  if(!PL_get_integer(PL_LIST,&list))
    return FALSE;

  glCallList((GLuint)list);

  PL_succeed;
}

/* Name: c_glClear
 * Params:
 * Returns:
 */
foreign_t c_glClear(term_t PL_Mask) {
  GLbitfield mask;
  int temp;

  if(!PL_get_integer(PL_Mask,&temp))
    return FALSE;
  mask = (GLbitfield)temp;
  glClear(mask);

  PL_succeed;
}

/* Name: c_glClearColor
 * Params:
 * Returns:
 */
foreign_t c_glClearColor(term_t PL_Red, term_t PL_Green, term_t PL_Blue, term_t PL_Alpha) {
  GLdouble r,g,b,a;
  GLclampf red,green,blue,alpha;
  if(!PL_get_float(PL_Red,&r) ||
     !PL_get_float(PL_Green,&g) ||
     !PL_get_float(PL_Blue,&b) ||
     !PL_get_float(PL_Alpha,&a) )
    return FALSE;
  red = (GLclampf)r;
  green = (GLclampf)g;
  blue = (GLclampf)b;
  alpha = (GLclampf)a;

  glClearColor(red,green,blue,alpha);

  PL_succeed;
}

/* Name: c_glClearDepth
 * Params:
 * Returns:
 */
foreign_t c_glClearDepth(term_t PL_Depth) {
  GLdouble d;
  GLclampd depth;
  if(!PL_get_float(PL_Depth,&d))
    return FALSE;
  depth = (GLclampd)d;

  glClearDepth(depth);

  PL_succeed;
}

/* Name: c_glClearIndex
 * Params:
 * Returns:
 */
foreign_t c_glClearIndex(term_t PL_Index) {
  double index;
  if(!PL_get_float(PL_Index,&index))
    return FALSE;

  glClearIndex((GLfloat)index);

  PL_succeed;
}

/* Name: c_glClearStencil
 * Params:
 * Returns:
 */
foreign_t c_glClearStencil(term_t PL_S) {
  int s;
  if(!PL_get_integer(PL_S,&s))
    return FALSE;

  glClearIndex((GLint)s);

  PL_succeed;
}

/* Name: c_glClipPlane
 * Params:
 * Returns:
 */
foreign_t c_glClipPlane(term_t PL_Plane, term_t PL_Equation, term_t PL_Num) {
  term_t head = PL_new_term_ref();      /* variable for the elements */
  term_t list = PL_copy_term_ref(PL_Equation);    /* copy as we need to write */

  int plane;
  int num;
  int count;
  double *equation;
  if(!PL_get_integer(PL_Num,&num))
	  return FALSE;

  equation = malloc(num * sizeof(double));

  if(!PL_get_integer(PL_Plane,&plane))
    return FALSE;

  printf("plane=%d\n",plane);

  count = 0;
  while( PL_get_list(list, head, list) ) {
    char *s;

    if ( PL_get_chars(head, &s, CVT_FLOAT) ){
      equation[count] = (GLdouble)(atof(s));
      printf("equation[%d]=%f\n", count, (atof(s)));
    }
    else
      PL_fail;

    count++;
  }

  glClipPlane((GLenum)plane, equation);

  free(equation);

  return PL_get_nil(list);

  PL_succeed;
}


/* Name: c_glColor3f
 * Params:
 * Returns:
 */
foreign_t c_glColor3f(term_t PL_Red, term_t PL_Green, term_t PL_Blue) {
  GLdouble red, green, blue;

  if(!PL_get_float(PL_Red,&red) ||
     !PL_get_float(PL_Green,&green) ||
     !PL_get_float(PL_Blue,&blue) )
    return FALSE;
  glColor3f((float)red,(float)green,(float)blue);

  PL_succeed;
}

/* Name: c_glColor4f
 * Params:
 * Returns:
 */
foreign_t c_glColor4f(term_t PL_Red, term_t PL_Green, term_t PL_Blue, term_t PL_Alpha) {
  GLdouble red, green, blue, alpha;

  if(!PL_get_float(PL_Red,&red) ||
     !PL_get_float(PL_Green,&green) ||
     !PL_get_float(PL_Blue,&blue) ||
     !PL_get_float(PL_Alpha,&alpha))
    return FALSE;
  glColor4f((float)red,(float)green,(float)blue,(float)alpha);

  PL_succeed;
}

/* Name: c_glCopyPixels
 * Params:
 * Returns:
 */
foreign_t c_glCopyPixels(term_t PL_X, term_t PL_Y, term_t PL_Width, term_t PL_Height, term_t PL_Type) {
  int x,y, width, height, type;

  if(!PL_get_integer(PL_X,&x) ||
     !PL_get_integer(PL_Y,&y) ||
     !PL_get_integer(PL_Width,&width) ||
     !PL_get_integer(PL_Height,&height) ||
     !PL_get_integer(PL_Type,&type))
    return FALSE;

  glCopyPixels(x, y, (GLsizei)width, (GLsizei)height, (GLenum)type);

  PL_succeed;
}

/* Name: c_glCopyTexImage1D
 * Params:
 * Returns:
 */
foreign_t c_glCopyTexImage1D(term_t PL_Target, term_t PL_Level, term_t PL_Internal,
                             term_t PL_X, term_t PL_Y, term_t PL_Width, term_t PL_Border) {
  int target, level, internal, x, y, width, border;

  if(!PL_get_integer(PL_Target,&target) ||
     !PL_get_integer(PL_Level,&level) ||
     !PL_get_integer(PL_Internal,&internal) ||
     !PL_get_integer(PL_X,&x) ||
     !PL_get_integer(PL_Y,&y) ||
     !PL_get_integer(PL_Width,&width) ||
     !PL_get_integer(PL_Border,&border))
    return FALSE;

  glCopyTexImage1D((GLenum)target, level, (GLenum)internal, x, y, (GLsizei)width, border);

  PL_succeed;
}

/* Name: c_glCopyTexImage2D
 * Params:
 * Returns:
 */
foreign_t c_glCopyTexImage2D(term_t PL_Target, term_t PL_Level, term_t PL_Internal,
                             term_t PL_X, term_t PL_Y,
                             term_t PL_Width, term_t PL_Height, term_t PL_Border) {
  int target, level, internal, x, y, width, height, border;

  if(!PL_get_integer(PL_Target,&target) ||
     !PL_get_integer(PL_Level,&level) ||
     !PL_get_integer(PL_Internal,&internal) ||
     !PL_get_integer(PL_X,&x) ||
     !PL_get_integer(PL_Y,&y) ||
     !PL_get_integer(PL_Width,&width) ||
     !PL_get_integer(PL_Height,&height) ||
     !PL_get_integer(PL_Border,&border))
    return FALSE;

  glCopyTexImage2D((GLenum)target, level, (GLenum)internal, x, y, (GLsizei)width, (GLsizei)height, border);

  PL_succeed;
}


/* Name: c_glCullFace
 * Params:
 * Returns:
 */
foreign_t c_glCullFace(term_t PL_Mode) {
  GLenum mode;
  int temp;

  if(!PL_get_integer(PL_Mode,&temp))
    return FALSE;
  mode = (GLenum)temp;

  glCullFace(mode);

  PL_succeed;
}

/* Name: c_glDepthFunc
 * Params:
 * Returns:
 */
foreign_t c_glDepthFunc(term_t PL_Mode) {
  GLenum mode;
  int temp;

  if(!PL_get_integer(PL_Mode,&temp))
    return FALSE;
  mode = (GLenum)temp;
  glDepthFunc(mode);
  PL_succeed;
}

/* Name: c_glDisable
 * Params:
 * Returns:
 */
foreign_t c_glDisable(term_t PL_Mode) {
  GLenum mode;
  int temp;

  if(!PL_get_integer(PL_Mode,&temp))
    return FALSE;
  mode = (GLenum)temp;
  glDisable(mode);
  PL_succeed;
}

/* Name: c_glEnable
 * Params:
 * Returns:
 */
foreign_t c_glEnable(term_t PL_Mode) {
  GLenum mode;
  int temp;

  if(!PL_get_integer(PL_Mode,&temp))
    return FALSE;
  mode = (GLenum)temp;
  glEnable(mode);
  PL_succeed;
}

/* Name: c_glEnd
 * Params:
 * Returns:
 */
foreign_t c_glEnd(void) {
  glEnd();
  PL_succeed;
}

/* Name: c_glEndList
 * Params:
 * Returns:
 */
foreign_t c_glEndList(void) {
  glEndList();
  PL_succeed;
}

/* Name: c_glFinish
 * Params:
 * Returns:
 */
foreign_t c_glFinish(void) {
  glFinish();
  PL_succeed;
}

/* Name: c_glFlush
 * Params:
 * Returns:
 */
foreign_t c_glFlush(void) {
  glFlush();
  PL_succeed;
}

/* Name: c_glFogf
 * Params:
 * Returns:
 */
foreign_t c_glFogf(term_t PL_PName, term_t PL_Param) {
  int pname;
  double param;

  if(!PL_get_integer(PL_PName,&pname) ||
     !PL_get_float(PL_Param,&param))
    return FALSE;

  glFogf((GLenum)pname, param);
  PL_succeed;
}

/* Name: c_glFogi
 * Params:
 * Returns:
 */
foreign_t c_glFogi(term_t PL_PName, term_t PL_Param) {
  int pname, param;

  if(!PL_get_integer(PL_PName,&pname) ||
     !PL_get_integer(PL_Param,&param))
    return FALSE;

  glFogi((GLenum)pname, param);
  PL_succeed;
}


/* Name: c_glFrustum
 * Params:
 * Returns:
 */
foreign_t c_glFrustum(term_t PL_L, term_t PL_R, term_t PL_T, term_t PL_B, term_t PL_N, term_t PL_F) {
  GLdouble left,right,top,bottom,nearVal,farVal;

  if(!PL_get_float(PL_L,&left) ||
     !PL_get_float(PL_R,&right) ||
     !PL_get_float(PL_T,&top) ||
     !PL_get_float(PL_B,&bottom) ||
     !PL_get_float(PL_N,&nearVal) ||
     !PL_get_float(PL_F,&farVal))
    return FALSE;

  glFrustum((GLdouble)left,(GLdouble)right,(GLdouble)top,(GLdouble)bottom,(GLdouble)nearVal,(GLdouble)farVal);

  PL_succeed;
}

/* Name: c_glGenTextures
 * Params:
 * Returns:
 */
foreign_t c_glGenTextures(term_t PL_N, term_t PL_TextureNames, term_t PL_Num) {
  term_t head = PL_new_term_ref();      /* variable for the elements */
  term_t list = PL_copy_term_ref(PL_TextureNames);    /* copy as we need to write */

  int n;
  int num;
  int count;
  unsigned int *parameters;
  if(!PL_get_integer(PL_Num,&num))
	  return FALSE;

  parameters = malloc(num * sizeof(int));

  if(!PL_get_integer(PL_N,&n))
    return FALSE;

  count = 0;
  while( PL_get_list(list, head, list) ) {
    char *s;

    if ( PL_get_chars(head, &s, CVT_INTEGER) ){
      parameters[count] = (atoi(s));
    }
    else
      PL_fail;

    count++;
  }

  glGenTextures((GLsizei)n, parameters);

  free(parameters);
  return PL_get_nil(list);

  PL_succeed;
}


/* Name: c_glHint
 * Params:
 * Returns:
 */
foreign_t c_glHint(term_t PL_Target, term_t PL_Hint) {
  GLenum target,hint;
  int t,h;

  if(!PL_get_integer(PL_Target,&t) ||
     !PL_get_integer(PL_Hint,&h))
    return FALSE;
  target = (GLenum)t;
  hint = (GLenum)h;

  glHint(target,hint);

  PL_succeed;
}

/* Name: c_glIndexi
 * Params:
 * Returns:
 */
foreign_t c_glIndexi(term_t PL_Index) {
  int index;

  if(!PL_get_integer(PL_Index,&index))
    return FALSE;

  glIndexi(index);
  PL_succeed;
}


/* Name: c_glLightfv
 * Params:
 * Returns:
 */
foreign_t c_glLightfv(term_t PL_Light, term_t PL_PName, term_t PL_Params, term_t PL_Num) {
  term_t head = PL_new_term_ref();      /* variable for the elements */
  term_t list = PL_copy_term_ref(PL_Params);    /* copy as we need to write */

  int light, pname;
  int num;
  int count;
  float *parameters;
  if(!PL_get_integer(PL_Num,&num))
	  return FALSE;

  parameters = malloc(num * sizeof(float));

  if(!PL_get_integer(PL_Light,&light) ||
     !PL_get_integer(PL_PName,&pname))
    return FALSE;

  count = 0;
  while( PL_get_list(list, head, list) ) {
    char *s;

    if ( PL_get_chars(head, &s, CVT_FLOAT) ){
      parameters[count] = (atof(s));
    }
    else
      PL_fail;

    count++;
  }

  glLightfv((GLenum)light, (GLenum)pname, parameters);

  free(parameters);
  return PL_get_nil(list);

  PL_succeed;
}

/* Name: c_glLightModelfv
 * Params:
 * Returns:
 */
foreign_t c_glLightModelfv(term_t PL_PName, term_t PL_Params, term_t PL_Num) {
  term_t head = PL_new_term_ref();      /* variable for the elements */
  term_t list = PL_copy_term_ref(PL_Params);    /* copy as we need to write */

  int pname;
  int num;
  int count;
  float *parameters;
  if(!PL_get_integer(PL_Num,&num))
	  return FALSE;

  parameters = malloc(num * sizeof(float));

  if(!PL_get_integer(PL_PName,&pname))
    return FALSE;

  count = 0;
  while( PL_get_list(list, head, list) ) {
    char *s;

    if ( PL_get_chars(head, &s, CVT_FLOAT) ){
      parameters[count] = (atof(s));
    }
    else
      PL_fail;

    count++;
  }

  glLightModelfv((GLenum)pname, parameters);

  free(parameters);
  return PL_get_nil(list);

  PL_succeed;
}

/* Name: c_glLineStipple
 * Params:
 * Returns:
 */
foreign_t c_glLineStipple(term_t PL_Factor, term_t PL_Pattern) {
  int factor, pattern;

  if(!PL_get_integer(PL_Factor,&factor) ||
     !PL_get_integer(PL_Pattern,&pattern))
    return FALSE;

  printf("factor:%d, pattern:%d\n",factor, pattern);

  glLineStipple((GLint)factor, (GLushort)pattern);
  PL_succeed;
}

/* Name: c_glLineWidth
 * Params:
 * Returns:
 */
foreign_t c_glLineWidth(term_t PL_Width) {
  GLdouble width;

  if(!PL_get_float(PL_Width,&width))
    return FALSE;

  glLineWidth((GLfloat)width);
  PL_succeed;
}

/* Name: c_glLoadIdentity
 * Params:
 * Returns:
 */
foreign_t c_glLoadIdentity(void) {
  glLoadIdentity();
  PL_succeed;
}

/* Name: c_glMaterialfv
 * Params:
 * Returns:
 */
foreign_t c_glMaterialfv(term_t PL_Face, term_t PL_PName, term_t PL_Params, term_t PL_Num) {
  term_t head = PL_new_term_ref();
  term_t list = PL_copy_term_ref(PL_Params);

  int face, pname;
  int num;
  int count;
  float *parameters;
  if(!PL_get_integer(PL_Num,&num))
	  return FALSE;

  parameters = malloc(num * sizeof(float));

  if(!PL_get_integer(PL_Face,&face) ||
     !PL_get_integer(PL_PName,&pname))
    return FALSE;

  count = 0;
  while( PL_get_list(list, head, list) ) {
    char *s;

    if ( PL_get_chars(head, &s, CVT_FLOAT) ) {
      parameters[count] = (atof(s));
      //printf("parameters[%d]: %f\n",count,atof(s));
    }
    else
      PL_fail;

    count++;
  }

  glMaterialfv((GLenum)face, (GLenum)pname, parameters);

  free(parameters);
  return PL_get_nil(list);

  PL_succeed;
}

/* Name: c_glMatrixMode
 * Params:
 * Returns:
 */
foreign_t c_glMatrixMode(term_t PL_Mode) {
  GLenum mode;
  int temp;

  if(!PL_get_integer(PL_Mode,&temp))
    return FALSE;

  mode = (GLenum)temp;

  glMatrixMode(mode);

  PL_succeed;
}

/* Name: c_glNewList
 * Params:
 * Returns:
 */
foreign_t c_glNewList(term_t PL_List, term_t PL_Mode) {
  GLenum mode;
  int list;
  int temp_list, temp_mode;

  if(!PL_get_integer(PL_List,&temp_list) ||
     !PL_get_integer(PL_Mode,&temp_mode))
    return FALSE;

  mode = (GLenum)temp_mode;
  list = (GLuint)temp_list;

  glNewList(list, mode);


  PL_succeed;
}

/* Name: c_glOrtho
 * Params:
 * Returns:
 */
foreign_t c_glOrtho(term_t PL_L, term_t PL_R, term_t PL_T, term_t PL_B, term_t PL_N, term_t PL_F) {
  GLdouble left,right,top,bottom,nearVal,farVal;

  if(!PL_get_float(PL_L,&left) ||
     !PL_get_float(PL_R,&right) ||
     !PL_get_float(PL_T,&top) ||
     !PL_get_float(PL_B,&bottom) ||
     !PL_get_float(PL_N,&nearVal) ||
     !PL_get_float(PL_F,&farVal))
    return FALSE;

  glOrtho((GLdouble)left,(GLdouble)right,(GLdouble)top,(GLdouble)bottom,(GLdouble)nearVal,(GLdouble)farVal);

  PL_succeed;
}

/* Name: c_glPixelStorei
 * Params:
 * Returns:
 */
foreign_t c_glPixelStorei(term_t PL_Mode, term_t PL_Param) {
  int mode, param;

  if(!PL_get_integer(PL_Mode,&mode) ||
     !PL_get_integer(PL_Param,&param))
    return FALSE;

  glPixelStorei((GLenum)mode,param);
  PL_succeed;
}

/* Name: c_glPointSize
 * Params:
 * Returns:
 */
foreign_t c_glPointSize(term_t PL_Size) {
  GLdouble size;

  if(!PL_get_float(PL_Size,&size))
    return FALSE;

  glPointSize((GLfloat)size);

  PL_succeed;
}

/* Name: c_glPopAttrib
 * Params:
 * Returns:
 */
foreign_t c_glPopAttrib(void) {
  glPopAttrib();
  PL_succeed;
}

/* Name: c_glPopClientAttrib
 * Params:
 * Returns:
 */
foreign_t c_glPopClientAttrib(void) {
  glPopClientAttrib();
  PL_succeed;
}

/* Name: c_glPopMatrix
 * Params:
 * Returns:
 */
foreign_t c_glPopMatrix(void) {
  glPopMatrix();
  PL_succeed;
}

/* Name: c_glPopName
 * Params:
 * Returns:
 */
foreign_t c_glPopName(void) {
  glPopName();
  PL_succeed;
}

/* Name: c_glPushAttrib
 * Params:
 * Returns:
 */
foreign_t c_glPushAttrib(term_t PL_Mask) {
  int mask;

  if(!PL_get_integer(PL_Mask,&mask))
    return FALSE;

  glPushAttrib((GLbitfield)mask);
  PL_succeed;
}

/* Name: c_glPushClientAttrib
 * Params:
 * Returns:
 */
foreign_t c_glPushClientAttrib(term_t PL_Mask) {
  int mask;

  if(!PL_get_integer(PL_Mask,&mask))
    return FALSE;

  glPushClientAttrib((GLbitfield)mask);
  PL_succeed;
}

/* Name: c_glPushMatrix
 * Params:
 * Returns:
 */
foreign_t c_glPushMatrix(void) {
  glPushMatrix();
  PL_succeed;
}

/* Name: c_glPushName
 * Params:
 * Returns:
 */
foreign_t c_glPushName(term_t PL_Name) {
  int name;

  if(!PL_get_integer(PL_Name,&name))
    return FALSE;

  glPushName((GLuint)name);
  PL_succeed;
}

/* Name: c_glRasterPos2i
 * Params:
 * Returns:
 */
foreign_t c_glRasterPos2i(term_t PL_X, term_t PL_Y) {
  int x,y;

  if(!PL_get_integer(PL_X,&x) ||
     !PL_get_integer(PL_Y,&y))
    return FALSE;

  glRasterPos2i(x,y);

  PL_succeed;
}

/* Name: c_glRectf
 * Params:
 * Returns:
 */
foreign_t c_glRectf(term_t PL_X1, term_t PL_Y1, term_t PL_X2, term_t PL_Y2) {
  GLdouble x1,y1,x2,y2;

  if(!PL_get_float(PL_X1,&x1) ||
     !PL_get_float(PL_Y1,&y1) ||
     !PL_get_float(PL_X2,&x2) ||
     !PL_get_float(PL_Y2,&y2))
    return FALSE;
  glRectf((GLfloat)x1,(GLfloat)y1,(GLfloat)x2,(GLfloat)y2);

  PL_succeed;
}


/* Name: c_glRotatef
 * Params:
 * Returns:
 */
foreign_t c_glRotatef(term_t PL_Angle, term_t PL_X, term_t PL_Y, term_t PL_Z) {
  GLdouble angle,x,y,z;

  if(!PL_get_float(PL_Angle,&angle) ||
     !PL_get_float(PL_X,&x) ||
     !PL_get_float(PL_Y,&y) ||
     !PL_get_float(PL_Z,&z))
    return FALSE;
  glRotatef((GLfloat)angle,(GLfloat)x,(GLfloat)y,(GLfloat)z);

  PL_succeed;
}

/* Name: c_glScalef
 * Params:
 * Returns:
 */
foreign_t c_glScalef(term_t PL_X, term_t PL_Y, term_t PL_Z) {
  GLdouble x,y,z;

  if(!PL_get_float(PL_X,&x) ||
     !PL_get_float(PL_Y,&y) ||
     !PL_get_float(PL_Z,&z))
    return FALSE;
  glScalef((float)x,(float)y,(float)z);

  printf("glScalef(%f, %f, %f)\n",(float)x,(float)y,(float)z);

  PL_succeed;
}

/* Name: c_glShadeModel
 * Params:
 * Returns:
 */
foreign_t c_glShadeModel(term_t PL_Mode) {
  GLenum mode;
  int temp;

  if(!PL_get_integer(PL_Mode,&temp))
    return FALSE;

  mode = (GLenum)temp;
  glShadeModel(mode);

  printf("glShadeModel(%x)\n",mode);

  PL_succeed;
}

/* Name: c_glStencilFunc
 * Params:
 * Returns:
 */
foreign_t c_glStencilFunc(term_t PL_Func, term_t PL_Ref, term_t PL_Mask) {
  int func, ref;
  int mask;

  if(!PL_get_integer(PL_Func,&func) ||
     !PL_get_integer(PL_Ref,&ref) ||
     !PL_get_integer(PL_Mask,&mask))
    return FALSE;

  glStencilFunc((GLenum)func, (GLint)ref, (GLuint)mask);

  PL_succeed;
}

/* Name: c_glStencilMask
 * Params:
 * Returns:
 */
foreign_t c_glStencilMask(term_t PL_Mask) {
  int mask;

  if(!PL_get_integer(PL_Mask,&mask))
    return FALSE;

  glStencilMask((GLuint)mask);

  PL_succeed;
}

/* Name: c_glStencilOp
 * Params:
 * Returns:
 */
foreign_t c_glStencilOp(term_t PL_Fail, term_t PL_zFail, term_t PL_zPass) {
  int fail, zfail, zpass;

  if(!PL_get_integer(PL_Fail,&fail) ||
     !PL_get_integer(PL_zFail,&zfail) ||
     !PL_get_integer(PL_zPass,&zpass))
    return FALSE;

  glStencilFunc((GLenum)fail, (GLenum)zfail, (GLenum)zpass);

  PL_succeed;
}

/* Name: c_glTexCoord2f
 * Params:
 * Returns:
 */
foreign_t c_glTexCoord2f(term_t PL_S, term_t PL_T) {
  GLdouble s,t;

  if(!PL_get_float(PL_S,&s) ||
     !PL_get_float(PL_T,&t))
    return FALSE;

  glTexCoord2f((float)s,(float)t);

  PL_succeed;
}

/* Name: c_glTexImage2D
 * Params:
 * Returns:
 */
foreign_t c_glTexImage2D(term_t PL_Target, term_t PL_Level, term_t PL_Internal, term_t PL_Width, term_t PL_Height,
                         term_t PL_Border, term_t PL_Format, term_t PL_Type, term_t PL_Texels) {
  int target, level, internal, width, height, border, format, type;
  void *texels;
  //int texels;
  //void *ptr;

  if(!PL_get_integer(PL_Target,&target) ||
     !PL_get_integer(PL_Level,&level) ||
     !PL_get_integer(PL_Internal,&internal) ||
     !PL_get_integer(PL_Width,&width) ||
     !PL_get_integer(PL_Height,&height) ||
     !PL_get_integer(PL_Border,&border) ||
     !PL_get_integer(PL_Format,&format) ||
     !PL_get_integer(PL_Type,&type) ||
     !PL_get_pointer(PL_Texels,&texels))
    return FALSE;

  printf("Texels:%p\n", &texels);

  //ptr = texels;

  //printf("ptr:%p\n",ptr);

  glTexImage2D((GLenum)target, (GLint)level, (GLint)internal, (GLsizei)width, (GLsizei)height,
               (GLint)border, (GLenum)format, (GLenum)type, &texels);

  PL_succeed;
}

/* Name: c_glTexParameteri
 * Params:
 * Returns:
 */
foreign_t c_glTexParameteri(term_t PL_Target, term_t PL_PName, term_t PL_Param) {
  int target, pname, param;

  if(!PL_get_integer(PL_Target,&target)||
     !PL_get_integer(PL_PName,&pname) ||
     !PL_get_integer(PL_Param,&param))
    return FALSE;

  glTexParameteri((GLenum)target, (GLenum)pname, param);

  PL_succeed;

}

/* Name: c_glTranslatef
 * Params:
 * Returns:
 */
foreign_t c_glTranslatef(term_t PL_X, term_t PL_Y, term_t PL_Z) {
  GLdouble x,y,z;

  if(!PL_get_float(PL_X,&x) ||
     !PL_get_float(PL_Y,&y) ||
     !PL_get_float(PL_Z,&z))
    return FALSE;
  glTranslatef((float)x,(float)y,(float)z);

  PL_succeed;
}

/* Name: c_glVertex2f
 * Params:
 * Returns:
 */
foreign_t c_glVertex2f(term_t PL_X, term_t PL_Y) {
  GLdouble x,y;

  if(!PL_get_float(PL_X,&x) ||
     !PL_get_float(PL_Y,&y))
    return FALSE;

  glVertex2f((float)x,(float)y);

  PL_succeed;
}

/* Name: c_glVertex2i
 * Params:
 * Returns:
 */
foreign_t c_glVertex2i(term_t PL_X, term_t PL_Y) {
  GLint x,y;

  if(!PL_get_integer(PL_X,&x) ||
     !PL_get_integer(PL_Y,&y))
    return FALSE;
  glVertex2i(x,y);

  PL_succeed;
}

/* Name: c_glVertex3d
 * Params:
 * Returns:
 */
foreign_t c_glVertex3d(term_t PL_X, term_t PL_Y, term_t PL_Z) {
  GLdouble x,y,z;

  if(!PL_get_float(PL_X,&x) ||
     !PL_get_float(PL_Y,&y) ||
     !PL_get_float(PL_Z,&z))
    return FALSE;
  glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);

  PL_succeed;
}

/* Name: c_glVertex3f
 * Params:
 * Returns:
 */
foreign_t c_glVertex3f(term_t PL_X, term_t PL_Y, term_t PL_Z) {
  GLdouble x,y,z;

  if(!PL_get_float(PL_X,&x) ||
     !PL_get_float(PL_Y,&y) ||
     !PL_get_float(PL_Z,&z))
    return FALSE;
  glVertex3f((float)x,(float)y,(float)z);

  PL_succeed;
}

/* Name: c_glVertex3i
 * Params:
 * Returns:
 */
foreign_t c_glVertex3i(term_t PL_X, term_t PL_Y, term_t PL_Z) {
  GLint x,y,z;

  if(!PL_get_integer(PL_X,&x) ||
     !PL_get_integer(PL_Y,&y) ||
     !PL_get_integer(PL_Z,&z))
    return FALSE;
  glVertex3i(x,y,z);

  PL_succeed;
}

/* Name: c_glViewport
 * Params:
 * Returns:
 */
foreign_t c_glViewport(term_t PL_X, term_t PL_Y, term_t PL_Width, term_t PL_Height) {
  GLint x,y;
  GLsizei width,height;

  if(!PL_get_integer(PL_X,&x) ||
     !PL_get_integer(PL_Y,&y) ||
     !PL_get_integer(PL_Width,&width) ||
     !PL_get_integer(PL_Height,&height))
    return FALSE;
  glViewport(x,y,width,height);

  PL_succeed;
}


/* ====================== glu Functions ==================== */

/* Name: c_gluLookAt
 * Params:
 * Returns:
 */
foreign_t c_gluLookAt(term_t PL_EyeX, term_t PL_EyeY, term_t PL_EyeZ,
                      term_t PL_CenterX, term_t PL_CenterY, term_t PL_CenterZ,
                      term_t PL_UpX, term_t PL_UpY, term_t PL_UpZ) {
  GLdouble eyex,eyey,eyez;
  GLdouble centerx, centery, centerz;
  GLdouble upx, upy, upz;

  if(!PL_get_float(PL_EyeX,&eyex) ||
     !PL_get_float(PL_EyeY,&eyey) ||
     !PL_get_float(PL_EyeZ,&eyez) ||
     !PL_get_float(PL_CenterX,&centerx) ||
     !PL_get_float(PL_CenterY,&centery) ||
     !PL_get_float(PL_CenterZ,&centerz) ||
     !PL_get_float(PL_UpX,&upx) ||
     !PL_get_float(PL_UpY,&upy) ||
     !PL_get_float(PL_UpZ,&upz) )
    return FALSE;

  gluLookAt((GLdouble)eyex, (GLdouble)eyey, (GLdouble)eyez,
            (GLdouble)centerx, (GLdouble)centery, (GLdouble)centerz,
            (GLdouble)upx, (GLdouble)upy, (GLdouble)upz);
  PL_succeed;
}

/* Name: c_gluOrtho2D
 * Params:
 * Returns:
 */
foreign_t c_gluOrtho2D(term_t PL_Left, term_t PL_Right, term_t PL_Bottom, term_t PL_Top) {
  GLdouble left, right, bottom, top;

  if(!PL_get_float(PL_Left,&left) ||
     !PL_get_float(PL_Right,&right) ||
     !PL_get_float(PL_Bottom,&bottom) ||
     !PL_get_float(PL_Top,&top) )
    return FALSE;

  gluOrtho2D((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top);
  PL_succeed;
}

/* Name: c_gluPerspective
 * Params:
 * Returns:
 */
foreign_t c_gluPerspective(term_t PL_Fovy, term_t PL_Aspect, term_t PL_Near, term_t PL_Far) {
  GLdouble fovy,aspect,nearVal,farVal;

  if(!PL_get_float(PL_Fovy,&fovy) ||
     !PL_get_float(PL_Aspect,&aspect) ||
     !PL_get_float(PL_Near,&nearVal) ||
     !PL_get_float(PL_Far,&farVal) )
    return FALSE;

  gluPerspective(fovy,aspect,nearVal,farVal);
  PL_succeed;
}


/* ====================== glut Functions ==================== */


/* Name: c_glutCreateWindow
 * Params:
 * Returns:
 */
foreign_t c_glutCreateWindow(term_t PL_String) {
  char *string;
  int id;

  if(!PL_get_atom_chars(PL_String,&string))
    return FALSE;
  id = glutCreateWindow(string);
  printf("call to glutCreateWindow(%s), id:%d\n",string, id);

  PL_succeed;
}

/* Name: c_glutDestroyWindow
 * Params:
 * Returns:
 */
foreign_t c_glutDestroyWindow(void) {
  glutDestroyWindow(1);
  PL_halt(1);
  PL_succeed;
}

/* Name: c_glutDisplayFunc
 * Params:
 * Returns:
 */
foreign_t c_glutDisplayFunc(void) {

  /*
  void (*fp)(char);
  fp = c_display;
  fp("display");
  */

  glutDisplayFunc(c_display);
  printf("glutDisplayFunc %p\n",c_display);
  PL_succeed;
}

/* Name: c_glutIdleFunc
 * Params:
 * Returns:
 */
foreign_t c_glutIdleFunc(term_t PL_String) {
  char *string;
  void (*fp)(void);
  //char *ptr = "spinDisplay";

  if(!PL_get_atom_chars(PL_String,&string))
    return FALSE;

  //glut_idle = string;


  fp = c_idle;

  //printf("idle:%s\n",string);
  if(strcmp(string,"null") == 0) {
    glutIdleFunc(NULL);
  }
  else {
    glutIdleFunc(fp);
  }

  PL_succeed;
}

/* Name: c_glutInit
 * Params:
 * Returns:
 */
foreign_t c_glutInit(void) {
  int argc = PL_query(PL_QUERY_ARGC);
  char **argv = (char **)PL_query(PL_QUERY_ARGV);

  glutInit(&argc, argv);
  printf("call to glutInit\n");

  PL_succeed;
}

/* Name: c_glutInitDisplayMode
 * Params:
 * Returns:
 */
foreign_t c_glutInitDisplayMode(term_t PL_DisplayMode) {
  unsigned int mode;
  int temp;

  if(!PL_get_integer(PL_DisplayMode,&temp))
    PL_fail;
  mode = (unsigned int)temp;
  glutInitDisplayMode(mode);
  printf("call to glutInitDisplayMode(%d)\n",mode);
  PL_succeed;
}

/* Name: c_glutInitWindowPosition
 * Params:
 * Returns:
 */
foreign_t c_glutInitWindowPosition(term_t PL_X, term_t PL_Y) {
  int x,y;

  if (!PL_get_integer(PL_X,&x))
    return FALSE;
  if (!PL_get_integer(PL_Y,&y))
    return FALSE;

  glutInitWindowPosition(x,y);

  PL_succeed;
}

/* Name: c_glutInitWindowSize
 * Params:
 * Returns:
 */
foreign_t c_glutInitWindowSize(term_t PL_Width, term_t PL_Height) {
  int width;
  int height;

  if (!PL_get_integer(PL_Width,&width))
    width = 640;
  if (!PL_get_integer(PL_Height,&height))
    height = 480;

  glutInitWindowSize(width,height);

  printf("call to glutInitWindowSize(%d,%d)\n",width,height);
  PL_succeed;
}

/* Name: c_glutKeyboardFunc
 * Params:
 * Returns:
 */
foreign_t c_glutKeyboardFunc(void) {
  void (*fp)(unsigned char, int, int);
  fp = c_keyboard;
  glutKeyboardFunc(fp);

  PL_succeed;
}

/* Name: c_glutMainLoop
 * Params:
 * Returns:
 */
foreign_t c_glutMainLoop(void) {
  glutMainLoop();

  printf("forking call to glutMainLoop()\n");

  PL_succeed;
}

/* Name: c_glutMotionFunc
 * Params:
 * Returns:
 */
foreign_t c_glutMotionFunc(void) {
  void (*fp)(int, int);
  fp = c_motion;
  glutMotionFunc(fp);

  PL_succeed;
}

/* Name: c_glutMouseFunc
 * Params:
 * Returns:
 */
foreign_t c_glutMouseFunc(void) {
  void (*fp)(int, int, int, int);
  fp = c_mouse;
  glutMouseFunc(fp);

  PL_succeed;
}

/* Name: c_glutPostRedisplay
 * Params:
 * Returns:
 */
foreign_t c_glutPostRedisplay(void) {
  glutPostRedisplay();
  PL_succeed;
}

/* Name: c_glutReshapeFunc
 * Params:
 * Returns:
 */
foreign_t c_glutReshapeFunc(void) {
  void (*fp)(int, int);
  fp = c_reshape;

  glutReshapeFunc(fp);
  printf("glutReshapeFunc %p\n",fp);
  PL_succeed;
}

/* Name: c_glutSetColor
 * Params:
 * Returns:
 */
foreign_t c_glutSetColor(term_t PL_Index, term_t PL_Red, term_t PL_Green, term_t PL_Blue) {
  GLdouble red, green, blue;
  GLint index;

  if(!PL_get_integer(PL_Index,&index))
    return FALSE;

  if(!PL_get_float(PL_Red,&red) ||
     !PL_get_float(PL_Green,&green) ||
     !PL_get_float(PL_Blue,&blue) )
    return FALSE;

  glutSetColor(index, (float)red,(float)green,(float)blue);

  PL_succeed;
}

/* Name: c_glutSolidCone
 * Params:
 * Returns:
 */
foreign_t c_glutSolidCone(term_t PL_Radius, term_t PL_Height, term_t PL_Slices, term_t PL_Stacks) {
  GLdouble radius, height;
  GLint slices, stacks;

  if(!PL_get_float(PL_Radius,&radius) ||
     !PL_get_float(PL_Height,&height) ||
     !PL_get_integer(PL_Slices,&slices) ||
     !PL_get_integer(PL_Stacks,&stacks))
    return FALSE;

  glutSolidCone((GLdouble)radius, (GLdouble)height, slices, stacks);

  PL_succeed;
}

/* Name: c_glutSolidCube
 * Params:
 * Returns:
 */
foreign_t c_glutSolidCube(term_t PL_Size) {
  GLdouble size;

  if(!PL_get_float(PL_Size,&size))
    return FALSE;

  glutSolidCube((GLdouble)size);

  PL_succeed;
}

/* Name: c_glutSolidSphere
 * Params:
 * Returns:
 */
foreign_t c_glutSolidSphere(term_t PL_Radius, term_t PL_Slices, term_t PL_Stacks) {
  GLdouble radius;
  GLint slices, stacks;

  if(!PL_get_float(PL_Radius,&radius) ||
     !PL_get_integer(PL_Slices,&slices) ||
     !PL_get_integer(PL_Stacks,&stacks))
    return FALSE;

  glutSolidSphere((GLdouble)radius, slices, stacks);

  PL_succeed;
}

/* Name: c_glutSolidTeapot
 * Params:
 * Returns:
 */
foreign_t c_glutSolidTeapot(term_t PL_Size) {
  GLdouble size;

  if(!PL_get_float(PL_Size,&size))
    return FALSE;

  glutSolidTeapot((GLdouble)size);

  PL_succeed;
}

/* Name: c_glutSolidTorus
 * Params:
 * Returns:
 */
foreign_t c_glutSolidTorus(term_t PL_InnerRadius, term_t PL_OuterRadius, term_t PL_NSides, term_t PL_Rings) {
  GLdouble innerRadius, outerRadius;
  GLint nsides, rings;

  if(!PL_get_float(PL_InnerRadius,&innerRadius) ||
     !PL_get_float(PL_OuterRadius,&outerRadius) ||
     !PL_get_integer(PL_NSides,&nsides) ||
     !PL_get_integer(PL_Rings,&rings))
    return FALSE;

  glutSolidTorus((GLdouble)innerRadius, (GLdouble)outerRadius, nsides, rings);

  PL_succeed;
}

/* Name: c_glutSwapBuffers
 * Params:
 * Returns:
 */
foreign_t c_glutSwapBuffers(void) {
  glutSwapBuffers();

  PL_succeed;
}

/* Name: c_glutWireCone
 * Params:
 * Returns:
 */
foreign_t c_glutWireCone(term_t PL_Radius, term_t PL_Height, term_t PL_Slices, term_t PL_Stacks) {
  GLdouble radius, height;
  GLint slices, stacks;

  if(!PL_get_float(PL_Radius,&radius) ||
     !PL_get_float(PL_Height,&height) ||
     !PL_get_integer(PL_Slices,&slices) ||
     !PL_get_integer(PL_Stacks,&stacks))
    return FALSE;

  glutWireCone((GLdouble)radius, (GLdouble)height, slices, stacks);

  PL_succeed;
}

/* Name: c_glutWireCube
 * Params:
 * Returns:
 */
foreign_t c_glutWireCube(term_t PL_Size) {
  GLdouble size;

  if(!PL_get_float(PL_Size,&size))
    return FALSE;

  glutWireCube((GLdouble)size);

  PL_succeed;
}

/* Name: c_glutWireSphere
 * Params:
 * Returns:
 */
foreign_t c_glutWireSphere(term_t PL_Radius, term_t PL_Slices, term_t PL_Stacks) {
  GLdouble radius;
  GLint slices, stacks;

  if(!PL_get_float(PL_Radius,&radius) ||
     !PL_get_integer(PL_Slices,&slices) ||
     !PL_get_integer(PL_Stacks,&stacks))
    return FALSE;

  glutWireSphere((GLdouble)radius, slices, stacks);

  PL_succeed;
}

/* Name: c_glutWireTeapot
 * Params:
 * Returns:
 */
foreign_t c_glutWireTeapot(term_t PL_Size) {
  GLdouble size;

  if(!PL_get_float(PL_Size,&size))
    return FALSE;

  glutWireTeapot((GLdouble)size);

  PL_succeed;
}

/* Name: c_glutWireTorus
 * Params:
 * Returns:
 */
foreign_t c_glutWireTorus(term_t PL_InnerRadius, term_t PL_OuterRadius, term_t PL_NSides, term_t PL_Rings) {
  GLdouble innerRadius, outerRadius;
  GLint nsides, rings;

  if(!PL_get_float(PL_InnerRadius,&innerRadius) ||
     !PL_get_float(PL_OuterRadius,&outerRadius) ||
     !PL_get_integer(PL_NSides,&nsides) ||
     !PL_get_integer(PL_Rings,&rings))
    return FALSE;

  glutWireTorus((GLdouble)innerRadius, (GLdouble)outerRadius, nsides, rings);

  PL_succeed;
}