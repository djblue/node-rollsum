# rollsum

[![Build Status](https://travis-ci.org/djblue/node-rollsum.svg?branch=master)](https://travis-ci.org/djblue/node-rollsum)

A direct port of [Camlistore rollsum](https://github.com/camlistore/camlistore/blob/master/pkg/rollsum/rollsum.go) to javascript.

## install

    $ npm install rollsum

## usage

```javascript
var data = getSomeData();
var sum = require('rollsum')();

for (var i = 0; i < data.length; i++) {
  sum.roll(data[i]);
  if (sum.onSplit()) {
    console.log('found split!');
  }
}
```

## test

with npm, do:

    npm test

for basic performance benchmarks, try `pv`;

    pv <large-file> | rollsum
