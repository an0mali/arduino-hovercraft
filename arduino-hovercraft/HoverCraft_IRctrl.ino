

void setupHCIR() {
  IR_Rec.enableIRIn();
}

long getValue(int from, int to, String res_str) { 
  const int len = to - from;
  char str[len];
  char *ptr;
  long value;

  int ind = 0;
  for (int i = from; i < to; i++) {
    str[ind] = res_str[i];
    ind += 1;
  };
  value = strtol(str, *ptr, 2);
  return value;
}

void dumpIR(decode_results *IR_Results, unsigned long level, long results[4]) {
  String labels[4] = {"YAxis", "XAxisFlip", "XAxis", "TrimButton"}; //Y axis max is 124//128; X is 29//32;
  int startbins[4] = {3, 14, 15, 22};
  int endbins[4] = {10, 15, 20, 24};

  //Set up character array of the binary signal
  String result_str;
  for (int i = 0; i < IR_Results ->rawlen - 2; i++) {
    String bitv = "0";
    if (IR_Results->rawbuf[i] > level) {
      bitv = "1";
    };
    result_str += bitv;
  };

  String mes = "IR Input> ";
  for (int i = 0; i<4; i++) {
    
    String label = labels[i];
    int startb = startbins[i];
    int endb = endbins[i];
    long val = getValue(startb, endb, result_str);
    results[i] = val;
    mes += label + ": " + String(val) + "\t ";
    
  };
  Serial.println(mes);
}
  
void checkIR() {
  if (IR_Rec.decode(&IR_Results)) {
    long inpResults[4];
    dumpIR(&IR_Results, 14, inpResults);//outputs to binResults
    setThrust(inpResults[0]);
    dirInput(inpResults[1], inpResults[2]);
  };
  IR_Rec.resume();
}
