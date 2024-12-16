#include <TimerOne.h>
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 22;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

int numbers[10][7] = {
  {1,1,1,0,1,1,1}, //0
  {0,0,1,0,1,0,0}, //1
  {0,1,1,1,0,1,1}, //2
  {0,1,1,1,1,1,0}, //3
  {1,0,1,1,1,0,0}, //4
  {1,1,0,1,1,1,0}, //5
  {1,1,0,1,1,1,1}, //6
  {0,1,1,0,1,0,0}, //7
  {1,1,1,1,1,1,1}, //8
  {1,1,1,1,1,0,0}  //9
};

int digitsPins[] = {13,12,11,10};

int temp = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  Serial.begin(9600);
}

int temperature = 1;

void loop() {
  // put your main code here, to run repeatedly:
  
  //int temperature = readTemp();
  //int numberOfdigits = numberOfDigits(temperature);

  float temperatureFromModule;
  float humidity;
  int tempToDisplay = (temperature * 9.0)/ 5.0 + 32.0;
  //int tempToDisplay = temperature;
  

  // /* Measure temperature and humidity.  If the functions returns
  //    true, then a measurement is available. */
  if( measure_environment( &temperatureFromModule, &humidity ) == true )
  {
    temperature = temperatureFromModule;
  }

  int numberOfdigits = numberOfDigits(tempToDisplay);

  for(int i = 0; i < numberOfdigits; i++){
    int display = int(tempToDisplay) % 10;
    displayNumber(display,i);
    //delay(500);
    tempToDisplay = tempToDisplay / 10;
  
  }

  
}

void clearNumber() {
  for(int i = 10; i <= 13; i++){
    digitalWrite(i,HIGH);
  }
  for(int i = 2; i <= 8; i++){
    digitalWrite(i,LOW);
  }
}

void displayNumber(int number,int digit) {
  clearNumber();

  int digitPin = digitsPins[digit];

  digitalWrite(digitPin,LOW);

  for(int i = 0; i < 7; i++){
    if(numbers[number][i] == 1){
      digitalWrite(i + 2,HIGH);
    }
  }
}

int numberOfDigits(int number) {
  int count = 0;
  while(number!=0){
    number = number/10;
    count++;
  }
  return count;
}

int readTemp() {
  int tempReading = analogRead(0);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  //temp = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempC = tempK - 273.15; 
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  return int(tempC);
  //

}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}
