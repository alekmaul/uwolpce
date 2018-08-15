//#include <huc.h>
#include "uwol_lvl.h"
#include "main.h"
#include "uwol_snd.h"

const unsigned char level_number[]= {
1,
1,2,
1,2,3,
1,2,3,4,
1,2,3,4,5,
1,2,3,4,5,6,
1,2,3,4,5,6,7,
1,2,3,4,5,6,7,8,
1,2,3,4,5,6,7,8,9,
1,2,3,4,5,6,7,8,9,10
};


// Structure for different screens, 
// 1 + 20 + 6 + 10 = 37 bytes per screen.
	
//FASE fases[45];

// Defining game screens according to this documentation:
// B:  L:   C:
// 0   1    Descriptor Byte 0 1 phase: TTPPPIII 
//          TT: 2 bits, background tile number (0 to 3) 
//          PPP: 3 bits, color PAPER (0-8) 
//          III: 3 bits, color of INK (0-8)
// 
// 1   2    Descriptor de objeto, 2 bytes: XXXXYYYYLLLLTTTD 
//          XXXX: 4 bits, coordenada X en tiles de la esquina sup.izq (0 a 15) 
//          YYYY: 4 bits, coordenada Y en tiles de la esquina sup.izq (0 a 15) 
//          LLLL: 4 bits, longitud del objeto en tiles (0 a 15) 
//          TTT: 3 bits, n�mero de objeto (0 a 7) 
//          D: 1 bit, direcci�n: 0 horz, 1 vert. 
// 
//          P.ej. una plataforma de 6 tiles de ancho en (2, 2) del tipo 2, ser�a: 
//          0010001001100100 
//          [ 2][ 2][ 6][2]0 
// 
// ... x 10 
// 21   2   Descriptor de enemigo, 2 bytes: AAAABBBBYYYYRTTV 
//          AAAA: 4 bits, n� tile horizontal l�mite izquierdo (0 a 15) 
//          BBBB: 4 bits, n� tile horizontal l�mite derecho (0 a 15) 
//          YYYY: 4 bits, n� tile vertical (0 a 15) 
//          R: 1 bit, reservado (por ahora no sirve pa n�).
//          TT: 2 bits, tipo de enemigo (0 a 3); 1 = franky, 2 = vampy, 3 = fanty.
//          V: 1 bit, velocidad: 0 lento (de 1 en 1 pixels), 1 r�pido (de 2 en 2 p�xels). 
// ... x 3 
// 27   1   Descriptor de moneda, 1 byte: XXXXYYYY 
//          XXXX: 4 bits, coordenada X en tiles (0 a 15) 
//          YYYY: 4 bits, coordenada Y en tiles (0 a 15) 
// ... x 10
// 

const unsigned char fases_data[1665] = {
	0x0D, 
	0xC2, 0x00, 0x30, 0x13, 0x30, 0x83, 0x20, 0x55, 0x30, 0x17, 0x30,	0x87, 0xCC, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x15, 0x1A, 0x63,	0x1A, 0x00, 0x00, 
	0x12, 0x32, 0x92, 0xA2, 0x54, 0x64, 0x16, 0x36, 0x86,	0xA6, 
	
	0x04, 0xC4, 0x00, 0x35, 0x01, 0x17, 0xA2, 0x17, 0x93, 0x36, 0x25,
	0x36, 0x85, 0x35, 0xB6, 0x36, 0x57, 0xC4, 0x09, 0x00, 0x00, 0x22, 0x69,
	0x50, 0x57, 0x00, 0x00, 0x92, 0x24, 0x34, 0x44, 0x84, 0xB5, 0x56, 0x66,
	0x76, 0x18, 0x0F, 0xC2, 0x00, 0xBC, 0x13, 0x1D, 0x05, 0x9C, 0x06, 0x1D,
	0xB7, 0xC0, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25,
	0x0B, 0x52, 0x1B, 0x84, 0x2A, 0x11, 0x51, 0x91, 0x14, 0x54, 0x94, 0xB6,
	0x17, 0x57, 0x97, 0x03, 0x90, 0x00, 0x35, 0xB0, 0x55, 0x01, 0x90, 0x33,
	0x25, 0xB4, 0x19, 0x15, 0x70, 0x26, 0x18, 0x98, 0xC0, 0x09, 0x00, 0x00,
	0x22, 0x1A, 0x43, 0x1A, 0x70, 0x2A, 0x21, 0x41, 0x61, 0x81, 0xA1, 0x14,
	0x34, 0x54, 0x74, 0x94, 0x17, 0xCC, 0x00, 0x28, 0x03, 0x28, 0x53, 0x28,
	0xA3, 0x19, 0x44, 0x32, 0x06, 0x22, 0x56, 0x32, 0x96, 0x1B, 0x78, 0xCA,
	0x09, 0x22, 0x05, 0x23, 0x6B, 0x53, 0x0B, 0x02, 0x32, 0x52, 0x62, 0x82,
	0xB2, 0x05, 0x25, 0x95, 0xB5, 0x06, 0xCA, 0x00, 0x67, 0xB1, 0x17, 0x42,
	0x57, 0x13, 0x17, 0x63, 0x26, 0x65, 0x17, 0x86, 0x36, 0x27, 0x36, 0x78,
	0xCA, 0x09, 0x13, 0x0A, 0x44, 0x25, 0x00, 0x00, 0x02, 0x62, 0x82, 0x03,
	0x04, 0x05, 0x06, 0x26, 0x36, 0x46, 0x16, 0xCC, 0x00, 0x75, 0x01, 0x75,
	0xB1, 0x19, 0x53, 0x19, 0x94, 0x19, 0x65, 0x19, 0x46, 0x19, 0x27, 0x19,
	0x87, 0xC0, 0x09, 0x13, 0x1A, 0x44, 0x18, 0x71, 0x37, 0x42, 0x62, 0x92,
	0x43, 0x93, 0x25, 0x45, 0x85, 0x26, 0x86, 0x03, 0x57, 0x61, 0x77, 0xA1,
	0x17, 0xB1, 0x17, 0x22, 0x57, 0x82, 0x36, 0x03, 0x27, 0x43, 0x17, 0xB5,
	0x52, 0x17, 0xC0, 0x09, 0x05, 0x0B, 0x50, 0x05, 0x00, 0x00, 0x52, 0x72,
	0x92, 0x73, 0x93, 0x74, 0x94, 0x00, 0x00, 0x00, 0x02, 0x15, 0x21, 0x34,
	0x51, 0x55, 0x03, 0x92, 0x15, 0x15, 0xB7, 0xC0, 0x09, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x16, 0x35, 0x6B, 0x62, 0x1B, 0x70,
	0x52, 0x62, 0x72, 0x26, 0x36, 0x86, 0x96, 0x00, 0x00, 0x07, 0x6D, 0x32,
	0x3D, 0x82, 0x1D, 0x43, 0x2C, 0xA3, 0x2C, 0x05, 0x5C, 0x45, 0x2D, 0x86,
	0x2C, 0x97, 0xC0, 0x09, 0x00, 0x00, 0x05, 0x0B, 0x21, 0x47, 0x83, 0x1B,
	0x44, 0x54, 0x64, 0x74, 0x57, 0x67, 0x00, 0x00, 0x00, 0x00, 0x4D, 0xCC,
	0x00, 0x19, 0x13, 0x19, 0xA3, 0x48, 0x44, 0x1D, 0x36, 0x1D, 0x86, 0x1D,
	0x17, 0x46, 0x47, 0x1D, 0xA7, 0xCC, 0x09, 0x30, 0x25, 0x51, 0x38, 0x82,
	0x45, 0x12, 0x42, 0x52, 0x62, 0x72, 0xA2, 0x46, 0x56, 0x66, 0x76, 0x47,
	0x3F, 0x71, 0x22, 0xA1, 0x2E, 0x22, 0x32, 0x54, 0x22, 0x16, 0x1F, 0x78,
	0x2E, 0xA8, 0x2E, 0x09, 0x13, 0x49, 0x00, 0x00, 0x04, 0x0B, 0x30, 0x06,
	0x82, 0x26, 0x70, 0xB0, 0x21, 0x31, 0x53, 0x63, 0x34, 0x15, 0x25, 0x77,
	0x4C, 0xC4, 0x00, 0x54, 0x03, 0x34, 0x83, 0x5B, 0xA4, 0x19, 0x65, 0x2A,
	0x37, 0x3A, 0x28, 0x19, 0x88, 0x6A, 0x09, 0x2A, 0xA9, 0x20, 0x39, 0x42,
	0x79, 0x65, 0x49, 0x01, 0x41, 0x81, 0x22, 0xA2, 0x64, 0x95, 0x67, 0x87,
	0xB7, 0x45, 0x72, 0x00, 0x11, 0x82, 0x5A, 0x34, 0x11, 0xA4, 0x11, 0x16,
	0x11, 0x37, 0x3A, 0x09, 0x72, 0x59, 0x00, 0x00, 0x00, 0x00, 0x22, 0x17,
	0x65, 0x3A, 0x80, 0x79, 0x81, 0x33, 0x43, 0x63, 0x73, 0xA3, 0x15, 0x36,
	0x78, 0x88, 0x42, 0x19, 0x22, 0x72, 0x52, 0x19, 0x33, 0x19, 0x44, 0x19,
	0x55, 0x32, 0x66, 0x22, 0xA7, 0x4E, 0x09, 0x6E, 0x69, 0x00, 0x00, 0x15,
	0x5B, 0x55, 0x68, 0x84, 0x36, 0x10, 0x01, 0x21, 0xB1, 0x32, 0x43, 0xB6,
	0x47, 0x57, 0x98, 0x56, 0xA2, 0x21, 0xA2, 0x13, 0xA2, 0x25, 0xA2, 0x17,
	0xC0, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x0A, 0x42, 0x0B, 0x62, 0x0A, 0xA0, 0xB0, 0x11, 0xB2, 0xB3, 0xB4,
	0x15, 0xB6, 0xB7, 0xB8, 0x44, 0x31, 0x63, 0x19, 0x34, 0x19, 0x94, 0x19,
	0x45, 0x19, 0x85, 0x30, 0x17, 0x48, 0x47, 0x30, 0x87, 0x5C, 0x09, 0x5C,
	0x79, 0x22, 0x79, 0x32, 0x35, 0x63, 0x1A, 0x61, 0x62, 0x44, 0x84, 0x16,
	0x46, 0x86, 0xA6, 0x58, 0x68, 0x46, 0x2C, 0x01, 0x48, 0x82, 0x38, 0x33,
	0x19, 0x04, 0x84, 0x25, 0xB6, 0x17, 0xCE, 0x09, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x15, 0x8B, 0x20, 0x35, 0x42, 0x28, 0x00, 0x10, 0xB1, 0x32,
	0x03, 0x84, 0x94, 0xA5, 0x06, 0xB6, 0x4B, 0x19, 0x91, 0x19, 0x82, 0x19,
	0xA2, 0x19, 0x73, 0x19, 0xB3, 0x19, 0x64, 0x19, 0x55, 0x19, 0x46, 0x19,
	0x37, 0xC4, 0x09, 0x02, 0x7B, 0x22, 0x07, 0x82, 0x2B, 0x90, 0x81, 0xA1,
	0x02, 0x72, 0xB2, 0x63, 0x54, 0x45, 0x36, 0x41, 0x40, 0x01, 0x13, 0xB1,
	0x19, 0x43, 0x6A, 0x53, 0x19, 0x34, 0x19, 0x15, 0x19, 0x26, 0x19, 0x37,
	0xA0, 0x09, 0x00, 0x00, 0x03, 0x0B, 0x21, 0x4A, 0x83, 0x35, 0x00, 0xB0,
	0x42, 0xA2, 0x33, 0x14, 0x25, 0x36, 0xA8, 0x00, 0x45, 0x1B, 0x22, 0x1B,
	0x92, 0x2A, 0x53, 0x3A, 0x05, 0x3A, 0x95, 0x4E, 0x47, 0x3E, 0x09, 0x3E,
	0x99, 0x00, 0x00, 0x00, 0x00, 0x11, 0x0B, 0x64, 0x14, 0x62, 0x7A, 0x21,
	0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x5D, 0x21,
	0x6C, 0x31, 0x1D, 0x02, 0x1D, 0xB2, 0x2C, 0x84, 0x2C, 0x35, 0x11, 0x76,
	0x4C, 0x58, 0x2E, 0x09, 0x2E, 0xA9, 0x01, 0x28, 0x22, 0x6A, 0x00, 0x00,
	0x60, 0x01, 0x72, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x00,
	0x81, 0x2E, 0x82, 0x3E, 0x73, 0x4E, 0x64, 0x5E, 0x55, 0x6E, 0x46, 0x7E,
	0x37, 0x8E, 0x28, 0xCE, 0x09, 0x00, 0x00, 0x11, 0x07, 0x34, 0x05, 0x52,
	0x03, 0x81, 0x63, 0x45, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85,
	0xC2, 0x00, 0x11, 0x01, 0x11, 0xB1, 0x80, 0x22, 0x42, 0x14, 0x42, 0x74,
	0x80, 0x26, 0x11, 0xB8, 0xC2, 0x09, 0x00, 0x00, 0x31, 0x1A, 0x54, 0x1A,
	0x73, 0x1A, 0x31, 0x81, 0x23, 0x43, 0x73, 0x93, 0x35, 0x85, 0x07, 0xB7,
	0x96, 0x1B, 0x21, 0x1B, 0x91, 0x1B, 0x42, 0x1B, 0x72, 0x2A, 0x54, 0x1B,
	0x46, 0x1B, 0x76, 0x1B, 0x27, 0x1B, 0x97, 0xC2, 0x09, 0x03, 0x0B, 0x34,
	0x29, 0x51, 0x0B, 0x20, 0x90, 0x41, 0x71, 0x53, 0x63, 0x45, 0x75, 0x26,
	0x96, 0x8F, 0xCE, 0x00, 0xC0, 0x01, 0xAA, 0x14, 0x50, 0x06, 0x50, 0x57,
	0xC0, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x0B,
	0x32, 0x0B, 0x51, 0x0B, 0x13, 0x33, 0x53, 0x63, 0x83, 0xA3, 0x68, 0x00,
	0x00, 0x00, 0x97, 0x87, 0x00, 0x2C, 0x11, 0x6C, 0x41, 0x97, 0xB1, 0x2E,
	0x23, 0x2E, 0x93, 0x2E, 0x15, 0x2E, 0x85, 0x2E, 0x27, 0xB2, 0x09, 0x21,
	0x3A, 0x45, 0x18, 0x63, 0x3A, 0x32, 0x14, 0x94, 0x36, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x8D, 0x84, 0x12, 0x26, 0x74, 0x26, 0xA4, 0x15, 0x96,
	0x19, 0x87, 0x19, 0xA7, 0x15, 0x68, 0x15, 0x78, 0x28, 0x09, 0x28, 0xA9,
	0x15, 0x1A, 0x54, 0xBB, 0x64, 0x68, 0x21, 0x41, 0x61, 0x81, 0x73, 0x93,
	0xB3, 0x95, 0x67, 0xB7, 0x8F, 0x19, 0xA5, 0x19, 0x57, 0x4E, 0x67, 0x19,
	0xA7, 0x19, 0x28, 0x19, 0x48, 0x1B, 0x09, 0x1B, 0x19, 0x19, 0x39, 0x2A,
	0xA9, 0x32, 0xAB, 0x60, 0x5A, 0x74, 0x02, 0x84, 0xA4, 0x56, 0x76, 0x86,
	0xA6, 0x27, 0x47, 0x38, 0xA8, 0x84, 0x19, 0x11, 0x19, 0xA2, 0x19, 0x03,
	0x19, 0x94, 0x19, 0x15, 0x19, 0xA6, 0x19, 0x07, 0x32, 0x09, 0x13, 0x59,
	0x32, 0x99, 0x02, 0x02, 0x32, 0x7B, 0x85, 0x1B, 0x10, 0xA1, 0x02, 0x93,
	0x14, 0xA5, 0x06, 0x58, 0x98, 0xB8, 0x86, 0x19, 0x04, 0x19, 0xB4, 0x19,
	0x45, 0x19, 0x27, 0x1D, 0x47, 0x19, 0x67, 0x28, 0x09, 0x1D, 0x39, 0x1D,
	0x59, 0x19, 0x79, 0x34, 0x04, 0x50, 0x23, 0x50, 0x57, 0x03, 0xB3, 0x44,
	0x26, 0x46, 0x66, 0x18, 0x38, 0x58, 0x78, 0x87, 0xCE, 0x00, 0x2B, 0x64,
	0x1B, 0x68, 0x13, 0x78, 0x13, 0x88, 0x2E, 0xA8, 0x28, 0x09, 0x1F, 0x49,
	0x1F, 0x59, 0x1B, 0x69, 0x72, 0x04, 0x70, 0x7A, 0x00, 0x00, 0x85, 0x17,
	0x57, 0x67, 0x77, 0xA7, 0x28, 0x48, 0x58, 0x00, 0x86, 0x30, 0x41, 0x71,
	0x81, 0x61, 0xA1, 0x51, 0x12, 0x11, 0x62, 0x11, 0x23, 0x20, 0x25, 0x50,
	0x07, 0x20, 0xA7, 0xCE, 0x09, 0x02, 0x05, 0x00, 0x6B, 0x64, 0x47, 0x93,
	0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8E, 0x82, 0x22,
	0x73, 0xB2, 0x22, 0x13, 0x7A, 0x44, 0x22, 0x15, 0x72, 0x26, 0x22, 0x17,
	0x12, 0x97, 0x5E, 0x09, 0x42, 0x89, 0x15, 0x6B, 0x42, 0x03, 0x74, 0x37,
	0x73, 0x93, 0x55, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x24,
	0x91, 0x3E, 0x23, 0x30, 0x93, 0x11, 0x74, 0x40, 0x56, 0x15, 0x17, 0x6E,
	0x09, 0x30, 0x89, 0x00, 0x00, 0x00, 0x00, 0x12, 0x28, 0x53, 0x0B, 0x82,
	0x15, 0xA0, 0x73, 0x15, 0x75, 0x77, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x86,
	0xA6, 0x00, 0x26, 0x91, 0x3E, 0x33, 0x3A, 0x93, 0x3A, 0x24, 0x4A, 0x16,
	0x2E, 0x86, 0x2A, 0x97, 0x6E, 0x09, 0x3A, 0x99, 0x14, 0x04, 0x74, 0x04,
	0x82, 0x18, 0xA0, 0x15, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC3, 0xCE, 0x00, 0x20, 0x52, 0x40, 0x14, 0x40, 0x74, 0x1D, 0x05, 0x1D,
	0x07, 0x40, 0x47, 0x1D, 0xB7, 0xCE, 0x09, 0x00, 0x00, 0x10, 0x0B, 0x33,
	0x17, 0x85, 0x28, 0x51, 0x61, 0x33, 0x83, 0x06, 0x56, 0x66, 0xB6, 0x48,
	0x78, 0xC5, 0x6C, 0x31, 0x1D, 0x22, 0x1D, 0x92, 0x3D, 0x13, 0x15, 0x54,
	0x1D, 0x26, 0x1D, 0x96, 0x6C, 0x37, 0x1D, 0xB8, 0xC4, 0x09, 0x30, 0x57,
	0x50, 0x69, 0x60, 0x38, 0x32, 0x23, 0x33, 0x93, 0x04, 0x16, 0xA6, 0x27,
	0x97, 0x00, 0xD6, 0x1D, 0x00, 0xBC, 0x10, 0x3A, 0x04, 0x2C, 0x54, 0x3A,
	0x94, 0x1D, 0x45, 0x1D, 0x75, 0x1D, 0x37, 0x1D, 0x87, 0xCC, 0x09, 0x33,
	0x1A, 0x61, 0x38, 0x00, 0x00, 0x21, 0x41, 0x71, 0x91, 0xB3, 0x55, 0x65,
	0x46, 0x76, 0x00, 0xCC, 0xC2, 0x00, 0x58, 0x04, 0x58, 0x74, 0x19, 0x66,
	0x19, 0x48, 0x13, 0x58, 0x19, 0x68, 0x19, 0x78, 0xC8, 0x09, 0x00, 0x00,
	0x34, 0x1A, 0x61, 0x15, 0x72, 0x0B, 0x11, 0x21, 0x51, 0xA1, 0x15, 0x25,
	0x35, 0x85, 0x95, 0xA5, 0xCF, 0xC0, 0x00, 0x21, 0x04, 0x21, 0x44, 0x31,
	0xB4, 0x40, 0x06, 0x10, 0x96, 0x54, 0x77, 0xC0, 0x09, 0x00, 0x00, 0x00,
	0x00, 0x21, 0x1A, 0x52, 0x58, 0x00, 0x00, 0x11, 0x51, 0x61, 0xA1, 0x15,
	0x25, 0x35, 0xA5, 0x00, 0x00, 0xCE, 0x13, 0x14, 0x11, 0x64, 0x11, 0x55,
	0x11, 0x75, 0x11, 0xA6, 0x11, 0x47, 0x11, 0x97, 0x1D, 0x38, 0x11, 0x68,
	0x2E, 0x09, 0x31, 0x0B, 0x65, 0x09, 0x72, 0x58, 0x82, 0x13, 0x54, 0x74,
	0xB4, 0x35, 0x95, 0x00, 0x00, 0x00, 0xCD, 0x17, 0x31, 0x17, 0x81, 0x2C,
	0x03, 0x17, 0x13, 0x17, 0xA3, 0x17, 0x35, 0x17, 0x85, 0x17, 0x07, 0x27,
	0xB7, 0x3C, 0x09, 0x01, 0x0B, 0x45, 0x0B, 0x61, 0x0B, 0x30, 0x80, 0x02,
	0x62, 0xB2, 0x34, 0x84, 0x06, 0xB6, 0x00, 0xD6, 0x32, 0x91, 0x1D, 0x42,
	0x1D, 0x72, 0x1D, 0x03, 0x1D, 0xB3, 0x1D, 0x15, 0x1D, 0xA5, 0x20, 0x57,
	0x22, 0x09, 0x32, 0x99, 0x33, 0x1A, 0x53, 0x29, 0x83, 0x2C, 0x00, 0x20,
	0x90, 0xB0, 0x33, 0x83, 0x36, 0x86, 0x18, 0xB8, 0xC6, 0x17, 0x21, 0x57,
	0x41, 0x57, 0x61, 0x17, 0x81, 0x17, 0xB3, 0x17, 0x95, 0x17, 0x87, 0x17,
	0x58, 0x26, 0x09, 0x36, 0x99, 0x02, 0x1A, 0x20, 0x7B, 0x85, 0x24, 0x51,
	0x52, 0x72, 0xB2, 0x53, 0x93, 0x54, 0x55, 0x56, 0x86
};

const unsigned int enem_frames [8] = {
    wolfi_1a,       wolfi_1a,
    franki_1a,      franki_1a,
    vampi_1a,       vampi_1a,
    fanti_l_1a,     fanti_r_1a
};

typedef struct {
	unsigned char descriptor;
	unsigned int obj[10];
	unsigned int movil[3];
	unsigned char coin[10];
} FASE;

#define BASETILE 0x100

//---------------------------------------------------------------------------------
void sp_TileSet(unsigned char col,unsigned char row,unsigned int tile) {
	unsigned int tilep;
	
	tilep=tile+BASETILE;
  if(tile>79) tilep=BASETILE+tile|(3<<12); 
	else if(tile>47) tilep=BASETILE+tile|(2<<12); 
	else if(tile>31) tilep=BASETILE+tile|(1<<12); 
	put_raw(tilep,col,row);
}

//---------------------------------------------------------------------------------
/*
void sp_TileGet(unsigned char col,unsigned char row) {
	unsigned int ptr;
	
	ptr=(row<<5)+col;
	fdcami[0]=mapValue[ptr++];
	fdcami[1]=mapValue[ptr++];
	ptr+=30;
	fdcami[2]=mapValue[ptr++];
	fdcami[3]=mapValue[ptr];
}
*/

//---------------------------------------------------------------------------------
void draw_lives (void) {
	sp_TileSet (6, 1, 98);
	sp_TileSet (7, 1, 88 + (lives / 10));
	sp_TileSet (8, 1, 88 + (lives % 10));
}

//---------------------------------------------------------------------------------
void draw_score (void) {
	sp_TileSet (24, 1, 98);
	sp_TileSet (25, 1, 88 + (total_coins / 100));
	sp_TileSet (26, 1, 88 + (total_coins % 100) / 10);
	sp_TileSet (27, 1, 88 + (total_coins % 10));
}

//---------------------------------------------------------------------------------
void draw_total_score (unsigned char x,unsigned char y) {
	sp_TileSet (x+0, y, 105);
	sp_TileSet (x+1, y, 106);
	sp_TileSet (x+2, y, 107);
	sp_TileSet (x+3, y, 108);
	sp_TileSet (x+4, y, 102);
	sp_TileSet (x+5, y, 98);
	sp_TileSet (x+6, y, 88 + total_score / 10000);
	sp_TileSet (x+7, y, 88 + (total_score % 10000) / 1000);
	sp_TileSet (x+8, y, 88 + (total_score % 1000) / 100);
	sp_TileSet (x+9, y, 88 + (total_score % 100) / 10);
	sp_TileSet (x+10, y, 88 + (total_score % 10));
}

//---------------------------------------------------------------------------------
void draw_time (void) {
	sp_TileSet (12, 1, 99);
	sp_TileSet (13, 1, 100);
	sp_TileSet (14, 1, 101);
	sp_TileSet (15, 1, 102);
	sp_TileSet (16, 1, 98);
	sp_TileSet (17, 1, 88 + (timee / 100));
	sp_TileSet (18, 1, 88 + (timee % 100) / 10);
	sp_TileSet (19, 1, 88 + (timee % 10));
}

// draw level number
//---------------------------------------------------------------------------------
void draw_level (unsigned char x, unsigned char y) {
	sp_TileSet (x+0,y, 103);
	sp_TileSet (x+1,y, 102);
	sp_TileSet (x+2,y, 104);
	sp_TileSet (x+3,y, 102);
	sp_TileSet (x+4,y, 103);
	if(level>=10) {
		sp_TileSet (x+5,y, 89);
		x++;
	}
	sp_TileSet (x+5,y, 88 + (level % 10));
	sp_TileSet (x+6,y, 83);
	if(level_number[n_pant]>=10) {
		sp_TileSet (x+7, y, 89);
		x++;
	}
	sp_TileSet (x+7, y, 88 + (level_number[n_pant] % 10));
}

//---------------------------------------------------------------------------------
// Display pyramid with current progress
void pyramid(void) {
	unsigned char i,j,x,y,l;
	unsigned int c;
	
	// Disable display
	disp_off();
	
	// Init sprites
	load_vram( SPRVRAM , spritestil , 0x40*29);
	load_vram( SPRVRAM + 0x40*32 , borderstil , 0x200); //0x40*29 = 0x740
	set_sprpal(0,spritespal);

	//ststop(); stinit(MAPVGM,0);		// play first track in the song file
	//stop_music();	play_music(map_uwol);
	play_snd(MUSMAP);
	for (i=0;i<32;i++)
		for (j=0;j<32;j++)
				put_raw (0x128, 	j,  i);

	draw_level(level<10 ? 12:11, 2);

	// Pyramid
	j = 16;
	y = 5;
	x = 0;
  l = 0;
  sx=0;
  sy=0;

	for (i = 0; i < 55; i ++) {
		if (i == ( (l*(l + 1))>>1) ) {
			j--;
			x = j;
			y++;
			l++;
		}
 
		if (n_pant == i) {
				c = 120+6;
				sx=x;
				sy=y;
		}
		else {
			if (visited [i])
				c = 120+6;
			else
				c = 120;
	 }
 
		sp_TileSet ( x,  y, c);
		sp_TileSet (x+1,y,	c+1);
		sp_TileSet (x+2,y,	82);
		sp_TileSet (x, 	y+1,80);
		sp_TileSet (x+1,y+1,80);
		sp_TileSet (x+2,y+1,81);
 
		x += 2;
	}

  // Info stats score
	draw_total_score(10,19);

  // Info stats
	sp_TileSet (4, 19, 34);sp_TileSet (5, 19, 35);sp_TileSet (4, 18, 32);sp_TileSet (5, 18, 33);
	sp_TileSet (6, 19, 98);
	sp_TileSet (7, 19, 88 + (lives / 10));
	sp_TileSet (8, 19, 88 + (lives % 10));
 
	sp_TileSet (22, 19, 38);sp_TileSet (23, 19, 39);sp_TileSet (22, 18, 36);sp_TileSet (23, 18, 37);
	sp_TileSet (24, 19, 98);
	sp_TileSet (25, 19, 88 + (total_coins / 100));
	sp_TileSet (26, 19, 88 + (total_coins % 100) / 10);
	sp_TileSet (27, 19, 88 + (total_coins % 10));

  // We expect you to press any key control
	for (j=0;j<(1+10+3+2+12+1); j++) {
		spr_set(j);spr_hide();
	}

	j=0;
	spr_set( 0 ); spr_pattern( SPRVRAM+arrow_1a); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP );
	spr_x(sx<<3);	spr_pal( 0 ); spr_pri( 1 );

	updatePalBG();
	
 	disp_on();

	while(1) {
		spr_y( ( (sy-2)<<3)+4+(j>>2)); 

		j=(j+1)%12;
		joyfunc();

		satb_update();
		vsync(1);
		
		if ( (tchpad & (sp_FIRE|sp_START)) != 0)
			break;
	}
	
	// remove sprite
	spr_hide();
	satb_update();

	play_snd(SNDJUMP);
	fade_palette_out( BG_PAL0, 4, SPR_PAL0, 2 , 2 );
}

void draw_minitile(unsigned char x, unsigned char y, unsigned char tile,unsigned char attr) {
	unsigned char is;
	
	if ( (x>3) && (x<28) && (y>1) && (y<22)) {
		//mapValue[x+(y<<5)]=tile;
		sp_TileSet (x, y, tile);
		is=sp_attr_buf[((x-2)>>1)+(y-2)*14]; 
		if (x & 1) {
			is = (is & 0xF0) | attr;
		}
		else {
			is = (is & 0x0F) | (attr<<4);
		}
		x=(x-2)>>1;
		sp_attr_buf[x+(y-2)*14]=is;			// each level (yes, huge size for coleco :/)
	}
}

void draw_tile (unsigned char x, unsigned char y, unsigned char tile, unsigned char off, unsigned char attr) {
    draw_minitile(    off + x,     off + y,     tile,attr);
    draw_minitile((off^1) + x,     off + y,  tile + 1,attr);
    draw_minitile(    off + x, (off^1) + y, tile + 2,attr);
    draw_minitile((off^1) + x, (off^1) + y, tile + 3,attr);
}

void draw_screen(void) {
	unsigned char is,ti,j, r_pant;
	unsigned int x, y, c,l, valobj;
	unsigned char *plevel;
	
	// clear screen
	disp_off();

	// Init level
	memset(sp_attr_buf,0x00,sizeof(sp_attr_buf));
	for (is=0;is<14*2;is++) { sp_attr_buf[is+14*18]=0x11; }
	
	// Calculate the tile background as height
	if (level < 4) {
		ti = 0;
	}
  else if (level < 7) {
    ti = 4;
	}
  else if (level < 9) {
    ti = 8;
	}
  else
    ti = 12;
 
	//for (x=0;x<(20*24);x++)
	//	mapValue[x]=0;

	// Background First
  for (y = 0; y < 10; y++) {
		for (x = 0; x < 12; x++) {
			draw_tile (4 + (x<<1), 2 + (y<<1), ti,0,0);
		}
	}
		
	// Yes folks, we are about trampucheros. No 55 screens. there are 45 
	// And repeat 10.
	r_pant=n_pant % 45;
    
	// Objects
	plevel=fases_data+r_pant*37+1;
	for (is = 0; is < 10; is ++) {
		valobj=(*(plevel+1)<<8) | *(plevel);		plevel+=2;
		if (valobj != 0) {
			x = (valobj >> 12) & 0x000F;
			y = (valobj >> 8) & 0x000F;
			l = (valobj >> 4) & 0x000F;
			c = (valobj >> 1) & 0x0007;
			if ( (valobj & 1) == 0 ) {
				// horizontal platform
				 for (j = x; j < x + l; j ++) {
					draw_tile (5 + (j<<1), 3 + (y<<1), 16 + ti,1,0);
				}
				for (j = x; j < x + l; j ++) {
					draw_tile (4 + (j<<1), 2 + (y<<1), 48 + (c<<2),0,1);
				}
			} 
			else {
				// vertical platform
				for (j = y; j < y + l; j ++) {
					draw_tile (5 + (x<<1), 3 + (j<<1), 16 + ti,1,0);
				}
				for (j = y; j < y + l; j ++) {
					draw_tile (4 + (x<<1), 2 + (j<<1), 48+ (c<<2),0,1);
				}
			}
		}
  }
	
  // Money : only if the room has not been visited
	plevel=fases_data+r_pant*37+1+10*2+3*2;
  if (visited[n_pant] == 0) {
		num_monedas = 0;
 
		for ( is = 0; is < 10; is ++) {
			// First we copy the values ??in our structure
			ti=*(plevel); plevel++;
			monedas[is].type = ti ? 1:0;
 
			if (ti!= 0) {
				// And now we draw
				x = (ti >> 4) ;
				y = (ti & 15) ;
				monedas[is].x=(4+(x<<1))<<3;
				monedas[is].y=(2+(y<<1))<<3;
 
				// save bg for 1st tile
				x=(4+(x<<1)); y=(2+(y<<1));
				// draw coin
				spr_set( 12+4+is ); spr_pattern( SPRVRAM+coin_1a); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP );
				spr_x(x<<3);	spr_y(y<<3); spr_pal( 0 ); spr_pri( 1 );
        num_monedas ++;
			}
    }
	}

	// Now loaded the moving objects
	plevel=fases_data+r_pant*37+1+10*2;
	for (is = 0; is < 3; is ++) {
		pMovil=&moviles[is];
		valobj=(*(plevel+1)<<8) | *(plevel);		plevel+=2;
		if (valobj == 0)      // Enumi desactivated
			pMovil->tipo = 0xff;//-1;
		else {
			l = ((valobj & 1)>>1)-((valobj & 1)>>2); // 180801 JGD
			c = (valobj >> 1) & 7;
			y = (valobj >> 4) & 15;
			sx =(valobj >> 12) & 15;
			sy =(valobj >> 8) & 15;
 
			pMovil->x = 32 + (sx << 4);
			pMovil->t1 = 32 + (sx << 4);
			pMovil->y = 16 + (y << 4);
			pMovil->t2 = 32 + (sy << 4);
			pMovil->vx = l*SPTWM + 1;
			pMovil->tipo = c;

			if (c != 3)
				moviles [is].next_frame = enem_frames [ 1 + (c << 1) ];
			moviles[is].current_frame=moviles[is].next_frame;
			moviles[is].next_frame+=64;
			
			spr_set( 12+1+is ); spr_pattern( SPRVRAM+moviles[is].current_frame); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP );
			spr_x(pMovil->x);spr_y(pMovil->y);spr_pal( 0 ); spr_pri( 1 );
		}
	}
	// because of fadeout
	updatePalBG();
	
	satb_update();
	disp_on();
}
