#include <ngine/window/window.hpp>
#include <ngine/renderer/renderer.hpp>
#include <ngine/renderer/glm_clip_control.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ngine/renderer/ortho_camera.hpp>
#include <ngine/window/keycodes.hpp>
#include <ngine/renderer/animation.hpp>
#include <ngine/renderer/default_models.hpp>

#include <array>

int main()
{
    ng::Window window {1280, 720, "OpenGL Platformer"};
    ng::Renderer render {window};

    window.setResizeCallback([&render, &window] (int width, int height)
    {
        render.resetViewport(window);
    });

    ng::Prefab prefab {render.newPrefab(ng::quadVertices.data(), ng::quadVertices.size(), 
                                        ng::quadIndices.data(), ng::quadIndices.size())};

    ng::Material material
    {
        .image {"main/res/awesomeface.png",
                ng::ImageShrinkFilter::PIXEL_LINEAR_NO_MIPMAP,
                ng::ImageEnlargeFilter::PIXEL_LINEAR,
                ng::ImageWrap::REPEAT,
                1},
        .color {1.0f}
    };

    ng::Animation animation {4};
    animation.addFrame(ng::ImageSampleData {glm::uvec2 {0}, material.image.width() / 2, material.image.height() / 2}, 0.25f);
    animation.addFrame(ng::ImageSampleData {glm::uvec2 {material.image.width() / 2, 0}, material.image.width() / 2, material.image.height() / 2}, 0.25f);
    animation.addFrame(ng::ImageSampleData {glm::uvec2 {material.image.width() / 2, material.image.height() / 2}, material.image.width() / 2, material.image.height() / 2}, 0.25f);
    animation.addFrame(ng::ImageSampleData {glm::uvec2 {0, material.image.height() / 2}, material.image.width() / 2, material.image.height() / 2}, 0.25f);
    animation.setLoop(false);
    animation.setOnEnd([] () { printf("LMAO ANIMATION ENDED LOL\n"); });

    glm::mat4 transform {1.0f};
    transform = glm::scale(transform, glm::vec3 {50.0f, 50.0f, 1.0f});

    ng::OrthographicCamera camera
    {
        .width {1280.0f}, .height {720.0f},
        .depth {100.0f},
        .center {0.0f},
        .rotation {0.0f},
        .zoom {1.0f}
    };

    while(window.userClosedWindow() == false)
    {
        window.pollEvents();

        if(window.keyPressed(NG_KEY_A))
        {
            animation.start(0);
        }

        glm::vec3 mouseMove {window.deltaMouseX() * camera.width / 2 / camera.zoom, 
                             window.deltaMouseY() * camera.height / 2 / camera.zoom, 
                             0.0f};
        transform = glm::translate(transform, mouseMove);

        std::array<ng::ModelData, 4> modelDatas
        {
            ng::ModelData {transform, animation.getFrame((float)window.deltaTime())},
            ng::ModelData {glm::translate(transform, glm::vec3 {105.0f, 0.0f, 0.0f}), glm::uvec2{0},
                           material.image.width(), material.image.height()},
            ng::ModelData {glm::translate(transform, glm::vec3 {0.0f, 105.0f, 0.0f}), glm::uvec2{0},
                           material.image.width(), material.image.height()},
            ng::ModelData {glm::translate(transform, glm::vec3 {105.0f, 105.0f, 0.0f}), glm::uvec2{0},
                           material.image.width(), material.image.height()}
        };

        material.color = glm::vec4 {abs(window.mouseX()), abs(window.mouseY()), 0.0f, 1.0f};
        material.color.b = (material.color.r + material.color.g) / 2.0f;

        render.setCamera(camera.generateMatrix());
        render.clear(glm::vec4 {0.05f, 0.05f, 0.05f, 0.0f});
        
        render.draw(prefab, modelDatas.data(), modelDatas.size(), material);
        
        window.swapBuffers();
    }

    return 0;
}