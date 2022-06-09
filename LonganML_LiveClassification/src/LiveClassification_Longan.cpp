#include <Arduino.h>
#include <SipeedLonganNano-_AQM_inferencing.h>
#define FREQUENCY_HZ        EI_CLASSIFIER_FREQUENCY
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

static unsigned long last_interval_ms = 0;
// to classify 1 frame of data you need EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE values
float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
//float features[40];
// keep track of where we are in the feature array
size_t feature_ix = 0;


int MiCs = PA6; //GND= 0, 5V=4095
int MQ5 = PA7; //PA4: GND= 110, 5V=4095
int MQ7 = PB0; //GND= 0, 5V=4095
int MQ3 = PB1; //PA6: GND= 110, 5V=4095 
float valMiCs = 0; 
float valMQ5 = 0;
float valMQ7 = 0; 
float valMQ3 = 0; 


void setup() {
    Serial.begin(115200);
    Serial.println("Started");
}

void loop() {
    //float x, y, z;

    if (millis() > last_interval_ms + INTERVAL_MS) {
        last_interval_ms = millis();
        // read sensor data in exactly the same way as in the Data Forwarder example
        //IMU.readAcceleration(x, y, z);
        valMiCs = analogRead(MiCs);  
        valMQ5 = analogRead(MQ5);
        valMQ7 = analogRead(MQ7); 
        valMQ3 = analogRead(MQ3); 
        Serial.print(valMiCs);
        Serial.print(",");
        Serial.print(valMQ5);
        Serial.print(","); 
        Serial.print(valMQ7);
        Serial.print(",");  
        Serial.println(valMQ3);   
        
        // fill the features buffer
        features[feature_ix++] = MiCs;
        features[feature_ix++] = MQ5;
        features[feature_ix++] = MQ7;
        features[feature_ix++] = MQ3;

        // features buffer full? then classify!
        if (feature_ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
            ei_impulse_result_t result = { 0 };
    
            // create signal from features frame
            signal_t signal;
            numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);

            // run classifier
            EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
            ei_printf("run_classifier returned: %d\n", res);
            if (res != 0) return;

            // print predictions
            ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
                result.timing.dsp, result.timing.classification, result.timing.anomaly);

            // print the predictions
            for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
                ei_printf("%s:\t%d\n", result.classification[ix].label, static_cast<int>(result.classification[ix].value*100));
            }
        #if EI_CLASSIFIER_HAS_ANOMALY == 1
            ei_printf("anomaly:\t%.3f\n", result.anomaly);
        #endif

            // reset features frame
            feature_ix = 0;
        
        }
        
    }
    
}

void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);

    if (r > 0) {
        Serial.write(print_buf);
    }
}




















