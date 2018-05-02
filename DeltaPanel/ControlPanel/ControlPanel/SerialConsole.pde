//import controlP5.*;

static final int CONSOLE_MAX_LEN = 20;

public class SerialConsoleBuffer {
  String[] sentence;
  int start, end;
  int bufferLen;
  boolean full, empty;
  
  SerialConsoleBuffer() {
    sentence = new String[CONSOLE_MAX_LEN];
    start = 0;
    end = 0;
    full = false;
    empty = true;
    bufferLen = CONSOLE_MAX_LEN;

    for (int i = 0; i < CONSOLE_MAX_LEN; i++) {
      sentence[i] = "";
    }
  }

  SerialConsoleBuffer(int buffer_len_) {
    sentence = new String[buffer_len_];
    start = 0;
    end = 0;
    full = false;
    empty = true;
    bufferLen = buffer_len_;
    
    for (int i = 0; i < bufferLen; i++) {
      sentence[i] = "";
    }
  }
  
  public boolean inc_end_pointer(){
    end = (end + 1) % bufferLen;
    empty = false;
    if (end == start) {
      full = true;
      return true;
    }
    return false;
  }
  
}

Textarea ConsoleTextArea;
SerialConsoleBuffer ConsBuf = new SerialConsoleBuffer();
int ConsoleMax = -1;


void init_console() {

  ConsoleTextArea = cp5.addTextarea("consoleTextArea")
                  .setPosition(width - 250, height * 0.945 - 400)
                  .setSize(250, 400)
                  .bringToFront()
                  .setFont(createFont("arial",19))
                  .setLineHeight(14)
                  .setColor(color(0))
                  .setColorBackground(color(255,255))
                  .setBorderColor(0)
                  .hide()
                  ;

}

void write_console(String text) {

  if (ConsoleMax == -1) {
    ConsoleTextArea.append(text);
  }
  else {
    ConsBuf.sentence[ConsBuf.end] = text;
    ConsBuf.inc_end_pointer();

    if (ConsBuf.full) {
      ConsoleTextArea.clear();
      for (int i = 0; i < ConsBuf.bufferLen; i++) {
        ConsoleTextArea.append(ConsBuf.sentence[(ConsBuf.end + i) % ConsBuf.bufferLen]);
      }
    }
  }

}

void ConsoleSetMax(int maxVal) {

  ConsBuf = new SerialConsoleBuffer(maxVal);
  ConsoleMax = maxVal;

  return;

}