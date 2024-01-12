#define _USE_MATH_DEFINES
#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<math.h>

struct HEADER
{
	//Chunk "RIFF"
	uint8_t ID[4]; //"RIFF"
	uint32_t Size; /*NOT IMPORTANT*/
	uint8_t FourCC[4]; //"WAVE"

	//SubChunk "fmt"
	uint8_t fmtID[4]; //"fmt "
	uint32_t fmtSize; //16
	//"fmt" Data
	uint16_t encodeMode; //1 for PCM, ...
	uint16_t numChannel; //1 for Mono, 2 for Stereo
	uint32_t samplingRate; //usually 44100 for lossless
	uint32_t byteRate; //numChannel * samplingRate * bitDepth / 8 /*NOT IMPORTANT*/
	uint16_t blockAlign; //numChannel * bitDepth / 8 /*NOT IMPORTANT*/
	uint16_t bitDepth; //usually 16 for lossless

	//SubChunk "data"
	uint8_t dataID[4]; //"data"
	uint32_t dataSize;
	//"data" Data is the kind chosen in "encodeMode"
};

double TriangleWave(int sampleIndex, double period) {
	return sin(2 * M_PI * sampleIndex / period);
}
double SquareWave(int sampleIndex, double period, double dutyRatio) {
	return 1 - 2 * ceil((double)sampleIndex / period - floor((double)sampleIndex / period) - dutyRatio);
}
int64_t Quantization(double sample, int bitDepth) {
	return (int64_t)((sample > 0) ? ((pow(2, bitDepth - 1) - 1) * sample) : (pow(2, bitDepth - 1) * sample));
}

void WriteWAV(int64_t wave, int waveSize, uint16_t numChannel,uint32_t samplingRate, uint16_t bitDepth) {
	uint32_t byteRate = numChannel * samplingRate * bitDepth / 8;
	uint16_t blockAlign = numChannel * bitDepth / 8;
	uint32_t dataSize = waveSize * bitDepth / 8;
	uint32_t Size = 4 + 4 + 16 + 4 + dataSize;
	struct HEADER header = {
		"RIFF",
		Size,
		"WAVE",
		"fmt ",
		16,
		1,
		1,
		samplingRate,
		byteRate,
		blockAlign,
		bitDepth,
		"data",
		dataSize
	};
	int numBaseByte = ceil((double)bitDepth / 8);


}


int main(void) {
	int channel = 1;
	int time = 5;
	int sample = 44100;
	int bit = 24;
	int pitch = 440;
	double duty = 0.5;
	double T = (double)sample / (double)pitch;

	int64_t* wave_o = (int64_t*)malloc(time * sample * sizeof(int64_t));
	//int16_t* wave_s = (int16_t*)malloc(time * sample * sizeof(int16_t));
	if (wave_o) {
		for (int i = 0; i < (time * sample); i++) {
			//wave_o[i] = 1 - 2 * ceil((double)i / T - floor((double)i / T) - duty); //Square wave by lowmst
			//wave_o[i] = sin(2 * M_PI * i / T);
			wave_o[i] = Quantization(SquareWave(i,T,duty), bit);
			//printf("%d\n", (int)SquareWave(i, T, duty));
		}
	}

	for (int i = 0; i < (time * sample); i++) {
		printf("%d\n", wave_o[i]);
	}

//	if (wave_s && wave_o) {
//		for (int i = 0; i < (time * sample); i++) {
//			if (wave_o[i] > 0) {
//				wave_s[i] = (int16_t)((pow(2, 15) - 1) * (wave_o[i]));
//			}
//			else
//			{
//				wave_s[i] = (int16_t)(pow(2, 15) * (wave_o[i]));
//			}
//		}
//	}

	uint32_t byteRate = channel * sample * bit / 8;
	uint16_t blockAlign = channel * bit / 8;
	uint32_t dataSize = time * sample * bit / 8;
	uint32_t Size = 4 + 4 + 16 + 4 + dataSize;
	struct HEADER header = {
		"RIFF",
		Size,
		"WAVE",
		"fmt ",
		16,
		1,
		1,
		sample,
		byteRate,
		blockAlign,
		bit,
		"data",
		dataSize
	};
	FILE* wav = fopen("out24.wav", "wb");
	fwrite(&header, sizeof header, 1, wav);
	if (wave_o) {
		fwrite(wave_o, 3, time * sample, wav);
	}
	fclose(wav);
	return 0;
}