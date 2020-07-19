#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__
#define _UNIVERSAL_CONST_GRAVITATION_ (100.f)

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
		void update();

		void registerObject(std::shared_ptr<PhysicsObject>);
#pragma region FORMULA
		float calculateAcceleration_x(Vector2f F, float m);
		float calculateAcceleration_y(Vector2f F, float m);
		Vector2f calculateMomentum(float m, Vector2f v);
		float calculateResultant(Vector2f v);
		float calculateRange(PhysicsObject & a, PhysicsObject & b);
		Vector2f calculuateGravitationalForce(PhysicsObject & a, PhysicsObject & b);
#pragma endregion
};

class PhysicsObject {
	friend class PhysicsEngine;
	protected:
		Point2 center;
		Rectf collider;
		float lX, lY, hlX, hlY;	// lengths and half lengths

		/*Variables which affect how a physics object reacts to player input*/
		Vector2f transform;
		Vector2f force;
		Vector2f acceleration;
		Vector2f velocity;
		Vector2f speed;
		float mass;
		Vector2f momentum;
		bool autoMove;

	public:
		PhysicsObject(const Point2 & center, float x, float y);
		PhysicsObject(const Point2 & center, float x, float y, float mass);
		PhysicsObject(const Point2 & center, float x, float y, float mass, Vector2f transform);
		PhysicsObject(const Point2 & center, float x, float y, float mass, Vector2f transform, Rectf collider);
		PhysicsObject(const Point2 & center, float x, float y, Vector2f force, float mass, Vector2f transform);
		PhysicsObject(const Point2 & center, float x, float y, Vector2f force, float mass, Vector2f transform, Rectf collider);

#pragma region GETTERS
		Point2 getCenter()						{ return center; }
		Rectf getCollider()						{ return collider; }
		float getLengthX()						{ return lX; }
		float getLengthY()						{ return lY; }
		float getHalfLengthX()					{ return hlX; }
		float getHalfLengthY()					{ return hlY; }

		Vector2f getRootTransform()				{ return transform; }
		Vector2f getForce()						{ return force; }
		Vector2f getAcceleration()				{ return acceleration; }
		Vector2f getVelocity()					{ return velocity; }
		Vector2f getSpeed()						{ return speed; }
		float getMass()							{ return mass; }
#pragma endregion

#pragma region SETTERS
		void setColliderTransform_X(Vector2f t) { collider.x = t.x; }
		void setColliderTransform_Y(Vector2f t) { collider.y = t.y; }
		void setColliderSize(Vector2f s)		{ collider.w = s.x; collider.h = s.y; }
		void setColliderSize_X(float s)			{ collider.w = s; }
		void setColliderSize_Y(float s)			{ collider.h = s; }
		void setRootTransform(Vector2f t)		{ transform = t; }
		void setRootTransform_X(float t)		{ transform.x += t; }
		void setRootTransform_Y(float t)		{ transform.y += t; }
		void setForce(Vector2f f)				{ force = f; }
		void setForce_X(Vector2f f)				{ force.x = f.x; }
		void setForce_Y(Vector2f f)				{ force.y = f.y; }
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

		bool isColliding(const PhysicsObject & other);
		void moveTo(Vector2f destination);
		void moveTo(Vector2f destination, Vector2f f);
};

#endif