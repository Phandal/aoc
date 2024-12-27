/*
 * Common operations used between days
 */

import * as fs from 'fs';

/**
  * Reads all input from the provided path or stdin by default
  * @param {fs.PathOrFileDescriptor} [fd]
  * @returns {string[]}
  */
export function input(fd = process.stdin.fd) {
  return fs.readFileSync(fd).toString().split('\n');
}
