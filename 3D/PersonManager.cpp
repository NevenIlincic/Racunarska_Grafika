#include "PersonManager.h"

PersonManager::PersonManager() {};

PersonManager::PersonManager(Shader shader) : shaderProgram(shader), isTimeToSpawnPeople(false) {
    timerInterval = 0.5f;
    startTime = 0.0f;
    spawningIndex = 0;
    numPeopleToSpawn = 0;
    isMovieFinished = false;
    allPeopleLeft = false;
    allPeopleSat = false;

    this->modelFixes[0] = { 0.25f, 270.0f, 0.0f, -0.15f}; //1. Scale, 2. Rotation, 3. y-rotacija, 4. z-rotacija, 5. yOffset
    this->modelFixes[1] = { 0.3f, 270.0f, 0.0f, -0.15f };
    this->modelFixes[2] = { 0.2f, 270, 0.0f, -0.15f };
    this->modelFixes[3] = { 0.005f, 180.0f, 0.0f, -0.15f };
    this->modelFixes[4] = { 0.15f, 180.0f, 0.0f, -0.15f };
    this->modelFixes[5] = { 0.15f, 180.0f, 0.0f, -0.15f };
    this->modelFixes[6] = { 0.15f, 180.0f, 0.0f, -0.15f };
    this->modelFixes[7] = { 0.1f, 155.0f, 0.0f, -0.15f };
    this->modelFixes[8] = { 0.1f, 120.0f, 0.0f, -0.15f };
    this->modelFixes[9] = { 0.2f, 180.0f, 90.0f, -0.15f };
    this->modelFixes[10] = { 0.5f, 180.0f, 0.0f, -0.15f };
    this->modelFixes[11] = { 0.0015f, 180.0f, 90.0f, -0.15f };
    this->modelFixes[12] = { 0.1f, 155.0f, 0.0f, -0.15f };
    this->modelFixes[13] = { 0.0015f, 180.0f, 0.0f, -0.15f }; //Person 14
    this->modelFixes[14] = { 0.25f, 270.0f, 0.0f, -0.15f };
};

void PersonManager::draw(FloorManager& floorManager) {
    if (isTimeToSpawnPeople) {
        if (canSpawnPerson()) {
            spawnPerson();
        }
        for (Person& person : spawnedPeople) {
            float yOffset = this->modelFixes[person.modelIndex][3];
            person.yOffset = yOffset;
            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, glm::vec3(person.x, person.y + yOffset, person.z));

            person.move();
            this->shaderProgram.use();
      
            this->shaderProgram.setBool("useTex", true);
          
            if (person.isSitting) {
                person.currentAngle = this->modelFixes[person.modelIndex][1] - 180.0f;
                person.z = person.destinationZ - 0.08f;
            }
            else if (person.isMovingHorizontaly) { // Krece se uz red
                person.z = person.destinationZ;
                if (this->isMovieFinished) {
                    person.currentAngle = this->modelFixes[person.modelIndex][1] - 90.0f;
                }
                else {
                    person.currentAngle = this->modelFixes[person.modelIndex][1] - 270.0f;
                }
            }
            else { // Krece se uz zid
                if (this->isMovieFinished) {
                    person.currentAngle = this->modelFixes[person.modelIndex][1] - 180.0f;
                }
                else {
                    person.currentAngle = this->modelFixes[person.modelIndex][1];

                }
            }

            float scale = this->modelFixes[person.modelIndex][0];
            modelMat = glm::rotate(modelMat, glm::radians(person.currentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMat = glm::rotate(modelMat, glm::radians(this->modelFixes[person.modelIndex][2]), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMat = glm::scale(modelMat, glm::vec3(scale, scale, scale));
            this->shaderProgram.setMat4("uM", modelMat);

            person.personModel.Draw(this->shaderProgram);
            this->shaderProgram.setBool("useTex", true);
            if (person.isSitting) {
                if (isMovieFinished) {
                    person.finishedWatching = true;
                    person.isSitting = false;
                }
            }
            else {
                floorManager.checkPersonCollision(person);
                cleanupPeople();
            }


        }
        this->allPeopleSat = didAllPeopleSit();
    }

}

void PersonManager::arrangePeople(std::vector<Seat> usedSeats) {
    int humanIndex = 0;
    for (Seat& seat : usedSeats) {
        Person person = Person(humanIndex, 0.75f, 0.15f, 0.99f, seat.x, seat.y, seat.z + 0.1f);
        this->people.push_back(person);
        humanIndex = (humanIndex + 1) % 15;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(this->people.begin(), this->people.end(), g);

    numPeopleToSpawn = generate_random_number(1, usedSeats.size());

}

void PersonManager::spawnPerson() {
    if (spawningIndex < numPeopleToSpawn) {
        spawnedPeople.push_back(this->people[spawningIndex]);
        spawningIndex++;
    }
}

bool PersonManager::canSpawnPerson() {
    if (glfwGetTime() - this->startTime > this->timerInterval) {
        this->startTime = glfwGetTime();
        return true;
    }
    return false;
}

void PersonManager::cleanupPeople() {
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

void PersonManager::resetManager() {
    timerInterval = 0.5f;
    startTime = 0.0f;
    spawningIndex = 0;
    numPeopleToSpawn = 0;
    isMovieFinished = false;
    allPeopleLeft = false;
    people.clear();
    spawnedPeople.clear();
    allPeopleSat = false;
}

bool PersonManager::didAllPeopleSit() {
    if (this->numPeopleToSpawn == 0) {
        return false;
    }
    if (this->numPeopleToSpawn != spawnedPeople.size()) {
        return false;
    }
    for (const Person& person : spawnedPeople) {
        if (!person.isSitting) {
            return false;
        }
    }
    return true;
}
