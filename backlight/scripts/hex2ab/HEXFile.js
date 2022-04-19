//@ts-check

const PAGE_SIZE = 128;

class HEXFile {
  constructor(file) {
    this.file = file;
    this.pos = 0;
    this.end_of_file = false;
    this.page = Array(PAGE_SIZE);
    this.page_pos = 0;
    this.pages = [];
  }

  collectPages() {
    this.pos = 0;
    while(!this.end_of_file) {
      if (this.pos >= this.file.length - 1) {
          return false;
      }
      if (!this.readLine()) {
          return false;
      }
    }
    if (this.page_pos != 0) {
      this.fillTail()
      this.pages.push(...this.page);
    }
    return true;
  }

  die(message) {
    console.log(message)
    console.log(`Position: ${this.pos}`)
    console.log(`Page position: ${this.page_pos}`)
    console.log(`Previous symbols: ${this.file.substring(this.pos - 10, this.pos)}`)
  }

  readLine() {
    if (this.file[this.pos] != ':') {
      this.die(`Wrong first symbol '${this.file[this.pos]}'`)
      return false;
    }
    this.pos++;
    const length = this.readHexByte() * 2;
    if (this.pos + 7 + length > this.file.length
     || length > PAGE_SIZE) {
      this.die('Wrong length')
      return false;
    }
    switch (this.file[this.pos + 5]) {
      case '0':
        this.pos += 6
        break;
      case '4':
        // Skip line
        this.pos += length + 9;
        return true;
      case '1':
        this.end_of_file = true;
        return true;
      default:
        this.die(`Unknown command symbol '${this.file[this.pos + 6]}'`)
        return false;
    }
    this.readBody(length);
    this.pos += 4;
    return true;
  }

  readBody(len) {
    const end = this.pos + len;
    while (this.pos < end) {
      this.page[this.page_pos] = this.readHexByte();
      this.page_pos++;
      if (this.page_pos == PAGE_SIZE) {
        this.pages.push(...this.page);
        this.page_pos = 0;
      }
    }
  }

  fillTail() {
    while (this.page_pos < PAGE_SIZE) {
      this.page[this.page_pos] = 0;
      this.page_pos++;
    }
  }

  readHexByte() {
    const first = this.file[this.pos++];
    const second = this.file[this.pos++];
    // console.log(`parseHex ${first + second} → ${parseInt(first + second, 16)}`)
    return parseInt(first + second, 16);
  }
}

module.exports = {
  HEXFile,
  PAGE_SIZE
}
