//
// Created by Anders on 30.09.2016.
//

#ifndef DIVELIGHT_LEDCLIENT_HPP
#define DIVELIGHT_LEDCLIENT_HPP

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>
#include <assert.h>
#include <SFML/System.hpp>
#include "easywsclient.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "beatdetection.hpp"

using easywsclient::WebSocket;

typedef unsigned char byte;

struct LED
{
    byte old_r, old_g, old_b;
    byte r, g, b;
    float alpha;

    LED(int c) : r((byte)c), g((byte)c), b((byte)c), alpha(1.0f),
            old_r((byte)c), old_g((byte)c), old_b((byte)c) { }
    LED(int c, float alpha) : r((byte)(c * alpha)), g((byte)(c * alpha)), b((byte)(c * alpha)), alpha(alpha),
            old_r((byte)c), old_g((byte)c), old_b((byte)c) { }
    LED(int r, int g, int b) : r((byte)r), g((byte)g), b((byte)b), alpha(1.0f),
            old_r((byte)r), old_g((byte)g), old_b((byte)b) { }
    LED(int r, int g, int b, float alpha) : r((byte)(r * alpha)), g((byte)(g * alpha)), b((byte)(b * alpha)), alpha(alpha),
            old_r((byte)r), old_g((byte)g), old_b((byte)b) { }

    void setColor(int c) {
        setColor(c, c, c);
    }

    void setColor(int cr, int cg, int cb) {
        old_r = (byte)cr;
        old_g = (byte)cg;
        old_b = (byte)cb;
        r = (byte)(old_r * alpha);
        g = (byte)(old_g * alpha);
        b = (byte)(old_b * alpha);
    }

    void setAlpha(float a) {
        alpha = a;
        r = (byte)(old_r * a);
        g = (byte)(old_g * a);
        b = (byte)(old_b * a);
    }

    std::string toStr() const {
        return std::to_string(g) + ","
               + std::to_string(r) + ","
               + std::to_string(b);
    }
};

class LEDClient
{
public:
    LEDClient(Config& config, std::mutex& lock);
    ~LEDClient();

    void setData(std::vector<LED> leds);
    void setData(float* data, unsigned int dataLen);
    void setData(float *avgData, int avgLen, std::vector<Beat*> *beatVec);
    bool send();
    void init();
    void poll();
    void prepare();
private:
    WebSocket *m_ws;
    std::string m_ledData;
    Config &m_config;

    float *averages;
    int averageLen;
    std::vector<Beat*> *beats;
    std::vector<LED> leds;

    bool startWsa();
};


#endif //DIVELIGHT_LEDCLIENT_HPP
