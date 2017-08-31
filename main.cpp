#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <vector>

#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>



#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"


//volatile int x=0,y=0,z=0;
static GLfloat x=0,y=0,z=0;
enum ERRORS{
	ERROR_Create_Socket_File_Descriptor = 1,
	ERROR_Bind,
	ERROR_Accept,
	ERROR_Read,
	ERROR_Thread,
	ERROR_Create_Socket_Echo 
};
int sockets[2];



void  error_print(int X){
	 std::cout<<"Error "<<X<<std::endl;	
	 exit(1);
}

void exit_func(int sig){
		close(sockets[0]);
		close(sockets[1]);
		exit(1);
}
class TCP_connect{

	private:
		int sockfd,sockcl,n;
		int PORT = 8787;
		socklen_t clilen;
		struct sockaddr_in myaddr,cliaddr;
		char buffer[255];

	
	public:
		TCP_connect(){
			sockets[0]=sockfd;
			sockets[1]=sockcl;
		
			sockfd = socket(AF_INET,SOCK_STREAM,0);
			if(sockfd == -1){
				error_print(ERROR_Create_Socket_File_Descriptor);
			}
	
			memset(&myaddr, 0, sizeof(struct sockaddr_in)-1);
			myaddr.sin_family = AF_INET;
			myaddr.sin_addr.s_addr = INADDR_ANY;
			myaddr.sin_port =  htons(PORT);
		
			if( bind(sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1){
				error_print(ERROR_Bind);
			}	
		}
	
		~TCP_connect(){
			close(sockcl);
			close(sockfd);
		}
	
		int lisen(){
			listen(sockfd,5);
			clilen = sizeof(cliaddr);
			sockcl = accept(sockfd,(struct sockaddr *) &cliaddr, &clilen); 
			if(sockcl == -1 ){
				error_print(ERROR_Accept);
			}
			return sockcl;
		}
	

		std::vector<int> parse(){
			std::vector<int> coords;
			int end_x,end_y,end_z;
			std::string tmp;
			std::string source=buffer;
			std::string x_s,y_s,z_s;
			end_x = source.find(",");
			x_s = source.substr(0,end_x);
			tmp = source.substr(end_x+1,source.size());
			end_y = tmp.find(",");
			y_s = source.substr(end_x+1,end_y);
			z_s = tmp.substr(end_y+1,tmp.size());
			coords.push_back(std::stoi(x_s));
			coords.push_back(std::stoi(y_s));
			coords.push_back(std::stoi(z_s));
			x = std::stoi(x_s);
			y = std::stoi(y_s);
			z = std::stoi(z_s);
			//std::cout<<"x"<<x+1<<" "<<"y"<<y<<" "<<z<<std::endl;
			return coords;
		}



		int read_from_socket(){
			while(strcmp(buffer,"exit\n")){
			bzero(buffer,256);
			n = read(sockcl,buffer,256);
			if(n<0){
				error_print(ERROR_Read);	
			}
			//std::cout<<buffer<<std::endl;
			parse();
		}	
		}
	
	
	};






void RenderScene( void )
{

	int bCull = 1;
	int bDepth = 1;
	int bOutline = 1;
	int shadeModel = 1;
  /* Ablak es melysegpuffer torlese */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  /* Poligon hatlapok rajzolasanak ki/be kapcsolasa */
  if( bCull )
    glEnable( GL_CULL_FACE );
  else
    glDisable( GL_CULL_FACE );
  
  /* Melysegellenorzes ki/be kapcsolasa */
  if( bDepth )
    glEnable( GL_DEPTH_TEST );
  else
    glDisable( GL_DEPTH_TEST );
  
  /* Poligon hatlapok korvonallal vagy kitoltve */
  if( bOutline )
    glPolygonMode( GL_BACK,GL_LINE );
  else
    glPolygonMode( GL_BACK,GL_FILL );

  /* Egyszinu vagy szinatmenetes kitoltes */
  if( shadeModel == 1 )
    glShadeModel( GL_FLAT );
  else
    glShadeModel( GL_SMOOTH );

  /* Matrixallapot mentese es forgatas a tengelyek korul */
  glPushMatrix();
  std::cout<<"x: "<<x<<" "<<"y: "<<y<<" "<<"z: "<<z<<std::endl;

  glRotatef( x, 1.0f, 0.0f, 0.0f );
  glRotatef( y, 0.0f, 1.0f, 0.0f );
  glRotatef( z, 0.0f, 0.0f, 1.0f );

  /* Kup modellezese */
  glColor3f(1.0f, 1.0f, 6.0f);	
  glutSolidTeapot(10);

  
  /* Elmentett transzformacio visszaallitasa */
  glPopMatrix();
    
  /* Modellezo parancsok vegrehajtasa */
  glutSwapBuffers();
}

/* Kezdeti megjelenitesi beallitasok */
void SetupRC()
{
  /* Fekete hatterszin */
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  
  /* Egyszinu kitoltesi mod */
  glShadeModel(GL_FLAT);
  
  /* Poligon elolapok az oramutato jarasaval ellentetes iranyban */
  glFrontFace(GL_CCW);
}




void ChangeSize( int w, int h )
{
  GLfloat nRange = 100.0f;
  
  /* Nullaval osztas elkerulese */
  if( h == 0 )
    h = 1;
  
  // Viewport beallitasa az ablak mereteihez */
  glViewport( 0, 0, w, h );
  
  // Reset projection matrix stack
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  
  /* Vagasi terresz beallitasa (bal, jobb, also, felso, kozeli, tavoli) */
  if ( w <= h )
    glOrtho ( -nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange );
  else 
    glOrtho ( -nRange * w / h, nRange * w / h, -nRange, nRange, -nRange, nRange );
  
  /* ModelView matrix inicializalasa */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}




void idle() {
  #define REGULATE_FPS
  #ifdef REGULATE_FPS
  static GLuint PreviousClock=glutGet(GLUT_ELAPSED_TIME);
  static GLuint Clock=glutGet(GLUT_ELAPSED_TIME);
  static GLfloat deltaT;
 
  Clock = glutGet(GLUT_ELAPSED_TIME);
  deltaT=Clock-PreviousClock;
  if (deltaT < 35) {return;} else {PreviousClock=Clock;}
  #endif
 
  printf(".");
  glutPostRedisplay();
}


void* TCP_handler(void *arg){
	TCP_connect myconnect;
	myconnect.lisen();
	myconnect.read_from_socket();
}



int main(int argc, char **argv)
{
	signal(SIGINT,exit_func);
	pthread_t TCP;
	if( pthread_create(&TCP,NULL,&TCP_handler, NULL) < 0){
		error_print(ERROR_Thread);
	}
	//pthread_join(TCP,NULL);
	
	glutInit( &argc, argv );
  	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a triangle");
    glutIdleFunc(idle);
    glutReshapeFunc( ChangeSize );
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutMainLoop();
    return 0;
}
