#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H


#define FFT_SIZE 	1024
#define MIC_LEFT 	1


void processAudioData(int16_t *data, uint16_t num_samples);

#endif /* AUDIO_PROCESSING_H */
