#include "IG1App.h"
#include "CheckML.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
using namespace std;

//#define TOW_WINDOWS

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

#ifdef TOW_WINDOWS
	mViewPort =
		new Viewport(mWinW/2, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
#else
	mViewPort =
	  new Viewport(mWinW, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
#endif // TOW_WINDOWS
	mCamera = new Camera(mViewPort);
	mScene = new Scene;

#ifdef TOW_WINDOWS
    mViewPort2 =
	  new Viewport(mWinW/2, mWinH); // glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera2 = new Camera(mViewPort2);
	mScene2 = new Scene;

	mViewPort2->setPos(mWinW / 2, 0);
	mCamera2->set2D();
	mCamera2->setSize(mViewPort2->width(), mViewPort2->height());
	mScene2->setScene(0);
#endif // TOW_WINDOWS
	
	mViewPort->setPos(0, 0);

	mCamera->set3D();

	mCamera->setSize(mViewPort->width(), mViewPort->height());

	mScene->setScene(7);
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
#ifdef TOW_WINDOWS
	delete mScene2;
	mScene2 = nullptr;
	delete mCamera2;
	mCamera2 = nullptr;
	delete mViewPort2;
	mViewPort2 = nullptr;
#endif
}

void
IG1App::display() const
{ // double buffering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

#ifndef TOW_WINDOWS
	if (m2Vistas) {
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
	}
#else
	twoScenes();
#endif // !TOW_WINDOWS

	glutSwapBuffers(); // swaps the front and back buffer
}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;
#ifdef TOW_WINDOWS

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth / 2, newHeight);

	mViewPort2->setSize(newWidth/2, newHeight);
	mViewPort2->setPos(newWidth / 2, 0);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
	mCamera2->setSize(mViewPort2->width(), mViewPort2->height());
#else
	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
#endif // TOW_WINDOWS
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
		case '5':
			mScene->setScene(5);
			mCamera->set3D();
			break;

		case '6':
			mScene->setScene(6);
			mCamera->set3D();
			mCamera->moveFB(400);
			break;
		case '7':
			mScene->setScene(7);
			mCamera->set3D();
			break;
		case 'u':
			if (pause) {
#ifdef TOW_WINDOWS
				if (mMouseCoord.x < mWinW / 2) {
					mCamera->update();
					mScene->update();
				}
				else {
					mCamera2->update();
					mScene2->update();
				}
#else
				mCamera->update();
				mScene->update();
#endif // TOW_WINDOWS
			}
			break;
		case 'U':
			pause = !pause;
			break;
	    case 'F':
			savePhoto();
			break;
		case 'p':
#ifdef TOW_WINDOWS


			if (mMouseCoord.x < mWinW / 2) {
				mCamera->changePrj();
			}
			else {
				mCamera2->changePrj();
			}
#else
			mCamera->changePrj();
#endif // TOW_WINDOWS
			break;
		case 'k':
#ifndef TOW_WINDOWS

			m2Vistas = !m2Vistas;
			if (!m2Vistas) {
				mViewPort->setSize(mWinW, mWinH);
				mCamera->setSize(mWinW, mWinH);
				mViewPort->setPos(0, 0);
			}
#endif // !TOW_WINDOWS
			break;

		case'f':
			mScene->rotate();
			break;
		case'g':
			mScene->orbit();
			break;
		case'q':
			mScene->dirLightOnOff(false);
			break;
		case'w':
			mScene->dirLightOnOff(true);
			break;
		case'a':
			mScene->posLightOnOff(false);
			break;
		case's':
			mScene->posLightOnOff(true);
			break;
		case'z':
			mScene->spotLightOnOff(false);
			break;
		case'x':
			mScene->spotLightOnOff(true);
			break;
		case'v':
			mScene->tieLightOnOff(false);
			break;
		case'b':
			mScene->tieLightOnOff(true);
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
#ifdef TOW_WINDOWS


		if (mMouseCoord.x < mWinW / 2) {
			mCamera->update();
			mScene->update();
		}
		else {
			mCamera2->update();
			mScene2->update();
		}
#else
		mCamera->update();
		mScene->update();
#endif // TOW_WINDOWS
	
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

	Camera* auxCamera = mCamera;

#ifdef TOW_WINDOWS
	if (mMouseCoord.x >= mWinW / 2) {
		auxCamera = mCamera2;
	}
#endif // TOW_WINDOWS



	if(mMouseButt == GLUT_RIGHT_BUTTON)
	{
		auxCamera->moveLR(-aux.x);
		auxCamera->moveUD(aux.y);
	   
	}
	else if (mMouseButt == GLUT_LEFT_BUTTON)
	{
		int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)
		if (mdf == GLUT_ACTIVE_CTRL)
		{
			auxCamera->yawReal(aux.x * 0.05);
			auxCamera->pitchReal(aux.y * 0.05);
		}
		else
		{
			auxCamera->yaw(aux.x * 0.05);
			auxCamera->pitch(aux.y * 0.05);
		   
		}

	}
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)
	Camera* auxCamera = mCamera;
#ifdef TOW_WINDOWS
	if (mMouseCoord.x >= mWinW / 2) {
		auxCamera = mCamera2;
	}
#endif // TOW_WINDOWS
	if (mdf == GLUT_ACTIVE_CTRL) {

		auxCamera->setScale(d * 0.1); // zoom in  (increases the scale)
	}
	else {
		auxCamera->moveFB(d * 100);
	}
		

}

void IG1App::twoScenes() const
{

	mScene->render(*mCamera);


	mScene2->render(*mCamera2);
}
