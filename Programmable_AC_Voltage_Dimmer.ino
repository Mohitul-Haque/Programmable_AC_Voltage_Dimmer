/*  
 *   WHAT THIS CODE WILL DO?:
 *   Connect a programmable voltage dimmer with the connection stated below.
 *   This code will dim the AC load according to the input serial value.
 *   If you 
 *      1.  Open serial monitor( (Ctrl + Shift + M)) 
 *      2.  Select "No Line Ending" & "9600 baud"
 *      3.  Type 3 and hit enter 
 *   then the connected AC load will get 30% voltage.
 *   PUT VALUES BETWEEN 1 to 9 ie (1,2,3,4,5,6,7,8,9 NOT 0 or 10 or fractions)
 *   If you type 4 in serial monitor the connected AC load will get 40% voltage etc.
 *  
 *  CONNECTION:
 *    Module's male header pins     Arduino Uno's pins
 *      GND                             GND
 *      RLY                             3
 *      O-X                             2
 *      +5V                             5V
 *  
 *    Module's Green Block
 *    (BE EXTREAMELY CAUTIOUS)              POWER BOARD
 *      220V AC block pin 1                   Live
 *      220V AC block pin 2(other one)        Neutral
 *      Load block pin 1                      One End of 220V AC compatible load
 *      Load block pin2 (other one)           Other End of 220V AC compatible load
 *      
 *  
 *  DEVELOPER:
 *    MD. FAHIMUL ISLAM
 *    Research Engineer
 *    Techshop Bangladesh
 *    im.fahimul.islam@gmail.com
 *    https://bd.linkedin.com/in/mdfahimulislam
 *  
 *  LAST UPDATED:
 *    02.03.17
 * 
 */


#define CORRECTION 700

int AC_LOAD = 3;    // Output to Opto Triac pin. That means connect RLY pin to Arduino Uno's pin number 3.
int dimming = 85;  // Dimming level (0-128)  0 = ON, 128 = OFF
int incomingByte;   // for incoming serial data

void setup()
{
  pinMode(AC_LOAD, OUTPUT);// Set AC Load pin as output
  attachInterrupt(0, zero_crosss_int, RISING);        // That means connect O-X to arduino pin number 2
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Programmable AC Voltage Dimmer");
}

//the interrupt function must take no parameters and return nothing
void zero_crosss_int()  //function to be fired at the zero crossing to dim the light
{
  int dimtime = (95*dimming);    // For 60Hz =>65    
  delayMicroseconds(dimtime+CORRECTION);    // Wait till firing the TRIAC    
  digitalWrite(AC_LOAD, HIGH);   // Fire the TRIAC
  delayMicroseconds(10);         // triac On propogation delay 
         // (for 60Hz use 8.33) Some Triacs need a longer period
  digitalWrite(AC_LOAD, LOW);    // No longer trigger the TRIAC (the next zero crossing will swith it off) TRIAC
}

void loop()  {
if (Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.read()-48;

    // say what you got:
    Serial.print("Brightness : ");
    Serial.print(incomingByte*10); Serial.println("%");
    dimming=100-incomingByte*10;
    delay(10);
  }

}

