import * as util from 'node:util';
import * as aoc from '../common/index.js';

class Present {
  length;
  width;
  height;

  /**
   * @param {number} length
   * @param {number} width
   * @param {number} height
   */
  constructor(length, width, height) {
    this.length = length;
    this.width = width
    this.height = height;
  }

  /**
   * the total surface area of the present
   */
  get surfaceArea() {
    return (
      (2 * this.length * this.width) +
      (2 * this.width * this.height) +
      (2 * this.height * this.length)
    )
  }

  get cubicVolume() {
    return (this.length * this.width * this.height);
  }

  /**
   * the area of the smallest side of the present
   */
  get smallestSideArea() {
    const [f, s, _] = [this.length, this.width, this.height].sort((a, b) => a - b);

    return (f * s);
  }

  get smallestSidePerimeter() {
    const [f, s, _] = [this.length, this.width, this.height].sort((a, b) => a - b);

    return ((f * 2) + (s * 2));
  }

  [util.inspect.custom]() {
    return `Present(${this.length}, ${this.width}, ${this.height}) | ${this.surfaceArea} | ${this.smallestSideArea}`;
  }
}

/**
 * Parse text input into present objects
 * @param {Present[]} acc
 * @param {string} input
 * @param {number} index
 * @returns {Present[]}
 */
function parse(acc, input, index) {
  if (input === '\n' || input === '') {
    return acc;
  }

  const sides = input.split('x');
  if (sides.length !== 3) {
    throw new SyntaxError(`invalid present string: ${index}| ${input}`);
  }
  const [l, w, h] = sides.map(Number);

  acc.push(new Present(l, w, h));

  return acc;
}

/**
  * Encapsulates all the processing needed for part1 of day1
  * @param {Present[]} input - the input presents
  * @returns {Promise<number>} - the result for part1
  */
async function part1(input) {
  return input.reduce((acc, present) => {
    const presentArea = (present.surfaceArea + present.smallestSideArea);
    // console.log(present, presentArea);
    return acc + presentArea;
  }, 0);
}

/**
  * Encapsulates all the processing needed for part2 of day1
  * @param {Present[]} input - the input presents
  * @returns {Promise<number>} - the result of part2
  */
async function part2(input) {
  return input.reduce((acc, present) => {
    const ribbonLength = present.cubicVolume + present.smallestSidePerimeter;
    return acc + ribbonLength;
  }, 0);
}

async function main() {
  const input = aoc.input().reduce(parse, []);

  const part1Result = await part1(input);
  const part2Result = await part2(input);

  console.log(`Part1: ${part1Result}`)
  console.log(`Part2: ${part2Result}`)
}

main()
  .catch((err) => {
    console.log(`Error: ${err}`)
  })
  .finally(() => console.log('Complete'));
