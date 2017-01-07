#include "speaker_detect.h"

extern QLabel *notifications;
extern QLabel *record_notifications;

struct WAVEFILEHEADER
{
    // don't change the order of attributes
    char m_lpcChunkId[4];
    int m_iChunkSize;
    char m_lpcFormat[4];
    char m_lpcSubChunkFmt[4];
    int m_iSubChunkFmtSize;
    short int m_siAudioFormat;
    short int m_siNumChannels;

    int m_iSampleRate;
    int m_iByteRate;
    short int m_siBlockAlign;
    short int m_siBitsPerSample;

    char m_lpcChunkData[4];
    int m_iSubChunkDataSize;
    unsigned char * m_data;
};


template <typename T>
void write(std::ofstream& stream, const T& t) {
    stream.write((const char*)&t, sizeof(T));
}

template <typename T>
void writeFormat(std::ofstream& stream) {
    write<short>(stream, 1);
}

template <>
void writeFormat<float>(std::ofstream& stream) {
    write<short>(stream, 3);
}

template <typename SampleType>
void writeWAVData(
    string outFile,
    SampleType* buf,
    size_t bufSize,
    int sampleRate,
    short channels)
{
    std::ofstream stream(outFile, std::ios::binary);
    stream.write("RIFF", 4);
    write<int>(stream, 36 + bufSize);
    stream.write("WAVE", 4);
    stream.write("fmt ", 4);
    write<int>(stream, 16);
    writeFormat<SampleType>(stream);                                // Format
    write<short>(stream, channels);                                 // Channels
    write<int>(stream, sampleRate);                                 // Sample Rate
    write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate
    write<short>(stream, channels * sizeof(SampleType));            // Frame size
    write<short>(stream, 16 * sizeof(SampleType));                   // Bits per sample
    stream.write("data", 4);
    stream.write((const char*)&bufSize, 4);
    stream.write((const char*)buf, bufSize);
}

void countdownTimer(time_t &currentTime, time_t &lastTime){

    // Wait for three seconds to prompt the user 
    for (int i = 3; i > 0; i--) 
    { 
        string notifications_tmp = "Starting capture in " + to_string(i) + "...";
        record_notifications->setText(QString::fromStdString(notifications_tmp));
        record_notifications->repaint();
        usleep(100000); 
    } 
    string notifications_tmp = "Recording...";
    record_notifications->setText(QString::fromStdString(notifications_tmp));
    record_notifications->repaint();
}


void record(double record_time, string wavFile, bool timer_on) { 
    const ALCchar *   devices; 
    const ALCchar *         ptr; 
    ALCdevice *       mainDev; 
    ALCcontext *      mainContext; 
    ALCdevice *       captureDev; 
    ALubyte           captureBuffer[1048576]; 
    ALubyte           *captureBufPtr; 
    ALint             samplesAvailable; 
    ALint             samplesCaptured; 
    time_t            currentTime; 
    time_t            lastTime; 
    ALuint            buffer; 
    ALuint            source; 
    ALint             playState; 
    int               i; 


    // Open a playback device and create a context first 
    mainDev = alcOpenDevice(NULL); 
    if (mainDev == NULL) 
    { 
      printf("Unable to open playback device!\n"); 
      exit(1); 
    } 
    devices = alcGetString(mainDev, ALC_DEVICE_SPECIFIER); 
    mainContext = alcCreateContext(mainDev, NULL); 
    if (mainContext == NULL) 
    { 
      printf("Unable to create playback context!\n"); 
      exit(1); 
    } 

    // Make the playback context current 
    alcMakeContextCurrent(mainContext); 
    alcProcessContext(mainContext); 


    // Open the default device 
    captureDev = alcCaptureOpenDevice(NULL, 8000, AL_FORMAT_MONO16, 800); 
    if (captureDev == NULL) 
    {  
      printf("   Unable to open device!\n"); 
      exit(1); 
    } 
    devices = alcGetString(captureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 

    // Only turn timer on for training 
    if(timer_on)
        countdownTimer(currentTime,lastTime);

    fflush(stdout); 
    // lastTime = currentTime; 
    // lastTime = currentTime = time(NULL);
    // Capture (roughly) five seconds of audio 
    alcCaptureStart(captureDev); 
    samplesCaptured = 0; 
    captureBufPtr = captureBuffer; 
    // while (currentTime < (lastTime + record_time)) 
    for(int i = 0; i < 90 * record_time; i++)
    { 
        // Get the number of samples available 
        alcGetIntegerv(captureDev, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable); 

        // Copy the samples to our capture buffer 
        if (samplesAvailable > 0) 
        { 
            alcCaptureSamples(captureDev, captureBufPtr, samplesAvailable); 
            samplesCaptured += samplesAvailable; 
            // printf("Captured %d samples (adding %d)\r", samplesCaptured, samplesAvailable); 
            // fflush(stdout); 

            // Advance the buffer (two bytes per sample * number of samples) 
            captureBufPtr += samplesAvailable * 2; 
        } 

        // Wait for a bit 
        usleep(10000); 

        // Update the clock 
        // currentTime = time(NULL); 
    } 
    
    // printf("\nDone capturing.\n"); 
    alcCaptureStop(captureDev); 

    if(timer_on){
        string notifications_tmp = "Recording Done!";
        record_notifications->setText(QString::fromStdString(notifications_tmp));
        record_notifications->repaint();
    }

    // Generate an OpenAL buffer for the captured data 
    alGenBuffers(1, &buffer); 
    alBufferData(buffer, AL_FORMAT_MONO16, captureBuffer,samplesCaptured*2, 8000); 

    // Save recoreded buffer to WAV file

    ALint sizeInBytes;
    ALint channels;
    ALint bits;
    ALint audioData;

    alGetBufferi(buffer, AL_SIZE, &sizeInBytes);
    alGetBufferi(buffer, AL_CHANNELS, &channels);
    alGetBufferi(buffer, AL_BITS, &bits);

    ALint frequency;

    alGetBufferi(buffer, AL_FREQUENCY, &frequency);

    float lengthInSamples = sizeInBytes * 8 / (channels * bits);
    float durationInSeconds = (float)lengthInSamples / (float)frequency;

    unsigned char* outBuffer = new unsigned char[sizeInBytes];
    for(int i=0; i<sizeInBytes; i++)
    	outBuffer[i] = (unsigned char) captureBuffer[i];

    writeWAVData(wavFile, outBuffer, sizeInBytes, (int)frequency, 1);

    // Shut down OpenAL 
    alDeleteSources(1, &source); 
    alDeleteBuffers(1, &buffer); 
    alcMakeContextCurrent(NULL); 
    alcCloseDevice(mainDev); 
    alcCaptureCloseDevice(captureDev); 
}
