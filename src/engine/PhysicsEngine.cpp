#include "PhysicsEngine.h"

PhysicsObject::PhysicsObject(const Point2& center, float x, float y) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.01f, 0.01f), mass(1.f), transform(0.f, 0.f), collider(235.f, 235.f, 30.f, 30.f) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.01f, 0.01f), mass(mass), transform(0.f ,0.f), collider(235.f, 235.f, 30.f, 30.f) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass, Vector2f transform) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.01f, 0.01f), mass(mass), transform(transform), collider(235.f, 235.f, 30.f, 30.f) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, Vector2f force, float mass, Vector2f transform) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(force), mass(mass), transform(transform), collider(235.f, 235.f, 30.f, 30.f) {}

bool PhysicsObject::isColliding(const PhysicsObject & other) {
    Rectf r1 = { center.x - hlX, center.y - hlY, lX, lY };
    Rectf r2 = { other.center.x - other.hlX, other.center.y - other.hlY, other.lX, other.lY };

	return r1.intersects(r2);
}

void PhysicsObject::applyForce(const Vector2f & v) {
	force = v;
}

void PhysicsObject::applyGravity(const PhysicsEngine & engine) {
	center += engine.gravity;
}

void PhysicsObject::applyAntiGravity(const PhysicsEngine & engine) {
	center -= engine.gravity;
}

void PhysicsObject::setMass(float m) {

	/*if the provided mass is negative or zero, set to smallest positive value for a float.*/
	if (m <= 0.f)
		m = FLT_MIN;
	mass = m;
}

/* PHYSICS ENGINE */

PhysicsEngine::PhysicsEngine() : gravity(Vector2f(0, DEFAULT_GRAVITY)) {}

void PhysicsEngine::setGravity(float val, float interval) {
	gravity = Vector2f(0, val * interval);
}

void PhysicsEngine::registerObject(std::shared_ptr<PhysicsObject> obj) {
	objects.push_back(obj);
}

void PhysicsEngine::update() {
	//called from TestGame::update() to invoke refactored physics behaviour.
}

/*
	Functions utilize Newton's equation F=ma to determine the appropriate value 
	of acceleration to be applied to an object given its mass and a value of force.
*/
float PhysicsEngine::calculateAcceleration_x(Vector2f F, float m) {
	float a = F.x / m;
	return a;
}

float PhysicsEngine::calculateAcceleration_y(Vector2f F, float m) {
	float a = F.y / m;
	return a;
}

/*
	Uses Pythagorean Theorem to calculate the resultant
	value of a vector's x and y components.
*/
float PhysicsEngine::calculateResultant(Vector2f v) {
	float res = (sqrtf(pow(v.x, 2) + pow(v.y, 2)));
	return res;
}
