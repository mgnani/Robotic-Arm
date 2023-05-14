#define TFT_DC    7
#define TFT_RST   8
#define SCR_WD   240
#define SCR_HT   240   // 320 - to allow access to full 240x320 frame buffer
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

#define pull false

typedef enum {seguridad, principal, repetir, funciones} menu;
menu OKA;
typedef enum {seguridad2, mover, grabar, repeticiones, borrar, calibrar, seguridad3} seleccion;
seleccion opciones;
typedef enum {seguridad4, moviendo, grabando, unica, multiple, borrando, calibrando} ejecucion;
ejecucion funcion;

int multi_funcion = 1;
int opcion = 1;
int muestras = 0;
int arduino = 10;

bool band_ard = true;
bool band_borrado = true;

const int bot_baja = 4;
const int  bot_sube = 5;
const int bot_select = 6;
const int bot_back = 3;

void pantalla(int x, int y, int color, int fondo, int altura, char texto[30]);
void navegacion();
void menu_principal();
void menu_funciones();
void veces();
void copyright();

void setup() {
  pinMode(arduino, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);

  pinMode(bot_baja, INPUT_PULLUP);
  pinMode( bot_sube, INPUT_PULLUP);
  pinMode(bot_select, INPUT_PULLUP);
  pinMode(bot_back, INPUT_PULLUP);
  OKA = principal;
  copyright();
}

void loop() {
  Serial.println(digitalRead(arduino));
  Serial.write(funcion);
  navegacion();
  switch (OKA) {
    case seguridad: {
        OKA = principal;
      }
      break;
    case principal: {
        menu_principal();
        band_ard = true;
        band_borrado=true;
      }
      break;
    case repetir: {
        veces();
      }
      break;
    case funciones: {
      if (band_borrado) {
          copyright();
          band_borrado=false;
        }
        menu_funciones();
      }
      break;
  }
}
///////////                                              FUNCIONES PRINCIPALES                                 ////////

void menu_principal() {
  ///////                                                 Opciones de menu (Seleccion)                              ///////
  switch (opcion) {
    case seguridad2: {
        opcion = calibrar;
      }
      break;
    case mover: {
        pantalla(53, 60, WHITE, BLACK, 3, "CALIBRAR");
        pantalla(210, 110, RED, BLACK, 5, " ");
        pantalla(-9, 110, GREEN, BLACK, 5, "  MOVER ");
        pantalla(10, 175, WHITE, BLACK, 3, "   ");
        pantalla(50, 175, WHITE, BLACK, 3, " GRABAR  ");
        pantalla(210, 175, RED, BLACK, 5, " ");
      }
      break;
    case grabar: {
        pantalla(42, 60, WHITE, BLACK, 3, "  MOVER  ");
        pantalla(210, 110, RED, BLACK, 5, " ");
        pantalla(-25, 110, GREEN, BLACK, 5, "  GRABAR");

        pantalla(20, 151, RED, BLACK, 3, "            ");
        pantalla(20, 175, RED, BLACK, 3, " ");
        pantalla(30, 175, WHITE, BLACK, 3, "  REPETIR  ");
        pantalla(20, 198, RED, BLACK, 3, "            ");
      }
      break;
    case repeticiones: {
        pantalla(50, 60, WHITE, BLACK, 3, " GRABAR  ");
        pantalla(226, 110, GREEN, BLACK, 2, " ");
        pantalla(226, 120, GREEN, BLACK, 2, " ");
        pantalla(226, 130, GREEN, BLACK, 2, " ");
        pantalla(-12, 110, GREEN, BLACK, 5, " REPETIR");
        pantalla(50, 175, WHITE, BLACK, 3, " BORRAR ");
      }
      break;
    case borrar: {
        pantalla(30, 60, WHITE, BLACK, 3, "  REPETIR ");
        pantalla(20, 86, RED, BLACK, 3, "            ");
        pantalla(-17, 110, GREEN, BLACK, 5, "  BORRAR");
        pantalla(218, 110, GREEN, BLACK, 3, " ");
        pantalla(218, 123, GREEN, BLACK, 3, " ");
        pantalla(30, 150, RED, BLACK, 3, "           ");
        pantalla(40, 175, WHITE, BLACK, 3, " CALIBRAR  ");
      }
      break;
    case calibrar: {
        pantalla(50, 60, WHITE, BLACK, 3, " BORRAR ");
        pantalla(20, 86, RED, BLACK, 3, "            ");
        pantalla(0, 110, GREEN, BLACK, 5, "CALIBRAR");
        pantalla(20, 175, WHITE, BLACK, 3, "  ");
        pantalla(190, 175, WHITE, BLACK, 3, "  ");
        pantalla(30, 150, RED, BLACK, 3, "           ");
        pantalla(42, 175, WHITE, BLACK, 3, "  MOVER  ");
         pantalla(30, 197, RED, BLACK, 3, "           ");
      }
      break;
    case seguridad3: {
        opcion = mover;
      }
      break;
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void menu_funciones() {

  switch (funcion) {
    case seguridad4: {
        //CASE SEGURIDAD
      }
      break;
    case moviendo: {
        pantalla(50, 60, WHITE, BLACK, 3, "         ");
        pantalla(0, 110, GREEN, BLACK, 5, "MOVIENDO");
        pantalla(50, 190, RED, BLACK, 3, "         ");
        pantalla(10, 175, WHITE, BLACK, 2, "Manipula la maqueta ");
      }
      break;
    case grabando: {
        pantalla(42, 60, BLACK, BLACK, 3, "  MOVER  ");
        pantalla(0, 110, GREEN, BLACK, 5, "GRABANDO");
        pantalla(21, 169, WHITE, BLACK, 2, "Total de muestras");
        pantalla(21, 185, WHITE, BLACK, 2, "                  ");

        pantalla(120, 200, WHITE, BLACK, 2, "/ 20");
        lcd.setCursor(95, 200);
        lcd.println(muestras);
        Serial.print(muestras);
        if (digitalRead(arduino) == pull and muestras < 20) {
          muestras++;
          while (digitalRead(arduino) == pull) {}
        }
        // pantalla(62, 185, GREEN, BLACK, 3, " 1/20  ");
      }
      break;
    case unica: {
        pantalla(45, 110, GREEN, BLACK, 5, "UNICA ");
      }
      break;
    case multiple: {
        pantalla(0, 110, GREEN, BLACK, 5, "Infinity");
      }
      break;
    case borrando: {
        int barra;
        if (digitalRead(arduino) == pull or band_ard == false) {
          band_ard = false;
          muestras = 0;
          pantalla(3, 86, WHITE, BLACK, 3, "              ");
          pantalla(-3, 110, GREEN, BLACK, 5, " BORRADO ");
          for (barra = 30; barra <= 210; barra++) {
            pantalla(barra, 150, BLACK, GREEN, 3, " ");
          }
          pantalla(42, 175, WHITE, BLACK, 3, "  LISTO  ");
        } else {
          pantalla(50, 60, WHITE, BLACK, 3, "         ");
          pantalla(3, 90, WHITE, BLACK, 3, "Presiona PLAY");
          pantalla(40, 113, WHITE, BLACK, 1, "                                ");
          pantalla(30, 120, WHITE, BLACK, 3, "   para    ");
          pantalla(40, 143, WHITE, BLACK, 1, "                                ");
          pantalla(37, 150, WHITE, BLACK, 3, "confirmar");
          pantalla(42, 175, WHITE, BLACK, 3, "         ");
        }
      }
      break;
    case calibrando: {
        pantalla(50, 60, WHITE, BLACK, 3, "        ");
        pantalla(0, 110, GREEN, BLACK, 5, "CALIBRAR");
        pantalla(20, 175, WHITE, BLACK, 2, "Todos los motores");
        pantalla(60, 191, WHITE, BLACK, 2, "Estan a 90");
      } break;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void veces() {
  switch (multi_funcion) {
    case seguridad: {
        multi_funcion = funciones;
        break;
      }
    case principal: {
        pantalla(210, 110, GREEN, BLACK, 5, " ");
        pantalla(-19, 110, GREEN, BLACK, 5, "  1 Vez ");
        pantalla(50, 60, WHITE, BLACK, 3, "        ");
        pantalla(50, 175, WHITE, BLACK, 3, "           ");
      }
      break;
    case repetir: {
        pantalla(50, 60, WHITE, BLACK, 3, "        ");
        pantalla(50, 175, WHITE, BLACK, 3, "           ");
        pantalla(0, 110, GREEN, BLACK, 5, "Infinito");
      }
      break;
    case funciones:
      {
        multi_funcion = principal;
      }
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void navegacion() {
  switch (OKA) {
    case principal: {
        while (digitalRead(bot_select) == pull) {
          funcion = opcion;
          switch (opcion) {
            case mover: {
                funcion = moviendo;
                OKA = funciones;
              }
              break;
            case grabar: {
                funcion = grabando;
                OKA = funciones;
              }
              break;
            case repeticiones: {
                OKA = repetir;
              }
              break;
            case borrar: {
                funcion = borrando;
                OKA = funciones;
              }
              break;
            case calibrar:
              {
                funcion = calibrando;
                OKA = funciones;
              }
              break;
          }
        }
      }
      break;
    case repetir: {
        while (digitalRead(bot_select) == pull) {
          OKA = funciones;
          funcion = multi_funcion + 2;
        }

        if (digitalRead(bot_back) == pull) {

          OKA = principal;
          while (digitalRead(bot_back) == pull) {
            copyright();
          }
        }
      }
      break;
    case funciones: {
        if (digitalRead(bot_back) == pull ) {
          OKA = repetir;
          if (opcion <= 2 or opcion >= 4) {
            OKA = principal;
          }
          while (digitalRead(bot_back) == pull) {
            copyright();
          }
        }
      }
      break;
  }
  if (digitalRead(bot_baja) == pull ) {
    switch (OKA) {
      case principal: {
          opcion++;
        }
        break;
      case repetir: {
          multi_funcion++;
        }
        break;
    }
    while (digitalRead(bot_baja) == pull) {
    } copyright();
  }

  if (digitalRead(bot_sube) == pull) {
    switch (OKA) {
      case principal: {
          opcion--;
        }
        break;
      case repetir: {
          multi_funcion--;
        }
        break;
    }
    while (digitalRead(bot_sube) == pull) {
    }
    copyright();
  }

  if (OKA != funciones) {
    funcion = 0;
  }


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void copyright() {
  if (OKA != funciones) {
    lcd.setTextColor(WHITE, BLACK);
    lcd.setTextSize(3);
    lcd.setCursor(0, 150);
    lcd.println("|");
    lcd.setCursor(0, 85);
    lcd.println("|");
    lcd.setCursor(0, 85);
    lcd.setTextColor(WHITE);
    lcd.println("^");
    lcd.setCursor(0, 155);
    lcd.println("v");

  } else {
    pantalla(0, 150, WHITE, BLACK, 3, " ");
    pantalla(0, 150, WHITE, BLACK, 3, " ");
    pantalla(0, 155, WHITE, BLACK, 3, " ");
    pantalla(0, 85, WHITE, BLACK, 3, " ");
    pantalla(0, 85, WHITE, BLACK, 3, " ");
  }

  switch (OKA) {
    case principal: {
        pantalla(49, 3, RED, BLACK, 3, " INICIO  ");
      }
      break;
    case repetir: {
        pantalla(64, 3, RED, BLACK, 3, "REPETIR");
      }
      break;
    case funciones: {
        pantalla(64, 3, RED, BLACK, 3, "FUNCION ");
      }
      break;
  }
  pantalla(0, 232, YELLOW, BLUE, 1, "               MasturGang               ");
}

void pantalla(int x, int y, int color, int fondo, int altura, char texto[30]) {
  lcd.setCursor(x, y);
  lcd.setTextColor(color, fondo);
  lcd.setTextSize(altura);
  lcd.println(texto);
}
