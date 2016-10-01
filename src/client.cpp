
#include "../include/client.hpp"


DiveClient::DiveClient()
{
    std::cout << "Starting DiveClient..." << std::endl;

    // TODO: Implement windows loopback streaming
    // init the stream if we use file stuff
    //m_stream = new DiveStream();

    m_fft = new DiveFFT(FFTSize::FFT2048);
    m_dataIn = new kiss_fft_scalar[m_fft->getFFTSize()];
    m_dataOut = new kiss_fft_cpx[m_fft->getOutDataCount()];

    // give the fft the buffer pointers it needs to do operations
    // we only really need to do this once
    m_fft->setData(m_dataIn, m_dataOut, m_fft->getFFTSize());

    // buffers to hold the magnitude values of the fft
    m_magBuffLen = m_fft->getOutDataCount();
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

    unsigned int fps = 60;
    m_ledClient = new LEDClient(fps, m_lock);
    m_ledClient->init();
    //m_ledClient->send();

    DataCallback callback = std::bind(&DiveClient::step, this, _1, _2);
    m_soundInput = new SoundInput(callback);

    m_soundInput->setFile(inPath);
    m_soundInput->begin();

    m_renderer = new ColumnRenderer(30, m_lock);
    m_renderer->setDataPointer(m_magBuff, m_soundInput->getSampleRate(), m_fft->getOutDataCount(), m_fft->getFFTSize());
    m_renderer->attachLeds(m_ledClient);
    m_renderer->loop();

    /*
    while (m_soundInput->isRunning())
    {
        sf::sleep(sf::seconds(0.1f));
    }
    */
}

void DiveClient::step(unsigned int sampleSize, unsigned int currentSample)
{
    // TODO: Handle edge cases with lower samples sizes
    //const std::vector<sf::Int16>& samples = m_stream->getSamples();
    //currentSample -= sampleSize;
    //const std::vector<uint16_t>& samples = m_soundInput->getSampleData();

    /*
    // normalize the short values of the 2 channel audio and
    // add it to the inData array
    for (int i = 0; i < sampleSize; i += 2) {
        // USHRT_MAX makes the data <1, 0], while SHRT_MAX makes it (at least in this case) <2, 0]
        m_dataIn[i / 2] = (
              ((unsigned short)samples[currentSample + i] +
               (unsigned short)samples[currentSample + i + 1])
              / 2.0f) / (float)SHRT_MAX;
        //int c = samples[currentSample + i] << 8 | samples[currentSample + i + 1];
        //m_intermediate[i / 2] = c / 32768.0f;
        //m_dataIn[i / 2] = 12313.0f; (float)samples[currentSample + i];
        //std::cout << std::fixed;
        //std::cout << m_intermediate[i / 2] << std::endl;
    }
 */
/*
    // attempt to do zero padding
    int remainder = m_fft->getFFTSize() - (sampleSize / 2);
    for (int j = (sampleSize / 2); j < m_fft->getFFTSize(); j++) {
        m_dataIn[j] = 0.0f;
    }
    */

    // TODO: Do zero padding when sampleSize is not equal to FFT size
    //int remainder = (m_fft->getFFTSize() / 2) - (sampleSize / 2);
    //if (remainder != 0)
    //{
    //    for (int j = (sampleSize / 2); j < m_fft->getFFTSize() / 2; j++)
    //   {
    //        m_intermediate[j]
    //    }
    //}

    const std::vector<Sample>& samples = m_soundInput->getSampleData();
    for (std::size_t i = 0; i < samples.size(); i++)
    {
        m_dataIn[i] = (float) samples[i];
    }

    // TODO: Window function selection
    m_fft->windowBlackman();
    // perform fft
    m_fft->doFFT();

    // TODO: Move the rest of this to own function?
    m_magBuffLen = m_fft->getOutDataCount();
    float amp, mag;

    for (unsigned int k = 0; k < m_magBuffLen; k++) {
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

        //std::cout << amp << std::endl;
    }

    //nt lmao = m_renderer->getInt();
    //m_renderer->setShit(nullptr);

    //float ampa = sqrt(m_dataOut[12].i * m_dataOut[12].i + m_dataOut[12].r * m_dataOut[12].r);
    //float maga = 20.0f * log10(ampa);
    //std::cout << "mag: " << maga << std::endl;
}

/*
void DiveClient::begin()
{
    std::string inPath;
    std::cout << "Audio path: \n";
    std::getline(std::cin, inPath);

    inPath = ""; //prepareTrack(inPath);

    // TODO: Put this code into a LoadFromFile function
    // Load the file into the stream and play it.
    sf::SoundBuffer soundBuffer;
    soundBuffer.loadFromFile(inPath);

    std::cout << soundBuffer.getChannelCount() << std::endl;
    std::cout << soundBuffer.getDuration().asSeconds() << std::endl;
    std::cout << soundBuffer.getSampleCount() << std::endl;
    std::cout << soundBuffer.getSampleRate() << std::endl;

    m_channelCount = soundBuffer.getChannelCount();
    m_sampleCount = soundBuffer.getSampleCount();
    m_sampleRate = soundBuffer.getSampleRate();

    // TODO: Support mono sound
    if (m_channelCount != 2)
    {
        std::cerr << "error: channelCount not supported." << std::endl;
        exit(1);
    }

    //m_stream->load(soundBuffer, m_channelCount * m_fft->getFFTSize(),
     //              std::bind(&DiveClient::step, this,
      //                       std::placeholders::_1, std::placeholders::_2));
    m_stream->play();

    while (m_stream->getStatus() == DiveStream::Playing)
    {
        sf::sleep(sf::seconds(0.1f));
    }
}
 */