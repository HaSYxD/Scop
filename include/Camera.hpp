# pragma once

# include <math.hpp>

# define SCROLL_SENSITIVITY	0.2
# define MIN_RADIUS		1.5
# define MAX_RADIUS		150

# define VELOCITY_FACTOR	0.15
# define VELOCITY_FALLOFF	0.02
# define MAX_VELOCITY		1
# define FRICTION_OVERRIDE	0.02

class	Camera
{
	public:
		Camera(const vec3 target);
		~Camera();

		void	update(const vec2 &scrollOff);

		void	setPosition(const vec3 &position);
		void	setTarget(const vec3 &target);
		void	setAngle(const double &angle);
		void	setRadius(const float &radius);

		vec3	getPostion();
		vec3	getTarget();
		double	getAngle();
		float	getRadius();
	
	private:
		vec3	_position;
		vec3	_target;
		double	_angle;
		double	_velocity;
		float	_radius;
};
