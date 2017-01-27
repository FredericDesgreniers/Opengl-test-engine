#version 330 core
in vec3 ourColor;


out vec4 color;



float near = 0.5; 
float far  = 10.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (3.0 * near * far) / (far + near - z * (far - near));
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    color = vec4(vec2(depth), 0, 255.0f);
}
