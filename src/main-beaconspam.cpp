#include <WiFi.h>
#include <esp_wifi.h>
#include <stdint.h>

// Disable frame sanity checks
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;  // Bypass the raw frame sanity check
}

// ===== Settings ===== //
const uint8_t channels[] = {1, 6, 11};  // Wi-Fi channels to be used (available: 1-14)
const bool wpa2 = false;                // WPA2 networks
const uint8_t ssidBatchSize = 12;       // Number of SSIDs to broadcast at once
const uint32_t ssidDuration = 5000;    // Duration (in ms) to keep SSIDs active (10 seconds)
const uint32_t delayBetweenSSIDs = 50;  // Delay between sending each SSID (50 ms)

/*
  SSIDs:
  - max 32 characters per SSID
  - don't forget the \n at the end of each SSID!
*/
const char ssids[] PROGMEM = {
  "Mom Use This One\n"
  "Abraham Linksys\n"
  "Benjamin FrankLAN\n"
  "Martin Router King\n"
  "John Wilkes Bluetooth\n"
  "Pretty Fly for a Wi-Fi\n"
  "Bill Wi the Science Fi\n"
  "I Believe Wi Can Fi\n"
  "Tell My Wi-Fi Love Her\n"
  "No More Mister Wi-Fi\n"
  "LAN Solo\n"
  "The LAN Before Time\n"
  "Silence of the LANs\n"
  "House LANister\n"
  "Winternet Is Coming\n"
  "Ping’s Landing\n"
  "The Ping in the North\n"
  "This LAN Is My LAN\n"
  "Get Off My LAN\n"
  "The Promised LAN\n"
  "The LAN Down Under\n"
  "FBI Surveillance Van 4\n"
  "Area 51 Test Site\n"
  "Drive-By Wi-Fi\n"
  "Planet Express\n"
  "Wu Tang LAN\n"
  "Darude LANstorm\n"
  "Never Gonna Give You Up\n"
  "Hide Yo Kids, Hide Yo Wi-Fi\n"
  "Loading…\n"
  "Searching…\n"
  "VIRUS.EXE\n"
  "Virus-Infected Wi-Fi\n"
  "Starbucks Wi-Fi\n"
  "Text ###-#### for Password\n"
  "Yell ____ for Password\n"
  "The Password Is 1234\n"
  "Free Public Wi-Fi\n"
  "No Free Wi-Fi Here\n"
  "Get Your Own Damn Wi-Fi\n"
  "It Hurts When IP\n"
  "Dora the Internet Explorer\n"
  "404 Wi-Fi Unavailable\n"
  "Porque-Fi\n"
  "Titanic Syncing\n"
  "Test Wi-Fi Please Ignore\n"
  "Drop It Like It’s Hotspot\n"
  "Life in the Fast LAN\n"
  "The Creep Next Door\n"
  "Ye Olde Internet\n"
  "Lan Before Time\n"
  "Lan Of The Lost\n"
};
// ==================== //

// run-time variables
char emptySSID[32];
uint8_t macAddr[6];
uint8_t wifi_channel = 1;
uint32_t currentTime = 0;
uint32_t packetSize = 0;
uint32_t attackTime = 0;
uint32_t packetRateTime = 0;
uint32_t batchStartTime = 0;  // Start time of the current batch
uint32_t ssidIndex = 0;       // To track which SSID batch we're sending
uint32_t packetCounter = 0;    // To track the number of packets sent per second

// beacon frame definition
uint8_t beaconPacket[109] = {
  0x80, 0x00, 0x00, 0x00,  // Type/Subtype: management beacon frame
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // Destination: broadcast
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,  // Source (random MAC)
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,  // Source (random MAC)

  // Fixed parameters
  0x00, 0x00,  // Fragment & sequence number (done by SDK)
  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,  // Timestamp
  0xe8, 0x03,  // Interval: 1s (0xe8, 0x03)
  0x31, 0x00,  // Capabilities

  // Tagged parameters (SSID)
  0x00, 0x20,  // Tag: SSID length 32
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,  // Placeholder SSID
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,

  // Supported Rates
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c,  // 802.11 rates

  // Current Channel
  0x03, 0x01, 0x01,  // Channel set to 1
};

// Function to switch to the next Wi-Fi channel
void nextChannel() {
  static uint8_t channelIndex = 0;
  wifi_channel = channels[channelIndex];
  channelIndex = (channelIndex + 1) % sizeof(channels);
  esp_wifi_set_channel(wifi_channel, WIFI_SECOND_CHAN_NONE);
}

// Generate a random MAC address
void randomMac() {
  for (int i = 0; i < 6; i++) {
    macAddr[i] = random(256);
  }
}

void setup() {
  for (int i = 0; i < 32; i++)
    emptySSID[i] = ' ';

  randomSeed(esp_random());

  packetSize = sizeof(beaconPacket);
  if (wpa2) {
    beaconPacket[34] = 0x31;
  } else {
    beaconPacket[34] = 0x21;
    packetSize -= 26;
  }

  randomMac();

  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);  // Set Wi-Fi to station mode
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();   // Start Wi-Fi

  esp_wifi_set_promiscuous(true);  // Enable promiscuous mode
  esp_wifi_set_channel(channels[0], WIFI_SECOND_CHAN_NONE);  // Start with the first channel

  batchStartTime = millis();  // Initialize the start time for the batch
  Serial.println("SSIDs:");
}

void loop() {
  currentTime = millis();

  // Send 12 SSIDs at a time
  for (int i = 0; i < ssidBatchSize; i++) {
    int currentSSIDIndex = (ssidIndex + i) % strlen_P(ssids);  // Get SSID index in the batch

    // Start reading SSID from the beginning of each line
    int ssidStart = 0;
    for (int j = 0; j < currentSSIDIndex; j++) {
      if (pgm_read_byte(ssids + j) == '\n') {
        ssidStart = j + 1;
      }
    }

    int j = 0;
    char tmp;
    do {
      tmp = pgm_read_byte(ssids + ssidStart + j);
      j++;
    } while (tmp != '\n' && j <= 32);

    uint8_t ssidLen = j - 1;

    macAddr[5] = currentSSIDIndex;  // Use the SSID index in MAC address
    memcpy(&beaconPacket[10], macAddr, 6);  // Source MAC
    memcpy(&beaconPacket[16], macAddr, 6);  // BSSID

    // Clear the SSID field and fill with spaces
    memcpy(&beaconPacket[38], emptySSID, 32);

    // Insert the SSID into the beacon frame
    memcpy_P(&beaconPacket[38], &ssids[ssidStart], ssidLen);

    // Set the channel in the beacon packet
    beaconPacket[82] = wifi_channel;

    // Send beacon packet
    esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, packetSize, false);

    Serial.print("Sent SSID: ");
    for (int k = ssidStart; k < ssidStart + ssidLen; k++) {
      Serial.print((char)pgm_read_byte(ssids + k));
    }
    Serial.print(" on channel: ");
    Serial.println(wifi_channel);

    packetCounter++;  // Increment the packet counter each time a beacon is sent

    delay(delayBetweenSSIDs);  // Add a delay between sending each SSID
  }

  // Check if 10 seconds have passed to switch to the next batch of SSIDs
  if (currentTime - batchStartTime > ssidDuration) {
    ssidIndex = (ssidIndex + ssidBatchSize) % strlen_P(ssids);  // Move to the next set of SSIDs
    batchStartTime = currentTime;  // Reset the timer for the next batch
    nextChannel();  // Switch to the next Wi-Fi channel
  }

  // Show packets per second every second
  if (currentTime - packetRateTime > 1000) {
    packetRateTime = currentTime;
    Serial.print("Packets/s: ");
    Serial.println(packetCounter);
    packetCounter = 0;
  }
}
