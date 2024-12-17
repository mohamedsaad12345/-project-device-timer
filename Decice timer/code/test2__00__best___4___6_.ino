
#include <SparkFunDS1307RTC.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Countimer.h>
LiquidCrystal lcd(7,6,5,4,3,2);

Countimer timer;
#define move 8
#define PRINT_USA_DATE
#define menu A0 // menu button and button_set
#define up A1 // up button
#define down A2 // down button
#define button_start  A3

//#define output1 9 // output pin
 //for relay 1
//#define output2 10 // output pin for relay 2


void printTime();
// variables
uint8_t m,h,s,m1,h1,s1,m2,h2,s2,m3,h3,s3,m4,h4,s4,x,md,sm,ssm,smm,dly ;
bool st,am,am1,am2,am3,am4,su,sd,dl,smn ;
//////////////////////////////////
int time_s = 0;
int time_m = 0;
int time_h = 0;

int set=0, mode=0;
int flag1=0, flag2=0, flag3=0,flage4=0 , flag11=0;

int motorPin = 11;
int fanPin = 10;
int buzzer = A5;
int y = 0;
int ReadOn=0;
int ReadOff=0;
int count = 0;
//////////////////////////////////
void setup() {
  //rtc.setTime(second, minute, hour, day, date, month, year);
  // Use the serial monitor to view time/date output
  Serial.begin(9600);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
////////////////////////////////////////////////////////////////////////////////////////////////////////

 // lcd.begin(16, 2);
 // lcd.print("TIME:12:00:00 AM");
 // lcd.setCursor(0, 1); 
 // lcd.print("OUTPUT RELAY:OFF");
//////////////////////////////////////////////////
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     WELCOME   ");
  lcd.setCursor(0, 1);
  lcd.print("  ON/OFF Timer  ");
  timer.setInterval(print_time, 999);
  delay(1000);
  lcd.clear();
  read_on_time();

/////////////////////////////////////////////////

  pinMode(move, INPUT_PULLUP);
  pinMode(menu, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(button_start, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

//  pinMode(output1, OUTPUT);
// pinMode(output2, OUTPUT);

  rtc.begin(); 

  // e.g. 7:32:16 | Monday October 31, 2016:
  // rtc.setTime(15, 59, 23, 4, 28, 2, 19);  // Uncomment to manually set time

  rtc.set12Hour(); // Use rtc.set12Hour to set to 12-hour mode

  s1 = EEPROM.read(20);
  if (s1 > 59) s1 = 0;
  m1 = EEPROM.read(21);
  if (m1 > 59) m1 = 0;
  h1 = EEPROM.read(22);
  if (h1 > 12) h1 = 1;
  am1 = EEPROM.read(23);
  if (am1 > 1) am1 = 0;

  s2 = EEPROM.read(24);
  if (s2 > 59) s2 = 0;
  m2 = EEPROM.read(25);
  if (m1 > 59) m2 = 0;
  h2 = EEPROM.read(26);
  if (h2 > 12) h2 = 1;
  am2 = EEPROM.read(27);
  if (am2 > 1) am2 = 0;


}
void loop() {
//////////////////////////////////////////////////////////
int read = digitalRead(up);

//Serial.print(   h1   );
 if( y%2==0 && read == LOW || flage4 == 1  && flag11 == 1 ){
  y++;
  flage4=0;
  flag11 = 0;
  //h2='\0';

 }
  
  do{
    while(count<2000 and read == HIGH and y%2==0){
      count++;
      if (count == 1998){
         y++;
         flage4=0;
        flag11 = 0;
        //h2='\0';
      }
   read = digitalRead(up);
   Serial.print(read);
   //delay(1000);
  
  /////////////////////////////////////////////////////////////
  timer.run();

  if (digitalRead(menu) == 0) {
    if (flag1 == 0 && flag2 == 0) {
      flag1 = 1;
      mode = mode + 1;
      if (mode > 2) {
        mode = 0;
        set = 0;
      }
      if (mode == 1) {
        read_on_time();
        flag3 = 0;
        flage4=0;
      }
      if (mode == 2) {
        read_of_time();
        flag3 = 1;
        flage4=1;
      }

      delay(100);
    }
  }

  if (digitalRead(up) == 0) {
    if (set == 0 && mode == 0) {
      timer.start();
      flag2 = 1;
      flag11 = 1;
    }
    if (set == 1) {
      time_s++;
    }
    if (set == 2) {
      time_m++;
    }
    if (set == 3) {
      time_h++;
    }
    if (time_s > 59) {
      time_s = 0;
    }
    if (time_m > 59) {
      time_m = 0;
    }
    if (time_h > 99) {
      time_h = 0;
    }
    if (set > 0 && mode == 1) {
      write_on_time();
    }
    if (set > 0 && mode == 2) {
      write_of_time();
    }
    delay(200);
  }

  if (digitalRead(down) == 0) {
    if (set == 0 && mode == 0) {
      timer.stop();
      flag2 = 0;
      flag11 = 0;
    }
    if (set == 1) {
      time_s--;
    }
    if (set == 2) {
      time_m--;
    }
    if (set == 3) {
      time_h--;
    }
    if (time_s < 0) {
      time_s = 59;
    }
    if (time_m < 0) {
      time_m = 59;
    }
    if (time_h < 0) {
      time_h = 99;
    }
    if (set > 0 && mode == 1) {
      write_on_time();
    }
    if (set > 0 && mode == 2) {
      write_of_time();
    }
    delay(200);
  }

  if (digitalRead(button_start) == 0) {
    if (mode == 0) {
      flag2 = 1;
      flag11 = 1;
      flag3 = 0;
      flage4=0;
      read_on_time();
      timer.restart();
      timer.start();
    }
    if (mode > 0 && flag1 == 0) {
      flag1 = 1;
      set = set + 1;
      if (set > 3) {
        set = 0;
      }
      delay(100);
    }
  }

  if (digitalRead(menu) == 1 && digitalRead(button_start) == 1) {
    flag1 = 0;
  }

  lcd.setCursor(0, 0);
  if (flag3 == 0) {
    lcd.print(" On ");
  }
  if (flag3 == 1) {
    lcd.print("Off ");
  }
  lcd.print("Timer");
  if (set == 0 && mode > 0) {
    lcd.print(" Mode S ");
  }
  if (set == 0 && mode == 0 && flag2 == 0) {
    lcd.print("  Stop ");
  }
  if (set == 0 && mode == 0 && flag2 == 1) {
    lcd.print(" Start ");
  }
  if (set == 1) {
    lcd.print(" Set SS  ");
  }
  if (set == 2) {
    lcd.print(" Set MM  ");
  }
  if (set == 3) {
    lcd.print(" Set HH  ");
  }

  lcd.setCursor(4, 1);
  if (time_h <= 9) {
    lcd.print("0");
  }
  lcd.print(time_h);
  lcd.print(":");
  if (time_m <= 9) {
    lcd.print("0");
  }
  lcd.print(time_m);
  lcd.print(":");
  if (time_s <= 9) {
    lcd.print("0");
  }
  lcd.print(time_s);
  lcd.print("   ");
  //write_on_time();
  //read_on_time();
  if (time_s == 0 && time_m == 0 && time_h == 0 && flag2 == 1) {
    flag3 = flag3 + 1;
    flage4=flage4+1;
    if (flag3 > 1) {
      flag3 = 0;
      flage4=0;
    }

    if (flag3 == 0) {
      read_on_time();
    }
    if (flag3 == 1) {
      read_of_time();
    }

    timer.restart();
    timer.start();

    digitalWrite(motorPin, flag3);
    digitalWrite(fanPin, flag3);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
  }

  if (flag2 == 1) {
    digitalWrite(motorPin, flag3);
    digitalWrite(fanPin, flag3);
  } else {
    digitalWrite(motorPin, LOW);
    digitalWrite(fanPin, LOW);
  }

  delay(1);

////////////////////////////////////////////////////////////////////////////////////
  int ee2on = EEPROM.read(2);
  int ee3on = EEPROM.read(3);
  if(ee2on>0){
    ee2on=ee2on*60;
     
  }
  if(ee3on>0){
    ee3on=ee3on*60*60;
     
  }
  ReadOn = EEPROM.read(1)+ee2on+ee3on;
  Serial.print(ReadOn);
  delay(0);

////////////////////////////////////////////////////////////////////////

  int ee12off = EEPROM.read(12);
  int ee13off = EEPROM.read(13);
  if(ee12off>0){
    ee12off=ee12off*60;
  }
  if(ee13off>0){
    ee13off=ee13off*60*60;
  }
  ReadOff = EEPROM.read(11)+ee13off+ee13off;
  Serial.print(ReadOff);


  //////////////////////////////////////////////////////////////////////////

    }
    break;
}while(count <= 2000 and read == HIGH  and y%2==0 );
  // y%2==0 && read == HIGH
int readtempon=ReadOn;
int readtempoff=ReadOff;
/////////////////////////////////////////////////////////
  static int8_t lastSecond = -1;
  ///////////////////////////////////////////////////////////
  
  ///////////////////////////////////////////////////////////
  rtc.update();
  dly++;

  if (dly > 200) {
    dly = 0;
    dl = !dl;
  }

  // Main loop
  if (md == 0) {
    // LCD time display
    lcd.setCursor(5, 0); 
    lcd.print(String(rtc.hour()));
    lcd.print(':');
    if (rtc.minute() < 10)
      lcd.print('0'); // Print leading '0' for minute
    lcd.print(String(rtc.minute())); // Print minute
    lcd.print(':');
    if (rtc.second() < 10)
      lcd.print('0'); // Print leading '0' for minute
    lcd.print(String(rtc.second())); // Print minute
    lcd.print(' ');

    if (rtc.is12Hour()) { // If we're in 12-hour mode
      if (rtc.pm()) 
        lcd.print("PM "); // Returns true if PM
      else 
        lcd.print("AM ");
    }
    
    // If menu button is pressed
    if (!digitalRead(menu) && md == 0) {
      st = 1;
      x++;
      if (x == 60) { // If delay time complete then go to menu loop
        md = 1;
        x = 0;
        lcd.clear();
      }
    }
     
    // Relay control
    if (am1 * 1000000 + h1 * 10000 + m1 * 100 + s1 <= (!rtc.pm()) * 1000000 + rtc.hour() * 10000 + rtc.minute() * 100 + rtc.second() && 
          am2 * 1000000 + h2 * 10000 + m2 * 100 + s2 >= (!rtc.pm()) * 1000000 + rtc.hour() * 10000 + rtc.minute() * 100 + rtc.second() ) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        time();
        lcd.setCursor(13, 1);
        lcd.print("ON ");
        ////////////////////////////////////
        if(digitalRead(button_start) == HIGH && y%2!=0 ){
          for(int i=0;readtempon>0 && y%2!=0 ;i++){
            readtempon--;
            delay(900);
            time();
            Serial.print(readtempon);
            Serial.print("y= "); Serial.print(y, DEC);
            Serial.print(   h2   );Serial.print("   h1   ");Serial.print(   m2   );Serial.print("   m2   ");
             if(digitalRead(button_start)==LOW){
              y++;
              lcd.clear();

          }
          }
          if(readtempon == 0){
            digitalWrite(11, HIGH);
            digitalWrite(10, HIGH);
            ////////////////////////////
            for(int j=0;readtempoff>0 && y%2!=0 ;j++){
              time();
              readtempoff--;
              delay(900);
              if(digitalRead(button_start)==LOW){
                y++;


              }
            }
          }
          if(readtempoff == 0){
            digitalWrite(11, LOW);
            digitalWrite(10, LOW);
  
          }
          readtempon = ReadOn ;
          readtempoff  = ReadOff ;
 //
          Serial.print("{{{{{");
          Serial.print( "sec is  = "); Serial.print(String(rtc.second())); 
  
          if(digitalRead(button_start)==LOW){
            y++;


          } 
   //     
        ///////////////////////////////////////
      }
        ///////////////////////////////////
    }
      
    else {

      lcd.setCursor(13, 1);
      lcd.print("OFF");
      time();

    }
  } 
  
  // Menu mode 
  if (!digitalRead(menu) && md != 0 && st == 0) {
    st = 1;
    if (sm == 0) {
      sm = 1;
    }
    if (sm != 0) {
      ssm++;
    }
  }
  
  if (!digitalRead(up) && md != 0 && st == 0) {
    st = 1;
    if (sm == 0) {
      md++;
      if (md > 3) md = 1;
    }
    if (ssm != 0) {
      su = 1;
    }
  }
  
  if (!digitalRead(down) && md != 0 && st == 0) {
    st = 1;
    if (sm == 0) {
      md--;
      if (md < 1) md = 3;
    }
    if (ssm != 0) {
      sd = 1;
    }
  }  

  if (digitalRead(menu) && digitalRead(up) && digitalRead(down) && st == 1) {
    st = 0;
  }

  // Option 1 in the menu    
  if (md == 1) {
    if (sm == 0) {
      lcd.setCursor(0, 0); 
      lcd.print("    TIME-SET    ");
      EEPROM.write(28, h);
      h = EEPROM.read(28);
      lcd.setCursor(0, 1); 
      lcd.print("   ");
      h = rtc.hour();
      lcd.print(String(h));
      
  //////////////////////////////////////
  
      lcd.print(':');
      m = rtc.minute();
      lcd.print(String(m));
      lcd.print(':');
      s = rtc.second();
      lcd.print(String(s));
      lcd.print(" ");
      if (rtc.is12Hour()) {
        if (rtc.pm()) 
          lcd.print("PM     "); 
        else 
          lcd.print("AM     ");
      }      
///////////////////////////////////////////
    }
    if (sm == 1) {
      if (ssm > 4) {
        ssm = 0;
        sm = 0;
        md = 1;
        rtc.setSecond(s);
        rtc.setMinute(m);
        // Adjustments to correctly set the hour in 12-hour format
      if (!am && h == 12) {
        rtc.setHour(0);  // 12 AM -> 00:00 in 24-hour format
      } else if (am && h == 12) {
        rtc.setHour(12);  // 12 PM -> 12:00 in 24-hour format
      } else if (am) {
        rtc.setHour(h);  // AM hours remain the same
      } else {
        rtc.setHour(h + 12);  // PM hours are converted to 24-hour format
      }
      rtc.set12Hour();
      lcd.clear();
        /////
      }
      if (su == 1) {
        su = 0;
        switch (ssm) {
          case 1:
            h++;
            if (h > 12) {
              h =1 ;

            }
            break;
          case 2:
            m++;
            if (m > 59) {
              m = 0;
            }
            break; 
          case 3:
            s++;
            if (s > 59) {
              s = 0;
            }
            break; 
          case 4:
            am = !am;
            break;                 
        }
      } 
      if (sd == 1) { 
        sd = 0;
        switch (ssm) {
          case 1:
            h--;
            if (h < 1) {
              h = 12;
            }
            break;
          case 2:
            m--;
            if (m == 255) {
              m = 59;
            }
            break; 
          case 3:
            s--;
            if (s == 255) {
              s = 59;
            }
            break;
          case 4:
            am = !am;
            break;             

        }
      } 
      
      lcd.setCursor(0, 0);
      lcd.print("  SETTING-TIME  ");

      lcd.setCursor(0, 1); 
      if (am) {
        lcd.print("TIME:" + String(h) + ":" + String(m) + ":" + String(s) + " " + "AM  ");
      }  else {
        lcd.print("TIME:" + String(h) + ":" + String(m) + ":" + String(s) + " " + "PM  ");
        }
            EEPROM.write(28, h);
            h = EEPROM.read(28);
            Serial.print(h);
    }
  }

  // Option 2 in the menu    
  if (md == 2) {
    if (sm == 0) {
      lcd.setCursor(0, 0); 
      lcd.print("  RELAY-ON-TIME  ");
      lcd.setCursor(0, 1); 
      if (!dl) {
        lcd.print("FROM:");
        lcd.print(String(h1));
        lcd.print(":");
        lcd.print(String(m1));
        lcd.print(":");
        lcd.print(String(s1));
        if (am1) 
          lcd.print(" AM    "); 
        else 
          lcd.print(" PM    ");
      } else {
        lcd.print("--TO:");
        lcd.print(String(h2));
        lcd.print(":");
        lcd.print(String(m2));
        lcd.print(":");
        lcd.print(String(s2));
        if (am2) 
          lcd.print(" AM    "); 
        else 
          lcd.print(" PM    ");
      }
    }
    if (sm == 1) {
      if (ssm > 8) {
        ssm = 0;
        sm = 0;
        md = 2;
        /*
        EEPROM.write(0, s1);
        EEPROM.write(1, m1);
        EEPROM.write(2, h1);
        EEPROM.write(3, am1);
        EEPROM.write(4, s2);
        EEPROM.write(5, m2);
        EEPROM.write(6, h2);
        EEPROM.write(7, am2);
        */
        lcd.clear();
      }
      if (su == 1) {
        su = 0;
        switch (ssm) {
          case 1:
            h1++;
            if (h1 > 12) {
              h1 = 1;
            }
            break;
          case 2:
            m1++;
            if (m1 > 59) {
              m1 = 0;
            }
            break; 
          case 3:
            s1++;
            if (s1 > 59) {
              s1 = 0;
            }
            break; 
          case 4:
            am1 = !am1;
            break;          
          case 5:
            h2++;
            if (h2 > 12) {
              h2 = 1;
            }
            break;
          case 6:
            m2++;
            if (m2 > 59) {
              m2 = 0;
            }
            break; 
          case 7:
            s2++;
            if (s2 > 59) {
              s2 = 0;
            }
            break; 
          case 8:
            am2 = !am2;
            break;   
        }
      } 
      if (sd == 1) { 
        sd = 0;
        switch (ssm) {
          case 1:
            h1--;
            if (h1 < 1) {
              h1 = 12;
            }
            break;
          case 2:
            m1--;
            if (m1 == 255) {
              m1 = 59;
            }
            break; 
          case 3:
            s1--;
            if (s1 == 255) {
              s1 = 59;
            }
            break; 
          case 4:
            am1 = !am1;
            break; 
          case 5:
            h2--;
            if (h2 < 1) {
              h2 = 12;
            }
            break;
          case 6:
            m2--;
            if (m2 == 255) {
              m2 = 59;
            }
            break; 
          case 7:
            s2--;
            if (s2 == 255) {
              s2 = 59;
            }
            break; 
          case 8:
            am2 = !am2;
            break;             
        }
      } 
      
      lcd.setCursor(0, 0);
      if (am1 == 1) {
        lcd.print("FROM:"+String(h1)+":"+String(m1)+":"+String(s1)+" "+"AM  ");
      } else {
        lcd.print("FROM:"+String(h1)+":"+String(m1)+":"+String(s1)+" "+"PM  ");
      }
  
      lcd.setCursor(0, 1); 
      if (am2 == 1) {
        lcd.print("  TO: "+String(h2)+":"+String(m2)+":"+String(s2)+" "+"AM  ");
      } else {
        lcd.print("  TO: "+String(h2)+":"+String(m2)+":"+String(s2)+" "+"PM  ");
      }
    }
  }
  
  // Exit
  if (md == 3) {
        EEPROM.write(20, s1);
        EEPROM.write(21, m1);
        EEPROM.write(22, h1);
        EEPROM.write(23, am1);
        EEPROM.write(24, s2);
        EEPROM.write(25, m2);
        EEPROM.write(26, h2);
        EEPROM.write(27, am2);
        
    lcd.setCursor(0, 0); 
    
    lcd.print("      EXIT      ");
    lcd.setCursor(0, 1); 
    lcd.print("                ");
    if (sm == 1) {
      sm = 0;
      md = 0;
      lcd.clear();
      // Standard display
      lcd.print("TIME:12:00:00 AM");
      lcd.setCursor(0, 1); 
      lcd.print("OUTPUT RELAY:OFF");
      
    }
  }
  //////////////////////////////////////////////
  
}
///////////////////////////////////////////
void print_time() {
  time_s = time_s - 1;
  if (time_s < 0) {
    time_s = 59;
    time_m = time_m - 1;
  }
  if (time_m < 0) {
    time_m = 59;
    time_h = time_h - 1;
  }
}

void write_on_time() {
  EEPROM.write(1, time_s);
  EEPROM.write(2, time_m);
  EEPROM.write(3, time_h);
}

void write_of_time() { 
  EEPROM.write(11, time_s);
  EEPROM.write(12, time_m);
  EEPROM.write(13, time_h);
}

void read_on_time() {
  time_s = EEPROM.read(1);
  time_m = EEPROM.read(2);
  time_h = EEPROM.read(3);
}

void read_of_time() {
  time_s = EEPROM.read(11);
  time_m = EEPROM.read(12);
  time_h = EEPROM.read(13);
}

//////////////////////////////////////////

void time(){
    rtc.update();

  lcd.setCursor(5, 0); 
    lcd.print(String(rtc.hour()));
    lcd.print(':');
    if (rtc.minute() < 10)
      lcd.print('0'); // Print leading '0' for minute
    lcd.print(String(rtc.minute())); // Print minute
    lcd.print(':');
    if (rtc.second() < 10)
      lcd.print('0'); // Print leading '0' for minute
    lcd.print(String(rtc.second())); // Print minute
    lcd.print(' ');

    if (rtc.is12Hour()) { // If we're in 12-hour mode
      if (rtc.pm())
        lcd.print("PM "); // Returns true if PM
      else
        lcd.print("AM ");
    }
    
    // If menu button is pressed
    if (!digitalRead(menu) && md == 0) {
      st = 1;
      x++;
      if (x == 60) { // If delay time complete then go to menu loop
        md = 1;
        x = 0;
        lcd.clear();
      }
    }
}
