const clip = require('bindings')('napi_clip')

const TEST_STRING = 'Test!@♂\n'
const TEST_IMG_DATA = 
  [0xffff0000, 0xff00ff00, 0xff0000ff,
   0x7fff0000, 0x7f00ff00, 0x7f0000ff]
const TEST_IMG_SPEC = {
  width: 3,
  height: 2
} 

// clear clipboard in between tests
beforeEach(clip.clear)
afterEach(clip.clear)

test('Getting and setting clipboard text data', () => {
  clip.setText(TEST_STRING)

  expect(clip.hasText()).toBe(true)

  expect(clip.getText()).toBe(TEST_STRING)
})

test('Getting and setting clipboard image data', () => {
  const imgDataArray = new Uint32Array(TEST_IMG_DATA)
  
  expect(clip.setImage(imgDataArray, TEST_IMG_SPEC)).toBe(true)

  expect(clip.hasImage()).toBe(true)

  expect(!!clip.getImage()).toBe(true)
})

// https://github.com/dacap/clip/issues/28
/* test('Clearing and empty format', () => {
  clip.clear()

  expect(clip.hasText()).toBe(false)
  expect(clip.hasImage()).toBe(false)
  // expect(clip.isEmpty()).toBe(true)
}) */
