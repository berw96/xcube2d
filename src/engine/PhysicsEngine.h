#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__
#define _UNIVERSAL_CONST_GRAVITATION_

#include <vector>
#include <memory>

#include "GameMath.h"

static const float DEFAULT_GRAVITY = -1.0f;

class PhysicsObject;

class PhysicsEngine {
	friend class XCube2Engine;
	friend class PhysicsObject;
	private:
		Vector2f gravity;
		bool gravityToggle;
		PhysicsEngine();

		std::vector<std::shared_ptr<PhysicsObject>> objects;

	public:
		/**
		* Note that gravity is naturally a negative value
		* update interval in seconds
		*/
		void setGravity(float gravityValue, float worldUpdateInterval);
		void update();

		void registerObject(std::shared_ptr<PhysicsObject>);

		float calculateAcceleration_x(Vector2f F, float m);
		float calculateAcceleration_y(Vector2f F, float m);
		float calculateResultant(Vector2f v);
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
		bool canMove;

		std::vector<std::shared_ptr<PhysicsObject>> children;
		void registerChild(std::shared_ptr<PhysicsObject>);

		void applyForce(const Vector2f &);
	public:
		PhysicsObject(const Point2 & center, float x, float y);
		PhysicsObject(const Point2 & center, float x, float y, float mass);
		PhysicsObject(const Point2 & center, float x, float y, float mass, Vector2f transform);
		PhysicsObject(const Point2 & center, float x, float y, float mass, Vector2f transform, Rectf collider);
		PhysicsObject(const Point2 & center, float x, float y, Vector2f force, float mass, Vector2f transform);
		PhysicsObject(const Point2 & center, float x, float y, Vector2f force, float mass, Vector2f transform, Rectf collider);

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

		void setColliderTransform_X(Vector2f t) { collider.x = t.x; }
		void setColliderTransform_Y(Vector2f t) { collider.y = t.y; }
		void setColliderSize(Vector2f s)		{ collider.w = s.x; collider.h = s.y; }
		void setColliderSize_X(float s)			{ collider.w = s; }
		void setColliderSize_Y(float s)			{ collider.h = s; }
		void setRootTransform(Vector2f t)		{ transform = t; }
		void setRootTransform_X(float t)		{ transform.x += t; }
		void setRootTransform_Y(float t)		{ transform.y += t; }
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

		bool isColliding(const PhysicsObject & other);
		/**
		* If we have different implementations of engines/gravity
		* this can be very useful
		*/
		virtual void applyGravity(const PhysicsEngine & engine);
		virtual void applyAntiGravity(const PhysicsEngine & engine);
};

#endif