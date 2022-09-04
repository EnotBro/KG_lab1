#include <stdio.h>
#include "Shaders.h"
#include <soil.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::mat4;
using glm::vec3;
using glm::rotate;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Компилирование нашей шейдерной программы
    Shader ourShader("vertexShader.txt", "fragmentShader.txt");  // путь к файлам шейдеров

    float vertices[] = {
        // координаты       // цвета
        -0.1f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,    0.2f, 0.0f,  //0
        -0.3f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.1f,  //1
        -0.3f,  0.1f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.3f,  //2
        -0.1f,  0.2f, 0.0f,   1.0f, 1.0f, 0.0f,    0.2f, 0.4f,  //3
         0.1f,  0.2f, 0.0f,   1.0f, 0.0f, 0.0f,    0.8f, 0.4f,  //4
         0.3f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,    1.0f, 0.3f,  //5
         0.3f, -0.1f, 0.0f,   1.0f, 0.0f, 0.0f,    1.0f, 0.1f,  //6
         0.1f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,    0.8f, 0.0f,  //7
        -0.25f, 0.1f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  //8
        -0.15f, 0.1f, 0.0f,   1.0f, 0.0f, 0.0f,    0.1f, 0.0f,  //9
        -0.25f, 0.4f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.3f,  //10
        -0.15f, 0.4f, 0.0f,   1.0f, 0.0f, 0.0f,    0.1f, 0.3f,  //11
        -0.15f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    0.1f, 0.4f,  //12
         0.15f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    0.4f, 0.4f,  //13
         0.15f, 0.4f, 0.0f,   1.0f, 0.0f, 0.0f,    0.4f, 0.3f,  //14
         0.25f, 0.4f, 0.0f,   1.0f, 0.0f, 0.0f,    0.5f, 0.3f,  //15
         0.15f, 0.1f, 0.0f,   1.0f, 0.0f, 0.0f,    0.4f, 0.0f,  //16
         0.25f, 0.1f, 0.0f,   1.0f, 0.0f, 0.0f,    0.5f, 0.0f   //17
    };

    unsigned int indices[] = {
        8,9,10,
        10,9,11,
        10,11,12,
        11,12,13,
        13,11,14,
        14,13,15,
        15,14,16,
        16,15,17,
        0,1,2,
        2,0,3,
        4,5,6,
        6,4,7
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Координатные артибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Цветовые атрибуты
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Текстурные атрибуты
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Установка параметров наложения текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Установка параметров фильтрации текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Загрузка изображения, создание текстуры и генерирование мипмап-уровней
    int width, height, nrChannels;
    unsigned char* data = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }


    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Обработка ввода
        processInput(window);

        // Рендеринг        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);

        //model = glm::translate(model, glm::vec3(0.3f, -0.3f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));

        //model = glm::rotate(model, 30.0f, vec3(0.0f, 0.0f, 1.0f));

        GLuint location = glGetUniformLocation(ourShader.ID, "model");
        if (location >= 0)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            //glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);
        }

        // Рендеринг квадрата
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6*sizeof(indices), GL_UNSIGNED_INT, 0);

        // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода\вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, высота и ширина будут значительно больше, чем указано, на Retina-дисплеях    
    glViewport(0, 0, width, height);
}