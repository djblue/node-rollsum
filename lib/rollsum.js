var windowSize = 64;
var charOffset = 31;

var blobBits = 13;
var blobSize = 1 << blobBits;

function rollsum () {
  if (!(this instanceof rollsum)) {
    return new rollsum();
  }

  this.s1 = windowSize * charOffset;
  this.s2 = windowSize * (windowSize - 1) * charOffset;
  this.window = new Buffer(windowSize);
  for (var i = 0; i < windowSize; i++) {
    this.window[i] = 0;
  }

  this.wofs = 0;
}

rollsum.prototype.add = function (drop, add) {
	this.s1 += add - drop;
  this.s2 += this.s1 - (windowSize * (drop + charOffset));
};

rollsum.prototype.roll = function (ch) {
	this.add(this.window[this.wofs], ch);
	this.window[this.wofs] = ch;
	this.wofs = (this.wofs + 1) % windowSize;
};

// onSplit returns whether at least 13 consecutive trailing
// bits of the current checksum are set the same way.
rollsum.prototype.onSplit = function () {
	return (this.s2 & (blobSize - 1)) === ((~0) & (blobSize - 1));
};

// onSplitWithBits returns whether at least n consecutive
// trailing bits of the current checksum are set the same
// way.
rollsum.prototype.onSplitWithBits = function (n) {
	var mask = (1 << n) - 1;
	return (this.s2 & mask) === ((~0) & mask);
};

rollsum.prototype.bits = function () {
	var bits = blobBits;
	var rsum = this.digest();
	rsum >>= blobBits;
	for (; (rsum >> 1) & 1 !== 0; bits++) {
		rsum >>= 1;
	}
	return bits;
};

rollsum.prototype.digest = function () {
	return (this.s1 << 16) | (this.s2 & 0xffff);
};

module.exports = rollsum;
