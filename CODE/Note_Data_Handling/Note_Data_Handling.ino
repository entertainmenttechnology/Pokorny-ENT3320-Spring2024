#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <mozzi_midi.h>
#include <ADSR.h>
#include <tables/sin2048_int8.h> // sine table for oscillator

#define CONTROL_RATE 128
Oscil <2048, AUDIO_RATE> aSin(SIN2048_DATA);

void setup(){

  aSin.setFreq(440);
  startMozzi(CONTROL_RATE);
}

void updateControl(){

}

int updateAudio(){
  return aSin.next();

}

void loop(){
  audioHook();
}

 // static const float __attribute__(()) fmidiToFreq[128] =
 // {
 // ucmtof(0), ucmtof(1), ucmtof(2),ucmtof(3), ucmtof(4), ucmtof(5), ucmtof(6), ucmtof(7),
 // ucmtof(8),ucmtof(9), ucmtof(10), ucmtof(11), ucmtof(12), ucmtof(13), ucmtof(14), ucmtof(15),
 // ucmtof(16), ucmtof(17), ucmtof(18), ucmtof(19), ucmtof(20), ucmtof(21), ucmtof(22), ucmtof(23),
 // ucmtof(24), ucmtof(25), ucmtof(26), ucmtof(27), ucmtof(28), ucmtof(29), ucmtof(30), ucmtof(31),
 // ucmtof(32), ucmtof(33), ucmtof(34), ucmtof(35), ucmtof(36), ucmtof(37), ucmtof(38), ucmtof(39),
 // ucmtof(40), ucmtof(41), ucmtof(42), ucmtof(43), ucmtof(44), ucmtof(45), ucmtof(46), ucmtof(47),
 // ucmtof(48), ucmtof(49), ucmtof(50), ucmtof(51), ucmtof(52), ucmtof(53), ucmtof(54), ucmtof(55),
 // ucmtof(56), ucmtof(57), ucmtof(58), ucmtof(59), ucmtof(60), ucmtof(61), ucmtof(62), ucmtof(63),
 // ucmtof(64), ucmtof(65), ucmtof(66), ucmtof(67), ucmtof(68), ucmtof(69), ucmtof(70), ucmtof(71),
 // ucmtof(72), ucmtof(73), ucmtof(74), ucmtof(75), ucmtof(76), ucmtof(77), ucmtof(78), ucmtof(79),
 // ucmtof(80), ucmtof(81), ucmtof(82), ucmtof(83), ucmtof(84), ucmtof(85), ucmtof(86), ucmtof(87),
 // ucmtof(88), ucmtof(89), ucmtof(90), ucmtof(91), ucmtof(92), ucmtof(93), ucmtof(94), ucmtof(95),
 // ucmtof(96),ucmtof(97), ucmtof(98), ucmtof(99), ucmtof(100), ucmtof(101), ucmtof(102), ucmtof(103),
 // ucmtof(104),ucmtof(105), ucmtof(106), ucmtof(107), ucmtof(108), ucmtof(109), ucmtof(110), ucmtof(111),
 // ucmtof(112),ucmtof(113), ucmtof(114), ucmtof(115), ucmtof(116), ucmtof(117), ucmtof(118), ucmtof(119),
 // ucmtof(120), ucmtof(121), ucmtof(122), ucmtof(123), ucmtof(124), ucmtof(125), ucmtof(126), ucmtof(127)
 // };
