# Radio duty cycling

Contiki có 3 cơ chế duty cycle: `ContikiMAC`, `X-MAX` và `LPP` là giao thức dựa trên các nguyên tắc của low-power listening (hiểu ngầm) nhưng tiết kiệm công suất hơn.

`X-MAX` dựa trên giao thức `X-MAX` gốc nhưng được cải tiến để giảm mức năng lượng tiêu thụ và duy trì mạng tốt.

`LPP` của Contiki dựa trên giao thức LowPower Probing nhưng được cải tiến mức tiêu tốn công suất như cung cấp các cơ chế broadcast data.

### Table of Contents
- [Radio duty cycling](#radio-duty-cycling)
    - [Table of Contents](#table-of-contents)
  - [Background on Radio Duty Cycling Protocols](#background-on-radio-duty-cycling-protocols)
    - [Low-Power Listening](#low-power-listening)
    - [Low-Power Probing](#low-power-probing)
  - [The Contiki Radio Duty Cycling Protocols](#the-contiki-radio-duty-cycling-protocols)
    - [The ContikiMAC Radio Duty Cycling Mechanism](#the-contikimac-radio-duty-cycling-mechanism)
    - [The Contiki X-MAC](#the-contiki-x-mac)
      - [Broadcast](#broadcast)
      - [Phase Awareness](#phase-awareness)
      - [Reliable Packet Transmissions](#reliable-packet-transmissions)
      - [812.15.4 Compatibility](#812154-compatibility)
  - [Implementation](#implementation)


## Background on Radio Duty Cycling Protocols

### Low-Power Listening

### Low-Power Probing

## The Contiki Radio Duty Cycling Protocols

###  The ContikiMAC Radio Duty Cycling Mechanism

### The Contiki X-MAC

#### Broadcast

#### Phase Awareness

#### Reliable Packet Transmissions

#### 812.15.4 Compatibility

## Implementation