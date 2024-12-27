import * as aoc from '../common/index.js';

/**
  * Encapsulates all the processing needed for part1 of day1
  * @param {string[]} input - the input source
  * @returns {Promise<number>} - the result for part1
  */
async function part1(input) {
  let floor = 0;

  for (const line of input) {
    for (const ch of line) {
      switch (ch) {
        case '(':
          ++floor;
          break;
        case ')':
          --floor;
          break;
        case '\n':
          continue;
        default:
          throw SyntaxError(`${ch} is invalid input`);
      }
    }
  }

  return floor;
}

/**
  * Encapsulates all the processing needed for part2 of day1
  * @param {string[]} input - the input source
  * @returns {Promise<number>} - the result of part2
  */
async function part2(input) {
  let floor = 0;
  let position = 0;

  for (const line of input) {
    for (const ch of line) {
      ++position;
      switch (ch) {
        case '(':
          ++floor;
          break;
        case ')':
          --floor;
          break;
        case '\n':
          continue;
        default:
          throw SyntaxError(`${ch} is invalid input`);
      }

      if (floor < 0) {
        return position;
      }
    }
  }

  return position;
}

async function main() {
  const input = aoc.input();

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
