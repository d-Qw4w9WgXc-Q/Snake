#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "opengl.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include "shaders.h"
#include "sprite.h"

#define WIDTH 800
#define HEIGHT 800

#define GRIDSIZE 25

#define APPLECOLOR (float[3]){1.0, 0.0, 0.0}
#define SNAKECOLOR (float[3]){0.0, 1.0, 0.0}

#define FPS 60 // Timings depend on vsync so change this to your monitor's hz

typedef struct {
        union {
                struct {
                        GLint x;
                        GLint y;
                };
                GLint vec[2];
        };
} ivec2;

static void handle_input(GLFWwindow *window, ivec2 *out_velocity) {
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && out_velocity->x != 1) {
                out_velocity->x = -1;
                out_velocity->y = 0;
        }
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && out_velocity->y != -1) {
                out_velocity->x = 0;
                out_velocity->y = 1;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && out_velocity->x != -1) {
                out_velocity->x = 1;
                out_velocity->y = 0;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && out_velocity->y != 1) {
                out_velocity->x = 0;
                out_velocity->y = -1;
        }
}

static void applepos_new(ivec2 *applepos) {
        // Apple can appear on top of snake but whatever

        applepos->x = rand() % GRIDSIZE;
        applepos->y = rand() % GRIDSIZE;
}


static int draw_snake(sprite *snake, GLuint program, GLuint score, ivec2 *snakepos, size_t snakepos_i) {

        const size_t headpos_i = snakepos_i;

        for(size_t i = 0; i <= score; ++i) {

                if(i != 0 && snakepos[snakepos_i].x == snakepos[headpos_i].x && snakepos[snakepos_i].y == snakepos[headpos_i].y)
                        return 1;

                sprite_pos(snake, snakepos[snakepos_i].vec);
                sprite_draw(snake, program, GRIDSIZE);

                snakepos_i = (snakepos_i - 1 + (GRIDSIZE * GRIDSIZE)) % (GRIDSIZE * GRIDSIZE);
        }

        return 0;
}

int main() {
        srand(time(NULL));

        if(!glfwInit())
                return 1;

        GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Snake", NULL, NULL);
        if(!window) {
                glfwTerminate();
                return 1;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        glViewport(0, 0, WIDTH, HEIGHT);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.2863, 0.3451, 1.0);


        if(glewInit() != GLEW_OK) {
                glfwTerminate();
                return 1;
        }

        static const char vertex[] = {
                #embed "src/shaders/vertex.glsl"
                , 0
        };

        static const char fragment[] = {
                #embed "src/shaders/fragment.glsl"
                , 0
        };

        GLuint program = program_new(vertex, fragment);

        sprite snake;
        sprite_rect(&snake, 1, 1);
        sprite_color(&snake, SNAKECOLOR);

        ivec2 snakepos[GRIDSIZE * GRIDSIZE];
        size_t snakepos_i = 0;

        snakepos[0] = (ivec2){GRIDSIZE / 2, GRIDSIZE / 2};
        sprite_pos(&snake, snakepos[0].vec);

        GLuint score = 0;
        GLuint frame = 0;
        ivec2 velocity = {0, 0};

        sprite apple;
        sprite_rect(&apple, 1, 1);
        sprite_color(&apple, APPLECOLOR);

        ivec2 applepos;
        applepos_new(&applepos);
        sprite_pos(&apple, applepos.vec);


        while(!glfwWindowShouldClose(window)) {
                glClear(GL_COLOR_BUFFER_BIT);

                handle_input(window, &velocity);

                if(frame % 10 == 0) {
                        ivec2 newpos = snakepos[snakepos_i];

                        newpos.x += velocity.x;
                        newpos.y += velocity.y;

                        if(newpos.x < 0 || newpos.x >= GRIDSIZE || newpos.y < 0 || newpos.y >= GRIDSIZE)
                                exit(0);

                        snakepos_i = (snakepos_i + 1) % (GRIDSIZE * GRIDSIZE);
                        snakepos[snakepos_i] = newpos;
                }

                if(snakepos[snakepos_i].x == applepos.x && snakepos[snakepos_i].y == applepos.y) {
                        score += 1;

                        printf("Score: %u\n", score);

                        applepos_new(&applepos);
                        sprite_pos(&apple, applepos.vec);
                }


                if(draw_snake(&snake, program, score, snakepos, snakepos_i) != 0)
                        exit(0);

                sprite_draw(&apple, program, GRIDSIZE);

                glfwSwapBuffers(window);
                glfwPollEvents();
                ++frame;
        }
}
