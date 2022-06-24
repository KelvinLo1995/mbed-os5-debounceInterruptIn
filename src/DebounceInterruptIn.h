#pragma once
#include <mbed.h>

struct DebounceIn {
    InterruptIn _in;
    EventQueue* queue;

    // counters and flags
    int _samples = 0;
    int _output = 0;
    int _output_last = 0;
    int _rising_flag = 0;
    int _falling_flag = 0;
    int _state_counter = 0;

    Callback<void(void)> onRise = NULL;
    Callback<void(void)> onFall = NULL;

    void rise(Callback<void(void)> f){
        onRise = f;
    }

    void fall(Callback<void(void)> f){
        onFall = f;
    }

    void _sample() {
        _samples = _samples >> 1;
        if (_in) _samples |= 0x80;  
        if (_samples == 0x00) _output = 0;
        else if (_samples == 0xFF) _output = 1;
    
        if ((_output == 1) && (_output_last == 0)) {
            if (onFall) onFall();
            _state_counter = 0;
        } else if ((_output == 0) && (_output_last == 1)) {
            if (onRise) onRise();
            _state_counter = 0;
        } else {
            _state_counter++;
        }

        _output_last = _output;
    }
    
    // return number of rising edges
    int rising(void) {
        int return_value = _rising_flag; 
        _rising_flag = 0;
        return(return_value);
    }
    
    // return number of falling edges
    int falling(void) {
        int return_value = _falling_flag; 
        _falling_flag = 0;
        return(return_value);
    }
    
    // return number of ticsk we've bene steady for
    int steady(void) {
    return(_state_counter);
    }
    
    // return the debounced status
    int read(void) {
        return(_output);
    }
    
    // shorthand for read()
    operator int() {
        return read();
    }
    
    inline DebounceIn(EventQueue* queue, PinName pin, uint16_t scanIntervalMs):
    queue(queue), _in(pin) {
        queue->call_every(scanIntervalMs, [this](){
            _sample();
        });
    }

};
