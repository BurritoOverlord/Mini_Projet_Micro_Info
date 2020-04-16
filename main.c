#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <chprintf.h>
#include <motors.h>
#include <sensors/proximity.h>
#include <audio/microphone.h>

#include <audio_processing.h>
#include <fft.h>
#include <communications.h>
#include <arm_math.h>

//référence aux BUS
messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

//uncomment to use proximity sensor
//#define USE_PROX_SENSOR

//uncomment to send the FFTs results from the real microphones
#define SEND_FROM_MIC
//uncomment to use double buffering to send the FFT to the computer
#define DOUBLE_BUFFERING

void SendUint8ToComputer(uint8_t* data, uint16_t size)
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

static void timer12_start(void){
    //General Purpose Timer configuration
    //timer 12 is a 16 bit timer so we can measure time
    //to about 65ms with a 1Mhz counter
    static const GPTConfig gpt12cfg = {
        1000000,        /* 1MHz timer clock in order to measure uS.*/
        NULL,           /* Timer callback.*/
        0,
        0
    };

    gptStart(&GPTD12, &gpt12cfg);
    //let the timer count to max value
    gptStartContinuous(&GPTD12, 0xFFFF);
}

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();
    //starts the USB communication
    usb_start();
    //starts timer 12
    timer12_start();
    serial_start();
    //inits the motors
    motors_init();

    messagebus_init(&bus, &bus_lock, &bus_condvar);
    //chprintf((BaseSequentialStream *)&SD3, "hello");
#ifdef USE_PROX_SENSOR //initialise senseur de proximité
    proximity_start();
    calibrate_ir();
#endif /* USE_PROX_SENSOR */

    //send_tab is used to save the state of the buffer to send (double buffering)
    //to avoid modifications of the buffer while sending it
    static float send_tab[FFT_SIZE];

#ifdef SEND_FROM_MIC
    //starts the microphones processing thread.
    //it calls the callback given in parameter when samples are ready
    mic_start(&processAudioData);
#endif  /* SEND_FROM_MIC */

    /* Infinite loop. */
    while (1) {

    	/****PROX_SENSOR****/
#ifdef USE_PROX_SENSOR
    	for(int i = 0; i < 8; i++)
    	{
    		int16_t prox_dist = get_calibrated_prox(i);
			if(prox_dist > 600)
				chprintf((BaseSequentialStream *)&SD3, "STOP by S%d\n", i);
    	}
    	/*
    	uint16_t prox_dist = get_calibrated_prox(0);
		chprintf((BaseSequentialStream *)&SD3, "S0 = %d\n", prox_dist);
		*/
#endif /* USE_PROX_SENSOR */


    	/****MICRO****/
#ifdef SEND_FROM_MIC
        //waits until a result must be sent to the computer
        wait_send_to_computer();

#ifdef DOUBLE_BUFFERING
        //we copy the buffer to avoid conflicts
        arm_copy_f32(get_audio_buffer_ptr(LEFT_OUTPUT), send_tab, FFT_SIZE);
        SendFloatToComputer((BaseSequentialStream *) &SD3, send_tab, FFT_SIZE);
#else
        SendFloatToComputer((BaseSequentialStream *) &SD3, get_audio_buffer_ptr(LEFT_OUTPUT), FFT_SIZE);
#endif  /* DOUBLE_BUFFERING */

#else

        float* bufferCmplxInput = get_audio_buffer_ptr(LEFT_CMPLX_INPUT);
        float* bufferOutput = get_audio_buffer_ptr(LEFT_OUTPUT);

        //attend de recevoir des données du scripte pyton
        uint16_t size = ReceiveInt16FromComputer((BaseSequentialStream *) &SD3, bufferCmplxInput, FFT_SIZE);

        if(size == FFT_SIZE){

        	volatile uint16_t time = 0;

            doFFT_optimized(FFT_SIZE, bufferCmplxInput);

            arm_cmplx_mag_f32(bufferCmplxInput, bufferOutput, FFT_SIZE);

            SendFloatToComputer((BaseSequentialStream *) &SD3, bufferOutput, FFT_SIZE);

        }
#endif  /* SEND_FROM_MIC */


    	//waits 1 second
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
