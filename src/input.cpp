
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../include/input.hpp"

SoundInput::SoundInput(DataCallback& dataCallback)
    : b_running(false),
      b_hasCallback(true),
      f_eventCallback(dataCallback),
      canTogglePause(true),
      invalidateProgressShape(true),
      canBackward(true),
      canForward(true)
{

}

SoundInput::~SoundInput() { }

void SoundInput::setFile(const std::string& path, FFTSize fftInputSize)
{
    m_stream = new DiveStream();
    m_format = InputFormat::File;
    m_fftSize = fftInputSize;

    std::string newPath = prepareTrack(path);

    // Load the file into the stream and play it.
    sf::SoundBuffer soundBuffer;
    soundBuffer.loadFromFile(newPath);

    trackDuration = soundBuffer.getDuration();

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

    m_stream->load(soundBuffer, channelCount * (unsigned int)fftInputSize, f_eventCallback);
}

const std::vector<Sample>& SoundInput::getSampleData(unsigned int sampleSize, unsigned int currentSample)
{
    switch (m_format)
    {
        case InputFormat::File:
            {
                const std::vector<sf::Int16>& samples = m_stream->getSamples();
                const size_t tCurrentSample = currentSample; //m_stream->currentSample();
                size_t currentSampleCount = sampleSize; //m_stream->currentSampleCount();
                unsigned int channelCount = m_stream->getChannelCount();

                // TODO: Fix indexOutOfRange when the song eventually comes to an end.

                m_samples.clear();

                if (channelCount == 1)
                {
                    // normalize the short values of the 2 channel audio and
                    // add it to the inData array
                    for (size_t i = 0; i < currentSampleCount; i++)
                    {
                        // USHRT_MAX makes the data <1, 0], while SHRT_MAX makes it (at least in this case) <2, 0]
                        m_samples.push_back(((unsigned short) samples[tCurrentSample + i]) / (float) SHRT_MAX);
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
                                      ((unsigned short) samples[tCurrentSample + i] +
                                      (unsigned short) samples[tCurrentSample + i + 1])
                                      / 2.0f) / (float) SHRT_MAX);
                    }
                }
                else if (channelCount > 2)
                {
                    std::cerr << "Channel count not supported." << std::endl;
                }

                // do some zero padding
                size_t remainder = (unsigned int)m_fftSize - (currentSampleCount / channelCount);
                for (size_t j = (currentSampleCount / channelCount); j < (unsigned int)m_fftSize; j++) {
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

void SoundInput::render(sf::RenderTarget *target)
{
    target->draw(*backdrop);
    target->draw(*progress);
}

void SoundInput::update(sf::RenderWindow *window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        sf::Time offset = m_stream->getPlayingOffset();
        m_stream->setPlayingOffset(offset - sf::seconds(1));
        canBackward = false;
    } else {
        canBackward = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        sf::Time offset = m_stream->getPlayingOffset();
        m_stream->setPlayingOffset(offset + sf::seconds(1));
        canForward = false;
    } else {
        canForward = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (m_stream->getStatus() == DiveStream::Playing && canTogglePause)
        {
            m_stream->pause();
            canTogglePause = false;
        }

        if (m_stream->getStatus() == DiveStream::Paused && canTogglePause) {
            m_stream->play();
            canTogglePause = false;
        }
    } else {
        canTogglePause = true;
    }

    if (invalidateProgressShape) {
        progress = new sf::RectangleShape(sf::Vector2f(0, 20));
        progress->setPosition(sf::Vector2f(0, 0)); //window->getSize().y - progress->getSize().y));
        progress->setFillColor(sf::Color(75, 105, 130));

        backdrop = new sf::RectangleShape(sf::Vector2f(window->getSize().x, 20));
        backdrop->setPosition(sf::Vector2f(0, 0)); //window->getSize().y - backdrop->getSize().y));
        backdrop->setFillColor(sf::Color(80, 80, 80));

        invalidateProgressShape = false;
    }

    float offset = m_stream->getPlayingOffset().asMicroseconds();
    float durr = trackDuration.asMicroseconds();
    float norm = Utils::norm(offset, 0, durr);

    progress->setSize(sf::Vector2f(window->getSize().y * norm, progress->getSize().y));
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

// http://stackoverflow.com/a/478960
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

std::string SoundInput::prepareTrack(const std::string& filePath)
{
    // TODO: use boost of some other filesystem library
    std::string resPath = filePath;
    bool force = false;

    if (Utils::stringContains(resPath, "u:")) {
        std::string url = resPath.substr(2, resPath.length());

        std::string cmdFilename = "youtube-dl --audio-format mp3 -x --no-playlist --get-filename " + url;
        std::string linkFilename = exec(cmdFilename.c_str());
        //system(cmdFilename.c_str());

        std::string cmd = "youtube-dl --audio-format mp3 -x --no-playlist " + url;
        std::string ret = exec(cmd.c_str());

        std::cout << "Downloaded mp3." << std::endl;
        resPath = linkFilename;
        force = true;

    } else if (Utils::stringContains(resPath, "\"")) {
        resPath.erase(
                std::remove(resPath.begin(), resPath.end(), '\"'),
                resPath.end()
        );
    }


    // TODO: Experiment with the c++ standard library experimental filesystem
    //using namespace std::experimental::filesystem;
    //onst char* a = filePath.c_str();
    //path file_path<const char*>(a);
    //file_path.

    // Convert the file to .wav using ffmpeg, if it's an mp3.
    if (Utils::stringContains(resPath, ".mp3") || force)
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