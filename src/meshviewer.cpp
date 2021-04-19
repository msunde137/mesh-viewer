// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include "mesh.h"
#include "osutils.h"

using namespace std;
using namespace glm;
using namespace agl;

// globals
Mesh theModel;
int theCurrentModel = 0;
vector<string> theModelNames;

// OpenGL IDs
GLuint theVboPosId;
GLuint theVboNormalId;
GLuint theElementbuffer;

// transforms
mat4 view;
mat4 model;
mat4 projection;

// window
int width = 500;
int height = 500;

void GLPrintUniformInfo(unsigned int program)
{
    int num_uniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &num_uniforms);
    char uniform_name[256];
    int length;
    int size;
    unsigned int type;
    for (int i = 0; i < num_uniforms; i++)
    {
        glGetActiveUniform(program, i, sizeof(uniform_name), &length, &size, &type, uniform_name);
        std::cout << "Name: " << uniform_name << ", Length: " <<
            length << ", Size: " << size << ", Type: " << type << std::endl;
    }
}

static void LoadModel(int modelId)
{
   assert(modelId >= 0 && modelId < theModelNames.size());
   theModel.loadPLY(theModelNames[theCurrentModel]);

   glBindBuffer(GL_ARRAY_BUFFER, theVboPosId);
   glBufferData(GL_ARRAY_BUFFER, theModel.numVertices() * 3 * sizeof(float), theModel.positions(), GL_DYNAMIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, theVboNormalId);
   glBufferData(GL_ARRAY_BUFFER, theModel.numVertices() * 3 * sizeof(float), theModel.normals(), GL_DYNAMIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theElementbuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, theModel.numTriangles() * 3 * sizeof(unsigned int), theModel.indices(), GL_DYNAMIC_DRAW);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action != GLFW_PRESS) return;

   if (key == GLFW_KEY_ESCAPE)
   {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
   else if (key == 'P')
   {
      if (--theCurrentModel < 0)
      {
         theCurrentModel = theModelNames.size() - 1;
      }
      cout << "Current file: " << theModelNames[theCurrentModel] << endl;
      LoadModel(theCurrentModel);
   }
   else if (key == 'N')
   {
      theCurrentModel = (theCurrentModel + 1) % theModelNames.size(); 
      cout << "Current file: " << theModelNames[theCurrentModel] << endl;
      LoadModel(theCurrentModel);
   }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   // Prevent a divide by zero
   if(height == 0) height = 1;
	
   // Set Viewport to window dimensions
   glViewport(0, 0, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
   double xpos, ypos;
   glfwGetCursorPos(window, &xpos, &ypos);

   // TODO: Camera controls

   int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
   if (state == GLFW_PRESS)
   {
       int keyPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
       if (keyPress == GLFW_PRESS) {}
   }
   else if (state == GLFW_RELEASE)
   {
   }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
   // TODO: Camera controls
}

static void PrintShaderErrors(GLuint id, const std::string label)
{
   std::cerr << label << " failed\n";
   GLint logLen;
   glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
   if (logLen > 0)
   {
      char* log = (char*)malloc(logLen);
      GLsizei written;
      glGetShaderInfoLog(id, logLen, &written, log);
      std::cerr << "Shader log: " << log << std::endl;
      free(log);
   }
}

static std::string LoadShaderFromFile(const std::string& fileName)
{
   std::ifstream file(fileName);
   if (!file)
   {
      std::cout << "Cannot load file: " << fileName << std::endl;
      return "";
   }

   std::stringstream code;
   code << file.rdbuf();
   file.close();

   return code.str();
}

static void LoadModels(const std::string& dir)
{
   std::vector<std::string> filenames = GetFilenamesInDir(dir, "ply");
   for (int i = 0; i < filenames.size(); i++)
   {
      std::string filename = filenames[i];
      theModelNames.push_back(dir+filename);
   }
}

static GLuint LoadShader(const std::string& vertex, const std::string& fragment)
{
   GLint result;
   std::string vertexShader = LoadShaderFromFile(vertex);
   const char* vertexShaderRaw = vertexShader.c_str();
   GLuint vshaderId = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vshaderId, 1, &vertexShaderRaw, NULL);
   glCompileShader(vshaderId);
   glGetShaderiv(vshaderId, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(vshaderId, "Vertex shader");
      return -1;
   }

   std::string fragmentShader = LoadShaderFromFile(fragment);
   const char* fragmentShaderRaw = fragmentShader.c_str();
   GLuint fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fshaderId, 1, &fragmentShaderRaw, NULL);
   glCompileShader(fshaderId);
   glGetShaderiv(fshaderId, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(fshaderId, "Fragment shader");
      return -1;
   }

   GLuint shaderId = glCreateProgram();
   glAttachShader(shaderId, vshaderId);
   glAttachShader(shaderId, fshaderId);
   glLinkProgram(shaderId);
   glGetShaderiv(shaderId, GL_LINK_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(shaderId, "Shader link");
      return -1;
   }

   glDetachShader(shaderId, fshaderId);
   glDetachShader(shaderId, vshaderId);
   glDeleteShader(fshaderId);
   glDeleteShader(vshaderId);
   return shaderId;
}


int main(int argc, char** argv)
{
   GLFWwindow* window;

   if (!glfwInit())
   {
      return -1;
   }

   // Explicitly ask for a 4.0 context 
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   /* Create a windowed mode window and its OpenGL context */
   window = glfwCreateWindow(width, height, "Mesh Viewer", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   // Make the window's context current 
   glfwMakeContextCurrent(window);

   glfwSetKeyCallback(window, key_callback);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetMouseButtonCallback(window, mouse_button_callback);
   glfwSetScrollCallback(window, scroll_callback);
   glfwSetCursorPosCallback(window, cursor_position_callback);

#ifndef APPLE
   if (glewInit() != GLEW_OK)
   {
      return -1;
   }
#endif

   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_CULL_FACE);
   glClearColor(0, 0, 0, 1);

   glGenBuffers(1, &theVboPosId);
   glGenBuffers(1, &theVboNormalId);
   glGenBuffers(1, &theElementbuffer);

   GLuint vaoId;
   glGenVertexArrays(1, &vaoId);
   glBindVertexArray(vaoId);

   glBindBuffer(GL_ARRAY_BUFFER, theVboPosId); // always bind before setting data
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLubyte*)NULL);
   glEnableVertexAttribArray(0); // 0 -> Sending VertexPositions to array #0 in the active shader

   glBindBuffer(GL_ARRAY_BUFFER, theVboNormalId); // always bind before setting data
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLubyte*)NULL);
   glEnableVertexAttribArray(1); // 1 -> Sending Normals to array #1 in the active shader

   // load model
   LoadModels("../models/");
   LoadModel(0);

   float triangle[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top  
   };
   float triangle_norms[] = {
       0.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f
   };
   unsigned int triangle_inds[]{
       0, 1, 2
   };

   //glBindBuffer(GL_ARRAY_BUFFER, theVboPosId);
   //glBufferData(GL_ARRAY_BUFFER, theModel.numVertices() * sizeof(float) * 3, theModel.positions(), GL_STATIC_READ);
   //glBindBuffer(GL_ARRAY_BUFFER, theVboNormalId);
   //glBufferData(GL_ARRAY_BUFFER, theModel.numVertices() * sizeof(float) * 3, theModel.normals(), GL_STATIC_READ);
   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theElementbuffer);
   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, theModel.numTriangles() * sizeof(unsigned int) * 3, theModel.indices(), GL_STATIC_READ);
   
   glBindBuffer(GL_ARRAY_BUFFER, theVboPosId);
   glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_READ);
   glBindBuffer(GL_ARRAY_BUFFER, theVboNormalId);
   glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_norms), triangle_norms, GL_STATIC_READ);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theElementbuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_inds), triangle_inds, GL_STATIC_READ);
   
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   // set model view projection
   projection = glm::perspective(45.0f, float(width) / float(height), 1.0f, 150.0f);
   view = glm::lookAt(vec3(0, 0, -3), vec3(0, 0, 0), vec3(0, 1, 0));
   model = translate(mat4(1), vec3(0, 0, 0));

   GLuint shaderId = LoadShader("../shaders/phong.vs", "../shaders/phong.fs");
   glUseProgram(shaderId);
   GLPrintUniformInfo(shaderId);
   int loc;
   loc = glGetUniformLocation(shaderId, "model");
   glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);
   loc = glGetUniformLocation(shaderId, "view");
   glUniformMatrix4fv(loc, 1, GL_FALSE, &view[0][0]);
   loc = glGetUniformLocation(shaderId, "projection");
   glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);
   loc = glGetUniformLocation(shaderId, "color");
   glUniform4f(loc, 1, 0, 0, 1);

   // Loop until the user closes the window 
   while (!glfwWindowShouldClose(window))
   {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

      // Draw primitive
      glBindVertexArray(vaoId);
      //glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

      // Swap front and back buffers
      glfwSwapBuffers(window);

      // Poll for and process events
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}


