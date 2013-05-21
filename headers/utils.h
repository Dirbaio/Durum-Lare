#ifndef UTILS_H
#define UTILS_H


#define _USE_MATH_DEFINES
#define GL_GLEXT_PROTOTYPES 1
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glext.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <list>

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#include <random>
typedef std::mt19937 mt19937;
#else
#include <boost/random.hpp>
typedef boost::random::mt19937 mt19937;
#endif

using namespace std;

extern sf::RenderWindow* App;
extern sf::Time global_frametime;
extern sf::Time scene_total_time;

typedef sf::Vector3<float> vec3;
typedef sf::Vector2<float> vec2;

extern bool useWiimotes;
extern bool closeNow;

struct color {
    float r, g, b, a;
};


struct Utils {
private:

    static mt19937 mersenne;
public:

    //Dot product
    static inline vec3 to3(const vec2& v)
    {
        return vec3(v.x, 0, v.y);
    }

    static inline float dot(const vec3& a, const vec3& b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    static inline float dot(const vec2& a, const vec2& b)
    {
        return a.x*b.x + a.y*b.y;
    }

    //Cross product
    static inline vec3 cross(const vec3& a, const vec3& b)
    {
        return vec3(a.y*b.z - b.y*a.z,
                    -a.x*b.z + b.x*a.z,
                    a.x*b.y - b.x*a.y);
    }

    //Returns random float between -f and f
    static inline float frand(float f)
    {
        return ((rand()%(1<<20))/float(1<<19) - 1)*f;
    }

    static inline bool prand(float f)
    {
        return frand(0.5) < f-0.5;
    }

    static inline float norm(vec3 v)
    {
        return sqrt(dot(v, v));
    }

    static inline int irand(int min, int max)
    {
        if(max < min)
            cerr<<"Eres tonto. ~Balaghi"<<endl;

        if(min == max) return min;
        max++;
        return min + rand() % (max-min);
    }

    static  inline float norm(vec2 v)
    {
        return sqrt(dot(v, v));
    }

    static inline void normalize(vec3& v)
    {
        v /= norm(v);
    }

    static inline void normalize(vec2& v)
    {
        v /= norm(v);
    }

    static inline float toRad(float deg)
    {
        return deg/180.0*M_PI;
    }

    static inline float toDeg(float rad)
    {
        return rad*180.0/M_PI;
    }

    static void randomSeed(int seed) {
        mersenne = mt19937(seed);
    }

    static int randomInt() {
        return (mersenne() & 0x7FFFFFFF);
    }

    //Returns random between [min, max]
    static int randomInt(int min, int max) {
        if (min >= max) return min;
        return (randomInt()%((max-min)+1)) + min;
    }

    static float vec2fToDeg(const sf::Vector2f& vector) {
        return ((float)atan(vector.y/vector.x)) * (float) M_PI * 2.0f;
    }


    static float dot2(const sf::Vector2f& a, const sf::Vector2f& b) {
        return a.x*b.x + a.y*b.y;
    }

    static bool rectTouchesCircle(const sf::FloatRect& r,
                                  const sf::Vector2f& c, const float& rad) {
        sf::Vector2f cDist;
        cDist.x = (float) abs((int) c.x - (int) r.left);
        cDist.y = (float) abs((int) c.y - (int) r.top);

        if (cDist.x > (r.width/2 + rad)) return false;
        if (cDist.y > (r.height/2 + rad)) return false;

        if (cDist.x <= (r.width)/2) return true;
        if (cDist.y <= (r.height)/2) return true;

        float cornerDist_sq = (cDist.x - r.width/2)*(cDist.x - r.width/2) +
                (cDist.y - r.height/2)*(cDist.y - r.height/2);

        return (cornerDist_sq <= (rad*rad));

    }

    static float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
        vec2 lol = p2 - p1;
        return norm(lol);
    }

    static std::string intToString(int n) {
        std::stringstream ss;
        ss << n;
        return ss.str();
    }

    static int charToInt(char c) {
        if (c < '0' || c > '9') return 0;

        return c - '0';
    }

    //Returns true if point r is counterclockwise over line pq
    static bool orientationTest(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r) {
        return (((p.x-r.x)*(q.y-r.y) - (q.x-r.x)*(p.y-r.y)) > 0 );
    }

    //Rotates boxes for attack action when front-clicked
    static int atkFClickBoxRot(int boxes, int rotations) {
        int rot[] = {3, 5, 8, 2, 7, 1, 4, 6};

        while (rotations > 0) {
            int brot = 0;
            while (boxes > 0) {
                brot = (brot*10) + rot[(boxes%10)-1];
                boxes = boxes/10;

            }
            --rotations;
            boxes = brot;
        }

        return boxes;
    }

    static bool rectCollision(sf::FloatRect r1, sf::FloatRect r2) {

        if (r1.left > (r2.left+r2.width)) return false;
        if ((r1.left+r1.width) < r2.left) return false;

        if (r1.top > (r2.top+r2.height)) return false;
        if ((r1.top+r1.height) < r2.top) return false;

        return true;
    }

    //Returns unitary vector of direction (from) -> (to)
    static sf::Vector2f getDirection(const sf::Vector2f& from, const sf::Vector2f& to) {
        sf::Vector2f direction = to - from;
        float magnitude = (float) sqrt((direction.x*direction.x)+(direction.y*direction.y));
        direction.x = direction.x/magnitude;
        direction.y = direction.y/magnitude;
        /*
        std::cerr << "utils, getdir: " << std::endl
            << "\tto: " << to.x << ", " << to.y << std::endl
            << "\tfrom: " << from.x << ", " << from.y << std::endl
            << "\tdir: " << direction.x << ", " << direction.y << std::endl
            << "\tmagn: " << magnitude << std::endl;
            */
        return direction;
    }
};

#endif // UTILS_H
