#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__
#define _UNIVERSAL_CONST_GRAVITATION_ (0.1f)
#define _DEFAULT_INIT_MASS_ (1.f)
#define _DEFAULT_INIT_TRANSFORM_ (Vector2f(0.f, 0.f))
#define _DEFAULT_RADIUS_ (10.f)

#include <vector>
#include <memory>

#include "GameMath.h"

class PhysicsObject;

class PhysicsEngine {
	friend class XCube2Engine;
	friend class PhysicsObject;
	private:
		PhysicsEngine();
		std::vector<std::shared_ptr<PhysicsObject>> objects;

	public:
		void registerObject(std::shared_ptr<PhysicsObject>);
		void mechanics(PhysicsObject & po);

#pragma region FORMULA
		float calculateAcceleration_x(PhysicsObject & po);
		float calculateAcceleration_y(PhysicsObject & po);
		float calculateResultant(Vector2f v);
		float calculateRange(PhysicsObject & a, PhysicsObject & b);
		float calculateRange_x(PhysicsObject & a, PhysicsObject & b);
		float calculateRange_y(PhysicsObject & a, PhysicsObject & b);
		Vector2f calculateGravitationalForce(PhysicsObject& a, PhysicsObject& b);
		Vector2f calculateMomentum(PhysicsObject & po);
		Vector2f calculateNetForce(std::vector<PhysicsObject> objects);
#pragma endregion
};

class PhysicsObject {
	//Gives the PhysicsEngine access to the private and protected members of PhysicsObject.
	friend class PhysicsEngine;
	protected:
		Point2 center;

		/*Variables which affect how a physics object reacts to player input*/
		Vector2f transform;
		Vector2f force;
		Vector2f acceleration;
		Vector2f velocity;
		Vector2f speed;

		float mass;
		float period;
		float radius;
		float lX, lY, hlX, hlY;	// lengths and half lengths

	public:
		PhysicsObject(const Point2 & center, float x, float y);
		PhysicsObject(const Point2 & center, float x, float y, float mass);
		PhysicsObject(const Point2 & center, float x, float y, float mass, Vector2f transform);
		PhysicsObject(const Point2 & center, float x, float y, float mass, Vector2f transform, float radius);

#pragma region GETTERS
		Point2 getCenter()						{ return center; }
		float getRadius()						{ return radius; }
		float getLengthX()						{ return lX; }
		float getLengthY()						{ return lY; }
		float getHalfLengthX()					{ return hlX; }
		float getHalfLengthY()					{ return hlY; }

		Vector2f getTransform()				{ return transform; }
		Vector2f getForce()						{ return force; }
		Vector2f getAcceleration()				{ return acceleration; }
		Vector2f getVelocity()					{ return velocity; }
		Vector2f getSpeed()						{ return speed; }
		float getMass()							{ return mass; }
#pragma endregion

#pragma region SETTERS
		void setTransform(Vector2f t)		{ transform = t; }
		void setTransform_X(float t)		{ transform.x = t; }
		void setTransform_Y(float t)		{ transform.y = t; }

		void setForce(Vector2f f)				{ force = f; }
		void setForce_X(float f)				{ force.x = f; }
		void setForce_Y(float f)				{ force.y = f; }
		void incrementForce_X(float f)			{ force.x += f; }
		void incrementForce_Y(float f)			{ force.y += f; }

		void setAcceleration(Vector2f a)		{ acceleration = a; }
		void setAcceleration_X(float a)			{ acceleration.x = a; }
		void setAcceleration_Y(float a)			{ acceleration.y = a; }

		void setVelocity(Vector2f v)			{ velocity = v; }
		void setVelocity_X(float v)				{ velocity.x = v; }
		void setVelocity_Y(float v)				{ velocity.y = v; }

		void setSpeed(Vector2f s)				{ speed = s; }
		void setSpeed_X(float s)				{ speed.x = s; }
		void setSpeed_Y(float s)				{ speed.y = s; }

		void setMass(float m);
#pragma endregion
};

#endif