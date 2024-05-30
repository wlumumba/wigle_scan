/*
  PARALLEL ARTIFACTS
  https://ahprojects.com/parallel-artifacts
  Adam Harvey 2022
*/

// Auto-generated from: castillo.json

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

#define NN 84
char* ssids[NN] = {
	"RedBull", 
	"Swarm Staff", 
	"Wynwood Marketplace FREE WiFi", 
	"swarm-POS-5G", 
	"Toast-POS", 
	"Swarm Staff", 
	"Wynwood Marketplace FREE WiFi", 
	"swarm-POS-5G", 
	"RedBull", 
	"swarm-POS-2G", 
	"DIRECT-81-HP ENVY 5000 series", 
	"Wynwood Marketplace FREE WiFi", 
	"Swarm Staff", 
	"Wynwood Marketplace FREE WiFi", 
	"swarm-POS-5G", 
	"RedBull", 
	"swarm-POS-5G", 
	"RedBull", 
	"Wynwood Marketplace FREE WiFi", 
	"DIRECT-B4-FireTV_6bcd", 
	"swarm-POS-2G", 
	"[LG_Window A/C]745a", 
	"Wynwood Marketplace FREE WiFi", 
	"Swarm Staff", 
	"RedBull", 
	"Wynwood Marketplace FREE WiFi", 
	"Wynwood Marketplace FREE WiFi", 
	"Guest WiFi", 
	"Xfinity Mobile", 
	"RedBull", 
	"swarm-POS-2G", 
	"Peppermint", 
	"Wynwood Marketplace FREE WiFi", 
	"swarm-POS-2G", 
	"RedBull", 
	"Xfinity Mobile", 
	"Hotspot9745", 
	"swarm-POS-5G", 
	"Gotham Investigations", 
	"Gotham Investigations", 
	"AVATAR_MIAMI-2.4", 
	"Swarm Staff", 
	"Wynwood Marketplace FREE WiFi", 
	"AVATAR_MIAMI-5.0", 
	"swarm-POS-5G", 
	"swarm-POS-2G", 
	"Amazon Shop Local", 
	"Swarm Staff", 
	"Wynwood Marketplace FREE WiFi", 
	"swarm-POS-2G", 
	"Peppermint", 
	"myChevrolet 8021", 
	"(add)wifi", 
	"swarm-POS-5G", 
	"myChevrolet3924", 
	"Redmi Note 11 Pro 5G", 
	"SBG8300Guest-6B41", 
	"myChevrolet3924", 
	"Alix’s iPad", 
	"My VW 7156", 
	"PROJcea4ca", 
	"PROJa3628f", 
	"myChevrolet 8021", 
	"DIRECT-HH-0004F3171B47", 
	"myChevrolet2915", 
	"Bullet Pointe2.4", 
	"Taycan_2166_5GHz", 
	"myChevrolet9559", 
	"Gallo", 
	"myChevrolet9559", 
	"Gotham Investigations", 
	"iPhone (2)", 
	"Rikenley’s iPhone", 
	"swarm-POS-2G", 
	"Parrot_00:12:1C:BE:E3:82", 
	"HotspotCCAC", 
	"Hotspot9681", 
	"SBG8300Guest-6B41-5G", 
	"Bullet Pointe 5G", 
	"GAC Guest", 
	"OpenStore", 
	"OpenStore Guest", 
	"Atomic Guest", 
	"xfinitywifi", 
};
uint8_t ssid_lengths[NN] = {
	7, 
	11, 
	29, 
	12, 
	9, 
	11, 
	29, 
	12, 
	7, 
	12, 
	29, 
	29, 
	11, 
	29, 
	12, 
	7, 
	12, 
	7, 
	29, 
	21, 
	12, 
	19, 
	29, 
	11, 
	7, 
	29, 
	29, 
	10, 
	14, 
	7, 
	12, 
	10, 
	29, 
	12, 
	7, 
	14, 
	11, 
	12, 
	21, 
	21, 
	16, 
	11, 
	29, 
	16, 
	12, 
	12, 
	17, 
	11, 
	29, 
	12, 
	10, 
	16, 
	9, 
	12, 
	15, 
	20, 
	17, 
	15, 
	11, 
	10, 
	10, 
	10, 
	16, 
	22, 
	15, 
	16, 
	16, 
	15, 
	5, 
	15, 
	21, 
	10, 
	17, 
	12, 
	24, 
	11, 
	11, 
	20, 
	16, 
	9, 
	9, 
	15, 
	12, 
	11, 
};

byte bssids[NN][6] = {
	{0x0E, 0xEC, 0xDA, 0xB8, 0xD3, 0x6A}, 
	{0x02, 0xEC, 0xDA, 0xB8, 0xD3, 0x6A}, 
	{0x06, 0xEC, 0xDA, 0xB8, 0xD3, 0x6A}, 
	{0x0A, 0xEC, 0xDA, 0xB8, 0xD3, 0x6A}, 
	{0x18, 0xE8, 0x29, 0x9B, 0x7C, 0x1B}, 
	{0x1E, 0xE8, 0x29, 0x9B, 0x7C, 0x1B}, 
	{0x22, 0xE8, 0x29, 0x9B, 0x7C, 0x1B}, 
	{0x26, 0xE8, 0x29, 0x9B, 0x7C, 0x1B}, 
	{0x2A, 0xE8, 0x29, 0x9B, 0x7C, 0x1B}, 
	{0x22, 0xE8, 0x29, 0x9A, 0x7C, 0x1B}, 
	{0x06, 0x0E, 0x3C, 0x5B, 0xFC, 0x81}, 
	{0x02, 0xEC, 0xDA, 0xB7, 0xD0, 0x17}, 
	{0x02, 0xEC, 0xDA, 0xB8, 0xD0, 0x17}, 
	{0x06, 0xEC, 0xDA, 0xB8, 0xD0, 0x17}, 
	{0x0A, 0xEC, 0xDA, 0xB8, 0xD0, 0x17}, 
	{0x0E, 0xEC, 0xDA, 0xB8, 0xD0, 0x17}, 
	{0x0A, 0xEC, 0xDA, 0xBE, 0x8D, 0x1D}, 
	{0x0E, 0xEC, 0xDA, 0xBE, 0x8D, 0x1D}, 
	{0x06, 0xEC, 0xDA, 0xBE, 0x8D, 0x1D}, 
	{0x1A, 0x74, 0x2E, 0x10, 0x7A, 0x35}, 
	{0x06, 0xEC, 0xDA, 0xBD, 0x8D, 0x1D}, 
	{0x16, 0x7F, 0x67, 0x6F, 0x74, 0x5A}, 
	{0x1E, 0xE8, 0x29, 0x9A, 0x7C, 0x1B}, 
	{0x02, 0xEC, 0xDA, 0xBE, 0x8D, 0x1D}, 
	{0x0E, 0xEC, 0xDA, 0xB8, 0xD3, 0x5E}, 
	{0x02, 0xEC, 0xDA, 0xBD, 0x8D, 0x1D}, 
	{0x02, 0xEC, 0xDA, 0xB7, 0xD3, 0x6A}, 
	{0x24, 0x79, 0x2A, 0x01, 0x50, 0x58}, 
	{0x06, 0xAB, 0x82, 0xAC, 0xF6, 0x97}, 
	{0x02, 0x9F, 0xC2, 0xFF, 0x69, 0x5E}, 
	{0x06, 0xEC, 0xDA, 0xB7, 0xD0, 0x17}, 
	{0x08, 0x36, 0xC9, 0x9A, 0x33, 0xE7}, 
	{0x06, 0xEC, 0xDA, 0xB8, 0xD3, 0x5E}, 
	{0x06, 0xEC, 0xDA, 0xB7, 0xD3, 0x5E}, 
	{0x06, 0x92, 0xBF, 0x68, 0x88, 0xA5}, 
	{0x12, 0x71, 0xC2, 0xE8, 0x65, 0xB8}, 
	{0x00, 0x54, 0xAF, 0x2D, 0x97, 0x45}, 
	{0x0A, 0xEC, 0xDA, 0xB8, 0xD3, 0x5E}, 
	{0x28, 0x74, 0xF5, 0x55, 0xD7, 0xAC}, 
	{0x28, 0x74, 0xF5, 0x55, 0xD7, 0xA8}, 
	{0x00, 0x71, 0xC2, 0xD4, 0xCE, 0x78}, 
	{0x02, 0xEC, 0xDA, 0xB8, 0xD3, 0x5E}, 
	{0x02, 0xEC, 0xDA, 0xB7, 0xD3, 0x5E}, 
	{0x00, 0x71, 0xC2, 0xE8, 0x65, 0xB8}, 
	{0x02, 0x92, 0xBF, 0x68, 0x88, 0xA5}, 
	{0x06, 0xEC, 0xDA, 0xB7, 0xD2, 0x2A}, 
	{0x06, 0x92, 0xBF, 0x68, 0x8A, 0x67}, 
	{0x18, 0xE8, 0x29, 0x9A, 0x7C, 0x1B}, 
	{0x02, 0xEC, 0xDA, 0xB7, 0xD5, 0x84}, 
	{0x06, 0xEC, 0xDA, 0xB7, 0xD5, 0x84}, 
	{0x08, 0x36, 0xC9, 0x9A, 0x33, 0xE8}, 
	{0x02, 0x24, 0xA5, 0x2A, 0xE5, 0x5D}, 
	{0x06, 0xEC, 0xDA, 0x14, 0x6A, 0x81}, 
	{0x02, 0x9F, 0xC2, 0xFF, 0x6C, 0x7A}, 
	{0x02, 0x92, 0xA5, 0xAF, 0x29, 0x4D}, 
	{0x06, 0x42, 0x07, 0x52, 0xD2, 0x24}, 
	{0x26, 0x94, 0xCB, 0x90, 0x9D, 0xAA}, 
	{0x02, 0x92, 0xA5, 0xA7, 0x29, 0x4D}, 
	{0x02, 0xF9, 0x2B, 0x3E, 0xB3, 0x4A}, 
	{0x1A, 0x48, 0xCA, 0x9E, 0x95, 0x58}, 
	{0x2A, 0x38, 0x5C, 0xCE, 0xA4, 0xCA}, 
	{0x2A, 0x38, 0x5C, 0xA3, 0x62, 0x8F}, 
	{0x02, 0x49, 0xA5, 0x2A, 0xE5, 0x5D}, 
	{0x00, 0x04, 0xF3, 0x17, 0x1B, 0x47}, 
	{0x2A, 0x07, 0xEB, 0xE6, 0x66, 0x59}, 
	{0x24, 0x94, 0xCB, 0x90, 0x9D, 0xEA}, 
	{0x12, 0xE8, 0xA7, 0x67, 0x22, 0x44}, 
	{0x02, 0x24, 0xA5, 0x8F, 0xD7, 0x6E}, 
	{0x0A, 0xA7, 0xC0, 0x2A, 0x5F, 0x3B}, 
	{0x02, 0x49, 0xA5, 0x8F, 0xD7, 0x6E}, 
	{0x28, 0x74, 0xF5, 0x55, 0xD7, 0xA4}, 
	{0x2E, 0x24, 0xF9, 0xAD, 0x36, 0x4C}, 
	{0x1A, 0x41, 0x0E, 0xA5, 0xAC, 0x8C}, 
	{0x06, 0xEC, 0xDA, 0xB7, 0xD3, 0x6A}, 
	{0x00, 0x12, 0x1C, 0xBE, 0xE3, 0x82}, 
	{0x22, 0xAD, 0x56, 0x2C, 0xCC, 0xAC}, 
	{0x00, 0x54, 0xAF, 0x76, 0x96, 0x81}, 
	{0x26, 0x94, 0xCB, 0x90, 0x9D, 0xAB}, 
	{0x24, 0x94, 0xCB, 0x90, 0x9D, 0xEB}, 
	{0x0E, 0x8D, 0xCB, 0x93, 0x75, 0xD6}, 
	{0x26, 0x3F, 0x1B, 0xF2, 0x16, 0x3F}, 
	{0x22, 0x3F, 0x1B, 0xF2, 0x16, 0x3F}, 
	{0x2A, 0x3F, 0x1B, 0xF2, 0x16, 0x3F}, 
	{0x0A, 0x2D, 0xC0, 0x1C, 0x5E, 0x46}, 
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