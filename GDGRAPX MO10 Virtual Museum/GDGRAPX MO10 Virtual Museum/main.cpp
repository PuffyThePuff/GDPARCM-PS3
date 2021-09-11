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
	GLfloat offsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&meteorObjData,
		1.0f,
		offsets
	);

	ObjData streetLightObjData;
	LoadObjFile(&streetLightObjData, "Street Light/Street Light.obj");
	GLfloat offsets2[] = { -1.0f, 0.0f, -3.5f };
	LoadObjToMemory(
		&streetLightObjData,
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
	SkyboxData skybox = LoadSkybox("Assets/Skybox/Space", faces);
	SkyboxData daybox = LoadSkybox("Assets/Skybox/Day", faces);

#pragma endregion

#pragma region Shader Loading

	// For the skybox
	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");


	// For directional light
	GLuint directionalShaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/directional_fragment.shader");
	glUseProgram(directionalShaderProgram);

	GLuint colorLoc = glGetUniformLocation(directionalShaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

	GLuint modelTransformLoc = glGetUniformLocation(directionalShaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(directionalShaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(directionalShaderProgram, "u_projection");

	GLuint normalTransformLoc = glGetUniformLocation(directionalShaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(directionalShaderProgram, "u_camera_position");

	glm::vec3 lightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	GLuint lightDirectionLoc = glGetUniformLocation(directionalShaderProgram, "u_light_direction");
	glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);

	GLuint ambientColorLoc = glGetUniformLocation(directionalShaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.2f, 0.2f, 0.2f);

	GLuint lightColorLoc = glGetUniformLocation(directionalShaderProgram, "u_light_color");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);


	// For point light
	GLuint pointShaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/point_fragment.shader");
	//glUseProgram(pointShaderProgram);

	GLuint colorLoc2 = glGetUniformLocation(pointShaderProgram, "u_color");
	glUniform3f(colorLoc2, 1.0f, 1.0f, 1.0f);

	GLuint modelTransformLoc2 = glGetUniformLocation(pointShaderProgram, "u_model");
	GLuint viewLoc2 = glGetUniformLocation(pointShaderProgram, "u_view");
	GLuint projectionLoc2 = glGetUniformLocation(pointShaderProgram, "u_projection");

	GLuint normalTransformLoc2 = glGetUniformLocation(pointShaderProgram, "u_normal");
	GLuint cameraPosLoc2 = glGetUniformLocation(pointShaderProgram, "u_camera_position");

	GLuint lightPositionLoc2 = glGetUniformLocation(directionalShaderProgram, "u_light_position");
	glUniform3f(lightPositionLoc2, 0.0f, 4.0f, 0.0f);

	GLuint ambientColorLoc2 = glGetUniformLocation(pointShaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc2, 0.2f, 0.2f, 0.2f);

	GLuint lightColorLoc2 = glGetUniformLocation(pointShaderProgram, "u_light_color");
	glUniform3f(lightColorLoc2, 1.0f, 1.0f, 1.0f);


	// For spot light
	GLuint spotShaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/spot_fragment.shader");
	//glUseProgram(spotShaderProgram);

	GLuint colorLoc3 = glGetUniformLocation(spotShaderProgram, "u_color");
	glUniform3f(colorLoc3, 1.0f, 1.0f, 1.0f);

	GLuint modelTransformLoc3 = glGetUniformLocation(spotShaderProgram, "u_model");
	GLuint viewLoc3 = glGetUniformLocation(spotShaderProgram, "u_view");
	GLuint projectionLoc3 = glGetUniformLocation(spotShaderProgram, "u_projection");

	GLuint normalTransformLoc3 = glGetUniformLocation(spotShaderProgram, "u_normal");
	GLuint cameraPosLoc3 = glGetUniformLocation(spotShaderProgram, "u_camera_position");

	GLuint lightPositionLoc3 = glGetUniformLocation(directionalShaderProgram, "u_light_position");
	glUniform3f(lightPositionLoc3, -1.0f, 4.0f, -5.0f);

	GLuint lightDirectionLoc3 = glGetUniformLocation(spotShaderProgram, "u_light_direction");
	glUniform3f(lightDirectionLoc3, 1.0f, 0.0f, 0.0f);

	GLuint ambientColorLoc3 = glGetUniformLocation(spotShaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc3, 0.2f, 0.2f, 0.2f);

	GLuint lightColorLoc3 = glGetUniformLocation(spotShaderProgram, "u_light_color");
	glUniform3f(lightColorLoc3, 1.0f, 1.0f, 1.0f);


	glm::mat4 trans = glm::mat4(1.0f);
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glm::mat4 inputTrans = glm::mat4(1.0f);
	glm::mat4 lightDirectionTrans = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

#pragma endregion

	// set bg color to sky blue
	glClearColor(0.6f, 0.84f, 0.84f, 0.0f);

	// variables for inputs
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;
	float elapsedTime = 0.0f;

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
		bool daytime = true;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
#pragma endregion

#pragma region Projection
		// Perspective Projection
		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 10.0f);

		// Set projection matrix in shaders
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(projectionLoc3, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View
		glm::mat4 view = glm::lookAt(camPosition, camPosition + camFront, camUp);
		glUniform3f(cameraPosLoc, camPosition.x, camPosition.y, camPosition.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view));

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

		if (daytime) {
			DrawSkybox(daybox, skyboxShaderProgram, view, projection);
		}
		else {
			DrawSkybox(skybox, skyboxShaderProgram, view, projection);
		}

		glBindVertexArray(meteorObjData.vaoId);
		glUseProgram(directionalShaderProgram);
		// else glUseProgram(pointShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		GLuint meteorTexture = meteorObjData.textures[meteorObjData.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, meteorTexture);
		glDrawElements(GL_TRIANGLES, meteorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 normalTrans = glm::transpose(glm::inverse(trans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans));

		glBindVertexArray(streetLightObjData.vaoId);
		glUseProgram(directionalShaderProgram);
		// else glUseProgram(pointShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		GLuint streetLightTexture = streetLightObjData.textures[streetLightObjData.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, streetLightTexture);
		glDrawElements(GL_TRIANGLES, streetLightObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 normalTrans2 = glm::transpose(glm::inverse(trans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans2));

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
		//glUniform3f(cameraPosLoc2, camPosition.x, camPosition.y, camPosition.z);
		//glUniform3f(cameraPosLoc3, camPosition.x, camPosition.y, camPosition.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view));

		lightDirectionTrans = glm::mat4(1.0f);
		lightDirectionTrans = glm::rotate(lightDirectionTrans, glm::radians(6.f * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
		lightDirection = lightDirectionTrans * glm::vec4(lightDirection, 1.0f);
		glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;
		elapsedTime += deltaTime;

		if (elapsedTime >= 30.0f) {
			if (daytime) {
				daytime = false;
			}
			else {
				daytime = true;
			}
			elapsedTime = 0.0f;
		}

		// --- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		// listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}