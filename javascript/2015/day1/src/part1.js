import * as fs from 'fs'

async function main() {
  const data = fs.readFileSync(process.stdin.fd).toString();
  let floor = 0;

  for (const c of data) {
    switch (c) {
      case '(':
        ++floor;
        break;
      case ')':
        --floor
        break;
      case '\n':
        continue;
      default:
        throw SyntaxError(`${c} is invalid input`);
    }
  }

  console.log('Floor', floor);
}

main()
  .catch((err) => {
    console.log(`Error: ${err}`)
  })
  .finally(() => console.log('Complete'));
