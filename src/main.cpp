#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <exception>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argc, char *argv[])
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw std::exception();
  }

  SDL_Window *window = SDL_CreateWindow("Triangle",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE |
	SDL_WINDOW_OPENGL);

  if(!SDL_GL_CreateContext( window ))
  {
	  throw std::exception();
  }

  if(glewInit() != GLEW_OK)
  {
	  throw std::exception();
  }





  const GLfloat positions[] =
  {
	  0.0f, 0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f
  };

  GLuint positionsVboId = 0;

  glGenBuffers( 1, &positionsVboId );

  if(!positionsVboId)
  {
	  throw std::exception();
  }

  glBindBuffer( GL_ARRAY_BUFFER, positionsVboId );
  glBufferData( GL_ARRAY_BUFFER, sizeof( positions ), positions, GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  GLuint vaoId = 0;
  
  glGenVertexArrays( 1, &vaoId );

  if(!vaoId)
  {
	  throw std::exception();
  }

  glBindVertexArray( vaoId );

  glBindBuffer( GL_ARRAY_BUFFER, positionsVboId );

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE,
	  3 * sizeof( GLfloat ), ( void * ) 0 );
  glEnableVertexAttribArray( 0 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );

  const GLchar *vertexShaderSrc =
	  "attribute vec3 in_Position;				"\
	  "											"\
	  "void main()								"\
	  "{										"\
	  "	gl_Position = vec4(in_Position, 1.0);	"\
	  "}										";

  GLuint vertexShaderId = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vertexShaderId, 1, &vertexShaderSrc, NULL );
  glCompileShader( vertexShaderId );
  GLint success = 0;
  glGetShaderiv( vertexShaderId, GL_COMPILE_STATUS, &success );

  if(!success)
  {
	  throw std::exception();
  }

  const GLchar *fragmentShaderSrc =
	  "void main()							"\
	  "{									"\
	  "gl_FragColor = vec4(0, 0, 1, 1);		"\
	  "}									";

  GLuint fragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fragmentShaderId, 1, &fragmentShaderSrc, NULL );
  glCompileShader( fragmentShaderId );
  glGetShaderiv( fragmentShaderId, GL_COMPILE_STATUS, &success );

  if(!success)
  {
	  throw std::exception();
  }

  GLuint programId = glCreateProgram();
  glAttachShader( programId, vertexShaderId );
  glAttachShader( programId, fragmentShaderId );

  glBindAttribLocation( programId, 0, "in_Position" );

  glLinkProgram( programId );
  glGetProgramiv( programId, GL_LINK_STATUS, &success );

  if(!success)
  {
	  throw std::exception();
  }

  glDetachShader( programId, vertexShaderId );
  glDeleteShader( vertexShaderId );
  glDetachShader( programId, fragmentShaderId );
  glDeleteShader( fragmentShaderId );




  bool quit = false;

  while (!quit)
  {
	  SDL_Event event = { 0 };

	  while (SDL_PollEvent(&event))
	  {
		  if (event.type == SDL_QUIT)
		  {
			  quit = true;
		  }
	  }

	  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	  glClear(GL_COLOR_BUFFER_BIT);
	  

	  glUseProgram(programId);
	  glBindVertexArray(vaoId);

	  glDrawArrays(GL_TRIANGLES, 0, 3);

	  glBindVertexArray(0);
	  glUseProgram(0);

	  SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
