# zip-node-addon
Node native addon which wrap the "kuba--/zip"

# Install
`npm install zip-node-addon`

# Quick Start
unzip by memory
```js
var zipAddon = require('zip-node-addon');
const fs = require('fs')
const path =require('path')

const buf = fs.readFileSync(path.join(__dirname, 'zip.test.zip'))
zipAddon.unzipStream(buf, "./temp").then(resp => {
    console.log(resp)
    let ab = JSON.parse(resp)
    console.log(ab)
}).catch(err => console.error(err))
```

unzip by file
```js
var zipAddon = require('zip-node-addon');
const fs = require('fs')
const path =require('path')
await zipAddon.unzipFile(path.join(__dirname, 'zip.test.zip'), "./temp/unzipFile").then(jsonStr => {
    console.log(jsonStr)
    let jsObj = JSON.parse(jsonStr)
    console.log(jsObj)
}).catch(err => console.error(err))
```

zip files and return buffer
```js
await zipAddon.zipBuffer({
    file1: Buffer.from("I am plain text"),
    file2: Buffer.from(fs.readFileSync(path.join(__dirname, '../README.md'))),
    'dir/file3': Buffer.from("file in dir"),
}).then(ab => {
    const buf = Buffer.from(ab)
    fs.writeFileSync(path.join(__dirname, '../temp/test.zip'), buf)
}).catch(err => console.error(err))
```

get all entries
```js
await zipAddon.listAllEntriesFromFile(path.join(__dirname, 'zip.test.zip')).then(jsonStr => {
    console.log(jsonStr)
    let jsObj = JSON.parse(jsonStr)
    console.log(jsObj)
}).catch(err => console.error(err))

// or
const buf = fs.readFileSync(path.join(__dirname, 'zip.test.zip'))
await zipAddon.listAllEntriesFromBuffer(buf).then(jsonStr => {
    console.log(jsonStr)
    let jsObj = JSON.parse(jsonStr)
    console.log(jsObj)
}).catch(err => console.error(err))
```

# Test
`npm run test`


# License
MIT license.See LICENSE file for details.
