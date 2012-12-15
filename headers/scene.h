#ifndef SCENE_H
#define SCENE_H

#define EXIT_SCN (Scene*)-1

class Scene
{
public:
    Scene();
    virtual ~Scene() {};


    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual void Update() = 0;
    virtual void Draw() = 0;

    Scene* nextScene;
private:

};

#endif // SCENE_H
