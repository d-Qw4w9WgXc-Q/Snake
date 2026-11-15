#include "sprite.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <string.h>

void sprite_new(sprite *out_sprite, struct vertex *vertices, size_t vertices_size, GLuint *indices, size_t indices_size) {
        glCreateVertexArrays(1, &out_sprite->vao);
        glBindVertexArray(out_sprite->vao);

        glCreateBuffers(2, out_sprite->buffers);
        glBindBuffer(GL_ARRAY_BUFFER, out_sprite->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out_sprite->ebo);

        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof(struct vertex), (void*)0);
        glEnableVertexAttribArray(0);

        out_sprite->indices_len = indices_size / sizeof(GLuint);

        glBindVertexArray(0);
}

void sprite_rect(sprite *out_sprite, unsigned int width, unsigned int height) {
        static GLuint indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        struct vertex vertices[] = {
                {0, 0},
                {width, 0},
                {width, height},
                {0, height}
        };

        return sprite_new(out_sprite, vertices, sizeof(vertices), indices, sizeof(indices));
}

void sprite_draw(sprite *sprite, GLuint program, GLuint grid_size) {

        glUniform1f(glGetUniformLocation(program, "u_scale"), 2.0 / grid_size);
        glUniform3f(glGetUniformLocation(program, "u_color"), sprite->color[0], sprite->color[1], sprite->color[2]);
        glUniform2i(glGetUniformLocation(program, "u_offset"), sprite->pos[0], sprite->pos[1]);

        glUseProgram(program);

        glBindVertexArray(sprite->vao);
        glDrawElements(GL_TRIANGLES, sprite->indices_len, GL_UNSIGNED_INT, NULL);
}

void sprite_color(sprite *sprite, float color[3]) {
        memcpy(sprite->color, color, sizeof(float) * 3);
}

void sprite_pos(sprite *sprite, GLint pos[2]) {
        memcpy(sprite->pos, pos, sizeof(GLuint) * 2);
}
