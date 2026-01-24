#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"
#include "../SeatsManager.cpp"
#include "../Canvas.cpp"
#include "../DarkRect.cpp"
#include "../Door.cpp"
#include "../Watermark.cpp"
#include "../PersonManager.cpp"
#include "../WallsManager.h"
#include "../FloorManager.h"
#include "../Camera.h"
#include "../model.hpp"
//#include "../Camera.cpp"
// Main fajl funkcija sa osnovnim komponentama OpenGL programa

// Projekat je dozvoljeno pisati počevši od ovog kostura
// Toplo se preporučuje razdvajanje koda po fajlovima (i eventualno potfolderima) !!!
// Srećan rad!

int screenWidth = 800;
int screenHeight = 800;

float uX = 0.0f;
float uY = 0.0f;

bool hasMovieStarted = false;

Camera camera;
SeatsManager seatsManager;
Canvas canvas;
DarkRect darkRect;
Door door;
Watermark watermark;
PersonManager personManager;

unsigned watermarkTexture;

bool canStartMovie = true;
bool canLetPeopleIn = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        if (glIsEnabled(GL_DEPTH_TEST)){ glDisable(GL_DEPTH_TEST); }
        else {
            glEnable(GL_DEPTH_TEST);
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        if (glIsEnabled(GL_CULL_FACE)) { glDisable(GL_CULL_FACE); }
        else {
            glEnable(GL_CULL_FACE);
        }
    }

    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
        int numSeats = key - GLFW_KEY_0;
        seatsManager.buySeats(numSeats);
    }

  /*  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        if (!canvas.hasMovieStarted && canLetPeopleIn && !seatsManager.fillUsedSeats().empty()) {
            darkRect.changeTransparency();
            seatsManager.canManipulateSeats = !seatsManager.canManipulateSeats;
            door.openDoor();
            std::vector<Seat> usedSeats = seatsManager.fillUsedSeats();
            personManager.arrangePeople(usedSeats);
            personManager.isTimeToSpawnPeople = true;
            canLetPeopleIn = false;
        }
    }*/
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (camera.firstMouse) {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos; // Obrnuto!

    camera.lastX = xpos;
    camera.lastY = ypos;

    // Sva matematika se dešava unutar klase
    camera.processMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        seatsManager.reserve(camera); // pozovi metodu SeatsManager-a
    }
}

void checkForSceneReset() {
    if (personManager.allPeopleLeft){
        personManager.allPeopleLeft = false;
        hasMovieStarted = false;
        darkRect.changeTransparency();
        seatsManager.canManipulateSeats = !seatsManager.canManipulateSeats;
        door.closeDoor();
        personManager.isTimeToSpawnPeople = false;
        seatsManager.resetSeats();
        personManager.resetManager();
        canvas.resetCanvas();
        canStartMovie = true;
        canLetPeopleIn = true;

    }
}

void checkForMovieFinish() {
    if (canvas.isMovieFinished) {
        personManager.isMovieFinished = true;
        door.openDoor();
    }

    if (personManager.allPeopleLeft) {
        canLetPeopleIn = true;
    }


}

void checkForMovieStart() {
    if (personManager.allPeopleSat && canStartMovie) {
        canvas.startMovie();
        door.closeDoor();
        canStartMovie = false;
    }
}
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    screenWidth = mode->width;
    screenHeight = mode->height;

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Bioskop", monitor, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);
  
    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.5, 0.5, 0.5, 1.0);
   
    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    glUseProgram(unifiedShader);
    glUniform1i(glGetUniformLocation(unifiedShader, "uTex"), 0);

    unsigned int textureShader = createShader("basic.vert", "texture.frag");

    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");
    unsigned int projLoc = glGetUniformLocation(unifiedShader, "uP");
    unsigned int uViewPos = glGetUniformLocation(unifiedShader, "uViewPos");

    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)
     
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

    camera = Camera(glm::vec3(-0.25f, 0.5f, 0.5f), unifiedShader);
    seatsManager = SeatsManager(5, 10, unifiedShader); // koristimo isti shader za sedišta
    WallsManager wallsManager = WallsManager(unifiedShader);
    FloorManager floorManager = FloorManager(unifiedShader);

    camera.addWorldSpaceObject(&floorManager);
    camera.addWorldSpaceObject(&wallsManager);
    canvas = Canvas(unifiedShader, 1.5f, 1.25f);
    /*darkRect = DarkRect(rectShader);*/
    door = Door(unifiedShader, 0.25f, 0.5f);
    watermark = Watermark(textureShader);
 /*   Model lija("res/test/characterlowpoly2.obj")*/
   // personManager = PersonManager(textureShader);


    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    unsigned int lightPosLoc = glGetUniformLocation(unifiedShader, "uLight.pos");
    unsigned int lightALoc = glGetUniformLocation(unifiedShader, "uLight.kA");
    unsigned int lightDLoc = glGetUniformLocation(unifiedShader, "uLight.kD");
    unsigned int lightSLoc = glGetUniformLocation(unifiedShader, "uLight.kS");

    unsigned int materialShineLoc = glGetUniformLocation(unifiedShader, "uMaterial.shine");
    unsigned int materialALoc = glGetUniformLocation(unifiedShader, "uMaterial.kA");
    unsigned int materialDLoc = glGetUniformLocation(unifiedShader, "uMaterial.kD");
    unsigned int materialSLoc = glGetUniformLocation(unifiedShader, "uMaterial.kS");

    glUniform3f(lightPosLoc, -0.5f, 0.75f, 0.8f);
    glUniform3f(lightALoc, 0.4, 0.4, 0.4);
    glUniform3f(lightDLoc, 0.75f, 0.75f, 0.75f);
    glUniform3f(lightSLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(uViewPos, camera.position.x, camera.position.y, camera.position.z);

 
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        double initFrameTime = glfwGetTime();
        glUseProgram(unifiedShader);
      
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.updateShader(projectionP);
        camera.proccessKeyInputs(window);

        glUniform3f(glGetUniformLocation(unifiedShader, "uViewPos"),camera.position.x, camera.position.y, camera.position.z);

        door.draw();
        seatsManager.draw(camera, projectionP); // Iscrtavanje sedista
        wallsManager.draw();
        floorManager.draw();
        canvas.draw();
        //darkRect.draw();
        watermark.draw();
        //personManager.draw();

       /* checkForMovieStart();
        checkForMovieFinish();
        checkForSceneReset();*/


               
        glfwSwapBuffers(window);
        glfwPollEvents();

        while (glfwGetTime() - initFrameTime < 1 / 75.0) {} //Frame-Limiter
    }

    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}