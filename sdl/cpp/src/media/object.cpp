#include "object.hpp"

namespace media {

/*
 * =============================================================================
 * Object
 * =============================================================================
 */
 
void Object::set(SDL_Texture *texture) {
    if (this->texture != nullptr) {
        this->free();
    }
    this->texture = texture;
}

void Object::free() {
    SDL_DestroyTexture(this->texture);
}

/*
 * =============================================================================
 * ClipObject
 * =============================================================================
 */

void ClipObject::set(SDL_Texture *texture) {
    if (this->texture != nullptr) {
        this->free();
    }
    this->texture = texture;
    SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);
}


};
