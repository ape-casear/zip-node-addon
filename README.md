# zip-node-addon
Node native addon which wrap the "[kuba--/zip](https://github.com/kuba--/zip)"

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
}).then(buf => {
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

# benchmark
其中JavaScript使用的是[JsZip](https://github.com/Stuk/jszip), node-addon使用的是封装的zip，c++是使用的[kuba--/zip](https://github.com/kuba--/zip), 流程就是解压已经读取到内存的压缩数据到指定位置（zip文件大小3.34MB,解压后26.1MB)

Unzip from memory to files.

下方表格是在windows10，Node版本 v12.22.1, CPU-i5-8400下测试的
|   impl  | cost  |
|  ----  | ----  |
| Javascript  | 259.938ms |
| node-addon  | 125.162ms |
| c/c++  | 132 ms |


下方表格是在mac，Node版本 v12.10.0, CPU-i5下测试的
|   impl  | cost  |
|  ----  | ----  |
| Javascript  | 266.769ms |
| node-addon  | 124.211ms |
| c/c++  | 122 ms |

# Test
`npm run test`


# License
MIT license.See LICENSE file for details.
