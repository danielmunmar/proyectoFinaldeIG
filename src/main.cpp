#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene    ();
void renderScene    ();
void drawBackground (Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void setCamera      ();
void setLights      (glm::mat4 P, glm::mat4 V);
void updateLights    ();
void drawObjectMat  (Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex  (Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChassis    (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHandlebar  (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBaseChassis(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawToilet     (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawWheels     (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelix      (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBlade      (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVehicle    (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTrasparent (glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);
void funTimer          (double seconds, double &t0);
void updateVehicle     ();

// Shaders
   Shaders shaders;
   Shaders shadersSky;

// Modelos
   Model sphere;
   Model wc;
   Model cover;
   Model cylinder;
   Model plane;
   Model wheel;
   Model cone;
   Model wcBack;

// Imagenes (texturas)
   Texture imgNoEmissive;
   Texture imgDay;
   Texture imgNight;
   Texture imgCarpetDiffuse;
   Texture imgCarpetSpecular;
   Texture imgCarpetNormal;
   Texture imgBladesDiffuse;
   Texture imgBladesSpecular;
   Texture imgBladesNormal;
   Texture imgGrassDiffuse;
   Texture imgGrassSpecular;
   Texture imgGrassNormal;

// Luces y materiales
   #define  NLD 1
   #define  NLP 1
   #define  NLF 2
   Light    lightG;
   Light    lightD[NLD];
   Light    lightP[NLP];
   Light    lightF[NLF];
   Material mluz;
   Material mchasis;
   Material mporcelain;
   Material mporcelaint;
   Material mblades;
   Material mcyan;
   Material mblackrubber;
   Textures texDay;
   Textures texNight;
   Textures texCarpet;
   Textures texBlades;
   Textures texGrass;

// Viewport
   int w = 500;
   int h = 500;
   glm::mat4 V;

// Animaciones
   float rotX       =   0.0;
   float rotY       =   0.0;
   float rotYH      =   0.0;
   float coverAngle =  0.0f;
   float turnWheel  =  0.0f;
   bool  fly        = false;
   bool  blend      =  true;

// Movimiento de camara
   float fovy1   = 60.0;
   float fovy2   = 60.0;
   float fovy3   = 60.0;
   float alphaX  =  0.0;
   float alphaY  =  0.0;
   float alphaX1 =  0.0;
   float alphaY1 =  0.0;
   int   camera  =    2;

// Vehicle movement
   glm::vec3 vehiclePos(0.0, 0.0, 0.0);
   float vehicleY     =   0.0;
   float vehicleSpeed =   0.0;
   float maxSpeed     =   5.0;
   float acceleration =   0.1;
   float deceleration =  0.05;
   float turnSpeed    =   2.5;
   float maxLimit     =  30.0;
   float minLimit     = -30.0;

// Flying parameters
   float flySpeed     =  0.1;
   float maxHeight    = 20.0;
   float minHeight    =  0.0;

// Lights
   glm::vec3 handleLeftLocal (-0.3, 0.8, -0.1);
   glm::vec3 handleRightLocal( 0.3, 0.8, -0.1);
   float focalI       =  1.0;
   bool focalOn       = true;
   float directionalI =  1.0;
   bool day           = true;

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
    glfwSetKeyCallback            (window, funKey);
    glfwSetScrollCallback         (window, funScroll);
    glfwSetCursorPosCallback      (window, funCursorPos);

 // Entramos en el bucle de renderizado
    configScene();
    double t0 = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
        funTimer(0.01, t0);
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
    shaders.initShaders   ("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");
    shadersSky.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshadersky.glsl");

 // Modelos
    sphere.initModel    ("resources/models/sphere.obj");
    wc.initModel        ("resources/models/wc.obj");
    cover.initModel     ("resources/models/wc_cover.obj");
    cylinder.initModel  ("resources/models/cylinder.obj");
    plane.initModel     ("resources/models/plane.obj");
    wheel.initModel     ("resources/models/wheel.obj");
    cone.initModel      ("resources/models/cone.obj");
    wcBack.initModel    ("resources/models/wc_back.obj");

 // Imagenes (texturas)
    imgNoEmissive.initTexture    ("resources/textures/imgNoEmissive.png");
    imgDay.initTexture           ("resources/textures/imgDay1.png");
    imgNight.initTexture         ("resources/textures/imgNight1.jpg");

    imgCarpetDiffuse.initTexture ("resources/textures/imgCarpetDiffuse.jpg");
    imgCarpetSpecular.initTexture("resources/textures/imgCarpetSpecular.jpg");
    imgCarpetNormal.initTexture  ("resources/textures/imgCarpetNormal.jpg");

    imgBladesDiffuse.initTexture ("resources/textures/imgBladesDiffuse.png");
    imgBladesSpecular.initTexture("resources/textures/imgBladesSpecular.png");
    imgBladesNormal.initTexture  ("resources/textures/imgBladesNormal.png");

    imgGrassDiffuse.initTexture  ("resources/textures/imgGrassDiffuse.png");
    imgGrassSpecular.initTexture ("resources/textures/imgGrassSpecular.png");
    imgGrassNormal.initTexture   ("resources/textures/imgGrassNormal.png");


 // Luz ambiental global
    lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

 // Luces direccionales
    lightD[0].direction = glm::vec3( -0.3, -1.0, -0.2);
    lightD[0].ambient   = glm::vec3( 0.25, 0.25, 0.25);
    lightD[0].diffuse   = glm::vec3( 0.7, 0.7, 0.7);
    lightD[0].specular  = glm::vec3( 0.4, 0.4, 0.4);

 // Luces posicionales
    lightP[0].position    = glm::vec3(0.0, 3.0, 3.0);
    lightP[0].ambient     = glm::vec3(0.0, 0.0, 0.0);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1000000.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

 // Luces focales
    for(int i = 0; i < NLF; i++) {
       lightF[i].ambient     = glm::vec3(0.05, 0.05, 0.05);
       lightF[i].diffuse     = glm::vec3(1.0, 1.0, 1.0);
       lightF[i].specular    = glm::vec3(0.6, 0.6, 0.6);

       lightF[i].innerCutOff = 12.0f;
       lightF[i].outerCutOff = lightF[i].innerCutOff + 8.0;

       lightF[i].c0 = 1.000;
       lightF[i].c1 = 0.090;
       lightF[i].c2 = 0.032;
    }

 // Materiales
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    mchasis.ambient   = glm::vec4(0.12, 0.12, 0.12, 1.0);
    mchasis.diffuse   = glm::vec4(0.50, 0.50, 0.50, 1.0);
    mchasis.specular  = glm::vec4(0.10, 0.10, 0.10, 1.0);
    mchasis.emissive  = glm::vec4(0.00, 0.00, 0.00, 1.0);
    mchasis.shininess = 15.0;

    mporcelain.ambient    = glm::vec4(0.10, 0.10, 0.10, 1.0);
    mporcelain.diffuse    = glm::vec4(0.85, 0.85, 0.85, 1.0);
    mporcelain.specular   = glm::vec4(0.20, 0.20, 0.20, 1.0);
    mporcelain.emissive   = glm::vec4(0.00, 0.00, 0.00, 1.0);
    mporcelain.shininess  = 30.0;

    mporcelaint.ambient   = glm::vec4(0.10, 0.10, 0.10, 0.8);
    mporcelaint.diffuse   = glm::vec4(0.85, 0.85, 0.85, 0.8);
    mporcelaint.specular  = glm::vec4(0.20, 0.20, 0.20, 0.8);
    mporcelaint.emissive  = glm::vec4(0.00, 0.00, 0.00, 1.0);
    mporcelaint.shininess = 30.0;

    mcyan.ambient   = glm::vec4(0.00000000, 0.1000000,0.06000000,1.0);
    mcyan.diffuse   = glm::vec4(0.00000000,0.50980392,0.50980392,1.0);
    mcyan.specular  = glm::vec4(0.50196078,0.50196078,0.50196078,1.0);
    mcyan.emissive  = glm::vec4(0.00000000,0.00000000,0.00000000,1.0);
    mcyan.shininess = 32.0;

    mblackrubber.ambient    = glm::vec4(0.02, 0.02, 0.02, 1.0);
    mblackrubber.diffuse    = glm::vec4(0.01, 0.01, 0.01, 1.0);
    mblackrubber.specular   = glm::vec4(0.40, 0.40, 0.40, 1.0);
    mblackrubber.emissive   = glm::vec4(0.00, 0.00, 0.00, 1.0);
    mblackrubber.shininess  = 10.0;

    texDay.diffuse       = imgDay.getTexture();
    texDay.specular      = imgDay.getTexture();
    texDay.emissive      = imgNoEmissive.getTexture();
    texDay.normal        = 0;
    texDay.shininess     = 1.0;

    texNight.diffuse     = imgNight.getTexture();
    texNight.specular    = imgNight.getTexture();
    texNight.emissive    = imgNoEmissive.getTexture();
    texNight.normal      = 0;
    texNight.shininess   = 1.0;

    texCarpet.diffuse    = imgCarpetDiffuse.getTexture();
    texCarpet.specular   = imgCarpetSpecular.getTexture();
    texCarpet.emissive   = imgNoEmissive.getTexture();
    texCarpet.normal     = imgCarpetNormal.getTexture();
    texCarpet.shininess  = 23.0;

    texBlades.diffuse    = imgBladesDiffuse.getTexture();
    texBlades.specular   = imgBladesSpecular.getTexture();
    texBlades.emissive   = imgNoEmissive.getTexture();
    texBlades.normal     = imgBladesNormal.getTexture();
    texBlades.shininess  = 36.0;

    texGrass.diffuse     = imgGrassDiffuse.getTexture();
    texGrass.specular    = imgGrassSpecular.getTexture();
    texGrass.emissive    = imgNoEmissive.getTexture();
    texGrass.normal      = imgGrassNormal.getTexture();
    texGrass.shininess   = 20.0;
}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float nplane =  0.1;
    float fplane = 200.0;
    float aspect = (float)w/(float)h;
    float fovy;
    if (camera == 1) {
       fovy = fovy1;
    } else if (camera == 2) {
       fovy = fovy2;
    } else {
       fovy = fovy3;
    }
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);;

   updateVehicle();

 // Matriz V
    setCamera();

 // Fijamos las luces
    updateLights();
    setLights(P, V);

 // Dibujamos la escena
    glm::mat4 S   = glm::scale    (I,glm::vec3( 50.0,  50.0,  50.0));
    glm::mat4 T2  = glm::translate( I, glm::vec3(0.00, -1.15, 0.00));
    glm::mat4 S2  = glm::scale    (I,glm::vec3(100.0, 100.0, 100.0));
    if (day) drawBackground(sphere, texDay, P, V,  S);
    else     drawBackground(sphere, texNight, P, V,S);
    drawObjectTex(plane , texGrass, P, V, T2 * S2);

    glm::mat4 T = glm::translate(I, vehiclePos);
    glm::mat4 R = glm::rotate   (I, glm::radians(vehicleY), glm::vec3(0,1,0));
    drawVehicle(P, V, T * R);
}

void drawBackground(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

   shadersSky.useShaders();
   shadersSky.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
   shadersSky.setMat4("uM"  , M);
   shadersSky.setMat4("uPVM",P*V*M);
   shadersSky.setBool("uWithMaterials",false);
   shadersSky.setTextures("utextures", textures);
   if(textures.normal != 0) shadersSky.setBool("uWithNormals", true);
   else                     shadersSky.setBool("uWithNormals",false);
   model.renderModel(GL_FILL);
   shaders.useShaders();

}

void setCamera() {
   if(camera == 2) {
      // Original camera (orbital)
      float x = 10.0f * glm::cos(glm::radians(alphaY)) * glm::sin(glm::radians(alphaX));
      float y = 10.0f * glm::sin(glm::radians(alphaY));
      float z = 10.0f * glm::cos(glm::radians(alphaY)) * glm::cos(glm::radians(alphaX));
      glm::vec3 eye    = vehiclePos + glm::vec3(x, y, z);
      glm::vec3 center = vehiclePos;
      glm::vec3 up(0.0, 1.0,  0.0);
      V = glm::lookAt(eye, center, up);
      shaders.setVec3("ueye",eye);

   } else if (camera == 3) {
      // Third person camera
      float yRad = glm::radians(vehicleY);

      glm::vec3 forward(-glm::sin(yRad), 0.0f, -glm::cos(yRad));

      glm::vec3 camOffset = -forward * 6.0f + glm::vec3(0.0, 3.0, 0.0);
      glm::vec3 eye       = vehiclePos + camOffset;
      glm::vec3 center    = vehiclePos + glm::vec3(0.0, 1.0, 0.0);

      V = glm::lookAt(eye, center, glm::vec3(0,1,0));
      shaders.setVec3("ueye", eye);
   } else {
      // First person
      glm::vec3 driverOffset(0.0, 1.0, 1.2);
      float x = 10.0f * glm::cos(glm::radians(alphaY1)) * glm::sin(glm::radians(alphaX1));
      float y = 10.0f * glm::sin(glm::radians(alphaY1));
      float z = 10.0f * glm::cos(glm::radians(alphaY1)) * glm::cos(glm::radians(alphaX1));

      glm::vec3 forward = glm::vec3(x, y, -z);
      glm::mat4 R       = glm::rotate(I, glm::radians(vehicleY), glm::vec3(0,1,0));
      glm::vec3 eye     = vehiclePos + glm::vec3(R * glm::vec4(driverOffset, 1.0f));
      glm::vec3 center  = eye + forward;

      V = glm::lookAt(eye, center, glm::vec3(0,1,0));
      shaders.setVec3("ueye", eye);
   }
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

void updateLights () {
 // Focal lights
   float yRad = glm::radians(vehicleY + rotY);

   // Direction forward
   glm::vec3 forward (-std::sin(yRad), -0.1, -std::cos(yRad));

   // Vehicle rotation
   glm::mat4 R = glm::rotate(I, glm::radians(vehicleY), glm::vec3(0,1,0));

   // Ajust position
   lightF[0].position  = vehiclePos + glm::vec3(R * glm::vec4(handleLeftLocal,  1.0));
   lightF[1].position  = vehiclePos + glm::vec3(R * glm::vec4(handleRightLocal, 1.0));

   // Set direction
   lightF[0].direction = forward;
   lightF[1].direction = forward;

   // Set intensity
   float Intensity = focalOn ? focalI : 0.0;

   for(int i = 0; i < NLF; i++) {
      lightF[i].diffuse  = glm::vec3(0.6f) * Intensity;
      lightF[i].specular = glm::vec3(0.4f) * Intensity;
   }

 // Directional lights
   lightD[0].diffuse  = glm::vec3(0.7f) * directionalI;
   lightD[0].specular = glm::vec3(0.4f) * directionalI;
}

void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial( "umaterial", material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  , M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures", textures);
    if(textures.normal != 0) shaders.setBool("uWithNormals",true);
    else                    shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}

void drawChassis(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   glm::mat4 R = glm::rotate(I, glm::radians(rotY), glm::vec3(0,1,0));

   drawBaseChassis(P, V, M);
   drawHandlebar(P, V, M * R);
}

void drawHandlebar(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   // Vertical Pole
   glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.8, 0.05));
   drawObjectMat(cylinder, mchasis, P, V, M * S);

   // Horizontal Pole
   glm::mat4 T1 = glm::translate(I, glm::vec3(18.0, 0.0, 0.0));
   glm::mat4 R  = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0,0,1));
   glm::mat4 S1 = glm::scale    (I, glm::vec3(0.04, 0.6, 0.04));
   drawObjectMat(cylinder, mchasis, P, V, M * R * S1 * T1);

   // Left Handle
   glm::mat4 T2 = glm::translate(I, glm::vec3(18.0, 3.1, 0.0));
   glm::mat4 S2 = glm::scale    (I, glm::vec3(0.04, 0.15, 0.05));
   drawObjectMat(cylinder, mcyan, P, V, M * R * S2 * T2);

   // Right Handle
   glm::mat4 T3 = glm::translate(I, glm::vec3(18.0, -3.1, 0.0));
   glm::mat4 S3 = glm::scale    (I, glm::vec3(0.04, 0.15, 0.05));
   drawObjectMat(cylinder, mcyan, P, V, M * R * S3 * T3);
}

void drawBaseChassis(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

   glm::mat4 R1 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0,0,1));
   glm::mat4 S1 = glm::scale (I, glm::vec3(0.05, 0.8, 0.05));

   // Front
   glm::mat4 T1 = glm::translate(I, glm::vec3(0.0, -0.8,  0.0));
   drawObjectMat(cylinder, mchasis, P, V, M * T1 * R1 * S1);

   // Back
   glm::mat4 T2 = glm::translate(I, glm::vec3(0.0, -0.8,  2.0));
   drawObjectMat(cylinder, mchasis, P, V, M * T2 * R1 * S1);


   glm::mat4 R2 = glm::rotate(I, glm::radians(90.0f), glm::vec3(1,0,0));
   glm::mat4 S2 = glm::scale (I, glm::vec3(0.05, 1.0, 0.05));

   // Left
   glm::mat4 T3 = glm::translate(I, glm::vec3(-0.5, -0.8, 1.0));
   drawObjectMat(cylinder, mchasis, P, V, M * T3 * R2 * S2);

   // Right
   glm::mat4 T4 = glm::translate(I, glm::vec3( 0.5, -0.8, 1.0));
   drawObjectMat(cylinder, mchasis, P, V, M * T4 * R2 * S2);

   // Base
   glm::mat4 T5 = glm::translate(I, glm::vec3(0.0, -0.8, 1.0));
   glm::mat4 S5 = glm::scale    (I, glm::vec3(0.5,  1.0, 1.0));
   drawObjectTex(plane, texCarpet, P, V, M * T5 * S5);
}

void drawToilet(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   glm::mat4 T = glm::translate(I, glm::vec3(0.0, -0.8, 1.5));
   glm::mat4 R = glm::rotate   (I, glm::radians(180.0f), glm::vec3(0,1,0));
   glm::mat4 S = glm::scale    (I, glm::vec3(2.0, 2.0, 2.0));
   drawObjectMat(wc, mporcelain, P, V, M * T * R * S);

   // Lift cover
   glm::mat4 Tcover;
   if (coverAngle == 0.0f) {
      Tcover = glm::translate(I, glm::vec3(0.0, -0.8, 1.5));
   } else {
      Tcover = glm::translate(I, glm::vec3(0.0, 0.03, 0.65));
   }
   glm::mat4 Rcover = glm::rotate(I, glm::radians(coverAngle), glm::vec3(1, 0, 0));
   drawObjectMat(cover, mporcelain, P, V, M * Tcover * R * Rcover * S);

   // Axis cylinder
   glm::mat4 T2 = glm::translate(I, glm::vec3(0.00, 1.58, 1.70));
   glm::mat4 S2 = glm::scale    (I, glm::vec3(0.05, 0.80, 0.05));
   drawObjectMat(cylinder, mchasis, P, V, M * T2 * S2);
}

void drawWheels(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

   glm::mat4 T1      = glm::translate(I, glm::vec3(-0.8, -0.8, 2.0));
   glm::mat4 T2      = glm::translate(I, glm::vec3( 0.8, -0.8, 2.0));
   glm::mat4 T3      = glm::translate(I, glm::vec3(-0.8, -0.8, 0.0));
   glm::mat4 T4      = glm::translate(I, glm::vec3( 0.8, -0.8, 0.0));

   glm::mat4 S       = glm::scale    (I, glm::vec3(0.5, 0.5, 0.5));
   glm::mat4 R       = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));
   glm::mat4 RLeft   = glm::rotate   (I, glm::radians(180.0f), glm::vec3(0,1,0));
   glm::mat4 RWheelL = glm::rotate   (I, glm::radians(turnWheel),  glm::vec3(1,0,0));
   glm::mat4 RWheelR = glm::rotate   (I, glm::radians(-turnWheel), glm::vec3(1,0,0));
   glm::mat4 RF;
   glm::mat4 RRight;

   if (fly) {
      R      = I;
      RF     = glm::rotate   (I, glm::radians(90.0f),  glm::vec3(0,0,1));
      RRight = glm::rotate   (I, glm::radians(180.0f), glm::vec3(0,1,0));
      T1     = glm::translate(I, glm::vec3(-0.4, -0.9, 2.0));
      T2     = glm::translate(I, glm::vec3( 0.4, -0.9, 2.0));
      T3     = glm::translate(I, glm::vec3(-0.4, -0.9, 0.0));
      T4     = glm::translate(I, glm::vec3( 0.4, -0.9, 0.0));
   } else {
      RF     = I;
      RRight = I;
   }

   // Back wheels
   drawObjectMat(wheel, mblackrubber, P, V, M * T1 * RF * RLeft  * RWheelL * S);
   drawObjectMat(wheel, mblackrubber, P, V, M * T2 * RF * RRight * RWheelR * S);
   // Front wheels
   drawObjectMat(wheel, mblackrubber, P, V, M * T3 * RF * R * RLeft  * RWheelL * S);
   drawObjectMat(wheel, mblackrubber, P, V, M * T4 * RF * R * RRight * RWheelR * S);
}

void drawHelix(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   int nblades = 6;
   glm::mat4 R = glm::rotate(I, glm::radians(360.0f/nblades), glm::vec3(0, 1, 0));
   for(int i = 0; i < nblades; i++) {
      drawBlade(P, V, M);
      M = M * R;
   }
}

void drawBlade(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   glm::mat4 T  = glm::translate(I, glm::vec3(0.00, -1.0, 0.0));
   glm::mat4 T2 = glm::translate(I, glm::vec3(0.00, 0.75, 0.0));
   glm::mat4 S  = glm::scale    (I, glm::vec3(0.05, 0.5, 0.25));
   glm::mat4 R  = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0, 0, 1));
   float rotation;
   if (fly) {
      rotation = 0.0f;
   } else {
      rotation = -60.0f;
   }
   glm::mat4 R2 = glm::rotate(I, glm::radians(rotation), glm::vec3(0, 0, 1));
   drawObjectTex(cone, texBlades, P, V, M * T2 * R * R2 * S * T);
}

void drawVehicle(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   drawChassis(P, V, M);
   drawToilet (P, V, M);
   drawWheels (P, V, M);

   glm::mat4 TH = glm::translate(I, glm::vec3(0.0, 1.58, 1.7));
   glm::mat4 RH  = glm::rotate  (I, glm::radians(rotYH), glm::vec3(0, 1, 0));
   drawHelix(P, V,M * TH * RH);
   drawTrasparent(P, V, M);
}

void drawTrasparent (glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   glm::mat4 T = glm::translate(I, glm::vec3(0.0, -0.8, 1.5));
   glm::mat4 R = glm::rotate   (I, glm::radians(180.0f), glm::vec3(0,1,0));
   glm::mat4 S = glm::scale    (I, glm::vec3(2.0, 2.0, 2.0));
   if(blend) {
      glEnable(GL_BLEND);
      glDepthMask(GL_FALSE);
   }
   drawObjectMat(wcBack, mporcelaint, P, V, M * T * R * S);
   if(blend) {
      glDisable(GL_BLEND);
      glDepthMask(GL_TRUE);
   }
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        case GLFW_KEY_W:  // Accelerate
          vehicleSpeed += acceleration;
          if(vehicleSpeed > maxSpeed) vehicleSpeed = maxSpeed;
          break;

        case GLFW_KEY_S:  // Slow down or backwards
          vehicleSpeed -= acceleration;
          if(vehicleSpeed < -maxSpeed / 2) vehicleSpeed = -maxSpeed / 2;
          break;

        case GLFW_KEY_A:  // Turn Left
          if(vehicleSpeed != 0)
             vehicleY += turnSpeed * (vehicleSpeed / maxSpeed);
          break;

        case GLFW_KEY_D:  // Turn Right
          if(vehicleSpeed != 0)
             vehicleY -= turnSpeed * (vehicleSpeed/ maxSpeed);
          break;

        case GLFW_KEY_UP:   // Fly up
          if(fly) {
             vehiclePos.y += flySpeed;
             if(vehiclePos.y > maxHeight) vehiclePos.y = maxHeight;
          }
          break;

        case GLFW_KEY_DOWN: // Fly down
          if(fly) {
             vehiclePos.y -= flySpeed;
             if(vehiclePos.y < minHeight) vehiclePos.y = minHeight;
          }
          break;

        case GLFW_KEY_1: if (action == GLFW_PRESS)     camera = 1;  break;
        case GLFW_KEY_2: if (action == GLFW_PRESS)     camera = 2;  break;
        case GLFW_KEY_3: if (action == GLFW_PRESS)     camera = 3;  break;
        case GLFW_KEY_RIGHT:  rotY -= rotY > -35.0f ? 5.0f : 0.0f;  break; // Turn handle right
        case GLFW_KEY_LEFT:   rotY += rotY <  35.0f ? 5.0f : 0.0f;  break; // Turn handel left
        case GLFW_KEY_T:  // Lift wc cover
          if(action == GLFW_PRESS) coverAngle = (coverAngle == 0.0f) ? -90.0f : 0.0f;
          break;
        case GLFW_KEY_F: if (action == GLFW_PRESS) fly = !fly;   break;            // Fly mode
        case GLFW_KEY_SPACE: if(action == GLFW_PRESS) vehicleSpeed = 0.0;   break; // Stop vehicle
        case GLFW_KEY_B: if (action == GLFW_PRESS) blend = !blend;   break;  // Change transparency
        case GLFW_KEY_R:  // Reset actual camera
          if(action == GLFW_PRESS) {
             if (camera == 1) {
                fovy1   = 60.0;
                alphaX1 =  0.0;
                alphaY1 =  0.0;
             } else if (camera == 2) {
                fovy2   = 60.0;
                alphaX  =  0.0;
                alphaY  =  0.0;
             } else {
                fovy3   = 60.0;
             }
          }
          break;
        case GLFW_KEY_L: if (action == GLFW_PRESS) focalOn = !focalOn;   break; // ON-OFF Focal lights
        case GLFW_KEY_RIGHT_BRACKET: // TECLA +     Increase focal lights intesity
          if (focalOn) if (action == GLFW_PRESS) focalI = 10.0;   break;
        case GLFW_KEY_SLASH: // TECLA -     Decrease focal lights intesity
          if (focalOn) if (action == GLFW_PRESS) focalI = 1.0;   break;
          
        case GLFW_KEY_0:   // Day-Night
          if (action == GLFW_PRESS) {
             day = !day;
             directionalI = (day) ? 1.0 : 0.0;
          }
          break;
        default:  // Reset everything but cameras
          rotX          =   0.0;
          rotY          =   0.0;
          rotYH         =   0.0;
          coverAngle    =  0.0f;
          turnWheel     =  0.0f;
          fly           = false;
          blend         =  true;

          vehiclePos    = glm::vec3(0.0, 0.0, 0.0);
          vehicleY      = 0.0;
          vehicleSpeed  = 0.0;

          focalI        =  1.0;
          focalOn       = true;
          directionalI  =  1.0;
          day           = true;
          break;
    }

}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

   if (camera == 1) {
      if(yoffset > 0) fovy1 -= fovy1 > 10.0f ? 5.0f : 0.0f;
      if(yoffset < 0) fovy1 += fovy1 < 90.0f ? 5.0f : 0.0f;
   } else if (camera == 2) {
      if(yoffset > 0) fovy2 -= fovy2 > 10.0f ? 5.0f : 0.0f;
      if(yoffset < 0) fovy2 += fovy2 < 90.0f ? 5.0f : 0.0f;
   } else {
      if(yoffset > 0) fovy3 -= fovy3 > 10.0f ? 5.0f : 0.0f;
      if(yoffset < 0) fovy3 += fovy3 < 90.0f ? 5.0f : 0.0f;
   }

}

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

   if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) return;

   float limY = 89.0;
   if (camera == 2) {
      alphaX = 90.0 * (2.0 * xpos / (float)w - 1.0);
      alphaY = 90.0 * (1.0 - 2.0 * ypos / (float)h);
      if(alphaY < -limY) alphaY = -limY;
      if(alphaY > limY ) alphaY =  limY;
   } else if (camera == 1) {
      alphaX1 = 90.0 * (2.0 * xpos / (float)w - 1.0);
      alphaY1 = 90.0 * (1.0 - 2.0 * ypos / (float)h);
      if(alphaY1 < -limY) alphaY1 = -limY;
      if(alphaY1 > limY ) alphaY1 =  limY;
   }
}

void funTimer(double seconds, double &t0) {

   double t1 = glfwGetTime();
   if(t1 - t0 > seconds) {
      rotYH += fly ? 5.0 : 0.0;
      t0 = t1;
   }

}

void updateVehicle() {

   // IMP! Manage movement before setLights
   double currentTime     = glfwGetTime();
   static double lastTime =   currentTime;
   double dt = currentTime - lastTime;
   lastTime  = currentTime;

   turnWheel += vehicleSpeed * 360.0f * dt / (2 * glm::pi<float>() * 0.4f);

   // Update position using velocity and direction
   if (!fly and vehiclePos.y != minHeight) { // Landing Y level
      vehicleSpeed = 0;
      vehiclePos.y -= flySpeed * 5;
      if(vehiclePos.y < minHeight) vehiclePos.y = minHeight;
   } else { // Driving in x and z only
      float yRad = glm::radians(vehicleY);
      vehiclePos.x -= vehicleSpeed * glm::sin(yRad) * dt;
      vehiclePos.z -= vehicleSpeed * glm::cos(yRad) * dt;
      if(vehiclePos.x > maxLimit) vehiclePos.x = maxLimit;
      if(vehiclePos.x < minLimit) vehiclePos.x = minLimit;
      if(vehiclePos.z > maxLimit) vehiclePos.z = maxLimit;
      if(vehiclePos.z < minLimit) vehiclePos.z = minLimit;

      // Natural slow down
      if(vehicleSpeed > 0.0) {
         vehicleSpeed -= deceleration;
         if(vehicleSpeed < 0.0) vehicleSpeed = 0.0;
      }
      if(vehicleSpeed < 0.0) {
         vehicleSpeed += deceleration;
         if(vehicleSpeed > 0.0) vehicleSpeed = 0.0;
      }
   }
}