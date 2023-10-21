#ifndef RCT_C620_HPP
#define RCT_C620_HPP

#include <mbed.h>

#include <array>

struct C620 {
  static constexpr int max = 16384;
  void set_current(const float current) {
    raw_current = max * current;
  }
  void parse_packet(const uint8_t data[8]) {
    rx.angle = uint16_t(data[0] << 8 | data[1]);
    rx.rpm = int16_t(data[2] << 8 | data[3]);
    rx.current = int16_t(data[4] << 8 | data[5]);
    rx.temp = data[6];
  }

  int16_t raw_current;
  struct {
    uint16_t angle;
    int16_t rpm;
    int16_t current;
    uint8_t temp;
  } rx;
};

struct C620Array {
  std::array<CANMessage, 2> to_msg() const {
    uint8_t buf[2][8];
    for(int i = 0; i < 2; ++i) {
      for(int j = 0; j < 4; ++j) {
        buf[i][2 * j] = arr_[4 * i + j].raw_current >> 8;
        buf[i][2 * j + 1] = arr_[4 * i + j].raw_current & 0xFF;
      }
    }
    return {CANMessage{0x200, buf[0]}, CANMessage{0x1FF, buf[1]}};
  }
  bool parse_packet(const CANMessage& msg) {
    if(msg.format == CANStandard && msg.type == CANData && msg.len == 8 && 0x201 <= msg.id && msg.id <= 0x208) {
      arr_[msg.id - 0x201u].parse_packet(msg.data);
      return true;
    }
    return false;
  }

  auto begin() {
    return arr_.begin();
  }
  auto end() {
    return arr_.end();
  }
  C620& operator[](int index) & {
    return arr_[index];
  }

 private:
  std::array<C620, 8> arr_;
};

#endif  /// RCT_C620_H_
