#include "Watermark.h"


Watermark::Watermark() {}

Watermark::Watermark(Shader _shader): shaderProgram(_shader) {

    preprocessTexture(watermarkTexture, "Resources/watermark.png", false);

    float quadVertices[] = {
        //  X,     Y,     U,    V
        -1.0f, -0.75f,    0.0f, 0.0f, // 1. Donje levo 
        -0.5f, -0.75f,    1.0f, 0.0f, // 2. Donje desno
        -0.5f, -0.85f,    1.0f, 1.0f, // 3. Gornje desno
        -1.0f, -0.85f,    0.0f, 1.0f
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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}

void Watermark::draw() {
    this->shaderProgram.use();
    GLboolean wasDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean wasCullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    // 2. Isključi Cull Face tako da se vidi čak i ako je naopako
    glDisable(GL_CULL_FACE);
       
    // Koristi Identity matrice da ignorišeš kameru
    glm::mat4 identity = glm::mat4(1.0f);
    this->shaderProgram.setMat4("uM", identity);
    this->shaderProgram.setMat4("uV", identity);
    this->shaderProgram.setMat4("uP", identity);
   /* this->shaderProgram.setInt("uTex", watermarkTexture);*/
    this->shaderProgram.setFloat("uAlpha", 0.75f);

   

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, watermarkTexture);
    this->shaderProgram.setInt("uTex", 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    if (wasDepthTestEnabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (wasCullFaceEnabled) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

  
}
