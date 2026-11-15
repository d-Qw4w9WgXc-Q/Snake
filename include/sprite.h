#pragma once
#include "opengl.h"
#include <stddef.h>

struct vertex {
        GLuint pos[2];
};

typedef struct {
        GLuint vao;
        union {
                struct {
                        GLuint vbo;
                        GLuint ebo;
                };
                GLuint buffers[2];
        };

        size_t indices_len;
        float color[3];
        GLint pos[2];
} sprite;

void sprite_new(sprite *out_sprite, struct vertex *vertices, size_t vertices_size, GLuint *indices, size_t indices_size);
void sprite_draw(sprite *sprite, GLuint program, GLuint grid_size);
void sprite_rect(sprite *out_sprite, unsigned int width, unsigned int height);
void sprite_color(sprite *sprite, float color[3]);
void sprite_pos(sprite *sprite, GLint pos[2]);
