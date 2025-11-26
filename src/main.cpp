#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"

void configScene();
void renderScene();
void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa  (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHeliceField(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funTime           (double &t0, double delay, bool r);

// Shaders
   Shaders shaders;

// Modelos
   Model plane;
   Model cone; // apartado 1 daniel munoz martinez

   Model wc;

// Viewport
   int w = 500;
   int h = 500;

// Animaciones
   float desZ = 0.0;
   float rotZ = 0.0;
   int numAspas = 5;   // apartado 2 daniel munoz martinez
   double t0 = glfwGetTime();   // apartado 3 daniel munoz martinez
   bool rotate = true; // apartado 4 daniel munoz martinez
   float rotY = 0.0; // apartado 5 daniel munoz martinez
   float height = 0.0f; // apartado 6 daniel munoz martinez

int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Sesion 5", NULL, NULL);
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
    glfwSetKeyCallback(window, funKey);

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
    glPolygonOffset(1.0,1.0);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    plane.initModel("resources/models/plane.obj");
    cone.initModel("resources/models/cone.obj");
    wc.initModel("resources/models/wc_toilet.obj");

}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

    funTime(t0,0.01,rotate);

 // Matriz P
    float fovy   = 60.0;
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    glm::vec3 eye   (0.0, 7.0, 10.0); // apartado 1 daniel munoz martinez
    glm::vec3 center(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);

 // Dibujamos la escena
    drawSuelo(P,V,I);

    glm::mat4 R = glm::rotate   (I, glm::radians(170.0f), glm::vec3(0,1,0));     // apartado 5 daniel munoz martinez
    glm::mat4 Ty = glm::translate(I, glm::vec3(0.0f, height, 0.0f)); // apartado 6 daniel munoz martinez
    //drawHeliceField(P,V,Ty*R*I); // apartado 5 y 6 daniel munoz martinez
    
    drawObject(wc,glm::vec3(1.0,1.0,1.0), I,R, I);
                 
}

void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM",P*V*M);

    glEnable(GL_POLYGON_OFFSET_FILL);
        shaders.setVec3("uColor",color);
        model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    shaders.setVec3("uColor",color * 0.75f);
    model.renderModel(GL_LINE);

}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(5.0, 1.0, 5.0));
    drawObject(plane, glm::vec3(0.0, 0.0, 1.0), P, V, M*S);

}

void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(0.25f, 1.0f, 0.05f));// apartado 1 daniel munoz martinez
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -1.0, 0.0));
    drawObject(cone, glm::vec3(1.0, 0.0, 0.0), P, V, M*T*S); // apartado 1 daniel munoz martinez

}

void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) { 

    float ang = 360.0f / numAspas; // apartado 2 daniel munoz martinez                                      
    glm::mat4 R = glm::rotate(I, glm::radians(ang), glm::vec3(0,0,1));   

    glm::mat4 Mk = M;                                                          // apartado 2 daniel munoz martinez
    for(int i=0; i<numAspas; i++) {                                            // apartado 2 daniel munoz martinez
        drawAspa(P, V, Mk);                                                    // apartado 2 daniel munoz martinez
        Mk = Mk * R;                                                           // apartado 2 daniel munoz martinez
    }               
}

void drawHeliceField(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 T1 = glm::translate(I, glm::vec3( 2.5f, 0.0f,  2.5f));            // apartado 5 daniel munoz martinez
    glm::mat4 R1 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));     // apartado 5 daniel munoz martinez
    drawHelice(P, V, T1 * R1 * M);                                                  // apartado 5 daniel munoz martinez


    glm::mat4 T2 = glm::translate(I, glm::vec3( 2.5f, 0.0f, -2.5f));            // apartado 5 daniel munoz martinez
    glm::mat4 R2 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));     // apartado 5 daniel munoz martinez
    drawHelice(P, V, T2 * R2 * M);                                                  // apartado 5 daniel munoz martinez

    glm::mat4 T3 = glm::translate(I, glm::vec3(-2.5f, 0.0f,  2.5f));            // apartado 5 daniel munoz martinez
    glm::mat4 R3 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));     // apartado 5 daniel munoz martinez
    drawHelice(P, V, T3 * R3 * M);                                                  // apartado 5 daniel munoz martinez

    glm::mat4 T4 = glm::translate(I, glm::vec3(-2.5f, 0.0f, -2.5f));            // apartado 5 daniel munoz martinez
    glm::mat4 R4 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));     // apartado 5 daniel munoz martinez
    drawHelice(P, V, T4 * R4 * M); 
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}
void funTime(double &t0, double delay, bool r){
	double t1 = glfwGetTime();
	if (r && t1-t0 >= delay){
		rotY += 3.6;
		t0 = t1;
   }
}
void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        case GLFW_KEY_LEFT:  rotZ += 5.0; break;
        case GLFW_KEY_RIGHT: rotZ -= 5.0; break;
        case GLFW_KEY_1: if(action==GLFW_PRESS) numAspas = 1; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_2: if(action==GLFW_PRESS) numAspas = 2; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_3: if(action==GLFW_PRESS) numAspas = 3; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_4: if(action==GLFW_PRESS) numAspas = 4; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_5: if(action==GLFW_PRESS) numAspas = 5; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_6: if(action==GLFW_PRESS) numAspas = 6; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_7: if(action==GLFW_PRESS) numAspas = 7; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_8: if(action==GLFW_PRESS) numAspas = 8; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_9: if(action==GLFW_PRESS) numAspas = 9; break;   // apartado 2 daniel munoz martinez
        case GLFW_KEY_R: if(action==GLFW_PRESS) rotate = !rotate; break;   // apartado 4 daniel munoz martinez
        case GLFW_KEY_UP:
         if(action == GLFW_PRESS &&  action != GLFW_REPEAT) //pasos
            height += 0.1f;          // apartado 6 daniel munoz martinez
         break;

        case GLFW_KEY_DOWN:
         if(action == GLFW_PRESS && action != GLFW_REPEAT) //pasos
            height -= 0.1f;          // apartado 6 daniel munoz martinez
         break;
        default:
            desZ = 0.0;
            rotZ = 0.0;
    }

}
