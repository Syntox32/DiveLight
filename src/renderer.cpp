
#include "../include/renderer.hpp"

Renderer::Renderer(Config& clientConfig,
                   SoundInput *stream,
                   DiveFFT *fft,
                   std::mutex& lock)
    : m_config(clientConfig),
      m_stream(stream),
      m_lock(lock),
      m_invalidateFreqs(true)
{
    m_window = new sf::RenderWindow(sf::VideoMode(1280, 720), "rendererrr", sf::Style::Default);
    m_window->setFramerateLimit(clientConfig.renderFrameRate);

    m_menu = new Menu(clientConfig);

    m_visualizer = new Visualizer(clientConfig, stream, fft);
    m_fft = fft;

    // init buffers
    m_rawData = new float[m_fft->getOutDataCount()];
    m_magBuff = new float[m_fft->getOutDataCount()];
    m_ampBuff = new float[m_fft->getOutDataCount()];

    m_box = new BoxSpectrum(clientConfig.columnCount, m_window->getSize(), lock);
    //m_box->setFreqLimits(40, 4000, m_visualizer);
}

void Renderer::begin()
{
    loop();
}

void Renderer::loop()
{
    //std::lock_guard<std::mutex> lock(m_lock);

    while (m_window->isOpen())
    {
        sf::Event event;
        while (m_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window->close();
            }

            m_menu->poll(event);
        }

        m_stream->update(m_window);
        render();
    }
}

void Renderer::setDataPointer(float *data)
{
    m_rawData = data;
}

void Renderer::attachLeds(LEDClient *ledClient)
{
    if (m_config.ledStripEnabled)
    {
        m_led = ledClient;
        std::cout << "attached LED client..." << std::endl;
    }
}

void Renderer::attachBeatDetect(BeatDetection *beatDetect)
{
    if (m_config.enableBeatDetect)
    {
        m_beatDetect = beatDetect;
        m_beatDetect->configure(m_ampBuff, m_config.columnCount, m_visualizer);
        std::cout << "attached BeatDetection class..." << std::endl;

        Beat* b = new Beat(m_config.historyAvgCount, 0, 1.08f, "main");
        m_beatDetect->setMainBeat(b);

        m_beatDetect->addBeatChannel(0, 1.05f);
        m_beatDetect->addBeatChannel(1, 1.1f);
        for (int i = 2; i < m_config.columnCount; i++) {
            m_beatDetect->addBeatChannel(i, 1.15f);
        }
    }
}

void Renderer::prepareData()
{
    m_box->prepareData(m_ampBuff, m_fft->getOutDataCount(), m_visualizer, m_config);
    m_beatDetect->detect();

    float avgData[m_config.columnCount];
    m_visualizer->getAverages(m_ampBuff, m_config.columnCount, avgData, m_config.freqLow, m_config.freqHigh);

    if (m_config.ledStripEnabled)
    {
        m_led->setData(avgData, m_config.columnCount, m_beatDetect->getBeats());
        m_led->prepare();
    }
}

void Renderer::render()
{
    m_visualizer->applyPostFFTScaling(m_rawData, m_fft->getOutDataCount(), m_magBuff, m_ampBuff);

    prepareData();

    m_window->clear(sf::Color::Black);

    m_box->render(m_window);
    m_menu->render(m_window);
    m_beatDetect->render(m_window);
    m_stream->render(m_window);

    if (m_config.ledStripEnabled) {
        m_led->send();
        m_led->poll();
    }
    //m_led->render(); // TODO: Implement render function in LED class

    m_window->display();

    //if (m_config.ledStripEnabled)

}
