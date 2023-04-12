
#pragma once
#include<GL/glew.h>
//#include"obj_mesh.h"

struct ObjData;

class ModelContainer
{
public:
	ModelContainer(GLfloat offset_x, GLfloat offset_y, GLfloat offset_z);

	ObjData* objData;
	GLfloat offsets[3] = { 0.f, 0.f, 0.f };
};
