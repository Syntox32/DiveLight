
#include "../include/input.hpp"

SoundInput::SoundInput(DataCallback& dataCallback)
    : b_running(false),
      b_hasCallback(true),
      f_eventCallback(dataCallback)
{ }

SoundInput::~SoundInput() { }

void SoundInput::setFile(const std::string& path, unsigned int fftInputSize)
{
    m_stream = new DiveStream();
    m_format = InputFormat::File;
    m_fftSize = fftInputSize;

    std::string newPath = prepareTrack(path);

    // Load the file into the stream and play it.
    sf::SoundBuffer soundBuffer;
    soundBuffer.loadFromFile(newPath);

    std::cout << soundBuffer.getChannelCount() << std::endl;
    std::cout << soundBuffer.getDuration().asSeconds() << std::endl;
    std::cout << soundBuffer.getSampleCount() << std::endl;
    std::cout << soundBuffer.getSampleRate() << std::endl;

    unsigned int channelCount = soundBuffer.getChannelCount();

    // TODO: Support mono sound
    if (channelCount > 2)
    {
        std::cerr << "error: channelCount not supported." << std::endl;
        exit(1);
    }

    m_stream->load(soundBuffer, channelCount * fftInputSize, f_eventCallback);
}

const std::vector<Sample>& SoundInput::getSampleData()
{
    switch (m_format)
    {
        case InputFormat::File:
            {
                const std::vector<sf::Int16>& samples = m_stream->getSamples();
                const size_t currentSample = m_stream->currentSample();
                size_t currentSampleCount = m_stream->currentSampleCount();
                unsigned int channelCount = m_stream->getChannelCount();

                m_samples.clear();

                if (channelCount == 1)
                {
                    // normalize the short values of the 2 channel audio and
                    // add it to the inData array
                    for (size_t i = 0; i < currentSampleCount; i++)
                    {
                        // USHRT_MAX makes the data <1, 0], while SHRT_MAX makes it (at least in this case) <2, 0]
                        m_samples.push_back(((unsigned short) samples[currentSample + i]) / (float) SHRT_MAX);
                    }
                }
                else if (channelCount == 2)
                {
                    // normalize the short values of the 2 channel audio and
                    // add it to the inData array
                    for (size_t i = 0; i < currentSampleCount; i += 2)
                    {
                        // USHRT_MAX makes the data <1, 0], while SHRT_MAX makes it (at least in this case) <2, 0]
                        m_samples.push_back((
                                      ((unsigned short) samples[currentSample + i] +
                                      (unsigned short) samples[currentSample + i + 1])
                                      / 2.0f) / (float) SHRT_MAX);
                    }
                }
                else if (channelCount > 2)
                {
                    std::cerr << "Channel count not supported." << std::endl;
                }

                // do some zero padding
                size_t remainder = m_fftSize - (currentSampleCount / channelCount);
                for (size_t j = (currentSampleCount / channelCount); j < m_fftSize; j++) {
                    m_samples.push_back(0.0f);
                }
            }
        break;
    }

    return m_samples;
}

void SoundInput::setLoopback()
{
    m_format = InputFormat::Loopback;
    // TODO: Init WASAPI loopback and prepare the samples for the callback function
}

void SoundInput::begin()
{
    switch (m_format)
    {
        case InputFormat::File:
        {
            b_running = true;
            m_streamThread = std::thread(&SoundInput::beginFileStream, this);
        }
        break;
    }
}

void SoundInput::beginFileStream()
{
    m_stream->play();

    while (m_stream->getStatus() == DiveStream::Playing)
        sf::sleep(sf::seconds(0.1f));
}

void SoundInput::stop()
{
    if (m_format == InputFormat::File)
    {
        if (m_stream->getStatus() == DiveStream::Playing)
            m_stream->stop();

        b_running = false;
    }
}

std::string SoundInput::prepareTrack(const std::string& path)
{
    // TODO: use boost of some other filesystem library
    std::string resPath = path;
    if (Utils::stringContains(resPath, "\""))
    {
        resPath.erase(
                std::remove(resPath.begin(), resPath.end(), '\"'),
                resPath.end()
        );
    }

    // Convert the file to .wav using ffmpeg, if it's an mp3.
    if (Utils::stringContains(path, ".mp3"))
    {
        std::string cmd = "ffmpeg -y -i \"" + resPath + "\" \"" + resPath+ ".wav\"";
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