const clip = require("bindings")("node_clip_wrapper");
const fs = require('fs')
const { readPngFileSync } = require('node-libpng')

// console.log(clip.getText())
// console.log(clip.hasImage())

// const haha = clip.getImage()

let img = readPngFileSync('../../try/unknown.png')
const trybuf = Buffer.from(Uint32Array.from([0xffff0000, 0xff00ff00, 0xff0000ff,
    0x7fff0000, 0x7f00ff00, 0x7f0000ff]).buffer)
console.log(trybuf)
console.log(img.data)


clip.setImage({
    data: trybuf,
    spec: {
        width: 3,
        height: 2
    }
})

console.log(clip.getImage().data)