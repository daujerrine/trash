/*
 * Games are divided into "scenes," allowing us to operate on them like finite
 * state machines.
 */

#ifndef SCENE_H
#define SCENE_H

enum SceneState {
    SCENE_TITLE = 0,
    SCENE_GAME,
    SCENE_GAME_OVER
};

class Scene {
    public:
        ~Scene() {}
        virtual void init()   = 0;
        virtual void draw()   = 0;
        virtual void update() = 0;
        virtual void close()  = 0;
};

#endif
