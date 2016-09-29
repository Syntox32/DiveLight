
#include "client.hpp"


DiveClient::DiveClient()
{
    std::cout << "Starting DiveClient..." << std::endl;

    // TODO: Implement windows loopback streaming
    // init the stream if we use file stuff
    m_stream = new DiveStream();

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

    m_renderer = new ColumnRenderer(20);
    m_renderer->beginLoop();
}

bool stringContains(const std::string& str, const std::string& contains)
{
    return (size_t)str.find(contains) != std::string::npos;
}

std::string prepareTrack(const std::string& path)
{
    std::string resPath = path;
    if (stringContains(resPath, "\""))
    {
        resPath.erase(
                std::remove(resPath.begin(), resPath.end(), '\"'),
                resPath.end()
        );
    }

    // Convert the file to .wav using ffmpeg, if it's an mp3.
    if (stringContains(path, ".mp3"))
    {
        std::string cmd = "ffmpeg -i \"" + resPath + "\" \"" + resPath+ ".wav\"";
        system(cmd.c_str()); // TODO: fix getting unicode error with this one
        std::cout << "Converted mp3 to wav." << std::endl;
        resPath += ".wav";
    }
    else
    {
        std::cout << "Didn't detect file as MP3; assuming WAV, OGG or FLAC." << std::endl;
    }

    return resPath;
}

/*
void step(int i, int k) {
    std::cout << "shit" << std::endl;
}
*/

void DiveClient::step(int sampleSize, int currentSample)
{
    // TODO: Handle edge cases with lower samples sizes
    const std::vector<sf::Int16>& samples = m_stream->getSamples();
    currentSample -= sampleSize;

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

    // attempt to do zero padding
    int remainder = m_fft->getFFTSize() - (sampleSize / 2);
    for (int j = (sampleSize / 2); j < m_fft->getFFTSize(); j++) {
        m_dataIn[j] = 0.0f;
    }

    // TODO: Do zero padding when sampleSize is not equal to FFT size
    //int remainder = (m_fft->getFFTSize() / 2) - (sampleSize / 2);
    //if (remainder != 0)
    //{
    //    for (int j = (sampleSize / 2); j < m_fft->getFFTSize() / 2; j++)
    //   {
    //        m_intermediate[j]
    //    }
    //}

    // TODO: Window function selection
    m_fft->windowHann();
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
        // TODO: Implement other scaling methods (lin, log, custom)
        mag = (float)(20.0f * log10(amp));
        m_magBuff[k] = mag;
    }

    m_renderer->setData(m_magBuff);

    //float amp = sqrt(m_dataOut[12].i * m_dataOut[12].i + m_dataOut[12].r * m_dataOut[12].r);
    //float mag = 20.0f * log10(amp);
    //std::cout << "mag: " << mag << std::endl;
}

void DiveClient::begin()
{
    std::string inPath;
    std::cout << "Audio path: \n";
    std::getline(std::cin, inPath);

    inPath = prepareTrack(inPath);

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

    m_stream->load(soundBuffer, m_channelCount * m_fft->getFFTSize(),
                   std::bind(&DiveClient::step, this,
                             std::placeholders::_1, std::placeholders::_2));
    m_stream->play();

    while (m_stream->getStatus() == DiveStream::Playing)
    {
        sf::sleep(sf::seconds(0.1f));
    }
}