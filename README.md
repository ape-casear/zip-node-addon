# zip-node-addon
Node native addon which wrap the "kuba--/zip"


# Quick Start
```
var zipAddon = require('zip-node-addon');
const fs = require('fs')
const path =require('path')

const buf = fs.readFileSync(path.join(__dirname, './zip.test.zip'))
const uint8Array = new Uint8Array(buf.buffer, buf.byteOffset, buf.length / Uint8Array.BYTES_PER_ELEMENT)
zipAddon.unzipStream(uint8Array, "./temp").then(resp => {
    console.log(resp)
    let ab = JSON.parse(resp)
    console.log(ab)
}).catch(err => console.error(err))
```

# Test
`npm run test`


# License
MIT license.See LICENSE file for details.
