
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
    // m_rects = new std::vector<sf::RectangleShape>();

    m_menu = new Menu(clientConfig);
    m_visualizer = new Visualizer(clientConfig, stream);
    m_fft = fft;

    // init buffers
    m_rawData = new float[m_fft->getOutDataCount()]; // this gotta be thread safe
    m_magBuff = new float[m_fft->getOutDataCount()];
    m_ampBuff = new float[m_fft->getOutDataCount()];

    m_box = new BoxSpectrum(clientConfig.columnCount, m_window->getSize(), lock);
    m_box->setFreqLimits(40, 4000, stream->getSampleRate(), fft->getFFTSize());
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

void Renderer::prepareData()
{
    m_box->prepareData(m_ampBuff, m_fft->getOutDataCount(), m_visualizer);
    m_led->setData(m_ampBuff, 40, 255, 0, 0);
}

void Renderer::render()
{
    m_visualizer->applyPostFFTScaling(m_rawData, m_fft->getOutDataCount(), m_magBuff, m_ampBuff);

    prepareData();

    m_window->clear(sf::Color::Black);

    m_box->render(m_window);
    m_menu->render(m_window);
    m_led->send();
    //m_led->render(); // TODO: Implement render function in LED class

    m_window->display();

    if (m_config.ledStripEnabled)
        m_led->poll();
}
