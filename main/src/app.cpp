#include <ngine/window/window.hpp>
#include <ngine/window/keycodes.hpp>
#include <ngine/renderer/renderer.hpp>
#include <ngine/tilemap/tilemap.hpp>
#include <ngine/renderer/image.hpp>
#include <ngine/renderer/ortho_camera.hpp>
#include <ngine/renderer/default_models.hpp>

int main()
{
	ng::Window window {1280, 720, "NGine"};
	ng::Renderer render {window};

	ng::OrthographicCamera camera {.dimensions {200.0f, 200.0f * 9.0f / 16.0f, 10.0f}};
	float cameraSpeed {150.0f};
	float zoomSpeed {3.0f};

	ng::Prefab quad {render.newPrefab(ng::quadVertices.data(), ng::quadVertices.size(),
									  ng::quadIndices.data(), ng::quadIndices.size())};

	ng::Tilemap map {"main/res/orthogonal-outside.tmx", render, 
					 ng::ImageShrinkFilter::PIXEL_NEAREST_NO_MIPMAP,
					 ng::ImageEnlargeFilter::PIXEL_NEAREST,
					 1, 10};

	while(window.userClosedWindow() == false)
	{
		window.pollEvents();
		float deltaTime = (float)window.deltaTime();

		if(window.keyPressed(NG_KEY_A))
		{
			camera.transform.position.x -= cameraSpeed * deltaTime;
		}
		else if(window.keyPressed(NG_KEY_D))
		{
			camera.transform.position.x += cameraSpeed * deltaTime;
		}

		if(window.keyPressed(NG_KEY_W))
		{
			camera.transform.position.y += cameraSpeed * deltaTime;
		}
		else if(window.keyPressed(NG_KEY_S))
		{
			camera.transform.position.y -= cameraSpeed * deltaTime;
		}

		if(window.keyPressed(NG_KEY_EQUAL))
		{
			camera.transform.scale -= glm::vec3 {zoomSpeed * deltaTime, zoomSpeed * deltaTime, 0.0f};
		}
		else if(window.keyPressed(NG_KEY_MINUS))
		{
			camera.transform.scale += glm::vec3 {zoomSpeed * deltaTime, zoomSpeed * deltaTime, 0.0f};
		}

		render.setCamera(camera.generateMatrix());
		render.clear(glm::vec4 {0.05f, 0.05f, 0.05f, 0.0f});

		map.draw(render, quad);

		window.swapBuffers();
	}

	return 0;
}