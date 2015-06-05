
// void handle_serial() {
//   if(!Serial.available()) {
//     return;
//   }
//   char command = Serial.read();

//   if (command == 'x') {
//     ts_min_x = Serial.parseInt();
//   } else if (command == 'X') {
//     ts_max_x = Serial.parseInt();
//   } else if (command == 'y') {
//     ts_min_y = Serial.parseInt();
//   } else if (command == 'Y') {
//     ts_max_y = Serial.parseInt();
//   }
// }

// uint16_t rgb(byte red, byte green, byte blue) {
//   return (((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255);
// }

// void setup(void) {
//   Serial.begin(9600);

//   tft.reset();
//   uint16_t identifier = tft.readID();
//   tft.begin(identifier);

//   // Fill with black
//   tft.setRotation(90);
//   tft.fillScreen(rgb(0, 0, 0));

//   tft.setCursor(50, 100);
//   tft.setTextColor(rgb(255, 255, 255));
//   tft.setTextSize(1);
//   tft.print("hello");

//   pinMode(13, OUTPUT);
// }

// #define MINPRESSURE 10
// #define MAXPRESSURE 1000

// void loop()
// {
//   handle_serial();

//   digitalWrite(13, HIGH);
//   TSPoint p = ts.getPoint();
//   digitalWrite(13, LOW);

//   pinMode(xm, OUTPUT);
//   pinMode(yp, OUTPUT);
// //  tft.fillRect(0, 0, 240, 320, rgb(random(255), random(255), random(255)));

//   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
//     // scale from 0->1023 to tft.width
//     p.x = map(p.x, ts_min_x, ts_max_x, tft.width(), 0);
//     p.y = map(p.y, ts_min_y, ts_max_y, tft.height(), 0);

//     if ((p.y) < tft.height()) {
//       tft.fillCircle(p.x, p.y, 3, rgb(0, 0, 255));
//     }
//   }
// }
