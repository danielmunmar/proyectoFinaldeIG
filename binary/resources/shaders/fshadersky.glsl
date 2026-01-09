#version 330 core

struct Material {
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    vec4  emissive;
    float shininess;
};

struct Textures {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    sampler2D normal;
    float     shininess;
};

uniform Material umaterial;
uniform Textures utextures;
uniform bool     uWithMaterials;
uniform bool     uWithNormals;
uniform vec3     ueye;

in  vec3 vnor;
in  vec3 vpos;
in  vec2 vtex;

out vec4 outColor;

void main() {

    vec3 N = normalize(vnor);
    vec3 V = normalize(ueye - vpos);

    Material material;

    if(uWithMaterials) {
        material.ambient   = umaterial.ambient;
        material.diffuse   = umaterial.diffuse;
        material.specular  = umaterial.specular;
        material.emissive  = umaterial.emissive;
        material.shininess = umaterial.shininess;
    }
    else {
        material.ambient   = texture(utextures.diffuse ,vtex)*0.3;
        material.diffuse   = texture(utextures.diffuse ,vtex);
        material.specular  = texture(utextures.specular,vtex);
        material.emissive  = texture(utextures.emissive,vtex);
        material.shininess = utextures.shininess;
        if(uWithNormals) N = normalize(texture(utextures.normal,vtex).rgb - 0.5);
    }

    vec3 color = material.diffuse.rgb;

    outColor = vec4(color, 1.0);

}