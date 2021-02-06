/*
 * Games are divided into "scenes," allowing us to operate on them like finite
 * state machines.
 */

#ifndef SCENE_H
#define SCENE_H

class Scene {
    public:
        
        virtual void init()   = 0;
        virtual void draw()   = 0;
        virtual void update() = 0;
        virtual void close()  = 0;
};

#endif
