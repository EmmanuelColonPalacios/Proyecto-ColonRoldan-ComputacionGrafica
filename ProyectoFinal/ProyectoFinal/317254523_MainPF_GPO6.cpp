#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

//Sound
#include <irrKlang.h>

using namespace irrklang;


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();
void puerta();
void resetElements();
void resetElements2();
void resetElements3();
void interpolation();
void interpolation2();
void interpolation3();


//Animaciones
// Puerta
float rotPuerta = 0.0f;
bool activar = false;
bool abierta = false;

//Keyframe plataforma
float plataRota = 0.0f;
float traslaRota = 0.0f;
float plataScaleX = 1.0;
float plataScaleZ = 1.0;

//KeyFrame antenA
float rotRot = -10.0f;
float antenaRot = -10.0f;

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Danny Movement
float DannyX = 0, DannyY = 0, DannyZ = 0;

// Camera
Camera  camera(glm::vec3(-95.0f, 7.0f, -65.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;


float tiempo = 1.0f;
float speed = 1.0f;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;
bool active2;
bool active3;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes

float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z;

//Primera animación compleja
float cafeKF = 4.0f, tapaKF = 3.85f, tapaVKF = 4.0f, vasoVKF = 5.0f, vasoKF = 3.85f;

//Segunda animación compleja
float botonUKF = 5.15f, botonDKF = 5.15f, cajonSecretoKF = 5.15f;

#define MAX_FRAMES 9
// Contador Animación compleja 1.
int i_max_steps = 190;
int i_curr_steps = 0;

// Contador Animación compleja 2.
int i_max_steps2 = 190;
int i_curr_steps2 = 0;


#define MAX_FRAMES 9
int i_max_steps3 = 190;
int i_curr_steps3 = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotInc;

	// Primera animación //
	float cafeKF = 0.0f, cafeInc = 0.0f;
	float tapaKF = 0.0f, tapaInc = 0.0f;
	float tapaVKF = 0.0f, tapaVInc = 0.0f;
	float vasoVKF, vasoVInc;
	float vasoKF = 0.0f, vasoInc = 0.0f;

	// Segunda animación //
	float botonUKF = 0.0f, botonDKF = 0.0f, cajonSecretoKF = 0.0f;
	float botonUInc = 0.0f, botonDInc = 0.0f, cajonSecretoInc = 0.0f;


	//Animacion plataforma
	float plataRota;
	float plataRotaInc;

	float traslaRota;
	float traslaRotaInc;

	float plataScaleX;
	float plataScaleXInc;

	float plataScaleZ;
	float plataScaleZInc;

	//Animacion antena
	float rotRot;
	float rotRotInc;

	float antenaRot;
	float antenaRotInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 4;			//introducir datos
bool play = false;
int playIndex = 0;

FRAME KeyFrame2[MAX_FRAMES];
int FrameIndex2 = 5;			//introducir datos
bool play2 = false;
int playIndex2 = 0;

FRAME KeyFrame3[MAX_FRAMES];
int FrameIndex3 = 5;			//introducir datos
bool play3 = false;
int playIndex3 = 0;

// Variables para ANIMAR

#define CAFE_MOV 0.01f
#define TAPA_MOV 0.1f
#define TAPAV_MAX 4.0f

float rotPuertaD = 0.0f, rotPuertaI= 0.0f, movCajon = 0.0f, movCajon2 = 0.0f;
bool abierto = false;
bool abrir = true;
bool cerrar = false;
bool abiertoCajon = false, abrirCajon = true, cerrarCajon = false;
bool abiertoCajon2 = false, abrirCajon2 = true, cerrarCajon2 = false;




// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY + 11.4,posZ - 5.3),
	glm::vec3(posX + 2.0 ,posY + 6.1, posZ - 4.5),
	glm::vec3(posX,posY + 11.4,posZ + 2.0),
	glm::vec3(0,0,0),
	//Luces del portal :/
	glm::vec3(posX - 4.1,posY + 8.5, posZ - 9.35),
	glm::vec3(posX + 2.3, posY + 8.5, posZ - 9.35),

	//Luces Danny
	glm::vec3(posX,posY + 26.0,posZ - 15.3),
	glm::vec3(posX,posY + 12.0,posZ - 26.0)
};

//Declaramos luces
glm::vec3 LightP1;
glm::vec3 LightP2;
glm::vec3 LightP3;
//interior Danny
glm::vec3 LightP4;
glm::vec3 LightP7;

//Emergencia Danny
glm::vec3 LightP5;
glm::vec3 LightP6;
//Emergencia Danny


int main()
{
	// Init GLFW
	glfwInit();


	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final ColónEmmanuel_RoldánAlexis", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lightingShader2("Shaders/anim2.vs", "Shaders/anim2.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");

	// Pinguino Jetpack modelo //
	Model Pinwi((char*)"Models/Pinwin/Pinwin.obj");
	
	// Fachada //
	Model Edificio((char*)"Models/EdificioGigante/EdificioGigante.obj");
	Model PuertaIzq((char*)"Models/PuertaIzquierda/PuertaIzquierda.obj");
	Model PuertaDer((char*)"Models/PuertaDerecha/PuertaDerecha.obj");
	Model CajonAbajo((char*)"Models/CajonAbajo/CajonAbajo.obj");
	Model CajonArriba((char*)"Models/CajonArriba/CajonArriba.obj");

	// 7 Objetos //
	Model Cabina((char*)"Models/Cabina/Cabina.obj");
	Model CabinaTrans((char*)"Models/CabinaTrans/CabinaTrans.obj"); //Transparente
	Model CabinaBotonUP((char*)"Models/BotonesUP/BotonesUP.obj");
	Model CabinaBotonDOWN((char*)"Models/BotonesDown/BotonesDown.obj");
	Model CajonSecreto((char*)"Models/CajonSecreto/CajonSecreto.obj");
	Model CafeteraMachine((char*)"Models/CafeteraMachine/CafeteraMachine.obj");
	Model Monitor((char*)"Models/Monitor/Monitor.obj");
	Model Mueble((char*)"Models/Mueble/Mueble.obj");
	Model Silla((char*)"Models/Silla/Silla.obj");
	Model TelefonoFijo((char*)"Models/Telefonito/Telefonito.obj");
	Model TelefonoGigante((char*)"Models/TelefonoBIG/TelefonoBIG.obj");


	// Modelos extra //
	Model Piso((char*)"Models/Piso/Piso.obj");
	Model Vaso((char*)"Models/Vaso/Vaso.obj");
	Model Tapa((char*)"Models/Tapa/Tapa.obj");
	Model Liquido((char*)"Models/Liquido/Liquido.obj");

	//Importación de modelos OBJ Danny Phantom
	//Fachada
	Model Casa((char*)"Models/Casa/fachada.obj");
	Model Calle((char*)"Models/Casa/calle.obj");
	//Objetos
	Model Puerta((char*)"Models/Casa/puerta.obj");
	Model Barril((char*)"Models/Barril/barril.obj");
	Model Computadora((char*)"Models/Computadora/computadora.obj");
	Model Locker((char*)"Models/Muebles/locker.obj");
	//Parte de las mesas
	Model Osciloscopio((char*)"Models/Osciloscopio/osciloscopio.obj");
	Model Trastes((char*)"Models/Trastes/trastes.obj");
	Model Barra((char*)"Models/Muebles/barra.obj");
	
	//Plattaforma
	Model Plataforma((char*)"Models/Plataforma/plataforma.obj");
	Model Disco((char*)"Models/Plataforma/discoAnim.obj");

	//Modelo portal
	Model Portal((char*)"Models/Portal/portalVerde.obj");
	Model EstrucPortal((char*)"Models/Portal/portalEstructura.obj");

	//Modelo de antena
	Model Antena((char*)"Models/Antena/antena.obj");
	Model Base((char*)"Models/Antena/base.obj");
	Model Rotor((char*)"Models/Antena/rotor.obj");

	//Personaje Danny Phantom
	Model Danny((char*)"Models/Personaje/DannyPhantom.obj");
	Model Fantasma((char*)"Models/DannyFantasma/DannyFantasma.obj");

	// Build and compile our shader program

	//Inicialización de KeyFrames
	
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].cafeKF = 0;
		KeyFrame[i].cafeInc = 0;
		KeyFrame[i].tapaKF = 0;
		KeyFrame[i].tapaInc = 0;
		KeyFrame[i].tapaVKF = 0;
		KeyFrame[i].tapaVInc = 0;
		KeyFrame[i].vasoVKF = 0;
		KeyFrame[i].vasoVInc = 0;
		KeyFrame[i].vasoKF = 0;
		KeyFrame[i].vasoInc = 0;
	}

	KeyFrame[0].vasoVKF = vasoVKF;
	KeyFrame[0].vasoKF = vasoKF;
	KeyFrame[0].cafeKF = cafeKF;
	KeyFrame[0].tapaKF = tapaKF;
	KeyFrame[0].tapaVKF = tapaVKF;

	KeyFrame[1].vasoKF = 3.83;
	KeyFrame[1].vasoVKF = 4.0f;
	KeyFrame[1].cafeKF = cafeKF;
	KeyFrame[1].tapaKF = tapaKF;
	KeyFrame[1].tapaVKF = tapaVKF;

	KeyFrame[2].vasoKF = 3.8f;
	KeyFrame[2].vasoVKF = 4.0f;
	KeyFrame[2].cafeKF = 3.5f;
	KeyFrame[2].tapaKF = 3.85f;
	KeyFrame[2].tapaVKF = 4.0f;

	KeyFrame[3].vasoKF = 3.75f;
	KeyFrame[3].vasoVKF = 4.0f;
	KeyFrame[3].cafeKF = 4.0f;
	KeyFrame[3].tapaKF = 3.75f;
	KeyFrame[3].tapaVKF = 3.75f;

	for (int w = 0; w < MAX_FRAMES; w++)
	{
		KeyFrame2[w].botonUKF = 0;
		KeyFrame2[w].botonDKF = 0;
		KeyFrame2[w].cajonSecretoKF = 0;
	}

	KeyFrame2[0].botonUKF = botonUKF;
	KeyFrame2[0].botonDKF = botonDKF;
	KeyFrame2[0].cajonSecretoKF = cajonSecretoKF;


	KeyFrame2[1].botonUKF = 5.185;
	KeyFrame2[1].botonDKF = botonDKF;
	KeyFrame2[1].cajonSecretoKF = cajonSecretoKF;


	KeyFrame2[2].botonUKF = 5.185;
	KeyFrame2[2].botonDKF = 5.185;
	KeyFrame2[2].cajonSecretoKF = cajonSecretoKF;

	KeyFrame2[3].botonUKF = 5.185;
	KeyFrame2[3].botonDKF = 5.185;
	KeyFrame2[3].cajonSecretoKF = 4.88;

	
	KeyFrame2[4].botonUKF = 5.15;
	KeyFrame2[4].botonDKF = 5.15;
	KeyFrame2[4].cajonSecretoKF = 4.88;



	for (int j = 0; j < MAX_FRAMES; j++)
	{
		KeyFrame3[j].plataRota = 0;
		KeyFrame3[j].plataRotaInc = 0;

		KeyFrame3[j].traslaRota = 0;
		KeyFrame3[j].traslaRotaInc = 0;

		KeyFrame3[j].plataScaleX = 0;
		KeyFrame3[j].plataScaleXInc = 0;

		KeyFrame3[j].plataScaleZ = 0;
		KeyFrame3[j].plataScaleZInc = 0;

		KeyFrame3[j].rotRot = 0;
		KeyFrame3[j].rotRotInc = 0;

		KeyFrame3[j].antenaRot = 0;
		KeyFrame3[j].antenaRotInc = 0;
	}

	KeyFrame3[0].plataRota = 0.0f;
	//KeyFrame[0].traslaRota = 0.0f;
	KeyFrame3[0].plataScaleX = 1.0f;
	KeyFrame3[0].plataScaleZ = 1.0f;
	KeyFrame3[0].rotRot = -10.0f;

	KeyFrame3[1].plataRota = 180.0f;
	//KeyFrame[1].traslaRota = 0.5f;
	KeyFrame3[1].plataScaleX = 0.10;
	KeyFrame3[1].plataScaleZ = 0.10;
	KeyFrame3[1].rotRot = 10.0f;
	KeyFrame3[1].antenaRot = 0.0f;

	KeyFrame3[2].plataRota = 0.0f;
	KeyFrame3[2].plataScaleX = 0.20;
	KeyFrame3[2].plataScaleZ = 0.20;
	KeyFrame3[2].rotRot = 10.0f;
	KeyFrame3[2].antenaRot = 10.0f;

	KeyFrame3[3].rotRot = 10.0f;
	KeyFrame3[3].antenaRot = 0.0f;


	KeyFrame3[4].rotRot = -10.0f;


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine) {
		std::cout << "Failed to create sound engine" << std::endl;
	}

	engine->play2D("media/DannyIntro.ogg",true);

	



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	glEnable(GL_BLEND);

	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	glDisable(GL_BLEND);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();
		puerta();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2
		glm::vec3 lightColor2;
		lightColor2.x = abs(0.5 * sin(glfwGetTime() * LightP2.x * 2));
		lightColor2.y = abs(0.5 * sin(glfwGetTime() * LightP2.y * 2));
		lightColor2.z = 0.5 * sin(glfwGetTime() * LightP2.z * 0.2);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), LightP2.x, LightP2.y, LightP2.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.075f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), LightP3.x, LightP3.y, LightP3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), LightP3.x, LightP3.y, LightP3.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), LightP4.x, LightP4.y, LightP4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), LightP4.x, LightP4.y, LightP4.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);


		//Luces portal 
		// 
		//Point light 5
		glm::vec3 lightColor5;
		lightColor5.x = abs(sin(glfwGetTime() * LightP5.x));
		lightColor5.y = abs(sin(glfwGetTime() * LightP5.y));
		lightColor5.z = sin(glfwGetTime() * LightP5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), LightP5.x, LightP5.y, LightP5.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.009f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.032f);

		//Luces portal
		//Point light 6
		glm::vec3 lightColor6;
		lightColor6.x = abs(sin(glfwGetTime() * LightP6.x));
		lightColor6.y = abs(sin(glfwGetTime() * LightP6.y));
		lightColor6.z = sin(glfwGetTime() * LightP6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), LightP6.x, LightP6.y, LightP6.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.009f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 0.032f);


		// Point light 7
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), LightP7.x, LightP7.y, LightP7.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), LightP7.x, LightP7.y, LightP7.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 0.032f);




		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

		lightingShader.Use();

		//Carga de modelo s
		view = camera.GetViewMatrix();
		glm::mat4 model(1);


		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-95.0f+DannyX, 5.0f+DannyY, -60.0f+DannyZ));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(DannyX, DannyY, DannyZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Danny.Draw(lightingShader);
		
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-95.0f + DannyX, 5.0f + DannyY, -63.0f + DannyZ));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(DannyX, DannyY, DannyZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Fantasma.Draw(lightingShader);
		
		// Fachada Casa Pingui//
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0.0f, 2.6f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Edificio.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(1.7f, 5.5f, 4.3f));
		model = glm::rotate(model, glm::radians(rotPuertaD), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		PuertaDer.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-1.7f, 5.5f, 4.3f));
		model = glm::rotate(model, glm::radians(rotPuertaI), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		PuertaIzq.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0.0f, 2.6f, movCajon));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		CajonArriba.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0.0f, 2.6f, movCajon2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		CajonAbajo.Draw(lightingShader);

		// Modelo pinwi jetpack //
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-2, 2.8, 8.0));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Pinwi.Draw(lightingShader);


		//Fachada casa de Danny Phantom

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 3, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Casa.Draw(lightingShader);

		//Calle
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 3, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Calle.Draw(lightingShader);

		//Puerta
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 3, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(7.87f, 0.0f, 0.7f));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);


		//Modelos DannyPhantom
		// 		//Antena
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-3.9f, 11.75f, 0.0f));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotRot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(antenaRot), glm::vec3(1.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Antena.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, -3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Base.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, -3.0f));
		model = glm::rotate(model, glm::radians(rotRot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Rotor.Draw(lightingShader);

		//Osciloscopio
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-1.0f, 3.35f, 6.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Osciloscopio.Draw(lightingShader);


		//Barril
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9f, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Barril.Draw(lightingShader);

		//Computadora
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9f, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(2.0f, 1.4f, 2.80f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Computadora.Draw(lightingShader);

		//Barra 1
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9f, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 1.4f, -4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Barra.Draw(lightingShader);

		//Barra 2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(2.5f, 1.4f, 3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Barra.Draw(lightingShader);

		//Trastes1
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(2.0f, 1.4f, 3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Trastes.Draw(lightingShader);

		//Trastes2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 1.4f, 3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Trastes.Draw(lightingShader);

		//Trastes3
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(2.0f, 1.4f, -4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Trastes.Draw(lightingShader);
		//Trastes4
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(2.0f, 1.4f, -4.0f));
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Trastes.Draw(lightingShader);

		//Locker
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(2.0f, 1.4f, 3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Locker.Draw(lightingShader);

		//Plataforma
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-7.0f, 1.0f, -5.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Plataforma.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.9f, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::translate(model, glm::vec3(-5.1f, 1.0f, 7.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, traslaRota, 0.0f));
		model = glm::scale(model, glm::vec3(plataScaleX, 1.0f, plataScaleZ));
		model = glm::rotate(model, glm::radians(plataRota), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Disco.Draw(lightingShader);

		//Portal
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.95f, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(6.5f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		EstrucPortal.Draw(lightingShader);


		// .Draw de modelos elegidos en el proyecto //

		// Cabina //
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(5.15f, 3.9f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		Cabina.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(botonUKF, 3.9f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		CabinaBotonUP.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(botonDKF, 3.9f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		CabinaBotonDOWN.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(cajonSecretoKF, 3.9f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
		CajonSecreto.Draw(lightingShader);

		// objetos transparentes //

		glEnable(GL_BLEND); //inicia transparencia

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(5.15f, 3.9f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 4.0f);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		CabinaTrans.Draw(lightingShader);
		
		

		glDisable(GL_BLEND); //termina transparencia

		// Teléfonos gigantes //
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(4.0f, 3.9f, 3.5f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		TelefonoGigante.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(4.0f, 3.9f, -3.5f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		TelefonoGigante.Draw(lightingShader);

		// Muebles //
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-1.0f, 3.9f, -3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Mueble.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-2.5f, 3.9f, -1.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Mueble.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-1.2f, 3.9f, -3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Mueble.Draw(lightingShader);
		

		/*view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(1.2f, 3.9f, -3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Mueble.Draw(lightingShader);
		*/

		// Teléfonos fijos //

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-3.9f, 4.6f, 1.5f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		TelefonoFijo.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-1.6f, 4.6f, 1.8f));
		model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		TelefonoFijo.Draw(lightingShader);

		/*view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(3.75f, 4.6f, 1.8f));
		model = glm::rotate(model, glm::radians(175.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		TelefonoFijo.Draw(lightingShader);
		*/
	
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(3.75f, 4.6f, -1.65f));
		model = glm::rotate(model, glm::radians(210.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		TelefonoFijo.Draw(lightingShader);

		// Sillas //

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(1.3f, 3.85f, 2.0f));
		model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Silla.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 3.85f, 2.0f));
		model = glm::rotate(model, glm::radians(-82.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Silla.Draw(lightingShader);

		
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(1.3f, 3.85f, 2.0f));
		model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Silla.Draw(lightingShader);
		

		// Monitores //

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-3.9f, 4.6f, 0.5f));
		model = glm::rotate(model, glm::radians(84.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Monitor.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-1.8f, 4.6f, 3.0f));
		model = glm::rotate(model, glm::radians(112.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Monitor.Draw(lightingShader);

		

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(3.75f, 4.6f, -0.6f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Monitor.Draw(lightingShader);

		

		

		// Máquina de café //

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(3.75f, 4.0f, 1.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		CafeteraMachine.Draw(lightingShader);


		// Extra //

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		Piso.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(vasoKF, vasoVKF, 1.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Vaso.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(3.75f, 4.0f, 1.3f));
		model = glm::translate(model, glm::vec3(tapaKF, tapaVKF, 1.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Tapa.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(3.75f, cafeKF, 1.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Liquido.Draw(lightingShader);

	
		//Espectro
		tiempo = glfwGetTime() * speed;
		animShader.Use();

		modelLoc = glGetUniformLocation(animShader.Program, "model");
		viewLoc = glGetUniformLocation(animShader.Program, "view");
		projLoc = glGetUniformLocation(animShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(animShader.Program, "time"), tiempo);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(0, 1.95f, 0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0F, -17.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(1.8f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.3f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Portal.Draw(animShader);

		//Traslucidez

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0);
		//objTras.Draw(lightingShader);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 8; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		

		// skybox cube
		glEnable(GL_BLEND);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
		
		glDisable(GL_BLEND);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	engine->drop();



	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{
	// Animación sencilla 1. Puerta Abrir/Cerrar, activado con tecla P //
	if (abierto) {

		if (abrir)
		{
			printf("\nQue se abran las puertas!");
			rotPuertaD -= 0.2;
			rotPuertaI += 0.2;
			if (rotPuertaD < -66.0f)
			{
				abrir = false;
			}

		}

		if (cerrar)
		{
			printf("\nQue se cierren las puertas!");
			rotPuertaD += 0.2;
			rotPuertaI -= 0.2;
			if (rotPuertaD >= 0.0f)
			{
				cerrar = false;

			}
		}

	}

	// Animación sencilla 2. Cajón abierto y cerrado, activado con tecla 9 el cajón de arriba y el de abajo con O //

	if (abiertoCajon) {

		if (abrirCajon)
		{
			movCajon += 0.01;
			if (movCajon >= 0.5f)
			{
				abrirCajon = false;
			}

		}

		if (cerrarCajon)
		{
			movCajon -= 0.01;
			if (movCajon <= 0.02f)
			{
				cerrarCajon = false;

			}
		}

	}

	if (abiertoCajon2) {

		if (abrirCajon2)
		{
			movCajon2 += 0.01;
			if (movCajon2 >= 0.5f)
			{
				abrirCajon2 = false;
			}

		}

		if (cerrarCajon2)
		{
			movCajon2 -= 0.01;
			if (movCajon2 <= 0.02f)
			{
				cerrarCajon2 = false;

			}
		}

	}

	//Animación compleja 1. Café
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("\nCafe servido!");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation

			cafeKF += KeyFrame[playIndex].cafeInc;
			tapaKF += KeyFrame[playIndex].tapaInc;
			tapaVKF += KeyFrame[playIndex].tapaVInc;
			vasoVKF += KeyFrame[playIndex].vasoVInc;
			vasoKF += KeyFrame[playIndex].vasoInc;

			i_curr_steps++;
		}

	}

	//Animación compleja 2. Cajón secreto
	if (play2)
	{
		if (i_curr_steps2 >= i_max_steps2) //end of animation between frames?
		{
			playIndex2++;
			if (playIndex2 > FrameIndex2 - 2)	//end of total animation?
			{
				printf("\nCajon desbloqueado, toma tu equipamiento!");
				playIndex2 = 0;
				play2 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps2 = 0; //Reset counter
				//Interpolation
				interpolation2();
			}
		}
		else
		{
			//Draw animation

			botonUKF += KeyFrame2[playIndex2].botonUInc;
			botonDKF += KeyFrame2[playIndex2].botonDInc;
			cajonSecretoKF += KeyFrame2[playIndex2].cajonSecretoInc;

			i_curr_steps2++;
		}

	}

	//Animación plataforma por Keyframes
	if (play3)
	{
		if (i_curr_steps3 >= i_max_steps3) //end of animation between frames?
		{
			playIndex3++;
			if (playIndex3 > FrameIndex3 - 2)	//end of total animation?
			{
				playIndex3 = 0;
				play3 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps3 = 0; //Reset counter
				//Interpolation
				interpolation3();
			}
		}
		else
		{
			//Draw animation
			plataRota += KeyFrame3[playIndex3].plataRotaInc;
			traslaRota += KeyFrame3[playIndex3].traslaRota;
			plataScaleX += KeyFrame3[playIndex3].plataScaleX * 0.01;
			plataScaleZ += KeyFrame3[playIndex3].plataScaleZ * 0.01;
			//Antena
			rotRot += KeyFrame3[playIndex3].rotRotInc;
			antenaRot += KeyFrame3[playIndex3].antenaRotInc;

			i_curr_steps3++;
		}

	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode){
	if (keys[GLFW_KEY_8])
	{
		if (play == false && (FrameIndex > 1))
		{
			printf("\nSirviendo café!");
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			printf("\n Cafe servido!");
			play = false;
		}

	}

	if (keys[GLFW_KEY_7])
	{
		if (play2 == false && (FrameIndex2 > 1))
		{
			printf("\nSecuencia secreta activada, desbloqueando cajón secreto!");
			resetElements2();
			//First Interpolation				
			interpolation2();

			play2 = true;
			playIndex2 = 0;
			i_curr_steps2 = 0;
		}
		else
		{
			printf("\nCajón mostrado!");
			play2 = false;
		}

	}

	if (keys[GLFW_KEY_M])
	{
		if (play == false && (FrameIndex3 > 1))
		{

			resetElements3();
			//First Interpolation				
			interpolation3();

			play3 = true;
			playIndex3 = 0;
			i_curr_steps = 0;
		}
		else
		{
			play3 = false;
		}

	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	//Animación luces 
	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active) {

			LightP1 = glm::vec3(1.0f, 1.0f, 1.0f);
			LightP3 = glm::vec3(1.0f, 1.0f, 1.0f);
			LightP4 = glm::vec3(1.0f, 1.0f, 1.0f);
			LightP7 = glm::vec3(1.0f, 1.0f, 1.0f);

		}
		else{
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
			LightP3 = glm::vec3(0.0f, 0.0f, 0.0f);
			LightP4 = glm::vec3(0.0f, 0.0f, 0.0f);
			LightP7 = glm::vec3(0.0f, 0.0f, 0.0f);

		}
	}

	//Animación luces de emergencia de local. 
	if (keys[GLFW_KEY_N])
	{
		active2 = !active2;
		if (active2) {
			//LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
			LightP5 = glm::vec3(1.0f, 0.0f, 0.0f);
			LightP6 = glm::vec3(1.0f, 0.0f, 0.0f);
	
		}
		else {
			//LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
			LightP5 = glm::vec3(0);
			LightP6 = glm::vec3(0);

		}
	}

	// Animación sencilla 3. Luces de la cabina telefónica encienden y apagan, se activa con L //

	if (keys[GLFW_KEY_L])
	{
		
		active3 = !active3;
		if (active3) {
			LightP2 = glm::vec3(1.0f, 0.0f, 0.0f);
			printf("Llamada entrante, luces de la cabina encendidas!");
		}
		else {

			LightP2 = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	// camera.ProcessMouseMovement(xOffset, yOffset);
	camera.ProcessMouseMovement(xOffset, 0);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		
		movCamera = 0.01f;//Manda una velocidad de 0.01 a la camara automatica

	}

	if (keys[GLFW_KEY_U])
	{
		activar = true;
	}


	if (keys[GLFW_KEY_P])
	{
		abierto = true;
		if (rotPuertaD < -65.0f) {
			
			cerrar = true;
			abrir = false;
		}
		else if (rotPuertaD > 0.0f) {
			printf("\nQue se abran las puertas!");
			cerrar = false;
			abrir = true;
			
		}
			
	}

	if (keys[GLFW_KEY_9])
	{
		abiertoCajon = true;
		if (movCajon >= 0.5f) {
			printf("\nCajon Cerrado!");
			cerrarCajon = true;
			abrirCajon = false;
		}
		else if (movCajon <= 0.02f) {
			printf("\nCajon Abierto!");
			cerrarCajon = false;
			abrirCajon = true;

		}

	}

	if (keys[GLFW_KEY_O])
	{
		abiertoCajon2 = true;
		if (movCajon2 >= 0.5f) {
			printf("\nCajon Cerrado!");
			cerrarCajon2 = true;
			abrirCajon2 = false;
		}
		else if (movCajon2 <= 0.02f) {
			printf("Cajon Abierto!");
			cerrarCajon2 = false;
			abrirCajon2 = true;

		}

	}

	// Camera controls
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
		DannyZ += 0.1f;

	}

	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		DannyZ -= 0.1f;

	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
		DannyX += 0.1f;

	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
		DannyX -= 0.1f;
	}

}

//Animación puerta fachada Danny
void puerta() {
	if (activar) {
		if (abierta) { //Si la puerta está abierta
			if (rotPuerta == 0.0f) {
				abierta = false;
				activar = false;
			}
			else {
				rotPuerta -= 1.0f;
			}
		}
		else { //Si la puerta está cerrada
			if (rotPuerta == 90.0f) {
				abierta = true;
				activar = false;
			}
			else {
				rotPuerta += 1.0f;
			}
		}
	}
}


void resetElements(void)
{
	cafeKF = KeyFrame[0].cafeKF;
	tapaKF = KeyFrame[0].tapaKF;
	tapaVKF = KeyFrame[0].tapaVKF;
	vasoVKF = KeyFrame[0].vasoVKF;
	vasoKF = KeyFrame[0].vasoKF;

}

void resetElements2(void)
{
	botonUKF = KeyFrame2[0].botonUKF;
	botonDKF = KeyFrame2[0].botonDKF;
	cajonSecretoKF = KeyFrame2[0].cajonSecretoKF;
}

//Animación platillo de plataforma y antena reset keyframes
void resetElements3(void)
{
	plataRota = KeyFrame3[0].plataRota;
	traslaRota = KeyFrame3[0].traslaRota;
	plataScaleX = KeyFrame3[0].plataScaleX;
	plataScaleZ = KeyFrame3[0].plataScaleZ;
	rotRot = KeyFrame3[0].rotRot;
	antenaRot = KeyFrame3[0].antenaRot;

}

void interpolation(void)
{

	KeyFrame[playIndex].cafeInc = (KeyFrame[playIndex + 1].cafeKF - KeyFrame[playIndex].cafeKF) / i_max_steps;
	KeyFrame[playIndex].tapaInc = (KeyFrame[playIndex + 1].tapaKF - KeyFrame[playIndex].tapaKF) / i_max_steps;
	KeyFrame[playIndex].tapaVInc = (KeyFrame[playIndex + 1].tapaVKF - KeyFrame[playIndex].tapaVKF) / i_max_steps;
	KeyFrame[playIndex].vasoVInc = (KeyFrame[playIndex + 1].vasoVKF - KeyFrame[playIndex].vasoVKF) / i_max_steps;
	KeyFrame[playIndex].vasoInc = (KeyFrame[playIndex + 1].vasoKF - KeyFrame[playIndex].vasoKF) / i_max_steps;
}

void interpolation2(void)
{

	KeyFrame2[playIndex2].botonUInc = (KeyFrame2[playIndex2 + 1].botonUKF - KeyFrame2[playIndex2].botonUKF) / i_max_steps;
	KeyFrame2[playIndex2].botonDInc = (KeyFrame2[playIndex2 + 1].botonDKF - KeyFrame2[playIndex2].botonDKF) / i_max_steps;
	KeyFrame2[playIndex2].cajonSecretoInc = (KeyFrame2[playIndex2 + 1].cajonSecretoKF - KeyFrame2[playIndex2].cajonSecretoKF) / i_max_steps;

}

//Animación platillo de plataforma y antena, interpolación
void interpolation3(void)
{
	KeyFrame3[playIndex3].plataRotaInc = (KeyFrame3[playIndex3 + 1].plataRota - KeyFrame3[playIndex3].plataRota) / i_max_steps;
	KeyFrame3[playIndex3].traslaRotaInc = (KeyFrame3[playIndex3 + 1].traslaRota - KeyFrame3[playIndex3].traslaRota) / i_max_steps;
	KeyFrame3[playIndex3].plataScaleXInc = (KeyFrame3[playIndex3 + 1].plataScaleX - KeyFrame3[playIndex3].plataScaleX) / i_max_steps;
	KeyFrame3[playIndex3].plataScaleZInc = (KeyFrame3[playIndex3 + 1].plataScaleZ - KeyFrame3[playIndex3].plataScaleZ) / i_max_steps;
	//Antena
	KeyFrame3[playIndex3].rotRotInc = (KeyFrame3[playIndex3 + 1].rotRot - KeyFrame3[playIndex3].rotRot) / i_max_steps;
	KeyFrame3[playIndex3].antenaRotInc = (KeyFrame3[playIndex3 + 1].antenaRot - KeyFrame3[playIndex3].antenaRot) / i_max_steps;
}