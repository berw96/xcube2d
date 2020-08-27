#include "PhysicsEngine.h"
#include <iostream>

/*PHYSICS OBJECTS*/
PhysicsObject::PhysicsObject(const Point2& center) : center(center), mass(_DEFAULT_INIT_MASS_), transform(_DEFAULT_INIT_TRANSFORM_), radius(_DEFAULT_RADIUS_) {}
PhysicsObject::PhysicsObject(const Point2& center, float mass) : center(center), transform(_DEFAULT_INIT_TRANSFORM_), radius(_DEFAULT_RADIUS_) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform) : center(center), transform(transform), radius(_DEFAULT_RADIUS_) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform, float radius) : center(center), transform(transform), radius(radius) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform, float radius, std::string tag) : center(center), transform(transform), radius(radius), tag(tag) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform, float radius, std::string tag, Vector2f boostForce) : center(center), transform(transform), radius(radius), tag(tag), boostForce(boostForce) { setMass(mass); }

void PhysicsObject::setMass(float m) {
	if (m <= 0.f)
		m = FLT_MIN;
	mass = m;
}

/* PHYSICS ENGINE */
PhysicsEngine::PhysicsEngine() {}

/*
	Registers a shared pointer of a PhysicsObject with the PhysicsEngine.
*/
void PhysicsEngine::registerObject(std::shared_ptr<PhysicsObject> obj) {
	objects.push_back(obj);
}

#pragma region MECHANICS
/*
	Implements Newton's 2nd Law of Physics which all instances of
	PhysicsObject registered with the PhysicsEngine will experience.

	Force, acceleration, speed, velocity and mass are all applied to
	the PhysicsObjects.
*/
void PhysicsEngine::mechanics() {
	for (auto obj: objects) {
		calculateNetForce(obj);

		obj->acceleration.x = obj->netForce.x / obj->mass;
		obj->acceleration.y = obj->netForce.y / obj->mass;

		obj->speed.x += obj->acceleration.x;
		obj->speed.y += obj->acceleration.y;

		obj->velocity.x = obj->speed.x;
		obj->velocity.y = obj->speed.y;

		obj->transform.x += obj->velocity.x;
		obj->transform.y += obj->velocity.y;

		obj->center.x = obj->transform.x;
		obj->center.y = obj->transform.y;
	}
}

/*
	Calculates the time which a satellite will take to complete a full orbit
	of a given target. The result is calculated for the distance at any
	given time/tick.
*/
void PhysicsEngine::calculateOrbitalPeriod(std::shared_ptr<PhysicsObject> satellite, std::shared_ptr<PhysicsObject> target) {
	satellite->period = sqrtf(2 * PI * pow(calculateRange(satellite,target), 3)/_UNIVERSAL_CONST_GRAVITATION_ * satellite->mass);
}

/*
	Calculates the velocity required for a satellite to orbit around a target
	at a constant distance. The result is calculated for the distance at any
	given time/tick.
*/
void PhysicsEngine::calculateRequiredVelocity(std::shared_ptr<PhysicsObject> satellite, std::shared_ptr<PhysicsObject> target) {
	float reqVelocity = sqrtf(_UNIVERSAL_CONST_GRAVITATION_ * target->mass / calculateRange(satellite,target));

	satellite->reqVelocity = Vector2f(	(target->transform.x - satellite->transform.x) * reqVelocity,
										(target->transform.y - satellite->transform.y) * reqVelocity);
}

/*
	Calculates the resultant (hypotenuse) of a given vector using
	Pythagorean Theorem.
*/
float PhysicsEngine::calculateResultant(Vector2f v) {
	float res = (sqrtf(pow(v.x, 2) + pow(v.y, 2)));
	return res;
}

/*
	Calculates the resultant range between two instance of PhysicsObject.
*/
float PhysicsEngine::calculateRange(std::shared_ptr<PhysicsObject> target, std::shared_ptr<PhysicsObject> satellite) {
	Vector2f range(	target->transform.x - satellite->transform.x, 
					target->transform.y - satellite->transform.y);

	float res_range = calculateResultant(range);
	if (res_range == 0.f)
		res_range = FLT_MIN;

	return abs((float)res_range);
}

/*
	Calculates the net gravitational force exerted on a given instance
	of PhysicsObject by all others using Newton's Law of Universal Gravitation.
*/
Vector2f PhysicsEngine::calculateNetGravitationalForce(std::shared_ptr<PhysicsObject> satellite) {
	float gravitationalForce = 0.f;
	Vector2f satellite_to_target (0.f, 0.f);
	for (auto t : objects) {
		if (t != satellite) {
			//calculates gravity between two instances
			gravitationalForce = ((t->mass * satellite->mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(t, satellite), 2));
			//increments the total gravity experienced by the satellite by this value
			satellite_to_target.x += (t->transform.x - satellite->transform.x) * gravitationalForce;
			satellite_to_target.y += (t->transform.y - satellite->transform.y) * gravitationalForce;
		}
	}
	return satellite_to_target;
}

/*
	Summates all the forces which an instance of PhysicsObject experiences.
	The net force of an object is considered as the actual force applied to it
	at all times.
*/
void PhysicsEngine::calculateNetForce(std::shared_ptr<PhysicsObject> po) {
	po->netForce.x = calculateNetGravitationalForce(po).x + po->boostForce.x;
	po->netForce.y = calculateNetGravitationalForce(po).y + po->boostForce.y;
}
#pragma endregion
