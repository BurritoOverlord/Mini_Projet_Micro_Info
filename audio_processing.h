#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H


#define FFT_SIZE 	1024
#define MIC_RIGHT 	0
#define MIC_LEFT 	1
#define MIC_BACK 	2
#define MIC_FRONT 	3

void processAudioData(int16_t *data, uint16_t num_samples);

#endif /* AUDIO_PROCESSING_H */
