#if !defined(TEXTURE_H)
#define TEXTURE_H

#include <string>
#include <glad/glad.h>


class Texture
{
private:
    GLuint ID;
    void load_image(std::string path);
public:
    Texture(std::string path);
    GLuint getID();
    int width, height;
};

#endif // TEXTURE_H
