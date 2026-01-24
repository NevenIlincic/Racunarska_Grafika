#pragma once

class Camera; // Forward deklaracija: "Hej kompajleru, klasa Camera postoji, videćeš je kasnije"

class WorldSpaceObject {
public:
    virtual ~WorldSpaceObject() {}
    virtual void checkCameraCollision(Camera& camera) = 0;
};