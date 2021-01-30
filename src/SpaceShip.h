#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__
#include "DisplayObject.h"
#include "Line.h"	// Used to create Whiskers
#include "TextureManager.h"

#include "ShipModeType.h"	// Used to determine flight mode

class SpaceShip final : public DisplayObject
{
public:
	
	// constructor(s)
	SpaceShip();

	// destructor
	~SpaceShip();
	
	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	void setDestination(glm::vec2 destination);
	void setMaxSpeed(float speed);
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);

	void setColliding(bool status);				// setter and getter for collision checks
	bool getColliding();

	ShipModeType getMode() const;				// Getting flight modes
	void setMode(const ShipModeType new_mode);	// Setting flight modes

	void setWhisker(Line whisker, glm::vec2 start, glm::vec2 end);	//Setting whiskers start and end
	
private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	bool m_isColliding = false;			// Collision Boolean

	friend class PlayScene;
	
	// Whisker objects for individual collision 
	Line m_leftWhisker, m_rightWhisker, m_middleWhisker;
	
	//enum m_mode {SEEK, FLEE, ARRIVE};
	ShipModeType m_mode = MODELESS;
	
	// private function
	void m_Seek();
	void m_Arrive();
	void m_Flee();
};

#endif // defined (__SPACE_SHIP__) 