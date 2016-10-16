//
// Created by Anders on 30.09.2016.
//

#include "../include/ledclient.hpp"

LEDClient::LEDClient(Config& config, std::mutex& lock)
    : m_config(config)
{

}

LEDClient::~LEDClient()
{
    delete m_ws;
}

void handle_message(const std::string& msg)
{
    std::cout << msg << std::endl;
}

void LEDClient::init()
{
    if (!startWsa()) std::cerr << "WSA Couldn't start for some reason" << std::endl;

    m_ws = WebSocket::from_url("ws://172.16.42.9:6969/divelight");
    //m_ws = WebSocket::from_url("ws://localhost:6969/divelight");
    assert(m_ws);

    while(m_ws->getReadyState() == WebSocket::CONNECTING)
    {
        sf::sleep(sf::seconds(0.1f));
        std::cout << "Waiting for websocket connection..." << std::endl;
    }

    if (m_ws->getReadyState() == WebSocket::OPEN)
        std::cout << "websocket opened" << std::endl;

    const int LEDCOUNT = 120;
    for (int i = 0; i < LEDCOUNT; i++) {
        leds.push_back(LED(255, 0, 0));
    }
}

bool LEDClient::startWsa()
{
#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc)
    {
        printf("WSAStartup Failed.\n");
        return false;
    }
#endif
    return true;
}

void stopWsa()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

bool LEDClient::send()
{
    m_ws->send(m_ledData);
}

void LEDClient::poll()
{
    m_ws->poll();
}

void LEDClient::setData(float *data, unsigned int dataLen)
{

}

void LEDClient::setData(float *avgData, int avgLen, std::vector<Beat*> *beatVec)
{
    averages = avgData;
    averageLen = avgLen;
    beats = beatVec;
}

std::string makePayload(std::vector<LED> leds)
{
    std::ostringstream stream;
    stream << "begin:" << leds.size() << ":";
    for (auto& l : leds) {
        stream << l.toStr() << ":";
    }
    stream << "end";
    return stream.str();
}

const float MIN = 2.0f;
const float MAX = 255.0f;
const float moveDelta = 20.0f;
const float decayUp = 0.8f;
const float decayDown = 0.2;
static float mainBeatValue = MIN;

void switchColor(std::vector<LED> &leds)
{
    static int c = 0;

    switch(c) {
        case 0: c = 1; for (auto& l : leds) l.setColor(0, 255, 0); break;
        case 1: c = 2; for (auto& l : leds) l.setColor(0, 0, 255); break;
        case 2: c = 0; for (auto& l : leds) l.setColor(255, 0, 0); break;
    }
}

void LEDClient::prepare()
{

    Beat* mainBeat = (*beats)[beats->size() - 1];

    bool isBeat = false;
    bool lowBeat = false;
    bool highBeat = false;
    int ISBEAT = 2;
    int beatCount = 0;
    int longestBeat = 0;
    int currentBeatCount = 0;


    for (auto* b : (*beats)) {
        if (b->isBeat) {
            currentBeatCount++;
        } else {
            if (currentBeatCount > longestBeat) longestBeat = currentBeatCount;
            currentBeatCount = 0;
        }
    }

    int lowBeatCount = 0;
    for (int i = 0; i < 4; i++) {
        if ((*beats)[i]->isBeat) lowBeatCount++;
    }
    if (lowBeatCount >= 3 || mainBeat->isBeat) {
        lowBeat = true;
        switchColor(leds);
    }

    /*
    for (auto* b : (*beats)) {
        if (b->isBeat) beatCount++;
    }
    */

    if (longestBeat >= ISBEAT || lowBeat) isBeat = true;
    //if (beatCount >= ISBEAT) isBeat = true;

    if (isBeat) {
        mainBeatValue = Utils::lerp(mainBeatValue, MAX, decayUp);
    } else {
        mainBeatValue = Utils::lerp(mainBeatValue, MIN, decayDown);
    }

    for (auto& l : leds) {
        l.setAlpha(Utils::norm(mainBeatValue, 0, 255));
    }

    m_ledData = makePayload(leds);

    //m_ws->send

    /*
    std::ostringstream stream;
    int count = 60; //m_config.columnCount;
    stream << "begin:" << ledCount << ":";
    int height = 2 * (int)Utils::map(data[index], 0, 500, 0, count);
    //std::cout << height << std::endl;
    for (unsigned int i = 0; i < count; i++)
    {
        if (i <= height)
        {
            stream << (int) g << "," << (int) r << "," << (int) b << ":";
        }
        else if (i > height)
        {
            stream << (int) 0 << "," << (int) 0 << "," << (int) 0 << ":";
        }
    }

    stream << "end";
    m_ledData = stream.str();
    m_ws->send(m_ledData);
    int i = 2323232;*/
}

void LEDClient::setData(std::vector<LED> leds)
{

}

