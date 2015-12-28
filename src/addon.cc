#include <nan.h>

#include "rollsum.h"

void InitAll(v8::Local<v8::Object> exports) {
  Rollsum::Init(exports);
}

NODE_MODULE(addon, InitAll)
