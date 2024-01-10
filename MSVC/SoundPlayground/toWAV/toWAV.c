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

int main(void) {
	int channel = 1;
	int time = 5;
	int sample = 44100;
	int bit = 16;
	int pitch = 2000;
	double duty = 0.5;
	double T = (double)sample / (double)pitch;

	double* wave_o = (double*)malloc(time * sample * sizeof(double));
	int16_t* wave_s = (int16_t*)malloc(time * sample * sizeof(int16_t));
	if (wave_o) {
		for (int i = 0; i < (time * sample); i++) {
			wave_o[i] = 1 - 2 * ceil((double)i / T - floor((double)i / T) - duty); //Square wave by lowmst
			//wave_o[i] = sin(2 * M_PI * i / T);
		}
	}
	if (wave_s && wave_o) {
		for (int i = 0; i < (time * sample); i++) {
			if (wave_o[i] > 0) {
				wave_s[i] = (int16_t)((pow(2, 15) - 1) * (wave_o[i]));
			}
			else
			{
				wave_s[i] = (int16_t)(pow(2, 15) * (wave_o[i]));
			}
		}
	}

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
	FILE* wav = fopen("out.wav", "wb");
	fwrite(&header, sizeof header, 1, wav);
	if (wave_s) {
		fwrite(wave_s, bit / 8, time * sample, wav);
	}
	fclose(wav);
	return 0;
}