#include "framebuffer.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

static const uint8_t font[1808] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc7, 0xc7, 0xc7, 0xcf, 0xcf, 0xff, 0xcf, 0xff,
    0x93, 0x93, 0x93, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x93, 0x01, 0x93, 0x93, 0x93, 0x01, 0x93, 0xff,
    0xef, 0x83, 0x2f, 0x83, 0xe9, 0x03, 0xef, 0xff,
    0x9d, 0x5b, 0x37, 0xef, 0xd9, 0xb5, 0x73, 0xff,
    0x8f, 0x27, 0x27, 0x8f, 0x25, 0x33, 0x81, 0xff,
    0xcf, 0xcf, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf3, 0xe7, 0xcf, 0xcf, 0xcf, 0xe7, 0xf3, 0xff,
    0x9f, 0xcf, 0xe7, 0xe7, 0xe7, 0xcf, 0x9f, 0xff,
    0xff, 0x93, 0xc7, 0x01, 0xc7, 0x93, 0xff, 0xff,
    0xff, 0xe7, 0xe7, 0x81, 0xe7, 0xe7, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xcf, 0x9f,
    0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xcf, 0xff,
    0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f, 0xff,
    0xc7, 0xb3, 0x39, 0x39, 0x39, 0x9b, 0xc7, 0xff,
    0xe7, 0xc7, 0xe7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x83, 0x39, 0xf1, 0xc3, 0x87, 0x1f, 0x01, 0xff,
    0x81, 0xf3, 0xe7, 0xc3, 0xf9, 0x39, 0x83, 0xff,
    0xe3, 0xc3, 0x93, 0x33, 0x01, 0xf3, 0xf3, 0xff,
    0x03, 0x3f, 0x03, 0xf9, 0xf9, 0x39, 0x83, 0xff,
    0xc3, 0x9f, 0x3f, 0x03, 0x39, 0x39, 0x83, 0xff,
    0x01, 0x39, 0xf3, 0xe7, 0xcf, 0xcf, 0xcf, 0xff,
    0x87, 0x3b, 0x1b, 0x87, 0x61, 0x79, 0x83, 0xff,
    0x83, 0x39, 0x39, 0x81, 0xf9, 0xf3, 0x87, 0xff,
    0xff, 0xcf, 0xcf, 0xff, 0xcf, 0xcf, 0xff, 0xff,
    0xff, 0xcf, 0xcf, 0xff, 0xcf, 0xcf, 0x9f, 0xff,
    0xf3, 0xe7, 0xcf, 0x9f, 0xcf, 0xe7, 0xf3, 0xff,
    0xff, 0xff, 0x01, 0xff, 0x01, 0xff, 0xff, 0xff,
    0x9f, 0xcf, 0xe7, 0xf3, 0xe7, 0xcf, 0x9f, 0xff,
    0x83, 0x01, 0x39, 0xf3, 0xc7, 0xff, 0xc7, 0xff,
    0x83, 0x7d, 0x45, 0x55, 0x41, 0x7f, 0x83, 0xff,
    0xc7, 0x93, 0x39, 0x39, 0x01, 0x39, 0x39, 0xff,
    0x03, 0x39, 0x39, 0x03, 0x39, 0x39, 0x03, 0xff,
    0xc3, 0x99, 0x3f, 0x3f, 0x3f, 0x99, 0xc3, 0xff,
    0x07, 0x33, 0x39, 0x39, 0x39, 0x33, 0x07, 0xff,
    0x01, 0x3f, 0x3f, 0x03, 0x3f, 0x3f, 0x01, 0xff,
    0x01, 0x3f, 0x3f, 0x03, 0x3f, 0x3f, 0x3f, 0xff,
    0xc1, 0x9f, 0x3f, 0x31, 0x39, 0x99, 0xc1, 0xff,
    0x39, 0x39, 0x39, 0x01, 0x39, 0x39, 0x39, 0xff,
    0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0x39, 0x83, 0xff,
    0x39, 0x33, 0x27, 0x0f, 0x07, 0x23, 0x31, 0xff,
    0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x81, 0xff,
    0x39, 0x11, 0x01, 0x01, 0x29, 0x39, 0x39, 0xff,
    0x39, 0x19, 0x09, 0x01, 0x21, 0x31, 0x39, 0xff,
    0x83, 0x39, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x03, 0x39, 0x39, 0x39, 0x03, 0x3f, 0x3f, 0xff,
    0x83, 0x39, 0x39, 0x39, 0x21, 0x33, 0x85, 0xff,
    0x03, 0x39, 0x39, 0x31, 0x07, 0x23, 0x31, 0xff,
    0x87, 0x33, 0x3f, 0x83, 0xf9, 0x39, 0x83, 0xff,
    0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x39, 0x39, 0x39, 0x11, 0x83, 0xc7, 0xef, 0xff,
    0x39, 0x39, 0x29, 0x01, 0x01, 0x11, 0x39, 0xff,
    0x39, 0x11, 0x83, 0xc7, 0x83, 0x11, 0x39, 0xff,
    0x99, 0x99, 0x99, 0xc3, 0xe7, 0xe7, 0xe7, 0xff,
    0x01, 0xf1, 0xe3, 0xc7, 0x8f, 0x1f, 0x01, 0xff,
    0xc3, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xc3, 0xff,
    0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xff,
    0x87, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x87, 0xff,
    0xc7, 0x93, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01,
    0xef, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0x3f, 0x3f, 0x03, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xff, 0x81, 0x3f, 0x3f, 0x3f, 0x81, 0xff,
    0xf9, 0xf9, 0x81, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xff, 0xff, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xf1, 0xe7, 0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0xff, 0xff, 0x81, 0x39, 0x39, 0x81, 0xf9, 0x83,
    0x3f, 0x3f, 0x03, 0x39, 0x39, 0x39, 0x39, 0xff,
    0xe7, 0xff, 0xc7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xf3, 0xff, 0xe3, 0xf3, 0xf3, 0xf3, 0xf3, 0x87,
    0x3f, 0x3f, 0x31, 0x03, 0x07, 0x23, 0x31, 0xff,
    0xc7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xff, 0xff, 0x03, 0x49, 0x49, 0x49, 0x49, 0xff,
    0xff, 0xff, 0x03, 0x39, 0x39, 0x39, 0x39, 0xff,
    0xff, 0xff, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xff, 0x03, 0x39, 0x39, 0x03, 0x3f, 0x3f,
    0xff, 0xff, 0x81, 0x39, 0x39, 0x81, 0xf9, 0xf9,
    0xff, 0xff, 0x91, 0x8f, 0x9f, 0x9f, 0x9f, 0xff,
    0xff, 0xff, 0x83, 0x3f, 0x83, 0xf9, 0x03, 0xff,
    0xe7, 0xe7, 0x81, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0xff, 0xff, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xff, 0xff, 0x99, 0x99, 0x99, 0xc3, 0xe7, 0xff,
    0xff, 0xff, 0x49, 0x49, 0x49, 0x49, 0x81, 0xff,
    0xff, 0xff, 0x39, 0x01, 0xc7, 0x01, 0x39, 0xff,
    0xff, 0xff, 0x39, 0x39, 0x39, 0x81, 0xf9, 0x83,
    0xff, 0xff, 0x01, 0xe3, 0xc7, 0x8f, 0x01, 0xff,
    0xf3, 0xe7, 0xe7, 0xcf, 0xe7, 0xe7, 0xf3, 0xff,
    0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xff,
    0x9f, 0xcf, 0xcf, 0xe7, 0xcf, 0xcf, 0x9f, 0xff,
    0xff, 0xff, 0x8f, 0x45, 0xe3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x93, 0x93, 0xff,
    0x83, 0x29, 0x29, 0x11, 0x29, 0x29, 0x83, 0xff,
    0x83, 0x39, 0x09, 0x11, 0x21, 0x39, 0x83, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x83, 0x11, 0x21, 0x7d, 0x21, 0x11, 0x83, 0xff,
    0x83, 0x11, 0x09, 0x7d, 0x09, 0x11, 0x83, 0xff,
    0x83, 0x11, 0x39, 0x55, 0x11, 0x11, 0x83, 0xff,
    0x83, 0x11, 0x11, 0x55, 0x39, 0x11, 0x83, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe7, 0xff, 0xe7, 0xe7, 0xc7, 0xc7, 0xc7, 0xff,
    0xef, 0x83, 0x29, 0x2f, 0x29, 0x83, 0xef, 0xff,
    0xc3, 0x99, 0x9f, 0x03, 0x9f, 0x9f, 0x01, 0xff,
    0xff, 0xa5, 0xdb, 0xdb, 0xdb, 0xa5, 0xff, 0xff,
    0x99, 0x99, 0xc3, 0x81, 0xe7, 0x81, 0xe7, 0xff,
    0xe7, 0xe7, 0xe7, 0xff, 0xe7, 0xe7, 0xe7, 0xff,
    0xc3, 0x99, 0x87, 0xdb, 0xe1, 0x99, 0xc3, 0xff,
    0x93, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc3, 0xbd, 0x66, 0x5e, 0x5e, 0x66, 0xbd, 0xc3,
    0x87, 0xc3, 0x93, 0xc3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc9, 0x93, 0x27, 0x93, 0xc9, 0xff, 0xff,
    0xff, 0xff, 0x81, 0xf9, 0xf9, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc3, 0xbd, 0x46, 0x5a, 0x46, 0x5a, 0xbd, 0xc3,
    0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xef, 0xd7, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe7, 0xe7, 0x81, 0xe7, 0xe7, 0xff, 0x81, 0xff,
    0xc7, 0xf3, 0xe7, 0xc3, 0xff, 0xff, 0xff, 0xff,
    0xc3, 0xe7, 0xf3, 0xc7, 0xff, 0xff, 0xff, 0xff,
    0xf7, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x33, 0x33, 0x33, 0x33, 0x09, 0x3f,
    0xc1, 0x95, 0xb5, 0x95, 0xc1, 0xf5, 0xf5, 0xff,
    0xff, 0xff, 0xff, 0xcf, 0xcf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xcf,
    0xe7, 0xc7, 0xe7, 0xc3, 0xff, 0xff, 0xff, 0xff,
    0xc7, 0x93, 0x93, 0xc7, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x27, 0x93, 0xc9, 0x93, 0x27, 0xff, 0xff,
    0xbd, 0x3b, 0xb7, 0xad, 0xd9, 0xb1, 0x7d, 0xff,
    0xbd, 0x3b, 0xb7, 0xa9, 0xdd, 0xbb, 0x71, 0xff,
    0x1d, 0xbb, 0xd7, 0x2d, 0xd9, 0xb1, 0x7d, 0xff,
    0xc7, 0xff, 0xc7, 0x9f, 0x39, 0x01, 0x83, 0xff,
    0xdf, 0xef, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xf7, 0xef, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xc7, 0x93, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xcb, 0xa7, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0x93, 0xff, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xef, 0xd7, 0xc7, 0x93, 0x39, 0x01, 0x39, 0xff,
    0xc1, 0x87, 0x27, 0x21, 0x07, 0x27, 0x21, 0xff,
    0xc3, 0x99, 0x3f, 0x3f, 0x99, 0xc3, 0xf7, 0xcf,
    0xdf, 0xef, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0xf7, 0xef, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0xc7, 0x93, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0x93, 0xff, 0x01, 0x3f, 0x03, 0x3f, 0x01, 0xff,
    0xef, 0xf7, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xf7, 0xef, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0xe7, 0xc3, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x99, 0xff, 0x81, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x87, 0x93, 0x99, 0x09, 0x99, 0x93, 0x87, 0xff,
    0xcb, 0xa7, 0x19, 0x09, 0x01, 0x21, 0x31, 0xff,
    0xdf, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xc7, 0x93, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xcb, 0xa7, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x93, 0xff, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xbb, 0xd7, 0xef, 0xd7, 0xbb, 0xff, 0xff,
    0x83, 0x39, 0x31, 0x29, 0x19, 0x39, 0x83, 0xff,
    0xdf, 0xef, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xc7, 0x93, 0xff, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x93, 0xff, 0x39, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x99, 0x99, 0xc3, 0xe7, 0xe7, 0xff,
    0x3f, 0x03, 0x39, 0x39, 0x39, 0x03, 0x3f, 0xff,
    0xc3, 0x99, 0x99, 0x93, 0x99, 0x89, 0x93, 0xff,
    0xdf, 0xef, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xf7, 0xef, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xc7, 0x93, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xcb, 0xa7, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0x93, 0xff, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xef, 0xd7, 0x83, 0xf9, 0x81, 0x39, 0x81, 0xff,
    0xff, 0xff, 0x83, 0xe9, 0x81, 0x2f, 0x83, 0xff,
    0xff, 0xff, 0x81, 0x3f, 0x3f, 0x81, 0xf7, 0xcf,
    0xdf, 0xef, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xf7, 0xef, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xc7, 0x93, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0x93, 0xff, 0x83, 0x39, 0x01, 0x3f, 0x83, 0xff,
    0xdf, 0xef, 0xff, 0xc7, 0xe7, 0xe7, 0x81, 0xff,
    0xf7, 0xef, 0xff, 0xc7, 0xe7, 0xe7, 0x81, 0xff,
    0xc7, 0x93, 0xff, 0xc7, 0xe7, 0xe7, 0x81, 0xff,
    0x93, 0xff, 0xc7, 0xe7, 0xe7, 0xe7, 0x81, 0xff,
    0x9b, 0x87, 0x67, 0x83, 0x39, 0x39, 0x83, 0xff,
    0xcb, 0xa7, 0x03, 0x39, 0x39, 0x39, 0x39, 0xff,
    0xdf, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xf7, 0xef, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xc7, 0x93, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xcb, 0xa7, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0x93, 0xff, 0x83, 0x39, 0x39, 0x39, 0x83, 0xff,
    0xff, 0xe7, 0xff, 0x81, 0xff, 0xe7, 0xff, 0xff,
    0xff, 0xff, 0x83, 0x31, 0x29, 0x19, 0x83, 0xff,
    0xdf, 0xef, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xf7, 0xef, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xc7, 0x93, 0xff, 0x39, 0x39, 0x39, 0x81, 0xff,
    0x93, 0xff, 0x39, 0x39, 0x39, 0x39, 0x81, 0xff,
    0xf7, 0xef, 0x39, 0x39, 0x39, 0x81, 0xf9, 0x83,
    0x3f, 0x3f, 0x03, 0x39, 0x39, 0x03, 0x3f, 0x3f,
    0x93, 0xff, 0x39, 0x39, 0x39, 0x81, 0xf9, 0x83,
    0xef, 0xd7, 0xab, 0x09, 0x11, 0x83, 0xd7, 0xef,
    0x83, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x83
};

static const uint8_t* drawColors;
static uint8_t* framebuffer;

static int w4_min (int a, int b) {
    return a < b ? a : b;
}

static int w4_max (int a, int b) {
    return a > b ? a : b;
}

static void drawPoint (uint8_t color, int x, int y) {
    int idx = (WIDTH * y + x) >> 2;
    int shift = (x & 0x3) << 1;
    int mask = 0x3 << shift;
    framebuffer[idx] = (color << shift) | (framebuffer[idx] & ~mask);
}

static void drawPointUnclipped (uint8_t color, int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        drawPoint(color, x, y);
    }
}

static void drawHLine (uint8_t color, int startX, int y, int endX) {
    int fillEnd = endX - (endX & 3);
    int fillStart = w4_min((startX + 3) & ~3, fillEnd);

    if (fillEnd - fillStart > 3) {
        for (int xx = startX; xx < fillStart; xx++) {
            drawPoint(color, xx, y);
        }

        int from = (WIDTH * y + fillStart) >> 2;
        int to = (WIDTH * y + fillEnd) >> 2;
        uint8_t fillColor = color * 0x55;

        memset(framebuffer+from, fillColor, to-from);
        startX = fillEnd;
    }

    for (int xx = startX; xx < endX; xx++) {
        drawPoint(color, xx, y);
    }
}

static void drawHLineUnclipped (uint8_t color, int startX, int y, int endX) {
    if (y >= 0 && y < HEIGHT) {
        if (startX < 0) {
            startX = 0;
        }
        if (endX > WIDTH) {
            endX = WIDTH;
        }
        if (startX < endX) {
            drawHLine(color, startX, y, endX);
        }
    }
}

void w4_framebufferInit (const uint8_t* drawColors_, uint8_t* framebuffer_) {
    drawColors = drawColors_;
    framebuffer = framebuffer_;
}

void w4_framebufferClear () {
    memset(framebuffer, 0, WIDTH*HEIGHT >> 2);
}

void w4_framebufferHLine (int x, int y, int len) {
    uint8_t dc0 = drawColors[0] & 0xf;
    if (dc0 == 0) {
        return;
    }

    uint8_t strokeColor = (dc0 - 1) & 0x3;
    drawHLineUnclipped(strokeColor, x, y, x + len);
}

void w4_framebufferVLine (int x, int y, int len) {
    if (y + len <= 0 || x < 0 || x >= WIDTH) {
        return;
    }

    uint8_t dc0 = drawColors[0] & 0xf;
    if (dc0 == 0) {
        return;
    }

    int startY = w4_max(0, y);
    int endY = w4_min(HEIGHT, y + len);
    uint8_t strokeColor = (dc0 - 1) & 0x3;
    for (int yy = startY; yy < endY; yy++) {
        drawPoint(strokeColor, x, yy);
    }
}

void w4_framebufferRect (int x, int y, int width, int height) {
    int startX = w4_max(0, x);
    int startY = w4_max(0, y);
    int endXUnclamped = x + width;
    int endYUnclamped = y + height;
    int endX = w4_min(endXUnclamped, WIDTH);
    int endY = w4_min(endYUnclamped, HEIGHT);

    uint8_t dc01 = drawColors[0];
    uint8_t dc0 = dc01 & 0xf;
    uint8_t dc1 = (dc01 >> 4) & 0xf;

    if (dc0 != 0) {
        uint8_t fillColor = (dc0 - 1) & 0x3;
        for (int yy = startY; yy < endY; ++yy) {
            drawHLine(fillColor, startX, yy, endX);
        }
    }

    if (dc1 != 0) {
        uint8_t strokeColor = (dc1 - 1) & 0x3;

        // Left edge
        if (x >= 0 && x < WIDTH) {
            for (int yy = startY; yy < endY; ++yy) {
                drawPoint(strokeColor, x, yy);
            }
        }

        // Right edge
        if (endXUnclamped >= 0 && endXUnclamped <= WIDTH) {
            for (int yy = startY; yy < endY; ++yy) {
                drawPoint(strokeColor, endXUnclamped - 1, yy);
            }
        }

        // Top edge
        if (y >= 0 && y < HEIGHT) {
            drawHLine(strokeColor, startX, y, endX);
        }

        // Bottom edge
        if (endYUnclamped >= 0 && endYUnclamped <= HEIGHT) {
            drawHLine(strokeColor, startX, endYUnclamped - 1, endX);
        }
    }
}

void w4_framebufferOval (int x, int y, int width, int height) {
    uint8_t dc01 = drawColors[0];
    uint8_t dc0 = dc01 & 0xf;
    uint8_t dc1 = (dc01 >> 4) & 0xf;

    if (dc1 == 0xf) {
        return;
    }

    uint8_t strokeColor = (dc1 - 1) & 0x3;
    uint8_t fillColor = (dc0 - 1) & 0x3;

    int a = width >> 1;
    int b = height >> 1;

    if (a <= 0) return;
    if (b <= 0) return;

    int x0 = x + a, y0 = y + b;
    int aa2 = a * a * 2, bb2 = b * b * 2;

    {
        int x = a, y = 0;
        int dx = (1 - 2 * a) * b * b, dy = a * a;
        int sx = bb2 * a, sy = 0;
        int e = 0;

        while (sx >= sy) {
            drawPointUnclipped(strokeColor, x0 + x, y0 + y); /*   I. Quadrant */
            drawPointUnclipped(strokeColor, x0 + x, y0 - y); /*  II. Quadrant */
            drawPointUnclipped(strokeColor, x0 - x, y0 + y); /* III. Quadrant */
            drawPointUnclipped(strokeColor, x0 - x, y0 - y); /*  IV. Quadrant */

            if (dc0 != 0) {
                int start = x0 - x + 1;
                int end = x0 + x;
                drawHLineUnclipped(fillColor, start, y0 + y, end); /*   I and III. Quadrant */
                drawHLineUnclipped(fillColor, start, y0 - y, end); /*  II and IV. Quadrant */
            }

            y++;
            sy += aa2;
            e += dy;
            dy += aa2;
            if (2 * e + dx > 0) {
                x--;
                sx -= bb2;
                e += dx;
                dx += bb2;
            }
        }
    }

    {
        int x = 0, y = b;
        int dx = b * b, dy = (1 - 2 * b) * a * a;
        int sx = 0, sy = aa2 * b;
        int e = 0;
        int ddx = 0;

        while (sy >= sx) {
            drawPointUnclipped(strokeColor, x0 + x, y0 + y); /*   I. Quadrant */
            drawPointUnclipped(strokeColor, x0 + x, y0 - y); /*  II. Quadrant */
            drawPointUnclipped(strokeColor, x0 - x, y0 + y); /* III. Quadrant */
            drawPointUnclipped(strokeColor, x0 - x, y0 - y); /*  IV. Quadrant */

            x++;
            sx += bb2;
            e += dx;
            dx += bb2;
            ddx++;
            if (2 * e + dy > 0) {
                if (dc0 != 0) {
                    int w = x - ddx - 1;
                    int start = x0 - w;
                    int end = x0 + w + 1;
                    drawHLineUnclipped(fillColor, start, y0 + y, end); /*   I and III. Quadrant */
                    drawHLineUnclipped(fillColor, start, y0 - y, end); /*  II and IV. Quadrant */
                }

                y--;
                sy -= aa2;
                e += dy;
                dy += aa2;
                ddx = 0;
            }
        }
    }
}

void w4_framebufferLine (int x1, int y1, int x2, int y2) {
    uint8_t dc0 = drawColors[0] & 0xf;
    if (dc0 == 0) {
        return;
    }
    uint8_t strokeColor = (dc0 - 1) & 0x3;

    if (y1 > y2) {
        int swap = x1;
        x1 = x2;
        x2 = swap;

        swap = y1;
        y1 = y2;
        y2 = swap;
    }

    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = y2 - y1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        drawPointUnclipped(strokeColor, x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1++;
        }
    }
}

void w4_framebufferText (const uint8_t* str, int x, int y) {
    for (int currentX = x; *str != '\0'; ++str) {
        if (*str == 10) {
            y += 8;
            currentX = x;
        } else if (*str < 32) {
            currentX += 8;
        } else {
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, (*str - 32) << 3, 8,
                false, false, false, false);
            currentX += 8;
        }
    }
}

void w4_framebufferText8 (const uint8_t* str, int byteLength, int x, int y) {
    for (int currentX = x; byteLength > 0; ++str, --byteLength) {
        if (*str == 10) {
            y += 8;
            currentX = x;
        } else if (*str < 32) {
            currentX += 8;
        } else {
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, (*str - 32) << 3, 8,
                false, false, false, false);
            currentX += 8;
        }
    }
}

void w4_framebufferText16 (const uint16_t* str, int byteLength, int x, int y) {
    for (int currentX = x; byteLength > 0; ++str, byteLength -= 2) {
        uint16_t c = w4_read16LE(str);
        if (c == 10) {
            y += 8;
            currentX = x;
        } else if (c < 32) {
            currentX += 8;
        } else if (c >= 256) {
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, 224 << 3, 8,
                false, false, false, false);
            currentX += 8;
        } else {
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, (c - 32) << 3, 8,
                false, false, false, false);
            currentX += 8;
        }
    }
}

void w4_framebufferTextFromUtf8 (const uint8_t* str, int byteLength, int x, int y) {
    // Instead of fully parsing UTF-8, make sure we can decode the lower (0-127)
    // and upper (128-255) halves of ASCII characters encoded as UTF-8.
    uint8_t previousByte = 0;
    for (int currentX = x; byteLength > 0; ++str, --byteLength) {
        uint8_t byte = *str;
        uint8_t offset;
        if (byte == 10) {
            y += 8;
            currentX = x;
        } else if (byte < 32) {
            currentX += 8;
        } else if ((byte & 0b11111110) == (0b1100001 << 1)) {
            // If str only contains UTF-8 encoded characters between U+0000
            // and U+00FF, this is the first byte of an upper ASCII character.
        } else {
            if (byte < 128) {
                // This is a lower ASCII character.
                offset = byte - 32;
            } else if (((previousByte & 0b11111110) == (0b1100001 << 1))
                && ((byte & 0b11000000) == (0b10 << 6))) {
                // This is an upper ASCII character.
                uint8_t combined = ((previousByte & 0b00000011) << 6) | (byte & 0b00111111);
                offset = combined - 32;
            } else {
                // This is either part of a UTF-8 encoded character above
                // U+00FF, or invalid UTF-8.
                offset = 224;
            }
            w4_framebufferBlit(font, currentX, y, 8, 8, 0, offset << 3, 8,
                false, false, false, false);
            currentX += 8;
        }
        previousByte = byte;
    }
}

void w4_framebufferBlit (const uint8_t* sprite, int dstX, int dstY, int width, int height,
    int srcX, int srcY, int srcStride, bool bpp2, bool flipX, bool flipY, bool rotate) {

    uint16_t colors = drawColors[0] | (drawColors[1] << 8);

    // Clip rectangle to screen
    int clipXMin, clipYMin, clipXMax, clipYMax;
    if (rotate) {
        flipX = !flipX;
        clipXMin = w4_max(0, dstY) - dstY;
        clipYMin = w4_max(0, dstX) - dstX;
        clipXMax = w4_min(width, HEIGHT - dstY);
        clipYMax = w4_min(height, WIDTH - dstX);
    } else {
        clipXMin = w4_max(0, dstX) - dstX;
        clipYMin = w4_max(0, dstY) - dstY;
        clipXMax = w4_min(width, WIDTH - dstX);
        clipYMax = w4_min(height, HEIGHT - dstY);
    }

    // Iterate pixels in rectangle
    for (int y = clipYMin; y < clipYMax; y++) {
        for (int x = clipXMin; x < clipXMax; x++) {
            // Calculate sprite target coords
            const int tx = dstX + (rotate ? y : x);
            const int ty = dstY + (rotate ? x : y);

            // Calculate sprite source coords
            const int sx = srcX + (flipX ? width - x - 1 : x);
            const int sy = srcY + (flipY ? height - y - 1 : y);

            // Sample the sprite to get a color index
            int colorIdx;
            int bitIndex = sy * srcStride + sx;
            if (bpp2) {
                uint8_t byte = sprite[bitIndex >> 2];
                int shift = 6 - ((bitIndex & 0x03) << 1);
                colorIdx = (byte >> shift) & 0x3;

            } else {
                uint8_t byte = sprite[bitIndex >> 3];
                int shift = 7 - (bitIndex & 0x07);
                colorIdx = (byte >> shift) & 0x1;
            }

            // Get the final color using the drawColors indirection
            uint8_t dc = (colors >> (colorIdx << 2)) & 0x0f;
            if (dc != 0) {
                drawPoint((dc - 1) & 0x03, tx, ty);
            }
        }
    }
}
