#ifndef SCENE_2_H
#define SCENE_2_H

#include "Scene.h"
#include "Mtx44.h"

class Scene2 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_TRIANGLE_1 = 0,
		GEO_TRIANGLE_2,
		NUM_GEOMETRY,

	};
	
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

public:
	Scene2();
	~Scene2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	
	float rotateAngle;
	float rotateFull;
	float translateX;
	float translateY;
	float translateY1;
	float scaleAll;

	unsigned speed;
	unsigned speed1;
	int dropping;
	int dropping1;

	bool rotate;
	bool scale;

};
#endif