/*
* @Author: Kamil Rocki
* @Date:   2017-02-23 09:37:54
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-02-23 09:38:13
*/

//vertex shader w/ two rotation matricies stored
/*char *vertex_shader = "\
#version 330\n\
layout(location = 0) in vec3 position;\
uniform vec3 offset;\
uniform mat4 perspective;\
uniform vec2 angle;\
uniform vec4 color;\
smooth out vec4 theColor;\
void main(){\
  mat4 xRMatrix = mat4(cos(angle.x), 0.0, sin(angle.x), 0.0,\
                        0.0, 1.0, 0.0, 0.0,\
                        -sin(angle.x), 0.0, cos(angle.x), 0.0,\
                        0.0, 0.0, 0.0, 1.0);\
  mat4 yRMatrix = mat4(1.0, 0.0, 0.0, 0.0,\
                  0.0, cos(angle.y), -sin(angle.y), 0.0,\
                  0.0, sin(angle.y), cos(angle.y), 0.0,\
                  0.0, 0.0, 0.0, 1.0);\
  vec4 rotatedPosition = vec4( position.xyz, 1.0f ) * xRMatrix * yRMatrix;\
  vec4 cameraPos = rotatedPosition + vec4(offset.x, offset.y, offset.z, 0.0);\
  gl_Position = perspective * cameraPos;\
  theColor = mix( vec4( color.x, color.y, color.z, color.w ), vec4( 0.0f, color.y, color.z, color.w ), position.y / 10 );\
}";
*/

char *vertex_shader = "\
#version 330\n\
layout(location = 0) in vec3 position;\
uniform vec3 offset;\
uniform mat4 perspective;\
uniform vec2 angle;\
uniform vec4 color;\
smooth out vec4 theColor;\
float hue2rgb(float p, float q, float t) \
{ \
  float tt = t; \
  if (tt < 0.0) tt += 1.0; \
  if (tt > 1.0) tt -= 1.0; \
  if (tt < 1.0/6.0) return p + (q - p) * 6.0 * tt; \
  if (tt < 1.0/2.0) return q; \
  if (tt < 2.0/3.0) return p + (q - p) * (2.0/3.0 - tt) * 6.0; \
  return p; \
} \
vec4 hslToRgb(float h, float s, float l)\
{\
  float r, g, b;\
  if(s == 0.0)\
  {\
    r = g = b = l; \
  }\
  else\
  {\
    float q;\
    if (l < 0.5)\
      q = l * (1.0 + s);\
    else\
      q = l + s - l * s;\
    float p = 2.0 * l - q;\
    r = hue2rgb(p, q, h + 1.0/3.0);\
    g = hue2rgb(p, q, h);\
    b = hue2rgb(p, q, h - 1.0/3.0);\
  }\
  return vec4(r, g, b, 0.75);\
} \
void main(){\
  mat4 xRMatrix = mat4(cos(angle.x), 0.0, sin(angle.x), 0.0,\
                        0.0, 1.0, 0.0, 0.0,\
                        -sin(angle.x), 0.0, cos(angle.x), 0.0,\
                        0.0, 0.0, 0.0, 1.0);\
  mat4 yRMatrix = mat4(1.0, 0.0, 0.0, 0.0,\
                  0.0, cos(angle.y), -sin(angle.y), 0.0,\
                  0.0, sin(angle.y), cos(angle.y), 0.0,\
                  0.0, 0.0, 0.0, 1.0);\
  vec4 rotatedPosition = vec4( position.xyz, 1.0f ) * xRMatrix * yRMatrix;\
  vec4 cameraPos = rotatedPosition + vec4(offset.x, offset.y, offset.z, 0.0);\
  gl_Position = perspective * cameraPos;\
  float yRange = 1.0; \
  float yMax = 0.5; \
  theColor = hslToRgb(0.6 * (yMax - position.y / 10) / yRange, 1, 0.5);\
}";

//fragment shader
char *fragment_shader = "\
#version 330\n\
smooth in vec4 theColor;\
out vec4 outputColor;\
void main(){\
  outputColor = theColor;\
}";