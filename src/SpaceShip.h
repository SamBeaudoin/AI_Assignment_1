#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__
#include "DisplayObject.h"
#include "TextureManager.h"

#include "ShipModeType.h"

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

	ShipModeType getMode() const;
	void setMode(const ShipModeType new_mode);

	/*GameObjectType getType() const;
	void setType(GameObjectType new_type);*/
	
	
private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	
	
	//enum m_mode {SEEK, FLEE, ARRIVE};
	ShipModeType m_mode = MODELESS;
	// private function
	void m_Seek();
	void m_Arrive();
	void m_Flee();
};

#endif // defined (__SPACE_SHIP__) 