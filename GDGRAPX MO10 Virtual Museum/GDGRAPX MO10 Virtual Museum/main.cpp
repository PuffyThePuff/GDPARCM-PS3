/*
Jose Miguel D. Bravo
Nathaniel Francis S. Filoteo
GDGRAPX S11
Virtual Museum Beta
*/

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp";
#include "obj_mesh.h"
#include "shader.h"
#include "skybox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
	stbi_set_flip_vertically_on_load(true);
#pragma region Initialization
	// initialize glfw
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return -1;
	}

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Virtual Museum", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Mesh Loading
	ObjData meteorObjData;
	LoadObjFile(&meteorObjData, "Meteor/Meteor.obj");
	GLfloat offsets2[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&meteorObjData,
		1.0f,
		offsets2
	);

	std::vector<std::string> faces{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};
	SkyboxData skybox = LoadSkybox("Assets/Skybox", faces);

#pragma endregion

#pragma region Shader Loading

	GLuint shaderProgram = LoadShaders("Shaders/directional_vertex.shader", "Shaders/directional_fragment.shader");
	glUseProgram(shaderProgram);

	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	GLuint normalTransformLoc = glGetUniformLocation(shaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(shaderProgram, "u_camera_position");

	glm::mat4 trans = glm::mat4(1.0f);
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glm::mat4 inputTrans = glm::mat4(1.0f);

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);

	//GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_position");
	//glUniform3f(lightPosLoc, 1.0f, 5.0f, 3.0f);

	glm::vec3 lightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	GLuint lightDirectionLoc = glGetUniformLocation(shaderProgram, "u_light_direction");
	glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);

	glm::mat4 lightDirectionTrans = glm::mat4(1.0f);

	GLuint ambientColorLoc = glGetUniformLocation(shaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.2f, 0.2f, 0.2f);

	GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "u_light_color");
	glUniform3f(lightColorLoc, 0.6f, 0.84f, 0.84f);

#pragma endregion

	// set bg color to sky blue
	glClearColor(0.6f, 0.84f, 0.84f, 0.0f);

	// variables for inputs
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;

	int state[8];
	glm::vec3 camPosition = glm::vec3(0.0f, 0.5f, 10.0f);
	glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// depth testing
	glEnable(GL_DEPTH_TEST);

	// face culling
	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window)) {

#pragma region Viewport
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
#pragma endregion

#pragma region Projection
		// Perspective Projection
		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 10.0f);

		// Set projection matrix in shader
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View
		glm::mat4 view = glm::lookAt(camPosition, camPosition + camFront, camUp);
		glUniform3f(cameraPosLoc, camPosition.x, camPosition.y, camPosition.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// (toggle to render wit GL_FILL or GL_LINE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw
		state[0] = glfwGetKey(window, GLFW_KEY_W);
		state[1] = glfwGetKey(window, GLFW_KEY_S);
		state[2] = glfwGetKey(window, GLFW_KEY_A);
		state[3] = glfwGetKey(window, GLFW_KEY_D);

		state[4] = glfwGetKey(window, GLFW_KEY_UP);
		state[5] = glfwGetKey(window, GLFW_KEY_DOWN);
		state[6] = glfwGetKey(window, GLFW_KEY_LEFT);
		state[7] = glfwGetKey(window, GLFW_KEY_RIGHT);


		DrawSkybox(skybox, skyboxShaderProgram, view, projection);

		glBindVertexArray(meteorObjData.vaoId);
		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0);

		GLuint meteorTexture = meteorObjData.textures[meteorObjData.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, meteorTexture);
		glDrawElements(GL_TRIANGLES, meteorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 normalTrans = glm::transpose(glm::inverse(trans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans));

		if (state[0] == GLFW_PRESS) {
			camPosition += 2.0f * deltaTime * camFront;
		}

		if (state[1] == GLFW_PRESS) {
			camPosition -= 2.0f * deltaTime * camFront;
		}

		if (state[2] == GLFW_PRESS) {
			camPosition += 2.0f * deltaTime * glm::normalize(glm::cross(camUp, camFront));
		}

		if (state[3] == GLFW_PRESS) {
			camPosition -= 2.0f * deltaTime * glm::normalize(glm::cross(camUp, camFront));
		}

		if (state[4] == GLFW_PRESS) {
			inputTrans = glm::mat4(1.0f);
			inputTrans = glm::rotate(inputTrans, 3.0f * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
			camFront = inputTrans * glm::vec4(camFront, 1.0f);
		}

		if (state[5] == GLFW_PRESS) {
			inputTrans = glm::mat4(1.0f);
			inputTrans = glm::rotate(inputTrans, -3.0f * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
			camFront = inputTrans * glm::vec4(camFront, 1.0f);
		}

		if (state[6] == GLFW_PRESS) {
			inputTrans = glm::mat4(1.0f);
			inputTrans = glm::rotate(inputTrans, 3.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
			camFront = inputTrans * glm::vec4(camFront, 1.0f);
		}

		if (state[7] == GLFW_PRESS) {
			inputTrans = glm::mat4(1.0f);
			inputTrans = glm::rotate(inputTrans, -3.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
			camFront = inputTrans * glm::vec4(camFront, 1.0f);
		}

		view = glm::lookAt(camPosition, camPosition + camFront, camUp);
		glUniform3f(cameraPosLoc, camPosition.x, camPosition.y, camPosition.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightDirectionTrans = glm::mat4(1.0f);
		lightDirectionTrans = glm::rotate(lightDirectionTrans, glm::radians(6.f * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
		lightDirection = lightDirectionTrans * glm::vec4(lightDirection, 1.0f);
		glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		// --- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		// listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}