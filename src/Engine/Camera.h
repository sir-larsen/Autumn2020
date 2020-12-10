/**
 * @file Camera.h
 * @brief Header file for the Camera object
 *
 */
#ifndef CAMERA_H
#define CAMERA_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = 10.0f;
const float PITCH = 0.0f;
const float SPEED = 150.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

//Related to third person camera
const float tYAW = 50.0f;
const float tPITCH = 20.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    //camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
    //Attributes related to third person camera
    glm::vec3 objPos;
    bool perspective;
    float distance; //Distance from object
    glm::vec3 dir;
    glm::mat4 view;
    float tYaw;
    float tPitch;


    Camera(glm::vec3 position = glm::vec3(0.0f, 0.35f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float tyaw = tYAW, float tpitch = tPITCH);
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void togglePerspective() { perspective = !perspective; std::cout << "PERSPECTIVE TOGGLED!" << std::endl; }
    void setObjPos(glm::vec3 pos);

private:
    void updateCameraVectors();
};
#endif