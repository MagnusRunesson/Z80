void plog(const char* _pszFormat, ...)
{
  char itoab[ 2048 ];
  va_list argptr;
  va_start(argptr, _pszFormat);
  //vfprintf(stderr, format, argptr);
  //sprintf(itoab, "%lu", _o);
  vsnprintf(itoab, 2048, _pszFormat, argptr);
  va_end(argptr);
  Serial.print(itoab);
}

void setup() {
  Serial.begin(9600);
  Serial.print("whaat! 2");
  //plog("Hello %s\n", "world");
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
}

void loop() {
}

