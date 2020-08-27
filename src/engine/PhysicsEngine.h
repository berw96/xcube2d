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
		std::vector<std::shared_ptr<PhysicsObject>> objects;

	public:
		void registerObject(std::shared_ptr<PhysicsObject> obj);
		void mechanics();
#pragma endregion

#pragma region FORMULA
		Vector2f calculateGravitationalForce(PhysicsObject& target, PhysicsObject& satellite);
		Vector2f calculateNetGravitationalForce(PhysicsObject& satellite);
		float calculateResultant(Vector2f v);
		float calculateRange(PhysicsObject& a, PhysicsObject& b);
		void calculateOrbitalPeriod(PhysicsObject& a, PhysicsObject& b);
		void calculateRequiredVelocity(PhysicsObject& a, PhysicsObject& b);
		void calculateMomentum(PhysicsObject& po);
		void calculateNetForce(PhysicsObject & po);
#pragma endregion
};

class PhysicsObject {
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
		std::string getTag()					const { return tag; }
		Point2 getCenter()						const { return center; }
		Vector2f getTransform()					const { return transform; }
		Vector2f getNetForce()					const { return netForce; }
		Vector2f getGravitationalForce()		const { return gravitationalForce; }
		Vector2f getBoostForce()				const { return boostForce; }
		Vector2f getAcceleration()				const { return acceleration; }
		Vector2f getVelocity()					const { return velocity; }
		Vector2f getSpeed()						const { return speed; }
		Vector2f getMomentum()					const { return momentum; }
		Vector2f getRequiredVelocity()			const { return reqVelocity; }
		float getMass()							const { return mass; }
		float getPeriod()						const { return period; }
		float getRadius()						const { return radius; }
#pragma endregion

#pragma region SETTERS
		void setMass(float m);
		void setTag(std::string tag)				{ this->tag = tag; }
		void setBoostForce_X(float boostForce_X)	{ this->boostForce.x = boostForce_X; }
		void setBoostForce_Y(float boostForce_Y)	{ this->boostForce.y = boostForce_Y; }
#pragma endregion
};

#endif
