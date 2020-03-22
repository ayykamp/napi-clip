const clip = require('bindings')('napi_clip')

const TEST_IMG_DATA = 
  [0xffff0000, 0xff00ff00, 0xff0000ff,
   0x7fff0000, 0x7f00ff00, 0x7f0000ff]
const TEST_IMG_SPEC = {
  width: 3,
  height: 2
}
clip.setImage(new Uint32Array(TEST_IMG_DATA), TEST_IMG_SPEC)
const xd = clip.getImage()

console.log(xd.data.length)