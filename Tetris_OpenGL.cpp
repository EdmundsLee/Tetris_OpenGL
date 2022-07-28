///Standard std Libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <windows.h>
#include <string>

///External Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void draw(GLuint*, GLuint*, GLuint*, GLuint, GLfloat, int, GLuint, int);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource =
"#version 330 core                              \n"
"layout (location = 0) in vec3 position;        \n"
"layout (location = 1) in vec4 color;           \n"
"out vec4 ourColor;                             \n"
"void main()                                    \n"
"{                                              \n"
"    gl_Position = vec4(position, 1.0f);        \n"
"    ourColor = color;                          \n"
"}                                              \0";

const GLchar* fragmentShaderSource =
"#version 330 core                              \n"
"in vec4 ourColor;                              \n"
"out vec4 color;                                \n"
"void main()                                    \n"
"{                                              \n"
"    color = vec4(ourColor);                    \n"
"}                                              \0";

GLfloat field_vertices[] = {
    ///  Pos X    Pos Y    Pos Z | Col R   Col G   Col B   Col A
         0.015f,  0.820f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f,  /// Top Right
         0.015f, -0.820f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f,  /// Bottom Right
        -0.735f, -0.820f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f,  /// Bottom Left
        -0.735f,  0.820f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f   /// Top Left
};

GLfloat field_block_vertices[] = {
    ///  Pos X    Pos Y    Pos Z | Col R   Col G   Col B   Col A
        -0.660f, -0.720f,  0.00f,  0.35f,  0.35f,  0.35f,  1.00f,  /// Top Right
        -0.660f, -0.800f,  0.00f,  0.35f,  0.35f,  0.35f,  1.00f,  /// Bottom Right
        -0.720f, -0.800f,  0.00f,  0.35f,  0.35f,  0.35f,  1.00f,  /// Bottom Left
        -0.720f, -0.720f,  0.00f,  0.35f,  0.35f,  0.35f,  1.00f   /// Top Left
};

GLfloat np_vertices[] = {
    ///  Pos X    Pos Y    Pos Z | Col R   Col G   Col B   Col A
         0.095f,  0.820f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f,  /// Top Right
         0.095f,  0.300f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f,  /// Bottom Right
         0.505f,  0.300f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f,  /// Bottom Left
         0.505f,  0.820f,  0.00f,  0.20f,  0.20f,  0.20f,  1.00f   /// Top Left
};

/// Vertexes for dropping pieces (Dynamic)
GLfloat piece_vertices[] = {
    -0.480f,  0.560f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f,  /// Top Right
    -0.480f,  0.480f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f,  /// Bottom Right
    -0.420f,  0.480f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f,  /// Bottom Left
    -0.420f,  0.560f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f   /// Top Left
};

GLfloat np_piece_vertices[] = {
    ///  Pos X    Pos Y    Pos Z | Col R   Col G   Col B   Col A
         0.180f,  0.480f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f,  /// Top Right
         0.180f,  0.400f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f,  /// Bottom Right
         0.240f,  0.400f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f,  /// Bottom Left
         0.240f,  0.480f,  0.00f,  1.00f,  1.00f,  1.00f,  1.00f   /// Top Left
};

///Indexes (Rectangles)
GLuint ver_indices[] = {
    0, 1, 3,  /// First Triangle
    1, 2, 3   /// Second Triangle
};

struct data {
    GLfloat vertex[28];
    unsigned int hor;
    unsigned int ver;
};

void assign_piece(std::vector<data>& database) {
    database.resize(database.size() + 1);
    std::copy(&piece_vertices[0], &piece_vertices[28], database[database.size() - 1].vertex);
    database[database.size() - 1].hor = 17; //18
    database[database.size() - 1].ver = 5;  //10
}

void print_title() {
    char a[] = { 32,186,205,187,188,200,201,219 };

    std::cout << std::endl;
    std::cout << a[0] << a[0] << a[0] <<
        a[0] << a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] << a[0] <<
        a[0] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] << a[0] <<
        a[0] << a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] <<
        a[0] << a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] << a[0] <<
        a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[3] << a[0] << a[0] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[3] << std::endl;

    std::cout << a[0] << a[0] << a[0] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[2] << a[2] << a[4] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[2] << a[2] << a[4] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[2] << a[2] << a[4] <<
        a[5] << a[2] << a[2] << a[7] << a[7] << a[6] << a[2] << a[4] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[1] <<
        a[5] << a[7] << a[7] << a[3] << a[7] << a[7] << a[6] << a[4] <<
        a[7] << a[7] << a[1] << std::endl;

    std::cout << a[0] << a[0] << a[0] <<
        a[7] << a[7] << a[1] << a[0] << a[0] << a[0] << a[0] << a[0] <<
        a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[6] << a[4] <<
        a[7] << a[7] << a[1] << a[0] << a[0] << a[7] << a[7] << a[1] <<
        a[5] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] << a[0] <<
        a[5] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] << a[0] <<
        a[0] << a[0] << a[0] << a[7] << a[7] << a[1] << a[0] << a[0] <<
        a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[6] << a[4] <<
        a[7] << a[7] << a[1] <<
        a[0] << a[5] << a[7] << a[7] << a[7] << a[6] << a[4] << a[0] <<
        a[7] << a[7] << a[1] << std::endl;

    std::cout << a[0] << a[0] << a[0] <<
        a[7] << a[7] << a[1] << a[0] << a[0] << a[0] << a[0] << a[0] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[1] << a[0] << a[0] << a[7] << a[7] << a[1] <<
        a[0] << a[5] << a[2] << a[2] << a[2] << a[7] << a[7] << a[1] <<
        a[0] << a[5] << a[2] << a[2] << a[2] << a[7] << a[7] << a[1] <<
        a[0] << a[0] << a[0] << a[7] << a[7] << a[1] << a[0] << a[0] <<
        a[7] << a[7] << a[6] << a[2] << a[2] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[1] <<
        a[0] << a[7] << a[7] << a[6] << a[7] << a[7] << a[3] << a[0] <<
        a[5] << a[2] << a[4] << std::endl;

    std::cout << a[0] << a[0] << a[0] <<
        a[5] << a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[1] << a[0] << a[0] << a[7] << a[7] << a[1] <<
        a[5] << a[7] << a[7] << a[7] << a[7] << a[7] << a[6] << a[4] <<
        a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[6] << a[4] <<
        a[7] << a[7] << a[7] << a[7] << a[7] << a[7] << a[6] << a[4] <<
        a[0] << a[0] << a[0] << a[7] << a[7] << a[1] << a[0] << a[0] <<
        a[7] << a[7] << a[1] << a[0] << a[0] << a[7] << a[7] << a[1] <<
        a[7] << a[7] << a[1] <<
        a[7] << a[7] << a[6] << a[4] << a[0] << a[7] << a[7] << a[3] <<
        a[7] << a[7] << a[3] << std::endl;

    std::cout << a[0] << a[0] << a[0] <<
        a[0] << a[5] << a[2] << a[2] << a[2] << a[2] << a[2] << a[4] <<
        a[5] << a[2] << a[4] << a[0] << a[0] << a[5] << a[2] << a[4] <<
        a[0] << a[5] << a[2] << a[2] << a[2] << a[2] << a[4] << a[0] <<
        a[5] << a[2] << a[2] << a[2] << a[2] << a[2] << a[4] << a[0] <<
        a[5] << a[2] << a[2] << a[2] << a[2] << a[2] << a[4] << a[0] <<
        a[0] << a[0] << a[0] << a[5] << a[2] << a[4] << a[0] << a[0] <<
        a[5] << a[2] << a[4] << a[0] << a[0] << a[5] << a[2] << a[4] <<
        a[5] << a[2] << a[4] <<
        a[5] << a[2] << a[4] << a[0] << a[0] << a[5] << a[2] << a[4] <<
        a[5] << a[2] << a[4] << std::endl;

    std::cout << std::endl;
    std::cout << "Made by - Lee Seng Wei: For a diploma capstone project" << std::endl;
    std::cout << std::endl;
    std::cout << "Press \"Left\" or \"Right\" to move" << std::endl;
    std::cout << "Press \"Up\" to rotate clockwise" << std::endl;
    std::cout << "Press \"Down\" to perform soft drop" << std::endl;
    std::cout << "Press \"Space\" to perform hard drop" << std::endl;
    std::cout << "Press \"Enter\" to start, pause or resume" << std::endl;
    std::cout << "Press \"Esc\" to quit" << std::endl;
    std::cout << std::endl;
}

void show_highscores(int(&s_rank)[5], int(&c_rank)[5], int(&l_rank)[5]) {
    std::ifstream read_lines("save.txt");
    int t = 0;
    if (read_lines.is_open()) {
        std::string line;
        while (std::getline(read_lines, line)) t++;
        read_lines.close();
    }

    std::ifstream read_scores("save.txt");
    int s, c, l;
    if (read_scores.is_open()) {
        std::string line;
        for (int i = 0; i < t; i++) {
            read_scores >> s >> c >> l;
            int temp;
            for (int j = 0; j < 5; j++) {
                if (s >= s_rank[j]) {
                    temp = s;
                    s = s_rank[j];
                    s_rank[j] = temp;

                    temp = c;
                    c = c_rank[j];
                    c_rank[j] = temp;

                    temp = l;
                    l = l_rank[j];
                    l_rank[j] = temp;
                }
            }
        }
        read_scores.close();
    }

    std::cout << "+----------------+" << std::endl;
    std::cout << "|   Highscores   |" << std::endl;
    std::cout << "+----------------+" << std::endl;
    std::cout << "#1: " << std::endl;
    std::cout << "Score: " << s_rank[0] << ", Lines: " << c_rank[0] << ", Level: " << l_rank[0] << std::endl;
    std::cout << std::endl;
    std::cout << "#2: " << std::endl;
    std::cout << "Score: " << s_rank[1] << ", Lines: " << c_rank[1] << ", Level: " << l_rank[1] << std::endl;
    std::cout << std::endl;
    std::cout << "#3: " << std::endl;
    std::cout << "Score: " << s_rank[2] << ", Lines: " << c_rank[2] << ", Level: " << l_rank[2] << std::endl;
    std::cout << std::endl;
    std::cout << "#4: " << std::endl;
    std::cout << "Score: " << s_rank[3] << ", Lines: " << c_rank[3] << ", Level: " << l_rank[3] << std::endl;
    std::cout << std::endl;
    std::cout << "#5: " << std::endl;
    std::cout << "Score: " << s_rank[4] << ", Lines: " << c_rank[4] << ", Level: " << l_rank[4] << std::endl;
    std::cout << std::endl;
}

void draw(GLuint* VBO, GLuint* VAO, GLuint* CBO, GLuint VEBO, GLfloat* ver, int sver, GLuint* ind, int sind) {
    ///Bind Array
    glBindVertexArray(*VAO);

    ///Location Buffer
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sver * sizeof(GLfloat), ver, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sind * sizeof(GLuint), ind, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///Color Buffer
    glBindBuffer(GL_ARRAY_BUFFER, *CBO);
    glBufferData(GL_ARRAY_BUFFER, sver * sizeof(GLfloat), ver, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sind * sizeof(GLuint), ind, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///Unbind Array
    glBindVertexArray(0);
}

void assign_colour(GLfloat* vertex, int index) {
    switch (index) {
    case 1:
        for (int i = 0; i < 4; i++) {
            vertex[3 + (7 * i)] = 0.00f;
            vertex[4 + (7 * i)] = 0.75f;
            vertex[5 + (7 * i)] = 0.75f;
        }
        break;

    case 2:
        for (int i = 0; i < 4; i++) {
            vertex[3 + (7 * i)] = 0.75f;
            vertex[4 + (7 * i)] = 0.75f;
            vertex[5 + (7 * i)] = 0.00f;
        }
        break;

    case 3:
        for (int i = 0; i < 4; i++) {
            vertex[3 + (7 * i)] = 0.75f;
            vertex[4 + (7 * i)] = 0.25f;
            vertex[5 + (7 * i)] = 0.00f;
        }
        break;

    case 4:
        for (int i = 0; i < 4; i++) {
            vertex[3 + (7 * i)] = 0.00f;
            vertex[4 + (7 * i)] = 0.75f;
            vertex[5 + (7 * i)] = 0.00f;
        }
        break;

    case 5:
        for (int i = 0; i < 4; i++) {
            vertex[3 + (7 * i)] = 0.00f;
            vertex[4 + (7 * i)] = 0.00f;
            vertex[5 + (7 * i)] = 0.75f;
        }
        break;

    case 6:
        for (int i = 0; i < 4; i++) {
            vertex[3 + (7 * i)] = 0.75f;
            vertex[4 + (7 * i)] = 0.00f;
            vertex[5 + (7 * i)] = 0.00f;
        }
        break;

    case 7:
        for (int i = 0; i < 4; i++) {
            vertex[3 + (7 * i)] = 0.75f;
            vertex[4 + (7 * i)] = 0.00f;
            vertex[5 + (7 * i)] = 0.75f;
        }
        break;
    }
}

int gen_pieces(bool(&grid)[4][4]) {
    int random = rand() % 7 + 1;
    int temp = random;
    int sim_count = 0;
    if (temp == random) {
        sim_count++;
        if (sim_count >= 2) {
            while (temp == random) random = rand() % 7 + 1;
        }
    }
    else sim_count = 0;

    switch (random) {
    case 1: {
        grid[0][2] = 1; ///{0,0,1,0},
        grid[1][2] = 1; ///{0,0,1,0},
        grid[2][2] = 1; ///{0,0,1,0},
        grid[3][2] = 1; ///{0,0,1,0}.
        break;
    }
    case 2: {
        grid[0][1] = 1; ///{0,0,0,0},
        grid[0][2] = 1; ///{0,0,0,0},
        grid[1][2] = 1; ///{0,1,1,0},
        grid[1][1] = 1; ///{0,1,1,0}.
        break;
    }
    case 3: {
        grid[0][1] = 1; ///{0,0,0,0},
        grid[0][2] = 1; ///{0,1,1,0},
        grid[1][2] = 1; ///{0,0,1,0},
        grid[2][2] = 1; ///{0,0,1,0}.
        break;
    }
    case 4: {
        grid[0][1] = 1; ///{0,0,0,0},
        grid[1][1] = 1; ///{0,1,0,0},
        grid[1][2] = 1; ///{0,1,1,0},
        grid[2][2] = 1; ///{0,0,1,0}.
        break;
    }
    case 5: {
        grid[0][1] = 1; ///{0,0,0,0},
        grid[0][2] = 1; ///{0,1,1,0},
        grid[1][1] = 1; ///{0,1,0,0},
        grid[2][1] = 1; ///{0,1,0,0}.
        break;
    }
    case 6: {
        grid[0][2] = 1; ///{0,0,0,0},
        grid[1][1] = 1; ///{0,0,1,0},
        grid[1][2] = 1; ///{0,1,1,0},
        grid[2][1] = 1; ///{0,1,0,0}.
        break;
    }
    case 7: {
        grid[0][2] = 1; ///{0,0,0,0},
        grid[1][1] = 1; ///{0,0,1,0},
        grid[1][2] = 1; ///{0,1,1,0},
        grid[2][2] = 1; ///{0,0,1,0}.
        break;
    }
    }
    return random;
}

///Scores
unsigned int level = 1;
unsigned int score = 0;
unsigned int lines = 0;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Crosstrix - by: Lee Seng Wei", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VBO, VAO, VEBO, CBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &CBO);
    glGenBuffers(1, &VEBO);

    ///Static Indices Array
    int sind = sizeof(ver_indices) / sizeof(GLuint);
    GLuint* ind = new GLuint[sind]();
    for (int i = 0; i < sind; i++) ind[i] = ver_indices[i];
    ///End declaration

    const int sver = sizeof(field_vertices) / sizeof(GLfloat);
    GLfloat* ver = new GLfloat[sver]();
    for (int i = 0; i < sver; i++) ver[i] = field_vertices[i];
    draw(&VBO, &VAO, &CBO, VEBO, ver, sver, ind, sind);

    ///Draw Block Grids on field
    const GLfloat bver[240][sver];
    for (int h = 0; h < 240; h++) {
        for (int i = 0; i < sver; i++) bver[h][i] = field_block_vertices[i];
    }
    GLuint bVBO[240], bVAO[240], bCBO[240];
    int b = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 12; j++) {
            for (int k = 0; k < j; k++) {
                for (int l = 0; l < 4; l++) bver[b][0 + (7 * l)] += 0.060f;
            }

            for (int k = 0; k < i; k++) {
                for (int l = 0; l < 4; l++) bver[b][1 + (7 * l)] += 0.080f;
            }
            if ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) {
                for (int l = 0; l < 4; l++) {
                    bver[b][3 + (7 * l)] = 0.400f;
                    bver[b][4 + (7 * l)] = 0.400f;
                    bver[b][5 + (7 * l)] = 0.400f;
                }
            }

            glGenVertexArrays(1, &bVAO[b]);
            glGenBuffers(1, &bVBO[b]);
            glGenBuffers(1, &bCBO[b]);
            draw(&bVBO[b], &bVAO[b], &bCBO[b], VEBO, bver[b], sver, ind, sind);
            b++;
        }
    }


    GLuint nVBO, nVAO, nCBO;
    glGenVertexArrays(1, &nVAO);
    glGenBuffers(1, &nVBO);
    glGenBuffers(1, &nCBO);

    GLfloat* nver = new GLfloat[sver]();
    for (int i = 0; i < sver; i++) nver[i] = np_vertices[i];
    draw(&nVBO, &nVAO, &nCBO, VEBO, nver, sver, ind, sind);

    srand(time(NULL));

    std::vector<data> database;
    database.reserve(324);
    bool pgrid[4][4] = { 0 };
    int rep = 0;
    int  n_random = gen_pieces(pgrid);
    ///Dynamic Block
    std::vector<GLuint> pVAO;
    std::vector<GLuint> pVBO;
    std::vector<GLuint> pCBO;

    for (int a = 0; a < 4; a++) {
        for (int b = 0; b < 4; b++) {
            if (pgrid[a][b] == 1) {
                int ta = 3 - a;
                assign_piece(database);
                database[rep].ver += b;
                database[rep].hor += ta;
                for (int k = 0; k < ta; k++) {
                    for (int m = 0; m < 4; m++) {
                        database[rep].vertex[1 + (7 * m)] += 0.080f;
                    }
                }

                for (int l = 0; l < b; l++) {
                    for (int n = 0; n < 4; n++) {
                        database[rep].vertex[0 + (7 * n)] += 0.060f;
                    }
                }

                assign_colour(database[rep].vertex, n_random);

                pVAO.resize(pVAO.size() + 1);
                pVBO.resize(pVBO.size() + 1);
                pCBO.resize(pCBO.size() + 1);
                glGenVertexArrays(1, &pVAO[rep]);
                glGenBuffers(1, &pVBO[rep]);
                glGenBuffers(1, &pCBO[rep]);
                draw(&pVBO[rep], &pVAO[rep], &pCBO[rep], VEBO, database[rep].vertex, sver, ind, sind);

                rep++;
            }
        }
    }

    GLfloat mver[4][sver];
    for (int h = 0; h < 4; h++) {
        for (int i = 0; i < sver; i++) mver[h][i] = np_piece_vertices[i];
    }
    GLuint mVBO[4], mVAO[4], mCBO[4];
    bool mgrid[4][4] = { 0 };
    int  m_random = gen_pieces(mgrid);
    unsigned int m_count = 0;
    for (int a = 0; a < 4; a++) {
        for (int b = 0; b < 4; b++) {
            if (mgrid[a][b] == 1) {
                int ta = 3 - a;
                for (int k = 0; k < ta; k++) {
                    for (int m = 0; m < 4; m++) {
                        mver[m_count][1 + (7 * m)] += 0.080f;
                    }
                }

                for (int l = 0; l < b; l++) {
                    for (int n = 0; n < 4; n++) {
                        mver[m_count][0 + (7 * n)] += 0.060f;
                    }
                }
                assign_colour(mver[m_count], m_random);

                glGenVertexArrays(1, &mVAO[m_count]);
                glGenBuffers(1, &mVBO[m_count]);
                glGenBuffers(1, &mCBO[m_count]);
                draw(&mVBO[m_count], &mVAO[m_count], &mCBO[m_count], VEBO, mver[m_count], sver, ind, sind);
                m_count++;
            }
        }
    }
    m_count = 0;


    /// Uncommenting this call will result in wireframe polygons.
    ///glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    bool pressed_a = false;
    bool pressed_d = false;
    bool pressed_w = false;
    bool pressed_s = false;
    bool pressed_space = false;
    bool pressed_enter = false;
    bool pause = true;
    bool space_lock = false;
    bool landed = false;
    bool topped_out = true;
    bool dsa = false;
    int line_for_level = 0;
    int bck_random = rand() % 7 + 1;

    unsigned int topmost[4] = { 0 };
    unsigned int bottommost[4] = { 20 };
    float time = 2.000f;
    float time_for_level = 0.750f;
    float prev = glfwGetTime();
    bool grid[21][13] = { false };
    int s_rank[5] = { 0 };
    int c_rank[5] = { 0 };
    int l_rank[5] = { 0 };
    GLfloat r_b = 0.15f;
    GLfloat g_b = 0.15f;
    GLfloat b_b = 0.15f;

    /// Game loop
    while (!glfwWindowShouldClose(window))
    {
        if (pause == false) {
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && pressed_a == false && space_lock == false) {
                unsigned int leftmost = 11; ///Initialize the leftmost position of the piece to be the right-most position of the board

                ///Check all 4 squares of the piece to find the left-most position
                for (int x = 1; x <= 4; x++) {
                    if (database[rep - x].ver < leftmost) leftmost = database[rep - x].ver;
                }
                ///Shift the piece left one position. The left-most position has a minimum of 1
                if (leftmost > 1) {
                    for (int q = 1; q <= 4; q++) {
                        for (int i = 0; i < 4; i++)
                            database[rep - q].vertex[0 + (7 * i)] -= 0.060f;
                        database[rep - q].ver--;
                        int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                        draw(&pVBO[rep - q], &pVAO[rep - q], &pCBO[rep - q], VEBO, database[rep - q].vertex, sver, ind, sind);
                    }
                }


                for (int w = 1; w <= 4; w++) {
                    for (int i = 0; i < (rep - w); i++) {
                        if (database[i].hor == database[rep - w].hor) {
                            if ((database[i].ver) == database[rep - w].ver) {
                                for (int q = 1; q <= 4; q++) {
                                    for (int i = 0; i < 4; i++)
                                        database[rep - q].vertex[0 + (7 * i)] += 0.060f;
                                    database[rep - q].ver++;
                                    int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                                    draw(&pVBO[rep - q], &pVAO[rep - q], &pCBO[rep - q], VEBO, database[rep - q].vertex, sver, ind, sind);
                                }
                            }
                        }
                    }
                }
                ///std::cout << "Key Pressed = LEFT,  Horizontal location = " << database[rep - 1].ver << std::endl;
                pressed_a = true;
            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && pressed_d == false && space_lock == false) {
                unsigned int rightmost = 0;
                for (int x = 1; x <= 4; x++) {
                    if (database[rep - x].ver > rightmost) rightmost = database[rep - x].ver;
                }
                if (rightmost < 12) {
                    for (int q = 1; q <= 4; q++) {
                        for (int i = 0; i < 4; i++)
                            database[rep - q].vertex[0 + (7 * i)] += 0.060f;
                        database[rep - q].ver++;
                        int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                        draw(&pVBO[rep - q], &pVAO[rep - q], &pCBO[rep - q], VEBO, database[rep - q].vertex, sver, ind, sind);
                    }
                }


                for (int w = 1; w <= 4; w++) {
                    for (int i = 0; i < (rep - w); i++) {
                        if (database[i].hor == database[rep - w].hor) {
                            if ((database[i].ver) == database[rep - w].ver) {
                                for (int q = 1; q <= 4; q++) {
                                    for (int i = 0; i < 4; i++)
                                        database[rep - q].vertex[0 + (7 * i)] -= 0.060f;
                                    database[rep - q].ver--;
                                    int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                                    draw(&pVBO[rep - q], &pVAO[rep - q], &pCBO[rep - q], VEBO, database[rep - q].vertex, sver, ind, sind);
                                }
                            }
                        }
                    }
                }
                ///std::cout << "Key Pressed = RIGHT, Horizontal location = " << database[rep - 1].ver << std::endl;
                pressed_d = true;
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && pressed_w == false && space_lock == false) {
                if (n_random == 1 || n_random == 3 || n_random == 4 || n_random == 5 || n_random == 6 || n_random == 7) {
                    int piv_x = database[rep - 2].ver;
                    int piv_y = database[rep - 2].hor;

                    for (int a = 1; a <= 4; a++) {
                        unsigned int x_prev = database[rep - a].ver;
                        unsigned int y_prev = database[rep - a].hor;
                        int x = database[rep - a].ver - piv_x;
                        int y = database[rep - a].hor - piv_y;
                        database[rep - a].ver = piv_x + y;
                        database[rep - a].hor = piv_y - x;

                        int dx = x_prev - database[rep - a].ver;
                        int dy = y_prev - database[rep - a].hor;
                        for (int i = 0; i < 4; i++) {
                            database[rep - a].vertex[0 + (7 * i)] -= (0.060f * dx);
                            database[rep - a].vertex[1 + (7 * i)] -= (0.080f * dy);
                        }
                        int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                        draw(&pVBO[rep - a], &pVAO[rep - a], &pCBO[rep - a], VEBO, database[rep - a].vertex, sver, ind, sind);
                    }

                    bool conflict = false;
                    for (int i = 1; i <= 20; i++)
                        for (int j = 1; j <= 12; j++)
                            if (grid[i][j] == true)
                                for (int c = 1; c <= 4; c++)
                                    if (database[rep - c].hor == i && database[rep - c].ver == j)
                                        conflict = true;

                    for (int b = 1; b <= 4; b++) {
                        if (database[rep - b].ver < 1 || database[rep - b].ver > 12 || conflict == true) {
                            for (int a = 1; a <= 4; a++) {
                                unsigned int x_prev = database[rep - a].ver;
                                unsigned int y_prev = database[rep - a].hor;
                                int x = database[rep - a].ver - piv_x;
                                int y = database[rep - a].hor - piv_y;
                                database[rep - a].ver = piv_x - y;
                                database[rep - a].hor = piv_y + x;

                                int dx = x_prev - database[rep - a].ver;
                                int dy = y_prev - database[rep - a].hor;
                                for (int i = 0; i < 4; i++) {
                                    database[rep - a].vertex[0 + (7 * i)] -= (0.060f * dx);
                                    database[rep - a].vertex[1 + (7 * i)] -= (0.080f * dy);
                                }
                                int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                                draw(&pVBO[rep - a], &pVAO[rep - a], &pCBO[rep - a], VEBO, database[rep - a].vertex, sver, ind, sind);
                            }
                            break;
                        }
                    }
                    conflict = false;
                }
                pressed_w = true;
            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && pressed_s == false && space_lock == false) {
                if (time == time_for_level) {
                    time = 0.150f;
                    score += 1;
                }
            }


            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && pressed_space == false && space_lock == false && landed == false) {

                for (int e = 0; e < 4; e++)
                    bottommost[e] = 20;
                for (int x = 1; x <= 4; x++) {
                    if (database[rep - x].hor < bottommost[x - 1])
                        bottommost[x - 1] = database[rep - x].hor;
                }

                for (int e = 0; e < 4; e++)
                    topmost[e] = 0;
                ///Check the dropped blocks on the board (static) and for each square of the piece, detect the highest position of the dropped block
                ///     below.
                for (int x = 1; x <= 4; x++) {
                    for (int y = 0; y < (rep - 4); y++) {
                        if (database[y].ver == database[rep - x].ver && database[y].hor > topmost[x - 1] && database[y].hor < database[rep - x].hor)
                            topmost[x - 1] = database[y].hor;
                    }
                }

                for (int x = 0; x < 4; x++) {
                    ///std::cout << topmost[x] << ", " << bottommost[x] << std::endl;
                }

                landed = false;
                while (landed == false) {
                    for (int q = 1; q <= 4; q++) {
                        for (int i = 0; i < 4; i++)
                            database[rep - q].vertex[1 + (7 * i)] -= 0.080f;
                        database[rep - q].hor--;
                        int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                        draw(&pVBO[rep - q], &pVAO[rep - q], &pCBO[rep - q], VEBO, database[rep - q].vertex, sver, ind, sind);
                    }
                    for (int x = 0; x < 4; x++) {
                        bottommost[x]--;
                    }
                    for (int x = 0; x < 4; x++) {
                        if (topmost[x] + 1 == bottommost[x])
                            landed = true;
                    }
                    score += 2;
                }

                ///std::cout << "Key Pressed = SPACE" << std::endl;
                pressed_space = true;
                space_lock = true;
            }

            /// Refresh Keys
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && pressed_a == true)      pressed_a = false;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE && pressed_d == true)     pressed_d = false;
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && pressed_w == true)        pressed_w = false;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && pressed_s == true)      pressed_s = false;
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && pressed_space == true) pressed_space = false;

            ///Delta Define Start
            float now = glfwGetTime();
            float distance = now - prev;

            prev = now;
            ///Delta Check
            time -= distance;

            for (int e = 0; e < 4; e++)
                bottommost[e] = 18;
            for (int x = 1; x <= 4; x++) {
                if (database[rep - x].hor < bottommost[x - 1])
                    bottommost[x - 1] = database[rep - x].hor;
            }

            for (int e = 0; e < 4; e++)
                topmost[e] = 0;
            ///Check the dropped blocks on the board (static) and for each square of the piece, detect the highest position of the dropped block
            ///     below.
            for (int x = 1; x <= 4; x++) {
                for (int y = 0; y < (rep - 4); y++) {
                    if (database[y].ver == database[rep - x].ver && database[y].hor > topmost[x - 1] && database[y].hor < database[rep - x].hor)
                        topmost[x - 1] = database[y].hor;
                }
            }

            landed = false;
            for (int x = 0; x < 4; x++) {
                if (topmost[x] + 1 == bottommost[x])
                    landed = true;
            }

            if (time <= 0.000f && landed == false) {
                ///std::cout << "Time Passed = " << 0.2 << "s,  Vertical location   = " << bottommost[1] << std::endl;
                for (int x = 0; x < 4; x++) {
                    ///std::cout << topmost[x] << ", ";
                }
                ///std::cout << std::endl;

                for (int q = 1; q <= 4; q++) {
                    for (int i = 0; i < 4; i++)
                        database[rep - q].vertex[1 + (7 * i)] -= 0.080f;
                    database[rep - q].hor--;
                    int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                    draw(&pVBO[rep - q], &pVAO[rep - q], &pCBO[rep - q], VEBO, database[rep - q].vertex, sver, ind, sind);
                }
                time = time_for_level; ///Reset Delta
            }

            if (time <= 0.000f && landed == true) {
                ///std::cout << "Block " << rep << " Successfully stopped " << std::endl;
                for (int q = 1; q <= 4; q++) {
                    grid[database[rep - q].hor][database[rep - q].ver] = true;
                }

                ///Check for completed lines (Maximum lines = 4)
                bool line_check;
                int line_count = 0;
                unsigned int comp_lines[4] = { 0 };
                for (unsigned int i = 1; i <= 20; i++) {
                    line_check = true;
                    for (unsigned int j = 1; j <= 12; j++) {
                        if (grid[i][j] == false) line_check = false;
                    }
                    if (line_check == true) {
                        comp_lines[line_count] = i;
                        line_count++;
                        lines++;
                    }
                }

                if (line_count == 1) score += (40 * level);
                if (line_count == 2) score += (100 * level);
                if (line_count == 3) score += (300 * level);
                if (line_count == 4) score += (1200 * level);

                line_for_level += line_count;

                int temp = level;
                if (level < 10 && line_for_level >= 20) {
                    level++;
                    time_for_level -= 0.025f;
                    line_for_level -= 20;

                }
                if (level >= 10 && level < 20 && line_for_level >= 15) {
                    level++;
                    time_for_level -= 0.025f;
                    line_for_level -= 15;
                }
                if (level >= 20 && level < 30 && line_for_level >= 10) {
                    level++;
                    time_for_level -= 0.025f;
                    line_for_level -= 10;
                }

                if (temp != level) {
                    int bck_temp = bck_random;
                    bck_random = rand() % 7 + 1;
                    if (bck_temp == bck_random) {
                        while (bck_temp == bck_random) bck_random = rand() % 7 + 1;
                    }

                    switch (bck_random) {
                    case 1:
                        r_b = 0.15f;
                        g_b = 0.15f;
                        b_b = 0.15f;
                        break;
                    case 2:
                        r_b = 0.30f;
                        g_b = 0.15f;
                        b_b = 0.15f;
                        break;
                    case 3:
                        r_b = 0.15f;
                        g_b = 0.30f;
                        b_b = 0.15f;
                        break;
                    case 4:
                        r_b = 0.15f;
                        g_b = 0.15f;
                        b_b = 0.30f;
                        break;
                    case 5:
                        r_b = 0.30f;
                        g_b = 0.30f;
                        b_b = 0.15f;
                        break;
                    case 6:
                        r_b = 0.30f;
                        g_b = 0.15f;
                        b_b = 0.30f;
                        break;
                    case 7:
                        r_b = 0.15f;
                        g_b = 0.30f;
                        b_b = 0.30f;
                        break;
                    }
                }

                ///Print titles
                system("cls");
                print_title();
                std::cout << "+--------------+" << std::endl;
                std::cout << "|  Your Score  |" << std::endl;
                std::cout << "+--------------+" << std::endl;
                std::cout << "Current Score: " << score << std::endl;
                std::cout << "Lines Cleared: " << lines << std::endl;
                std::cout << "Current Level: " << level << std::endl;
                std::cout << std::endl;


                ///Delete the lines
                for (int u = 0; u < rep; u++) {
                    for (int v = 0; v < 4; v++) {
                        if (database[u].hor == comp_lines[v]) {
                            ///Delete vertex arrays and buffers
                            glDeleteVertexArrays(1, &pVAO[u]);
                            glDeleteBuffers(1, &pVBO[u]);
                            glDeleteBuffers(1, &pCBO[u]);

                            database.erase(database.begin() + u);
                            pVAO.erase(pVAO.begin() + u);
                            pVBO.erase(pVBO.begin() + u);
                            pCBO.erase(pCBO.begin() + u);
                            rep--;
                            u--;
                        }
                    }
                }

                ///Delete completed line in vertex memory, reset line counter
                for (int u = 0; u < 4; u++) {
                    for (int v = 1; v <= 12; v++) {
                        grid[comp_lines[u]][v] = false;
                    }
                }


                ///Shift remaining blocks downwards
                int u = 0;
                while (comp_lines[u] > 0 && comp_lines[u] < 20 && u < 4) {
                    for (int z = comp_lines[u] + 1; z <= 20; z++) {
                        for (int x = 1; x <= 12; x++) {
                            if (grid[z][x] == true) {
                                grid[z][x] = false;
                                grid[z - 1][x] = true;
                            }
                        }
                    }
                    for (int v = 0; v <= rep; v++) {
                        if (database[v].hor > comp_lines[u]) {
                            database[v].hor--;
                            for (int p = 0; p < 4; p++) {
                                database[v].vertex[1 + (7 * p)] -= 0.080f;
                            }
                        }
                        int sver = sizeof(piece_vertices) / sizeof(GLfloat);
                        draw(&pVBO[v], &pVAO[v], &pCBO[v], VEBO, database[v].vertex, sver, ind, sind);
                    }
                    u++;
                    for (int b = u; b < 4; b++) comp_lines[b]--;
                }


                ///Draw New Piece
                time = time_for_level;
                for (int a = 0; a < 4; a++) {
                    for (int b = 0; b < 4; b++) {
                        pgrid[a][b] = 0;
                    }
                }

                ///Transfer coordinates from next piece to the play field

                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) pgrid[i][j] = mgrid[i][j];
                }
                n_random = m_random;
                for (int a = 0; a < 4; a++) {
                    for (int b = 0; b < 4; b++) {
                        if (pgrid[a][b] == 1) {
                            int ta = 3 - a;
                            assign_piece(database);
                            database[rep].ver += b;
                            database[rep].hor += ta;
                            for (int k = 0; k < ta; k++) {
                                for (int m = 0; m < 4; m++) {
                                    database[rep].vertex[1 + (7 * m)] += 0.080f;
                                }
                            }

                            for (int l = 0; l < b; l++) {
                                for (int n = 0; n < 4; n++) {
                                    database[rep].vertex[0 + (7 * n)] += 0.060f;
                                }
                            }
                            assign_colour(database[rep].vertex, n_random);
                            ///std::cout << rep << " | " << database[rep].ver << ", " << database[rep].hor << " owo " << std::endl;
                            pVAO.resize(pVAO.size() + 1);
                            pVBO.resize(pVBO.size() + 1);
                            pCBO.resize(pCBO.size() + 1);
                            glGenVertexArrays(1, &pVAO[rep]);
                            glGenBuffers(1, &pVBO[rep]);
                            glGenBuffers(1, &pCBO[rep]);
                            draw(&pVBO[rep], &pVAO[rep], &pCBO[rep], VEBO, database[rep].vertex, sver, ind, sind);

                            rep++;
                        }
                    }
                }

                ///Draw next piece
                for (int a = 0; a < 4; a++) {
                    for (int b = 0; b < 4; b++) {
                        mgrid[a][b] = 0;
                    }
                }

                m_random = gen_pieces(mgrid);
                for (int u = 0; u < 4; u++) {
                    mver[u][0] = 0.180f;
                    mver[u][1] = 0.480f;
                    mver[u][7] = 0.180f;
                    mver[u][8] = 0.400f;
                    mver[u][14] = 0.240f;
                    mver[u][15] = 0.400f;
                    mver[u][21] = 0.240f;
                    mver[u][22] = 0.480f;
                }

                for (int a = 0; a < 4; a++) {
                    for (int b = 0; b < 4; b++) {
                        if (mgrid[a][b] == 1) {
                            int ta = 3 - a;
                            for (int k = 0; k < ta; k++) {
                                for (int m = 0; m < 4; m++) {
                                    mver[m_count][1 + (7 * m)] += 0.080f;
                                }
                            }

                            for (int l = 0; l < b; l++) {
                                for (int n = 0; n < 4; n++) {
                                    mver[m_count][0 + (7 * n)] += 0.060f;
                                }
                            }

                            assign_colour(mver[m_count], m_random);

                            draw(&mVBO[m_count], &mVAO[m_count], &mCBO[m_count], VEBO, mver[m_count], sver, ind, sind);
                            m_count++;
                        }
                    }
                }
                m_count = 0;


                landed = false;
                space_lock = false;
            }

            for (int i = 0; i < (rep - 4); i++) {
                if (database[i].hor > 19 && topped_out == false) {
                    pause = true;
                    std::ofstream record_score;

                    record_score.open("save.txt", std::ios_base::app);
                    record_score << score << " ";
                    record_score << lines << " ";
                    record_score << level << " ";
                    record_score << "\n";
                    record_score.close();

                    system("cls");
                    print_title();
                    std::cout << "+---------------+" << std::endl;
                    std::cout << "|  Final Score  |" << std::endl;
                    std::cout << "+---------------+" << std::endl;
                    std::cout << "Score: " << score << ", Lines : " << lines << ", Level: " << level << std::endl;
                    std::cout << std::endl;
                    show_highscores(s_rank, c_rank, l_rank);

                    for (int j = 0; j < rep; j++) {
                        for (int k = 0; k < 4; k++) {
                            database[j].vertex[3 + (7 * k)] = 0.750f;
                            database[j].vertex[4 + (7 * k)] = 0.750f;
                            database[j].vertex[5 + (7 * k)] = 0.750f;
                        }
                        draw(&pVBO[j], &pVAO[j], &pCBO[j], VEBO, database[j].vertex, sver, ind, sind);
                    }

                    topped_out = true;
                }
            }

        }

        if (dsa == false) {
            system("cls");
            print_title();
            show_highscores(s_rank, c_rank, l_rank);
            dsa = true;
        }

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && pressed_enter == false) {
            if (pause == false) {
                pause = true;
            }
            else {
                ///If topped out, reset game;
                if (topped_out == true) {

                    ///Clear all block renders in the play field
                    for (int i = 0; i < rep; i++) {
                        glDeleteVertexArrays(1, &pVAO[i]);
                        glDeleteBuffers(1, &pVBO[i]);
                        glDeleteBuffers(1, &pCBO[i]);
                    }
                    database.erase(database.begin(), database.end());
                    pVAO.erase(pVAO.begin(), pVAO.end());
                    pVAO.shrink_to_fit();
                    pVBO.erase(pVBO.begin(), pVBO.end());
                    pVBO.shrink_to_fit();
                    pCBO.erase(pCBO.begin(), pCBO.end());
                    pCBO.shrink_to_fit();
                    rep = 0;

                    ///Delete all registers in the grid vertex
                    for (int u = 1; u <= 20; u++) {
                        for (int v = 1; v <= 12; v++) {
                            grid[u][v] = false;
                        }
                    }

                    ///Stream piece in next field into play field
                    for (int a = 0; a < 4; a++) {
                        for (int b = 0; b < 4; b++) {
                            pgrid[a][b] = 0;
                        }
                    }

                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) pgrid[i][j] = mgrid[i][j];
                    }
                    n_random = m_random;
                    for (int a = 0; a < 4; a++) {
                        for (int b = 0; b < 4; b++) {
                            if (pgrid[a][b] == 1) {
                                int ta = 3 - a;
                                assign_piece(database);
                                database[rep].ver += b;
                                database[rep].hor += ta;
                                for (int k = 0; k < ta; k++) {
                                    for (int m = 0; m < 4; m++) {
                                        database[rep].vertex[1 + (7 * m)] += 0.080f;
                                    }
                                }

                                for (int l = 0; l < b; l++) {
                                    for (int n = 0; n < 4; n++) {
                                        database[rep].vertex[0 + (7 * n)] += 0.060f;
                                    }
                                }
                                assign_colour(database[rep].vertex, n_random);
                                ///std::cout << rep << " | " << database[rep].ver << ", " << database[rep].hor << " owo " << std::endl;
                                pVAO.resize(pVAO.size() + 1);
                                pVBO.resize(pVBO.size() + 1);
                                pCBO.resize(pCBO.size() + 1);
                                glGenVertexArrays(1, &pVAO[rep]);
                                glGenBuffers(1, &pVBO[rep]);
                                glGenBuffers(1, &pCBO[rep]);
                                draw(&pVBO[rep], &pVAO[rep], &pCBO[rep], VEBO, database[rep].vertex, sver, ind, sind);

                                rep++;
                            }
                        }
                    }

                    ///Generate piece in next field
                    for (int a = 0; a < 4; a++) {
                        for (int b = 0; b < 4; b++) {
                            mgrid[a][b] = 0;
                        }
                    }

                    m_random = gen_pieces(mgrid);
                    for (int u = 0; u < 4; u++) {
                        mver[u][0] = 0.180f;
                        mver[u][1] = 0.480f;
                        mver[u][7] = 0.180f;
                        mver[u][8] = 0.400f;
                        mver[u][14] = 0.240f;
                        mver[u][15] = 0.400f;
                        mver[u][21] = 0.240f;
                        mver[u][22] = 0.480f;
                    }

                    for (int a = 0; a < 4; a++) {
                        for (int b = 0; b < 4; b++) {
                            if (mgrid[a][b] == 1) {
                                int ta = 3 - a;
                                for (int k = 0; k < ta; k++) {
                                    for (int m = 0; m < 4; m++) {
                                        mver[m_count][1 + (7 * m)] += 0.080f;
                                    }
                                }

                                for (int l = 0; l < b; l++) {
                                    for (int n = 0; n < 4; n++) {
                                        mver[m_count][0 + (7 * n)] += 0.060f;
                                    }
                                }

                                assign_colour(mver[m_count], m_random);

                                glGenVertexArrays(1, &mVAO[m_count]);
                                glGenBuffers(1, &mVBO[m_count]);
                                glGenBuffers(1, &mCBO[m_count]);
                                draw(&mVBO[m_count], &mVAO[m_count], &mCBO[m_count], VEBO, mver[m_count], sver, ind, sind);
                                m_count++;
                            }
                        }
                    }
                    m_count = 0;

                    ///Miscellaneous reset
                    time = 2.000f;
                    time_for_level = 0.750f;
                    score = 0;
                    lines = 0;
                    level = 1;
                    topped_out = false;

                }
                pause = false;
                system("cls");
                print_title();
                std::cout << "+--------------+" << std::endl;
                std::cout << "|  Your Score  |" << std::endl;
                std::cout << "+--------------+" << std::endl;
                std::cout << "Current Score: " << score << std::endl;
                std::cout << "Lines Cleared: " << lines << std::endl;
                std::cout << "Current Level: " << level << std::endl;
                std::cout << std::endl;
            }
            pressed_enter = true;
        }
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && pressed_enter == true) pressed_enter = false;

        glfwPollEvents();
        ///Delta Define End

        /// Render
        /// Clear the colorbuffer
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glClearColor(r_b, g_b, b_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        /// Draw playfield
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        for (int i = 0; i < 240; i++) {
            glBindVertexArray(bVAO[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        /// Draw box for next piece
        glBindVertexArray(nVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        for (int i = 0; i < 4; i++) {
            glBindVertexArray(mVAO[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        /// Draw pieces
        for (int i = 0; i < rep; i++) {
            glBindVertexArray(pVAO[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        /// Swap the screen buffers
        glfwSwapBuffers(window);
    }
    /// Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &CBO);
    glDeleteBuffers(1, &VEBO);

    for (int i = 0; i < 240; i++) {
        glDeleteVertexArrays(1, &bVAO[i]);
        glDeleteBuffers(1, &bVBO[i]);
        glDeleteBuffers(1, &bCBO[i]);
    }

    glDeleteVertexArrays(1, &nVAO);
    glDeleteBuffers(1, &nVBO);
    glDeleteBuffers(1, &nCBO);

    for (int i = 0; i < 4; i++) {
        glDeleteVertexArrays(1, &mVAO[i]);
        glDeleteBuffers(1, &mVBO[i]);
        glDeleteBuffers(1, &mCBO[i]);
    }

    for (int i = 0; i < rep; i++) {
        glDeleteVertexArrays(1, &pVAO[i]);
        glDeleteBuffers(1, &pVBO[i]);
        glDeleteBuffers(1, &pCBO[i]);
    }
    /// Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

/// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}