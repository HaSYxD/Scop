# include <Camera.hpp>
# include <cmath>

Camera::Camera(const vec3 target) : _position((vec3){0, 0, -2}), _target(target), _angle(0), _velocity(0), _radius(5) {}

Camera::~Camera() {}

void	Camera::update(const vec2 &scrollOff)
{
	float	rDelta = this->_radius += scrollOff.entries[1] * SCROLL_SENSITIVITY;

	if (rDelta > MAX_RADIUS)	this->_radius = MAX_RADIUS;
	else if (rDelta < MIN_RADIUS)	this->_radius = MIN_RADIUS;
	else				this->_radius = rDelta;

	float	vDelta = this->_velocity += scrollOff.entries[0] * VELOCITY_FACTOR;

	if (vDelta > MAX_VELOCITY)		this->_velocity = MAX_VELOCITY;
	else if (vDelta < MIN_VELOCITY)		this->_velocity = MIN_VELOCITY;
	else if (vDelta < 0.1 && vDelta > -0.1)	this->_velocity = 0;
	else					this->_velocity = vDelta;

	this->_angle += this->_velocity;

	this->_position = {
		this->_radius * sinf(this->_angle),
		this->_radius * cosf(this->_angle),
		this->_position.entries[2]
	};
	if (this->_velocity > 0)	this->_velocity -= VELOCITY_FALLOFF;
	else				this->_velocity += VELOCITY_FALLOFF;
}

void	Camera::setPosition(const vec3 &position)
{
	this->_position = position;
}

void	Camera::setTarget(const vec3 &target)
{
	this->_target = target;
}

void	Camera::setAngle(const double &angle)
{
	this->_angle = angle;
}

void	Camera::setRadius(const float &radius)
{
	this->_radius = radius;
}

vec3	Camera::getPostion()
{
	return (this->_position);
}

vec3	Camera::getTarget()
{
	return (this->_target);
}

double	Camera::getAngle()
{
	return (this->_angle);
}

float	Camera::getRadius()
{
	return (this->_radius);
}