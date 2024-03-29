
#include <Arduino.h>
#include <U8g2lib.h>

#include <Wire.h>

//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);//rotate 0
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);//rotate 180

#define INPUT_LED0 4
#define INPUT_LED1 5
#define SCK_PIN   13 
#define MISO_PIN  12  
#define MOSI_PIN  11 
#define SS_PIN    2  

uint8_t digit_raw[8];

void setup(void) {
  delay(100);
  pinMode(SCK_PIN, INPUT);
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);  // (only if bidirectional mode needed)
  pinMode(SS_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SS_PIN), ss_irq, FALLING);

  u8g2.begin();
  u8g2.clearBuffer();  
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_haxrcorp4089_t_cyrillic) ;	
  u8g2.setCursor(10,20);
  u8g2.print(F("MAX7219 to OLED"));
  u8g2.setCursor(10,40);
  u8g2.print(F("by Payalne"));
  u8g2.sendBuffer();
  
  digit_raw[0]=0x7e;
  digit_raw[1]=0x7e;
  digit_raw[2]=0x7e;
  digit_raw[3]=0x7e;
  delay(3000);
  
}



void ss_irq() {
    SPCR = (1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(1<<SPR0); // SPI on
    uint16_t timeout=5000;
    while(!(SPSR & (1<<SPIF)))
    {
        if(--timeout==0 ) return;
    }
    uint8_t i = SPDR;               // store high-order byte
    i--;
    i&=0x07;
    while(!(SPSR & (1<<SPIF)))     
    {
        if(--timeout==0 ) return;
    }
    digit_raw[i] = SPDR;               // store low-order byte
    //else
      //i=SPDR;
    SPCR = (0<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(1<<SPR0);  // SPI off
}

bool if_raw_dot(uint8_t value)
{
  return ((value&0x80)==0x80);
}

char raw_to_char(uint8_t value)
{
  switch (value&0x7F)
  {
    case 0x7E:
      return '0';
    case 0x30:
      return '1';
    case 0x6D:
      return '2';
    case 0x79:
      return '3';
    case 0x33:
      return '4';
    case 0x5B:
      return '5';
    case 0x5F:
      return '6';
    case 0x70:
      return '7';
    case 0x7F:
      return '8';
    case 0x7B:
      return '9';
    case 0x0F:
      return 'T';
    case 0x3D:  
      return 'D';    
    case 0x00:  
      return ' ';      
  }
}

const uint8_t bdf_font[383] U8G2_FONT_SECTION("bdf_font") = 
  "\21\0\4\4\4\6\5\7\6\22'\0\331\376\0\0\0\0\0\0\0\1b \6\0@`\15+\16\231"
  "\310S;\303\246y\300f\330\64\0,\7\63\310\314\11\11-\10\71HU\13\17\30.\7\63\310\314\11"
  "\11/\6\0@`\13\60!_\316L\35-B\254\30\221\242\244\375_\226\35\35\36@\350\261%\355\377"
  "\262D\212\21+B\264\0\61\23S\346L-!\36<p!$T\210\7\17\134\10\11\62\37_\316L"
  "\15NX\4Q!x\376_\13Y\21\242E\210%\242\7\317\377\261\10%!\230\70\63\36_\316L\15"
  "NX\4Q!x\376_\13Y\21\242\315\212\320\202\347\377\211\12\21,^\0\64\32_\316L\15\241\307"
  "\226\264\377\227eG\254\10\321fEh\301\363\377\265\360\0\65\36_\316L\35/X\210P\62x\376\217"
  "E\207X\323fEh\301\363\377D\205\10\26/\0\66!_\316L\35/X\210P\62x\376\217E\207"
  "X\323\42\304\212\261%\355\377\262D\212\21+B\264\0\67\31_\316L\15NX\4Q!x\376_\13"
  "\17\17<\264\340\371\377Zx\0\70$_\316L\35-B\254\30\221\242\244\375_\226\35\261\42D\213\20"
  "+\306\226\264\377\313\22)F\254\10\321\2\0\71 _\316L\35-B\254\30\221\242\244\375_\226\35\261"
  "\42D\233\25\241\5\317\377\23\25\42X\274\0:\11\363H\322\11\351\1'\0\0\0\4\377\377\0";

const uint8_t u8g2_font_clock_font_tn[3532] U8G2_FONT_SECTION("u8g2_font_clock_font_tn") = 
  ";\0\5\4\5\6\4\7\7#\62\0\0+\0+\11\6h\0\0\15\263 \6\0@`\23!!("
  "m\341\363\4#\312 dT\242\214\372\21R\302\15f\20\62\352o\220\62\214\70a\227Q\67\0\42\17"
  ",a\61\24\24\241\237H\203\10Q\204\0#g\273c%\70\211#\36)\244\25bX!\245\25RZ"
  "!\245\225B\322\20#\220\60\304 \225\10\301\210K\240`H!f\20!\304\20B\220D\12i\245\24"
  "VHi\205\224\64\304\20BL\244\210 \210\10\242\210\63\314\70\341\14\63\216(\42\10\42\202(i\230"
  "\341\12)\314\220\302J)\254\20\303J!n\34\1\3\12\12\0$>\370\335\341\266\5;h\231\65Y"
  "\242K\17\204\363\300\70\17\204\42\204+d\326o\22:\2K!\270\364@Hn\61\21\354\240d\326o"
  "\222\342\204(\17\204\363\300\70\17\204\344^\231ut\64\0%v\376lcy\17\134\16i\306\230e\14"
  "R\5\31e\212\20D\204c\12\61\303\224R\21S\252aL)e\224SJ\21\346\224B\204IB\20"
  "!Fi\306\24\206\314p\346\10H>$F.\205<C\214+\4\61S\14\63C\10\42D*\203\224"
  "r\214(\245\30\63J)\246\220RJ\61\244\224B\214\31\206\224\202\202 B\24\243\14*\353\34\322J"
  "\32N,\301C\17\31\0&UZ\334d\27\251\307\232cm\5\261\326\260\245\326Z\70\205\215RX!"
  "e\31RV)$\254@B \42\264`F\10/\230\322\310\30a\254\21D\30\343\215B\32)e\25"
  "bV!\205\25RX!\205\25RV)e\225R\30\61B(\61\24\23A\271\306\236Z\0'\13%"
  "a\261\22| \11!\0((N\334d\24\231s\314\71\246\10A\12Q%\25dPIu\211\254\360"
  "\201\12l\250\222\352\227\310\22b\240\20\12\272\11\0)(O\324dt\34\204!\243\306\10m\254\242j"
  "\251\250j\21\66\230\370@\217U\222IE\325S\344\14!NI\347\134\10\0*>\66[\251v\5:"
  "d\211\265TF\21&\30A\304\11G\214p\304\21\42\240qB\20\307\224a\274\21\306\234\20\204!'"
  "\10q\5\42\214\70\201\210\23\312\250R!\201\25h\42\221\203\6\6\0+$\324\342g\66\11\71`y"
  "up(#B\60\3\11\24\36(\1\11\64\14\61,\304\1\313\253\203C\212\4\0,\15\6Q\242\62"
  "\250P'\6\11\3\0-\24\264`lV<\20\306\3#<P\302\3c<\20\4\0.\13\246\320\244"
  "\62\224\240B\25\0/.\230T\244\326\212Y\244\211F\232hI\23-i\242\221e\226I\350\370P\11"
  "\264H\23\215\64\321&M\264\244\221e\22J\350\250\342\6\13\0\60dX\334\344\226<@H\10\17\210"
  "\20\206\10N\10A\2\23C\20\241\4\21\205\25QX\21e\225QV\31e\225QV\31e\225QV"
  "\31\204\225\61\34!\1\212\17\241P\304\33\203\64\62\312*\243\254\62\312*\243\254\62\312*\242\260\42\12"
  "+\242\260\42\210P\241\210!X\30C\204\27\304\10\341\1\21\2y\200\20\0\61\35\347\353\345\323\204\42"
  "\310\30D\224PD\375\6!\201\206\62\6\21\365\67F\21&\14\0\62\71X\334\344\226<@\314\3\42"
  "\4\343\204\70L\14\244\4\231\225,\263\276\303\2\61.\10\363@Hn\61$*\241e\326\311\62\253I"
  "\204BC\260#\302\63!< \314\3\204\0\63\67W\344\344v<@\312\3\42\204\342\204\60L\214\243"
  "\4\221U,\262>\303\2).\210\362@@N\61\21\352\230D\326\337Q\241\30\26FyA\220\7D"
  "\10\343\1B\0\64\65\366\343\345\266\16Y\340q\211\251\265V\11e\225PV\11e\225PV\11e\225"
  "@X\11#\260@D\10.\10\362@\70.\61\21\350\220$\326\237$S\324\60\0\65\67V\344\344v"
  "<@D\10\17\210!\202#$\260B\204\62%\326/\22\71\2;!\270\363@\70.\61\21\350\220$"
  "\326\237Q\241\24\26\6yA\214\7D\10\342\1\62\0\66QW\334\344\226<@F\10\17\10\42\202+"
  "$\60C\204:E\326O\222\71\2C!\70\364@@N\61\21\210xC\220FDYE\224UDY"
  "E\224UDY%\24VBa%\24V\2\21*\224\60\4\13C\210\360\202\20!< B\30\17\220"
  "\1\0\67'\24\364\345\26<@\304\3\42\4\341\204\30L\14\242\4y\225+\257>H\242\370`\7\71"
  " y\365\7I\24\63\14\0\70jX\334\344\226<@H\10\17\210\20\206\10N\10A\2\23C\20\241"
  "\4\21\205\25QX\21e\225QV\31e\225QV\31e\225QV\31\204\225\61\2\13\204\204\340\202\60"
  "\17\204\344\26\23\241\210\67\6id\224UFYe\224UFYe\224UDaE\24VDaE\20"
  "\241B\21C\260\60\206\10/\210\21\302\3\42\4\362\0!\0\71QW\344\344v<@F\10\17\210\20"
  "\204\10N\210@\2\23#\20\241\4\11\205\225PX\11e\25QV\21e\25QV\21e\25QV\21"
  "\204\25\61\2\13d\204\340\202(\17\4\344\24\23\241\216Id\375\35\25\212aa\224\27\4y@\204\60"
  "\36 \4\0:\20\250\333$s\324\211\362\341|\31\65Q\6\0;\24H\134\42s\324\211\362\341\235\62"
  "\212(\243\216\14\23\12\0<\31\17[\347t\21U\222A\347\334\337=\351\226\214\262\322IF\225E\4"
  "\0=\42\265Yjv< \306\3$<P\304\3\203<\20>d\36\10\343\1\22\36\60\341\1\62\36"
  "\20\2\0>\31\17[\347T\220UTQV\272\222QV\272\354\71\367C&\25E\26\0?\63\71U"
  "\341\226<\20\324\3A\275\325\332\22\302\222Zf\241\365!\26\310qA\234\7\202r\214%aI-\264"
  "n\26Z\243\244\16\33>\314\24Zf\241\25\5@p^\334d\30=P\322\3E\4\363@\21\342<"
  "\60\204(B<\20\304 D\222Qb\31%\226Q$\31\5%!H\71\353\24\323L\61\354\20\42\204"
  "\22\202\14B\330(\241\24EVIe\225TVI\205\210RR\31\204\20UF!B\244@F\71J"
  "\214Q\14\23b\224\303D\30\5\61Rr\321d\13\361@\61A<p\314\3\312<\200\320\3\206\0A"
  "dX\334\344\226<@H\10\17\210\20\206\10N\10A\2\23C\20\241\4\21\205\25QX\21e\225Q"
  "V\31e\225QV\31e\225QV\31\204\225\61\2\13\204\204\340\202\60\17\204\344\26\23\241\210\67\6i"
  "d\224UFYe\224UFYe\224UDaE\24VDaE\24V\4qc\14(\206\220a\204"
  "\31\6\0B[X\334\344\226<\24\302;\42\70C\2;D(!Had\24VDYe\224UF"
  "Ye\224UFYe\224U\6ae\214\300\2!!\270 \314\3!\271\305D(\342\215A\32\31e"
  "\225QV\31e\225QV\31e\25QX\21\205\25Q\30\31$,!\310\10\355\210\340\222\227\0C."
  "X\334\344\26=\20\316CO\65$\304Bd\326o\22:l\370\260 *\241e\326\311\62\13%t\210"
  "\225\202h\351\201\220\36\10\351\201\60\0DUX\334\344\226<\24\302;\42\70C\2;D(!Ha"
  "d\24VDYe\224UFYe\224UFYe\224U\6ae\14GH\200\342C(\24\361\306 "
  "\215\214\262\312(\253\214\262\312(\253\214\262\212(\254\210\302\212(\214\14\22\226\20d\204vDp\311K"
  "\0E;X\334\344\226<PF\10\17\210\42\302+$\64C\304:e\326o\22:\2K!\270\364@"
  "Hn\61$*\241e\326\311\62\253I\304:C\64#\302\3\241\204\360\300(\17\224\1\0F\61\70\134"
  "\345\226<PF\10\17\210\42\302+$\64C\304:e\326o\22:\2K!\270\364@Hn\61$*"
  "\241e\326\311\62\253I\350\250\302\206\13\0GBX\334\344\26=\20\316CO\65$\304Bd\326o\216"
  "*\316\201\311-\227\216\70'\214A\32\31e\225QV\31e\225QV\31e\225QV\21\205\25QX"
  "\31\204\21\62\204\22\302\4\301\226S\215-\4\0H\134X\334\344\366\206\21\244\30\2\16A\34\21\205\25"
  "QX\21e\31QV\31e\225QV\31e\225QV\31e\225AX\31#\260@H\10.\10\363@"
  "Hn\61\21\212xc\220FFYe\224UFYe\224UFYE\24VDaE\24VDaE"
  "\20\67\306\200b\10\31F\230a\0I\36G\354\344\323\204\42\310\30DT\241\210\372\15B\2\15e\14"
  "\42\352O\20\62\212\60\321\0J\67X\334\344\366\6+\352\240d\326d\231\365QR\305\207P\260\203\22"
  "\42Z\31\204\225QV\31e\225QV\21\205\25QX\31\204\221\42\204\22\302\4\301\324S\215-\4\0"
  "KPV\334\344V\222\21Z\31b\31A\322\21\305\240Q\12\42\205\240R\6\62E\240S\302I%\30"
  "UBY%\20F\344\10\354\204\340\316\3\341\270\304D\30\342\215@\32\11e\225PV\11e\225PV"
  "\11e)\246c\311\215\60\240\10B\206\20f\10\0L)\65\334\344v\4*$\211\5\326\37$q\314"
  "\360!*$\211\5\326\275\2+H\304\42C\264!\302\3A\204\360\300\20\17\24M_X\334\344\226<"
  "@H\10\17\210\20\206\10N\10\61\4\23C\20\241\4\21\205\25QX\21\245\10RF!d\324\377\33"
  "\204\224Q\306(\205\20\22L)\202\225Y\225P\204!e\14bD!\243\254\62\312*\243\254\62\312*"
  "\243\254\42\12+\242\260\42\12+\242\260\42\210\33c@\61\204\14#\314\60\0NdX\334\344\266\30\26"
  "F\20F\11\61\302AC\220p\14\21%\34RD\21\206\24Q\206\21F\24bB\31\245\224PF)"
  "%\224Q\14\11e\224UFYe\20V\306p\204\4(>\204B\21o\14\322\310(\253\214\262\312("
  "\253\214\262\312(\253\210\302\212(\254\210\302\212(\254\10\342\306\30P\14!\303\10\63\14\0OQX\334"
  "\344\26-\326\224[,\11\241\204(\204\221QX\21e\225QV\31e\225QV\31e\225QV\31\204"
  "\225\61\34!\1\212\17\241P\304\33\203\64\62\312*\243\254\62\312*\243\254\62\312*\242\260\42\12+\203"
  "\60R\204PB\230 \230z\252\261\205\0PDX\334\344\226<\24\302;\42\70C\2;D(!H"
  "ad\24VDYe\224UFYe\224UFYe\224U\6ae\214\300\2!!\270 \314\3!"
  "\271\305\220\250\204\226Y'\313\254MBG\25\66\134\0QWW\334\344\26)\306\224K\15\5\261\204 "
  "\243\21QV\21e\25QV\21e\25QV\11\205\225PX\11\304\21!\240\370\220\22O\14\322\210("
  "\253\210\262J\60\253\204\302J(\207\210!\312)\42\214rL)\347\220\202L\31\301\14SB@\342\24"
  "%\214A\244\34c\210\0RZX\334\344\226<\24\302;\42\70C\2;D(!Had\24VD"
  "Ye\224UFYe\224UFYe\224U\6ae\214\300\2!!\270 \314\3!\271\305\220\250d"
  "\14W\4iE\24V\204YE\34U\306A\245\234S\314\61\345\30S\220)D\31\62\226!\301\25J"
  "\10\0S\63X\334\344\26\271\364@\70\17\214\363@(B\270Bf\375&\241#\260\24\202K\17\204\344"
  "\26\23\301\16Jf\375&)N\210\362@\70\17\214\363@H\16\1T'W\344\344v<@\304\3_"
  "x\340\210\7\312\7$\324\61\213\254\233\204\212\17\361P\307$\262~\261\310\332\34\65\64\0UTX\334"
  "\344\366\206\21\244\30\2\16A\34\21\205\25QX\21\205\25QV\31e\225QV\31e\225QV\31e"
  "\225AX\31\303\21\22\240\370\20\12E\274\61H#\243\254\62\312*\243\254\62\312*\243\254\42\12+\242"
  "\260\62\10#E\10%\204\11\202\251\247\32[\10\0VQ\67\334\244v\4\31\304\200B\20\67DaD"
  "\24FDaD\224UDYE\224UDYE\224UDYE\220F\204\200\342CJ\260\62\210*\244"
  " C\312\71\244\230SJA\245\224c\12\71\246\220s\312\70\250\210\223\212\60\252\210\262\212 m\324@"
  "\1W]X\334\344\366\206\21\244\30\2\16A\34\21\205\25QX\21e\31QV\31e\225QV\31e"
  "\225QV\31e\225AL(e\14\63\12!\301\224\42X\231E\232\22\212(\245\214AH!d\324\377"
  "O\224\62H\21\305\204RDaE\20\261\2\21C\264 \206\20N\204\21\302\3\302<@\10\0XV"
  "W\334\244v\10(\306pC\24FDaD\30U\204If\230sH\71\246X\307\14\203\214\70\251\10"
  "\263\210(\214\10\342\206 n\210\361\201\37b\70\42\206#\202\260\42\310\62\242$\63J\62\303\34S,"
  "c\310\71f\230T\204Q&\24FDaD\20\67\304xC\0Y=V\344\344\266\206\20\244\10\2\36"
  "\227\230n\225PV\11e\225PV\11e\225PV\11\204\225\60\2\13D\204\340\202 \17\204\343\22\23"
  "\201\16Ib\375\42)L\210\342\316\3\341\270\304\20\0Z\67X\334\344v<@\314\3\304<@\314\3"
  "\2\275\17\30\231\225\64\321H\23-id\231\202\206:(\231EZ\321\222F\226I>`.=\20\316"
  "\3\343<\60\316\3\203\0\0\0\0";

void loop(void) {

  // This problem applies only to full buffer mode
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  //u8g2.setFont(u8g2_font_8bitclassic_tf) ;	
  u8g2.setFont(u8g2_font_luRS12_tr    ) ;	
  u8g2.setCursor(15,20);
  if (digitalRead(INPUT_LED0))
    u8g2.print("QFE");
  u8g2.setCursor(70,20);
  if (digitalRead(INPUT_LED1))
    u8g2.print("QNH");


  char str[10]="";
  //#u8g2.setCursor(10,20);
  //sprintf(str,"%02X%02X%02X%02X", digit_raw[0], digit_raw[1], digit_raw[2], digit_raw[3]);
  //u8g2.print(str);



  u8g2.setFont(u8g2_font_clock_font_tn) ;	 
  
  for (uint8_t i=0;i<4;i++)
    str[i]=raw_to_char(digit_raw[3-i]);

  for (uint8_t a=0;a<4;a++)
  {
    u8g2.setCursor(a*30+4,68);
    if (str[a]=='1')
      u8g2.setCursor(a*30+16+4,68);
    u8g2.print(str[a]);
  }
  
  if (if_raw_dot(digit_raw[2]))
  {
    u8g2.setCursor(1*30+16+12,68);
    u8g2.print('.');
  }
  u8g2.sendBuffer();

  delay(10);

}

