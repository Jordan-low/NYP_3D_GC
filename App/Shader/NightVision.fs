#version 400
in vec2 TexCoords;
out vec3 color;
 
uniform sampler2D screenTexture; // frame buffer
uniform sampler2D noiseTexture; // noise texture
uniform sampler2D googleTexture; // google texture
uniform float elapsedTime; // seconds for noise effect
 
float contrast = 0.5;
uniform float intensityAdjust; // = 1;
uniform float noiseAmplification; // 1
uniform float bufferAmplication; // 1
 
void main()
{
// uv coord for noise texture and swirl effect
vec2 uv;
uv.x = 0.35*sin(elapsedTime*50.0);
uv.y = 0.35*cos(elapsedTime*50.0);           
 
// noise texture color + rotation by time
vec3 noise = texture(noiseTexture, TexCoords.st + uv).rgb * noiseAmplification;
 
// google mask
vec3 googleColor = texture(googleTexture, TexCoords.st).rgb;
 
// frame buffer + litle swirl
vec3 sceneColor = texture(screenTexture, TexCoords.st + (noise.xy*0.005)).rgb * bufferAmplication;      
 
//color intensity - color dominant
const vec3 lumvec = vec3(0.30, 0.59, 0.11);
float intentisy = dot(lumvec,sceneColor) ;
 
// adjust contrast - 0...1
intentisy = clamp(contrast * (intentisy - 0.5) + 0.5, 0.0, 1.0);
 
// final green result 0...1
float green = clamp(intentisy / 0.59, 0.0, 1.0) * intensityAdjust;
 
// vision color - getting green max
vec3 visionColor = vec3(0,green,0);//vec3(0.1, 0.95, 0.2); 
 
// final color
color = (sceneColor + (noise*0.2)) * visionColor * googleColor;
 
}