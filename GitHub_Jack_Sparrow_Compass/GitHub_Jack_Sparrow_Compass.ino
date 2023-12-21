// Project Created for the HAN University Of Applied Sciences elective project "Vrij Project" by Daan Bohlander for education of BA Mechanical Engineering.
// Publication Version 1; 21-12-2023


//==================================================================[ Global ][ 1 ][ GPS Module ]

#include "TinyGPS++.h"
TinyGPSPlus gps;
int validFixCount = 0;                                       // Counter for valid fixes
double latitude, longitude;
//double latitude = 51.98904037475586;                         //HAN
//double longitude = 5.949984073638916;

bool gotFix = false;                                         // Flag to indicate whether we've got a valid GPS fix
float degree;
int InvalidCounter =0;

void GPSFix(){                                               // - VOID FUNCTIE 1 -
  while (!gotFix) {                                          // Boolean voorwaarde, GPS interpretatie stopt als TRUE
    while (Serial1.available() > 0) {                        // Checkt of signaal van GPS-transmission binnen komt
      if (gps.encode(Serial1.read())) {                      // Start interpretatie wanneer signaal gelezen kan worden
        if (gps.location.isValid()) {                        // Checkt of gelezen signaal correct is
          validFixCount++;                                   // Telt hoevaak het signaal geïnterpreteerd wordt
          
          // Wait until we get 50 valid fixes for better accuracy
          if (validFixCount >= 50) {                         // Als 50 gps signalen binnen zijn start operatie
            latitude = gps.location.lat();                   // Sla 50e gps Latitude op
            longitude = gps.location.lng();                  // Sla 50e gps Longitude op

            Serial.print("==========["); 
            Serial.print(InvalidCounter);                    // Print hoevaak signaal wordt doorgestuurd zonder GPS
            Serial.println("]==========");
            Serial.print("Fix Count: ");
            Serial.println(validFixCount);
            Serial.println(" GPS Coordinate found: ");
            Serial.print("Latitude: ");             
            Serial.println(latitude, 8);
            Serial.print("Longitude: ");
            Serial.println(longitude, 8);
            Serial.println(" Searching Library ");
            
            gotFix = true;                                   // Gewild GPS coordinaat is opgeslagen, einde operatie
          }
        } else {                                             // Print dat signaal binnenkomt maar geen coordinaat is gevonden
           Serial.print("==========["); 
           Serial.print(InvalidCounter);                     // Print hoevaak signaal wordt doorgestuurd zonder GPS
           Serial.println("]==========");
           Serial.println("Location data is not valid.");
           Serial.print("Fix Count:  ");
           Serial.println(validFixCount);
            InvalidCounter++;                                // Telt aantal keren invald voordat coordinaat is gevonden
            if (InvalidCounter > 1000) {InvalidCounter=0;}   // Reset teller zodat getal niet te groot wordt
            } } } } }
  

//=================================================================[ Global ][ 2 ][ Hall sensor ]

int hallsensorPin = 2;
int state = 0;

//=======================================================================[ Global ][ 3 ][ Motor ]

#include <Stepper.h>
const int stepsPerRevolution = 2048;
const int motorPin1 = 6;
const int motorPin2 = 4;
const int motorPin3 = 5;
const int motorPin4 = 3;
Stepper MacMotor = Stepper(stepsPerRevolution, motorPin1, motorPin2, motorPin3, motorPin4);
int StepCalibrate = 10;
float Step;
float angle;
float rotation;
                                                             
void MotorCalibrate(){                                       // - VOID FUNCTIE 2 -
  state = digitalRead(hallsensorPin);
  while(state == HIGH){
    state = digitalRead(hallsensorPin);
    MacMotor.step(StepCalibrate);
  }
  turnOffMotorPins(); }
  
void turnOffMotorPins (){                                    // - VOID FUNCTIE 3 -
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW); }

//================================================================[ Global ][ 4 ][ Magnetometer ]

#include <QMC5883L.h>
const float Pi = 3.14159;
int x, y, z;
QMC5883L compass;
int Noord = 0;
int MagCount = 0;
float headingRadians;
float BaseHeading;
float UpdatedHeading;
float DeltaHeading;
int Updates = 0;

void ReadHeading(){                                          // - VOID FUNCTIE 4 -
    while(Noord == LOW){
    compass.read(&x, &y, &z); 
    headingRadians = atan2(y, x);
    MagCount++;
    if(MagCount >= 20){
    Serial.print("MagCount: ");
    Serial.println(MagCount);
    Serial.print("x: ");
    Serial.print(x);
    Serial.print("    y: ");
    Serial.print(y);
    Serial.print("    z: ");
    Serial.println(z);
    Serial.println();
    Serial.print("Orientation angle from North: ");
    Serial.println(headingRadians);
    Serial.println();
    Noord = HIGH;
    BaseHeading = headingRadians;
    }  } }

//=================================================================[ Global ][ 5 ][ Coordinaten ]

bool MacCord = false;                                        // Boolean, zet de for-loop uit
bool PrintMacCord = false;                                   // Boolean hulpmiddel om de serial print aan het einde te volbrengen
float Afstand1 = 10;                                         // Afstand startpunt om 'lager-dan' te laten werken
float Afstand2;                                              // Afstand variabele die wordt berekend
float DeltaLat;                                              // Verschil tussen input Lat en array [i][0]
float DeltaLng;                                              // Verschil tussen input Lng en array [i][1]
float ResultLat;                                             // Resulterend Lat-coordinaat
float ResultLng;                                             // Resulterend Lng-coordinaat

float MCD[258][2]{
{  52.1145918  , 4.2826052 } , //  1 Netherlands:
{ 52.9585365  , 5.8117158 } , //  2
{ 52.4155623  , 4.9116305 } , //  3
{ 53.1793201  , 5.7971201 } , //  4
{ 53.2019403  , 5.7692001 } , //  5
{ 51.5373551  , 5.0420495 } , //  6
{ 51.6046391  , 5.5245181 } , //  7
{ 51.3761455  , 5.9851273 } , //  8
{ 52.3399661  , 6.6423938 } , //  9
{ 51.572464 , 4.6680097 } , //  10
              
{ 51.5351483  , 4.4845812 } , //  11
{ 52.0493478  , 5.1180859 } , //  12
{ 52.355041 , 4.8900013 } , //  13
{ 51.9474387  , 4.5551244 } , //  14
{ 52.6300761  , 4.7478263 } , //  15
{ 52.3943378  , 5.2714742 } , //  16
{ 52.3719847  , 5.2207667 } , //  17
{ 52.3552354  , 5.2110933 } , //  18
{ 52.1539518  , 5.3852228 } , //  19
{ 52.1949326  , 5.3963577 } , //  20
              
{ 52.3019509  , 4.8636224 } , //  21
{ 52.3149986  , 4.9535104 } , //  22
{ 52.2123728  , 5.9647267 } , //  23
{ 52.2354428  , 5.9752157 } , //  24
{ 52.1793041  , 5.9853354 } , //  25
{ 51.9841034  , 5.9027835 } , //  26
{ 51.9645778  , 5.89121 } , //  27
{ 51.9756586  , 5.9600265 } , //  28
{ 51.8702363  , 4.548311  } , //  29
{ 52.1628687  , 4.4851447 } , //  30
              
{ 51.9183004  , 4.4800853 } , //  31
{ 51.58236  , 4.73042 } , //  32
{ 52.4000378  , 4.9357141 } , //  33
{ 52.0786889  , 4.3111223 } , //  34
{ 52.2756826  , 5.1624251 } , //  35
{ 52.3584968  , 4.6551378 } , //  36
{ 51.8684839  , 4.4535874 } , //  37
{ 50.8565019  , 5.7174629 } , //  38
{ 52.0104606  , 4.3606981 } , //  39
{ 52.0266492  , 4.3624455 } , //  40
              
{ 51.6901869  , 5.3019447 } , //  41
{ 51.6853782  , 5.2867599 } , //  42
{ 52.2496793  , 6.2042835 } , //  43
{ 52.3325747  , 4.9788642 } , //  44
{ 52.159979 , 4.4915151 } , //  45
{ 51.813205 , 4.6669017 } , //  46
{ 52.0435067  , 5.6700387 } , //  47
{ 52.3632682  , 4.9303705 } , //  48
{ 51.4361576  , 5.4221954 } , //  49
{ 51.8939402  , 4.5092961 } , //  50
              
{ 52.3792223  , 4.6334583 } , //  51
{ 52.0114433  , 4.3165065 } , //  52
{ 50.8882198  , 50.8882198  } , //  53
{ 52.287043 , 6.7644416 } , //  54
{ 52.2416731  , 6.7635481 } , //  55
{ 53.216438 , 6.5679923 } , //  56
{ 52.225612 , 5.1765435 } , //  57
{ 52.2210592  , 5.1443303 } , //  58
{ 53.2136636  , 6.5405484 } , //  59
{ 51.9522164  , 4.5653766 } , //  60
              
{ 51.9204007  , 4.4835609 } , //  61
{ 52.2188669  , 6.8988016 } , //  62
{ 52.0551877  , 4.5229085 } , //  63
{ 52.365952 , 4.8669798 } , //  64
{ 51.9218994  , 4.4756512 } , //  65
{ 52.1125051  , 4.2835357 } , //  66
{ 52.0934191  , 5.1160152 } , //  67
{ 53.1995176  , 5.797018  } , //  68
{ 52.1668818  , 4.4462573 } , //  69
{ 52.1038508  , 5.0600818 } , //  70
              
{ 52.0903253  , 4.3843012 } , //  71
{ 52.3645956  , 4.8837866 } , //  72
{ 51.4397644  , 5.4781604 } , //  73
{ 52.3000801  , 4.9447282 } , //  74
{ 52.0475665  , 5.0870295 } , //  75
{ 52.3772607  , 4.8964479 } , //  76
{ 51.8433925  , 5.8613354 } , //  77
{ 51.8254841  , 5.8039986 } , //  78
{ 52.3557352  , 4.7728089 } , //  79
{ 52.3596082  , 4.8055245 } , //  80
              
{ 51.9176207  , 4.4751265 } , //  81
{ 52.0919678  , 5.1173611 } , //  82
{ 52.1144714  , 5.1111269 } , //  83
{ 51.9107747  , 4.4348797 } , //  84
{ 51.9255913  , 4.5661878 } , //  85
{ 52.0359731  , 4.3162202 } , //  86
{ 52.1420811  , 4.4829784 } , //  87
{ 52.4256994  , 4.6467648 } , //  88
{ 51.9101694  , 4.4454924 } , //  89
{ 53.2162861  , 6.5841147 } , //  90
              
{ 51.9261496  , 4.4154681 } , //  91
{ 52.3376366  , 4.9214733 } , //  92
{ 51.8948512  , 4.5315545 } , //  93
{ 52.0611402  , 4.4876315 } , //  94
{ 51.5558334  , 5.09073 } , //  95
{ 52.0784678  , 4.3199241 } , //  96
{ 51.3704481  , 6.16942 } , //  97
{ 51.9173391  , 4.3323346 } , //  98
{ 52.0763421  , 4.3094632 } , //  99
{ 52.0753557  , 4.359474  } , //  100
              
{ 50.8499782  , 5.6889346 } , //  101
{ 52.0666809  , 4.3192429 } , //  102
{ 53.2157008  , 6.557346  } , //  103
{ 51.4696515  , 5.476912  } , //  104
{ 52.224899 , 6.8809614 } , //  105
{ 52.3232446  , 4.9562667 } , //  106
{ 50.8496025  , 5.700141  } , //  107
{ 52.4383459  , 4.8164543 } , //  108
{ 52.4536448  , 4.8240509 } , //  109
{ 52.4273214  , 4.8202936 } , //  110
              
{ 52.0441253  , 4.5017852 } , //  111
{ 52.206085 , 6.8905772 } , //  112
{ 51.8871038  , 4.4903609 } , //  113
{ 51.9330729  , 4.4783508 } , //  114
{ 52.5125022  , 6.0925683 } , //  115
{ 52.5223988  , 6.1167849 } , //  116
{ 52.4962702  , 6.1166938 } , //  117
{ 51.8528681  , 4.675802  } , //  118
{ 52.6159075  , 4.7589447 } , //  119
{ 52.3549737  , 6.6643131 } , //  120
              
{ 52.1114384  , 4.6612496 } , //  121
{ 52.2973605  , 4.9473695 } , //  122
{ 52.3768951  , 4.897672  } , //  123
{ 52.3738047  , 4.8936824 } , //  124
{ 52.3753997  , 4.8946633 } , //  125
{ 52.3711733  , 4.8920084 } , //  126
{ 53.3186404  , 6.8798279 } , //  127
{ 51.9592572  , 5.8953251 } , //  128
{ 51.9816132  , 5.9062688 } , //  129
{ 53.0263789  , 6.5721171 } , //  130
              
{ 52.7718325  , 6.8966291 } , //  131
{ 52.719743 , 6.4676425 } , //  132
{ 52.6948826  , 6.2050863 } , //  133
{ 52.4823542  , 5.5054246 } , //  134
{ 53.108703 , 6.0814163 } , //  135
{ 52.9651303  , 5.9316476 } , //  136
{ 52.848569 , 5.7207576 } , //  137
{ 52.1691314  , 5.5648313 } , //  138
{ 51.8558151  , 5.7511459 } , //  139
{ 51.9451065  , 6.3105892 } , //  140
              
{ 51.9673446  , 5.9891253 } , //  141
{ 52.3711268  , 6.0195757 } , //  142
{ 52.3157947  , 5.5646262 } , //  143
{ 51.8798989  , 5.2141057 } , //  144
{ 51.8063904  , 5.8527335 } , //  145
{ 51.8469196  , 5.8648707 } , //  146
{ 51.9175657  , 5.5681509 } , //  147
{ 51.8899461  , 5.3830863 } , //  148
{ 51.8314765  , 5.2494592 } , //  149
{ 51.9743516  , 6.7035704 } , //  150
              
{ 52.149208 , 6.1965215 } , //  151
{ 52.9805372  , 6.9649777 } , //  152
{ 53.1434592  , 7.0445626 } , //  153
{ 51.1040201  , 5.8627208 } , //  154
{ 50.956406 , 5.8133291 } , //  155
{ 50.904947 , 5.9435831 } , //  156
{ 50.8634266  , 6.0140189 } , //  157
{ 50.9172928  , 6.0225787 } , //  158
{ 51.2775391  , 5.7393974 } , //  159
{ 51.1939075  , 5.989772  } , //  160
              
{ 51.1981841  , 5.9873212 } , //  161
{ 50.9943987  , 5.8422479 } , //  162
{ 51.3807044  , 6.1886664 } , //  163
{ 51.547993 , 5.995238  } , //  164
{ 51.254256 , 5.7064896 } , //  165
{ 51.4866049  , 4.2783177 } , //  166
{ 51.5038955  , 5.413232  } , //  167
{ 51.6002554  , 4.7385588 } , //  168
{ 51.7313621  , 5.8523684 } , //  169
{ 51.6902421  , 5.3019561 } , //  170
              
{ 51.4409823  , 5.4766954 } , //  171
{ 51.4986773  , 4.7471809 } , //  172
{ 51.7353545  , 5.5105575 } , //  173
{ 51.4720972  , 5.6976201 } , //  174
{ 51.6263514  , 4.869002  } , //  175
{ 51.6737663  , 4.8243827 } , //  176
{ 51.7194429  , 5.3356075 } , //  177
{ 51.6708658  , 5.6008164 } , //  178
{ 51.6890946  , 5.0429493 } , //  179
{ 52.4801519  , 4.6658109 } , //  180
              
{ 52.9586181  , 4.7606842 } , //  181
{ 52.9162154  , 4.793278  } , //  182
{ 52.6975071  , 5.2731964 } , //  183
{ 52.663014 , 4.8138223 } , //  184
{ 52.2894586  , 4.7233249 } , //  185
{ 52.6478949  , 5.0334979 } , //  186
{ 52.4857888  , 4.9510261 } , //  187
{ 52.7821917  , 4.8088656 } , //  188
{ 52.3211824  , 4.7922831 } , //  189
{ 52.3103902  , 4.7606636 } , //  190
              
{ 52.2362047  , 6.1781387 } , //  191
{ 52.5752543  , 6.5971023 } , //  192
{ 52.25371933 , 6.435264587 } , //  193
{ 52.3617017  , 6.5091175 } , //  194
{ 52.2850742  , 6.928612  } , //  195
{ 52.7992771  , 6.1202877 } , //  196
{ 52.1138502  , 5.2600626 } , //  197
{ 52.0149705  , 5.0626822 } , //  198
{ 52.1288627  , 5.0350463 } , //  199
{ 52.0430455  , 5.556495  } , //  200
              
{ 52.0255411  , 5.5549165 } , //  201
{ 52.0772278  , 4.9042479 } , //  202
{ 51.482826 , 3.8962406 } , //  203
{ 51.4830884  , 3.6250892 } , //  204
{ 51.3047158  , 3.8536208 } , //  205
{ 52.0680775  , 4.7399641 } , //  206
{ 52.0101563  , 4.3624556 } , //  207
{ 52.0691032  , 4.3364335 } , //  208
{ 52.0775304  , 4.3143807 } , //  209
{ 51.7742045  , 4.6473862 } , //  210
              
{ 51.8413454  , 5.0053229 } , //  211
{ 52.0197741  , 4.6828297 } , //  212
{ 51.8351254  , 4.6175232 } , //  213
{ 52.2759575  , 4.5725706 } , //  214
{ 51.8745084  , 4.3579678 } , //  215
{ 52.1604483  , 4.5501115 } , //  216
{ 51.8751477  , 4.6046257 } , //  217
{ 51.9216027  , 4.4796928 } , //  218
{ 51.859626 , 4.2926892 } , //  219
{ 51.8394612  , 5.0909626 } , //  220
              
{ 52.0100715  , 4.2936514 } , //  221
{ 53.1822392  , 5.4624373 } , //  222
{ 52.3507826  , 6.6768665 } , //  223
{ 52.1291059  , 4.661423  } , //  224
{ 53.09587  , 6.1356309 } , //  225
{ 52.0429909  , 4.2420775 } , //  226
{ 51.9445089  , 5.7570347 } , //  227
{ 52.3738828  , 4.8928372 } , //  228
{ 53.0348405  , 5.6222125 } , //  229
{ 51.5859963  , 5.0286217 } , //  230
              
{ 51.585927 , 4.7758374 } , //  231
{ 51.4984131  , 4.7407378 } , //  232
{ 51.8993827  , 4.9810029 } , //  233
{ 51.6853782  , 5.2867599 } , //  234
{ 48.2176235  , 16.3947487  } , //  235 Vienna:
{ 48.2395861  , 16.3772736  } , //  236
{ 48.1869562  , 16.4207902  } , //  237
{ 48.2082104  , 16.3796115  } , //  238
{ 48.1859336  , 16.3679354  } , //  239
{ 48.1978502  , 16.3485264  } , //  240
              
{ 48.2149123  , 16.3351768  } , //  241
{ 48.2902942  , 16.3773064  } , //  242
{ 48.1493902  , 16.3218782  } , //  243
{ 48.188525 , 16.3847931  } , //  244
{ 48.2402527  , 16.4283583  } , //  245
{ 48.2402527  , 16.4283583  } , //  246
{ 48.2177471  , 16.391398 } , //  247
{ 48.2002361  , 16.3677648  } , //  248
{ 48.2176235  , 16.3947487  } , //  249
{ 48.1869562  , 16.4207902  } , //  250
              
{ 48.1964823  , 16.3376732  } , //  251
{ 48.2082104  , 16.3796115  } , //  252
{ 48.1978502  , 16.3485264  } , //  253
{ 48.2395861  , 16.3772736  } , //  254
{ 48.2257231  , 16.428376 } , //  255
{ 48.2572102  , 16.3991922  } , //  256
{ 48.2239825  , 16.4121486  } , //  257
{ 47.8124488  , 16.2432292  } , //  258


};

void FindMacAndRotation(){                                   // - VOID FUNCTIE 5 - 
//[ DEEL 1 ][ Bereken McDonald's ]
  while(!MacCord){
    for (int i = 0; i<=257;i++ ){                            // Functie die de array volledig laat doorrekenen
      
      DeltaLat = latitude - MCD[i][0];                       // Berekent verschil tussen input- en array Latitutde
      DeltaLng = longitude - MCD[i][1];                      // Berekent verschil tussen input- en array Longitude
      Afstand2 = sqrt(sq(DeltaLat)+sq(DeltaLng));            // Berekent absolute hartafstand van GPS en array McDonald's
      if(Afstand2<Afstand1){                                 // Checkt of berekende afstand kleiner is dan opgeslagen afstand
         Afstand1=Afstand2;                                  // Vervangt Opgeslagen afstand door kortere afstand
         ResultLat = MCD[i][0];                              // Slaat bijbehorende array Latitude op
         ResultLng = MCD[i][1];} }                           // Slaat bijbehorende array Longitude op
    Serial.println("=[ Closest McDonald's is at ]=");
    Serial.print("("); 
    Serial.print(ResultLat,7);                               // Print Latitude van dichtstbijzijnde McDonald's tot nu toe
    Serial.print(", "); 
    Serial.print(ResultLng,7);                               // Print Longitude van dichtstbijzijnde McDonald's tot nu toe  
    Serial.println(")");          
    MacCord = true;} 

//[ DEEL 2 ][ Bereken Hoekverdraaiing ]
    angle = atan2((ResultLat-latitude),(ResultLng-longitude));
    Serial.print("Angle (rad): ");
    Serial.println(angle);
    degree = angle * 180 / PI;
    Serial.print("Angle (deg): ");
    Serial.println(degree);
  
    if ( angle <= 0){
    rotation = (2*Pi) + angle;
    } else {
    rotation = angle ; }
    Serial.print("Angle to rotate motor: ");
    Serial.println(rotation); }

//===============================================================================================
//======================================================================================[ SETUP ]
//===============================================================================================
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);                                      // 0 - Print Setup
  Serial.println();                                          
  Serial.println(" BEGIN ");
 
  Serial1.begin(9600);                                       // 1 - GPS
  pinMode (hallsensorPin, INPUT);                            // 2 - Hall Sensor
  MacMotor.setSpeed(10);                                     // 3 - Motor
  MacMotor.step(10);
  turnOffMotorPins();
  compass.init();                                            // 4 - Magnetometer 
                                                             // 5 - Coördinaten 
  UpdateSystem();                                              // 6 - System Functies
}

//===============================================================================================
//=======================================================================================[ LOOP ]
//===============================================================================================
void loop() {
  delay(3000);
  UpdatePointer();
  Updates ++;
  if(Updates == 20){
    MotorCalibrate;
    Step = (stepsPerRevolution / (2*Pi) * (rotation + headingRadians))+ 512;
    MacMotor.step(Step);
    turnOffMotorPins(); }
  if(Updates == 40){
    UpdateSystem;
    Updates = 0;}
}

//===============================================================================================
//===========================================================================[ System Functions ]
void UpdateSystem(){                                         // - VOID FUNCTIE 6 -
  MotorCalibrate();     // [ 1 ][ Kalibreer  ]
  //GPSFix();             // [ 2 ][   GPS Fix  ]
  FindMacAndRotation(); // [ 3 ][ McDonald's Locatie en Hoekverdraaiing ]
  //ReadHeading();        // [ 4 ][ Zoek Noord ]
  
  Step = (stepsPerRevolution / (2*Pi) * (rotation + headingRadians))+ 512;
  Serial.print("Cumulative rotational angle: "); 
  Serial.println(rotation + headingRadians);
  Serial.println(Step);
  MacMotor.step(Step);
  turnOffMotorPins();
  
}
void UpdatePointer(){                                        // - VOID FUNCTIE 7 -
  
  compass.read(&x, &y, &z);
  headingRadians = atan2(y, x);
  UpdatedHeading = headingRadians;
  DeltaHeading = (UpdatedHeading - BaseHeading);
  Step = (stepsPerRevolution / (2*Pi) * (DeltaHeading));
  MacMotor.step(Step);
  turnOffMotorPins;
  BaseHeading = UpdatedHeading;
}
  
