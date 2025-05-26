#include <ngine/window/window.hpp>
#include <ngine/renderer/renderer.hpp>
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

    ng::Transform transform {.scale {100.0f, 100.0f, 1.0f}};

    ng::OrthographicCamera camera {.dimensions {1280.0f, 720.0f, 100.0f}};

    while(window.userClosedWindow() == false)
    {
        window.pollEvents();

        if(window.keyPressed(NG_KEY_A))
        {
            camera.transform.scale += 0.001f;
        }

        if(window.keyPressed(NG_KEY_D))
        {
            camera.transform.scale -= 0.001f;
        }

        transform.position += glm::vec3 {window.deltaMouseX() * camera.dimensions.x / 2.0f / camera.transform.scale.x, 
                                         window.deltaMouseY() * camera.dimensions.y / 2.0f / camera.transform.scale.y, 
                                         0.0f};
        ng::Transform transform2 {transform};
        transform2.position += glm::vec3 {105.0f, 0.0f, 0.0f};

        std::array<ng::ModelData, 2> modelDatas
        {
            ng::ModelData {transform.generateMatrix(), animation.getNextFrame((float)window.deltaTime())},
            ng::ModelData {transform2.generateMatrix(), animation.getNextFrame((float)window.deltaTime())},
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