#define _USE_MATH_DEFINES
#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<math.h>
#include"wavheader.h"


double TriangleWave(int sampleIndex, double period) {
	return sin(2 * M_PI * sampleIndex / period);
}
double SquareWave(int sampleIndex, double period, double dutyRatio) {
	return 1 - 2 * ceil((double)sampleIndex / period - floor((double)sampleIndex / period) - dutyRatio);
}
int64_t Quantization(double sample, int bitDepth) { 
	return (int64_t)((sample > 0) ? ((pow(2, bitDepth - 1) - 1) * sample) : (pow(2, bitDepth - 1) * sample));
}

void WriteMonoWAV(int64_t* wave, int waveSize, uint32_t samplingRate, uint16_t bitDepth, char* fileName) {
	/* wave为PCM字节数组，waveSize为采样数，sanplingRate为采样率 */
	int numBaseByte = (int)ceil((double)bitDepth / 8);
	uint16_t numChannel = 1;
	uint32_t byteRate = numChannel * samplingRate * bitDepth / 8;
	uint16_t blockAlign = numChannel * bitDepth / 8;
	uint32_t dataSize = waveSize * numBaseByte;
	uint32_t Size = 4 + 4 + 16 + 4 + dataSize;
	struct WAV_HEADER header = {
		.ID = "RIFF",
		.Size = Size,
		.FourCC = "WAVE",
		.fmtID = "fmt ",
		.fmtSize = 16,
		.encodeMode = 1,
		.numChannel = numChannel,
		.samplingRate = samplingRate,
		.byteRate = byteRate,
		.blockAlign = blockAlign,
		.bitDepth = bitDepth,
		.dataID = "data",
		.dataSize = dataSize
	};
	//对8位及以下使用无符号数
	if (bitDepth <= 8) {
		for (int i = 0; i < waveSize; i++) {
			wave[i] += (int64_t)pow(2, bitDepth - 1);
		}
	}
	//非8整数倍bit左对齐
	if (bitDepth % 8) {
		for (int i = 0; i < waveSize; i++) {
			wave[i] = wave[i] << (numBaseByte * 8 - bitDepth);
		}
	}
	
	int8_t* byteWave = (int8_t*)malloc(numBaseByte * waveSize * sizeof(int8_t));
	if (byteWave) {
		for (int i = 0; i < waveSize; i++) { //逐字节按小端方式写入，此处先转换为小端字节序的byteWave数组
			for (int j = 0; j < numBaseByte; j++) {
				byteWave[numBaseByte * i + j] = (int8_t)((wave[i] >> (8 * j)) & 0xFF);
			}
		}
		FILE* wav = fopen(fileName, "wb");
		fwrite(&header, sizeof header, 1, wav);
		fwrite(byteWave, 1, numBaseByte * waveSize, wav);
		fclose(wav);
	}
}


int main(void) {
	int channel = 1;
	int time = 5;
	int sample = 44100;
	int bit = 4;
	int pitch = 440;
	double duty = 0.5;
	double T = (double)sample / (double)pitch;

	int64_t* wave = (int64_t*)malloc(time * sample * sizeof(int64_t));
	if (wave) {
		for (int i = 0; i < (time * sample); i++) {
			wave[i] = Quantization(SquareWave(i,T,duty), bit);
		}
	}
	WriteMonoWAV(wave, time * sample, sample, bit, "squ4.wav");

	return 0;
}
