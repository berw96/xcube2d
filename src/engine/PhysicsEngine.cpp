#include "PhysicsEngine.h"
#include <iostream>

PhysicsObject::PhysicsObject(const Point2& center, float x, float y) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.01f, 0.01f), mass(1.f), transform(0.f, 0.f), collider(235.f, 235.f, 30.f, 30.f) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.01f, 0.01f), mass(mass), transform(0.f ,0.f), collider(235.f, 235.f, 30.f, 30.f) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass, Vector2f transform) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.01f, 0.01f), mass(mass), transform(transform), collider(235.f, 235.f, 30.f, 30.f) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass, Vector2f transform, Rectf collider) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.01f, 0.01f), mass(mass), transform(transform), collider(collider) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, Vector2f force, float mass, Vector2f transform) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(force), mass(mass), transform(transform), collider(235, 235.f, 30.f, 30.f) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, Vector2f force, float mass, Vector2f transform, Rectf collider) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(force), mass(mass), transform(transform), collider(collider) {}

bool PhysicsObject::isColliding(const PhysicsObject & other) {
    Rectf r1 = { center.x - hlX, center.y - hlY, lX, lY };
    Rectf r2 = { other.center.x - other.hlX, other.center.y - other.hlY, other.lX, other.lY };

	return r1.intersects(r2);
}

void PhysicsObject::setMass(float m) {

	/*if the provided mass is negative or zero, set to smallest positive value for a float.*/
	if (m <= 0.f)
		m = FLT_MIN;
	mass = m;
}

/*
	Shifts the transform of the PhysicsObject to a given destination.
	Once it reaches the destination it stops.

	One overload for linear movement, a second for gravitation.
*/
void PhysicsObject::moveTo(Vector2f destination) {
	if (destination.x != getRootTransform().x &&
		destination.y != getRootTransform().y) {
		if (destination.x > getRootTransform().x) {
			//move RIGHT
			setSpeed_X(1.f);
		}

		if (destination.x < getRootTransform().x) {
			//move LEFT
			setSpeed_X(-1.f);
		}

		if (destination.y > getRootTransform().y) {
			//move DOWN
			setSpeed_Y(1.f);
		}

		if (destination.y < getRootTransform().y) {
			//move UP
			setSpeed_Y(-1.f);
		}
	}
	else {
		setSpeed(Vector2f(0.f, 0.f));
	}
}

void PhysicsObject::moveTo(Vector2f destination, Vector2f f){
	if (destination.x != getRootTransform().x &&
		destination.y != getRootTransform().y) {
		if (destination.x > getRootTransform().x) {
			//move RIGHT
			setSpeed_X(getSpeed().x + getAcceleration().x);
		}

		if (destination.x < getRootTransform().x) {
			//move LEFT
			setSpeed_X(getSpeed().x - getAcceleration().x);
		}

		if (destination.y > getRootTransform().y) {
			//move DOWN
			setSpeed_Y(getSpeed().y + getAcceleration().y);
		}

		if (destination.y < getRootTransform().y) {
			//move UP
			setSpeed_Y(getSpeed().y - getAcceleration().y);
		}
	}
	else {
		setSpeed(Vector2f(0.f, 0.f));
	}
}

/* PHYSICS ENGINE */

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::registerObject(std::shared_ptr<PhysicsObject> obj) {
	objects.push_back(obj);
}

void PhysicsEngine::update() {
	/*
		Called from AbstractGame::updatePhysics(), to invoke refactored physics behaviour.
	*/
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

Vector2f PhysicsEngine::calculateMomentum(float m, Vector2f v) {
	Vector2f mv (m * v.x, m * v.y);
	return mv;
}

/*
	Uses Pythagorean Theorem to calculate the resultant
	value of a vector's x and y components.
*/
float PhysicsEngine::calculateResultant(Vector2f v) {
	float res = (sqrtf(pow(v.x, 2) + pow(v.y, 2)));
	return res;
}

/*
	Creates a new Vector2f instance containing the displacement
	between PhysicsObjects a and b on each axis, assigns the
	resultant to a float and returns its modulus.
*/
float PhysicsEngine::calculateRange(PhysicsObject & a, PhysicsObject & b) {
	Vector2f AB (
		a.getRootTransform().x - b.getRootTransform().x,
		a.getRootTransform().y - b.getRootTransform().y);

	float range = calculateResultant(AB);
	/*if the range is negative or zero, set to smallest positive value for a float.*/
	if (range <= 0.f) {
		range = FLT_MIN;
	}
	return abs(range);
}

/*
	Calculates the product of mass and the set universal constant of
	gravitation, divides by the range between the two PhysicsObjects
	(squared) and assigns the result to the return value Vector2f.
*/
Vector2f PhysicsEngine::calculuateGravitationalForce(PhysicsObject & a, PhysicsObject & b){
	float gForce = (a.getMass() * b.getMass() * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(a, b), 2);
	Vector2f gVector(gForce, gForce);
	
	return gVector;
}
