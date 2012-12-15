#include "animation.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "graphics_engine.h"

Animation::Animation() {
    data = NULL;
    animSelected = 0;
    loopsLeft = 0;
    frameSelected = 0;
    frameTimeLeft = 0;
}

Animation::Animation(AnimationData *data) {
    setAnimData(data);
}

void Animation::Update(float GameTime) {
    if (data == NULL) return;

    float time = GameTime;
    while (time != 0) {
        if (time <= frameTimeLeft) {
            frameTimeLeft -= time;
            time = 0;
        }
        else {
            time -= frameTimeLeft;
            NextFrame();
        }
    }
}

void Animation::NextFrame() {
    const AnimationTrack& anim = data->animations[animSelected];
    if (frameSelected+1 >= (int)anim.frames.size()) {
        if (loopsLeft > 0) --loopsLeft;
        if (loopsLeft != 0) frameSelected = 0;
    }
    else frameSelected++;
    frameTimeLeft = anim.frames[frameSelected].time;
}

bool Animation::setAnimData(AnimationData* _data) {
    if (_data->animations.size() == 0) return false;

    data = _data;
    if (!SelectAnim(0)) {
        data = NULL;
        return false;
    }

    return true;
}

sf::Sprite* Animation::getCurrentFrame() {
    if (data == NULL) return NULL;

    AnimationTrack& anim = data->animations[animSelected];
    AnimationFrame& fram = anim.frames[frameSelected];

    return &(fram.spr);
}

int Animation::getAnimID(std::string name) {
    if (data == NULL) return -1;
    std::map<std::string, int>::iterator it = data->animNames.find(name);
    if (it == data->animNames.end()) return -1;
    return (int)it->second;
}

int Animation::getLoopsLeft() {
    return loopsLeft;
}

bool Animation::SelectAnim(std::string name) {
    int id = getAnimID(name);
    if (id < 0) return false;
    return SelectAnim(id);
}

bool Animation::SelectAnim(int animID) {
    if (data == NULL) return false;
    if (data->animations[animID].frames.size() == 0) return false;

    animSelected  = animID;
    const AnimationTrack& anim = data->animations[animSelected];
    loopsLeft = anim.numOfLoops;
    if (loopsLeft == 0) loopsLeft = -1;

    frameSelected = 0;
    frameTimeLeft = anim.frames[frameSelected].time;

    return true;
}

bool AnimationData::Load(std::string filename) {
    std::ifstream in(filename.c_str());

    if (in.fail()) {
        std::cerr << "Error: " << filename << " doesn't exist." << std::endl;
        return false;
    }

    std::string currentAnimName;
    AnimationTrack *currentAnimTrack = NULL;
    std::string line;
    int lineNum = 0;

    while( getline(in, line) ) {
        //Ignore comments
        size_t comment_pos = line.find('#', 0);
        if (comment_pos != std::string::npos) {
            line.erase(comment_pos, std::string::npos);
        }

        std::string param;
        std::stringstream sline(line);

        lineNum = lineNum + 1;
        if (!(sline >> param)) {
            continue; //Ignore blank line
        }
        if (param == std::string("ANIM") ) {
            ReadANIM(currentAnimName, currentAnimTrack, line, lineNum);
        }
        else if (currentAnimTrack == NULL) {
            std::cerr << "Error "<<lineNum<<": Animation name undefined yet." << std::endl;
        }
        else {
            ReadFRAME(currentAnimTrack, line, lineNum);
        }
    }

    std::vector<AnimationTrack>::iterator it = animations.begin();
    while(it != animations.end()) {
        if (it->frames.size() == 0) {
            std::cerr << "Warning: Animation with no frames. Deleting." << std::endl;
            animations.erase(it);
        }
        else ++it;
    }

    if (animations.size() == 0) {
        std::cerr << "Error: Zero valid animations loaded." << std::endl;
        return false;
    }

    return true;
}

bool AnimationData::Save(const char* filename) {
    std::ofstream out(filename, std::ios_base::out | std::ios_base::trunc);

    std::vector<std::string> aux(animations.size());
    std::map<std::string, int>::iterator it = animNames.begin();
    for(;it != animNames.end(); it++) {
        aux[it->second] = it->first;
    }

    for (unsigned int i = 0; i < animations.size(); ++i) {
        AnimationTrack &track = animations[i];
        out << "ANIM \"" << aux[i] << "\" \"" << track.spritesheetFilename << "\" " << track.numOfLoops << std::endl;
        for (unsigned int j = 0; j < track.frames.size(); ++j) {
            AnimationFrame &frame = track.frames[j];
                        out << frame.spr.getLocalBounds().left <<" "<< frame.spr.getLocalBounds().top <<" "
                << frame.spr.getLocalBounds().width <<" "<< frame.spr.getLocalBounds().height <<" "
                << frame.time << std::endl;
        }
    }
    return true;
}

bool AnimationData::ReadANIM(std::string &currentAnimName, AnimationTrack *&currentAnimTrack, std::string &line, int lineNum) {

    //Leemos el nombre de la animacion situado entre comillas.
    size_t quoteStart = line.find('\"', 0);
    if (quoteStart == std::string::npos) {
        std::cerr <<"Error "<< lineNum <<": ANIM name not found."<<std::endl;
        return false;
    }
    quoteStart++;

    size_t quoteEnd = line.find('\"', quoteStart);
    if (quoteEnd == std::string::npos) {
        std::cerr <<"Error "<< lineNum <<": ANIM name incomplete."<<std::endl;
        return false;
    }

    std::string AnimName = line.substr(quoteStart, quoteEnd - quoteStart);
    quoteEnd++;

    quoteStart = line.find('\"', quoteEnd);
    if (quoteStart == std::string::npos) {
        std::cerr <<"Error "<< lineNum <<": ANIM spritesheet filename not found. Skipping line."<<std::endl;
        return false;
    }
    quoteStart++;

    quoteEnd = line.find('\"', quoteStart);
    if (quoteEnd == std::string::npos) {
        std::cerr <<"Error "<< lineNum <<": ANIM spritesheet filename incomplete. Skipping line."<<std::endl;
        return false;
    }

    std::string spritesheetFilename = line.substr(quoteStart, quoteEnd - quoteStart);
    quoteEnd++;

    std::string lineLeft = line.substr(quoteEnd, line.size() - quoteEnd);
    std::stringstream sline(lineLeft);

    int loops = 0;
    if (!(sline >> loops)) {
        std::cerr <<"Error "<< lineNum <<": ANIM num of loops not found.  Skipping line."<<std::endl;
        return false;
    }

    std::map<std::string, int>::iterator it = animNames.find(AnimName);
    if ( it != animNames.end() ) { //EXISTS
        std::cerr<<"Warning "<< lineNum <<": Animation name redefined. Overriding attributes."<<std::endl;
        currentAnimTrack = &animations[it->second];
    }
    else {
        int ID = (int)animations.size();
        animations.push_back(AnimationTrack());
        animNames[AnimName] = ID;
        currentAnimTrack = &animations[ID];
    }

    currentAnimName = AnimName;
    currentAnimTrack->numOfLoops = loops;
    currentAnimTrack->spritesheetFilename = spritesheetFilename;

    return true;
}

bool AnimationData::ReadFRAME(AnimationTrack *&currentAnimTrack, std::string &line, int lineNum)  {
        //TODO CHECK ERRORS
        std::stringstream sline(line);

        AnimationFrame frame;

        int x, y, w, h;
        float time;

        sline >> x;
        sline >> y;
        sline >> w;
        sline >> h;
        sline >> time;

        GraphEng* graphics = graphics->getInstance();

        frame.spr.setTexture(*(graphics->getTexture(currentAnimTrack->spritesheetFilename)));

        sf::IntRect rect(x, y, w, h);

                frame.spr.setTextureRect(rect);
        frame.time = time;
                frame.spr.setOrigin(frame.spr.getLocalBounds().width*0.5f, frame.spr.getLocalBounds().height*0.5f);

        //if(frame.time < 1) {
        //    std::cerr << "Error "<< lineNum <<": time isn't bigger than 0, setting time to 1." <<std::endl;
        //    frame.time = 1;
        //}

        currentAnimTrack->frames.push_back(frame);

        return true;
}

std::set<std::string> AnimationData::getContentFilename() {
    std::set<std::string> contentFilename;
    std::vector<AnimationTrack>::iterator it = animations.begin();
    for(;it != animations.end(); ++it) {
        contentFilename.insert(it->spritesheetFilename);
    }
    return contentFilename;
}

void AnimationData::getContentFilename(std::set<std::string> &contentFilename)  {
    std::vector<AnimationTrack>::iterator it = animations.begin();
    for(;it != animations.end(); ++it) {
        contentFilename.insert(it->spritesheetFilename);
    }
}
