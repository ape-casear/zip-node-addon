# zip-node-addon
Node native addon which wrap the "kuba--/zip"


# Quick Start
unzip from memory
```
var zipAddon = require('zip-node-addon');
const fs = require('fs')
const path =require('path')

const buf = fs.readFileSync(path.join(__dirname, 'zip.test.zip'))
const uint8Array = new Uint8Array(buf.buffer, buf.byteOffset, buf.length / Uint8Array.BYTES_PER_ELEMENT)
zipAddon.unzipStream(uint8Array, "./temp").then(resp => {
    console.log(resp)
    let ab = JSON.parse(resp)
    console.log(ab)
}).catch(err => console.error(err))
```

unzip from file
```
var zipAddon = require('zip-node-addon');
const fs = require('fs')
const path =require('path')
await zipAddon.unzipFile(path.join(__dirname, 'zip.test.zip'), "./temp/unzipFile").then(jsonStr => {
    console.log(jsonStr)
    let jsObj = JSON.parse(jsonStr)
    console.log(jsObj)
}).catch(err => console.error(err))
```


# Test
`npm run test`


# License
MIT license.See LICENSE file for details.
