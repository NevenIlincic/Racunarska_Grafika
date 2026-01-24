#include "SeatsManager.h"



SeatsManager::SeatsManager() {};

SeatsManager::SeatsManager(int _rows, int _cols, Shader shader) : rows(_rows), cols(_cols), shaderProgram(shader) {
    // Primer: 5x10 sedišta
    float spacingX = 0.15f;
    float spacingY = 0.3f;
    seatSize = 0.6f;

    canManipulateSeats = true;

    takenSeats = 0;

    stepHeight = 0.1f;

    float spacingZ = 0.3f;
        

    float offsetZ = -0.06f;
    float offsetY = 0.06f;

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            float posX = j * spacingX - 1.0f;
            float posY = i * stepHeight;
            float posZ = -i * spacingZ;
            seats.push_back(Seat(posX, posY + offsetY, posZ, i, j));
        }
    }

    // Kreiranje jednog VAO/VBO za kvadrat
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

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(1);*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // 1: NORMALE (X, Y, Z) - offset je 9 float-ova (3 pos + 4 col + 2 tex)
    // Shader traži normale na lokaciji 1!
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 2: UV Koordinate (S, T) - offset je 7 float-ova (3 pos + 4 col)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Crtanje svih sedišta
void SeatsManager::draw() {
    /*glUseProgram(shaderProgram); *///
    this->shaderProgram.use();
    glBindVertexArray(VAO);

    // Uzimamo samo lokacije koje nam trebaju za materijal sedišta i model matricu
    /*unsigned int modelLoc = glGetUniformLocation(shaderProgram, "uM");
    unsigned int diffLoc = glGetUniformLocation(shaderProgram, "uMaterial.kD");
    unsigned int ambLoc = glGetUniformLocation(shaderProgram, "uMaterial.kA");
    unsigned int specLoc = glGetUniformLocation(shaderProgram, "uMaterial.kS");
    unsigned int shineLoc = glGetUniformLocation(shaderProgram, "uMaterial.shine");*/
   
    // Postavljamo zajednički sjaj za sva sedišta jednom
    /*glUniform3f(specLoc, 0.1f, 0.1f, 0.1f);*/
    this->shaderProgram.setVec3("uMaterial.kS", 0.1f, 0.1f, 0.1f);
    /* glUniform1f(shineLoc, 32.0f);*/
    this->shaderProgram.setFloat("uMaterial.shine", 32.0f);

    for (Seat& seat : seats) {
        // Boja konkretnog sedišta
        /*glUniform3f(diffLoc, seat.r, seat.g, seat.b);
        glUniform3f(ambLoc, seat.r, seat.g, seat.b);*/
        this->shaderProgram.setVec3("uMaterial.kD", seat.r, seat.g, seat.b);
        this->shaderProgram.setVec3("uMaterial.kA", seat.r, seat.g, seat.b);

        // Pozicija konkretnog sedišta
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(seat.x, seat.y, seat.z));
        model = glm::scale(model, glm::vec3(seatSize, seatSize, seatSize));
        /*glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/
        this->shaderProgram.setMat4("uM", model);

        for (int i = 0; i < 6; i++) {
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
        }
    }
    glBindVertexArray(0);
}

void SeatsManager::reserve(Camera& camera) {
    for (Seat& seat : this->seats) {
        if (this->isCameraLookingAt(camera, seat, 0.35f)) {
            seat.reserveSeat(); // Rezerviši samo ono što gledaš i što je blizu
            break;
        }
    }
}

bool SeatsManager::isCameraLookingAt(Camera& cam, Seat& seat, float maxDistance) {
    glm::vec3 seatPos = glm::vec3(seat.x, seat.y, seat.z);

    // 1. Izračunaj stvarnu udaljenost između kamere i sedišta
    float dist = glm::distance(cam.position, seatPos);

    if (dist > maxDistance) { //Ako je predaleko kamera, ne moze se rezervisati
        return false;
    }

    glm::vec3 toSeat = glm::normalize(seatPos - cam.position);
    float cosAngle = glm::dot(cam.front, toSeat);

    return cosAngle > 0.98f;
}

void SeatsManager::buySeats(int numSeats) {
    if (numSeats > seats.size() - takenSeats) {
        return;
    }
    bool canTakeSeats = false;
    int checkedFreeSeats = 0;
    int startingSeatIndex = seats.size() - 1;
    int numSeatsLeftToCheck = numSeats;

    if (this->canManipulateSeats) {
        for (int i = seats.size() - 1; i >= 0; i--) {
            if (i - (numSeats - 1) < 0) { break; }

            Seat currentSeat = seats[i];
            if (currentSeat.state != State::Free) { continue; }

            startingSeatIndex = i;

            Seat lastSeat = seats[i - numSeats + 1];
            if (currentSeat.row != lastSeat.row) {
                if (currentSeat.row - 1 < 0) { break; }
                i = (lastSeat.row * this->cols) + this->cols;
                continue;
            }
            for (int y = 0; y < numSeats; y++) {
                Seat checkingSeat = seats[i - y];
                if (checkingSeat.state != State::Free) { break; }
                if (y == numSeats - 1) {
                    canTakeSeats = true;
                }

            }
            if (canTakeSeats) { break; }
        }
    }
    if (canTakeSeats) {
        for (int z = 0; z < numSeats; z++) {
            Seat& currentSeat = seats[startingSeatIndex - z];
            currentSeat.buySeat();
            takenSeats++;
        }
    }
       
}
std::vector<Seat> SeatsManager::fillUsedSeats() {
    usedSeats.clear();
    for (size_t i = 0; i < seats.size(); i++) {
        Seat seat = seats[i];
        if (seat.state == State::Reserved || seat.state == State::Bought) {
            usedSeats.push_back(seat);
        }
    }

    return usedSeats;
}

void SeatsManager::resetSeats() {
    usedSeats.clear();
    takenSeats = 0;
    for (Seat& seat : seats) {
        seat.resetSeat();
    }
}
