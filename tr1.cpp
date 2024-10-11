#include <GL/glut.h>
#include <iostream>


// Dimensiones del botón linea pendiente menor 1
int botonX0 = -70, botonY0 = -45, botonX1 = -25, botonY1 = -35;
bool botonPresionado = false;
// Dimensiones del botón linea pendiente mayor 1
int botonX0_2 = -70, botonY0_2 = -25, botonX1_2 = -25, botonY1_2 = -15;
bool botonPresionado_2 = false;

// Prototipos
void inicio(void);
void display(void);
void myreshape(int, int);
void puntoMedio(int, int, int, int);
void renderBitmapString(float x, float y, void* font, const char* string);
void mouse(int button, int state, int x, int y);
void dibujarBoton();
void dibujarBoton_2();
void dibujarGrafico();

// Función para actualizar el gráfico con nuevas coordenadas
void actualizarGrafico() {
	//botonPresionado = true;
    glutPostRedisplay(); // Redibuja la ventana principal
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simulacion de Boton con GLUT");

    inicio();
    glutDisplayFunc(display);
    glutReshapeFunc(myreshape);
    glutMouseFunc(mouse); // Registra el manejador de eventos de mouse

    glutMainLoop();
    return 0;
}

void inicio(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    // Dibuja el botón a la izquierda
    dibujarBoton();

	dibujarBoton_2();

    // Dibuja el gráfico a la derecha
    dibujarGrafico();

    glPopMatrix();
    glFlush();
}

void myreshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (GLfloat)w / (GLfloat)h;

    if (w <= h) {
        float adjustedHeight = 50.0 * aspect;
        glOrtho(-50.0, 50.0, -adjustedHeight, adjustedHeight, -10, 10);
    }
    else {
        float adjustedWidth = 50.0 * aspect;
        glOrtho(-adjustedWidth, adjustedWidth, -50.0, 50.0, -10, 10);
        
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// Algoritmo del punto medio para dibujar la línea
void puntoMedio(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int incX = (x1 > x0) ? 1 : -1;
    int incY = (y1 > y0) ? 1 : -1;
    bool pendienteMayorA1 = dy > dx;

    if (pendienteMayorA1) {
        std::swap(dx, dy);
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(incX, incY);
    }

    int d = (2 * dy) - dx;
    int dE = 2 * dy;
    int dNE = 2 * (dy - dx);
    int x = x0;
    int y = y0;

    glBegin(GL_POINTS);
    if (pendienteMayorA1) {
        glVertex2f(y, x);
    }
    else {
        glVertex2f(x, y);
    }

    while (x != x1) {
        if (d <= 0) {
            d += dE;
        }
        else {
            d += dNE;
            y += incY;
        }
        x += incX;

        if (pendienteMayorA1) {
            glVertex2f(y, x);
        }
        else {
            glVertex2f(x, y);
        }
    }
    glEnd();
}

// Dibuja el botón como un rectángulo en la columna izquierda
void dibujarBoton() {
    glPushMatrix();
    // Dibuja el rectángulo del botón
    glColor3f(0.7, 0.7, 0.7); // Color gris
    glBegin(GL_QUADS);
    glVertex2f(botonX0, botonY0);
    glVertex2f(botonX1, botonY0);
    glVertex2f(botonX1, botonY1);
    glVertex2f(botonX0, botonY1);
    glEnd();

    // Dibuja el texto del botón
    glColor3f(0, 0, 0); // Color negro para el texto
    renderBitmapString(botonX0 + 2, botonY0 + 2, GLUT_BITMAP_HELVETICA_18, "Linea con pendiente < 1");
    glPopMatrix();
}
// Boton linea pendiente mayor 1
void dibujarBoton_2() {
    glPushMatrix();
    // Dibuja el rectángulo del botón
    glColor3f(0.7, 0.7, 0.7); // Color gris
    glBegin(GL_QUADS);
    glVertex2f(botonX0_2, botonY0_2);
    glVertex2f(botonX1_2, botonY0_2);
    glVertex2f(botonX1_2, botonY1_2);
    glVertex2f(botonX0_2, botonY1_2);
    glEnd();

    // Dibuja el texto del botón
    glColor3f(0, 0, 0); // Color negro para el texto
    renderBitmapString(botonX0_2 + 2, botonY0_2 + 2, GLUT_BITMAP_HELVETICA_18, "Linea con pendiente > 1");
    glPopMatrix();
}

void dibujarGrafico() {
    // Variables para las coordenadas de la línea
    int px0 = 0, py0 = 0, px1 = 0, py1 = 0;

    glPushMatrix();

    glTranslatef(20.0f, 0.0f, 0.0f);

    // Dibuja los ejes
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-20, 0);
    glVertex2f(50, 0);
    glVertex2f(0, -50);
    glVertex2f(0, 50);
    glEnd();
    
    if (botonPresionado) {
        px0 = 0; py0 = 0; px1 = 30; py1 = 15;
        // Dibuja la línea con el algoritmo de punto medio
        glColor3f(0, 0, 1); // Azul
        glPointSize(5); // Fija el grosor de pixel
        puntoMedio(px0, py0, px1, py1);

        // Convertir las coordenadas a cadenas de texto
        char coordInicio[50], coordFin[50];
        sprintf_s(coordInicio, "(%d, %d)", px0, py0);
        sprintf_s(coordFin, "(%d, %d)", px1, py1);

        // Muestra las coordenadas en los extremos de la línea
        glColor3f(1, 0, 0); // Rojo para el texto
        renderBitmapString(px0 + 1, py0 + 1, GLUT_BITMAP_HELVETICA_12, coordInicio); // Coordenada inicial
        renderBitmapString(px1 + 1, py1 + 1, GLUT_BITMAP_HELVETICA_12, coordFin); // Coordenada final
    }
	else if (botonPresionado_2) {
		px0 = 0; py0 = 0; px1 = 15; py1 = 30;
		// Dibuja la línea con el algoritmo de punto medio
		glColor3f(0, 0, 1); // Azul
		glPointSize(5); // Fija el grosor de pixel
		puntoMedio(px0, py0, px1, py1);

		// Convertir las coordenadas a cadenas de texto
		char coordInicio[50], coordFin[50];
		sprintf_s(coordInicio, "(%d, %d)", px0, py0);
		sprintf_s(coordFin, "(%d, %d)", px1, py1);

		// Muestra las coordenadas en los extremos de la línea
		glColor3f(1, 0, 0); // Rojo para el texto
		renderBitmapString(px0 + 1, py0 + 1, GLUT_BITMAP_HELVETICA_12, coordInicio); // Coordenada inicial
		renderBitmapString(px1 + 1, py1 + 1, GLUT_BITMAP_HELVETICA_12, coordFin); // Coordenada final
	}
    else {
        glColor3f(1.0f, 0.0f, 0.0f); // Color rojo
        glBegin(GL_QUADS); // Comenzar a dibujar un cuadrado
        glVertex2f(5.0f, 5.0f); // Esquina inferior izquierda
        glVertex2f(15.0f, 5.0f);  // Esquina inferior derecha
        glVertex2f(15.0f, 15.0f);   // Esquina superior derecha
        glVertex2f(5.0f, 15.0f);  // Esquina superior izquierda
        glEnd();
    }
    

    glPopMatrix();
}



// Detectar clics del mouse
void mouse(int button, int state, int x, int y) {
    // Convertir las coordenadas de pantalla a coordenadas de OpenGL
    int ventanaAncho = glutGet(GLUT_WINDOW_WIDTH);
    int ventanaAlto = glutGet(GLUT_WINDOW_HEIGHT);

    // Valores de glOrtho usados en reshape
    float left, right, bottom, top;
    float aspect = (GLfloat)ventanaAncho / (GLfloat)ventanaAlto;

    // Calcular las coordenadas de acuerdo al reshape
    if (ventanaAncho <= ventanaAlto) {
        float adjustedHeight = 50.0 * aspect;
        left = -50.0;
        right = 50.0;
        bottom = -adjustedHeight;
        top = adjustedHeight;
    }
    else {
        float adjustedWidth = 50.0 * aspect;
        left = -adjustedWidth;
        right = adjustedWidth;
        bottom = -50.0;
        top = 50.0;
    }

    // Mapear las coordenadas del mouse a las coordenadas de OpenGL
    float glX = left + (right - left) * ((float)x / (float)ventanaAncho);
    float glY = top - (top - bottom) * ((float)y / (float)ventanaAlto);


    // Imprimir las coordenadas para verificar
    std::cout << "Mouse click at (" << glX << ", " << glY << ")\n";

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Verifica si el clic está dentro del área del botón
        if (glX >= botonX0 && glX <= botonX1 && glY <= botonY1 && glY >= botonY0) {
            std::cout << "Botón presionado!" << std::endl;
            if (botonPresionado) {
				botonPresionado = true;
				botonPresionado_2 = false;
			}
            else {
                botonPresionado = true;
				botonPresionado_2 = false;
            }
            
            actualizarGrafico(); // Llama a la función para actualizar el gráfico
		}
		else if (glX >= botonX0_2 && glX <= botonX1_2 && glY <= botonY1_2 && glY >= botonY0_2) {
			std::cout << "Botón presionado" << std::endl;
			if (botonPresionado_2) {
				botonPresionado_2 = true;
				botonPresionado = false;
			}
			else {
				botonPresionado_2 = true;
				botonPresionado = false;
			}
			actualizarGrafico(); // Llama a la función para actualizar el gráfico
		}
        else {
            std::cout << "Clic fuera del botón." << std::endl;
        }
    }
}


// Función para dibujar texto en pantalla
void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
