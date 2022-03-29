var zipAddon = require('bindings')('zipAddon');
const fs = require('fs')
const path =require('path')

console.log(zipAddon.hello());


async function run () {
    const buf = fs.readFileSync(path.join(__dirname, './zip.test.zip'))
    const uint8Array = new Uint8Array(buf.buffer, buf.byteOffset, buf.length / Uint8Array.BYTES_PER_ELEMENT)
    await zipAddon.unzipStream(uint8Array, "./temp").then(jsonStr => {
        console.log(jsonStr)
        let jsObj = JSON.parse(jsonStr)
        console.log(jsObj)
    }).catch(err => console.error(err))
    
    
    await zipAddon.unzipFile(path.join(__dirname, 'zip.test.zip'), "./temp/unzipFile").then(jsonStr => {
        console.log(jsonStr)
        let jsObj = JSON.parse(jsonStr)
        console.log(jsObj)
    }).catch(err => console.error(err))
}
run ()