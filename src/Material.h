#if !defined(MATERIAL_H)
#define MATERIAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
} Material;

namespace Materials
{
    extern Material emerald;

    extern Material jade;

    extern Material obsidian;

    extern Material pearl;

    extern Material ruby;

    extern Material turquoise;

    extern Material brass;

    extern Material bronze;

    extern Material chrome;

    extern Material copper;

    extern Material gold;

    extern Material silver;

    extern Material black_plastic;

    extern Material cyan_plastic;

    extern Material green_plastic;

    extern Material red_plastic;

    extern Material white_plastic;

    extern Material yellow_plastic;

    extern Material black_rubber;

    extern Material cyan_rubber;

    extern Material green_rubber;

    extern Material red_rubber;

    extern Material white_rubber;

    extern Material yellow_rubber;

    
} // namespace Material
  

#endif // MATERIAL_H
