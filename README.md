# DeauthKeychain – ESP32-C3 Wi-Fi Deauthentication Tool  

[![GitHub](https://img.shields.io/github/stars/zRCrackiiN/DeauthKeychain?style=social)](https://github.com/zRCrackiiN/DeauthKeychain)  
This project is a **Wi-Fi deauthentication tool** for the **ESP32-C3**, capable of scanning nearby networks and sending **deauth packets** to disconnect clients from their access points.  

## Features  
✅ **Automatic Wi-Fi Scanning** – Detects and stores access points (APs) within range.  
✅ **Mass Deauthentication** – Sends multiple deauth packets per AP to disrupt client connections.  
✅ **ESP32-C3 Optimized** – Uses `esp_wifi_80211_tx()` for raw frame injection.  
✅ **Efficient Packet Transmission** – Sends **100 deauth frames per AP** for increased effectiveness.  
✅ **Minimal Delay** – Rapid execution with only **10ms delay** between deauth cycles.  

## Hardware Requirements  
- **ESP32-C3** development board  
- USB-C cable for flashing firmware  

## Installation  
1. Clone this repository:  
   ```sh
   git clone https://github.com/zRCrackiiN/DeauthKeychain.git
   ```  
2. Install **PlatformIO** or **Arduino IDE** with ESP32 board support.  
3. Flash the firmware to your ESP32-C3 board.  

## Usage  
- **Power on the ESP32-C3**, and it will automatically:  
  - Scan for Wi-Fi networks.  
  - Send **deauth packets** to every detected AP.  
  - Continuously loop with minimal delay for maximum disruption.  

## Disclaimer ⚠  
This project is intended for **educational and security research purposes only**.  
Unauthorized use of deauthentication attacks is illegal in many jurisdictions. **Only use on networks you own or have explicit permission to test.**  

## License  
This project is open-source and provided without warranty. Use at your own risk.  

📌 **GitHub Repository**: [DeauthKeychain](https://github.com/zRCrackiiN/DeauthKeychain)  
