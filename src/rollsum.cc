#include "rollsum.h"

Nan::Persistent<v8::Function> Rollsum::constructor;

#include <memory.h>

Rollsum::Rollsum() {
  s1 = ROLL_WINDOWSIZE * ROLLSUM_CHAR_OFFSET;
  s2 = ROLL_WINDOWSIZE * (ROLL_WINDOWSIZE-1) * ROLLSUM_CHAR_OFFSET;
  wofs = 0;
  memset(window, 0, ROLL_WINDOWSIZE);
}

Rollsum::~Rollsum() {
}

void Rollsum::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("rollsum").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "roll", Roll);

  constructor.Reset(tpl->GetFunction());
  Nan::Set(exports,
    Nan::New("rollsum").ToLocalChecked(),
    Nan::GetFunction(tpl).ToLocalChecked());
}

void Rollsum::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    Rollsum* rs = new Rollsum();
    rs->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void Rollsum::Roll(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (!info[0]->IsObject()) {
    return Nan::ThrowTypeError("first argument should be a buffer");
  }

  Rollsum* rs = ObjectWrap::Unwrap<Rollsum>(info.Holder());

  v8::Local<v8::Object> bufferObj = info[0]->ToObject();
  char*         bufferData   = node::Buffer::Data(bufferObj);
  size_t        bufferLength = node::Buffer::Length(bufferObj);

  if (bufferLength > 65536) {
    return Nan::ThrowRangeError("buffer exceeds maximum size of 65536");
  }
;
  size_t splits[65536];
  size_t len = 0;

  for (size_t i = 0; i < bufferLength; i++) {
    rs->roll(bufferData[i]);
    if (rs->split()) {
      splits[len++] = i;
    }
  }

  v8::Local<v8::Array> array = Nan::New<v8::Array>(len);

  for (size_t i = 0; i < len; i++) {
    Nan::Set(array,
      Nan::New<v8::Number>(i),
      Nan::New<v8::Number>(splits[i]));
  }

  info.GetReturnValue().Set(array);
}


void Rollsum::roll(char ch) {
  uint32_t next = wofs + 1;
  add(window[wofs], ch);
  window[wofs] = ch;
  wofs = (next == ROLL_WINDOWSIZE)? 0 : next;
}

// These formulas are based on rollsum.h in the librsync project.
void Rollsum::add(uint8_t drop, uint8_t add) {
  s1 += add - drop;
  s2 += s1 - (ROLL_WINDOWSIZE * (drop + ROLLSUM_CHAR_OFFSET));
}


uint32_t Rollsum::split() {
  return ((s2 & (ROLL_BLOBSIZE - 1)) == ((~0) & (ROLL_BLOBSIZE - 1)));
}

