const clip = require('bindings')('napi_clip')

const TEST_STRING = 'Test!@♂\n'
const TEST_IMG_DATA = 
  [0xffff0000, 0xff00ff00, 0xff0000ff,
   0x7fff0000, 0x7f00ff00, 0x7f0000ff]
const TEST_IMG_SPEC = {
  width: 3,
  height: 2,
  bitsPerPixel: 32,
  bytesPerRow: 3 * 4,
  redMask: 0xff000000,
  greenMask: 0xff0000,
  blueMask: 0xff00,
  alphaMask: 0xff,
  redShift: 24,
  greenShift: 16,
  blueShift: 8,
  alphaShift: 0
} 

// clear clipboard in between tests
beforeEach(clip.clear)
afterEach(clip.clear);

test('Getting and setting clipboard text data', () => {
  clip.setText(TEST_STRING)

  // expect(clip.isEmpty()).toBe(false)
  expect(clip.hasText()).toBe(true)

  expect(clip.getText()).toBe(TEST_STRING)
})

test('Getting and setting clipboard image data', () => {
  const imgDataBuffer = Buffer.from(Uint32Array.from(TEST_IMG_DATA).buffer)
  
  clip.setImage({
    data: imgDataBuffer,
    spec: TEST_IMG_SPEC
  })

  // expect(clip.isEmpty()).toBe(false)
  expect(clip.hasImage()).toBe(true)

  const img = clip.getImage()
  expect(img.spec).toEqual(clip.getImage().spec)
  expect(img.data).toEqual(clip.getImage().data)
})

// https://github.com/dacap/clip/issues/28
/* test('Clearing and empty format', () => {
  clip.clear()

  expect(clip.hasText()).toBe(false)
  expect(clip.hasImage()).toBe(false)
  expect(clip.isEmpty()).toBe(true)
}) */
