var clip = require("bindings")("node_clip_wrapper");

// console.log(clip.getText())
// console.log(clip.hasImage())

const haha = clip.getImage()

console.log(haha)

clip.setImage({
    data: Buffer.from([0xffff0000, 0xff00ff00, 0xff0000ff,
                       0x7fff0000, 0x7f00ff00, 0x7f0000ff]),
    spec: {
        width: 3,
        height: 2
    }
})