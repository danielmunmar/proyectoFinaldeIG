#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChassis(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHandlebar(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);

// Shaders
   Shaders shaders;

// Modelos
   Model sphere;
   Model wc;
   Model cylinder;

// Imagenes (texturas)


// Luces y materiales
   #define   NLD 1
   #define   NLP 1
   #define   NLF 2
   Light     lightG;
   Light     lightD[NLD];
   Light     lightP[NLP];
   Light     lightF[NLF];
   Material  mluz;
   Material  mporcelain;
   Material  mcyan;
   Material  mblackrubber;
   //Textures  texRuby;

// Viewport
   int w = 500;
   int h = 500;

// Animaciones
   float rotX = 0.0;
   float rotY = 0.0;
   float desZ = 0.0;

// Movimiento de camara
   float fovy   = 60.0;
   float alphaX =  0.0;
   float alphaY =  0.0;

int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Proyecto Final", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback      (window, funKey);
    glfwSetScrollCallback   (window, funScroll);
    glfwSetCursorPosCallback(window, funCursorPos);

 // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void configScene() {

 // Test de profundidad
    glEnable(GL_DEPTH_TEST);

 // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    sphere.initModel("resources/models/sphere.obj");
    wc.initModel("resources/models/wc_toilet.obj");
    cylinder.initModel("resources/models/cylinder.obj");

 // Imagenes (texturas)


 // Luz ambiental global
    lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

 // Luces direccionales
    lightD[0].direction = glm::vec3(-1.0, 0.0, 0.0);
    lightD[0].ambient   = glm::vec3( 0.1, 0.1, 0.1);
    lightD[0].diffuse   = glm::vec3( 0.7, 0.7, 0.7);
    lightD[0].specular  = glm::vec3( 0.7, 0.7, 0.7);

 // Luces posicionales
    lightP[0].position    = glm::vec3(0.0, 3.0, 3.0);
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

 // Luces focales
    lightF[0].position    = glm::vec3(-2.0,  2.0,  5.0);
    lightF[0].direction   = glm::vec3( 2.0, -2.0, -5.0);
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;
    lightF[1].position    = glm::vec3( 2.0,  2.0,  5.0);
    lightF[1].direction   = glm::vec3(-2.0, -2.0, -5.0);
    lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].innerCutOff = 5.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

 // Materiales
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    mporcelain.ambient   = glm::vec4(0.25, 0.20725, 0.20725, 0.922);
    mporcelain.diffuse   = glm::vec4(1.0, 0.829, 0.829, 0.922);
    mporcelain.specular  = glm::vec4(0.296648, 0.296648, 0.296648, 0.922);
    mporcelain.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    mporcelain.shininess = 11.264;

    mcyan.ambient   = glm::vec4(0.0, 0.1,0.06,1.0);
    mcyan.diffuse   = glm::vec4(0.0,0.50980392,0.50980392,1.0);
    mcyan.specular  = glm::vec4(0.50196078,0.50196078,0.50196078,1.0);
    mcyan.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    mcyan.shininess = 32.0;

    mblackrubber.ambient   = glm::vec4(0.02, 0.02, 0.02, 1.0);
    mblackrubber.diffuse   = glm::vec4(0.01, 0.01, 0.01, 1.0);
    mblackrubber.specular  = glm::vec4(0.4, 0.4, 0.4, 1.0);
    mblackrubber.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    mblackrubber.shininess = 10.0;
}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (  x,   y,   z);
    glm::vec3 center(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena
    drawChassis(P, V, I);
}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1));
        drawObjectMat(sphere, mluz, P, V, M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
        drawObjectMat(sphere, mluz, P, V, M);
    }

}

void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}

void drawChassis(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   glm::mat4 R = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));
   drawHandlebar(P, V, R);
}

void drawHandlebar(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   // Vertical Pole
   glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.8, 0.05));
   drawObjectMat(cylinder, mporcelain, P, V, M*S);

   // Horizontal Pole
   glm::mat4 T1 = glm::translate(I, glm::vec3(18.0, 0.0, 0.0));
   glm::mat4 R1 = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,0,1));
   glm::mat4 S1 = glm::scale    (I, glm::vec3(0.04, 0.6, 0.04));
   drawObjectMat(cylinder, mporcelain, P, V, M*R1*S1*T1);

   // Left Handle
   glm::mat4 T2 = glm::translate(I, glm::vec3(18.0, 3.0, 0.0));
   glm::mat4 R2 = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,0,1));
   glm::mat4 S2 = glm::scale    (I, glm::vec3(0.04, 0.15, 0.05));
   drawObjectMat(cylinder, mcyan, P, V, M*R2*S2*T2);

   // Right Handle
   glm::mat4 T3 = glm::translate(I, glm::vec3(18.0, -3.0, 0.0));
   glm::mat4 R3 = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,0,1));
   glm::mat4 S3 = glm::scale    (I, glm::vec3(0.04, 0.15, 0.05));
   drawObjectMat(cylinder, mcyan, P, V, M*R3*S3*T3);
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) { fovy -= fovy > 25.0f ? 5.0f : 0.0f;
        case GLFW_KEY_UP:    rotX -= 5.0f;   break;
        case GLFW_KEY_DOWN:  rotX += 5.0f;   break;
        case GLFW_KEY_LEFT:  rotY -= rotY > -75.0f ? 5.0f : 0.0f;   break;
        case GLFW_KEY_RIGHT: rotY += rotY < 75.0f ? 5.0f : 0.0f;   break;
        case GLFW_KEY_Z:
            if(mods==GLFW_MOD_SHIFT) desZ -= desZ > -24.0f ? 0.1f : 0.0f;
            else                     desZ += desZ <   5.0f ? 0.1f : 0.0f;
            break;
        default:
            rotX = 0.0f;
            rotY = 0.0f;
            break;
    }

}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

    if(yoffset>0) fovy -= fovy>10.0f ? 5.0f : 0.0f;
    if(yoffset<0) fovy += fovy<90.0f ? 5.0f : 0.0f;

}

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) return;

    float limY = 89.0;
    alphaX = 90.0*(2.0*xpos/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*ypos/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;

}
