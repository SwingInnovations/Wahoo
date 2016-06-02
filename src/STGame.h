#ifndef WAHOO_STECHWINDOW_H
#define WAHOO_STECHWINDOW_H

#include "../ext/GL/glew.h"
extern "C"{
    #include "../ext/SDL2/SDL.h"
    #include "../ext/SDL2/SDL_opengl.h"
};

#define GLEW_STATIC

#include <string>
#include <iostream>
#include <vector>

#include "Input.h"
#include "STGameState.h"
#include "Math/Vector.h"
#include "STResourceManager.h"

class Input;

class STGameState;
class STGraphics;
class GLGraphics;
class Camera;

//! STGame Class
/*!
 *  Central class for creating a game with Swing Tech 1. Class holds
 *  information regarding current game state, a resource manager,
 *  and delta time.
 */
class STGame {
public:
    static int RES_WIDTH;
    static int RES_HEIGHT;
    //! Constructor
    /*!
     *  Default Constructor.
     */
    STGame();

    //! Constructor
    /*!
     *  \param title Window Title.
     *  \param WIDTH Window Width.
     *  \param HEIGHT Window Height.
     */
    STGame(const std::string title, unsigned int WIDTH, unsigned int HEIGHT);
    //! Sets the OpenGL Version
    /*!
     *  \param majorVersion Sets the major OpenGL Version.
     *  \param minorversion Sets the minor OpenGL Version.
     */
    void setOpenGLVersion(int majorVersion, int minorVersion);
    //! A destructor
    virtual ~STGame();
    //! Begins the game.
    /*!
     * Starts the game loop.
     */
    void start();
    //! Centers Cursor to screen.
    void centerCursor();
    //! Adds STGameState to game.
    /*!
     *  \param STGameState* STGameState to be added to the game.
     */
    void addState(STGameState *); // TODO - Add Structure Level Handler

    /*--General Getters and Setters--*/
    /*-The Setters-*/
    void useScript(bool v){ this->useLua = v; }
    //! Sets Game to Fullscreen
    /*!
     *  \param v Fullscreen state.
     */
    void useFullScreen(bool v){ this->isFullScreen = v; }
    //! Sets Game to Pause.
    /*!
     *  \param v Pause state.
     */
    void setPause(bool v){ this->isPause = v; }

    //! Sets Target FPS for game.
    /*!
     *  \param val Target FPS(Frames per Second)
     */
    void setTargetFPS(float val){ this->fps = (Uint32)val; }

    //! Sets Cursor Visible
    /*!
     *  \param v Cursor Visibility state.
     */
    void showCursor(bool v){ this->isCursorOnDisplay = v; }
    void setIcon(const std::string& filePath);
    //! Sets Current State of Game
    /*!
     *  \param v Current State
     */
    void enterState(unsigned int);
    //! Sets Clear Color for background of screen.
    /*!
     *  \param r Red value for clear color. From range [0.0f - 1.0f]
     *  \param g Green value for clear color. From range [0.0f - 1.0f]
     *  \param b Blue value for clear color. From range [0.0f - 1.0f]
     *  \param a Alpha value for clear color. From range [0.0f - 1.0f]
     */
    void setClearColor(stReal, stReal, stReal, stReal);
    //! Sets Clear Color for background of screen.
    /*!
     *  \param clearColor - Vector4 describing clear color;
     */
    void setClearColor(const Vector4<stReal> clearColor);
    //! Sets Camera for Game
    /*!
     *  Sets Camera for STGame. Note, camera will be persistant
     *  for all game states.
     *  \param cam Pointer to Camera
     */
    void addCamera(Camera* cam);

    void setWidth(int val){
        this->WIDTH = val;
        STGame::SetResolutionWidth(val);
    }

    void setHeight(int val){
        this->HEIGHT = val;
        STGame::SetResolutionHeight(val);
    }

    static void SetResolutionWidth(int val){ STGame::RES_WIDTH = val; }
    static void SetResolutionHeight(int val){ STGame::RES_HEIGHT = val; }

    /*-The Getters-*/
    Input* getInput();
    Camera* getCamera();
    STGraphics* getGraphics();
    STResourceManager* getResourceManager();
    int getWidth(){ return this->WIDTH; }
    int getHeight(){ return this->HEIGHT; }
    SDL_Window* getWindow(){ return this->m_Window; }
    Uint32 getDelta(){ return this->delta; }
protected:
    bool isRunning;
    void init(); // initialize all of the gamestates if added.
    void updateLogic();
    void updateInput(SDL_Event& event);
    void render();
    void calcDelta();
private:
    bool isPause;
    bool useLua;
    bool isFullScreen;
    bool isCursorOnDisplay;

    unsigned int m_currentIndex;
    std::vector<STGameState *> m_gameStates;
    Input* m_input;
    SDL_Window* m_Window;
    SDL_GLContext m_Context;
    SDL_Event m_e;
    STGraphics * g;
    unsigned int WIDTH, HEIGHT;
    int m_CurrentState;
    Uint32 delta, oldTime, newTime, fps;
    Vector4<stReal> m_clearColor;
    STResourceManager* resourceManager;
};


#endif //WAHOO_STECHWINDOW_H
