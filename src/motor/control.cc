#include "Wire.h"

#include "control.h"
#include "log.h"

Link::Link(Dial* dial) : dial(dial) { }

Link::Link(const Hardware& hardware) : dial(new Dial(hardware)) { }

Stem::Stem(const Hardware& hardware) {
  left  = new Dial(hardware);
  right = new Dial(hardware);
  right->set_dial_2();
}

void Control::init() {
  left.instruction.channel = channel;
  right.instruction.channel = channel;

}

Control::Control(int channel, Stem& stem) :
  left(stem.left), right(stem.right), channel(channel) {
  init();
}

Control::Control(int channel, const Hardware& hardware) :
  left(hardware), right(hardware), channel(channel) {
  init();
}

static void Control::update_link(Link& link) {
  const static int channel = link.instruction.channel;
  const static int count = sizeof(Instruction);
  char* buffer = (char*)&link.instruction;
  if (link.dial->update() != link.instruction.reading) {

    Wire.requestFrom(channel, count);

    if (Wire.available() == count) {
      Wire.readBytes(buffer, count);
    }
    link.instruction.reading = link.dial->reading;
    link.instruction.command = Command::perform;
    link.instruction.respond = Command::none;
    Wire.beginTransmission(channel);
    Wire.write(buffer, count);
    Wire.endTransmission();
    print_instruction(link.instruction);
  }
}

Command Control::update() {
  update_link(left);
  update_link(right);
}
