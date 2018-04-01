int sensors[8] = {6, 8, -1, -1, -1, -1, -1, -1};

#define uchar unsigned char

//FIND MINIMUM INDEX
int minIndex(uchar *arr) {
    int ind = -1;
    int m = 255;
    for (int i = 0; i < 15; i += 2) {
        int a = arr[i];
        if (a < m) {
            m = a;
            ind = i;
        }
    }
    return ind;
}

// Function to convert from centimeters to steps
int CMtoSteps(float cm) {

/*  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14); // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step  (cm/20steps)

  float f_result = cm / cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)

  return result;  // End and return result
  */
    return 3.333 * cm;


}

bool outOfBounds(){
  bool b=true;
   for (int i = 0; i < 8; i++) {
        if (-1 != sensors[i])
            b=false;
    }
    return b;
}

//which sensor detect black
bool isActive(int val) {
    for (int i = 0; i < 8; i++) {
        if (val == sensors[i])
            return true;
    }
    return false;
}

void printSensorLogOdd(uchar *data) {
      Serial.print("data[1]:");
     Serial.println(data[1]);
     Serial.print("data[3]:");
     Serial.println(data[3]);
     Serial.print("data[5]:");
     Serial.println(data[5]);
     Serial.print("data[7]:");
     Serial.println(data[7]);
     Serial.print("data[9]:");
     Serial.println(data[9]);
     Serial.print("data[11]:");
     Serial.println(data[11]);
     Serial.print("data[13]:");
     Serial.println(data[13]);
     Serial.print("data[15]:");
     Serial.println(data[15]);
          Serial.println("");

     delay(1000);

}

void printSensorLog(uchar *data) {
      Serial.print("data[0]:");
     Serial.println(data[0]);
     Serial.print("data[2]:");
     Serial.println(data[2]);
     Serial.print("data[4]:");
     Serial.println(data[4]);
     Serial.print("data[6]:");
     Serial.println(data[6]);
     Serial.print("data[8]:");
     Serial.println(data[8]);
     Serial.print("data[10]:");
     Serial.println(data[10]);
     Serial.print("data[12]:");
     Serial.println(data[12]);
     Serial.print("data[14]:");
     Serial.println(data[14]);
     Serial.println("");
     delay(1000);
}

bool isLine(){
    bool b=false;
    for(int i=0;i<8;i++){
        if(sensors[i] != -1){
            b=true;
        }
    }
    return b;
}
