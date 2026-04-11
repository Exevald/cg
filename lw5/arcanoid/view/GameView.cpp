#include "GameView.h"
#include "TextureBuilder.h"

#include <GLUT/glut.h>
#include <algorithm>
#include <cmath>
#include <string>

namespace
{

constexpr float OBJECT_DEPTH = 0.55f;

void SetColor(const float r, const float g, const float b)
{
	const float diffuse[] = { r, g, b, 1.0f };
	const float ambient[] = { r * 0.35f, g * 0.35f, b * 0.35f, 1.0f };
	constexpr float specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 35.0f);
	glColor3f(r, g, b);
}

std::string TexturePath(const std::string& filename)
{
	return std::string(ARCANOID_TEXTURE_DIR) + "/" + filename;
}

std::string GetPowerUpName(const PowerUpType type)
{
	switch (type)
	{
	case PowerUpType::ExtraLife:
		return "Life";
	case PowerUpType::SlowBall:
		return "Slow";
	case PowerUpType::FastBall:
		return "Fast";
	case PowerUpType::WidePaddle:
		return "Wide";
	case PowerUpType::NarrowPaddle:
		return "Narrow";
	case PowerUpType::StickyBall:
		return "Sticky";
	case PowerUpType::MultiBall:
		return "Multi";
	}
	return "Bonus";
}

void DrawText(const float x, const float y, const char* text, void* font)
{
	glRasterPos2f(x, y);
	for (const char* cursor = text; *cursor != '\0'; ++cursor)
	{
		glutBitmapCharacter(font, *cursor);
	}
}

void DrawText(const float x, const float y, const std::string& text, void* font)
{
	DrawText(x, y, text.c_str(), font);
}

void RenderHud(const int width, const int height, const GameModel& model)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	DrawText(
		24.0f,
		static_cast<float>(height) - 34.0f,
		"3D ARCANOID",
		GLUT_BITMAP_TIMES_ROMAN_24);
	DrawText(
		24.0f,
		static_cast<float>(height) - 70.0f,
		"Level: " + std::to_string(model.GetLevel()),
		GLUT_BITMAP_TIMES_ROMAN_24);
	DrawText(
		24.0f,
		static_cast<float>(height) - 102.0f,
		"Lives: " + std::to_string(model.GetLives()),
		GLUT_BITMAP_TIMES_ROMAN_24);
	DrawText(
		24.0f,
		static_cast<float>(height) - 134.0f,
		"Score: " + std::to_string(model.GetScore()),
		GLUT_BITMAP_TIMES_ROMAN_24);
	DrawText(
		24.0f,
		34.0f,
		"Left/Right or A/D - move   Space - launch   P - pause   R - restart",
		GLUT_BITMAP_TIMES_ROMAN_24);

	float effectY = static_cast<float>(height) - 168.0f;
	for (const auto& [type, timeLeft] : model.GetActiveEffects())
	{
		DrawText(
			24.0f,
			effectY,
			std::string(GetPowerUpName(type))
				+ ": "
				+ std::to_string(static_cast<int>(std::ceil(timeLeft)))
				+ "s",
			GLUT_BITMAP_TIMES_ROMAN_24);
		effectY -= 28.0f;
	}

	if (model.GetState() == GameState::Paused)
	{
		glColor3f(1.0f, 0.85f, 0.25f);
		DrawText(
			static_cast<float>(width) * 0.5f - 45.0f,
			static_cast<float>(height) * 0.5f,
			"PAUSED",
			GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (model.GetState() == GameState::GameOver)
	{
		glColor3f(1.0f, 0.2f, 0.22f);
		DrawText(
			static_cast<float>(width) * 0.5f - 70.0f,
			static_cast<float>(height) * 0.5f,
			"GAME OVER",
			GLUT_BITMAP_TIMES_ROMAN_24);
		glColor3f(1.0f, 1.0f, 1.0f);
		DrawText(
			static_cast<float>(width) * 0.5f - 64.0f,
			static_cast<float>(height) * 0.5f - 34.0f,
			"Press R to restart",
			GLUT_BITMAP_HELVETICA_18);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void DrawTexturedBox(const float width, const float height, const float depth)
{
	const float x = width * 0.5f;
	const float y = height * 0.5f;
	const float z = depth * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-x, -y, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, -y, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-x, y, z);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, -y, -z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-x, y, -z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y, -z);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-x, y, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y, -z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-x, y, -z);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, -y, -z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, -y, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-x, -y, z);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, -y, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, -y, -z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y, -z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y, z);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-x, -y, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-x, y, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-x, y, -z);

	glEnd();
}

void DrawPowerUpSymbol(const PowerUpType type)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.04f, 0.05f, 0.06f);
	glRasterPos3f(-0.11f, -0.05f, 0.24f);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, GetPowerUpName(type)[0]);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void Setup3D(const int width, const int height, const GameModel& model)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const float aspect = height == 0 ? 1.0f : static_cast<float>(width) / static_cast<float>(height);
	gluPerspective(48.0, aspect, 0.1, 80.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		model.GetFieldWidth() * 0.5f,
		model.GetFieldHeight() * 0.18f,
		24.0,
		model.GetFieldWidth() * 0.5f,
		model.GetFieldHeight() * 0.62f,
		0.0,
		0.0,
		1.0,
		0.0);

	constexpr float light0[] = { -4.0f, 9.0f, 12.0f, 1.0f };
	constexpr float light1[] = { 14.0f, 3.0f, 9.0f, 1.0f };
	constexpr float ambient0[] = { 0.22f, 0.22f, 0.24f, 1.0f };
	constexpr float diffuse0[] = { 0.9f, 0.86f, 0.78f, 1.0f };
	constexpr float diffuse1[] = { 0.28f, 0.45f, 0.65f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT1, GL_POSITION, light1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
}

} // namespace

GameView::GameView() = default;

GameView::~GameView()
{
	const GLuint textures[] = {
		m_backgroundTexture,
		m_wallTexture,
		m_paddleTexture,
		m_ballTexture,
		m_brickTexture,
		m_powerUpTexture,
	};
	glDeleteTextures(6, textures);
}

void GameView::Render(const int width, const int height, const GameModel& model)
{
	if (!m_initialized)
	{
		Init();
	}

	glViewport(0, 0, width, height);
	glClearColor(0.03f, 0.04f, 0.055f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Setup3D(width, height, model);
	RenderField(model);
	RenderBricks(model);
	RenderPowerUps(model);
	RenderPaddle(model);
	RenderBalls(model);
	RenderHud(width, height, model);
}

void GameView::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	m_backgroundTexture = TextureBuilder::CreateCheckerTexture(18, 35, 48, 7, 11, 20);
	m_wallTexture = TextureBuilder::CreateTextureFromFile(TexturePath("wall.bmp"));
	if (m_wallTexture == 0)
	{
		m_wallTexture = TextureBuilder::CreateCheckerTexture(65, 75, 82, 28, 35, 44);
	}
	m_paddleTexture = TextureBuilder::CreateTextureFromFile(TexturePath("platform.bmp"));
	if (m_paddleTexture == 0)
	{
		m_paddleTexture = TextureBuilder::CreateStripeTexture(210, 60, 64, 245, 210, 90);
	}
	m_ballTexture = TextureBuilder::CreateTextureFromFile(TexturePath("ball.bmp"));
	if (m_ballTexture == 0)
	{
		m_ballTexture = TextureBuilder::CreateRadialTexture(245, 245, 255, 70, 170, 210);
	}
	m_brickTexture = TextureBuilder::CreateTextureFromFile(TexturePath("block.bmp"));
	if (m_brickTexture == 0)
	{
		m_brickTexture = TextureBuilder::CreateCheckerTexture(210, 62, 84, 150, 35, 64);
	}
	m_powerUpTexture = TextureBuilder::CreateRadialTexture(255, 244, 124, 90, 210, 160);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	m_initialized = true;
}

void GameView::RenderField(const GameModel& model) const
{
	glBindTexture(GL_TEXTURE_2D, m_wallTexture);
	SetColor(0.55f, 0.65f, 0.75f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -0.45f);
	glTexCoord2f(2.0f, 0.0f);
	glVertex3f(model.GetFieldWidth(), 0.0f, -0.45f);
	glTexCoord2f(2.0f, 3.0f);
	glVertex3f(model.GetFieldWidth(), model.GetFieldHeight(), -0.45f);
	glTexCoord2f(0.0f, 3.0f);
	glVertex3f(0.0f, model.GetFieldHeight(), -0.45f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, m_wallTexture);
	SetColor(0.55f, 0.58f, 0.6f);
	glPushMatrix();
	glTranslatef(-0.18f, model.GetFieldHeight() * 0.5f, 0.0f);
	DrawTexturedBox(0.28f, model.GetFieldHeight(), 0.55f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(model.GetFieldWidth() + 0.18f, model.GetFieldHeight() * 0.5f, 0.0f);
	DrawTexturedBox(0.28f, model.GetFieldHeight(), 0.55f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(model.GetFieldWidth() * 0.5f, model.GetFieldHeight() + 0.18f, 0.0f);
	DrawTexturedBox(model.GetFieldWidth() + 0.6f, 0.28f, 0.55f);
	glPopMatrix();
}

void GameView::RenderBricks(const GameModel& model) const
{
	for (const auto& brick : model.GetBricks())
	{
		const float damage = static_cast<float>(brick.hitPoints) / static_cast<float>(brick.maxHitPoints);
		const int palette = std::clamp(brick.maxHitPoints - 1, 0, 3);
		const float brightness = 0.6f + 0.4f * damage;
		constexpr float colors[4][3] = {
			{ 1.00f, 0.32f, 0.36f },
			{ 0.35f, 0.70f, 1.00f },
			{ 0.42f, 0.95f, 0.52f },
			{ 1.00f, 0.78f, 0.28f },
		};
		glBindTexture(GL_TEXTURE_2D, m_brickTexture);
		SetColor(
			colors[palette][0] * brightness,
			colors[palette][1] * brightness,
			colors[palette][2] * brightness);
		glPushMatrix();
		glTranslatef(brick.position.x, brick.position.y, 0.0f);
		DrawTexturedBox(brick.size.x, brick.size.y, OBJECT_DEPTH);
		glPopMatrix();
	}
}

void GameView::RenderBalls(const GameModel& model) const
{
	glBindTexture(GL_TEXTURE_2D, m_ballTexture);
	SetColor(0.8f, 0.95f, 1.0f);
	for (const auto& ball : model.GetBalls())
	{
		glPushMatrix();
		glTranslatef(ball.position.x, ball.position.y, 0.45f);
		glutSolidSphere(ball.radius, 28, 28);
		glPopMatrix();
	}
}

void GameView::RenderPaddle(const GameModel& model) const
{
	const auto& paddle = model.GetPaddle();
	glBindTexture(GL_TEXTURE_2D, m_paddleTexture);
	SetColor(1.0f, 0.75f, 0.35f);
	glPushMatrix();
	glTranslatef(paddle.position.x, paddle.position.y, 0.18f);
	DrawTexturedBox(paddle.width, paddle.height, 0.55f);
	glPopMatrix();
}

void GameView::RenderPowerUps(const GameModel& model) const
{
	glBindTexture(GL_TEXTURE_2D, m_powerUpTexture);
	for (const auto& powerUp : model.GetPowerUps())
	{
		SetColor(0.95f, 0.95f, 0.35f);
		glPushMatrix();
		glTranslatef(powerUp.position.x, powerUp.position.y, 0.35f);
		glutSolidCube(powerUp.size);
		DrawPowerUpSymbol(powerUp.type);
		glPopMatrix();
	}
}