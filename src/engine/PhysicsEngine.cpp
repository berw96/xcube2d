#include "PhysicsEngine.h"
#include <iostream>

/*PHYSICS OBJECTS*/

/*
	@param Point2 center
*/
PhysicsObject::PhysicsObject(const Point2& center) : center(center), mass(_DEFAULT_INIT_MASS_), transform(_DEFAULT_INIT_TRANSFORM_), radius(_DEFAULT_RADIUS_) {}
/*
	@param Point2 center
	@param float mass
*/
PhysicsObject::PhysicsObject(const Point2& center, float mass) : center(center), transform(_DEFAULT_INIT_TRANSFORM_), radius(_DEFAULT_RADIUS_) { setMass(mass); }
/*
	@param Point2 center
	@param float mass
	@param Vector2f transform
*/
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform) : center(center), transform(transform), radius(_DEFAULT_RADIUS_) { setMass(mass); }
/*
	@param Point2 center
	@param float mass
	@param Vector2f transform
	@param float radius
*/
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform, float radius) : center(center), transform(transform), radius(radius) { setMass(mass); }
/*
	@param Point2 center
	@param float mass
	@param Vector2f transform
	@param float radius
	@param std::string tag
*/
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform, float radius, std::string tag) : center(center), transform(transform), radius(radius), tag(tag) { setMass(mass); }
/*
	@param Point2 center
	@param float mass
	@param Vector2f transform
	@param float radius
	@param std::string tag
	@param Vector2f boostForce
*/
PhysicsObject::PhysicsObject(const Point2& center, float mass, Vector2f transform, float radius, std::string tag, Vector2f boostForce) : center(center), transform(transform), radius(radius), tag(tag), boostForce(boostForce) { setMass(mass); }
/*
	Sets the mass of a PhysicsObject. If the provided mass is negative or
	zero then it is set to smallest positive value for a float.

	@param float mass
*/
void PhysicsObject::setMass(float m) {
	if (m <= 0.f)
		m = FLT_MIN;
	mass = m;
}

/* PHYSICS ENGINE */
PhysicsEngine::PhysicsEngine() {}
/*
	@param PhysicsObject po
*/
PhysicsEngine::PhysicsEngine(PhysicsObject & po) { registerObject(po); }

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

	@param PhysicsObject po
*/
void PhysicsEngine::mechanics(PhysicsObject & po) {

	po.acceleration.x = po.netForce.x / po.mass;
	po.acceleration.y = po.netForce.y / po.mass;

	po.speed.x += po.acceleration.x;
	po.speed.y += po.acceleration.y;

	po.velocity.x = po.speed.x;
	po.velocity.y = po.speed.y;

	po.transform.x += po.velocity.x;
	po.transform.y += po.velocity.y;

	po.center.x = po.transform.x;
	po.center.y = po.transform.y;
}

/*
	Calculates the momentum of a PhysicsObject and returns it as a vector.

	@param PhysicsObject
*/
void PhysicsEngine::calculateMomentum(PhysicsObject& po) {
	po.momentum = Vector2f(po.getMass() * po.getVelocity().x, po.getMass() * po.getVelocity().y);
}

/*
	Registers an instance of PhysicsObject with the PhysicsEngine.
	A PhysicsObject does not need to be registered with the engine
	in order to have behaviour implemented, however this is useful
	for calculating the net force excerted on each instance.

	@param PhysicsObject
*/
void PhysicsEngine::registerObject(PhysicsObject& obj) {
	objects.push_back(obj);
}

/*
	@param PhysicsObject satellite
	@param PhysicsObject target
*/
void PhysicsEngine::calculateOrbitalPeriod(PhysicsObject & satellite, PhysicsObject & target) {
	satellite.period = sqrtf(2 * PI * pow(calculateRange(satellite,target), 3)/_UNIVERSAL_CONST_GRAVITATION_ * satellite.mass);
}

/*
	@param PhysicsObject satellite
	@param PhysicsObject target
*/
void PhysicsEngine::calculateRequiredVelocity(PhysicsObject & satellite, PhysicsObject & target) {
	float reqVelocity = sqrtf(_UNIVERSAL_CONST_GRAVITATION_ * target.mass / calculateRange(satellite,target));

	satellite.reqVelocity = Vector2f(	(target.transform.x - satellite.transform.x) * reqVelocity,
										(target.getTransform().y - satellite.getTransform().y) * reqVelocity);
}

/*
	Uses Pythagorean Theorem to calculate the resultant
	value of a vector's x and y components.

	@param Vector2f v
*/
float PhysicsEngine::calculateResultant(Vector2f v) {
	float res = (sqrtf(pow(v.x, 2) + pow(v.y, 2)));
	return res;
}

/*
	Creates a new Vector2f instance containing the displacement
	between PhysicsObjects a and b on each axis, assigns the
	resultant to a float and returns its modulus.

	@param PhysicsObject target
	@param PhysicsObject object
*/
float PhysicsEngine::calculateRange(PhysicsObject& target, PhysicsObject& satellite) {
	Vector2f range(	target.getTransform().x - satellite.getTransform().x, 
					target.getTransform().y - satellite.getTransform().y);

	float res_range = calculateResultant(range);
	if (res_range == 0.f)
		res_range = FLT_MIN;

	return abs(res_range);
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

	@param PhysicsObject target
	@param PhysicsObject object
*/
Vector2f PhysicsEngine::calculateGravitationalForce(PhysicsObject& target, PhysicsObject& satellite) {
	float gravitationalForce = ((target.mass * satellite.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(target, satellite), 2));

	Vector2f satellite_to_target (	(target.transform.x - satellite.transform.x) * gravitationalForce,
									(target.transform.y - satellite.transform.y) * gravitationalForce);

	//Gravity is applied identically across both axis'.
	return satellite_to_target;
}


/*
	@param PhysicsObject satellite
*/
Vector2f PhysicsEngine::calculateGravitationalForce(PhysicsObject& satellite) {
	float gravitationalForce = 0.f;
	Vector2f satellite_to_target;
	for (auto t : objects) {
		gravitationalForce += ((t.mass * satellite.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(t, satellite), 2));
		satellite_to_target.x += (t.transform.x - satellite.transform.x) * gravitationalForce;
		satellite_to_target.y += (t.transform.y - satellite.transform.y) * gravitationalForce;
	}
	//Gravity is applied identically across both axis'.
	return satellite_to_target;
}

/*
	Takes a collection of PhysicsObjects and calculates the net force
	created between them. This is achieved by summating all the forces
	excerted on a single object by the others.

	Forces include:
		+ Gravity
		+ Boosts

	@param PhysicsObject a
	@param PhysicsObject b
	@param PhysicsObject c
*/
void PhysicsEngine::calculateNetForce(PhysicsObject& a, PhysicsObject& b, PhysicsObject& c) {
	a.netForce.x = calculateGravitationalForce(b,a).x + calculateGravitationalForce(c,a).x + a.boostForce.x;
	a.netForce.y = calculateGravitationalForce(b,a).y + calculateGravitationalForce(c,a).y + a.boostForce.y;
}
#pragma endregion
