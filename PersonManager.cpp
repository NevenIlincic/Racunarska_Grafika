#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Person.cpp"
#include "Header/Util.h"

class PersonManager {
public:
	std::vector<Person> people;
    std::vector<Person> spawnedPeople;
	unsigned int VAO, VBO;     // zajednički VAO/VBO
	unsigned int shaderTextureProgram;
    unsigned personTexture;
    unsigned sittingPersonTexture;

    float timerInterval;

    double startTime;

    int spawningIndex;
    int numPeopleToSpawn;

    bool isTimeToSpawnPeople;
    bool isMovieFinished;

    bool allPeopleLeft;
    

	PersonManager() {};


	PersonManager(unsigned int shader): shaderTextureProgram(shader), isTimeToSpawnPeople(false) {
        preprocessTexture(personTexture, "Resources/stickman.png", true);
        preprocessTexture(sittingPersonTexture, "Resources/sitting_stickman.png", true);

        timerInterval = 0.5f;
        startTime = 0.0f;
        spawningIndex = 0;
        numPeopleToSpawn = 0;
        isMovieFinished = false;
        allPeopleLeft = false;

        float quadVertices[] = {
            //  X (Lokalni), Y (Lokalni), U,     V
                 0.0f,       0.0f,    0.0f, 1.0f, // Gornje levo (GL): (0.0, 0.0) -> Pocinje na (uX, uY)
                 0.0f,      -0.2f,    0.0f, 0.0f, // Donje levo (DL): (0.0, -0.2)
                 0.1f,      -0.2f,    1.0f, 0.0f, // Donje desno (DR): (0.1, -0.2)
                 0.1f,       0.0f,    1.0f, 1.0f  // Gornje desno (GR): (0.1, 0.0)
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // pozicija
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // tekstura
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    };

    void draw() {
        if (isTimeToSpawnPeople) {
            if (canSpawnPerson()) {
                spawnPerson();
            }
            for (Person& person : spawnedPeople) {
                person.move();
                glUseProgram(shaderTextureProgram);
                glActiveTexture(GL_TEXTURE1);
                glUniform1i(glGetUniformLocation(shaderTextureProgram, "uTex"), 1);
                glUniform1f(glGetUniformLocation(shaderTextureProgram, "uAlpha"), 1.0f);
             
                if (person.isSitting) {
                    glBindTexture(GL_TEXTURE_2D, sittingPersonTexture);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uSx"), 0.4f);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uSy"), 0.4f);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uX"), person.x + 0.03f);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uY"), person.y - 0.26f);

                    if (isMovieFinished) {
                        person.finishedWatching = true;
                        person.isSitting = false;
                    }
                }
                else {
                    glBindTexture(GL_TEXTURE_2D, personTexture);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uSx"), 1.0f);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uSy"), 1.0f);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uX"), person.x);
                    glUniform1f(glGetUniformLocation(shaderTextureProgram, "uY"), person.y);
                    cleanupPeople();
                }

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glBindVertexArray(0);

            }
        }
    }

    void arrangePeople(std::vector<Seat> usedSeats) {
        for (Seat& seat : usedSeats) {
            Person person = Person(-1.0f, 1.0f, seat.x - 0.05f, seat.y);
            people.push_back(person);
        }

        numPeopleToSpawn = generate_random_number(1, usedSeats.size());

    }

    void spawnPerson() {
        if (spawningIndex < numPeopleToSpawn) {
            spawnedPeople.push_back(people[spawningIndex]);
            spawningIndex++;
        }
    }

    bool canSpawnPerson() {
        if (glfwGetTime() - this->startTime > this->timerInterval) {
            this->startTime = glfwGetTime();
            return true;
        }
        return false;
    }

    void cleanupPeople() {
        spawnedPeople.erase(
            std::remove_if(spawnedPeople.begin(), spawnedPeople.end(), [](const Person& person) {
                return person.hasExited;
                }),
            spawnedPeople.end()
        );

        if (spawnedPeople.empty()) {
            allPeopleLeft = true;
        }
    }

    void resetManager() {
        timerInterval = 0.5f;
        startTime = 0.0f;
        spawningIndex = 0;
        numPeopleToSpawn = 0;
        isMovieFinished = false;
        allPeopleLeft = false;
        people.clear();
        spawnedPeople.clear();
    }
};