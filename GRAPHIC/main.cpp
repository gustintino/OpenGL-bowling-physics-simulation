#include <iostream>
#include <vector>
#include <fstream>

#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Cube.h"
#include "GameObject.h"
#include "ObjectFromFile.h"
#include "Player.h"
#include "ForceGenerator.h"
#include "RotatingSquare.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "AABBCollider.h"

//TODO:
// - programID static variable

//GLuint vao;
//GLuint vbo;
//GLuint vao2;
//GLuint vbo2;

glm::vec3 ambient;
glm::vec3 diffuseIntensity;
glm::vec3 lightPosition;
glm::vec3 specularIntensity;
glm::vec3 specularColor;
float shinines;

GLuint modelLocation;
GLuint viewLocation;
GLuint projLocation;

double oldTime;
double currentTime;

glm::vec4 red(1, 0, 0, 1);
glm::vec4 green(0, 1, 0, 1);
glm::vec4 blue(0, 0, 1, 1);

//struct CollisionInformation
//{
//    GameObject* object1;
//    GameObject* object2;
//    glm::vec3 collisionNormal;
//    glm::vec3 penetrationDepth;
//};
std::vector<CollisionInformation> collisions;

void KeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (action == GLFW_PRESS)
    {
        GameObject::keyMap[key] = true;
        //if(key != 256)
            //std::cout << glfwGetKeyName(key, scancode) << " pressed." << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        GameObject::keyMap[key] = false;
        //std::cout << glfwGetKeyName(key, scancode) << " de-pressed." << std::endl;
    }
}

void Resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

class Shader
{
private:
    GLuint shaderID;
    GLuint programID;

public:

    Shader(const char* vertexPath, const char* fragmentPath)
    {
        programID = glCreateProgram();

        MakeShader(GL_VERTEX_SHADER, vertexPath);
        MakeShader(GL_FRAGMENT_SHADER, fragmentPath);

        glLinkProgram(programID);
        glUseProgram(programID);
    }

    GLuint GetProgramID()
    {
        return programID;
    }

    void MakeShader(GLenum shaderType, const char* shaderPath)
    {
        char* shaderCode = ReadTextFile(shaderPath);
        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shaderCode, NULL);
        glCompileShader(shaderID);
        CompileChecker(shaderID);
        glAttachShader(programID, shaderID);
    }

    GLuint ID()
    {
        return shaderID;
    }

    void CompileChecker(GLuint shader) // TODO: throw catch thing idk how it works
    {
        GLint result = GL_FALSE;
        int logLength;

        try
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
            glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
            std::cout << &vertShaderError[0] << std::endl;

            if (result == GL_FALSE)
            {
                throw - 1;
            }
        }
        catch (int e)
        {
            std::cout << "Shader acting up." << e << std::endl;
        }
    }

    char* ReadTextFile(const char* textFilepath)
    {
        FILE* filePointer;
        fopen_s(&filePointer, textFilepath, "rb");
        char* content = NULL;
        long numVal = 0;

        fseek(filePointer, 0L, SEEK_END);
        numVal = ftell(filePointer);
        fseek(filePointer, 0L, SEEK_SET);
        content = (char*)malloc((numVal + 1) * sizeof(char));
        fread(content, 1, numVal, filePointer);
        content[numVal] = '\0';
        fclose(filePointer);
        return content;
    }
};

class Camera
{
public:
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUpVector;
    glm::vec3 forward;
    glm::vec3 side;
    glm::vec3 newUp;

    
    Camera()
    {
        cameraPosition = glm::vec3(0, 0, -2);
        cameraTarget = glm::vec3(0, 0, 0);
        cameraUpVector = glm::vec3(0, 1, 0);

        forward = glm::normalize(cameraPosition - cameraTarget);
        side = glm::normalize(glm::cross(cameraUpVector, forward));
        newUp = glm::normalize(glm::cross(forward, side));


        //projection matrix
        //glm::mat4 projMatrix = glm::ortho(-0.1f, 0.1f, -0.1f, 0.1f, 0.01f, 10.0f);
        //glm::mat4 projMatrix = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.01f, 10.0f);
        glm::mat4 projMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 10.0f);

        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));
    }

    ~Camera()
    {
    }

    void UpdateCamera()
    {
        //view matrix
        glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, newUp);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    }

    void Move()
    {

    }
};


int main(int argc, char** argv)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(800, 800, "thing", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, Resize);

    if (!window)
    {
        std::cout << "ERROR: The window has not been created!" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (gl3wInit())
    {
        std::cout << "ERROR: Gl3w has not been initialized!" << std::endl;
        return -1;
    }

    glfwSwapInterval(1);

    glfwSetKeyCallback(window, KeyboardInput);

    Shader shader("vert.glsl", "frag.glsl");

    
    std::vector<GameObject*> objects;
    
    //object creation-----------------------------------------------------------------------
    Mesh* cube = new Cube();
    Mesh* sphereObjBlue = new ObjectFromFile("objs/sphere.obj", blue, glm::vec3(1, 1, 1));
    Mesh* sphereObjRed = new ObjectFromFile("objs/sphere.obj", red, glm::vec3(1,1,1));
    Mesh* cubeObjGreen = new ObjectFromFile("objs/cube.obj", green, glm::vec3(1, 1, 1));
    Mesh* sphereWireframe = new ObjectFromFile("objs/sphere.obj", green, glm::vec3(1, 1, 1));
    Mesh* planeMesh = new ObjectFromFile("objs/plane.obj", glm::vec4(1, 1, 1, 1), glm::vec3(10, 1, 2));
    Mesh* bowlingPinMesh = new ObjectFromFile("objs/bowling_pin.obj", glm::vec4(1, 1, 1, 1), glm::vec3(1, 1, 1));
    Mesh* bowlingPinWireframe = new ObjectFromFile("objs/cube.obj", glm::vec4(0, 1, 0, 1), glm::vec3(0.4f, 1.1f, 0.4f));

    Collider* sphereCollider = new SphereCollider(1.0f, sphereWireframe);
    Collider* bowlingPinCollider = new AABBCollider(bowlingPinWireframe);

    //GameObject* cubeObject1 = new GameObject(sphereObjRed, 3.0, glm::vec3(-1.0f, 0.0f, 0.0f), new SphereCollider(1.0f, sphereWireframe));
    GameObject* player = new Player(sphereObjRed, 1.0f, glm::vec3(0.0f, 1.5f, 2.0f), 75.0f, sphereCollider);
    //GameObject* cubeObj = new GameObject(cubeObjGreen, 1.0f, glm::vec3(-1.0f, 0.0f, 0.0f), new SphereCollider(1.0f, sphereWireframe));

    //GameObject* rigid = new RotatingSquare(cubeObjGreen, 1.0f, 1.0f, 5.0f, glm::vec3(0.0f, 0.0f, 2.0f));

    GameObject* bowlingPin = new GameObject(bowlingPinMesh, 1.0f, glm::vec3(0, 0, 2), 0.0f, glm::vec3(0, 0, 0), bowlingPinCollider);
    GameObject* withCollider = new GameObject(sphereObjBlue, 2.0f, glm::vec3(-3, 1, 2), 0.0f, glm::vec3(0, 0, 0), new SphereCollider(1.0f, sphereWireframe));
    //GameObject* plane = new GameObject(planeMesh, 1.0f, glm::vec3(0, -1, 2), 0.0f, glm::vec3(0,0,0), new PlaneCollider(planeMesh, glm::vec3(0, 1, 0)));

    //objects.push_back(cubeObject1);
    objects.push_back(player);
    //objects.push_back(rigid);
    //objects.push_back(cubeObj);
    objects.push_back(withCollider);
    //objects.push_back(withCollider2);
    //objects.push_back(plane);
    //objects.push_back(bowlingPin);

    ForceGenerator gravity(glm::vec3(0.0f, -9.8f, 0.0f));
    //gravity.AddObject(cubeObject1);
    //gravity.AddObject(cubeObj);
    //gravity.AddObject(player);

    
    //location location location
    GLuint ambientLocation = glGetUniformLocation(shader.GetProgramID(), "ambientIntensity");
    GLuint diffuseLocation = glGetUniformLocation(shader.GetProgramID(), "diffuseIntensity");
    GLuint lightPosLocation = glGetUniformLocation(shader.GetProgramID(), "lightPosition");
    GLuint specIntensityLocation = glGetUniformLocation(shader.GetProgramID(), "specularIntensity");
    GLuint specColorLocation = glGetUniformLocation(shader.GetProgramID(), "specularColor");
    GLuint cameraPosLocation = glGetUniformLocation(shader.GetProgramID(), "cameraPosition");
    GLuint shininesLocation = glGetUniformLocation(shader.GetProgramID(), "shinines");

    modelLocation = glGetUniformLocation(shader.GetProgramID(), "modelMatrix");
    viewLocation = glGetUniformLocation(shader.GetProgramID(), "viewMatrix");
    projLocation = glGetUniformLocation(shader.GetProgramID(), "projMatrix");

    Mesh::modelLocation = modelLocation;

    Camera camera;

    ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    diffuseIntensity = glm::vec3(0.8f, 0.8f, 0.8f);
    lightPosition = glm::vec3(0.0f, 1.0f, 0.0f);
    specularIntensity = glm::vec3(1.0f);
    specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    shinines = 256.0f;

    //send it
    glUniform3fv(ambientLocation, 1, glm::value_ptr(ambient));
    glUniform3fv(diffuseLocation, 1, glm::value_ptr(diffuseIntensity));
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPosition));
    glUniform3fv(specIntensityLocation, 1, glm::value_ptr(specularIntensity));
    glUniform3fv(specColorLocation, 1, glm::value_ptr(specularColor));
    glUniform3fv(cameraPosLocation, 1, glm::value_ptr(camera.cameraPosition));
    glUniform1f(shininesLocation, shinines);

    glEnable(GL_DEPTH_TEST);
    float depthValue = 100.0f;

    //main loop-------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        oldTime = currentTime;
        currentTime = glfwGetTime();
        
        double deltaTime = currentTime - oldTime;
        //std::cout << deltaTime << std::endl;

        const float backgroundColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glClearBufferfv(GL_COLOR, 0, backgroundColour);
        glClearBufferfv(GL_DEPTH, 0, &depthValue);
        
        glfwPollEvents();

        camera.UpdateCamera();


        gravity.Apply();
       
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->Update(deltaTime);
            objects[i]->Draw();
        }

        CollisionInformation collision;
        for (int i = 0; i < objects.size(); i++)
        {
            for (int j = i + 1; j < objects.size(); j++)
            {
                if (objects[i] != objects[j] && objects[i]->collider != nullptr && objects[j]->collider != nullptr)
                {
                    if (objects[i]->collider->CheckForCollision(objects[j]->collider, collision))
                    {
                        collision.object1 = objects[i];
                        collision.object2 = objects[j];
                        collisions.push_back(collision);
                        std::cout << "Collision detected" << std::endl;
                    }
                }
            }
        }
        
        for (int i = 0; i < collisions.size(); i++)
        {
            collisions[i].object1->collider->CollisionResponse(collisions[i]);
            
        }
        collisions.clear();

        glfwSwapBuffers(window);
    }
    //----------------------------------------------------------------------------------------------------

    //cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    //for (int i = 0; i < objects.size(); i++)
    //{
    //    delete objects[i]->mesh;
    //}

    return 0;
}

