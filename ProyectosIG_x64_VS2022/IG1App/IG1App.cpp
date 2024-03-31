#include "IG1App.h"
#include "CheckML.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
using namespace std;

// static single instance (singleton pattern)

IG1App IG1App::s_ig1app; // default constructor (constructor with no parameters)

void
IG1App::close()
{
	if (!mStop) { // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop(); // stops main loop and destroy the OpenGL context
		mStop = true;        // main loop stopped
	}
	free();
}

void
IG1App::run() // enters the main event processing loop
{
	if (mWinId == 0) {      // if not initialized
		init();         // initialize the application
		glutMainLoop(); // enters the main event processing loop
		mStop = true;   // main loop has stopped
	}
}

void
IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context
	// allocate memory and resources
	mViewPort =
	  new Viewport(mWinW, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mScene = new Scene;

    mViewPort2 =
	  new Viewport(mWinW, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera2 = new Camera(mViewPort2);
	mScene2 = new Scene;

	
	mCamera->set3D();
	mCamera2->set2D();

	mScene->setScene(4);
	mScene2->setScene(0);
}

void
IG1App::iniWinOpenGL()
{ // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);                   // GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH); // window size
	// glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |
	                    GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth
	                                                    // buffer and stencil buffer

	mWinId = glutCreateWindow(
	  "IG1App"); // with its associated OpenGL context, return window's identifier

	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update);

	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

void
IG1App::free()
{ // release memory and resources
	delete mScene;
	mScene = nullptr;
	delete mCamera;
	mCamera = nullptr;
	delete mViewPort;
	mViewPort = nullptr;
}

void
IG1App::display() const
{ // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

	/*if (m2Vistas) {
		Viewport auxVP = *mViewPort;
		mCamera->setSize(mWinW / 2, mWinH);
		Camera auxCam = *mCamera;
		mViewPort->setPos(0, 0);
		mViewPort->setSize(mWinW/2,mWinH);
		mScene->render(*mCamera);
		mViewPort->setPos(mWinW / 2, 0);
		auxCam.setCenital();
		mScene->render(auxCam);
		*mViewPort = auxVP;
	}
	else {
		mScene->render(*mCamera); // uploads the viewport and camera to the GPU
	}*/

	twoScenes();

	glutSwapBuffers(); // swaps the front and back buffer
}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}

void
IG1App::key(unsigned char key, int x, int y)
{
	bool need_redisplay = true;

	switch (key) {
		case 27:                     // Escape key
			glutLeaveMainLoop(); // stops main loop and destroy the OpenGL context
			break;
		case '+':
			mCamera->setScale(+0.01); // zoom in  (increases the scale)
			break;
		case '-':
			mCamera->setScale(-0.01); // zoom out (decreases the scale)
			break;
		case 'l':
			mCamera->set3D();
			break;
		case 'o':
			mCamera->set2D();
			break;
		case '0':
			mScene->setScene(0);
			mCamera->set2D();
			/*auto vM = mCamera->viewMat();
			 vM = glm::translate(vM,glm::dvec3(0,200,0));*/
			break;
		case '1':
			mScene->setScene(1);
			mCamera->set3D();
			break;
		case '2':
			mScene->setScene(2);
			mCamera->set3D();
			break;
	    case '3':
			mScene->setScene(3);
			mCamera->set3D();
			break;
		case '4':
			mScene->setScene(4);
			mCamera->set3D();
			//mCamera->setCenital();
			break;
		case 'u':
			if (pause) {
				mCamera->update();
				mScene->update();
			}
			break;
		case 'U':
			pause = !pause;
			break;
	    case 'F':
			savePhoto();
			break;
		case 'p':
			mCamera->changePrj();
			break;
		case 'k':
			m2Vistas = !m2Vistas;
			if (!m2Vistas) {
				mViewPort->setSize(mWinW, mWinH);
				mCamera->setSize(mWinW, mWinH);
				mViewPort->setPos(0, 0);
			}
			break;
		default:
			need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to
		                     // display()
}

void
IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	switch (key) {
		case GLUT_KEY_RIGHT:
			if (mdf == GLUT_ACTIVE_CTRL)
				//mCamera->pitch(-1); // rotates -1 on the X axis
				//mCamera->moveFB(-1);
				mCamera->rollReal(1);
			else
				//mCamera->pitch(1); // rotates 1 on the X axis
				//mCamera->moveLR(1);
				mCamera->yawReal(1);
			break;
		case GLUT_KEY_LEFT:
			if (mdf == GLUT_ACTIVE_CTRL)
				//mCamera->yaw(1); // rotates 1 on the Y axis
				//mCamera->moveFB(1);
				mCamera->rollReal(-1);
			else
				//mCamera->yaw(-1); // rotate -1 on the Y axis
				//mCamera->moveLR(-1);
				mCamera->yawReal(-1);
			break;
		case GLUT_KEY_UP:
			//mCamera->roll(1); // rotates 1 on the Z axis
			//mCamera->moveUD(1);
			mCamera->pitchReal(-1);
			break;
		case GLUT_KEY_DOWN:
			//mCamera->roll(-1); // rotates -1 on the Z axis
			//mCamera->moveUD(-1);
			mCamera->pitchReal(1);
			break;
		default:
			need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to
		                     // display()
}

void IG1App::update()
{
	if (!pause) {
		mCamera->update();
		mScene->update();
		glutPostRedisplay();
	}
}

void IG1App::savePhoto()
{
	Texture* tex = new Texture();

	tex->loadColorBuffer(800, 600);

	tex->saveData("save.bmp");
}

void IG1App::mouse(int button, int state, int x, int y)
{
	mMouseButt = button;

	//no tengo muy claro cual tenemos que usar, asi que dejo ambas para preguntar mas tarde
	//mMouseCoord = { x, glutGet(GLUT_WINDOW_HEIGHT) - y }; //coordenadas de viewport
	mMouseCoord = { x, y }; //coordenadas de ventana
}

void IG1App::motion(int x, int y)
{
	glm::dvec2 aux = {mMouseCoord.x - x, mMouseCoord.y - y };
	mMouseCoord = { x, y };


	if(mMouseButt == GLUT_RIGHT_BUTTON)
	{
	    mCamera->moveLR(-aux.x);
		mCamera->moveUD(aux.y);
	}
	else if (mMouseButt == GLUT_LEFT_BUTTON)
	{
		int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)
		if (mdf == GLUT_ACTIVE_CTRL)
		{
			mCamera->yawReal(-aux.x);
			mCamera->pitchReal(-aux.y);
		}
		else
		{
		    mCamera->yaw(-aux.x);
	    	mCamera->pitch(-aux.y);
		}

	}
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)
	if (mdf == GLUT_ACTIVE_CTRL)
		mCamera->setScale(d * 0.01); // zoom in  (increases the scale)
	else
		mCamera->moveFB(d * 5);
		

}

void IG1App::twoScenes() const
{
	mCamera->setSize(mWinW / 2, mWinH);
	mViewPort->setPos(0, 0);
	mViewPort->setSize(mWinW / 2, mWinH);
	mScene->render(*mCamera);

	mCamera2->setSize(mWinW / 2, mWinH);
	mViewPort2->setPos(mWinW / 2, 0);
	mViewPort2->setSize(mWinW / 2, mWinH);
	mScene2->render(*mCamera2);
}
