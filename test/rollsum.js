var crypto = require('crypto');
var test = require('tap').test;

var rollsum = require('../');

var size = 32*1024;

var getData = function () {
  return crypto.pseudoRandomBytes(size);
};

var hash = function (data) {
  return crypto.createHash('sha1').update(data).digest('hex');
};

var calcHashes = function (data) {

  var hashes = [];
  var rs = rollsum();

  var i = 0;
  var splits = rs.roll(data);

  splits.forEach(function (j) {
    hashes.push(hash(data.slice(i,j)));
    i = j;
  });

  if (i < data.length - 1) {
    hashes.push(hash(data.slice(i)));
  }

  return hashes;
};

test('roll expects a buffer', function (t) {
  t.plan(1);

  t.throws(function () {
    rollsum().roll(123);
  }, new TypeError('first argument should be a buffer'));

});

test('roll expect certain size buffer', function (t) {
  t.plan(1);

  t.throws(function () {
    rollsum().roll(new Buffer(65537));
  }, new RangeError('buffer exceeds maximum size of 65536'));

});

// splitting data should always have the same result.
test('splits are deterministic', function (t) {

  t.plan(1);
  var data = getData();
  t.deepEqual(calcHashes(data), calcHashes(data));

});

// changing the first bit should only affect the first
// split, all other splits should be the same.
test('changing first byte', function (t) {

  t.plan(2);
  var d1 = getData();
  var d2 = new Buffer(d1);
  d2[0] = d2[0] + 1;

  var h1 = calcHashes(d1);
  var h2 = calcHashes(d2);

  t.notEqual(h1[0], h2[0]);
  t.deepEqual(h1.slice(1), h2.slice(1));

});

// changeing the first and last bit, should only effect
// the first and last split.
test('changing first and last byte', function (t) {

  t.plan(3);
  var d1 = getData();
  var d2 = new Buffer(d1);
  d2[0] = d2[0] + 1;
  d2[d2.length - 1] = d2[d2.length - 1] + 1;

  var h1 = calcHashes(d1);
  var h2 = calcHashes(d2);

  t.notEqual(h1[0], h2[0]);
  t.deepEqual(h1.slice(1, -1), h2.slice(1, -1));
  t.notEqual(h1.slice(-1)[0], h2.slice(-1)[0]);

});

// changeing a random byte should only affect a single hash
// value
test('changing random byte', function (t) {

  t.plan(1);

  var d1 = getData();
  var d2 = new Buffer(d1);
  var j = Math.floor(Math.random() * size) % size;

  d2[j] = d2[j] + 1;

  var h1 = calcHashes(d1);
  var h2 = calcHashes(d2);

  var diff = 0;

  for (var i = 0; i < h1.length; i++) {
    if (h1[i] !== h2[i]) {
      diff++;
    }
  }

  t.equal(diff, 1);

});
