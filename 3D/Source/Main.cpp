#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"
#include "../SeatsManager.cpp"
#include "../Canvas.cpp"
#include "../DarkRect.cpp"
#include "../Door.cpp"
#include "../Watermark.cpp"
#include "../PersonManager.cpp"
#include "../Camera.cpp"
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
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glEnable(GL_DEPTH_TEST); //Ukljucivanje testiranja Z bafera
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glDisable(GL_DEPTH_TEST);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        glEnable(GL_CULL_FACE);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        glDisable(GL_CULL_FACE);
    }

   /* if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
        int numSeats = key - GLFW_KEY_0;
        seatsManager.buySeats(numSeats);
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
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
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Pretvori u OpenGL koordinatni sistem [-1,1]
        float normX = (mouseX / screenWidth) * 2.0f - 1.0f;
        float normY = 1.0f - (mouseY / screenHeight) * 2.0f;

        seatsManager.reserve(normX, normY); // pozovi metodu SeatsManager-a
    }
}

void checkForSceneReset() {
    if (personManager.allPeopleLeft) {
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
    float vertices[] =
    {
        //X    Y    Z      R    G    B    A         S   T
          // Prednja strana (1)
          0.1, 0.1, 0.1,   1.0, 0.0, 0.0, 1.0,      0,  0,    0, 0, 1,
         -0.1, 0.1, 0.1,   1.0, 0.0, 0.0, 1.0,      1,  0,    0, 0, 1,
         -0.1,-0.1, 0.1,   1.0, 0.0, 0.0, 1.0,      1,  1,    0, 0, 1,
          0.1,-0.1, 0.1,   1.0, 0.0, 0.0, 1.0,      0,  1,    0, 0, 1,

          // Leva strana (2)
         -0.1, 0.1, 0.1,   0.0, 0.0, 1.0, 1.0,      0,  0,    -1, 0, 0,
         -0.1, 0.1,-0.1,   0.0, 0.0, 1.0, 1.0,      1,  0,    -1, 0, 0,
         -0.1,-0.1,-0.1,   0.0, 0.0, 1.0, 1.0,      1,  1,    -1, 0, 0,
         -0.1,-0.1, 0.1,   0.0, 0.0, 1.0, 1.0,      0,  1,    -1, 0, 0,

         // Donja strana (3)
         0.1,-0.1, 0.1,   1.0, 1.0, 1.0, 1.0,      0,  0,    0, -1, 0,
        -0.1,-0.1, 0.1,   1.0, 1.0, 1.0, 1.0,      1,  0,    0, -1, 0,
        -0.1,-0.1,-0.1,   1.0, 1.0, 1.0, 1.0,      1,  1,    0, -1, 0,
         0.1,-0.1,-0.1,   1.0, 1.0, 1.0, 1.0,      0,  1,    0, -1, 0,

         // Gornja strana (4)
         0.1, 0.1, 0.1,   1.0, 1.0, 0.0, 1.0,      0,  0,    0, 1, 0,
         0.1, 0.1,-0.1,   1.0, 1.0, 0.0, 1.0,      1,  0,    0, 1, 0,
        -0.1, 0.1,-0.1,   1.0, 1.0, 0.0, 1.0,      1,  1,    0, 1, 0,
        -0.1, 0.1, 0.1,   1.0, 1.0, 0.0, 1.0,      0,  1,    0, 1, 0,

        // Desna strana (5)
        0.1, 0.1, 0.1,   0.0, 1.0, 0.0, 1.0,      0,  0,    1, 0, 0,
        0.1,-0.1, 0.1,   0.0, 1.0, 0.0, 1.0,      1,  0,    1, 0, 0,
        0.1,-0.1,-0.1,   0.0, 1.0, 0.0, 1.0,      1,  1,    1, 0, 0,
        0.1, 0.1,-0.1,   0.0, 1.0, 0.0, 1.0,      0,  1,    1, 0, 0,

        // Zadnja strana (6)
        0.1, 0.1,-0.1,   1.0, 0.5, 0.0, 1.0,      0,  0,    0, 0, -1,
        0.1,-0.1,-0.1,   1.0, 0.5, 0.0, 1.0,      1,  0,    0, 0, -1,
       -0.1,-0.1,-0.1,   1.0, 0.5, 0.0, 1.0,      1,  1,    0, 0, -1,
       -0.1, 0.1,-0.1,   1.0, 0.5, 0.0, 1.0,      0,  1,    0, 0, -1,
    };
    unsigned int stride = (3 + 4 + 2 + 3) * sizeof(float);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    glUseProgram(unifiedShader);
    glUniform1i(glGetUniformLocation(unifiedShader, "uTex"), 0);

    unsigned int textureShader = createShader("basic.vert", "texture.frag");

    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");
    unsigned int projLoc = glGetUniformLocation(unifiedShader, "uP");

    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)
     
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
    camera = Camera(glm::vec3(0.0f, 0.0f, 2.0f), unifiedShader);
   // seatsManager = SeatsManager(rectShader); // koristimo isti shader za sedišta
 /*   canvas = Canvas(rectShader);
    darkRect = DarkRect(rectShader);
    door = Door(rectShader);*/
    watermark = Watermark(textureShader);
   // personManager = PersonManager(textureShader);


    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        double initFrameTime = glfwGetTime();
        glUseProgram(unifiedShader);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.updateShader(projectionP);
        camera.proccessKeyInputs(window);

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        // Tvoja kocka ima 6 strana, svaka je Triangle Fan od 4 temena
        for (int i = 0; i < 6; i++) {
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
        }
        glBindVertexArray(0);
       // door.draw();
       // seatsManager.draw(); // Iscrtavanje sedista
        //canvas.draw();
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