/* MTP Layer3 parsing tests */
#include <mtp/mtp_level3.h>

#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

struct mtp_test {
	const uint8_t *input;
	const uint16_t length;
	struct mtp_level_3_hdr hdr;

	int has_mng;
	struct mtp_level_3_mng mng;

	int has_prohib;
	struct mtp_level_3_prohib prohib;
};

static const unsigned char pkt1[] = {
0x81, 0x88, 0xc0, 0x16, 0x00, 0x11, 0xe0, 0x62,
0x61, 0x72, 0x62, 0x61, 0x6e, 0x6f, 0x62, 0x61,
0x72, 0x6e, 0x61, 0x62, 0x6f };

static const unsigned char pkt3[] = {
0x81, 0x88, 0xc0, 0x16, 0x00, 0x21, 0xe0, 0x41,
0x6d, 0x69, 0x74, 0x20, 0x43, 0x68, 0x61, 0x6e,
0x64, 0x72, 0x61, 0x00, 0x00 };

static const unsigned char pkt7[] = {
0x80, 0x88, 0xc0, 0x16, 0x00, 0x14, 0x56, 0x00 };

#if 0
static const unsigned char pkt2[] = {
0x81, 0x5b, 0x00, 0x22, 0x00, 0x11, 0xe0, 0x41,
0x6d, 0x69, 0x74, 0x20, 0x43, 0x68, 0x61, 0x6e,
0x64, 0x72, 0x61, 0x00, 0x00 };

static const unsigned char pkt4[] = {
0x81, 0x5b, 0x00, 0x22, 0x00, 0x21, 0xe0, 0x62,
0x61, 0x72, 0x62, 0x61, 0x6e, 0x6f, 0x62, 0x61,
0x72, 0x6e, 0x61, 0x62, 0x6f };

static const unsigned char pkt5[] = {
0x81, 0x88, 0xc0, 0x16, 0x00, 0x21, 0xe0, 0x62,
0x61, 0x72, 0x62, 0x61, 0x6e, 0x6f, 0x62, 0x61,
0x72, 0x6e, 0x61, 0x62, 0x6f };

static const unsigned char pkt6[] = {
0x80, 0x5b, 0x00, 0x22, 0x00, 0x17 };

static const unsigned char pkt8[] = {
0x80, 0x88, 0xc0, 0x16, 0x00, 0x14, 0x00, 0x00 };

static const unsigned char pkt9[] = {
0x80, 0x88, 0xc0, 0x16, 0x00, 0x17 };

static const unsigned char pkt10[] = {
0x83, 0x5b, 0x00, 0x22, 0x20, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0x01, 0x04,
0xc3, 0x88, 0x00, 0x01, 0x05, 0x03, 0xfe, 0x5b,
0x00, 0x01 };

static const unsigned char pkt11[] = {
0x83, 0x88, 0xc0, 0x16, 0xd0, 0x09, 0x00, 0x03,
0x05, 0x07, 0x02, 0x42, 0x01, 0x02, 0x42, 0x01,
0x05, 0x01, 0xfe, 0x5b, 0x00, 0x00 };

static const unsigned char pkt12[] = {
0x83, 0x5b, 0x00, 0x22, 0x30, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x30,
0x04, 0x01, 0x20 };

static const unsigned char pkt13[] = {
0x83, 0x5b, 0x00, 0x22, 0x40, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x0f, 0x04, 0x01, 0x07 };

static const unsigned char pkt14[] = {
0x83, 0x5b, 0x00, 0x22, 0x50, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x10, 0x04, 0x01, 0x07 };

static const unsigned char pkt15[] = {
0x83, 0x5b, 0x00, 0x22, 0x60, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x11, 0x04, 0x01, 0x07 };

static const unsigned char pkt16[] = {
0x83, 0x5b, 0x00, 0x22, 0x70, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x12, 0x04, 0x01, 0x07 };

static const unsigned char pkt17[] = {
0x83, 0x5b, 0x00, 0x22, 0x80, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x13, 0x04, 0x01, 0x07 };

static const unsigned char pkt18[] = {
0x83, 0x5b, 0x00, 0x22, 0x90, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x14, 0x04, 0x01, 0x07 };

static const unsigned char pkt19[] = {
0x83, 0x5b, 0x00, 0x22, 0xa0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x15, 0x04, 0x01, 0x07 };

static const unsigned char pkt20[] = {
0x83, 0x5b, 0x00, 0x22, 0xb0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x16, 0x04, 0x01, 0x07 };

static const unsigned char pkt21[] = {
0x83, 0x5b, 0x00, 0x22, 0xc0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x17, 0x04, 0x01, 0x07 };

static const unsigned char pkt22[] = {
0x83, 0x5b, 0x00, 0x22, 0xd0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x18, 0x04, 0x01, 0x07 };

static const unsigned char pkt23[] = {
0x83, 0x5b, 0x00, 0x22, 0xe0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x19, 0x04, 0x01, 0x07 };

static const unsigned char pkt24[] = {
0x83, 0x5b, 0x00, 0x22, 0xf0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1a, 0x04, 0x01, 0x07 };

static const unsigned char pkt25[] = {
0x83, 0x5b, 0x00, 0x22, 0x00, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1b, 0x04, 0x01, 0x07 };

static const unsigned char pkt26[] = {
0x83, 0x5b, 0x00, 0x22, 0x10, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1c, 0x04, 0x01, 0x07 };

static const unsigned char pkt27[] = {
0x83, 0x5b, 0x00, 0x22, 0x20, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1d, 0x04, 0x01, 0x07 };

static const unsigned char pkt28[] = {
0x83, 0x5b, 0x00, 0x22, 0x30, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1e, 0x04, 0x01, 0x07 };

static const unsigned char pkt29[] = {
0x83, 0x88, 0xc0, 0x16, 0x60, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x0f };

static const unsigned char pkt30[] = {
0x83, 0x88, 0xc0, 0x16, 0x70, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x10 };

static const unsigned char pkt31[] = {
0x83, 0x88, 0xc0, 0x16, 0x80, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x11 };

static const unsigned char pkt32[] = {
0x83, 0x88, 0xc0, 0x16, 0x90, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x12 };

static const unsigned char pkt33[] = {
0x83, 0x88, 0xc0, 0x16, 0xa0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x13 };

static const unsigned char pkt34[] = {
0x83, 0x88, 0xc0, 0x16, 0xb0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x14 };

static const unsigned char pkt35[] = {
0x83, 0x88, 0xc0, 0x16, 0xc0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x15 };

static const unsigned char pkt36[] = {
0x83, 0x88, 0xc0, 0x16, 0xd0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x16 };

static const unsigned char pkt37[] = {
0x83, 0x88, 0xc0, 0x16, 0xe0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x17 };

static const unsigned char pkt38[] = {
0x83, 0x88, 0xc0, 0x16, 0xf0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x18 };

static const unsigned char pkt39[] = {
0x83, 0x88, 0xc0, 0x16, 0x00, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x19 };

static const unsigned char pkt40[] = {
0x83, 0x88, 0xc0, 0x16, 0x10, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1a };

static const unsigned char pkt41[] = {
0x83, 0x88, 0xc0, 0x16, 0x20, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1b };

static const unsigned char pkt42[] = {
0x83, 0x88, 0xc0, 0x16, 0x30, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1c };

static const unsigned char pkt43[] = {
0x83, 0x88, 0xc0, 0x16, 0x40, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1d };

static const unsigned char pkt44[] = {
0x83, 0x88, 0xc0, 0x16, 0x50, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1e };

static const unsigned char pkt45[] = {
0x83, 0x5b, 0x00, 0x22, 0x40, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x0f, 0x04, 0x01, 0x07 };

static const unsigned char pkt46[] = {
0x83, 0x5b, 0x00, 0x22, 0x50, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x10, 0x04, 0x01, 0x07 };

static const unsigned char pkt47[] = {
0x83, 0x5b, 0x00, 0x22, 0x60, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x11, 0x04, 0x01, 0x07 };

static const unsigned char pkt48[] = {
0x83, 0x5b, 0x00, 0x22, 0x70, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x12, 0x04, 0x01, 0x07 };

static const unsigned char pkt49[] = {
0x83, 0x5b, 0x00, 0x22, 0x80, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x13, 0x04, 0x01, 0x07 };

static const unsigned char pkt50[] = {
0x83, 0x5b, 0x00, 0x22, 0x90, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x14, 0x04, 0x01, 0x07 };

static const unsigned char pkt51[] = {
0x83, 0x5b, 0x00, 0x22, 0xa0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x15, 0x04, 0x01, 0x07 };

static const unsigned char pkt52[] = {
0x83, 0x5b, 0x00, 0x22, 0xb0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x16, 0x04, 0x01, 0x07 };

static const unsigned char pkt53[] = {
0x83, 0x5b, 0x00, 0x22, 0xc0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x17, 0x04, 0x01, 0x07 };

static const unsigned char pkt54[] = {
0x83, 0x5b, 0x00, 0x22, 0xd0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x18, 0x04, 0x01, 0x07 };

static const unsigned char pkt55[] = {
0x83, 0x5b, 0x00, 0x22, 0xe0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x19, 0x04, 0x01, 0x07 };

static const unsigned char pkt56[] = {
0x83, 0x5b, 0x00, 0x22, 0xf0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1a, 0x04, 0x01, 0x07 };

static const unsigned char pkt57[] = {
0x83, 0x5b, 0x00, 0x22, 0x00, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1b, 0x04, 0x01, 0x07 };

static const unsigned char pkt58[] = {
0x83, 0x5b, 0x00, 0x22, 0x10, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1c, 0x04, 0x01, 0x07 };

static const unsigned char pkt59[] = {
0x83, 0x5b, 0x00, 0x22, 0x20, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1d, 0x04, 0x01, 0x07 };

static const unsigned char pkt60[] = {
0x83, 0x5b, 0x00, 0x22, 0x30, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0xc3, 0x5b, 0x00, 0xfe, 0x04,
0x43, 0x88, 0x00, 0xfe, 0x09, 0x00, 0x07, 0x40,
0x01, 0x00, 0x1e, 0x04, 0x01, 0x07 };

static const unsigned char pkt61[] = {
0x83, 0x88, 0xc0, 0x16, 0x60, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x0f };

static const unsigned char pkt62[] = {
0x83, 0x88, 0xc0, 0x16, 0x70, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x10 };

static const unsigned char pkt63[] = {
0x83, 0x88, 0xc0, 0x16, 0x80, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x11 };

static const unsigned char pkt64[] = {
0x83, 0x88, 0xc0, 0x16, 0x90, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x12 };

static const unsigned char pkt65[] = {
0x83, 0x88, 0xc0, 0x16, 0xa0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x13 };

static const unsigned char pkt66[] = {
0x83, 0x88, 0xc0, 0x16, 0xb0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x14 };

static const unsigned char pkt67[] = {
0x83, 0x88, 0xc0, 0x16, 0xc0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x15 };

static const unsigned char pkt68[] = {
0x83, 0x88, 0xc0, 0x16, 0xd0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x16 };

static const unsigned char pkt69[] = {
0x83, 0x88, 0xc0, 0x16, 0xe0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x17 };

static const unsigned char pkt70[] = {
0x83, 0x88, 0xc0, 0x16, 0xf0, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x18 };

static const unsigned char pkt71[] = {
0x83, 0x88, 0xc0, 0x16, 0x00, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x19 };

static const unsigned char pkt72[] = {
0x83, 0x88, 0xc0, 0x16, 0x10, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1a };

static const unsigned char pkt73[] = {
0x83, 0x88, 0xc0, 0x16, 0x20, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1b };

static const unsigned char pkt74[] = {
0x83, 0x88, 0xc0, 0x16, 0x30, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1c };

static const unsigned char pkt75[] = {
0x83, 0x88, 0xc0, 0x16, 0x40, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1d };

static const unsigned char pkt76[] = {
0x83, 0x88, 0xc0, 0x16, 0x50, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x06, 0x00, 0x04, 0x48,
0x01, 0x00, 0x1e };

static const unsigned char pkt77[] = {
0x83, 0x88, 0xc0, 0x16, 0x60, 0x09, 0x00, 0x03,
0x07, 0x0b, 0x04, 0x43, 0x88, 0x00, 0xfe, 0x04,
0x43, 0x5b, 0x00, 0xfe, 0x03, 0x00, 0x01, 0x31 };
#endif

static struct mtp_test tests[] = {
	{
	    .input = pkt1,
	    .length = sizeof(pkt1),
	    .hdr = {
		    .ni		= 0x02,
		    .spare	= 0x00,
		    .ser_ind	= 0x01,
	    },
	    .has_mng = 1,
	    .mng = {
		    .cmn = {
			.h0		= 0x01,
			.h1		= 0x01,
		    },
		    .length		= 14,
	    },
	},

	{
	    .input = pkt3,
	    .length = sizeof(pkt3),
	    .hdr = {
		    .ni		= 0x02,
		    .spare	= 0x00,
		    .ser_ind	= 0x01,
	    },
	    .has_mng = 1,
	    .mng = {
		    .cmn = {
			    .h0		= 0x01,
			    .h1		= 0x02,
		    },

		    .length	= 14,
	    },
	},

	{
	    .input = pkt7,
	    .length = sizeof(pkt7),
	    .hdr = {
		    .ni = 2,
		    .spare = 0,
		    .ser_ind = 0,
	    },

	    .has_prohib = 1,
	    .prohib = {
		    .cmn = {
			    .h0 = 0x04,
			    .h1 = 0x1,
		    },

	    },
	}
};

static void check_hdr(const uint8_t *data, const struct mtp_level_3_hdr *t_hdr)
{
	struct mtp_level_3_hdr *hdr;
	hdr = (struct mtp_level_3_hdr *) data;
	if (memcmp(hdr, t_hdr, sizeof(*hdr)) == 0)
		return;

	if (hdr->ni != t_hdr->ni)
		fprintf(stderr, "NI failed.\n");
	if (hdr->spare != t_hdr->spare)
		fprintf(stderr, "spare not equal\n");
	if (hdr->ser_ind != t_hdr->ser_ind)
		fprintf(stderr, "ser_ind not equal\n");
	if (hdr->addr != t_hdr->addr)
		fprintf(stderr, "routing data not equal\n");

	fprintf(stderr, "FAIL: Comparing headers failed.\n");
	abort();
}

static void check_mng(const uint8_t *data, const struct mtp_level_3_mng *t_mng)
{
	struct mtp_level_3_hdr *hdr = (struct mtp_level_3_hdr *) data;
	struct mtp_level_3_mng *mng = (struct mtp_level_3_mng *) &hdr->data[0];

	if (memcmp(mng, t_mng, sizeof(*mng)) == 0)
		return;

	if (mng->cmn.h0 != t_mng->cmn.h0)
		fprintf(stderr, "h0 not equal.\n");
	if (mng->cmn.h1 != t_mng->cmn.h1)
		fprintf(stderr, "h1 not equal.\n");
	if (mng->length != t_mng->length)
		fprintf(stderr, "length not euqal.\n");
	fprintf(stderr, "FAIL: Comparing the mtp_level_3_mng\n");
	abort();
}

static void check_prohib(const uint8_t *data, const struct mtp_level_3_prohib *t_prohib)
{
	struct mtp_level_3_hdr *hdr = (struct mtp_level_3_hdr *) data;
	struct mtp_level_3_prohib *prohib = (struct mtp_level_3_prohib *) &hdr->data[0];

	if (memcmp(prohib, t_prohib, sizeof(*prohib)) == 0)
		return;

	if (prohib->cmn.h0 != t_prohib->cmn.h0)
		fprintf(stderr, "h0 not equal.\n");
	if (prohib->cmn.h1 != t_prohib->cmn.h1)
		fprintf(stderr, "h1 not equal.\n");
	if (ntohs(prohib->apoc) != t_prohib->apoc)
		fprintf(stderr, "apoc not euqal.\n");
	fprintf(stderr, "FAIL: Comparing the mtp_level_3_prohib\n");
	abort();
}

int main(int argc, char **argv)
{
	uint32_t addr;
	int i;

	printf("Basic MTP Structure testing.\n");

        /* set the addresses here due big endian MTP_ADDRESS macro */
	tests[0].hdr.addr = MTP_ADDR(0x00, 136, 91);
	tests[1].hdr.addr = MTP_ADDR(0x00, 136, 91);
	tests[2].hdr.addr = MTP_ADDR(0x00, 136, 91);
	tests[2].prohib.apoc = MTP_MAKE_APOC(86);

	for (i = 0; i < ARRAY_SIZE(tests); ++i) {
		check_hdr(tests[i].input, &tests[i].hdr);
		if (tests[i].has_mng)
			check_mng(tests[i].input, &tests[i].mng);
		if (tests[i].has_prohib)
			check_prohib(tests[i].input, &tests[i].prohib);
	}

	if (MTP_READ_OPC(tests[0].hdr.addr) != 91) {
		fprintf(stderr, "Failed to read OPC address\n");
		abort();
	}

	if (MTP_READ_DPC(tests[1].hdr.addr) != 136) {
		fprintf(stderr, "Failed to read DPC address\n");
		abort();
	}

	/* check the SCCP unitdata */
	{
		struct sccp_con_ctrl_prt_mgt prt = {
			.sst = 0x03,
			.assn = 254,
			.apoc = MTP_MAKE_APOC(91),
			.mul_ind = 1,
		};

		uint8_t data[] = { 0x03, 0xfe, 0x5b, 0x00, 0x01 };
		if (memcmp(&prt, data, 5) != 0) {
			uint8_t *d = (uint8_t *) &prt;
			fprintf(stderr, "GOT: 0x%.2x 0x%.2x 0x%.2x 0x%.2x 0x%.2x\n",
				d[0], d[1], d[2], d[3], d[4]);
			abort();
		}
	}

	/* verify decoding of the sls */
	for (i = 0; i < 16; ++i) {
		addr = MTP_ADDR(i, 136, 91);
		if (MTP_LINK_SLS(addr) != i) {
			fprintf(stderr, "0x%x/0x%x does not match 0x%x\n", addr, MTP_LINK_SLS(addr), i);
			abort();
		}
	}

	printf("All tests passed.\n");
	return 0;
}
