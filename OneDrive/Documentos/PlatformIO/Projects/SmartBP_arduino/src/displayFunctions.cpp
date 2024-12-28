#include "displayFunctions.h"
#include "globalObjects.h"

void displayLoop() {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure"));
    display.display();
    delay(500);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure."));
    display.display();
    delay(500);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure.."));
    display.display();
    delay(500);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure..."));
    display.display();
    delay(500);
}

void displayNewMeasure(){
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Measure started!"));
    display.display();
}

void displayDataSent() {
    display.clearDisplay();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Data Sent!"));

    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
}

void displayConnected() {

}
