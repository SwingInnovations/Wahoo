#include <SDL2/SDL_video.h>
#include "STGame.h"

#include "../Graphics/STGraphics.h"
#include "../Graphics/GL/GLGraphics.h"
#include "../Graphics/Camera.h"

int STGame::RES_WIDTH = 0;
int STGame::RES_HEIGHT = 0;
STGame* STGame::m_instance = nullptr;

STGame::STGame() {
    m_Window = nullptr;
    m_Context = 0;
    WIDTH = 800;
    HEIGHT = 600;
    isRunning = false;
    m_currentIndex = 0;
    isPause = false;
}

STGame::~STGame() {

    m_currentIndex = 0;
    if(!m_gameStates.empty()){
        for(auto state : m_gameStates){
            delete state;
        }
    }
    m_gameStates.clear();
    g->cleanup();
    delete g;
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
    m_Window = nullptr;
    m_Context = 0;
}

STGame::STGame(const std::string title, unsigned int WIDTH, unsigned int HEIGHT) {
    setWidth(WIDTH);
    setHeight(HEIGHT);
    if(SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_JOYSTICK) == -1){
        std::cout << "Error 399: Failed to load SDL in general: " << SDL_GetError() << std::endl;
        if(SDL_NumJoysticks() < 1){
            std::cout << "Error 399b: No Joysticks Detected. " << std:: endl;
        }
    }else{
        m_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, this->WIDTH,
                                    this->HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if(m_Window == nullptr){
            std::cout << "Error 400: Failed to load Window:  " << SDL_GetError() << std::endl;
        }else{
            Input::Start(this, m_e);
        }
    }

    STGame::SetResolutionWidth(WIDTH);
    STGame::SetResolutionHeight(HEIGHT);

    m_currentIndex = 0;
    oldTime = 0;
    newTime = SDL_GetTicks();
    delta = 0;

    isPause = false;
}

void STGame::setOpenGLVersion(int MajorVersion, int MinorVersion) {
    m_graphics_MAJOR = MajorVersion;
    m_graphics_MINOR = MinorVersion;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MinorVersion);
    if(MajorVersion < 3 && MinorVersion < 2){
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        m_graphics_Profile = 0;
    }else{
        m_graphics_Profile = 1;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }


    m_Context = SDL_GL_CreateContext(m_Window);
    STGraphics::RENDERER = STGraphics::OPENGL;
    if(m_Context == NULL){
        std::cout << "Error 401: Unable to initialize GLContext" << std::endl;
    }else{
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();

        if(err != GLEW_OK){
            std::cout << "Error loading GLEW: " << glewGetErrorString(err);
        }else{
            std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
        }

        std::cout << "Using OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "Using Graphics Driver: " << glGetString(GL_VENDOR) << std::endl;
        g = new GLGraphics(this);
    }
}

void STGame::addState(STGameState * gameState) {
    m_gameStates.push_back(gameState);
}

Input* STGame::getInput() {
    return Input::Get();
}

void STGame::calcDelta() {
    if(newTime > oldTime){
        delta = newTime - oldTime;
        Uint32 targetInterval = (Uint32)(1000.0f / this->fps);
        if(delta < targetInterval || delta > targetInterval){
            delta = targetInterval;
        }
        oldTime = newTime;
        newTime = SDL_GetTicks();
    }
}

void STGame::start(){
    isRunning = true;
    init();
    while(isRunning){
        if(!isPause){
            calcDelta();
            Input::Get()->setDelta(delta);
        }else{
            delta = 0;
        }
        while(SDL_PollEvent(&m_e)) Input::Get()->poll(m_e);
        update();

        render();
    }
    if(Input::m_instance != nullptr) delete Input::m_instance;
    if(STGame::m_instance != nullptr) delete STGame::m_instance;
}

void STGame::init() {
    if(!m_gameStates.empty()){
        m_gameStates[m_currentIndex]->init_Internal(this);
        m_gameStates[m_currentIndex]->init(this);
    }
}

void STGame::enterState(unsigned int index) {
    if(!m_gameStates.empty() && index < m_gameStates.size()){
        m_currentIndex = index;
    }
}

void STGame::update() {
    auto input = Input::Get();
    if(input->isCloseRequested()){
        isRunning = false;
    }
    if(getCamera() != nullptr) getCamera()->update(input);
    if(!m_gameStates.empty() && m_currentIndex < m_gameStates.size()) m_gameStates[m_currentIndex]->update(this);
}

void STGame::render() {
    if(!m_gameStates.empty()){
        m_gameStates.at(m_currentIndex)->render(this);
    }

    SDL_GL_SwapWindow(m_Window);
}

void STGame::addCamera(Camera* cam){
    g->addCamera(cam);
}

Camera*STGame::getCamera(){
    return g->getActiveCamera();
}

STGraphics* STGame::getGraphics() {
    return g;
}

void STGame::centerCursor() {
    Input::Get()->centerMouseInWindow();
}

STGame *STGame::Init(const std::string &title, const stUint WIDTH, const stUint HEIGHT) {
    m_instance = new STGame(title, WIDTH, HEIGHT);
    return m_instance;
}

STGame *STGame::Get() {
    return m_instance;
}

void STGame::setIcon(const std::string &filePath) {
    SDL_Surface* img = nullptr;
    img = IMG_Load(filePath.c_str());
    if(img == nullptr){
        std::cerr << "Failed to load filepath. Invalid file?" << std::endl;
    }
    SDL_SetWindowIcon(m_Window, img);

    SDL_FreeSurface(img);
    img = 0;
}

void STGame::setActiveCamera(stUint index) {
    this->g->setCameraIndex(index);
}

void STGame::setFullScreen(int flag) {
    SDL_DisplayMode DM;

    switch(flag){
        case 0:
            SDL_SetWindowFullscreen(m_Window, 0);
            g->setResolution(WIDTH, HEIGHT);
            break;
        case 1:
            SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN);
            SDL_GetDesktopDisplayMode(0, &DM);
            g->setResolution(DM.w, DM.h);
            std::cout << "Fullscreen Desktop with " << DM.w << " : " << DM.h << std::endl;
            break;
        default:
            ;
    }
}

STScene *STGame::GetCurrentScene() {
    return m_gameStates[m_currentIndex]->getScene();
}