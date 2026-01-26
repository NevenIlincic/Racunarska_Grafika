#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Person.h"
#include "Header/Util.h"
#include "Seat.h"
#include "shader.hpp"
#include "FloorManager.h"

class PersonManager {
public:
    std::vector<Person> people;
    std::vector<Person> spawnedPeople;
    unsigned int VAO, VBO;     // zajednički VAO/VBO
    Shader shaderProgram;
    unsigned personTexture;
    unsigned sittingPersonTexture;

    float timerInterval;

    double startTime;

    int spawningIndex;
    int numPeopleToSpawn;

    bool isTimeToSpawnPeople;
    bool isMovieFinished;

    bool allPeopleLeft;
    bool allPeopleSat;


    PersonManager();


    PersonManager(Shader _shader);

    void draw(FloorManager& floorManager);
    void arrangePeople(std::vector<Seat> usedSeats);
    void spawnPerson();
    bool canSpawnPerson();
    void cleanupPeople();
    void resetManager();
    bool didAllPeopleSit();
};