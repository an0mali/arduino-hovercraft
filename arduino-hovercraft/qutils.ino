
void vprint(String func, String mes) {
  if (verbose_debug) {
    Serial.println("::Mel::\t" + func + ": " + mes); 
  };
}

int user_inp(String info_mes, int count) {
 vprint("UserInput", info_mes);
 while (Serial.available() < count) {
  };
  int inp = Serial.parseInt();
  vprint("input", String(inp) + " recieved");
  return inp;
}
