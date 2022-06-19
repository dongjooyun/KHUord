#include <Vector.h>

// Setting I/O
const int SW_C1 = 2;  // 1행 스위치
const int SW_C2 = 3;  // 2행 스위치
const int SW_C3 = 4;  // 3행 스위치
const int SW_R1 = A0;  // 1열 스위치 A0
const int SW_R2 = A1;  // 2열 스위치 A1
const int SW_R3 = A2;  // 3열 스위치 A2
const int SW_R4 = A3;  // 4열 스위치 A3
const int SW_R5 = A4;  // 5열 스위치 A4
const int SW_R6 = A5;  // 6열 스위치 A5
int SWPinCol[3]={SW_C1, SW_C2, SW_C3};
int SWPinRow[6]={SW_R1, SW_R2, SW_R3, SW_R4, SW_R5, SW_R6};

const int VM_C1 = 7;  // 1행 진동모터
const int VM_C2 = 6;  // 2행 진동모터 
const int VM_C3 = 5;  // 3행 진동모터
const int VM_R1 = 8;  // 1열 진동모터
const int VM_R2 = 9;  // 2열 진동모터
const int VM_R3 = 10;  // 3열 진동모터
const int VM_R4 = 11;  // 4열 진동모터
const int VM_R5 = 12;  // 5열 진동모터
const int VM_R6 = 13;  // 6열 진동모터
int VMPinCol[3]={VM_C1, VM_C2, VM_C3};
int VMPinRow[6]={VM_R1, VM_R2, VM_R3, VM_R4, VM_R5, VM_R6};

int value;

int aChordA[] = {8, 9, 10};
int aChordC[] = {4, 8, 13};
int aChordD[] = {9, 11, 16};
int aChordE[] = {3, 7, 8};
int aChordG_3[] = {7, 12, 17};
int aChordG_2_1[] = {7, 12};
int aChordG_2_2[] = {7, 17};
int aChordG_2_3[] = {12, 17};
int aChordG_1_1[] = {7};
int aChordG_1_2[] = {12};
int aChordG_1_3[] = {17};

const int ELEMENT_COUNT_MAX = 3;
typedef Vector<int> Elements;

void setup() {
  // Setting I/O PinMode
  for (int i = 0; i < 3; i++)
  {
    pinMode(SWPinCol[i],OUTPUT);
    digitalWrite(SWPinCol[i], LOW);
    pinMode(VMPinCol[i],OUTPUT);
    digitalWrite(VMPinCol[i], LOW);
  }

  for (int i = 0; i < 6; i++)
  {
    pinMode(VMPinRow[i],OUTPUT);
    digitalWrite(VMPinRow[i], HIGH);
  }

  // Setting serial monitor
  Serial.begin(115200); 
  while(!Serial); // Waiting for console
  delay(1000); 
  Serial.println("Get started ...");
}

void loop() {
  int v;
  
  // Motor test
//  vmotor(17,true);
//  delay(500);
//  vmotor(17,false);

  // Switch test
  printSwitchIndex();
}

// ON → Nth index motor on
// OFF → Nth index motor off
void vmotor(int num, boolean on_off)
{
  int col, row;
  
  col = num / 6;
  row = num % 6;
  if (on_off)
  {
     digitalWrite(VMPinCol[col], HIGH);
     digitalWrite(VMPinRow[row], LOW);
  }
  else
  {
     digitalWrite(VMPinCol[col], LOW);
     digitalWrite(VMPinRow[row], HIGH);
  }
}

void printSwitchIndex()
{
  int num, key; 
  int storage_array[ELEMENT_COUNT_MAX];
  Elements vec;
  vec.setStorage(storage_array);
  
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(SWPinCol[i], HIGH);
    num = i * 6;
    for (int j = 0; j < 6; j++)
    {
      if (analogRead(SWPinRow[j]) > 1000)
      {
        key = num + j; 
        Serial.println(key);
        for (int i=0; i<3; i++)
        {
          vec.push_back(key);
        }
      }
    }
    digitalWrite(SWPinCol[i], LOW);  
    delay(100);  
  }
  for (int elem : vec)
  {
    if (IsArrIncludeElem(aChordG_3, elem) == false)
    {
      vmotor(elem, true);
      delay(300);
      vmotor(elem, false);
    }
    else
    {
      if (IsArrIncludeElem(aChordG_2_1, elem) == false || IsArrIncludeElem(aChordG_2_2, elem) == false || IsArrIncludeElem(aChordG_2_3, elem) == false)
      {
        vmotor(elem, true);
        delay(300);
        vmotor(elem, false);
      }
      else
      {
        if (IsArrIncludeElem(aChordG_1_1, elem) == false || IsArrIncludeElem(aChordG_1_2, elem) == false || IsArrIncludeElem(aChordG_1_3, elem) == false)
        {
          vmotor(elem, true);
          delay(300);
          vmotor(elem, false);
        }
        else
        {
          Serial.println('G');  
        }
      }
    }
  }
}

boolean IsArrIncludeElem(int arr[], int elem){
  for (int i = 0; i <= sizeof(arr); i++) { 
    if (arr[i] == elem){
      return true;
    }
  }
  return false;
}
