var clip = require("bindings")("node_clip_wrapper");

// clip.setText('haha youre gay')

// console.log(clip.getText())
// console.log(clip.hasImage())


const haha = clip.getImage()
console.log(haha)
clip.setImage(haha)