#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 2;

int IRstate = LOW;                    // IR 接收器輸出腳位狀態
int IRstate_last = LOW;               // IR 接收器輸出腳位狀態(上一次)
unsigned long int time_last = 0;      // 上一次 IRstate 變化的時間
boolean isIdle = true;                // 是否在等待 IR 訊號模式Idle）
const long int durationMax = 10000;   // 一段時間沒變化就進入 Idle，單位 us
const long int durationMin = 300;     // 電壓狀態不變的最小持續時間，單位 us

IRrecv irrecv(kRecvPin);


void setup() {
    Serial.begin(115200);
    irrecv.enableIRIn();  // Start the receiver
    while (!Serial) {  // Wait for the serial connection to be establised.
        delay(50);
    }
    Serial.println();
    Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
    Serial.println(kRecvPin);
}

void printCode(decode_results codes) {
    for (int i = 63; i >= 0; i--) {
        if (codes.value & ((uint64_t)1 << i)) {
            Serial.print("1");
        } else {
            Serial.print(".");
        }
    }
    Serial.println();
}

inline bool time_approx_eq(int32_t a, int32_t b)
{
    int32_t diff = a - b;
    return (abs(diff) < 300);
}

void IR_rec_Check(){
    IRstate = digitalRead(kRecvPin);        // 讀取腳位狀態
    if (IRstate != IRstate_last) {          //
        long int timeNow = micros();        // 記錄目前時間
        long int dT = timeNow - time_last;  // 與上一次腳位變化的時間差

        if( (dT >= durationMax) && !isIdle ) {     // 時間間隔大於設定的時間，且原本的狀態為接收中狀態
            isIdle = true; //進入等待狀態
            Serial.println();
        } else if ((dT < durationMax) && (dT > durationMin)) {
            isIdle = false; //進入接收中狀態
            /*
            if (IRstate == HIGH) {
                Serial.print(dT);
                Serial.print(" ");
            } else {
                Serial.print( 0-dT );
                Serial.println();
            }
            */
            
            if (IRstate == LOW) {
                if (time_approx_eq(dT, 2250)) {
                    Serial.print(">");
                } else if (time_approx_eq(dT, 1675)) {
                    Serial.print("1");
                } else if (time_approx_eq(dT, 450)) {
                    Serial.print(".");
                } else if (time_approx_eq(dT, 8340)) {
                    Serial.print("E");
                } else {
                    Serial.print("?");
                }
            } else {
                if (time_approx_eq(dT, 3750)) {
                    Serial.print("^");
                } else if (time_approx_eq(dT, 750)) {
                    ;
                } else {
                    Serial.print("+");
                }
            }
            
        }
        // 記錄此次時間
        time_last = timeNow;
    }
    // 記錄此次狀態
    IRstate_last = IRstate;
}   //end IR_rec_Check

void loop() {
  /*
    decode_results results;
    if (irrecv.decode(&results)) {
        // print() & println() can't handle printing long longs. (uint64_t)
        //serialPrintUint64(results.value, HEX);
        //Serial.println("");
        printCode(results);
        irrecv.resume();  // Receive the next value
    }
    delay(100);
    */
    IR_rec_Check();
    delayMicroseconds(5);
}
