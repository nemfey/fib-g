#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

out vec4 gfrontColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform float step = 0.1;

const vec4 grey = vec4(vec3(0.8),1);

void cubeVertex(bool x, bool y, bool z, vec3 bt, vec3 N) {
	gfrontColor = grey*N.z;
	vec3 V = vec3(x?0.5:-0.5, y?0.5:-0.5, z?0.5:-0.5) * step;
	gl_Position = modelViewProjectionMatrix * vec4(V+bt,1);
	EmitVertex();
}

void paintCube(vec3 BT) {
  // front
  vec3 N=normalMatrix*vec3(0, 0, 1);
  cubeVertex(false, false, true, BT, N);
  cubeVertex(true, false, true, BT, N);
  cubeVertex(false, true, true, BT, N);
  cubeVertex(true, true, true, BT, N);
  EndPrimitive();
  // back
  N=normalMatrix*vec3(0, 0, -1);
  cubeVertex(false, true, false, BT, N);
  cubeVertex(true, true, false, BT, N);
  cubeVertex(false, false, false, BT, N);
  cubeVertex(true, false, false, BT, N);
  EndPrimitive();
  // left
  N=normalMatrix*vec3(-1, 0, 0);
  cubeVertex(false, false, false, BT, N);
  cubeVertex(false, false, true, BT, N);
  cubeVertex(false, true, false, BT, N);
  cubeVertex(false, true, true, BT, N);
  EndPrimitive();
  // right
  N=normalMatrix*vec3(1, 0, 0);
  cubeVertex(true, true, false, BT, N);
  cubeVertex(true, true, true, BT, N);
  cubeVertex(true, false, false, BT, N);
  cubeVertex(true, false, true, BT, N);
  EndPrimitive();
  // top
  N=normalMatrix*vec3(0, 1, 0);
  cubeVertex(false, true, true, BT, N);
  cubeVertex(true, true, true, BT, N);
  cubeVertex(false, true, false, BT, N);
  cubeVertex(true, true, false, BT, N);
  EndPrimitive();
  // bottom
  N=normalMatrix*vec3(0, -1, 0);
  cubeVertex(true, false, false, BT, N);
  cubeVertex(true, false, true, BT, N);
  cubeVertex(false, false, false, BT, N);
  cubeVertex(false, false, true, BT, N);
  EndPrimitive();
}

void main( void )
{
    vec3 bt = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz)/3;
    
    bt = bt/step;
    bt.x = int(bt.x);
    bt.y = int(bt.y);
    bt.z = int(bt.z);
    bt = bt * step;
    paintCube(bt);
}
