#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__
#define _UNIVERSAL_CONST_GRAVITATION_ (0.01f)
#define _DEFAULT_INIT_MASS_ (1.f)
#define _DEFAULT_INIT_TRANSFORM_ (Vector2f(0.f, 0.f))
#define _DEFAULT_RADIUS_ (10.f)

#include <vector>
#include <string>
#include <memory>

#include "GameMath.h"

class PhysicsObject;

class PhysicsEngine {
	friend class XCube2Engine;
	friend class PhysicsObject;
	private:
#pragma region IMPLEMENTATION
		PhysicsEngine();
		PhysicsEngine(PhysicsObject & objects);
		std::vector<PhysicsObject> objects;

	public:
		std::vector<PhysicsObject> getObjects() { return objects; };
		void registerObject(PhysicsObject & obj);
		void mechanics(PhysicsObject & po);
#pragma endregion

#pragma region FORMULA
		float calculateResultant(Vector2f v);
		float calculateRange(PhysicsObject& a, PhysicsObject& b);
		void calculateOrbitalPeriod(PhysicsObject& a, PhysicsObject& b);
		void calculateRequiredVelocity(PhysicsObject& a, PhysicsObject& b);
		void calculateMomentum(PhysicsObject& po);
		Vector2f calculateGravitationalForce(PhysicsObject& target, PhysicsObject& satellite);
		Vector2f calculateGravitationalForce(PhysicsObject& satellite);
		void calculateNetForce(PhysicsObject& a, PhysicsObject& b, PhysicsObject& c);
#pragma endregion
};

class PhysicsObject {
	//Gives the PhysicsEngine access to the private and protected members of PhysicsObject.
	friend class PhysicsEngine;
	protected:
		std::string tag;
#pragma region FIELDS
		Point2 center;
		Vector2f transform;
		Vector2f netForce;
		Vector2f gravitationalForce;
		Vector2f boostForce;
		Vector2f acceleration;
		Vector2f velocity;
		Vector2f speed;
		Vector2f momentum;
		Vector2f reqVelocity;
		float mass;
		float period;
		float radius;
#pragma endregion

	public:
#pragma region CONSTRUCTORS
		PhysicsObject(const Point2 & center);
		PhysicsObject(const Point2 & center, float mass);
		PhysicsObject(const Point2 & center, float mass, Vector2f transform);
		PhysicsObject(const Point2 & center, float mass, Vector2f transform, float radius);
		PhysicsObject(const Point2 & center, float mass, Vector2f transform, float radius, std::string tag);
		PhysicsObject(const Point2 & center, float mass, Vector2f transform, float radius, std::string tag, Vector2f boostForce);
#pragma endregion

#pragma region GETTERS
		std::string getTag()					{ return tag; }
		Point2 getCenter()						{ return center; }
		Vector2f getTransform()					{ return transform; }
		Vector2f getNetForce()					{ return netForce; }
		Vector2f getGravitationalForce()		{ return gravitationalForce; }
		Vector2f getBoostForce()				{ return boostForce; }
		Vector2f getAcceleration()				{ return acceleration; }
		Vector2f getVelocity()					{ return velocity; }
		Vector2f getSpeed()						{ return speed; }
		Vector2f getMomentum()					{ return momentum; }
		Vector2f getRequiredVelocity()			{ return reqVelocity; }
		float getMass()							{ return mass; }
		float getPeriod()						{ return period; }
		float getRadius()						{ return radius; }
#pragma endregion

#pragma region SETTERS
		void setMass(float m);
		void setTag(std::string tag)				{ this->tag = tag; }
		void setBoostForce_X(float boostForce_X)	{ this->boostForce.x = boostForce_X; }
		void setBoostForce_Y(float boostForce_Y)	{ this->boostForce.y = boostForce_Y; }
#pragma endregion
};

#endif
