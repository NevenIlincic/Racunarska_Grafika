#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"
#include "../SeatsManager.cpp"
#include "../Canvas.cpp"
#include "../DarkRect.cpp"
#include "../Door.cpp"
#include "../Watermark.cpp"
#include "../PersonManager.cpp"
// Main fajl funkcija sa osnovnim komponentama OpenGL programa

// Projekat je dozvoljeno pisati počevši od ovog kostura
// Toplo se preporučuje razdvajanje koda po fajlovima (i eventualno potfolderima) !!!
// Srećan rad!

int screenWidth = 800;
int screenHeight = 800;

float uX = 0.0f;
float uY = 0.0f;

bool hasMovieStarted = false;

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

    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
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
    }
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

    glClearColor(0.2f, 0.8f, 0.6f, 1.0f);



    unsigned int rectShader = createShader("rect.vert", "rect.frag");
    glUseProgram(rectShader);
    glUniform1i(glGetUniformLocation(rectShader, "uTex0"), 0);
    glUniform1i(glGetUniformLocation(rectShader, "uTex1"), 1);
    unsigned int textureShader = createShader("texture.vert", "texture.frag");

    seatsManager = SeatsManager(rectShader); // koristimo isti shader za sedišta
    canvas = Canvas(rectShader);
    darkRect = DarkRect(rectShader);
    door = Door(rectShader);
    watermark = Watermark(textureShader);
    personManager = PersonManager(textureShader);


    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    GLFWcursor* cursor = loadImageToCursor("Resources/movie_camera.png");
    glfwSetCursor(window, cursor);

    while (!glfwWindowShouldClose(window))
    {
        double initFrameTime = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT);

        door.draw();
        seatsManager.draw(); // Iscrtavanje sedista
        canvas.draw();
        darkRect.draw();
        watermark.draw();
        personManager.draw();

        checkForMovieStart();
        checkForMovieFinish();
        checkForSceneReset();
       

        glfwSwapBuffers(window);
        glfwPollEvents();

        while (glfwGetTime() - initFrameTime < 1 / 75.0) {} //Frame-Limiter
    }

    glDeleteProgram(rectShader);
    glDeleteProgram(textureShader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}