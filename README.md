# YouTube Link: https://youtu.be/eA4jYPgblv4

3 Documentation
# NFC-Based Cashless Bus Fare System - Arduino Demo

# Overview
This project demonstrates a cashless bus fare payment system using NFC technology. The system tracks boarding and alighting times to calculate fares based on travel duration (1 second = 1 km). It includes a wallet system with credit limits and debt prevention features.

- It lacks the use of resistors since the power used is from a controlled circuit.

# Hardware Requirements
- Arduino Nano
- PN532 NFC/RFID module
- NFC cards/tags (Mifare Classic recommended)
- 3 LEDs (Green, White, Red)
- Breadboard and jumper wires

# Wiring Diagram
| Component | Arduino Pin |
|-----------|------------|
| PN532 VCC | 3.3V       |
| PN532 GND | GND        |
| PN532 SDA | A4         |
| PN532 SCL | A5         |
| PN532 IRQ | D2         |
| PN532 RST | D3         |
| Green LED | D4         |
| White LED | D5         |
| Red LED   | D6         |

# Features
- Boarding/alighting detection via NFC
- Time-based fare calculation (1s = 1km)
- Wallet system with credit limit
- Debt prevention (blocks boarding when in debt)
- Visual feedback via LEDs:
  - Green: Payment from wallet
  - White: Payment from credit
  - Red: Insufficient funds/debt

# Setup Instructions
1. Connect hardware as per wiring diagram
2. Set PN532 switches to I2C mode (1,0)
3. Upload the Arduino sketch
4. Open Serial Monitor (115200 baud)

# Usage
1. Tap registered NFC card to board (first tap)
2. Tap again to alight (second tap)
3. System calculates fare based on travel time
4. LED indicates payment status:
   - Green: Paid from wallet
   - White: Used credit
   - Red: Insufficient funds (debt incurred)

# Customization
- Edit `registeredCards[]` array to add your NFC card UIDs
- Adjust fare rate by changing `farePerKm` value
- Modify initial balances in `walletBalance` and `creditLimit`

## Troubleshooting
- If getting gibberish output:
  - Verify baud rate (115200)
  - Check PN532 power (use 3.3V)
  - Confirm I2C switch settings (1,0)
- If LED doesn't light:
  - Check anode/cathode orientation
