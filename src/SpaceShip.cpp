#include "SpaceShip.h"


#include "Game.h"
#include "Util.h"

SpaceShip::SpaceShip()
{
	TextureManager::Instance()->load("../Assets/textures/arrow.png", "spaceship");

	auto size = TextureManager::Instance()->getTextureSize("spaceship");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SPACE_SHIP);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);

	setMode(NUM_OF_MODES);
	
}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw()
{
	TextureManager::Instance()->draw("spaceship", 
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	// Commented out Debug line
	//Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 60.0f) );
	Util::DrawLine(m_leftWhisker.Start(), m_leftWhisker.End());
	Util::DrawLine(m_rightWhisker.Start(), m_rightWhisker.End());
	Util::DrawLine(m_middleWhisker.Start(), m_middleWhisker.End());
}

void SpaceShip::update()
{
	m_middleWhisker.setLine(this->getTransform()->position,
		this->getTransform()->position + Util::getOrientation(m_rotationAngle) * 100.0f);
	m_leftWhisker.setLine(this->getTransform()->position,
		this->getTransform()->position + Util::getOrientation(m_rotationAngle - 30) * 50.0f);
	m_rightWhisker.setLine(this->getTransform()->position,
		this->getTransform()->position + Util::getOrientation(m_rotationAngle + 30) * 50.0f);
	if (this->getMode() == SEEK)
	{
		m_Seek();
	}
	else if (this->getMode() == ARRIVE) 
	{
		m_Arrive();
	}
	else if (this->getMode() == FLEE) 
	{
		m_Flee();
	}
	/*switch (m_mode) //	Using switch Case changes speed and acceleration?
	{ 				  //	Possibly because of delta time being changed? IDK?
	
	case SEEK:
		m_Seek();

	case ARRIVE:
		m_Arrive();
		
	case FLEE:
		m_Flee();
	}
	*/
}

void SpaceShip::clean()
{
}

ShipModeType SpaceShip::getMode() const
{
	return m_mode;
}

void SpaceShip::setMode(const ShipModeType new_mode)
{
	m_mode = new_mode;
}

void SpaceShip::setWhisker(Line whisker, glm::vec2 start, glm::vec2 end)
{
	whisker.setLine(start, end);
}

void SpaceShip::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void SpaceShip::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

glm::vec2 SpaceShip::getOrientation() const
{
	return m_orientation;
}

float SpaceShip::getTurnRate() const
{
	return m_turnRate;
}

void SpaceShip::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float SpaceShip::getAccelerationRate() const
{
	return m_accelerationRate;
}

void SpaceShip::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void SpaceShip::setColliding(bool status)
{
	m_isColliding = status;
}

bool SpaceShip::getColliding()
{
	return m_isColliding;
}

void SpaceShip::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}

void SpaceShip::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float SpaceShip::getRotation() const
{
	return m_rotationAngle;
}

void SpaceShip::m_Flee()
{
	std::cout << "Fleeing!" << std::endl;
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;

	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	
	// Multiplied Orientation by -1 to reversi swap to exact opposite of target
	auto target_rotation = Util::signedAngle((getOrientation() * -1.0f), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
	}

	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}

void SpaceShip::m_Arrive()
{
	std::cout << "Arriving!" << std::endl;
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;

	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
	}

	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;

	//ToDo: Add arrival radius checks
}


void SpaceShip::m_Seek()
{
	std::cout << "Seeking!" << std::endl;
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;
	
	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;
	
	if (!getColliding())		// to eliminate jittery-ness, lol IDK how to do it
	{
		if (abs(target_rotation) > turn_sensitivity)
		{
			if (target_rotation > 0.0f)
			{
				setRotation(getRotation() + getTurnRate());
			}
			else if (target_rotation < 0.0f)
			{
				setRotation(getRotation() - getTurnRate());
			}
		}
	}

	
	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}
