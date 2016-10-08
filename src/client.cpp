
#include "../include/client.hpp"


DiveClient::DiveClient()
    : m_config()
{

    std::cout << "Starting DiveClient..." << std::endl;

    // TODO: Implement windows loopback streaming
    // init the stream if we use file stuff
    //m_stream = new DiveStream();

    m_fft = new DiveFFT(m_config.fftSize);
    m_dataIn = new kiss_fft_scalar[m_fft->getFFTSize()];
    m_dataOut = new kiss_fft_cpx[m_fft->getOutDataCount()];

    // give the fft the buffer pointers it needs to do operations
    // we only really need to do this once
    m_fft->setData(m_dataIn, m_dataOut, m_fft->getFFTSize());

    // buffers to hold the magnitude values of the fft
    m_magBuffLen = m_fft->getOutDataCount();
    m_rawData = new float[m_fft->getOutDataCount()];
    m_magBuff = new float[m_fft->getOutDataCount()];
    m_ampBuff = new float[m_fft->getOutDataCount()];
}

void DiveClient::init()
{
    using namespace std::placeholders;

#ifdef _WIN32
   //std::string inPath = "\"D:\\Library\\Music\\Soundcloud\\Ambient\\Trentemoller - Miss You - Copy.wav\"";
    std::string inPath = "\"D:\\Library\\Music\\Freestylers - Cracks Ft Belle Humble Flux Pavilion Remix.mp3\"";
#elif __linux__
    std::string inPath = "/home/syn/Dropbox/Dev/Alarm/music/higher-love.wav-edit.wav";
#endif

    std::cout << "Audio path: \n";
    //std::getline(std::cin, inPath);

    unsigned int fps = m_config.ledRefreshRate;
    if (m_config.ledStripEnabled)
    {
        m_ledClient = new LEDClient(m_config, m_lock);
        m_ledClient->init();
        m_ledClient->send();
    }

    //m_config.ledStripEnabled = false;
    DataCallback callback = std::bind(&DiveClient::step, this, _1, _2);
    m_soundInput = new SoundInput(callback);

    m_soundInput->setFile(inPath);
    m_soundInput->begin();

    m_renderer = new Renderer(m_config, m_soundInput, m_fft, m_lock);
    m_renderer->attachLeds(m_ledClient);
    m_renderer->setDataPointer(m_rawData);

    m_renderer->begin();
}

void DiveClient::step(unsigned int sampleSize, unsigned int currentSample)
{
    // TODO: Handle edge cases with lower samples sizes
    const std::vector<Sample>& samples = m_soundInput->getSampleData();
    for (std::size_t i = 0; i < samples.size(); i++)
    {
        m_dataIn[i] = (float) samples[i];
    }

    switch (m_config.windowFunction)
    {
        case WindowFunction::Hann:
            m_fft->windowHann();
            break;
        case WindowFunction::Hamming:
            m_fft->windowHamming();
            break;
        case WindowFunction::Blackman:
            m_fft->windowBlackman();
            break;
        case WindowFunction::WindowNone:
            break;
    }

    // perform fft
    m_fft->doFFT();

    // TODO: Move the rest of this to own function?
    m_magBuffLen = m_fft->getOutDataCount();
    float amp, mag;

    //std::lock_guard<std::mutex> lock_guard(m_lock);
    for (unsigned int k = 0; k < m_magBuffLen; k++) {
        m_rawData[k] = m_dataOut[k].r * m_dataOut[k].r;

        /*
        //if (k < 20) std::cout << m_dataOut[k].r << " ";
        // we only take the magnitude of the real part, because
        // the complex part is always zero when doing a real to complex transform
        amp = (float)sqrt(m_dataOut[k].r * m_dataOut[k].r);
        // TODO: Check if 'amp' is not 'mag'
        m_ampBuff[k] = amp;

        std::lock_guard<std::mutex> lock_guard(m_lock);
        // TODO: Implement other scaling methods (lin, log, custom)
        mag = (float)(20.0f * log10(amp));
        //mag = (float)(4.0f * sqrt(amp));
        if (mag < 0.0f) mag = 0.0f;
        m_magBuff[k] = mag;
     */
    }
}

DiveClient::~DiveClient()
{
    // I think this is how I clean up things
    delete m_soundInput;
    delete m_fft;
    delete m_dataIn;
    delete m_dataOut;
    delete m_ampBuff;
    delete m_magBuff;
    delete m_rawData;
    delete m_renderer;

    if (m_config.ledStripEnabled)
        delete m_ledClient;
}