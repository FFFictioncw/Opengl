/*

	Specification : display a hierarchical object with 3 parts : Robotic hand example in Lecture

*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <gl/glut.h>

//////////////////////////////////////////////////////////////////
// 
// Include the header file of our rotation user-interface.
// 
#include "gsrc.h"
// 
//////////////////////////////////////////////////////////////////


#define PI			3.141592654  // Prime
#define WIN_POSX    500
#define WIN_POSY    100
#define WIN_WIDTH   500
#define WIN_HEIGHT  700


int    option;                   // specify whether the animation is uniform, accelerating or decelerating
double exponent;                 // control the rate of change of acceleration/decleration
double t_prev;                   // previous time elapsed
int i = 0, j = 1;
double  t1r, t2r, t3r, t4r, t5r, t6r, t7, t8, t9, t1l, t2l, t3l, t4l, t5l, t6l;			      // rotation angles of robot, lower-and-upper arm, upper arm respectivley
GLUquadricObj *pObj1, *pObj2, *pObj3,*pObj7,*pObj8,*pObj9,*pObj10, *pObj11, *pObj12, *pObj4, *pObj5, *pObj6; //quadric objects to store properties of the quadric mesh

//所有正立方体的原函数//

void cube()
// draw a standard 2 x 2 x 2 cube whose center is at (0, 1, 0)
{

	/* The use of glPushMatrix and glPopMatrix here protects the glRotatef from
	   exercising its influence on the relative transformation beyond this function */
	glPushMatrix();

	glTranslatef(0, 1, 0);
	glutSolidCube(2);

	glPopMatrix();

}


//一：先画躯干，立方体

static const float vertex_list[][3] =
{
	-50, 0, -30,
	50, 0, -30,
	-40, 110, -30,
	40, 110, -30,
	-50, 0, 30,
	50, 0, 30,
	-40, 110, 30,
	40, 110, 30,
};

// 将要使用的顶点的序号保存到一个数组里面 

static const GLint line_list[][2] =//2点确定12条线段//
{
	{0, 1},
	{2, 3},
	{4, 5},
	{6, 7},
	{0, 2},
	{1, 3},
	{4, 6},
	{5, 7},
	{0, 4},
	{1, 5},
	{7, 3},
	{2, 6}
};

static const GLint face_list[][4] =//4点确定6个面//
{
	{0, 1,3,2},
	{2, 3,7,6},
	{4, 5,0,1},
	{6, 7,5,4},
	{0, 2,6,4},
	{1, 3,7,5},	
};


// 绘制立方体

void draw_torso(void)
{
	int m, n;

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.5f, 1.5f);
	for (m = 0; m < 12; ++m) // 12 条线段

	{
		for (n = 0; n < 2; ++n) // 每条线段 2个顶点

		{
			glVertex3fv(vertex_list[line_list[m][n]]);
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.5f, 0.0f);
	for (m = 0; m < 6; ++m) // 6个面

	{
		for (n = 0; n < 4; ++n) // 每个面 4个顶点

		{
			glVertex3fv(vertex_list[face_list[m][n]]);
		}
	}
	

	glEnd();
}


	 //   作者：bcbobo21cn
		//来源：CSDN
		//原文：https ://blog.csdn.net/bcbobo21cn/article/details/51058836 
	 //   版权声明：本文为博主原创文章，转载请附上博文链接












//二：画右手//

//画右肩膀r=15,h=50//

void draw_shouder(void)
// draw base : blue cylinder of radius 15 height 40  x-z as base
{
	glPushMatrix();

	glColor3f(0.0, 0.0, 1.0);
	

	// draws a hollow cylinder along the z axis with base on x-y axis, origin at (0, 0, 0) 
	gluCylinder(pObj1, 15, 15, 50, 30, 30);
	// base radius 15  
	// top  radius 15 
	// height 50
	// 10 x 10 controls the sampling

	// draw a solid disk to cover the base 
	
	gluDisk(pObj2, 0, 15, 30, 30);//画圆柱的上下底面
	// draw a solid disk to cover the top
	glPushMatrix();
	glTranslatef(0, 0, 50);

	gluDisk(pObj3, 0, 15, 30, 30);//10代表圆由10个点组成
	glPopMatrix();

	glPopMatrix();
}

void draw_lower_arm(void)
{
	glPushMatrix();					// draw lower robotic arm : green box of dimension 24 x 40+15 x 24
	glColor3f(0.0, 1.0, 0.0);
	glScalef(12.0, 27.5, 12.0);
	cube();
	glPopMatrix();
}

void draw_upper_arm(void)
{
	glPushMatrix();					// draw upper robotic arm : yellow box of dimension 24 x 40 x 24
	glColor3f(1.0, 1.0, 0.0);
	glScalef(12.0, 20.0, 12.0);
	cube();
	glPopMatrix();
	
}
void draw_right_arm(void)
{

	glRotatef(t1r, 0.0, 0.0, 1.0);      // rotate the whole robot arm by t1r degrees

	draw_shouder();

	glTranslatef(0.0, 0.0, 38.0);		// M_(lower arm to base)

	glRotatef(t2r, 1.0, 0.0, 0.0);      // rotate upper and lower arm by t2r degrees

	draw_lower_arm();

	glTranslatef(0.0, 55.0, 0.0);      // M_(upper arm to lower arm) 

	glRotatef(t3r, 0.0, 0.0, 1.0);      // rotate upper arm by t3r degrees

	draw_upper_arm();


}

void draw_left_arm(void)
{

	glRotatef(t1l, 0.0, 0.0, 1.0);      // rotate the whole robot arm by t3 degrees

	draw_shouder();

	glTranslatef(0.0, 0.0, 38.0);		// M_(lower arm to base)

	glRotatef(t2l, 1.0, 0.0, 0.0);      // rotate upper and lower arm by t4 degrees

	draw_lower_arm();

	glTranslatef(0.0, 55.0, 0.0);      // M_(upper arm to lower arm) 

	glRotatef(t3l, 0.0, 0.0, 1.0);      // rotate upper arm by t5 degrees

	draw_upper_arm();

	
}








//三：画上半身体//


void draw_neck(void)
//画脖子r=30, h=10//
{
	

	glPushMatrix();

	glColor3f(0.0, 1.0, 1.0);

	glRotatef(-90, 1.0, 0.0, 0.0);// rotate about x axis by -90 deg.
	// draws a hollow cylinder along the z axis with base on x-y axis, origin at (0, 0, 0) 
	gluCylinder(pObj7, 20, 20, 5, 100, 100);
	// base radius 20 
	// top  radius 20 
	// height 5
	// 100 x 100 controls the sampling

	// draw a solid disk to cover the base 

	/*gluDisk(pObj8, 0, 30, 100, 100);//画圆柱的上下底面
	// draw a solid disk to cover the top
	glPushMatrix();
	glTranslatef(0, 10, 0);

	gluDisk(pObj9, 0, 30, 100, 100);//
	glPopMatrix();*/

	glPopMatrix();
	
}


void draw_head(void)
{
	glPushMatrix();

	glColor3f(1.0, 0.0, 1.0);

	glRotatef(-90.0, 1.0, 0.0, 0.0);   // rotate about x axis by -90 deg.
	// draws a hollow cylinder along the z axis with base on x-y axis, origin at (0, 0, 0) 
	gluCylinder(pObj10, 40,40, 60, 100, 100);
	// base radius 40  
	// top  radius 40 
	// height 80
	// 100 x 100 controls the sampling

	// draw a solid disk to cover the base 

	gluDisk(pObj11, 0, 40, 100, 100);//画圆柱的上下底面
	// draw a solid disk to cover the top
	glPushMatrix();
	glTranslatef(0, 0, 60);

	gluDisk(pObj12, 0, 40, 100, 100);//
	glPopMatrix();

	glPopMatrix();

}



void draw_uppper_body(void)
{
	draw_torso();

	glPushMatrix();
	
	glTranslatef(0.0, 110.0, 0.0);
		
	draw_neck();
    
	glTranslatef(0.0, 5.0, 0.0); 
	glRotatef(t7, 1.0, 0.0, 0.0);// rotate head by t7 degrees
	draw_head();

	glPopMatrix();


	glPushMatrix();

	glTranslatef(21.5, 93.0, 0.0);
	glRotatef(5.8, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-180, 0.0, 0.0, 1.0);
	draw_right_arm();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-21.5, 93.0, 0.0);
	glRotatef(-5.8, 0.0, 0.0, 1.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-180, 0.0, 0.0, 1.0);
	draw_left_arm();

	glPopMatrix();

	
}








//四：画裆部//


void draw_crotch(void)
{
	glPushMatrix();					// draw lower robotic arm : green box of dimension 10 x 40 x 40
	glColor3f(0.0, 1.0, 0.0);
	
	glScalef(5.0, 20.0, 20.0);
	glutSolidCube(2);
	glPopMatrix();


}












//五：画右脚//

void draw_leg(void)
// draw base : blue cylinder of radius 30 height 40  x-z as base
{
	glPushMatrix();

	glColor3f(0.0, 0.0, 1.0);
	

	// draws a hollow cylinder along the z axis with base on x-y axis, origin at (0, 0, 0) 
	gluCylinder(pObj4, 20, 20, 40, 100, 100);
	// base radius 20  
	// top  radius 20 
	// height 40
	// 100 x 100 controls the sampling

	// draw a solid disk to cover the base 
	gluDisk(pObj5, 0, 20, 100, 100);//画圆柱的上下底面

	// draw a solid disk to cover the top
	glPushMatrix();
	glTranslatef(0, 0, 40);

	gluDisk(pObj6, 0, 20, 100, 100);//100代表圆由100个点组成
	glPopMatrix();

	glPopMatrix();

	

}

void draw_foot(void)
{
	glPushMatrix();					// draw lower robotic arm : green box of dimension 30 x 50+20 x 40
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(0, 0, 20);
	glScalef(15.0, 35.0, 15.0);
	cube();
	glPopMatrix();


}

void draw_shoe(void)
{
	glPushMatrix();					// draw lower robotic arm : green box of dimension 30+10 x 10 x 40
	glColor3f(0.0, 1.0, 1.0);
	glTranslatef(0, 0, 20);
	glScalef(20.0, 5.0, 20.0);
	cube();
	glPopMatrix();
	
}

void draw_right_leg(void)
{ 

	glRotatef(t4r, 0.0, 0.0, 1.0);      // rotate the whole robot arm by t4 degrees

	draw_leg();

	glTranslatef(0.0, 0.0, 0.0);		// M_(lower arm to base)
	glRotatef(t5r, 0.0, 0.0, 1.0);      // rotate upper and lower arm by t5 degrees

	draw_foot();

	glTranslatef(5.0, 70.0, 0.0);      // M_(upper arm to lower arm) 
	glRotatef(t6r, 0.0, 0.0, 1.0);

	draw_shoe();


}
void draw_left_leg(void)
{

	glRotatef(-t4l, 0.0, 0.0, 1.0);      // rotate the whole robot arm by t3 degrees

	draw_leg();

	glTranslatef(0.0, 0.0, 0.0);		// M_(lower arm to base)

	glRotatef(-t5l, 0.0, 0.0, 1.0);      // rotate upper and lower arm by t4 degrees

	draw_foot();

	glTranslatef(5.0, 70.0, 0.0);      // M_(upper arm to lower arm) 
	glRotatef(-t6l, 0.0, 0.0, 1.0);

	draw_shoe();

	
}








//六：画最后的机器人//


void draw_robot(void)
{

	//////////////////////////////////////////////////////////////////
	// 
	// Setup perspective projection and the rotation
	// 
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // viewport is by default the display window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, double(viewport[2]) / viewport[3], 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 700, 0, 0, 0, 0, 1, 0);
	glMultMatrixf(gsrc_getmo());  // get the rotation matrix from the rotation user-interface
  //
  //////////////////////////////////////////////////////////////////


  /*  Enable Z buffer method for visibility determination. */
  //  Z buffer code starts

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Z buffer code ends */

	glClearColor(1.0, 1.0, 1.0, 0.0);	// Set display-window color to white.
	glClear(GL_COLOR_BUFFER_BIT);		// Clear display window.



	glRotatef(t9, 0.0, 1.0, 0.0);

	glPushMatrix();

	glRotatef(t8, 1.0, 0.0, 0.0);   //t8是弯腰的角度
	draw_crotch();
	glTranslatef(0.0, 20.0, 0.0);

	draw_uppper_body();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-45.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-180, 0.0, 0.0, 1.0);
	draw_right_leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-180, 0.0, 0.0, 1.0);
	draw_left_leg();
	glPopMatrix();

	glutSwapBuffers();
}

void animate(void)
// this animation function will swing theta from 0 deg. to 90 deg. in 1 seconds, then stop at 90 deg.
{
	

	double t;
	double t1 = 45;                  // 90 degrees
	double loop_time = 2000.0;				 // 1000 ms

				// return elapsed time in ms since the last call  


			/*t1 = -t1 / 2 + t1 * t / swing_time;
			t3 = -t3 / 2 + t3 * t / swing_time;*/

	t = glutGet(GLUT_ELAPSED_TIME);
	if (t - i > loop_time)
	{
		i = i + loop_time;
	}

	if (sin(PI * t / (loop_time)) >= 0)
	{
		t1r = t1l = -t1 + 2 * t1 * pow(sin(PI * (t - i) / (2 * loop_time)), 0.8);//0.8更慢一点
		t5r = t5l = -t1 + 2 * t1 * pow(sin(PI * (t - i) / (2 * loop_time)), 0.8);//0.7更慢一点
		t3r = t3l = -t1 + 2 * t1 * pow(sin(PI * (t - i) / (2 * loop_time)), 0.5);//0.5更快一点
		/*t1 = -t1 + 2 * t1 * (1 - pow(cos(PI * (t - t_in_loop) / (2 * loop_time)), 0.8));
		t3 = -t3 + 2 * t3 * (1 - pow(cos(PI * (t - t_in_loop) / (2 * loop_time)), 0.8));*/
	}

	if (sin(PI * t / (loop_time)) < 0)
	{
		/*t1 = t1 - 2 * t1 * pow(sin(PI * (t - t_in_loop) / (2 * loop_time)), 0.8);
		t3 = t3 - 2 * t3 * pow(sin(PI * (t - t_in_loop) / (2 * loop_time)), 0.8);*/
		t1r = t1l = t1 - 2 * t1 * (1 - pow(cos(PI * (t - i) / (2 * loop_time)), 0.8));
		t5r = t5l = t1 - 2 * t1 * (1 - pow(cos(PI * (t - i) / (2 * loop_time)), 0.8));//0.7更慢一点
		t3r = t3l = t1 - 2 * t1 * (1 - pow(cos(PI * (t - i) / (2 * loop_time)), 0.5));
	}

	t9 += 0.1;
	if (t9 > 360) t9 -= 360;

	glutPostRedisplay();
}

void drawing(GLubyte key, GLint xMouse, GLint yMouse)
{

	/* simple animation can be achieved by repeating key tabs */

	switch (key) {
	case '0':		t1r = t2r = t3r = t4r = t5r = t6r = t7 = t8 = t9= t1l = t2l = t3l = t4l = t5l = t6l = 0;
		break;
	case '1':		t1r = t1l += 30;
		break;
	case '2':		t2r = t2l += 30;
		break;
	case '3':		t3r = t3l += 30;
		break;
	case '4':		t4r = t4l += 30;
		break;
	case '5':		t5r = t5l += 30;
		break;
	case '6':		t6r = t6l += 30;
		break;
	case '7':		 t7 += 30;
		break;
	case '8':		 t8 += 30;
		break;
	case '9':		 t9 += 30;
		break;
	case 'w':
		i = i + j;
		t2r = t2l = 10;
		t9 += 1 ;
		if (i > -10 && i < 10)
		{

			t1r = t1l = 5 * i;
			if (i > 0)
			{
			t3l = 5 * i;
			t3r = 0;
           }
		else
		{
			t3r= 5 * i;
			t3l = 0;
			
		}

			t5r = t5l = 5 * i;
			t6r = t6l = 5 * i;
		}
		else 
		{
			j = -j;
		
		}
		break;

	case 'r':
		i = i + j;
		t2r = t2l = 20;
		t8 = 45;
		t9 = 90;
		if (i > -10 && i < 10)
		{

			t1r = t1l = 10 * i;
			if (i > 0)
			{
				t3l = 10 * i;
				t3r = 0;
			}
			else
			{
				t3r = 10 * i;
				t3l = 0;

			}

			t5r = t5l = 10 * i;
			t6r = t6l = 5 * i;
		}
		else
		{
			j = -j;

		}
		break;
	default:		printf("invalid choices!");
	}

	glutPostRedisplay();                   // redisplay the modified object

}


void main(int argc, char** argv)
{

	glutInit(&argc, argv);			                      // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Set display mode

	glutInitWindowPosition(WIN_POSX, WIN_POSY);         // Set display-window position at (WIN_POSX, WIN_POSY) 
														  // where (0, 0) is top left corner of monitor screen
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);		  // Set display-window width and height.

	glutCreateWindow("Robot");					  // Create display window.

	t1r = t1l = 0;  t2r = t2l = 0;  t3r = t3l = 0; t7 = t8 =t9=0;

	pObj1 = gluNewQuadric();
	pObj2 = gluNewQuadric();
	pObj3 = gluNewQuadric();
	pObj4 = gluNewQuadric();
	pObj5 = gluNewQuadric();
	pObj6 = gluNewQuadric(); 
	pObj7 = gluNewQuadric();
	pObj8 = gluNewQuadric();
	pObj9 = gluNewQuadric(); 
	pObj10 = gluNewQuadric();
	pObj11 = gluNewQuadric();
	pObj12 = gluNewQuadric();

	//printf("Type                                              \n");
	//printf("0 : Initial position							   \n");
	//printf("1 : Rotate only the shouuder by 30 deg           \n");
	//printf("2 : Rotate the lower arm by 30 deg                \n");
	//printf("3 : Rotate the upper arm by 30 deg                \n");
	//printf("4 : Rotate the leg by 30 deg                \n");
	//printf("5 : Rotate the foot by 30 deg                \n");
	//printf("6 : Rotate the shoe by 30 deg                \n");
	//printf("7 : Rotate the head by 30 deg                \n");
	//printf("8 : Rotate the upper body by 30 deg                \n");
	//printf("9 : Rotate the body by 30 deg                \n");
	//printf("w : the robot can walk                \n");
	//printf("r : the robot can run                \n");
	//glutKeyboardFunc(drawing);


	////////////////////////////////////////////////////////////////////
	//// 
	//// Register mouse-click and mouse-move glut callback functions
	//// for the rotation user-interface.
	//// 
	//glutMouseFunc(gsrc_mousebutton);
	//glutMotionFunc(gsrc_mousemove);
	////
	////////////////////////////////////////////////////////////////////


	glutDisplayFunc(draw_robot);   // put everything you wish to draw in drawscene

	glutIdleFunc(animate);

	glutMainLoop();

}