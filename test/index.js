var zipAddon = require('bindings')('zipAddon');
const fs = require('fs')
const path = require('path')

console.log(zipAddon.hello());

async function run() {
    console.time('unzipStream')
    const buf = fs.readFileSync(path.join(__dirname, './zip.test.zip'))
    await zipAddon.unzipStream(buf, "./temp").then(jsonStr => {
        let jsObj = JSON.parse(jsonStr)
        console.log(jsObj)
    }).catch(err => console.error(err))
    console.timeEnd('unzipStream')

    console.time('unzipFile')
    await zipAddon.unzipFile(path.join(__dirname, 'zip.test.zip'), "./temp/unzipFile").then(jsonStr => {
        let jsObj = JSON.parse(jsonStr)
        console.log(jsObj)
    }).catch(err => console.error(err))
    console.timeEnd('unzipFile')

    console.time('listAllEntriesFromFile')
    await zipAddon.listAllEntriesFromFile(path.join(__dirname, 'zip.test.zip')).then(jsonStr => {
        let jsObj = JSON.parse(jsonStr)
        console.log(jsObj)
    }).catch(err => console.error(err))
    console.timeEnd('listAllEntriesFromFile')

    console.time('listAllEntriesFromBuffer')
    await zipAddon.listAllEntriesFromBuffer(buf).then(jsonStr => {
        let jsObj = JSON.parse(jsonStr)
        console.log(jsObj)
    }).catch(err => console.error(err))
    console.timeEnd('listAllEntriesFromBuffer')

    console.time('zipBuffer')
    await zipAddon.zipBuffer({
        file1: Buffer.from("I am plain text"),
        file2: Buffer.from(fs.readFileSync(path.join(__dirname, '../README.md'))),
        'dir/file3': Buffer.from("file in dir"),
    }).then(buf => {
        fs.writeFileSync(path.join(__dirname, '../temp/test.zip'), buf)
    }).catch(err => console.error(err))
    console.timeEnd('zipBuffer')
}
run()