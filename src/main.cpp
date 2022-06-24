#include <mbed.h>
#include "DebounceInterruptIn.h"

EventQueue queue;
DebounceIn button(&queue, USER_BUTTON, 10);
Serial pc(USBTX, USBRX, 115200);
int main(){
    button.rise([](){
        pc.printf("rise\r\n");
    });
    button.fall([](){
        pc.printf("fall\r\n");
    });
    queue.dispatch_forever();
    return 0;
}
