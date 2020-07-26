#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__
#define _UNIVERSAL_CONST_GRAVITATION_ (0.01f)
#define _DEFAULT_FORCE_ (Vector2f(0.01f, 0.01f))
#define _DEFAULT_MASS_ (1.f)
#define _DEFAULT_TRANSFORM_ (Vector2f(0.f, 0.f))
#define _DEFAULT_COLLIDER_ (Rectf(235.f, 235.f, 30.f, 30.f))

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
		void setMovable(PhysicsObject& po, bool b);
		void collision(PhysicsObject& a, PhysicsObject & b);
		void push(PhysicsObject& a, PhysicsObject & b);

#pragma region FORMULA
		float calculateAcceleration_x(PhysicsObject & po);
		float calculateAcceleration_y(PhysicsObject & po);
		Vector2f calculateMomentum(PhysicsObject & po);
		float calculateResultant(Vector2f v);
		float calculateRange(PhysicsObject & a, PhysicsObject & b);
		Vector2f calculuateGravitationalForce(PhysicsObject & a, PhysicsObject & b);
		Vector2f calculateResultantVelocity(PhysicsObject & po, float t);
#pragma endregion
};

class PhysicsObject {
	//Gives the PhysicsEngine access to the private and protected members of PhysicsObject.
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
		bool autoMove;
		bool isChild;
		std::vector<PhysicsObject> children;

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
		PhysicsObject getChild(int i)			{ return children[i]; }
#pragma endregion

#pragma region SETTERS
		void setColliderTransform(Vector2f t)	{ collider.x = t.x; collider.y = t.y; }
		void setColliderTransform_X(float t)	{ collider.x = t; }
		void setColliderTransform_Y(float t)	{ collider.y = t; }
		void setColliderSize(Vector2f s)		{ collider.w = s.x; collider.h = s.y; }
		void setColliderSize_X(float s)			{ collider.w = s; }
		void setColliderSize_Y(float s)			{ collider.h = s; }
		/*The axial setRootTransform functions increment to implement movement.*/
		void setRootTransform(Vector2f t)		{ transform = t; }
		void setRootTransform_X(float t)		{ transform.x += t; }
		void setRootTransform_Y(float t)		{ transform.y += t; }
		void setForce(Vector2f f)				{ force = f; }
		void setForce_X(float f)				{ force.x = f; }
		void setForce_Y(float f)				{ force.y = f; }
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

		void moveTo(Vector2f destination);
		void registerChild(PhysicsObject & po);
		void forgetChildren();
};

#endif