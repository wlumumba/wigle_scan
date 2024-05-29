/*
  PARALLEL ARTIFACTS
  https://ahprojects.com/parallel-artifacts
  Adam Harvey 2022
*/

// Auto-generated from: old_havana_trial-validated.json

#include "display.h"

byte bssid[6] = {};        // store next bssid
uint8_t ncounter = 0;      // network counter
uint8_t ch_counter = 0;    // channel increment counter
uint8_t loop_counter = 0;  // loop counter interval for changing channel
uint8_t ssid_len = 0;      // holder for ssid length
uint8_t channel_pre = 0;
uint8_t packet_length = 0;

// beacon interval is 100.24 milliseconds (0x64)
unsigned long beacon_interval_us = 102400;
unsigned long last_beacon_us = 0;
char* hidden_ssid = "\x00";  // 0-length SSID (Network name unavailable)
#define NUM_TS_PLACES 8
byte time_packet[NUM_TS_PLACES];  // placeholder for timestamp data#define NUM_TS_PLACES 8
char cc[2];                       // placeholer for timestamp data

float WIFI_POWER_DBM = 19.5;  // ESP8266 only




// ---------------------------------------------------------
// START template data
// ---------------------------------------------------------
#define ESP32 1

#ifdef ESP32
#include "WiFi.h"
extern "C" {
#include "esp_wifi.h"
  esp_err_t esp_wifi_set_channel(uint8_t primary, wifi_second_chan_t second);
  esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void* buffer, int len, bool en_sys_seq);
}
#else
#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}
#endif

// #define USE_HIDDEN_SSIDS

#define NN 52
char* ssids[NN] = {
	"ARRIS-E5C7-5G", 
	"DP Kitchen", 
	"WAG_5", 
	"ipet", 
	"908-B", 
	"DENON_6CAB47C91408CBF0C0631F10D3", 
	"PetcoCare", 
	"ARRIS-8133", 
	"Sonos", 
	"PetcoStore", 
	"3201 Main", 
	"PetcoCare", 
	"PetcoGuests", 
	"ipet", 
	"spectra", 
	"416298-2.4", 
	"Daniela-5", 
	"Apt 1608", 
	"908-B", 
	"PetcoGuests", 
	"mgc-5G", 
	"HFLPD100", 
	"Guest 1902 -5", 
	"415A70-5", 
	"ARRIS-4CF4-5G", 
	"PetcoCare", 
	"NetGear83!", 
	"Ron WiFi", 
	"Master bedroom", 
	"ChezLouis", 
	"PetcoStore", 
	"Murano 3301 - 2", 
	"PetcoStore", 
	"Daisy1105-5G", 
	"PAREKH 5G", 
	"PetcoGuests", 
	"ATTepah28T", 
	"PetcoGuests", 
	"HartmanSouth", 
	"spectra", 
	"Gee", 
	"Miami", 
	"ARRIS-AD20-5G", 
	"Loco Layla-5G", 
	"PetcoCare", 
	"Ron WiFi", 
	"DIRECT-F6-HP ENVY 4510 series", 
	"scratchie", 
	"MuranoDH", 
	"ARRIS-5666-5G", 
	"ATTJY4xbn2", 
	"Yvonne", 
};
uint8_t ssid_lengths[NN] = {
	13, 
	10, 
	5, 
	4, 
	5, 
	32, 
	9, 
	10, 
	5, 
	10, 
	9, 
	9, 
	11, 
	4, 
	7, 
	10, 
	9, 
	8, 
	5, 
	11, 
	6, 
	9, 
	13, 
	8, 
	13, 
	9, 
	10, 
	8, 
	14, 
	9, 
	10, 
	15, 
	10, 
	12, 
	9, 
	11, 
	10, 
	11, 
	12, 
	7, 
	3, 
	5, 
	13, 
	13, 
	9, 
	8, 
	29, 
	9, 
	8, 
	13, 
	10, 
	6, 
};

byte bssids[NN][6] = {
	{0x60, 0xD2, 0x48, 0xF1, 0x19, 0xD4}, 
	{0x20, 0x6A, 0x94, 0xDB, 0xB2, 0xC8}, 
	{0x90, 0xA7, 0xC1, 0x81, 0x71, 0xED}, 
	{0x04, 0x62, 0x73, 0xB4, 0xEA, 0xEF}, 
	{0x74, 0x3E, 0x2B, 0x11, 0x6E, 0x38}, 
	{0x00, 0x05, 0xCD, 0x99, 0xDA, 0x53}, 
	{0x04, 0x62, 0x73, 0xB4, 0xEA, 0xE6}, 
	{0x84, 0xBB, 0x69, 0xAA, 0xE3, 0x1C}, 
	{0xB4, 0xFB, 0xE4, 0x14, 0x4E, 0x73}, 
	{0x04, 0x62, 0x73, 0xC5, 0x7A, 0x7D}, 
	{0xD4, 0xC1, 0x9E, 0x3E, 0x61, 0xAC}, 
	{0x04, 0x62, 0x73, 0xC5, 0x7A, 0x79}, 
	{0x04, 0x62, 0x73, 0xC5, 0x7A, 0x7E}, 
	{0x04, 0x62, 0x73, 0xC5, 0x7A, 0x70}, 
	{0x04, 0x62, 0x73, 0xC5, 0x7A, 0x7F}, 
	{0x60, 0x3D, 0x26, 0x41, 0x62, 0x9C}, 
	{0xE0, 0xDB, 0xD1, 0x6E, 0x5B, 0xE8}, 
	{0x60, 0x6C, 0x63, 0xCE, 0x7D, 0xF8}, 
	{0x74, 0x3E, 0x2B, 0x11, 0x6E, 0x3C}, 
	{0x04, 0x62, 0x73, 0xC5, 0x7A, 0x71}, 
	{0x84, 0xBB, 0x69, 0xAA, 0xCF, 0x94}, 
	{0x34, 0xEA, 0xE7, 0xB1, 0x17, 0x4F}, 
	{0x08, 0xA7, 0xC0, 0xAB, 0xCB, 0x40}, 
	{0x60, 0x3D, 0x26, 0x41, 0x5A, 0x7C}, 
	{0x2C, 0x58, 0x4F, 0x94, 0xB4, 0xF2}, 
	{0x04, 0x62, 0x73, 0xC5, 0x7A, 0x76}, 
	{0xB0, 0x6A, 0x41, 0x88, 0xB9, 0xB9}, 
	{0x18, 0xE8, 0x29, 0xC4, 0xE6, 0x1B}, 
	{0x5C, 0xA5, 0xBC, 0x03, 0xAC, 0xA5}, 
	{0x28, 0xBD, 0x89, 0xC7, 0x05, 0x2E}, 
	{0x04, 0x62, 0x73, 0xB4, 0xEA, 0xED}, 
	{0xCC, 0xF4, 0x11, 0x72, 0x12, 0x90}, 
	{0x04, 0x62, 0x73, 0x95, 0x54, 0x5D}, 
	{0x60, 0x3D, 0x26, 0x44, 0x75, 0x34}, 
	{0x90, 0xA7, 0xC1, 0x1A, 0xD3, 0x1D}, 
	{0x04, 0x62, 0x73, 0x95, 0x54, 0x5E}, 
	{0x08, 0x9B, 0xB9, 0xE2, 0x2A, 0xF4}, 
	{0x04, 0x62, 0x73, 0xB4, 0xEA, 0xEE}, 
	{0x90, 0xCA, 0xFA, 0xA5, 0x70, 0x3D}, 
	{0x04, 0x62, 0x73, 0x95, 0x54, 0x5F}, 
	{0xD4, 0xD7, 0x48, 0x21, 0x13, 0x70}, 
	{0xEC, 0x74, 0x27, 0xF8, 0x14, 0xE6}, 
	{0x60, 0xD2, 0x48, 0xF6, 0x0C, 0x8B}, 
	{0xD4, 0xE2, 0xCB, 0x03, 0x1E, 0x28}, 
	{0x04, 0x62, 0x73, 0x95, 0x54, 0x59}, 
	{0x18, 0xE8, 0x29, 0xC4, 0xB3, 0xAB}, 
	{0xDC, 0x4A, 0x3E, 0xB7, 0x42, 0xF7}, 
	{0x98, 0xED, 0x7E, 0xB1, 0xBC, 0x07}, 
	{0xA0, 0xFF, 0x70, 0x90, 0xC0, 0x90}, 
	{0x70, 0x4F, 0xB8, 0xFF, 0x06, 0xD5}, 
	{0xB4, 0x63, 0x6F, 0x1D, 0xB3, 0xAC}, 
	{0xE8, 0x9F, 0x80, 0xB0, 0x70, 0x9A}, 
};

#define N_CHANNELS 13
byte channels[N_CHANNELS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// {{TEMPLATE:DBM_LEVELS
// TODO
// TEMPLATE:DBM_LEVELS}}

// ---------------------------------------------------------
// END template data
// ---------------------------------------------------------


// Variables referencing templated data
uint8_t channel = channels[0];  // Transmit on this channel
unsigned long ap_epochs[NN];

// Beacon frame packet structure
uint8_t packet[128] = {
  /*0*/ 0x80, 0x00,                                       // Frame Control
  /*2*/ 0x00, 0x00,                                       // Duration
  /*4*/ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,               // Destination address
  /*10*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // Source address - overwritten later
  /*16*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // BSSID - overwritten to the same as the source address
  /*22*/ 0xc0, 0x6c,                                      // Seq-ctl
  /*24*/ 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,  // Timestamp - the number of microseconds the AP has been active
  /*32*/ 0x64, 0x00,                                      // Beacon interval = 0.102400 seconds
                                                          // /*34*/ 0x01, 0x04,                                    // Capability info
  /*34*/ 0x31, 0x14,                                      // Capability inf
  /*36*/ 0x00
  /* SSID is added after here */
};

uint8_t packet_tail[13] = {
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c,  // supported rate
  0x03, 0x01, 0x04                                             // DSSS (Current Channel)
};


// ---------------------------------------------------------------
// Convert milliseconds to timestamp for beacon frame
// ---------------------------------------------------------------
void get_timestamp(char* times) {

  long t = micros();
  unsigned int tlen = 8;
  String time_hex_forward = String(t, HEX);

  while (time_hex_forward.length() < tlen * 2) time_hex_forward = "0" + time_hex_forward;
  String time_hex_rev = time_hex_forward;  // reversed
  for (int i = tlen; i > 0; i--) {
    time_hex_rev[(tlen * 2) - (i * 2)] = time_hex_forward.charAt((i * 2) - 2);      // 1
    time_hex_rev[(tlen * 2) - (i * 2) + 1] = time_hex_forward.charAt((i * 2) - 1);  // 2
  }
  // convert to 8-length byte array
  for (int i = 0; i < tlen; i++) {

    cc[0] = time_hex_rev.charAt(i * 2);
    cc[1] = time_hex_rev.charAt((i * 2) + 1);
    long hex_val = strtol(cc, NULL, 16);
    times[i] = hex_val;
  }
}

void set_timestamp(byte* p, byte n) {
  // return time in reverse order as byte array pointer
  long t = millis() + ap_epochs[n];  // random start time
  p[0] = (t & 0x00000000000000FF);
  p[1] = (t & 0x000000000000FF00) >> 8;
  p[2] = (t & 0x0000000000FF0000) >> 16;
  p[3] = (t & 0x00000000FF000000) >> 24;
  p[4] = (t & 0x000000FF00000000) >> 32;
  p[5] = (t & 0x0000FF0000000000) >> 40;
  p[6] = (t & 0x00FF000000000000) >> 48;
  p[7] = (t & 0x00FF000000000000) >> 56;
}


long strtol(const char* __nptr, char** __endptr, int __base);  // additional function for string to long conversion


// ---------------------------------------------------------------
// Send the beacon frame
// ---------------------------------------------------------------
void send_beacon_frame(uint8_t i, uint8_t c) {

  // Set BSSID
  memcpy(&bssid, bssids[i], 6);
  memcpy(&packet[10], bssid, 6);
  memcpy(&packet[16], bssid, 6);

  // Set SSID
  ssid_len = ssid_lengths[i];
  packet[37] = ssid_len;
  memcpy(&packet[38], ssids[i], ssid_len);
  memcpy(&packet[38 + ssid_len], packet_tail, 13);
  packet[50 + ssid_len] = channels[c];

  // store new packet length
  packet_length = 51 + ssid_len;

  // Send the packet
#ifdef ESP32
  esp_wifi_80211_tx(WIFI_IF_STA, packet, packet_length, 0);  // esp32
#else
  wifi_send_pkt_freedom(packet, packet_length, 0);  // esp8266
#endif
}


void change_channel(uint8_t i) {
#ifdef ESP32
  // esp_wifi_set_promiscuous(false);
  esp_wifi_set_channel(channels[i], WIFI_SECOND_CHAN_NONE);  // esp32
  // esp_wifi_set_promiscuous(true);
#else
  wifi_set_channel(channels[i]);                    // esp8266
#endif
}


// ---------------------------------------------------------------
// Initialize the ESP8266
// Set max power and put into promiscious mode
// ---------------------------------------------------------------
void setup() {
  setup_display();
#ifdef ESP32
  WiFi.mode(WIFI_MODE_STA);
  WiFi.setTxPower(WIFI_POWER_19dBm);
  esp_wifi_set_channel(channels[0], WIFI_SECOND_CHAN_NONE);
#else
  WiFi.setOutputPower(WIFI_POWER_DBM);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1);
#endif
}

// ---------------------------------------------------------------
// Transmit Beacon Frame
// ---------------------------------------------------------------
void loop() {
  // Limit to every 100ms
  if (micros() - last_beacon_us < beacon_interval_us) {
    return;
  }

  // change channel every loop
  loop_counter += 1;
  if (loop_counter > 10) {
    ch_counter = (ch_counter + 1) % N_CHANNELS;
    change_channel(ch_counter);
    loop_counter = 0;
  }
  
  // Send all ssid packets
  for (int i = 0; i < NN; i++) {
    send_beacon_frame(i, ch_counter);
    delay(1);
  }

  // update time of broadcast
  last_beacon_us = micros();

  // display
  run_display();
}