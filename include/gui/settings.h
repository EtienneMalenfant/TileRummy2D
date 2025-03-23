#pragma once
#include <string>

namespace gui {
    
    class IWindowSettings {
    public:
        virtual int getBaseWindowWidth() const = 0;
        virtual int getBaseWindowHeight() const = 0;
        virtual int getXRes() const = 0;
        virtual int getYRes() const = 0;
        virtual bool isResizable() const = 0;
        virtual bool isFullscreen() const = 0;
        virtual std::string getTitle() const = 0;
    };

    class WindowSettings : public IWindowSettings {
    private:
        const int baseWindowWidth;
        const int baseWindowHeight;
        const bool resizable;
        const bool fullscreen;
        const std::string title;

        const int xRes = 1920;
        const int yRes = 1080;
    public:
        WindowSettings(int windowWidth, int windowHeight, bool resizable, bool fullscreen, const std::string& title)
         :  baseWindowWidth(windowWidth), baseWindowHeight(windowHeight), 
            resizable(resizable), fullscreen(fullscreen), title(title) {};
        int getBaseWindowWidth() const override { return baseWindowWidth; }
        int getBaseWindowHeight() const override { return baseWindowHeight; }
        int getXRes() const override { return xRes; }
        int getYRes() const override { return yRes; }
        bool isResizable() const override { return resizable; }
        bool isFullscreen() const override { return fullscreen; }
        std::string getTitle() const override { return title; }
    };

}