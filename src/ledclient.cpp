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
    //m_ws->send(m_ledData);
}

void LEDClient::poll()
{
    m_ws->poll();
}

void LEDClient::setData(float *data, unsigned int dataLen)
{

}

void LEDClient::setData(float *data, unsigned int index, byte r, byte g, byte b)
{
    std::ostringstream stream;
    int count = 60; //m_config.columnCount;
    stream << "begin:" << count << ":";
    int height = 5 * (int)Utils::map(data[index], 0, 500, 0, count);
    std::cout << height << std::endl;
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
    int i = 2323232;
}

void LEDClient::setData(std::vector<LED> leds)
{

}

