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

using easywsclient::WebSocket;

typedef unsigned char byte;

struct LED
{
    byte r, g, b;
};

class LEDClient
{
public:
    LEDClient(Config& config, std::mutex& lock);
    ~LEDClient();

    void setData(std::vector<LED> leds);
    void setData(float* data, unsigned int dataLen);
    void setData(float* data, unsigned int dataLen, byte r, byte g, byte b);
    bool send();
    void init();
    void poll();
private:
    WebSocket *m_ws;
    std::string m_ledData;
    Config &m_config;

    bool startWsa();
};


#endif //DIVELIGHT_LEDCLIENT_HPP
