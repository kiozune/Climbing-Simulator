#include "MainMenu.h"
#include "Utility.h"

void MainMenu::applyMaterial(Mesh* model) {
	model->material.kAmbient.Set(0.15f, 0.15f, 0.15f);
	model->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	model->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
	model->material.kShininess = 1.0f;
}

void MainMenu::renderMesh(Mesh* model, bool enableLight) {
	Mtx44 modelView, modelView_inverse_transpose;

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

void MainMenu::initText() {
	models[TEXT] = MeshBuilder::GenerateText("TEXT", 16, 16);
	models[TEXT]->applyTexture("Image//calibri.tga");
}

void MainMenu::renderText(Mesh* mesh, const std::string text, Color color) {
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

void MainMenu::renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) 
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