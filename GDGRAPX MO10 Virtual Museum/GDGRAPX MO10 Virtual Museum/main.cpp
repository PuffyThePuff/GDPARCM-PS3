/*
Jose Miguel D. Bravo
Nathaniel Francis S. Filoteo
GDPARCM X22
Problem Set 3
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
	GLfloat offsets2[] = { -10.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&streetLightObjData,
		1.0f,
		offsets2
	);

	ObjData chairObjData;
	LoadObjFile(&chairObjData, "Chair/Chair.obj");
	GLfloat offsets3[] = { -5.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&chairObjData,
		1.0f,
		offsets3
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

	// Load the skybox program
	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");


	// Load directional light program; declare uniform location variables
	GLuint directionalShaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/directional_fragment.shader");
	glUseProgram(directionalShaderProgram);

	GLuint colorLoc = glGetUniformLocation(directionalShaderProgram, "u_color");

	GLuint modelTransformLoc = glGetUniformLocation(directionalShaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(directionalShaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(directionalShaderProgram, "u_projection");

	GLuint normalTransformLoc = glGetUniformLocation(directionalShaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(directionalShaderProgram, "u_camera_position");

	GLuint lightDirectionLoc = glGetUniformLocation(directionalShaderProgram, "u_light_direction");

	GLuint ambientColorLoc = glGetUniformLocation(directionalShaderProgram, "u_ambient_color");
	GLuint lightColorLoc = glGetUniformLocation(directionalShaderProgram, "u_light_color");

	GLuint textureOneLoc = glGetUniformLocation(directionalShaderProgram, "texture_diffuse");
	GLuint textureTwoLoc = glGetUniformLocation(directionalShaderProgram, "texture2_diffuse");
	

	// Set values for directional light
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

	glm::mat4 trans = glm::mat4(1.0f);
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glm::vec3 lightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);

	glUniform3f(ambientColorLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	glUniform1i(textureOneLoc, 0);
	glUniform1i(textureTwoLoc, 1);


	// Load point light program; declare uniform location variables
	GLuint pointShaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/point_fragment.shader");

	GLuint colorLoc2 = glGetUniformLocation(pointShaderProgram, "u_color");

	GLuint modelTransformLoc2 = glGetUniformLocation(pointShaderProgram, "u_model");
	GLuint viewLoc2 = glGetUniformLocation(pointShaderProgram, "u_view");
	GLuint projectionLoc2 = glGetUniformLocation(pointShaderProgram, "u_projection");

	GLuint normalTransformLoc2 = glGetUniformLocation(pointShaderProgram, "u_normal");
	GLuint cameraPosLoc2 = glGetUniformLocation(pointShaderProgram, "u_camera_position");

	GLuint lightPositionLoc2 = glGetUniformLocation(pointShaderProgram, "u_light_position");

	GLuint ambientColorLoc2 = glGetUniformLocation(pointShaderProgram, "u_ambient_color");
	GLuint lightColorLoc2 = glGetUniformLocation(pointShaderProgram, "u_light_color");

	GLuint textureOneLoc2 = glGetUniformLocation(pointShaderProgram, "texture_diffuse");
	GLuint textureTwoLoc2 = glGetUniformLocation(pointShaderProgram, "texture2_diffuse");


	// Load spotlight program; declare uniform location variables
	GLuint spotShaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/spot_fragment.shader");

	GLuint colorLoc3 = glGetUniformLocation(spotShaderProgram, "u_color");

	GLuint modelTransformLoc3 = glGetUniformLocation(spotShaderProgram, "u_model");
	GLuint viewLoc3 = glGetUniformLocation(spotShaderProgram, "u_view");
	GLuint projectionLoc3 = glGetUniformLocation(spotShaderProgram, "u_projection");

	GLuint normalTransformLoc3 = glGetUniformLocation(spotShaderProgram, "u_normal");
	GLuint cameraPosLoc3 = glGetUniformLocation(spotShaderProgram, "u_camera_position");

	GLuint lightPositionLoc3 = glGetUniformLocation(spotShaderProgram, "u_light_position");
	GLuint lightDirectionLoc3 = glGetUniformLocation(spotShaderProgram, "u_light_direction");

	GLuint ambientColorLoc3 = glGetUniformLocation(spotShaderProgram, "u_ambient_color");
	GLuint lightColorLoc3 = glGetUniformLocation(spotShaderProgram, "u_light_color");

	GLuint textureOneLoc3 = glGetUniformLocation(spotShaderProgram, "texture_diffuse");
	GLuint textureTwoLoc3 = glGetUniformLocation(spotShaderProgram, "texture2_diffuse");


	// Declare other transform matrices
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
	bool daytime = true;

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

		// Set projection matrix in shaders
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

		if (daytime) {
			DrawSkybox(daybox, skyboxShaderProgram, view, projection);
		}
		else {
			DrawSkybox(skybox, skyboxShaderProgram, view, projection);
		}


		glBindVertexArray(meteorObjData.vaoId);

		if (daytime) {
			glUseProgram(directionalShaderProgram);

			glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);
			glUniform3f(ambientColorLoc, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			glUniform1i(textureOneLoc, 0);
			glUniform1i(textureTwoLoc, 1);
		}

		else {
			glUseProgram(pointShaderProgram);

			glUniform3f(colorLoc2, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelTransformLoc2, 1, GL_FALSE, glm::value_ptr(trans));
			glUniform3f(lightPositionLoc2, 0.0f, -40.0f, 0.0f);
			glUniform3f(ambientColorLoc2, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightColorLoc2, 1.0f, 1.0f, 1.0f);
			glUniform1i(textureOneLoc2, 0);
			glUniform1i(textureTwoLoc2, 1);
		}

		glm::mat4 normalTrans = glm::transpose(glm::inverse(trans));
		if (daytime) glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans));
		else glUniformMatrix4fv(normalTransformLoc2, 1, GL_FALSE, glm::value_ptr(normalTrans));

		glActiveTexture(GL_TEXTURE0);
		GLuint meteorTexture = meteorObjData.textures[meteorObjData.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, meteorTexture);

		glDrawElements(GL_TRIANGLES, meteorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glBindVertexArray(streetLightObjData.vaoId);
		if (daytime) {
			glUseProgram(directionalShaderProgram);

			glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);
			glUniform3f(ambientColorLoc, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			glUniform1i(textureOneLoc, 0);
			glUniform1i(textureTwoLoc, 1);
		}

		else {
			glUseProgram(pointShaderProgram);

			glUniform3f(colorLoc2, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelTransformLoc2, 1, GL_FALSE, glm::value_ptr(trans));
			glUniform3f(lightPositionLoc2, 0.0f, -40.0f, 0.0f);
			glUniform3f(ambientColorLoc2, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightColorLoc2, 1.0f, 1.0f, 1.0f);
			glUniform1i(textureOneLoc2, 0);
			glUniform1i(textureTwoLoc2, 1);
		}

		glm::mat4 normalTrans2 = glm::transpose(glm::inverse(trans));
		if (daytime) glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans2));
		else glUniformMatrix4fv(normalTransformLoc2, 1, GL_FALSE, glm::value_ptr(normalTrans2));

		GLuint streetLightTexture = streetLightObjData.textures[streetLightObjData.materials[0].diffuse_texname];
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, streetLightTexture);

		GLuint streetLightTexture2 = streetLightObjData.textures[streetLightObjData.materials[1].diffuse_texname];
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, streetLightTexture2);

		glDrawElements(GL_TRIANGLES, streetLightObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glBindVertexArray(chairObjData.vaoId);
		if (daytime) {
			glUseProgram(directionalShaderProgram);

			glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);
			glUniform3f(ambientColorLoc, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			glUniform1i(textureOneLoc, 0);
			glUniform1i(textureTwoLoc, 1);
		}

		else {
			glUseProgram(pointShaderProgram);

			glUniform3f(colorLoc2, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelTransformLoc2, 1, GL_FALSE, glm::value_ptr(trans));
			glUniform3f(lightPositionLoc2, 0.0f, -40.0f, 0.0f);
			glUniform3f(ambientColorLoc2, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightColorLoc2, 1.0f, 1.0f, 1.0f);
			glUniform1i(textureOneLoc2, 0);
			glUniform1i(textureTwoLoc2, 1);
		}

		glm::mat4 normalTrans3 = glm::transpose(glm::inverse(trans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans3));

		glActiveTexture(GL_TEXTURE0);
		GLuint chairTexture = chairObjData.textures[chairObjData.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, chairTexture);
		glDrawElements(GL_TRIANGLES, chairObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		glBindTexture(GL_TEXTURE_2D, 0);


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
		if (daytime) {
			glUniform3f(cameraPosLoc, camPosition.x, camPosition.y, camPosition.z);
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		}
		else {
			glUniform3f(cameraPosLoc2, camPosition.x, camPosition.y, camPosition.z);
			glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		}

		lightDirectionTrans = glm::mat4(1.0f);
		lightDirectionTrans = glm::rotate(lightDirectionTrans, glm::radians(6.f * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
		lightDirection = lightDirectionTrans * glm::vec4(lightDirection, 1.0f);
		if (daytime) glUniform3f(lightDirectionLoc, lightDirection.x, lightDirection.y, lightDirection.z);

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