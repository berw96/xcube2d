#include "PhysicsEngine.h"
#include <iostream>

/*PHYSICS OBJECTS*/
PhysicsObject::PhysicsObject(const Point2& center, float x, float y) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(_DEFAULT_INIT_FORCE_), mass(_DEFAULT_INIT_MASS_), transform(_DEFAULT_INIT_TRANSFORM_), collider(_DEFAULT_COLLIDER_) {}
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(_DEFAULT_INIT_FORCE_), transform(_DEFAULT_INIT_TRANSFORM_), collider(_DEFAULT_COLLIDER_) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass, Vector2f transform) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(_DEFAULT_INIT_FORCE_), transform(transform), collider(_DEFAULT_COLLIDER_) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, float mass, Vector2f transform, Rectf collider) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(_DEFAULT_INIT_FORCE_), transform(transform), collider(collider) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, Vector2f force, float mass, Vector2f transform) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(force), transform(transform), collider(_DEFAULT_COLLIDER_) { setMass(mass); }
PhysicsObject::PhysicsObject(const Point2& center, float x, float y, Vector2f force, float mass, Vector2f transform, Rectf collider) : center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(force), transform(transform), collider(collider) { setMass(mass); }

void PhysicsObject::setMass(float m) {
	/*if the provided mass is negative or zero, set to smallest positive value for a float.*/
	if (m <= 0.f)
		m = FLT_MIN;
	mass = m;
}

#pragma region CHILDREN
void PhysicsObject::registerChild(PhysicsObject & po) {
		children.push_back(po);
}

void PhysicsObject::forgetChildren() {
	children.clear();
}
#pragma endregion


/* PHYSICS ENGINE */
PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::registerObject(std::shared_ptr<PhysicsObject> obj) {
	objects.push_back(obj);
}

void PhysicsEngine::setMovable(PhysicsObject& po, bool b) {
	po.autoMove = b;
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
*/
void PhysicsEngine::mechanics(PhysicsObject & po) {
	if (po.autoMove == true) {
		/*Physics engine calculates the appropriate acceleration
		for each PhysicsObject using its mass and set force vector.*/
		po.acceleration.x = calculateAcceleration_x(po);
		po.acceleration.y = calculateAcceleration_y(po);

		//accleration increments speed
		po.speed.x += po.acceleration.x;
		po.speed.y += po.acceleration.y;

		//speed sets the magnitude of velocity
		po.velocity.x = po.speed.x;
		po.velocity.y = po.speed.y;

		//velocity moves PhysicsObjects by incrementing its transform
		po.transform.x += (po.velocity.x);
		po.transform.y += (po.velocity.y);
	}

	/*
		Collider follows PhysicsObject (implicit component) at
		all times regardless if the root can move.
		Half lengths are substracted from the collider's transform
		to align its center with that of the root component's.
	*/
	po.collider.x = po.transform.x - po.hlX;
	po.collider.y = po.transform.y - po.hlY;
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
float PhysicsEngine::calculateRange(PhysicsObject& a, PhysicsObject& b) {
	Vector2f range(a.getRootTransform().x - b.getRootTransform().x,
		a.getRootTransform().y - b.getRootTransform().y);

	float res_range = calculateResultant(range);
	return abs(res_range);
}

/*
	Functions for calculating the axial ranges between PhysicsObjects.
*/
float PhysicsEngine::calculateRange_x(PhysicsObject& a, PhysicsObject& b) {
	float rx = (a.getRootTransform().x - b.getRootTransform().x);
	return rx;
}

float PhysicsEngine::calculateRange_y(PhysicsObject& a, PhysicsObject& b) {
	float ry = (a.getRootTransform().y - b.getRootTransform().y);
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
Vector2f PhysicsEngine::calculateGravitationalForce(PhysicsObject& a, PhysicsObject& b) {
	float grav = ((a.getMass() * b.getMass() * _UNIVERSAL_CONST_GRAVITATION_) / pow(calculateRange(a, b), 2));

	//Gravity is applied identically across both axis'.
	Vector2f gravity(grav, grav);
	return gravity;
}
#pragma endregion

#pragma region COLLISION
/*
	Function checks if two colliders are intersecting, if they are
	the push function is invoked on them to simulate solidity.

	Objects may only be moved if they are allowed by the autoMove bool.
*/
void PhysicsEngine::collision(PhysicsObject & a, PhysicsObject & b) {
	if (a.getCollider().intersects(b.getCollider())){
		push(a,b);
	}
}

/*
	Function applies a pushing effect on the two PhysicsObjects. The effect is
	achieved by means of setting the root transform of the colliding objects as
	the current transform on each axis, minus the difference between their
	velocities on that axis.

	If object b is not allowed to move, then it must be a static object (like
	a floor or a wall).
*/
void PhysicsEngine::push(PhysicsObject & a, PhysicsObject & b) {
	a.transform = (Vector2f(a.transform.x - (a.velocity.x - b.velocity.x),
							a.transform.y - (a.velocity.y - b.velocity.y)));

	if (b.autoMove == true) {
		b.transform = (Vector2f(b.transform.x - (b.velocity.x - a.velocity.x),
								b.transform.y - (b.velocity.y - a.velocity.y)));
	}
}
#pragma endregion

