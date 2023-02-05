#version 130

uniform vec2 u_resolution;
uniform float u_time;

vec2 sphIntersect( in vec3 ro, in vec3 rd, in vec3 ce, float ra )
{
    vec3 oc = ro - ce;
    float b = dot( oc, rd );
    float c = dot( oc, oc ) - ra*ra;
    float h = b*b - c;
    if( h<0.0 ) return vec2(-1.0); // no intersection
    h = sqrt( h );
    return vec2( -b-h, -b+h );
}

vec3 castRay(in vec3 ro, in vec3 rd)
{
	vec2 it = sphIntersect(ro, rd, vec3(0.0, 0.0, 0.0), 1.0);
	if(it.x < 0.0) return vec3(0.0);
	vec3 itPos = ro + rd * it.x;
	vec3 n = itPos;
	vec3 light = normalize(vec3(cos(u_time), 0.75, sin(u_time)));
	float diffuse = max(0.0, dot(light, n)) * 0.5;
	float specular = pow(max(0.0, dot(reflect(rd, n), light)), 32.0);
	return vec3(diffuse + specular);
}

void main()
{
	vec2 uv = (gl_TexCoord[0].xy - 0.5) * u_resolution / u_resolution.y; // преобразование текстурных координат в экранные координаты с учётом aspect-ratio
	vec3 rayPos = vec3(-5.0, 0.0, 0.0);
	vec3 rayDirection = normalize(vec3(1.0, uv)); // 1.0 - смотрит в сторону +х, uv - изменяем угол луча таким образом, чтобы он смотрел на пиксель
	vec3 col = castRay(rayPos, rayDirection);
	gl_FragColor = vec4(col, 1.0);
}