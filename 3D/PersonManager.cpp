#include "PersonManager.h"

PersonManager::PersonManager() {};

PersonManager::PersonManager(Shader shader): shaderProgram(shader), isTimeToSpawnPeople(false) {
    timerInterval = 0.5f;
    startTime = 0.0f;
    spawningIndex = 0;
    numPeopleToSpawn = 0;
    isMovieFinished = false;
    allPeopleLeft = false;
    allPeopleSat = false;
};

void PersonManager::draw(FloorManager& floorManager) {
    if (isTimeToSpawnPeople) {
        if (canSpawnPerson()) {
            spawnPerson();
        }
        for (Person& person : spawnedPeople) {
            person.move();
            this->shaderProgram.use();
            this->shaderProgram.setBool("useTex", false);

            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, glm::vec3(person.x, person.y, person.z));
            
            
            if (person.isSitting) {
                person.currentAngle = 0.0f;
            }
            else if (person.isMovingHorizontaly) {
                person.currentAngle = -90.0f;
            }
            else {
                person.currentAngle = 180.0f;
            }

            float scale = 0.05f;
            modelMat = glm::rotate(modelMat, glm::radians(person.currentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMat = glm::scale(modelMat, glm::vec3(scale, scale, scale));
            this->shaderProgram.setMat4("uM", modelMat);

            person.personModel.Draw(this->shaderProgram);
             
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
    for (Seat& seat : usedSeats) {
        Person person = Person(0.75f, 0.15f, 0.99f, seat.x, seat.y, seat.z + 0.1f);
        this->people.push_back(person);
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
