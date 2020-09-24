#version 330

in vec2 position;
uniform vec2 center;
uniform vec2 value;

void main()
{

	vec2 V 	= position - center;			//translate to (0,0)
	V 		= V * vec2(value.x, value.x);	//scale using "value" 
	V 		= V + center;					//translate back to position

	gl_Position = vec4( V, 0.0, 1.0);
}
