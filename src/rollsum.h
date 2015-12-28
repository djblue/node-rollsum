#ifndef __ROLLSUM_H
#define __ROLLSUM_H

#include <stdint.h>

#include <nan.h>

#define ROLL_BLOBBITS (13)
#define ROLL_BLOBSIZE (1<<ROLL_BLOBBITS)
#define ROLL_WINDOWBITS (6)
#define ROLL_WINDOWSIZE (1<<ROLL_WINDOWBITS)

// According to librsync/rollsum.h:
// "We should make this something other than zero to improve the
// checksum algorithm: tridge suggests a prime number."
// apenwarr: I unscientifically tried 0 and 7919, and they both ended up
// slightly worse than the librsync value of 31 for my arbitrary test data.
#define ROLLSUM_CHAR_OFFSET 31

class Rollsum : public Nan::ObjectWrap {

  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    uint32_t s1, s2;
    uint8_t window[ROLL_WINDOWSIZE];
    uint32_t wofs;
    
    explicit Rollsum();
    virtual ~Rollsum();

    void roll(char ch);
    void add(uint8_t drop, uint8_t add);
    uint32_t split();

    static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Roll(const Nan::FunctionCallbackInfo<v8::Value>& info);

    static Nan::Persistent<v8::Function> constructor;

};

#endif /* __ROLLSUM_H */
