//
// Created by Anders on 10.10.2016.
//

#include "../include/beatdetection.hpp"

BeatDetection::BeatDetection(Config& conf)
    : config(conf)
{
    float columnHeight = 20.0f;
    sf::Vector2f size(10, columnHeight);
    sf::RectangleShape *temp;

    for (unsigned int i = 0; i < conf.columnCount + 1; i++)
    {
        temp = new sf::RectangleShape(); //sf::Vector2f());
        temp->setFillColor(sf::Color::Red);
        temp->setSize(size);
        temp->setPosition(800 +(20.0f * i + (i * 2)), 200 - columnHeight);

        m_rects.push_back(temp);
    }
}

void BeatDetection::configure(float *dataPointer, int avgLen, Visualizer *vzp) {
    inData = dataPointer;
    averageLen = avgLen;
    vz = vzp;
    //if (averages != NULL) {
    //    delete averages;
    //}
    averages = new float[averageLen];
    cooldownTimer.restart();
}

void BeatDetection::detect() {
    vz->getAverages(inData, averageLen, averages, config.freqLow, config.freqHigh);

    float totalAvg = Utils::average(inData, 0, averageLen);
    mainBeat->history->push(totalAvg);

    if (totalAvg > mainBeat->beatThresh * mainBeat->history->avg()) {
        mainBeat->isBeat = true;
        mainBeat->beatCount++;
        mainBeat->lastBeat = cooldownTimer.getElapsedTime();
    } else {
        mainBeat->isBeat = false;
    }

    float ny;
    auto* rect = m_rects[m_rects.size() - 1];
    if (mainBeat->isBeat) {
        rect->setFillColor(sf::Color::Cyan);
        ny = Utils::lerp(rect->getSize().y, rect->getSize().y + config.moveDelta, 1.0);
    } else {
        float delta = rect->getSize().y - config.moveDelta;
        ny = 20.0f;
        //ny = Utils::lerp(rect->getSize().y, 20.0f, 1); //(delta > 20.0f ? delta : 20.0f), 0.1);
    }
    rect->setSize(sf::Vector2f(rect->getSize().x, ny));

    int j = 0;
    for (int i = 0; i < averageLen; i++) {
        for (; j < beatManager.size(); j++) {
            if (beatManager[i]->index == i) {
                beatManager[i]->history->push(averages[i]);
                break;
            }
        }
    }

    float avg, var;
    for (auto* b : beatManager) {
        avg = b->history->avg();
        //var = b->history->sqVar(avg);

        float f1 = b->history->first();
        float f2 = b->beatThresh * avg;
        if (f1 > b->C(avg, b->beatThresh) * f2 //if (f1 > b->beatThresh * f2 //f1 > f2
                )//&& ((cooldownTimer.getElapsedTime().asMilliseconds() - b->lastBeat.asMilliseconds()) > b->cooldown.asMilliseconds()) && b->beatCount != 0)
        {
            b->beatCount++;
            b->isBeat = true;
            b->lastBeat = cooldownTimer.getElapsedTime();
        } else {
            b->isBeat = false;
        }
    }

    sf::Vector2f tempSize;
    //float moveDelta = 4.0f;
    for (unsigned int i = 0; i < beatManager.size(); i++) {
        tempSize = m_rects[i]->getSize();
        float newY = 0;
        if (beatManager[i]->isBeat) {
            newY = Utils::lerp(tempSize.y, tempSize.y + config.moveDelta, 0.8);
        } else {
            float delta = tempSize.y - config.moveDelta;
            newY = Utils::lerp(tempSize.y, (delta > 20.0f ? delta : 20.0f), 0.5);
        }
        m_rects[i]->setSize(sf::Vector2f(tempSize.x, newY));
    }
}

void BeatDetection::addBeatChannel(int index, float thresh) {
    Beat* beat = new Beat(config.historyAvgCount, index, thresh, "test");
    beatManager.push_back(beat);
}

void BeatDetection::render(sf::RenderTarget *target) {
    for (const auto& r : m_rects) {
        target->draw(*r);
    }
}

BeatDetection::~BeatDetection() {
    for (auto* v : beatManager) {
        delete v->history;
        delete v;
    }
}
