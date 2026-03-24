#pragma once

struct Change {
  int channel = -1;
  char* buffer = 0;
  int to_read = 0;
  int to_write = 0;
};

struct Rhythm {
  // present
  long now = -1;

  // past
  long last = -1;
  int missed = 0;

  // id
  int group = -1;
  
  // policy
  int beats = 8;       // eight beats a measure
  int value = 10;      // hang in with the beat
  int calls = 3;       // give them a few tries
  int layoff = 1000;   // hold up just a second
  
};

static int entrance(const Rhythm& rhythm) {
  return rhythm.group % rhythm.beats;  // sign-preserving modulo
}

static int stick(const Rhythm& rhythm) {
  return rhythm.now % rhythm.beats;
}

static bool on_beat(const Rhythm& rhythm) {
  int beat = stick(rhythm);
  if (beat < 0) { // I can't tell you to go if you don't give me the time.
    return false;
  }
  return entrance(rhythm) == beat;
}

static bool fresh(const Rhythm& rhythm) {
  return rhythm.last < 0;
}

static bool steady(const Rhythm& rhythm) {
  return rhythm.missed <= rhythm.calls;
}

static bool on_cue(const Rhythm& rhythm) {
  if (fresh(rhythm)) {
    return true;
  } else if (steady(rhythm)) {
    return rhythm.now >= rhythm.last + rhythm.value;
  } 
  return rhythm.now >= rhythm.last + rhythm.layoff;
}

static bool go(const Rhythm& rhythm) {
  return on_cue(rhythm) and on_beat(rhythm);
}

void keep(Rhythm& rhythm);

using Move = int (*)();
using Call = int (*)(Change& change);

static int follow(Rhythm& rhythm, Call call, Change& change)  {
  keep(rhythm);
  if (go(rhythm)) {
    rhythm.last = rhythm.now;
    
    int response = call(change);
    if (response > 0) {
      rhythm.missed = 0;
    } else {
      rhythm.missed++;
    }
    
    return response;
  }
  return 0;
}

static int follow(Rhythm& rhythm, Move move) {
  keep(rhythm);
  if (go(rhythm)) {
    rhythm.last = rhythm.now;

    int result = move();
    if (result) {
      rhythm.missed = 0;
    } else {
      rhythm.missed++;
    }

    return result;
  }

  return 0;
}

