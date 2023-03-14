#pragma once
#include "Application.h"

int main() {
    Application* app = new Application();
    if (app->Startup()) {

        while (app->applicationIsActive) {
            app->Update();

            if (!app->applicationIsActive) {
                break;
            }

            app->Draw();
        }

        app->Shutdown();
    }

    delete app;
	return 0;
}