# DeauthKeychain – ESP32-C3 Wi-Fi Deauthentication Tool  

[![GitHub](https://img.shields.io/github/stars/zRCrackiiN/DeauthKeychain?style=social)](https://github.com/zRCrackiiN/DeauthKeychain)  
Absolutely! Here's your updated README while keeping the original content and adding details about the **multi-environment setup** and **Beacon Spam** mode at the bottom.

---

# DeauthKeychain – ESP32-C3 Wi-Fi Deauthentication Tool  

[![GitHub](https://img.shields.io/github/stars/zRCrackiiN/DeauthKeychain?style=social)](https://github.com/zRCrackiiN/DeauthKeychain)  

This project is a **Wi-Fi deauthentication tool** for the **ESP32-C3**, capable of scanning nearby networks and sending **deauth packets** to disconnect clients from their access points.  

## Features  
✅ **Automatic Wi-Fi Scanning** – Detects and stores access points (APs) within range.  
✅ **Mass Deauthentication** – Sends multiple deauth packets per AP to disrupt client connections.  
✅ **ESP32-C3 Optimized** – Uses `esp_wifi_80211_tx()` for raw frame injection.  
✅ **Efficient Packet Transmission** – Sends **100 deauth frames per AP** for increased effectiveness.  
✅ **Minimal Delay** – Rapid execution with only **10ms delay** between deauth cycles.  
✅ **Multi-Environment Support** – Easily switch between different attack modes using **PlatformIO environments**.  

## Hardware Requirements  
- **ESP32-C3** development board  
- USB-C cable for flashing firmware  

## Installation  
1. Clone this repository:  
   ```sh
   git clone https://github.com/zRCrackiiN/DeauthKeychain.git
   ```  
2. Install **PlatformIO** or **Arduino IDE** with ESP32 board support.
3. Select your enviroment.
4. Flash the firmware to your ESP32-C3 board.  

## Multi-Environment Support (PlatformIO)  
This project is designed to support **multiple attack modes** using PlatformIO’s environment configuration. Each mode has its own `main.cpp`, automatically selected during compilation.  

### Available Environments:  
| Environment | Function |
|------------|----------|
| `deauth`   | Scans for nearby networks and sends **deauth packets** to disconnect clients. |
| `beacon_spam` | Floods the airwaves with **fake beacon frames**, creating multiple rogue access points. |

### Compiling & Uploading:  
To build and upload a specific environment, use:  
```sh
pio run -e [environment_name] -t upload
```  
Replace `[environment_name]` with one of the available environments, e.g., `deauth` or `beacon_spam`.  

## Usage  

### **Deauthentication Attack**  
- Flash the `deauth` environment to your ESP32-C3.  
- Upon startup, the device will scan for nearby networks and send deauth packets to disconnect clients.  

### **Beacon Spam Attack**  
- Flash the `beacon_spam` environment to your ESP32-C3.  
- The device will broadcast numerous fake beacon frames to simulate multiple rogue access points.  

## Disclaimer ⚠  
This project is intended for **educational and security research purposes only**.  
Unauthorized use of deauthentication or beacon spam attacks is illegal in many jurisdictions. **Only use on networks you own or have explicit permission to test.**  

## License  
This project is open-source and provided without warranty. Use at your own risk.  

📌 **GitHub Repository**: [DeauthKeychain](https://github.com/zRCrackiiN/DeauthKeychain)  
