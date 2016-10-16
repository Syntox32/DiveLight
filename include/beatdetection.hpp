//
// Created by Anders on 10.10.2016.
//

#ifndef DIVELIGHT_BEATDETECTION_HPP
#define DIVELIGHT_BEATDETECTION_HPP

#include "config.hpp"
#include "visualizer.hpp"
#include "bufferpool.hpp"

#include <vector>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

struct Beat {
    int   index;
    float beatThresh;
    bool  isBeat;
    int   beatCount;
    sf::Time cooldown;
    sf::Time lastBeat;
    BufferPool* history;
    std::string name;

    Beat(int bufferCount, int idx, float thresh, std::string name = "") {
        if (name == "")
            name = "beat " + std::to_string(idx);

        index = idx;
        beatThresh = thresh;
        history = new BufferPool(bufferCount);
        cooldown = sf::milliseconds(10);
        lastBeat = sf::milliseconds(0);
    }

    static float C(float var, float thresh) {
        return (float)(-1.0f * 0.0025714f) + thresh; //1.5142857f;
    }
};

class BeatDetection
{
public:
    BeatDetection(Config& conf);
    ~BeatDetection();

    void configure(float *dataPointer, int avgLen, Visualizer *vzp);
    void detect();
    void addBeatChannel(int index, float thresh);

    void render(sf::RenderTarget *target);

    inline std::vector<Beat*>* getBeats() { return &this->beatManager; }
    inline void setMainBeat(Beat* beat) { mainBeat = beat; }
private:
    Config &config;
    Visualizer *vz;
    sf::Clock cooldownTimer;
    float *inData;
    float *averages;
    int averageLen;
    Beat* mainBeat;
    std::vector<Beat*> beatManager;
    std::vector<sf::RectangleShape*> m_rects;
};


#endif //DIVELIGHT_BEATDETECTION_HPP
