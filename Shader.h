#ifndef CG_LAB1_SHADER_H
#define CG_LAB1_SHADER_H
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>

using namespace std;

class Shader {
public:
    GLuint id;
    Shader (const char *vsName, const char *fsName) {
        ifstream vsFile;
        ifstream fsFile;
        string vertexShader;
        string fragmentShader;

        string line;

        vsFile.open(vsName);
        if (vsFile.is_open()) {
            while (getline(vsFile, line)) {
                vertexShader.append(line);
                vertexShader.push_back('\n');
            }
            vsFile.close();
        } else {
            fprintf(stderr, "Error while opening vertex shader file %s\n",vsName);
            exit(0);
        }

        fsFile.open(fsName);
        if (fsFile.is_open()) {
            while (getline(fsFile, line)) {
                fragmentShader.append(line);
                fragmentShader.push_back('\n');
            }
            fsFile.close();
        } else {
            fprintf(stderr, "Error while opening fragment shader file %s\n",fsName);
            exit(0);
        }

        id = CompileShaders(vertexShader.c_str(), fragmentShader.c_str());
    }

    void Use () {
        glUseProgram(id);
    }

    void setUniformVec3(const GLchar *uniformName, glm::vec3 uniformValue) {
        Use();
        glUniform3fv(glGetUniformLocation(this->id, uniformName),1,value_ptr(uniformValue));
    }
    void setUniformMat3(const GLchar *uniformName, glm::mat3 uniformValue) {
        Use();
        glUniformMatrix3fv(glGetUniformLocation(this->id, uniformName),1,GL_FALSE,value_ptr(uniformValue));
    }
    void setUniformMat4(const GLchar *uniformName, glm::mat4 uniformValue) {
        Use();
        glUniformMatrix4fv(glGetUniformLocation(this->id, uniformName),1,GL_FALSE,value_ptr(uniformValue));
    }
    void setUniformFloat(const GLchar *uniformName, float uniformValue) {
        Use();
        glUniform1f(glGetUniformLocation(this->id, uniformName), uniformValue);
    }
    void setUniformInt(const GLchar *uniformName, int uniformValue) {
        Use();
        glUniform1i(glGetUniformLocation(this->id, uniformName), uniformValue);
    }

private:
    static void AddShader(GLuint ShaderProgram, const char *pShaderText, GLenum ShaderType)
    {
        // create a shader object
        GLuint ShaderObj = glCreateShader(ShaderType);

        if (ShaderObj == 0)
        {
            fprintf(stderr, "Error creating shader type %d\n", ShaderType);
            exit(0);
        }
        // Bind the source code to the shader, this happens before compilation
        glShaderSource(ShaderObj, 1, &pShaderText, NULL);
        // compile the shader and check for errors
        glCompileShader(ShaderObj);
        GLint success;
        // check for shader related errors using glGetShaderiv
        glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar InfoLog[1024];
            glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
            fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
            exit(1);
        }
        // Attach the compiled shader object to the program object
        glAttachShader(ShaderProgram, ShaderObj);
    }

    GLuint CompileShaders(const char *vertShader, const char *fragShader)
    {
        //Start the process of setting up our shaders by creating a program ID
        //Note: we will link all the shaders together into this ID
        GLuint shaderProgramID = glCreateProgram();
        if (shaderProgramID == 0)
        {
            fprintf(stderr, "Error creating shader program\n");
            exit(1);
        }

        // Create two shader objects, one for the vertex, and one for the fragment shader
        AddShader(shaderProgramID, vertShader, GL_VERTEX_SHADER);
        AddShader(shaderProgramID, fragShader, GL_FRAGMENT_SHADER);

        GLint Success = 0;
        GLchar ErrorLog[1024] = {0};


        // After compiling all shader objects and attaching them to the program, we can finally link it
        glLinkProgram(shaderProgramID);
        // check for program related errors using glGetProgramiv
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
        if (Success == 0)
        {
            glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
            exit(1);
        }
//        setUniformInt("tex",0);
//        setUniformInt("normalMap",2);
//        setUniformInt("skymap",9);
        // program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
//        glValidateProgram(shaderProgramID);
//        // check for program related errors using glGetProgramiv
//        glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
//        if (!Success)
//        {
//            glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
//            fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
//            exit(1);
//        }
        // Finally, use the linked shader program
        // Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
        glUseProgram(shaderProgramID);
        return shaderProgramID;
    }
};

#endif //CG_LAB1_SHADER_H
