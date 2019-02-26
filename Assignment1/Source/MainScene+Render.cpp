#include "MainScene.h"
#include "Utility.h"

void MainScene::applyMaterial(Mesh* model) {
	model->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	model->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	model->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
	model->material.kShininess = 1.0f;
}

void MainScene::renderMesh(Mesh* model, bool enableLight) {
	Mtx44 modelView, modelView_inverse_transpose;

	if (e_Phases == FIRST_PASS)
	{
		Mtx44 lightDepthMVP = lightProj * lightView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_FIRSTPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

		model->render();
		return;
	}

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	Mtx44 mTop = modelStack.Top();
	Mtx44 vTop = viewStack.Top();

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight && lightingEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		Mtx44 lightDepthMVP = lightProj * lightView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &model->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &model->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &model->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], model->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (model ->isTextured()) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->getTextureID());
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	} else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	model->render();

	if (model->isTextured()) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void MainScene::initText() {
	models[TEXT] = MeshBuilder::GenerateText("TEXT", 16, 16);
	models[TEXT]->applyTexture("Image//calibri.tga");
	models[JOINONLINE_QUAD] = MeshBuilder::GenerateText("Join Online Game", 16, 16);
	models[JOINONLINE_QUAD]->applyTexture("Image//calibriOpacity.tga");
	models[STARTLOCAL_QUAD] = MeshBuilder::GenerateText("Start Local", 16, 16);
	models[STARTLOCAL_QUAD]->applyTexture("Image//calibri.tga");
	models[JOINLOCAL_QUAD] = MeshBuilder::GenerateText("JOIN LOCAL", 16, 16);
	models[JOINLOCAL_QUAD]->applyTexture("Image//calibriOpacity.tga");
	models[EXIT] = MeshBuilder::GenerateText("EXIT", 16, 16);
	models[EXIT]->applyTexture("Image//calibriOpacity.tga");
	models[LOBBY_QUAD] = MeshBuilder::GenerateText("LOBBY", 16, 16);
	models[LOBBY_QUAD]->applyTexture("Image//calibri.tga");
	models[ONLINELOBBY_QUAD] = MeshBuilder::GenerateText("ONLINE", 16, 16);
	models[ONLINELOBBY_QUAD]->applyTexture("Image//calibri.tga");
	models[LOCALLOBY_QUAD] = MeshBuilder::GenerateText("LOCAL", 16, 16);
	models[LOCALLOBY_QUAD]->applyTexture("Image//calibriOpacity.tga");
	models[BACK_QUAD] = MeshBuilder::GenerateText("BACK", 16, 16);
	models[BACK_QUAD]->applyTexture("Image//calibriOpacity.tga");
	models[STARTONLINE_QUAD] = MeshBuilder::GenerateText("Create Online Game", 16, 16);
	models[STARTONLINE_QUAD]->applyTexture("Image//calibri.tga");
}

void MainScene::renderText(Mesh* mesh, const std::string text, Color color) {
	if (!mesh || mesh->getTextureID() <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID());
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each  character, you may change this value
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() *
			characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void MainScene::renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) 
{
	if (!mesh || mesh->getTextureID() <= 0) //Proper error check
		return;

	int lineCount = 0;
	int offsetReset = 0;
	int newLineCount = 0;

	for (char& c : text)
		if (c == '\n') newLineCount++;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y + newLineCount, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID());
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		if (text[i] == '\n')
		{
			lineCount++;
			offsetReset = i + 1;
		}
		else
		{
			Mtx44 characterSpacing;
			characterSpacing.SetToTranslation(i * 1.0f - offsetReset, -lineCount, 0); //1.0f is the spacing of each character, you may change this value
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

			mesh->render((unsigned)text[i] * 6, 6);
		}

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
//RenderText for main menu ( Uses Different shader from main scene)
void MainScene::renderTextOnScreenMenu(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->getTextureID() <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED_MENU], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR_MENU], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED_MENU], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID());
	glUniform1i(m_parameters[U_COLOR_TEXTURE_MENU], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP_MENU], 1, GL_FALSE, &MVP.a[0]);

		mesh->render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED_MENU], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
//Render Menu ( Uses different shader from main game )
void MainScene::renderMenu2D(Mesh* model,float sizex,float sizey,float sizez,float x,float y, bool enableLight)
{
	projectionStack.LoadIdentity();
	Mtx44 Orthographic;
	Orthographic.SetToOrtho(-80, 60, -80, 80, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(Orthographic);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(sizex, sizey, sizez);
	modelStack.Translate(x, y, 0);
	
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP_MENU], 1, GL_FALSE, &MVP.a[0]);
	if (model->isTextured())
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED_MENU], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->getTextureID());
		glUniform1i(m_parameters[U_COLOR_TEXTURE_MENU], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_MENU], 0);
	}
	model->render();


	if (model->isTextured()) 
	{
	glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void MainScene::renderMeshMenu(Mesh* model, bool enableLight)
{

	Mtx44 modelView, modelView_inverse_transpose;


	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(m_parameters[U_MVP_MENU], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	Mtx44 mTop = modelStack.Top();
	Mtx44 vTop = viewStack.Top();

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW_MENU], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight && lightingEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE_MENU], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT_MENU], 1, &model->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE_MENU], 1, &model->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR_MENU], 1, &model->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS_MENU], model->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (model->isTextured()) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED_MENU], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->getTextureID());
		glUniform1i(m_parameters[U_COLOR_TEXTURE_MENU], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED_MENU], 0);
	}

	model->render();

	if (model->isTextured()) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}