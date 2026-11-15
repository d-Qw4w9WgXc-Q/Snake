#include "shaders.h"
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>

static GLuint compile_shader(const char *str, GLenum type) {
        GLuint shader = glCreateShader(type);

        glShaderSource(shader, 1, &str, NULL);
        glCompileShader(shader);

        GLint success;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
                GLint len;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

                char *error = malloc(len);

                glGetShaderInfoLog(shader, len, NULL, error);

                fputs(error, stderr);

                free(error);
                return 0;
        }

        return shader;
}

GLuint program_new(const char *v_src, const char *f_src) {
        GLuint program = glCreateProgram();

        GLuint v = compile_shader(v_src, GL_VERTEX_SHADER);
        if(v == 0) return 0;
        GLuint f = compile_shader(f_src, GL_FRAGMENT_SHADER);
        if(f == 0) return 0;

        glAttachShader(program, v);
        glAttachShader(program, f);

        glLinkProgram(program);

        GLint success;

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success) {
                GLint len;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

                char *error = malloc(len);

                glGetProgramInfoLog(program, len, NULL, error);

                fputs(error, stderr);

                free(error);
                return 0;
        }

        return program;
}
