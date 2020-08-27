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

void PhysicsEngine::registerObject(std::shared_ptr<PhysicsObject> obj) {
	objects.push_back(obj);
}

#pragma region MECHANICS
void PhysicsEngine::mechanics() {
	for (auto obj: objects) {
		calculateNetForce(*obj);

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

void PhysicsEngine::calculateMomentum(PhysicsObject& po) {
	po.momentum = Vector2f(po.getMass() * po.getVelocity().x, po.getMass() * po.getVelocity().y);
}

void PhysicsEngine::calculateOrbitalPeriod(PhysicsObject & satellite, PhysicsObject & target) {
	satellite.period = sqrtf(2 * PI * pow(calculateRange(satellite,target), 3)/_UNIVERSAL_CONST_GRAVITATION_ * satellite.mass);
}

void PhysicsEngine::calculateRequiredVelocity(PhysicsObject & satellite, PhysicsObject & target) {
	float reqVelocity = sqrtf(_UNIVERSAL_CONST_GRAVITATION_ * target.mass / calculateRange(satellite,target));

	satellite.reqVelocity = Vector2f(	(target.transform.x - satellite.transform.x) * reqVelocity,
										(target.getTransform().y - satellite.getTransform().y) * reqVelocity);
}

float PhysicsEngine::calculateResultant(Vector2f v) {
	float res = (sqrtf(pow(v.x, 2) + pow(v.y, 2)));
	return res;
}

float PhysicsEngine::calculateRange(PhysicsObject& target, PhysicsObject& satellite) {
	Vector2f range(	target.getTransform().x - satellite.getTransform().x, 
					target.getTransform().y - satellite.getTransform().y);

	float res_range = calculateResultant(range);
	if (res_range == 0.f)
		res_range = FLT_MIN;

	return abs(res_range);
}

Vector2f PhysicsEngine::calculateGravitationalForce(PhysicsObject& target, PhysicsObject& satellite) {
	float gravitationalForce = ((target.mass * satellite.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(target, satellite), 2));

	Vector2f satellite_to_target (	(target.transform.x - satellite.transform.x) * gravitationalForce,
									(target.transform.y - satellite.transform.y) * gravitationalForce);

	return satellite_to_target;
}

Vector2f PhysicsEngine::calculateNetGravitationalForce(PhysicsObject& satellite) {
	float gravitationalForce = 0.f;
	Vector2f satellite_to_target (0.f, 0.f);
	for (auto t : objects) {
		if (t->getTag() != satellite.getTag()) {
			gravitationalForce += ((t->mass * satellite.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(*t, satellite), 2));
			satellite_to_target.x += (t->transform.x - satellite.transform.x) * gravitationalForce;
			satellite_to_target.y += (t->transform.y - satellite.transform.y) * gravitationalForce;
		}
	}
	return satellite_to_target;
}

void PhysicsEngine::calculateNetForce(PhysicsObject & po) {
	po.netForce.x = calculateNetGravitationalForce(po).x + po.boostForce.x;
	po.netForce.y = calculateNetGravitationalForce(po).y + po.boostForce.y;
}
#pragma endregion
