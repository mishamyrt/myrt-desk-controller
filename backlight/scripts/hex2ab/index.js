//@ts-check
const { readFile, writeFile } = require('fs/promises');
const { HEXFile, PAGE_SIZE } = require('./HEXFile')
const { argv } = require('process')
const { basename, dirname } = require('path')

/**
 * @param {string} path
 * @returns {Promise<string>}
 */
const readTextFile = (path) => readFile(path, { encoding: 'utf-8' })

/**
 * @param {string} hexPath
 * @returns {Promise<void>}
 */
async function main(hexPath) {
  const content = await readTextFile(hexPath)
  const file = new HEXFile(content)
  if (!file.collectPages()) {
    console.log('HEX reading error')
    process.exit(1)
  }
  const outPath = `${dirname(hexPath)}/${basename(hexPath, '.hex')}.ab`
  console.log(`Parsed ${file.pages.length} bytes → ${file.pages.length / PAGE_SIZE} pages`)
  await writeFile(outPath, Buffer.from(file.pages))
  console.log(`Writed AVRLord binary file to ${outPath}`)
}

if (argv.length < 2) {
    console.log('Too few arguments')
    process.exit(1);
}

main(argv[2])
