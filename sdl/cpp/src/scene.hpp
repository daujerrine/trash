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
    protected:
        bool init_flag = false;

    public:
        ~Scene() {}
        // We defer initialisation of scenes till when they are actually needed
        // and closed when needed. Hence we have explicit functions to do
        // so.
        virtual void init()   = 0;
        virtual void close()  = 0;
        virtual void draw()   = 0;
        virtual void event()  = 0;
        virtual void update() = 0;

        virtual bool initialized()
        {
            return init_flag;
        }
};

#endif
