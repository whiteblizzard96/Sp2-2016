#include "SceneModel.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

SceneModel::SceneModel()
{
}

SceneModel::~SceneModel()
{
}

void SceneModel::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	//glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Shading.vertexshader","Shader//LightSource.fragmentshader");
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Blending.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID,"MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID,"MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_programID);

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 20, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutOff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	//Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutOff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(10, 40, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutOff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutOff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS],1);

	//Initialize camera settings
	camera.Init(Vector3(10, 0, 70), Vector3(0, 0, 0), Vector3(0, 10, 0));
	meshList[GEO_REF_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0));
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 0), 1, 1);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Lucy.tga");
	
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1,1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_MCHAIR] = MeshBuilder::GenerateOBJ("chair", "OBJ//chair.obj");
	meshList[GEO_MCHAIR]->textureID = LoadTGA("Image//chair.tga");
	
	meshList[GEO_MDART] = MeshBuilder::GenerateOBJ("dart", "OBJ//dart.obj");
	meshList[GEO_MDART]->textureID = LoadTGA("Image//dart.tga");

	meshList[GEO_MDARTBOARD] = MeshBuilder::GenerateOBJ("dartboard", "OBJ//dartboard.obj");
	meshList[GEO_MDARTBOARD]->textureID = LoadTGA("Image//dartboard.tga");

	meshList[GEO_MDOORMAN] = MeshBuilder::GenerateOBJ("doorman", "OBJ//doorman.obj");
	meshList[GEO_MDOORMAN]->textureID = LoadTGA("Image//doorman.tga");

	meshList[GEO_MSHOE] = MeshBuilder::GenerateOBJ("shoe", "OBJ//shoe.obj");
	meshList[GEO_MSHOE]->textureID = LoadTGA("Image//shoe.tga");

	meshList[GEO_MWINEBOTTLE] = MeshBuilder::GenerateOBJ("winebottle", "OBJ//winebottle.obj");
	meshList[GEO_MWINEBOTTLE]->textureID = LoadTGA("Image//winebottle.tga");

	meshList[GEO_MLAND] = MeshBuilder::GenerateOBJ("land", "OBJ//Land.obj");
	meshList[GEO_MLAND]->textureID = LoadTGA("Image//Land.tga");

	meshList[GEO_MFLY] = MeshBuilder::GenerateOBJ("fly", "OBJ//Fly.obj");
	meshList[GEO_MFLY]->textureID = LoadTGA("Image//Fly.tga");

	meshList[GEO_MHOLDER] = MeshBuilder::GenerateOBJ("holder", "OBJ//Holder.obj");
	meshList[GEO_MHOLDER]->textureID = LoadTGA("Image//Holder.tga");


	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSpheres("Sph", Color(1, 1, 1), 18, 36);

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}

static float LSPEED = 10.f;
bool Lighting6 = true;

void SceneModel::Update(double dt)
{
	camera.Update(dt);

	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('5'))
	{
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);

		light[1].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	}
	if (Application::IsKeyPressed('6'))
	{
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);

		light[1].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	}
	if (Application::IsKeyPressed('7'))
	{
		light[0].type =Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);

		light[1].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	}
		
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);


		if (Application::IsKeyPressed('C'))
		light[1].position.z -= (float)(LSPEED * dt);
		if (Application::IsKeyPressed('V'))
		light[1].position.z += (float)(LSPEED * dt);
		if (Application::IsKeyPressed('B'))
		light[1].position.x -= (float)(LSPEED * dt);
		if (Application::IsKeyPressed('N'))
		light[1].position.x += (float)(LSPEED * dt);
		if (Application::IsKeyPressed('M'))
		light[1].position.y -= (float)(LSPEED * dt);
		if (Application::IsKeyPressed(','))
		light[1].position.y += (float)(LSPEED * dt);


	if (Application::IsKeyPressed('Z'))
		Lighting6 = false;

	if (Application::IsKeyPressed('X'))
		Lighting6 = true;

}
void SceneModel::RenderMesh(Mesh * mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight && Lighting6)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
void SceneModel::RenderSkybox()
{
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, 0.9);
	modelStack.Translate(0, -20, -200);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);

	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, -0.9);
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0.9, 0, 0);
	modelStack.Translate(-100, -20, -100);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(-180, 1, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here	
	modelStack.Translate(-0.5, 0, 0);
	modelStack.Translate(100, -20, -100);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -120, -100);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -0.9, 0);
	modelStack.Translate(0, 80, -100);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(360, 0, 0, 1);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
}
void SceneModel::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	
	modelStack.LoadIdentity();
//new code
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	
	RenderMesh(meshList[GEO_REF_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 91);

	RenderSkybox();
	
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(10, 0, -90);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(30,30, 30);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, -80);
	RenderMesh(meshList[GEO_MCHAIR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-15, -2, -80);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_MDOORMAN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3, 17, -100);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_MDARTBOARD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(-1, -35, -100);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_MDART], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3, 0, -60);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_MSHOE], true);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-5, -20, -70);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_MLAND], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -60);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_MFLY], true);
	modelStack.PushMatrix();
	modelStack.Translate(0, -0.1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_MHOLDER], true);
	modelStack.PopMatrix();
	
	modelStack.PopMatrix();

}
void SceneModel::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}