var zipAddon = require('bindings')('zipAddon');
const fs = require('fs')
const path =require('path')

console.log(zipAddon.hello());

const buf = fs.readFileSync(path.join(__dirname, './zip.test.zip'))
const uint8Array = new Uint8Array(buf.buffer, buf.byteOffset, buf.length / Uint8Array.BYTES_PER_ELEMENT)
zipAddon.unzipStream(uint8Array, "./temp").then(resp => {
    console.log(resp)
    let ab = JSON.parse(resp)
    console.log(ab)
}).catch(err => console.error(err))