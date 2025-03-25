#include <Wire.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// LED Pins
const int greenLed = 4;  // Sufficient funds
const int whiteLed = 5;  // Used Crediit
const int redLed = 6;    // Insufficient funds

// Payment System
float walletBalance = 50.0;
float creditLimit = 20.0;
float farePerKm = 2.30;
float debt = 0.0;  // Tracks user's debt amount

bool isBoarded = false;
bool hasDebt = false;  // Tracks the user's debt status
String cardUID = "";
unsigned long boardingTime = 0;
unsigned long lastSwipeTime = 0;

String registeredCards[] = {// Add card UIDs to register them
  "f34e322a",
};

void setup() {
  Serial.begin(115200);
  pinMode(greenLed, OUTPUT);
  pinMode(whiteLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 not found!");
    while (1);
  }
  nfc.SAMConfig();
  Serial.println("System ready. Waiting for card...");
}

void loop() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    unsigned long currentTime = millis();

    if (currentTime - lastSwipeTime < 4000) {
      Serial.println("Please wait...");
      return;
    }
    lastSwipeTime = currentTime;

    String currentUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      currentUID += String(uid[i], HEX);
    }

    bool isRegistered = false;
    for (int i = 0; i < sizeof(registeredCards)/sizeof(registeredCards[0]); i++) {
      if (currentUID == registeredCards[i]) {
        isRegistered = true;
        break;
      }
    }

    if (isRegistered) {
      Serial.print("Card detected: "); Serial.println(currentUID);

      if (hasDebt) {  // Check for debt before allowing boarding
        Serial.println("CARD DECLINED - Outstanding debt!");
        blinkLed(redLed);
        return;  // exit without processing when there is unsettled debt
      }

      if (!isBoarded) {
        Serial.println("Boarding registered.");
        isBoarded = true;
        boardingTime = currentTime;
      } else {
        float distance = (currentTime - boardingTime)/1000.0;
        float fare = distance * farePerKm;
        Serial.print("Alighting. Distance: "); Serial.print(distance); Serial.println(" km");
        Serial.print("Fare: "); Serial.print(fare); Serial.println(" bob");
        processPayment(fare);
        isBoarded = false;
      }
    } else {
      Serial.println("Unregistered card!");
    }
  }
  delay(500);
}

void processPayment(float fare) {
  if (walletBalance >= fare) {
    walletBalance -= fare;
    Serial.print("Paid from wallet. Balance: "); Serial.println(walletBalance);
    blinkLed(greenLed);
  } 
  else if (walletBalance + creditLimit >= fare) {
    float creditUsed = fare - walletBalance;
    walletBalance = 0;
    creditLimit -= creditUsed;
    Serial.print("Used credit: "); Serial.print(creditUsed); 
    Serial.print(" Credit left: "); Serial.println(creditLimit);
    blinkLed(whiteLed);
  } 
  else {
    float unpaid = fare - (walletBalance + creditLimit);
    walletBalance = 0;
    creditLimit = 0;
    debt += unpaid;
    hasDebt = true;  // Set debt flag for the card UID
    Serial.print("DEBT INCURRED: "); Serial.print(unpaid); 
    Serial.print(" Total debt: "); Serial.println(debt);
    blinkLed(redLed);
  }
}

void blinkLed(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(200);  // pause between blinks to show processing
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
}