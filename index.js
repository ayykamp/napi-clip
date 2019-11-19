const binary = require('node-pre-gyp')
const path = require('path')
const binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')))
module.exports = require(binding_path)