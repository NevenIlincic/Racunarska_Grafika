#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Walls {
public:
    unsigned int VAO, VBO;
    unsigned int shaderProgram;

    Walls(){}

    Walls(unsigned int shader) : shaderProgram(shader) {
        // Definišemo kocke koje su razvučene da izgledaju kao zidovi
        // Donja leva (-2, 0, -5) do gornja desna (2, 4, 2) - prilagodi dimenzije sali
        float vertices[] = {
            // Pozicije          // Normale (bitno za svetlo kasnije)
            // Pod (Y = 0)
            -1.2f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // Prednje levo
             1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // Prednje desno
             1.0f, 0.0f, -2.0f,  0.0f, 1.0f, 0.0f, // Zadnje desno
            -1.2f, 0.0f, -2.0f,  0.0f, 1.0f, 0.0f, // Zadnje levo

            // LEVI ZID (X = -5, normala gleda u +X)
            -1.2f, 3.0f, -2.5f,  1.0f, 0.0f, 0.0f, // Gore pozadi
            -1.2f, 3.0f,  1.0f,  1.0f, 0.0f, 0.0f, // Gore napred
            -1.2f, 0.0f,  1.0f,  1.0f, 0.0f, 0.0f, // Dole napred
            -1.2f, 0.0f, -2.5f,  1.0f, 0.0f, 0.0f, // Dole pozadi

            // DESNI ZID (X = 5, normala gleda u -X)
             1.0f, 3.0f,  1.0f, -1.0f, 0.0f, 0.0f, // Gore napred
             1.0f, 3.0f, -2.5f, -1.0f, 0.0f, 0.0f, // Gore pozadi
             1.0f, 0.0f, -2.5f, -1.0f, 0.0f, 0.0f, // Dole pozadi
             1.0f, 0.0f,  1.0f, -1.0f, 0.0f, 0.0f, // Dole napred

             // ZADNJI ZID (Z = 5, normala gleda u -Z - zid iza leđa sedišta)
             1.0f, 3.0f,  -2.5f,  0.0f, 0.0f, -1.0f, //Gore desno pozadi
             -1.2f, 3.0f, -2.5f,  0.0f, 0.0f, -1.0f, //Gore levo pozadi
             -1.2f, 0.0f, -2.5f,  0.0f, 0.0f, -1.0f, //Dole levo pozadi
             1.0f, 0.0f, -2.5f,  0.0f, 0.0f, -1.0f, //Dole desno pozadi

             // PREDNJI ZID
             -1.2f, 3.0f, 1.0f,  0.0f, 0.0f, 1.0f, //Gore napred desno
              1.0f, 3.0f, 1.0f,  0.0f, 0.0f, 1.0f, //Gore napred levo
              1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, //Dole napred levo
             -1.2f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f  //Dole napred desno
                };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Pozicije
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normale (ako tvoj shader koristi location 3 za normale kao u SeatsManager-u)
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(3);

    }

    void draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "uM");
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "uColor");

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Boja za pod (tamno siva)
        glUniform3f(colorLoc, 0.2f, 0.2f, 0.2f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // Boja za zidove (nešto svetlija)
        glUniform3f(colorLoc, 0.4f, 0.4f, 0.4f),
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Pod
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4); // Levi zid
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4); // Desni zid
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4); // Zadnji zid
        glDrawArrays(GL_TRIANGLE_FAN, 16, 4); // Prednji zid

        glBindVertexArray(0);
    }
};