#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <SFML/Graphics.hpp>

struct AnimationFrame {
    sf::Sprite spr;
    float time; //Seconds
};

struct AnimationTrack {
    short numOfLoops; //0 = Infinite
    std::string spritesheetFilename;
    std::vector<AnimationFrame> frames;
};

class AnimationData {
public:
    std::vector<AnimationTrack> animations;
    std::map<std::string, int> animNames; //Name -> Vector pos

        bool Load(std::string filename);
    bool Save(const char* filename);

    std::set<std::string> getContentFilename();
    void getContentFilename(std::set<std::string>& contentFilename);

private:
    bool ReadANIM (
        std::string& currentAnimName,
        AnimationTrack*& currentAnimTrack,
        std::string& line, int lineNum);

    bool ReadFRAME (
        AnimationTrack*& currentAnimTrack,
        std::string& line, int lineNum);
};

class Animation
{
public:
    Animation();
    Animation(AnimationData *data);

    virtual void Update(float GameTime);

    int getAnimID(std::string name);
    int getLoopsLeft();
    bool setAnimData(AnimationData* data);
    bool SelectAnim(std::string name);
    bool SelectAnim(int animID);

    sf::Sprite* getCurrentFrame();

private:
    AnimationData* data;
    int animSelected;
    int loopsLeft;
    int frameSelected;
    float frameTimeLeft;

    void NextFrame();
};

#endif // ANIMATION_H
