#include <mbed.h>

#include "C620.hpp"

// Const variable

// Function prototype

// IO
// CAN can{PA_11, PA_12, (int)1e6};
CAN can{PB_12, PB_13, (int)1e6};
/// @note INPUT PULLUP
DigitalIn button{BUTTON1};

// Struct definition

// Global variable
C620Array c620_array{};

/// @brief The application entry point.
int main() {
  // put your setup code here, to run once:
  printf("\nsetup\n");
  while(1) {
    // put your main code here, to run repeatedly:
    auto now = HighResClock::now();
    static auto pre = now;
    if(now - pre > 20ms) {
      int16_t out = 0;
      if(!button) out = 6000;
      printf("% 7d ", out);

      for(auto& e: c620_array) {
        e.raw_current = out;
      }

      auto msgs = c620_array.to_msg();
      if(!can.write(msgs[0]) || !can.write(msgs[1])) {
        printf("failed to write c620 msg ");
      }

      printf("\n");
      pre = now;
    }
  }
}

// Function definition
