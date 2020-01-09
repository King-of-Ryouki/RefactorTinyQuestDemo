#include "game.hpp"

// #pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup") // delete console

Game cGame;

// glut funcs is "C".
void AppReshape(int w, int h) { cGame.Reshape(w, h); }
void AppRender() { cGame.Render(); }
void AppKeyboard(unsigned char key, int x, int y) { cGame.ReadKeyboard(key, x, y, true); }
void AppKeyboardUp(unsigned char key, int x, int y) { cGame.ReadKeyboard(key, x, y, false); }

void AppSpecialKeys(int key, int x, int y) { cGame.ReadSpecialKeyboard(key, x, y, true); }
void AppSpecialKeysUp(int key, int x, int y) { cGame.ReadSpecialKeyboard(key, x, y, false); }
void AppMouse(int button, int status, int x, int y) { cGame.ReadMouse(button, status, x, y); }
void AppIdle() {
    if (!cGame.Loop()) { exit(0); }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH); // RGBA with double buffer

    // create centered window
    int screen_w = 600;
    int screen_h = 860;

    glutInitWindowSize(screen_w, screen_h);

    int screen_x = (glutGet(GLUT_SCREEN_WIDTH) - screen_w) >> 1;
    int screen_y = (glutGet(GLUT_SCREEN_HEIGHT) - screen_h) >> 1;

    glutInitWindowPosition(screen_x, screen_y);
    glutCreateWindow("tiny_quest_demo");

    // register callback functions
    glutReshapeFunc(AppReshape);
    glutDisplayFunc(AppRender);
    glutKeyboardFunc(AppKeyboard);
    glutKeyboardUpFunc(AppKeyboardUp);

    glutSpecialFunc(AppSpecialKeys);
    glutSpecialUpFunc(AppSpecialKeysUp);
    glutMouseFunc(AppMouse);
    glutIdleFunc(AppIdle);

    GLint GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult) {

        cout << "ERROR : " << glewGetErrorString(GlewInitResult) << endl;

        exit(EXIT_FAILURE);
    }

    cGame.GameLevelInit(1); // game initialization

    glutMainLoop(); // application loop
    return 0;
}