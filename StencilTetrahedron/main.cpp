#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

// �������
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main(){
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec4 color;

    void main(){
        FragColor = color;
    }
)";

// ������� ��� ���������� �������
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    // �������� ������
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// ������� ��� �������� ��������� ���������
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // �������� ���������
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // �������� ������
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // �������� ��������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

// ���������� ��������� ������� ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ��������� ��� �������
struct Torus {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int numc, numt;
};

// ������� ��������� �������
Torus generateTorus(float innerRadius, float outerRadius, unsigned int numc, unsigned int numt) {
    Torus torus;
    torus.numc = numc;
    torus.numt = numt;

    for (unsigned int i = 0; i <= numc; ++i) {
        for (unsigned int j = 0; j <= numt; ++j) {
            float s = (float)i / numc * 2.0f * glm::pi<float>();
            float t = (float)j / numt * 2.0f * glm::pi<float>();

            float x = (outerRadius + innerRadius * cos(t)) * cos(s);
            float y = (outerRadius + innerRadius * cos(t)) * sin(s);
            float z = innerRadius * sin(t);

            torus.vertices.push_back(x);
            torus.vertices.push_back(y);
            torus.vertices.push_back(z);
        }
    }

    for (unsigned int i = 0; i < numc; ++i) {
        for (unsigned int j = 0; j < numt; ++j) {
            unsigned int first = i * (numt + 1) + j;
            unsigned int second = first + numt + 1;

            torus.indices.push_back(first);
            torus.indices.push_back(second);
            torus.indices.push_back(first + 1);

            torus.indices.push_back(second);
            torus.indices.push_back(second + 1);
            torus.indices.push_back(first + 1);
        }
    }

    return torus;
}

int main() {
    // ������������� GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // ��������� GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������� ����
    GLFWwindow* window = glfwCreateWindow(800, 600, "Stencil Tetrahedron", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // ��������� ����������� ��������� �������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ������������� GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ��������� ����� ������� � ������ ���������
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    // �������� ��������� ���������
    GLuint shaderProgram = createShaderProgram();

    // ������� ���������
    float tetrahedronVertices[] = {
        // �������
        1.0f,  1.0f,  1.0f,  // ������� A
       -1.0f, -1.0f,  1.0f,  // ������� B
       -1.0f,  1.0f, -1.0f,  // ������� C
        1.0f, -1.0f, -1.0f   // ������� D
    };

    // ������� ������ ���������
    unsigned int tetrahedronIndices[] = {
        0, 1, 2, // ����� ABC
        0, 3, 1, // ����� ABD
        0, 2, 3, // ����� ACD
        1, 3, 2  // ����� BCD
    };

    // �������� VAO � VBO ��� ���������
    GLuint tetraVAO, tetraVBO, tetraEBO;
    glGenVertexArrays(1, &tetraVAO);
    glGenBuffers(1, &tetraVBO);
    glGenBuffers(1, &tetraEBO);

    glBindVertexArray(tetraVAO);

    // �������
    glBindBuffer(GL_ARRAY_BUFFER, tetraVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronVertices), tetrahedronVertices, GL_STATIC_DRAW);

    // �������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetraEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedronIndices), tetrahedronIndices, GL_STATIC_DRAW);

    // �������� ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // ��������� �������
    Torus torus = generateTorus(0.3f, 0.8f, 30, 30);

    // �������� VAO � VBO ��� �������
    GLuint torusVAO, torusVBO, torusEBO;
    glGenVertexArrays(1, &torusVAO);
    glGenBuffers(1, &torusVBO);
    glGenBuffers(1, &torusEBO);

    glBindVertexArray(torusVAO);

    glBindBuffer(GL_ARRAY_BUFFER, torusVBO);
    glBufferData(GL_ARRAY_BUFFER, torus.vertices.size() * sizeof(float), &torus.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, torus.indices.size() * sizeof(unsigned int), &torus.indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // ������ ��������� ��� ��������� (������ ������)
    glm::vec3 holeCenters[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f)
    };

    // ������� ��� ��������� �����
    auto drawCircle = [&](glm::vec3 center) {
        int numSegments = 100;
        float radius = 0.2f;
        std::vector<float> vertices;
        for (int i = 0; i <= numSegments; ++i) {
            float angle = 2.0f * glm::pi<float>() * i / numSegments;
            vertices.push_back(center.x + radius * cos(angle));
            vertices.push_back(center.y + radius * sin(angle));
            vertices.push_back(center.z);
        }

        GLuint circleVAO, circleVBO;
        glGenVertexArrays(1, &circleVAO);
        glGenBuffers(1, &circleVBO);

        glBindVertexArray(circleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);

        glDeleteBuffers(1, &circleVBO);
        glDeleteVertexArrays(1, &circleVAO);
        };

    // ��������� ������ ���� � ��������
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        800.0f / 600.0f, 0.1f, 100.0f);

    // ��������� �������������� uniform-����������
    glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // ��������� ���������� ��� ����������������
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ���������� ��� ��������
    float angle = 0.0f;
    double previousTime = glfwGetTime();

    // �������� ����
    while (!glfwWindowShouldClose(window)) {
        // ����
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // ���������� ������� � ���� ��������
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        angle += 50.0f * deltaTime; // 50 �������� � �������
        if (angle > 360.0f)
            angle -= 360.0f;

        // �������� ������
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotation));

        // ������� �������
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // ��������� ��������� � ����� ���������
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // ������� ����
        glBindVertexArray(tetraVAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // ��������� ���������
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // ���� �� �����
        for (auto& center : holeCenters) {
            drawCircle(center);
        }
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        // ��������� ������� (�������������� �������)
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 0, 0xFF); // �� ������ �� ��������
        glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 0.5f); // �������������� �����
        glBindVertexArray(torusVAO);
        glDrawElements(GL_TRIANGLES, torus.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // ����� ������� � ��������� �������
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ������� ��������
    glDeleteVertexArrays(1, &tetraVAO);
    glDeleteBuffers(1, &tetraVBO);
    glDeleteBuffers(1, &tetraEBO);
    glDeleteVertexArrays(1, &torusVAO);
    glDeleteBuffers(1, &torusVBO);
    glDeleteBuffers(1, &torusEBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
