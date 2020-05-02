#include "ch.h"
#include "hal.h"
#include <main.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <motors.h>
#include <audio/microphone.h>
#include <audio_processing.h>
#include <fft.h>
#include <arm_math.h>
#include <moteur_manager.h>

//semaphore
static BSEMAPHORE_DECL(sendToComputer_sem, TRUE);

//2 times FFT_SIZE because these arrays contain complex numbers (real + imaginary)
static float micLeft_cmplx_input[2 * FFT_SIZE];

//Arrays containing the computed magnitude of the complex numbers
static float micLeft_output[FFT_SIZE];

#define MIN_VALUE_THRESHOLD 30000

#define MIN_FREQ 		18 //we don’t analyze before this index to not use resources for nothing
#define FREQ_FORWARD 	19 //296Hz = 19 * 15.625
#define FREQ_LEFT 		22 //343Hz = 22 * 15.625
#define FREQ_RIGHT 		26 //406HZ = 26 * 15.625
#define FREQ_BACKWARD 	29 //453Hz = 29 * 15.625
#define FREQ_STOP 		32 //500Hz = 32 * 15.625
#define FREQ_SPEED		38 //593Hz = 38 * 15.625
#define MAX_FREQ 		39 //we don’t analyze after this index to not use resources for nothing

#define FREQ_SPEED_L 	(FREQ_SPEED-1)
#define FREQ_SPEED_H 	(FREQ_SPEED+1)
#define FREQ_FORWARD_L 	(FREQ_FORWARD-1)
#define FREQ_FORWARD_H 	(FREQ_FORWARD+1)
#define FREQ_LEFT_L 	(FREQ_LEFT-1)
#define FREQ_LEFT_H 	(FREQ_LEFT+1)
#define FREQ_RIGHT_L 	(FREQ_RIGHT-1)
#define FREQ_RIGHT_H 	(FREQ_RIGHT+1)
#define FREQ_BACKWARD_L (FREQ_BACKWARD-1)
#define FREQ_BACKWARD_H (FREQ_BACKWARD+1)
#define FREQ_STOP_L 	(FREQ_STOP-1)
#define FREQ_STOP_H 	(FREQ_STOP+1)

/*
* Simple function used to detect the highest value in a buffer
* and to execute a motor command depending on it
* Stoke la fréquence correspondant dans state_frequence:
* 0 = forward
* 1 = left
* 2 = right
* 3 = backward
* 4 = stop
* 5 = speed change
* 6 = aucune directive
*/
void sound_remote(float* data) {
	uint8_t state_frequence;
	float max_norm = MIN_VALUE_THRESHOLD;
	int16_t max_norm_index = -1;

	//search for the highest peak
	for (uint16_t i = MIN_FREQ; i <= MAX_FREQ; i++) {
		if (data[i] > max_norm) {
			max_norm = data[i];
			max_norm_index = i;
		}
	}

	//go forward et state_frequence = 0
	if (max_norm_index >= FREQ_FORWARD_L && max_norm_index <= FREQ_FORWARD_H) {
		state_frequence = 0;
	}
	//turn left et state_frequence = 1
	else if (max_norm_index >= FREQ_LEFT_L && max_norm_index <= FREQ_LEFT_H) {
		state_frequence = 1;
	}
	//turn right et state_frequence = 2
	else if (max_norm_index >= FREQ_RIGHT_L && max_norm_index <= FREQ_RIGHT_H) {
		state_frequence = 2;
	}
	//go backward et state_frequence = 3
	else if (max_norm_index >= FREQ_BACKWARD_L && max_norm_index <= FREQ_BACKWARD_H) {
		state_frequence = 3;
	}
	//stop et state_frequence = 4
	else if (max_norm_index >= FREQ_STOP_L && max_norm_index <= FREQ_STOP_H) {
		state_frequence = 4;
	}
	//change difficulty et state_frequence = 5;
	else if (max_norm_index >= FREQ_SPEED_L && max_norm_index <= FREQ_SPEED_H) {
		state_frequence = 5;
	}
	//aucune directive
	else {
		state_frequence = 6;
	}

	moteur_manager_deplacement(state_frequence);
}

/*
 * 1024/160 = 6.4 * 10ms = 64ms pour remplir le buffer complètement => toute les 70ms processAudioData output une valeur
 *
 *	Callback called when the demodulation of the four microphones is done.
 *	We get 160 samples per mic every 10ms (16kHz)
 *
 *	params :
 *	int16_t *data			Buffer containing 4 times 160 samples. the samples are sorted by micro
 *							so we have [micRight1, micLeft1, micBack1, micFront1, micRight2, etc...]
 *	uint16_t num_samples	Tells how many data we get in total (should always be 640)
 */
void processAudioData(int16_t *data, uint16_t num_samples) {

	/*
	 *	We get 160 samples per mic every 10ms
	 *	So we fill the samples buffers to reach
	 *	1024 samples, then we compute the FFTs.
	 *
	 */

	static uint16_t nb_samples = 0;
	static uint8_t mustSend = 0;

	//loop to fill the buffers
	for (uint16_t i = 0; i < num_samples; i += 4) {
		//construct an array of complex numbers. Put 0 to the imaginary part
		micLeft_cmplx_input[nb_samples] = (float) data[i + MIC_LEFT];
		nb_samples++;
		micLeft_cmplx_input[nb_samples] = 0;
		nb_samples++;

		//stop when buffer is full
		if (nb_samples >= (2 * FFT_SIZE)) {
			break;
		}
	}

	if (nb_samples >= (2 * FFT_SIZE)) {
		/* FFT proccessing
		 *
		 * This FFT function stores the results in the input buffer given.
		 * This is an "In Place" function.
		 */
		doFFT_optimized(FFT_SIZE, micLeft_cmplx_input);
		/* Magnitude processing
		 *
		 * Computes the magnitude of the complex numbers and
		 * stores them in a buffer of FFT_SIZE because it only contains
		 * real numbers.
		 *
		 */
		arm_cmplx_mag_f32(micLeft_cmplx_input, micLeft_output, FFT_SIZE);

		//sends only one FFT result over 10 for 1 mic to not flood the computer
		//sends to UART3
		if (mustSend > 8) {
			//signals to send the result to the computer
			chBSemSignal(&sendToComputer_sem);
			mustSend = 0;
		}
		nb_samples = 0;
		mustSend++;
		sound_remote(micLeft_output);
	}
}
