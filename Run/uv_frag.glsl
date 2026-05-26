#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

uniform sampler2D tex;
uniform int renderVariant;

vec3 lightPos = vec3(0, -1000, 1000);
vec3 lightColor = vec3(1,1,1);

vec3 ambient = vec3(0.2);

void main() {
   switch(renderVariant) {
      case 0: {
         vec3 norm = normalize(Normal);
         vec3 lightDir = normalize(lightPos - FragPos);

         float diff = max(dot(norm, lightDir), 0.0);
         vec3 diffuse = diff * lightColor;

         vec4 text = texture(tex, UV);
         if(text.w < 0.1f) discard;
         
         vec3 objColor = text.xyz;

         vec3 result = (ambient + diffuse) * objColor;
         FragColor = vec4(result, 1.0f);
         break;
      }
      case 1: {
         int id = gl_PrimitiveID;
         float r = float((id * 97) % 255) / 255.0;
         float g = float((id * 57) % 255) / 255.0;
         float b = float((id * 23) % 255) / 255.0;

         FragColor += vec4(r, g, b, 1.0);
         break;
      }
      case 2: {
         vec3 norm = normalize(Normal);
         vec3 lightDir = normalize(lightPos - FragPos);

         float diff = max(dot(norm, lightDir), 0.0);
         vec3 diffuse = diff * lightColor;

         vec3 objColor = (Normal+vec3(1,1,1))*0.5;

         vec3 result = (ambient + diffuse) * objColor;
         FragColor = vec4(result, 1.0f);
         break;
      }
      case 3: {
         vec4 text = texture(tex, UV);
         if(text.w < 0.1f) discard;
         FragColor = vec4(text.xyz, 1.0f);
      }
   }
} 
