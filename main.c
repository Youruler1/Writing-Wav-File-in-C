#include <stdio.h>
#include<string.h>
#include <stdint.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define DURATION 5
#define FREQUENCY 440.0

typedef struct {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
} WAVHeader;

void writeWAVHeader(FILE *file, int sampleRate, int numSamples) {
    WAVHeader header;

    // RIFF chunk
    memcpy(header.chunkID, "RIFF", 4);
    header.chunkSize = 36 + numSamples * 2;
    memcpy(header.format, "WAVE", 4);

    // fmt subchunk
    memcpy(header.subchunk1ID, "fmt ", 4);
    header.subchunk1Size = 16;
    header.audioFormat = 1; // PCM
    header.numChannels = 1; // Mono
    header.sampleRate = sampleRate;
    header.byteRate = sampleRate * 2;
    header.blockAlign = 2;
    header.bitsPerSample = 16;

    // data subchunk
    memcpy(header.subchunk2ID, "data", 4);
    header.subchunk2Size = numSamples * 2;

    fwrite(&header, sizeof(WAVHeader), 1, file);
}

int main() {
    FILE *file = fopen("output.wav", "wb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    int numSamples = SAMPLE_RATE * DURATION;
    writeWAVHeader(file, SAMPLE_RATE, numSamples);

    for (int i = 0; i < numSamples; i++) {
        double t = (double)i / SAMPLE_RATE;
        int16_t sample = (int16_t)(32767 * sin(2 * M_PI * FREQUENCY * t));
        fwrite(&sample, sizeof(int16_t), 1, file);
    }

    fclose(file);
    printf("WAV file generated: output.wav\n");
    return 0;
}