#include <stdio.h>
#include "render.h"
#include <opencv2/opencv.hpp>

int main()
{

    render *r = new render("Hello World", 800, 600, true);
    input *inputManager = r->inputHandler;

    if (inputManager == nullptr)
    {
        printf("Error: Input manager is null.\n");
        return -1;
    }
    printf("Input manager initialized successfully.\n");

    cv::VideoCapture cap(1, cv::CAP_DSHOW); // Open the default camera (0) or use 2 for an external camera

    // std::cout << cv::getBuildInformation() << std::endl;

    if (!cap.isOpened())
    {
        printf("Error: Could not open camera.\n");
        return -1;
    }

    printf("Camera opened successfully.\n");
    cv::Mat frame;

    // Create OpenGL texture
    unsigned int webcamTexture = r->createTexture();

    while (!r->WindowShouldClose())
    {
        r->beginRender();

        // 1. Capture a frame from the webcam
        if (!cap.read(frame))
        {
            printf("Error: Could not read frame from camera.\n");
            r->endRender();
            continue;
        }

        if (frame.empty())
        {
            printf("Error: Empty frame captured from camera.\n");
            r->endRender();
            continue;
        }

        // 2. Convert to RGB (OpenCV default is BGR)
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        // 3. Update the OpenGL texture
        r->UpdateTexture(webcamTexture, frame.data, frame.cols, frame.rows);
        // 4. Draw the texture on the screen
        r->drawQuad(0.0f, 0.0f, 1.0f, 1.0f, webcamTexture);

        // 5. Draw a circle at the mouse position
        float xpos, ypos;
        inputManager->getMousePosition(xpos, ypos);
        r->drawCircle(xpos, ypos, 0.02f, 1.0f, 1.0f, 0.0f, 1.0f);

        r->endRender();
    }

    delete r;
    return 0;
}
