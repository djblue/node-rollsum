# rollsum

[![Build Status](https://travis-ci.org/djblue/node-rollsum.svg?branch=master)](https://travis-ci.org/djblue/node-rollsum)

A rollsum library for calculating content-based splits of buffers.

The cpp code that calculates the splits is from [Camlistore rollsum](https://github.com/camlistore/camlistore/blob/master/pkg/rollsum/rollsum.go).

## install

    $ npm install --save rollsum

## usage

```
var rollsum = require('rollsum');

var splits = 0;
var rs = rollsum();

process.stdin.on('data', function (data) {
  // s will contain the indexes of the split
  // locations in the buffer
  var s = rs.roll(data);
  splits += s.length;
});

process.stdin.on('end', function () {
  console.log('splits = ' + splits);
});
```

__NOTE__: the maximum buffer size you can pass to `roll` is currently
`65536`; an error will be thrown if the buffer exceeds that size.

## cli

Calculates the number of splits found in `file.txt`.

    $ npm install -g rollsum
    $ cat file.txt | rollsum
    splits = <n>

## test

    $ npm test


for basic performance benchmarks, try `pv`:

    $ pv <large-file> | rollsum

## licence

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
