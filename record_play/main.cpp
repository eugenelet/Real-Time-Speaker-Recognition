#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <iostream>
#include <unistd.h>
#include <AL/al.h> 
#include <AL/alc.h> 
#include <sys/time.h>
#include <ctime> 
#include <fstream>

using namespace std;

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

void saveWav(int bits, int sizeInBytes, int samplerate, ALubyte* audioData){
	ofstream f( "example.wav", ios::binary );

	WAVEFILEHEADER whdr;
	FILE* fid = fopen("newaudio.wav","wb"); // important to use binary file
	whdr.m_lpcChunkId[0] = 'R';
	whdr.m_lpcChunkId[1] = 'I';
	whdr.m_lpcChunkId[2] = 'F';
	whdr.m_lpcChunkId[3] = 'F';
    whdr.m_iChunkSize = 0;
    whdr.m_lpcFormat[0] = 'W';
    whdr.m_lpcFormat[1] = 'A';
    whdr.m_lpcFormat[2] = 'V';
    whdr.m_lpcFormat[3] = 'E';
    whdr.m_lpcSubChunkFmt[0] = 'f';
    whdr.m_lpcSubChunkFmt[1] = 'm';
    whdr.m_lpcSubChunkFmt[2] = 't';
    whdr.m_lpcSubChunkFmt[3] = ' ';
    whdr.m_iSubChunkFmtSize = 16;
    whdr.m_siAudioFormat = 1;
    whdr.m_siNumChannels = 1;

    whdr.m_iSampleRate = samplerate;
    whdr.m_iByteRate = samplerate*sizeof(int);
    whdr.m_siBlockAlign = sizeof(int);
    whdr.m_siBitsPerSample = bits;

    whdr.m_lpcChunkData[0] = 'd';
    whdr.m_lpcChunkData[1] = 'a';
    whdr.m_lpcChunkData[2] = 't';
    whdr.m_lpcChunkData[3] = 'a';
    whdr.m_iSubChunkDataSize = sizeInBytes*sizeof(int);

    unsigned char* m_data = new unsigned char[sizeInBytes*sizeof(unsigned char)];
    for(int i = 0; i < sizeInBytes; i++)
    	m_data[i] = audioData[i];

    whdr.m_data = m_data;


	fwrite(&whdr, 1, sizeof(WAVEFILEHEADER), fid);

}




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
  char const* outFile,
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

int main(void) 
{ 
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

// Print the list of capture devices 
printf("Available playback devices:\n");

devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER); 
ptr = devices; 

while (*ptr)
{ 
   printf("   %s\n", ptr); 
   ptr += strlen(ptr) + 1; 
} 

// Open a playback device and create a context first 
printf("Opening playback device:\n"); 
mainDev = alcOpenDevice(NULL); 
if (mainDev == NULL) 
{ 
  printf("Unable to open playback device!\n"); 
  exit(1); 
} 
devices = alcGetString(mainDev, ALC_DEVICE_SPECIFIER); 
printf("   opened device '%s'\n", devices); 
mainContext = alcCreateContext(mainDev, NULL); 
if (mainContext == NULL) 
{ 
  printf("Unable to create playback context!\n"); 
  exit(1); 
} 
printf("   created playback context\n"); 

// Make the playback context current 
alcMakeContextCurrent(mainContext); 
alcProcessContext(mainContext); 

// Print the list of capture devices 

printf("Available capture devices:\n"); 
devices = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
ptr = devices; 

while (*ptr)
{ 
   printf("   %s\n", ptr); 
   ptr += strlen(ptr) + 1; 
}

// Open the default device 
printf("Opening capture device:\n"); 
captureDev = alcCaptureOpenDevice(NULL, 8000, AL_FORMAT_MONO16, 800); 
if (captureDev == NULL) 
{  
  printf("   Unable to open device!\n"); 
  exit(1); 
} 
devices = alcGetString(captureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
printf("   opened device %s\n", devices); 

// Wait for three seconds to prompt the user 
for (i = 3; i > 0; i--) 
{ 
  printf("Starting capture in %d...\r", i); 
  fflush(stdout); 
  lastTime = time(NULL); 
  currentTime = lastTime; 
  while (currentTime == lastTime) 
  { 
     currentTime = time(NULL); 
     usleep(100000); 
  } 
} 

printf("Starting capture NOW!\n"); 
fflush(stdout); 
lastTime = currentTime; 

// Capture (roughly) five seconds of audio 
alcCaptureStart(captureDev); 
samplesCaptured = 0; 
captureBufPtr = captureBuffer; 
while (currentTime < (lastTime + 5)) 
{ 
  // Get the number of samples available 
  alcGetIntegerv(captureDev, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable); 

  // Copy the samples to our capture buffer 
  if (samplesAvailable > 0) 
  { 
     alcCaptureSamples(captureDev, captureBufPtr, samplesAvailable); 
     samplesCaptured += samplesAvailable; 
     printf("Captured %d samples (adding %d)\r", samplesCaptured, 
        samplesAvailable); 
     fflush(stdout); 

     // Advance the buffer (two bytes per sample * number of samples) 
     captureBufPtr += samplesAvailable * 2; 
  } 

  // Wait for a bit 
  usleep(10000); 

  // Update the clock 
  currentTime = time(NULL); 
} 
/*printf("\nPausing capture.\n"); 
alcCaptureStop(captureDev); 

// Wait for three seconds to prompt the user 
for (i = 3; i > 0; i--) 
{ 
  printf("Resuming capture in %d...\r", i); 
  fflush(stdout); 
  lastTime = time(NULL); 
  currentTime = lastTime; 
  while (currentTime == lastTime) 
  { 
     currentTime = time(NULL); 
     usleep(100000); 
  } 
} 

printf("Resuming capture NOW!\n"); 
fflush(stdout); 
lastTime = currentTime; 

// Capture (roughly) five seconds of audio 
alcCaptureStart(captureDev); 
while (currentTime < (lastTime + 5)) 
{ 
  // Get the number of samples available 
  alcGetIntegerv(captureDev, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable); 

  // Copy the samples to our capture buffer 
  if (samplesAvailable > 0) 
  { 
     alcCaptureSamples(captureDev, captureBufPtr, samplesAvailable); 
     samplesCaptured += samplesAvailable; 
     printf("Captured %d samples (adding %d)\r", samplesCaptured, 
        samplesAvailable); 
     fflush(stdout); 

     // Advance the buffer (two bytes per sample * number of samples) 
     captureBufPtr += samplesAvailable * 2; 
  } 

  // Wait for a bit 
  usleep(10000); 

  // Update the clock 
  currentTime = time(NULL); 
} 
*/
printf("\nDone capturing.\n"); 
alcCaptureStop(captureDev); 

// Play back the captured data 
printf("Starting playback...\n"); 
fflush(stdout); 

// Generate an OpenAL buffer for the captured data 
alGenBuffers(1, &buffer); 
alGenSources(1, &source); 
alBufferData(buffer, AL_FORMAT_MONO16, captureBuffer,samplesCaptured*2, 8000); 
alSourcei(source, AL_BUFFER, buffer); 
alSourcePlay(source); 

// Wait for the source to stop playing 
playState = AL_PLAYING; 
while (playState == AL_PLAYING) 
{ 
  printf("  source %d is playing...\r", source); 
  fflush(stdout); 
  alGetSourcei(source, AL_SOURCE_STATE, &playState); 
  usleep(100000); 
} 
printf("\nDone with playback.\n"); 
fflush(stdout); 

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


cout << "Size In Bytes: " << sizeInBytes << endl;
cout << "Channels: " << channels << endl;
cout << "bits: " << bits << endl;
cout << "Duration in Seconds: " << durationInSeconds << endl;
cout << "Length in Samples: " << lengthInSamples << endl;

// alGetBufferi(buffer, AL_DATA, &audioData);

// saveWav(bits, sizeInBytes, (int)frequency, captureBuffer);
unsigned char* outBuffer = new unsigned char[sizeInBytes];
for(int i=0; i<sizeInBytes; i++)
	outBuffer[i] = (unsigned char) captureBuffer[i];

writeWAVData("mySound.wav", outBuffer, sizeInBytes, (int)frequency, 1);


// Shut down OpenAL 
alDeleteSources(1, &source); 
alDeleteBuffers(1, &buffer); 
alcMakeContextCurrent(NULL); 
alcCloseDevice(mainDev); 
alcCaptureCloseDevice(captureDev); 
}
