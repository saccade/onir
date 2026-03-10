struct Timing {
  // present
  long now = -1;

  // past
  long seen = -1;
  long gone = -1;

  // id
  int channel = -1;
  
  // policy
  int update = 10;    // stay on the beat
  int checkup = 1000; // hang on a second
  int beats = 8;
  
};

static int modulo(int modulus, int value) {
  if (modulus <= 0) { return -1 ;}
  int remains = value % modulus;
  if (remains < 0) { remains += modulus; }
  return remains;
}

static int stick(const Timing& timing) {
  if (timing.now < 0) return -1;
  return modulo(timing.beats, timing.now);
}

static int entrance(const Timing& timing) {
  return modulo(timing.beats, timing.channel);
}

static bool on_beat(const Timing& timing) {
  int beat = stick(timing);
  if (beat < 0) {
    // if you don't tell me what time it is, I won't tell you to go.
    return false;
  }
  return entrance(timing) == beat;
}

static bool unchecked(const Timing& timing) {
  return timing.seen < 0 and timing.gone < 0;
}

static bool up(const Timing& timing) {
  return timing.seen > 0 and timing.seen >= timing.gone;
}

static bool down(const Timing& timing) {
  return timing.gone > 0 and timing.gone > timing.seen;
}

static bool ready(const Timing& timing) {
  if (unchecked(timing)) {
    return true;
  } else if (up(timing)) {
    return timing.now >= timing.seen + timing.update;
  } else if (down(timing)) {
    return timing.now >= timing.gone + timing.checkup;
  } else {
    int what = 1 / 0;
    return false;
  }
}

static bool go(const Timing& timing) {
  return ready(timing) and on_beat(timing);
}
