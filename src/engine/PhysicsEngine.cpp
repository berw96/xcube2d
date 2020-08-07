#include "PhysicsEngine.h"
#include <iostream>

/*PHYSICS OBJECTS*/
PhysicsObject::PhysicsObject(const Point2& center, float x, float y) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), mass(_DEFAULT_INIT_MASS_), transform(_DEFAULT_INIT_TRANSFORM_), radius(_DEFAULT_RADIUS_) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), transform(_DEFAULT_INIT_TRANSFORM_), radius(_DEFAULT_RADIUS_) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass, Vector2f transform) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), transform(transform), radius(_DEFAULT_RADIUS_) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass, Vector2f transform, float radius) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), transform(transform), radius(radius) { setMass(mass); }

void PhysicsObject::setMass(float m) {
	/*if the provided mass is negative or zero, set to smallest positive value for a float.*/
	if (m <= 0.f)
		m = FLT_MIN;
	mass = m;
}

/* PHYSICS ENGINE */
PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::registerObject(std::shared_ptr<PhysicsObject> obj) {
	objects.push_back(obj);
}

#pragma region MECHANICS
/*
	Implements Newtonian Mechnanics (acceleration, velocity and speed)
	to a PhysicsObject instance. The members necessary for its movement
	are calculated and set here.

	Mechanics puts a PhysicsObject in a state of
	de facto movement, which comprises of an acceleration
	parameter which can be affected by forces (e.g. gravity).

	Positive forces move the instance down or right, where
	negative forces move it up or left, depending on the
	respective axis.

	@param PhysicsObject
*/
void PhysicsEngine::mechanics(PhysicsObject & po) {
	po.acceleration.x = calculateAcceleration_x(po);
	po.acceleration.y = calculateAcceleration_y(po);

	po.speed.x += po.acceleration.x;
	po.speed.y += po.acceleration.y;

	po.velocity.x = po.speed.x;
	po.velocity.y = po.speed.y;

	po.transform.x += po.velocity.x;
	po.transform.y += po.velocity.y;

	po.center.x = po.transform.x - po.hlX;
	po.center.y = po.transform.y - po.hlY;
}

/*
	Functions utilize Newton's equation F=ma to determine the appropriate value
	of acceleration to be applied to an object given its mass and a value of force.
*/
float PhysicsEngine::calculateAcceleration_x(PhysicsObject& po) {
	float a = po.getForce().x / po.getMass();
	return a;
}

float PhysicsEngine::calculateAcceleration_y(PhysicsObject& po) {
	float a = po.getForce().y / po.getMass();
	return a;
}

/*
	Calculates the momentum of a PhysicsObject and returns it as a vector.
*/
Vector2f PhysicsEngine::calculateMomentum(PhysicsObject& po) {
	Vector2f mv(po.getMass() * po.getVelocity().x, po.getMass() * po.getVelocity().y);
	return mv;
}

/*
	Takes a collection of PhysicsObjects and calculates the net force
	created between them. This is achieved by adding each object's force
	vector onto the netForce, and returning the netForce vector.

	This can be assigned to the force variable of each instance.
*/
Vector2f PhysicsEngine::calculateNetForce(std::vector<PhysicsObject> objects) {
	Vector2f netForce;
	for (auto obj : objects) {
		netForce.x += obj.force.x;
		netForce.y += obj.force.y;
	}
	return netForce;
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
float PhysicsEngine::calculateRange(PhysicsObject& target, PhysicsObject& object) {
	Vector2f range(calculateRange_x(target, object), calculateRange_y(target, object));

	float res_range = calculateResultant(range);
	return abs(res_range);
}

/*
	Functions for calculating the axial ranges between PhysicsObjects.
*/
float PhysicsEngine::calculateRange_x(PhysicsObject& target, PhysicsObject& object) {
	float rx = (target.getTransform().x - object.getTransform().x);
	return rx;
}

float PhysicsEngine::calculateRange_y(PhysicsObject& target, PhysicsObject& object) {
	float ry = (target.getTransform().y - object.getTransform().y);
	return ry;
}

/*
	Calculates the gravitational force between objects based on their RESULTANT RANGE.

	Multiplies mass and the set universal constant of
	gravitation, divides by the range between the two PhysicsObjects
	(squared) on EACH AXIS and assigns the result to the return value Vector2f.

	The closer the PhysicsObjects, the stronger the force between them.
	By virtue this means that if the range between the two gravitating
	PhysicsObjects is infinitely small then the force between them and
	the ensuing acceleration would be infinitely large - like a blackhole.
*/
Vector2f PhysicsEngine::calculateGravitationalForce(PhysicsObject& target, PhysicsObject& object) {
	float gravitationalForce = ((target.getMass() * object.getMass() * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(target, object), 2));

	Vector2f object_to_target ((calculateRange_x(target, object) * gravitationalForce), calculateRange_y(target, object) * gravitationalForce);

	//Gravity is applied identically across both axis'.
	return object_to_target;
}
#pragma endregion

