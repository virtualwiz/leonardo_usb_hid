#include <LiquidCrystal.h>
#include <Mouse.h>
#include <Keyboard.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define IDLE 0

#define MOUSE_LEFTCLICK 1
#define MOUSE_RIGHTCLICK 2
#define MOUSE_DOUBLECLICK 3
#define KEY_CUSTOM 4

#define KEY_MAP_SIZE 28

int adc_key_in;
char mode = 1;
int interval = 1000;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const char keyMap_Ascii[28]  = 
{
	0x00,0x80,0x81,0x82,0x83,
	0xDA,0xD9,0xD8,0xD7,
	0xB2,0xB3,0xB0,0xB1,
	' ',0xD4,0xC1,
	0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD
};

const String keyMap_Name[28]  = 
{
	"NULL","CTRL","SHFT","ALT ","WIN ",
	"UP  ","DOWN","LEFT","RGHT",
	"BKSP","TAB ","RTRN","ESC ",
	"SPAC","DELE","CAPS",
	"F1  ","F2  ","F3  ","F4  ","F5  ","F6  ","F7  ","F8  ","F9  ","F10 ","F11 ","F12 "
};

unsigned char Key_GetKeyCode(int delaytime)
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   {
    delay(delaytime);
    return btnRIGHT;
  }
  if (adc_key_in < 250)  {
    delay(delaytime);
    return btnUP;
  }
  if (adc_key_in < 450)  {
    delay(delaytime);
    return btnDOWN;
  }
  if (adc_key_in < 650)  {
    delay(delaytime);
    return btnLEFT;
  }
  if (adc_key_in < 850)  {
    delay(delaytime);
    return btnSELECT;
  }
}

void LCD_ClearLine(char line)
{
  lcd.setCursor(0, line);
  lcd.print("                ");
}

int keyDownTime = 100;
long int keyUpTime = 1000;

void SetupInterval()
{
  char refresh_flag = 1;
  char confirm_flag = 0;
  int currentValue;

  LCD_ClearLine(1);
  LCD_ClearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("KEY DOWN TIME?");
  lcd.setCursor(0, 1);
  lcd.print("Dn     Up       ");

  while (!confirm_flag)
  {
    switch (Key_GetKeyCode(200))
    {
      case btnDOWN: keyDownTime -= 100; refresh_flag = 1; break;
      case btnUP: keyDownTime += 100; refresh_flag = 1; break;
      case btnSELECT: confirm_flag = 1; break;
    }
    if (keyDownTime < 0)	keyDownTime = 0;
    if (refresh_flag)
    {
      lcd.setCursor(3, 1);
      if (keyDownTime != 0)
      {
        lcd.print(keyDownTime);
        lcd.print(" ");
      }
      else
        lcd.print("RAN ");
      refresh_flag = 0;
    }
  }
  confirm_flag = 0;
  refresh_flag = 1;
  LCD_ClearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("KEY UP TIME?");
  while (!confirm_flag)
  {
    switch (Key_GetKeyCode(50))
    {
      case btnDOWN: keyUpTime -= 200; refresh_flag = 1; break;
      case btnUP: keyUpTime += 200; refresh_flag = 1; break;
      case btnSELECT: confirm_flag = 1; break;
    }
    if (keyUpTime < 0)	keyUpTime = 0;
    if (refresh_flag)
    {
      lcd.setCursor(10, 1);
      if (keyUpTime != 0)
      {
        lcd.print(keyUpTime);
        lcd.print(" ");
      }
      else
        lcd.print("RAN ");
      refresh_flag = 0;
    }
  }
}

int ranLower = 100;
long int ranUpper = 1000;

void SetupRandomTimeRange()
{
  char refresh_flag = 1;
  char confirm_flag = 0;
  int currentValue;

  LCD_ClearLine(1);
  LCD_ClearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("RANDOM LOWER?");
  lcd.setCursor(0, 1);
  lcd.print("Lo     Hi       ");

  while (!confirm_flag)
  {
    switch (Key_GetKeyCode(200))
    {
      case btnDOWN: ranLower -= 100; refresh_flag = 1; break;
      case btnUP: ranLower += 100; refresh_flag = 1; break;
      case btnSELECT: confirm_flag = 1; break;
    }
    if (ranLower < 0)	ranLower = 0;
    if (refresh_flag)
    {
      lcd.setCursor(3, 1);
      lcd.print(ranLower);
      lcd.print(" ");
      refresh_flag = 0;
    }
  }
  confirm_flag = 0;
  refresh_flag = 1;
  LCD_ClearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("RANDOM UPPER?");
  while (!confirm_flag)
  {
    switch (Key_GetKeyCode(50))
    {
      case btnDOWN: ranUpper -= 200; refresh_flag = 1; break;
      case btnUP: ranUpper += 200; refresh_flag = 1; break;
      case btnSELECT: confirm_flag = 1; break;
    }
    if (ranUpper < ranLower)	ranUpper = ranLower;
    if (refresh_flag)
    {
      lcd.setCursor(10, 1);
      if (ranUpper != 0)
        lcd.print(ranUpper);
      lcd.print(" ");
      refresh_flag = 0;
    }
  }
}

void Mouse_Click(byte key)
{
  unsigned long int count = 0;
  SetupInterval();
  delay(500);
  if (keyUpTime == 0)
    SetupRandomTimeRange();
  while (1)
  {
    LCD_ClearLine(0);
    lcd.setCursor(0, 0);
    lcd.print(count++);
    lcd.print(" Hits!");
    switch (key)
    {
      case MOUSE_LEFTCLICK:
        {
          Mouse.press(MOUSE_LEFT);
          if (keyDownTime != 0) delay(keyDownTime);
          else delay(random(10, 100));
          Mouse.release(MOUSE_LEFT);
          if (keyUpTime != 0) delay(keyUpTime);
          else delay(random(ranLower, ranUpper));
        } break;

      case MOUSE_RIGHTCLICK:
        {
          Mouse.press(MOUSE_RIGHT);
          if (keyDownTime != 0) delay(keyDownTime);
          else delay(random(10, 100));
          Mouse.release(MOUSE_RIGHT);
          if (keyUpTime != 0) delay(keyUpTime);
          else delay(random(ranLower, ranUpper));
        } break;

      case MOUSE_DOUBLECLICK:
        {
          Mouse.press(MOUSE_LEFT);
          delay(50);
          Mouse.release(MOUSE_LEFT);
          delay(50);
          Mouse.press(MOUSE_LEFT);
          delay(50);
          Mouse.release(MOUSE_LEFT);
          if (keyUpTime != 0) delay(keyUpTime);
          else delay(random(ranLower, ranUpper));
        } break;
    }
  }
}

void Keyboard_Type()
{
  unsigned long int count = 0;
  char refresh_flag = 1;
  char confirm_flag = 0;
  byte keyCode[3] = {0,0,0};
  char selectedKey = 0;
  SetupInterval();
  LCD_ClearLine(0);
  LCD_ClearLine(1);
  lcd.setCursor(0,0);
  lcd.print(" K1   K2   K3    ");
  delay(500);
  while (!confirm_flag)
  {
    switch (Key_GetKeyCode(150))
    {
	  case btnLEFT: selectedKey--;	 refresh_flag = 1; break;
	  case btnRIGHT: selectedKey++;	 refresh_flag = 1; break;
      case btnSELECT: confirm_flag = 1; break;
	  case btnDOWN: keyCode[selectedKey]--; refresh_flag = 1; break;
      case btnUP: keyCode[selectedKey]++; refresh_flag = 1; break;
    }

    if (selectedKey < 0)	selectedKey=2;
	if (selectedKey > 2)	selectedKey=0;	
	
    if (refresh_flag)
    {
	  lcd.setCursor(selectedKey*5+1,1);
	  switch(selectedKey)
	  {
	      case 2:
		  {
			  if (keyCode[selectedKey] <= 0)	keyCode[selectedKey]=0;
			  if (keyCode[selectedKey] >= 0xFF)	keyCode[selectedKey]=0xFF;
			  lcd.print("0x");lcd.print(keyCode[selectedKey],HEX);lcd.print(" ");break;
		  }
		  case 0:
		  case 1:
		  {
			if (keyCode[selectedKey] <= 0)	keyCode[selectedKey]=0;
			if (keyCode[selectedKey] > KEY_MAP_SIZE-1)	keyCode[selectedKey]=KEY_MAP_SIZE-1;
			lcd.print(keyMap_Name[keyCode[selectedKey]]);
		  }break;
	  }
      switch(selectedKey)
	  {
		  case 0:lcd.setCursor(0,1);break;
		  case 1:lcd.setCursor(5,1);break;
		  case 2:lcd.setCursor(10,1);break;
	  }
    }
	refresh_flag = 0;
  }
  LCD_ClearLine(0);
  while (1)
  {
    LCD_ClearLine(0);
    lcd.setCursor(0, 0);
    lcd.print(count++);
    lcd.print(" Hits!");
	
	Keyboard.press(keyMap_Ascii[keyCode[0]]);
	Keyboard.press(keyMap_Ascii[keyCode[1]]);
	Keyboard.press(keyCode[2]);
	if (keyDownTime != 0) delay(keyDownTime);
    else delay(random(10, 100));
	Keyboard.releaseAll();
    if (keyUpTime != 0) delay(keyUpTime);
    else delay(random(ranLower, ranUpper));	
  }
}

void Select_Operation()
{
  char refresh_flag = 1;
  char confirm_flag = 0;
  LCD_ClearLine(0);
  LCD_ClearLine(1);
  lcd.setCursor(0, 0);
  lcd.print("MODE?");
  lcd.setCursor(0, 1);
  while (!confirm_flag)
  {
    switch (Key_GetKeyCode(200))
    {
      case btnDOWN: mode++; refresh_flag = 1; break;
      case btnUP: mode--; refresh_flag = 1; break;
      case btnSELECT: confirm_flag = 1; break;
    }
    if (mode < MOUSE_LEFTCLICK)	mode = KEY_CUSTOM;
    if (mode > KEY_CUSTOM)	mode = MOUSE_LEFTCLICK;
    if (refresh_flag)
    {
      LCD_ClearLine(1); lcd.setCursor(0, 1);
      switch (mode)
      {
        case MOUSE_LEFTCLICK:
          lcd.print("1.Mouse Btn L"); break;
        case MOUSE_RIGHTCLICK:
          lcd.print("2.Mouse Btn R"); break;
        case MOUSE_DOUBLECLICK:
          lcd.print("3.Double Btn L"); break;
        case KEY_CUSTOM:
          lcd.print("4.ASCII Key"); break;
      }
      refresh_flag = 0;
    }
  }
  switch (mode)
  {
    case MOUSE_LEFTCLICK:
      Mouse_Click(MOUSE_LEFTCLICK); break;
    case MOUSE_RIGHTCLICK:
      Mouse_Click(MOUSE_RIGHTCLICK); break;
    case MOUSE_DOUBLECLICK:
      Mouse_Click(MOUSE_DOUBLECLICK); break;
    case KEY_CUSTOM:
      Keyboard_Type(); break;
  }
}

void setup()
{
  randomSeed(analogRead(A5));
  pinMode(13, OUTPUT);
  Mouse.begin();
  Keyboard.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.blink();
  lcd.print("USB KEY REPEATER");
  lcd.setCursor(0, 1);
  lcd.print("    DFROBOT     ");
  delay(2000);
  Select_Operation();
}

void loop()
{

}
