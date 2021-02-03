#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	
	///CollisionManager::AABBCheck(m_pSpaceShip, m_pObstacle))

	// My hilarious attempts at some sort of real obstacle avoidance
	if (CollisionManager::lineRectCheck(m_pSpaceShip->m_leftWhisker.Start(), m_pSpaceShip->m_leftWhisker.End(), 
		m_pObstacle->getTransform()->position - glm::vec2(75.0f, 100.0f), 150,200))
	{
		m_pSpaceShip->setColliding(true);
		m_pSpaceShip->setRotation(m_pSpaceShip->getRotation()+ 0.3f * Util::Rad2Deg);
		m_pSpaceShip->getRigidBody()->velocity *= 0.0f;
		
	}

	if (CollisionManager::lineRectCheck(m_pSpaceShip->m_rightWhisker.Start(), m_pSpaceShip->m_rightWhisker.End(),
		m_pObstacle->getTransform()->position - glm::vec2(75.0f, 100.0f), 150, 200))
	{
		m_pSpaceShip->setColliding(true);
		m_pSpaceShip->setRotation(m_pSpaceShip->getRotation() - 0.3f * Util::Rad2Deg);
		m_pSpaceShip->getRigidBody()->velocity *= 0.0f;

	}
	
	if (!CollisionManager::lineRectCheck(m_pSpaceShip->m_middleWhisker.Start(), m_pSpaceShip->m_middleWhisker.End(),
		m_pObstacle->getTransform()->position - glm::vec2(75.0f, 100.0f), 150, 200))
	{
		m_pSpaceShip->setColliding(false);
	}
	
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	addChild(m_pTarget);

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(500.0f, 300.0f);
	addChild(m_pObstacle);

	// instantiating spaceship
	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pSpaceShip->setEnabled(false);
	m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pSpaceShip);

	const SDL_Color red = { 255, 0, 0, 255 };
	m_pPlayInstructions = new Label("Use the ( ` ) key and the following buttons to interact", "Consolas", 15, red, glm::vec2(400.0f, 25.0f));
	m_pPlayInstructions->setParent(this);
	addChild(m_pPlayInstructions);

	// Music for PlayScene
	SoundManager::Instance().load("../Assets/audio/punchy_tunes.ogg", "Punchy", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(25);
	SoundManager::Instance().playMusic("Punchy", 0);

	// Sound Effect for Arrow
	SoundManager::Instance().load("../Assets/audio/Fantasy_Grab.wav", "Schwing", SOUND_SFX);

	// Sound Effect for De-Spawning
	SoundManager::Instance().load("../Assets/audio/beep-07.wav", "Beep", SOUND_SFX);

	// Set Volume for sounds
	SoundManager::Instance().setSoundVolume(25);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Assignment_1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float speed = 10.0f;
	if(ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if(ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pSpaceShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pSpaceShip->getRotation();
	if(ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pSpaceShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if(ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pSpaceShip->setTurnRate(turn_rate);
	}
	
	if(ImGui::Button("Seek"))
	{
		m_pSpaceShip->setMode(SEEK);
		m_pSpaceShip->setEnabled(true);
		SoundManager::Instance().playSound("Schwing", 0);
	}

	ImGui::SameLine();

	if (ImGui::Button("Arrive"))
	{
		m_pSpaceShip->setMode(ARRIVE);
		m_pSpaceShip->setEnabled(true);
		SoundManager::Instance().playSound("Schwing", 0);
	}

	ImGui::SameLine();

	if (ImGui::Button("Flee"))
	{
		m_pSpaceShip->setMode(FLEE);
		m_pSpaceShip->setEnabled(true);
		SoundManager::Instance().playSound("Schwing", 0);
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		m_pSpaceShip->setMode(NUM_OF_MODES);
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pSpaceShip->setEnabled(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pSpaceShip->getRotation();
		SoundManager::Instance().playSound("Beep", 0);
	}

	

	

	ImGui::Separator();

	static float targetPosition[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
