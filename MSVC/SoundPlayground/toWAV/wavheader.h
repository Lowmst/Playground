#pragma once
#include<stdint.h>
struct WAV_HEADER
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